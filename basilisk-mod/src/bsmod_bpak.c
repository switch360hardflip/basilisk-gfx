#include <bsmod_bpak.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/prefab/bsgfx_prefab.h>
#include <types/tile/bsgfx_tile.h>
#include <types/light/bsgfx_light.h>
#include <bs_log.h>
#include <bs_math.h>
#include <stdarg.h>>
#include <string.h>>
#include <assert.h>>
#include <bsmod.h>

static bs_List _bsmod_packages = { .unit_size = sizeof(bsmod_Package), .increment = 4 };

bs_List* bsmod_packages() {
	return &_bsmod_packages;
}

bsmod_Package* bsmod_queryPackage(const char* name) {
	bs_U64 hash = bs_stringHash(name);

	for (int i = 0; i < _bsmod_packages.count; i++) {
		bsmod_Package* package = bs_fetchUnit(&_bsmod_packages, i);

		if (hash == package->name_hash)
			return package;
	}

	return NULL;
}

bsmod_Package* bsmod_ensurePackage(const char* name) {
	bsmod_Package* existing = bsmod_queryPackage(name);
	if (existing)
		return existing;

	bs_infoF("Created package %s\n", name);
	return bs_pushBack(&_bsmod_packages, &(bsmod_Package) {
		.name = name,
		.name_hash = bs_stringHash(name),
		.chunks = bs_list(sizeof(bsmod_Chunk), 16),
		.resources = bs_list(sizeof(bsmod_Resource), 32),
	});
}

static bsmod_Resource* bsmod_ensureResource(bsmod_Package* package, char* name) {
	bs_U64 hash = bs_stringHash(name);

	for (int i = 0; i < package->resources.count; i++) {
		bsmod_Resource* resource = bs_fetchUnit(&package->resources, i);
		if (hash == resource->header.name_hash)
			return resource;
	}

	return bs_pushBack(&package->resources, &(bsmod_Resource) {
		.header = {
			.name_hash = hash,
			.name_length = strlen(name),
		},
		.name = strdup(name),
	});
}

void bsmod_iniPackage(const char* package_name) {
	int previous_count = _bsmod_packages.count;
	bsmod_Package* package = bsmod_ensurePackage(package_name);
	if (previous_count == _bsmod_packages.count)
		return bs_warnF("Package \"%s\" is already initialized\n", package_name);

	bs_except(BSX_NOT_FOUND);
	bs_String* bpak = bs_loadFileF("%s.bpak", package_name);
	bs_except(0);
	if (!bpak)
		return;

	int chunks_count = 0;

	for (int i = 0; i < (bpak->len - 1);) {
		bs_ResourceHeader* header = bpak->value + i;
		i += sizeof(header->header);
		if (i >= bpak->len)
			return bs_throwBasilisk(BSX_CORRUPTED);

		char* name = bpak->value + i;
		i += header->header.name_length + 1;
		if (i >= bpak->len)
			return bs_throwBasilisk(BSX_CORRUPTED);

		char* end = strchr(name, '\n');
		if (!end)
			return bs_throwBasilisk(BSX_CORRUPTED);
		end[0] = '\0';

		bsmod_Resource* added = bs_pushBack(&package->resources, &(bsmod_Resource) {
			.header = header->header,
			.name = strdup(name),
		});

		chunks_count = BS_MAX(chunks_count, (header->header.chunk + 1));
	}

	for (int i = 0; i < chunks_count; i++) {
		bs_String* chunk_bin = bs_loadFileF("%s_%03d.bpak", package_name, (i + 1));

		bsmod_Chunk* chunk = bs_pushBack(&package->chunks, &(bsmod_Chunk) {
			.bin = bs_list(sizeof(unsigned char), 10000),
			.id = i,
		});

		bs_ensureSize(&chunk->bin, chunk_bin->len);
		memcpy(chunk->bin.data, chunk_bin->value, chunk_bin->len);
		chunk->bin.count = chunk_bin->len;

		bs_free(chunk_bin);
	}

	bs_free(bpak);
}

static bsmod_Chunk* bsmod_ensureChunk(bsmod_Package* package, int size) {
	bsmod_Chunk* chunk;
	for (int i = 0; i < package->chunks.count; i++) {
		chunk = bs_fetchUnit(&package->chunks, i);
		if (chunk->bin.count == 0 || (chunk->bin.count + size) < BSMOD_BPAK_CHUNK_SIZE) {
			bs_ensureSize(&chunk->bin, size);
			return chunk;
		}
	}

	bs_infoF("Created chunk\n");
	chunk = bs_pushBack(&package->chunks, &(bsmod_Chunk) {
		.bin = bs_list(sizeof(unsigned char), 10000),
		.id = package->chunks.count,
	});
	bs_ensureSize(&chunk->bin, size);
	return chunk;
}

void bsmod_packResource(bs_ResourceType type, unsigned char* data, size_t data_size, const char* package_name, const char* resource_name) {
	bsmod_Package* package = bsmod_ensurePackage(package_name);

	bsmod_Resource* resource = bsmod_ensureResource(package, resource_name);
	resource->type = type;

	bs_except(BSX_UNINITIALIZED);
	bsmod_Chunk* chunk = bs_fetchUnit(&package->chunks, resource->header.chunk);
	bs_except(0);
	
	if (resource->header.size != data_size) {
		if (resource->header.size > 0) {
			bs_infoF("Deleting old resource %s " BS_PRINT_COLOR("(-%d bytes)", BS_PRINT_RED) "\n", resource_name, resource->header.size);
			
			size_t remaining = chunk->bin.count - resource->header.offset;
			assert(remaining >= resource->header.size);

			memmove(
				chunk->bin.data + resource->header.offset,
				chunk->bin.data + resource->header.offset + resource->header.size,
				chunk->bin.count - (resource->header.offset + resource->header.size));
			chunk->bin.count -= resource->header.size;
			chunk->has_changes = true;

			for (int i = 0; i < package->resources.count; i++) {
				bsmod_Resource* r = bs_fetchUnit(&package->resources, i);
				if (r != resource && r->header.chunk == resource->header.chunk && r->header.offset > resource->header.offset) {
					bs_infoF("  Adjusted offset for resource \"%s\"\n", r->name);
					r->header.offset -= resource->header.size;
					assert(r->header.offset >= 0);
				}
			}
		}

		chunk = bsmod_ensureChunk(package, data_size);
		resource->header.chunk = chunk->id;
		resource->header.offset = chunk->bin.count;
		resource->header.size = data_size;
		chunk->bin.count += data_size;
		bs_infoF("Created resource %s " BS_PRINT_COLOR("(+%d bytes)", BS_PRINT_GREEN) "\n", resource_name, data_size);
	} 
	else
		bs_infoF("Packed resource %s " BS_PRINT_COLOR("(+-0 bytes)", BS_PRINT_GRAY) "\n", resource_name);

	memcpy(chunk->bin.data + resource->header.offset * chunk->bin.unit_size, data, data_size);

	chunk->has_changes = true;
	package->has_changes = true;
	resource->has_changes = true;
}

void bsmod_packResourceV(bs_ResourceType type, unsigned char* data, size_t data_size, const char* package_name, const char* format, va_list args) {
	static bs_String* formatted;
	formatted = bs_stringV(formatted, format, args);
	bsmod_packResource(type, data, data_size, package_name, formatted->value);
}

void bsmod_packResourceF(bs_ResourceType type, unsigned char* data, size_t data_size, const char* package_name, const char* format, ...) {
	va_list args;
	va_start(args, format);
	bsmod_packResourceV(type, data, data_size, package_name, format, args);
	va_end(args);
}

static void bsmod_loadResource(bs_ResourceType type, int package_id, char* name) {
	if (!bs_instance()->device)
		return;
	bs_Scope scope = bs_enterSingle();

	bs_except(BSX_FAILED_TO_QUERY);
	bs_Resource* resource = bs_queryResource(package_id, name);
	bs_except(0);
	bs_List* sources = bs_objectSources();

	switch (type) {
	case BSGFX_RESOURCE_PREFAB:
		bsgfx_loadPrefabs(bsgfx_getType(BSGFX_TYPE_PREFAB)->package_id, bsgfx_prefabModel());
		break;
	case BSGFX_RESOURCE_PRIMITIVE:
		bsgfx_loadPrimitives(bsgfx_getType(BSGFX_TYPE_PRIMITIVE)->package_id);
	case BSGFX_RESOURCE_TILE:
		bsgfx_loadTiles(bsgfx_getType(BSGFX_TYPE_TILE)->package_id, true);
		break;
	case BSGFX_RESOURCE_LIGHT:
	//	bsgfx_loadLights(bsgfx_getType(BSGFX_TYPE_LIGHT)->package_id);
		break;
	case BS_RESOURCE_IMAGE:

		bs_Image* existing_image = NULL;
		if (resource && resource->image) {
			for (int i = 0; i < sources->count; i++ ){
				bs_ObjectSource* source = bs_fetchUnit(sources, i);
				if (source->type == BS_OBJECT_IMAGE) {
					for (int j = 0; j < source->ids_count; j++) {
						bs_Object* image = source->ids[j].object;
						if (resource->image == image->image) {
							existing_image = image->image;
							goto image_end;
						}
					}
				}
			}
		image_end:
			void;
		}

		if (existing_image) {
			bs_Image* image = bs_loadImage(BS_IMAGE(existing_image->head.source_id, existing_image->head.id, BS_OBJECT_FORCE_DESTROY), package_id, name, existing_image->flags)->image;

			for (int i = 0; i < bs_instance()->bind_sets_count; i++) {
				bs_BindSet* bind_set = bs_instance()->bind_sets + i;
				for (int j = 0; j < bind_set->bindings_count; j++) {
					bs_Binding* binding = bind_set->bindings + j;
					bs_Descriptor* descriptor = ((unsigned char*)bind_set->descriptors) + binding->location;

					bs_ImageDescriptor* new_descriptors = NULL;
					int new_descriptors_count = 0;
					for (int k = 0; k < binding->descriptors_count; k++, descriptor++) {
						if (descriptor->object_type != BS_OBJECT_IMAGE)
							continue;

						if (descriptor->as_image.image == existing_image)
							new_descriptors = _alloca(binding->descriptors_count * sizeof(bs_ImageDescriptor));
					}

					if (new_descriptors) {
						descriptor = ((unsigned char*)bind_set->descriptors) + binding->location;
						for (int k = 0; k < binding->descriptors_count; k++, descriptor++) {
							assert(descriptor->object_type == BS_OBJECT_IMAGE);

							new_descriptors[k] = (bs_ImageDescriptor){
								.image = descriptor->as_image.image,
								.layout = descriptor->as_image.vk_image_layout,
								.sampler = descriptor->as_image.sampler,
							};

							if (descriptor->as_image.image == existing_image)
								new_descriptors[k].image = image;
						}

						bs_bindImages(binding->set, binding->slot, new_descriptors, binding->descriptors_count);
					}
				}
			}
		}
		else {
			bs_warnF("Could not reload resource \"%s\", object id could not be found\n", name);
		}


		break;
	case BS_RESOURCE_ATLAS:

		bs_Atlas* existing_atlas = NULL;
		if (resource && resource->atlas) {
			for (int i = 0; i < sources->count; i++ ){
				bs_ObjectSource* source = bs_fetchUnit(sources, i);
				if (source->type == BS_OBJECT_IMAGE) {
					for (int j = 0; j < source->ids_count; j++) {
						bs_Object* o = source->ids[j].object;
						if (o && resource->atlas == o->atlas) {
							existing_atlas = o->atlas;
							goto atlas_end;
						}
					}
				}
			}
		atlas_end:
			void;
		}

		if (existing_atlas) {
			bs_Atlas* atlas = bs_loadAtlas(BS_ATLAS(existing_atlas->head.source_id, existing_atlas->head.id, BS_OBJECT_FORCE_DESTROY), package_id, name, 0)->atlas;
			bsmod_bindAtlases();
		}
		else {
			bs_warnF("Could not reload resource \"%s\", object id could not be found\n", name);
		}
		
		break;
	case BS_RESOURCE_SHADER:
		bs_except(BSX_FAILED_TO_QUERY);
		bs_queryResource(package_id, name);
		if (!bs_except(0))
			bs_shader(package_id, name, 0, NULL);

		break;
	case BS_RESOURCE_MODEL:
		bs_except(BSX_FAILED_TO_QUERY);
		bs_Resource* existing = bs_queryResource(package_id, name);
		if (!bs_except(0)) {
			if (existing->model == bsgfx_prefabModel()) {
				bs_Resource* resource = bs_model(package_id, name, 0);
				bs_Batch* batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED)->batch;
				bs_unpushBatch(batch);
				bsgfx_loadPrefabs(bsgfx_getType(BSGFX_TYPE_PREFAB)->package_id, resource->model);
			}
		}

	break;
	//case BS_RESOURCE_TEXTURE:
	//	bs_except(BSX_FAILED_TO_QUERY);
	//	bs_queryResource(package_id, name);
	//	if (!bs_except(0))
	//		bsgfx_textureArray(0, package_id, name);
	//
	//	//bsgfx_loadTextureDimension(package_id, name, 64, BSGFX_IMAGE_64, BSGFX_SET_64, BSGFX_BINDING_64);
	//	break;
	}
	bs_leaveSingle(&scope);
}

void bsmod_savePackage(const char* name) {
	bsmod_Package* package = bsmod_ensurePackage(name);
	if (!package->has_changes)
		return;
	package->has_changes = false;

	/**
	 Binary
	 */
	for (int i = 0; i < package->chunks.count; i++) {
		bsmod_Chunk* chunk = bs_fetchUnit(&package->chunks, i);
		if (!chunk->has_changes)
			continue;

		bs_saveFileF(chunk->bin.data, chunk->bin.count, "%s_%03d.bpak", name, (i + 1));
		chunk->has_changes = false;
	}

	/**
	 Headers
	 */
	int name_lengths = 0;
	for (int i = 0; i < package->resources.count; i++) {
		bsmod_Resource* resource = bs_fetchUnit(&package->resources, i);
		name_lengths += strlen(resource->name);
	}

	bsmod_Resource* t;
	const size_t header_size = sizeof(t->header);

	size_t size = package->resources.count * (header_size + 1) + name_lengths;
	unsigned char* data = bs_malloc(size);

	size_t offset = 0;
	for (int i = 0; i < package->resources.count; i++) {
		bsmod_Resource* resource = bs_fetchUnit(&package->resources, i);

		memcpy(data + offset, &resource->header, header_size);
		offset += header_size;
		memcpy(data + offset, resource->name, resource->header.name_length);
		offset += resource->header.name_length;
		memcpy(data + offset, "\n", 1);
		offset++;
	}

	bs_saveFileF(data, size, "%s.bpak", name);

	bs_free(data);

	/**
	 Reload resources
	 */
	int package_id = bs_loadPackage(name);

	for (int i = 0; i < package->resources.count; i++) {
		bsmod_Resource* resource = bs_fetchUnit(&package->resources, i);
		if (resource->has_changes) {
			bsmod_loadResource(resource->type, package_id, resource->name);
			resource->has_changes = false;
		}
	}
}