#include <types/bsgfx_type.h>
#include <bsmod_type.h>
#include <bsmod_bpak.h>
#include <bsgfx_scene.h>
#include <bsmod.h>
#include "../bsgfx_contracts.h"
#include <bsgfx.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/prefab/bsgfx_prefab.h>
#include <types/tile/bsgfx_tile.h>

bsgfx_TypeId bsmod_queryType(const char* plural) {
	for (int i = 0; i < BSGFX_TYPE_COUNT; i++) {
		if (strcmp(bsgfx_getType(i)->plural, plural) == 0)
			return i;
	}

	bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Type (%s)", plural);

	return BSGFX_TYPE_UNDEFINED;
}



/*=============================================================================
 * Type Editor
 *============================================================================*/

void bsmod_delete(bsgfx_TypeId type_id, int id) {
	//bsgfx_saveHistory(type_id);

	bsgfx_Type* type = bsgfx_getType(type_id);
	for (int i = id; i < type->count - 1; i++) {
		void* instance = bsgfx_get(type_id, i);
		void* raw_instance = bsgfx_getRaw(type_id, i);

		void* next_instance = bsgfx_get(type_id, i + 1);
		void* next_raw_instance = bsgfx_getRaw(type_id, i + 1);

		memcpy(instance, next_instance, type->mapped_unit_size);
		memcpy(raw_instance, next_raw_instance, type->unmapped_unit_size);
	}
	type->count--;
	bsmod_deselectAll();

	bsmod_saveType(type_id, "Deleted %s", type->singular);
}

void bsmod_deleteSelected(bsgfx_TypeId type_id) {
	// bsgfx_saveHistory(type_id);

	bsgfx_Type* type = bsgfx_getType(type_id);
	int count = bsmod.selected_ids.count;
	int* ids = bsmod.selected_ids.data;

	for (int i = 0; i < count - 1; i++) {
		for (int j = i + 1; j < count; j++) {
			if (ids[i] < ids[j]) {
				int temp = ids[i];
				ids[i] = ids[j];
				ids[j] = temp;
			}
		}
	}

	for (int i = 0; i < count; i++) {
		for (int j = ids[i]; j < type->count - 1; j++) {
			void* instance = bsgfx_get(type_id, j);
			void* raw_instance = bsgfx_getRaw(type_id, j);

			void* next_instance = bsgfx_get(type_id, j + 1);
			void* next_raw_instance = bsgfx_getRaw(type_id, j + 1);

			memcpy(instance, next_instance, type->mapped_unit_size);
			memcpy(raw_instance, next_raw_instance, type->unmapped_unit_size);
		}
		type->count--;
	}

	bsmod_deselectAll();

	if (count == 1)
		bsmod_saveType(type_id, "Deleted %s", type->singular);
	else
		bsmod_saveType(type_id, "Deleted %d %s", count, type->plural);
}

/*
static int bs_comparePrefabZ(const bsgfx_RawPrefab* a, const bsgfx_RawPrefab* b) {
	if (a->position.z == b->position.z) return 0;
	else if (a->position.z < b->position.z) return -1;
	else return 1;
}

static int bs_compareAccessorZ(const int* a_id, const int* b_id) {
	bsgfx_Type* type = bsgfx_types + BSGFX_TYPE_PREFAB;
	bsgfx_RawPrefab* a = type->unmapped + *a_id;
	bsgfx_RawPrefab* b = type->unmapped + *b_id;

	return bs_comparePrefabZ(a, b);
}
*/

void bsmod_saveType(bsgfx_TypeId id, const char* comment_format, ...) {
	bsgfx_Type* type = bsgfx_getType(id);

	// bsmod.history = BS_I64_MAX; // ?

	bsgfx_TypeHeader* data;
	size_t size = sizeof(*data) + type->count * sizeof(int) + type->count * type->unmapped_unit_size + type->flexible_count * type->unmapped_flexible_size;
	data = bs_malloc(size);

	if (size == 0) return;

	memset(data, 0, sizeof(*data));
	data->count = type->count;
	data->magic = BSGFX_TYPE_MAGIC;
	data->version = type->version;
	data->flexible_count = type->flexible_count;

	memcpy(data->accessors, type->unmapped_accessors, type->count * sizeof(int));
	memcpy(data->accessors + type->count, type->unmapped, type->count * type->unmapped_unit_size + type->flexible_count * type->unmapped_flexible_size);

	bsgfx_ResourceType resource_type = 0;
	switch (id) {
	case BSGFX_TYPE_PRIMITIVE: resource_type = BSGFX_RESOURCE_PRIMITIVE; break;
	case BSGFX_TYPE_PREFAB: resource_type = BSGFX_RESOURCE_PREFAB; break;
	case BSGFX_TYPE_TILE: resource_type = BSGFX_RESOURCE_TILE; break;
	case BSGFX_TYPE_LIGHT: resource_type = BSGFX_RESOURCE_LIGHT; break;
	default:
		bs_throwBasilisk(BSX_NOT_IMPLEMENTED);
	}

	bsgfx_remap(id); // debugging purposes

	// TODO: get application package name
	bsmod_packResourceF(resource_type, data, size, bsmod_applicationContentPath(), "levels/%s_%s", bsgfx_currentScene()->name, type->plural);

	// bs_saveFileF(data, size, "resources/levels/%s/%s.bin", bsgfx_fetchLevel(poser()->current_level)->name, type->plural);
	bs_free(data);

	if (comment_format) {
		va_list args;
		va_start(args, comment_format);
		bs_logWithTimestampV(BS_INFO_HEADER, sizeof(BS_INFO_HEADER) - 1, comment_format, args);
		va_end(args);
	}
}

// remove if data is NULL, otherwise insert
static void* bsmod_shiftFlexible(bsgfx_TypeId type_id, int id, void* data) {
	bsgfx_Type* type = bsgfx_getType(type_id);

	int unmapped_offset = 0, mapped_offset = 0;

	if (data && type->flexible_count >= type->flexible_capacity) {
		type->flexible_capacity += 128;

		int unmapped_capacity = type->capacity * type->unmapped_unit_size + type->flexible_capacity * type->unmapped_flexible_size;
		int mapped_capacity = type->capacity * type->mapped_unit_size + type->flexible_capacity * type->mapped_flexible_size;

		type->unmapped = bs_realloc(type->unmapped, unmapped_capacity);
		type->mapped = bs_realloc(type->mapped, mapped_capacity);
	}

	int dir = data ? 1 : -1;
	void* result = NULL;

	for (int i = 0; i <= id; i++) {
		int* unmapped_flexible_count = type->unmapped + unmapped_offset + type->unmapped_flexible_offset;
		int* mapped_flexible_count = type->mapped + mapped_offset + type->mapped_flexible_offset;

		unmapped_offset += type->unmapped_unit_size + *unmapped_flexible_count * type->unmapped_flexible_size;
		mapped_offset += type->mapped_unit_size + *mapped_flexible_count * type->mapped_flexible_size;

		if (i == id) {
			int unmapped_tail_size = type->count * type->unmapped_unit_size + type->flexible_count * type->unmapped_flexible_size - unmapped_offset;
			int mapped_tail_size = type->count * type->mapped_unit_size + type->flexible_count * type->mapped_flexible_size - mapped_offset;

			memmove(
				(char*)type->unmapped + unmapped_offset + type->unmapped_flexible_size,
				(char*)type->unmapped + unmapped_offset,
				unmapped_tail_size);

			memmove(
				(char*)type->mapped + mapped_offset + type->mapped_flexible_size,
				(char*)type->mapped + mapped_offset,
				mapped_tail_size);

			if (data)
				result = memcpy(
					(char*)type->unmapped + unmapped_offset,
					data,
					type->unmapped_flexible_size);

			(*unmapped_flexible_count) += dir;
			(*mapped_flexible_count) += dir;
		}
	}

	type->flexible_count += dir;

	for (int i = id + 1; i < type->count; i++) {
		type->unmapped_accessors[id] += type->unmapped_flexible_size * dir;
		type->mapped_accessors[id] += type->mapped_flexible_size * dir;
	}
	return result;
}

void* bsmod_addFlexible(bsgfx_TypeId type_id, int id, void* data) {
	bsgfx_Type* type = bsgfx_getType(type_id);

	void* result = bsmod_shiftFlexible(type_id, id, data);

	bsgfx_map(type_id, id);

	return result;
}

void* bsmod_removeFlexible(bsgfx_TypeId type_id, int id) {
	bsgfx_Type* type = bsgfx_getType(type_id);

	void* result = bsmod_shiftFlexible(type_id, id, NULL);

	bsgfx_map(type_id, id);

	return result;
}

void* bsmod_add(bsgfx_TypeId id, void* data) {
	bsgfx_Type* type = bsgfx_getType(id);

	if (type->count >= type->capacity) {
		type->capacity += 128;

		size_t mapped_capacity = type->capacity * type->mapped_unit_size + type->flexible_capacity * type->mapped_flexible_size;
		size_t unmapped_capacity = type->capacity * type->unmapped_unit_size + type->flexible_capacity * type->unmapped_flexible_size;

		size_t mapped_old_capacity = type->count * type->mapped_unit_size + type->flexible_count * type->mapped_flexible_size;
		size_t unmapped_old_capacity = type->count * type->unmapped_unit_size + type->flexible_count * type->unmapped_flexible_size;

		type->mapped = bs_realloc(type->mapped, mapped_capacity);
		type->unmapped = bs_realloc(type->unmapped, unmapped_capacity);
		type->unmapped_accessors = bs_realloc(type->unmapped_accessors, type->capacity * sizeof(int));
		type->mapped_accessors = bs_realloc(type->mapped_accessors, type->capacity * sizeof(int));

		memset(type->mapped + mapped_old_capacity, 0, mapped_capacity - mapped_old_capacity);
		memset(type->unmapped + unmapped_old_capacity, 0, unmapped_capacity - unmapped_old_capacity);
		memset(type->unmapped_accessors + type->count, 0, type->capacity * sizeof(int) - type->count * sizeof(int));
		memset(type->mapped_accessors + type->count, 0, type->capacity * sizeof(int) - type->count * sizeof(int));
	}

	//bs_warnF("Max number of %s exceeded!\n", type->plural);

	int* this = type->unmapped_accessors + type->count;
	int* last = this - 1;
	*this = type->count == 0 ? 0 : (*last + type->unmapped_unit_size + bsgfx_flexibleCount(id, type->count - 1));

	int* this_mapped = type->mapped_accessors + type->count;
	int* last_mapped = this_mapped - 1;
	*this_mapped = type->count == 0 ? 0 : (*last_mapped + type->mapped_unit_size + bsgfx_flexibleCount(id, type->count - 1));

	bs_except(BSX_OUT_OF_BOUNDS);
	void* unmapped = bsgfx_getRaw(id, type->count);
	bs_except(BSX_OUT_OF_BOUNDS);
	void* mapped = bsgfx_get(id, type->count);
	bs_except(0);

	if (data)
		memcpy(unmapped, data, type->unmapped_unit_size);
	else
		memset(unmapped, 0, type->unmapped_unit_size); // unnecessary?

	type->mapper(unmapped, mapped);
	type->count++;

	return unmapped;
}



/*=============================================================================
 * Type Selection
 *============================================================================*/

bool bsmod_isSelected(bs_List* list, bsgfx_TypeId type, int id) {
	if (bsmod.selected_type != type)
		return false;

	for (int i = 0; i < list->count; i++) {
		int* this = bs_fetchUnit(list, i);
		if (*this == id)
			return true;
	}
	return false;
}

void bsmod_select(bs_List* list, bsgfx_TypeId type, int id) {
	if (bsmod.selected_type != type) {
		bsmod_deselectAll();
		bsmod.selected_type = type;
	}

	if (bsmod_isSelected(list, type, id))
		return;

	if (list == BSMOD_IDS)
		bs_infoF("Selected %s %d\n", bsgfx_getType(bsmod.selected_type)->singular, id);

	// bsgfx_highlightEditorTab("instance");

	//int max_count = bsgfx_count(type);
	//if (id >= max_count || id < 0)
	//	return bs_throwBasiliskF(BSX_OUT_OF_BOUNDS, "%s id %d (0-%d)", bsgfx_getType(type)->singular, id, max_count);

	bs_pushBack(list, &id);
}

static void bsmod_clearTiles() {
	bs_Batch* batch = bs_fetch(BSMOD_BATCHES, BSMOD_BATCH_TILE)->batch;
	bs_Scope last = *bs_getScope();
	bs_setScope(&(bs_Scope) { .queue = bs_singleTimesQueue() });
	bs_unpushBatch(batch);
	bs_pushBatch(batch, BS_U32_MAX, BS_U32_MAX);
	bs_setScope(&last);
}

void bsmod_deselectAll() {
	if (bsmod.selected_ids.count != 0 || bsmod.selected_tiles.count != 0)
		bs_infoF("Deselected all %s\n", bsgfx_getType(bsmod.selected_type)->plural);

	if (bsmod.selected_type == BSGFX_TYPE_TILE)
		bsmod_clearTiles();

	bsmod.selected_tiles.count = 0;
	bsmod.selected_ids.count = 0;
	bsmod.selected_type = BSGFX_TYPE_UNDEFINED;
	bsmod.edit_type = BSMOD_EDIT_UNDEFINED;
}

void bsmod_deselectRange(bs_List* list, int first, int count) {
	if (count > 0)
		bs_infoF("Deselected %s %d-%d\n", bsgfx_getType(bsmod.selected_type)->plural, first, (first + count));

	for (int i = first; i + count < list->count; i++) {
		int* this = bs_fetchUnit(list, i);
		int* next = bs_fetchUnit(list, i + count);
		*this = *next;
	}

	list->count -= count;

	if (list->count == 0)
		bsmod_deselectAll();
}

void bsmod_deselectIndex(bs_List* list, int id) {
	if (id < 0)
		return;

	for (int i = id; i < list->count; i++) {
		int* this = bs_fetchUnit(list, i);
		int* next = bs_fetchUnit(list, i + 1);
		*this = *next;
	}

	bs_infoF("Deselected %s %d\n", bsgfx_getType(bsmod.selected_type)->singular, id);
	if (--list->count == 0)
		bsmod_deselectAll();
}

void bsmod_deselect(bs_List* list, int id) {
	int start = -1;
	for (int i = 0; i < list->count; i++) {
		int* this = bs_fetchUnit(list, i);
		if (*this == id) {
			start = i;
			break;
		}
	}

	bsmod_deselectIndex(list, start);
}

int bsmod_firstSelectedId(bs_List* list) {
	if (list->count <= 0)
		return -1;

	int* first = list->data;
	//	if (*first < 0 || (*first >= bsgfx_getType(bsmod.selected_type)->count))
	//		return -1;

	return *first;
}

int bsmod_lastSelectedId(bs_List* list) {
	int* first = list->data;
	//	if (first < 0 || (bsmod.selected_ids.count - 1) >= bsgfx_getType(bsmod.selected_type)->count)
	//		return -1;

	int* last = bs_fetchUnit(list, list->count - 1);
	return *last;
}



/*=============================================================================
 * Version Converters
 *============================================================================*/

 /**
  Tile version converter
  */
static void bsmod_convertTileVersion1(bsgfx_TypeHeader* old_tiles, bsgfx_TypeHeader* new_tiles) {
	unsigned char* old_data = old_tiles->accessors + old_tiles->count;
	unsigned char* new_data = new_tiles->accessors + new_tiles->count;

	size_t offset = 0;
	for (int i = 0; i < old_tiles->count; i++) {
		bsgfx_RawTileV1* old_tile = old_data + old_tiles->accessors[i];
		bsgfx_RawTile* new_tile = new_data + offset;

		*new_tile = (bsgfx_RawTile){
			.axis = 0,
			.coords = bs_iv2V2(old_tile->coords),
			.flags = old_tile->flags,
			.primitive = old_tile->primitive,
			.texture_hash = old_tile->texture_hash,
		};

		new_tiles->accessors[i] = offset += sizeof(bsgfx_RawTile);
	}
}

static void bsmod_convertTileVersion(int package_id, bsgfx_Scene* scene) {
	bs_Package* package = bs_fetchUnit(bs_packages(), package_id);
	static bs_String* s;
	bs_except(BSX_FAILED_TO_QUERY);
	bs_Resource* tiles = bs_loadResource(package_id, (s = bs_stringF(s, "levels/%s_tiles", scene->name))->value, 0);
	if (bs_except(0))
		return;
	bsgfx_TypeHeader* old_tiles = tiles->data->value;
	if (old_tiles->version == BSGFX_TILE_VERSION)
		return bs_infoF("Tiles are up to date.\n");

	size_t size =
		sizeof(bsgfx_TypeHeader) +
		old_tiles->count * sizeof(int) +
		old_tiles->count * sizeof(bsgfx_RawTile);

	bsgfx_TypeHeader* new_tiles = bs_malloc(size);
	memcpy(new_tiles, old_tiles, sizeof(bsgfx_TypeHeader));
	new_tiles->version = BSGFX_TILE_VERSION;

	switch (old_tiles->version) {
	case 1: bsmod_convertTileVersion1(old_tiles, new_tiles); break;
	default:
		bs_throwBasiliskF(BSX_NOT_IMPLEMENTED, "Tile version %d converter", old_tiles->version);
	}

	bsmod_packResource(BSGFX_RESOURCE_TILE, new_tiles, size, bs_fileName(package->path), s->value);
	bs_free(new_tiles);
}

/***/
void bsmod_ensureTypeVersionsAreUpToDate(int package_id) {
	bs_Package* package = bs_fetchUnit(bs_packages(), package_id);

	char* package_name = bs_fileName(package->path);
	bsmod_iniPackage(package_name);

	bsmod_convertTileVersion(package_id, bsgfx_currentScene());

	//bs_except(BSX_FAILED_TO_QUERY);
	//bs_Resource* primitives = bs_loadResource(package_id, (s = bs_stringF(s, "levels/%s_primitives", scene->name))->value, 0);
	//
	//bs_except(BSX_FAILED_TO_QUERY);
	//bs_Resource* prefabs = bs_loadResource(package_id, (s = bs_stringF(s, "levels/%s_prefabs", scene->name))->value, 0);
	//bs_except(0);

	bsmod_savePackage(package_name);
}



/*=============================================================================
 * Hovering Pixel Data
 * TODO: move some of this somewhere else
 *============================================================================*/

void bsmod_copyHoveringDataToBuffer() {
	if (!bs_exists(BSGFX_RENDERERS, BSGFX_RENDERER_LO_RES)) return;
	bs_Renderer* renderer = bs_fetch(BSGFX_RENDERERS, BSGFX_RENDERER_LO_RES)->renderer;

	bs_ivec2 resolution = bs_resolution();
	bs_ivec2 pixel_resolution = bs_iv2(resolution.x / BSGFX_PIXEL_SCALE, resolution.y / BSGFX_PIXEL_SCALE);
	bs_vec2 p = bs_v2Mul(bs_cursorPosition(), BS_IV2_TO_V2(resolution));
	bs_ivec2 cursor_position = bs_iv2(p.x / BSGFX_PIXEL_SCALE, p.y / BSGFX_PIXEL_SCALE);

	if (bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS) &&
		bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_SCREENSHOT))
	{
		bs_Buffer* cursor_reads_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS)->buffer;
		bs_Buffer* tile_read_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_SCREENSHOT)->buffer;

		for (int i = 0; i < renderer->num_outputs; i++) {
			bs_Image* image = renderer->outputs[i].image;
			if (!(image->flags & BS_IMAGE_USAGE_TRANSFER_SRC_BIT))
				continue;

			bs_except(BSX_OUT_OF_BOUNDS);
			bs_copyImageToBufferAsync(
				image,
				cursor_reads_buffer,
				0,
				BS_LAYOUT_GENERAL,
				i * 4,
				cursor_position,
				bs_iv2(1, 1));

			bs_except(BSX_OUT_OF_BOUNDS);
			if (bsmod.queue.screenshot)
				bs_copyImageToBufferAsync(image, tile_read_buffer, 0, BS_LAYOUT_GENERAL, pixel_resolution.x * pixel_resolution.y * i * 4, bs_iv2Iv1(0), pixel_resolution);
			bs_logBasilisk(bs_except(0));
		}
	}
}

void bsmod_readHoveringInstanceData() {
	bsmod.hovering.instance_type = -1;
	bsmod.hovering.subtype = -1;

	for (int j = 0; j < BSGFX_INSTANCE_TYPE_COUNT; j++) {
		if (bsmod.hovering.flags & (1 << j)) {
			bsmod.hovering.instance_type = j;
			break;
		}
	}

	if (bsmod.hovering.instance_type != -1) {
		bsgfx_InstanceBuffer* instance = bs_bufferMap(poser()->instance_buffers[bsmod.hovering.instance_type]);
		instance += bsmod.hovering.index;
		bsmod.hovering.instance_id = instance->header.id;

		bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
		bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

		for (int j = 0; j < metadata->subtypes_count; j++) {
			if (metadata->instance_subtypes[j].instance_type != bsmod.hovering.instance_type) continue;

			struct bsgfx_InstanceSubtypeMetadata* subtype_metadata = metadata->instance_subtypes + j;

			if (bsmod.hovering.index >= metadata->instance_subtypes[j].instance_offset &&
				bsmod.hovering.index < (metadata->instance_subtypes[j].instance_offset + metadata->instance_subtypes[j].instance_count)) {

				bsmod.hovering.subtype = j;
				break;
			}
		}

	}
}

void bsmod_readHoveringVertex() {
	if (bsmod.hovering.instance_type < 0)
		return;

	if (!bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS))
		return;
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS)->buffer;

	bs_U32* data = buffer->_->data ? bs_bufferMap(buffer) : bs_mapBuffer(buffer, buffer->num_bytes);

	bsgfx_InstanceBuffer* instance = bs_bufferMap(poser()->instance_buffers[bsmod.hovering.instance_type]);

	bool is_unselected_primitive = bsmod.hovering.flags & BSGFX_ID_IS_PRIMITIVE && !bsmod_isSelected(BSMOD_IDS, BSGFX_TYPE_PRIMITIVE, instance->header.id);
	if (is_unselected_primitive)
		bsmod.hovering.closest_primitive = bsmod.hovering.instance_id;

	bs_RGBA position = *(bs_RGBA*)(data + BSGFX_LO_SUBPASS_0_OUT_POSITION_LOCAL);
	bs_vec3 normal = BS_V3((position.r / 255.0 - 0.5) * 2.0, (position.g / 255.0 - 0.5) * 2.0, (position.b / 255.0 - 0.5) * 2.0);
	normal.x = (normal.x == 0.0f) ? 0.0f : (normal.x > 0.0f ? 1.0f : -1.0f);
	normal.y = (normal.y == 0.0f) ? 0.0f : (normal.y > 0.0f ? 1.0f : -1.0f);
	normal.z = (normal.z == 0.0f) ? 0.0f : (normal.z > 0.0f ? 1.0f : -1.0f);

	if (is_unselected_primitive)
		bsmod.hovering.closest_vertex = normal;
}

void bsmod_readHoveringOutputs() {
	bsmod.hovering.color = BS_RGBA(0, 0, 0, 0);
	bsmod.hovering.normal = bs_v3V1(0);
	bsmod.hovering.flags = bsmod.hovering.index = 0;

	if (!bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS))
		return;
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS)->buffer;

	bs_U32* data = buffer->_->data ? bs_bufferMap(buffer) : bs_mapBuffer(buffer, buffer->num_bytes);

	bs_RGBA* normal_color = data + BSGFX_LO_SUBPASS_0_OUT_NORMAL;
	bsmod.hovering.normal = BS_V3((float)normal_color->r / 255.0, (float)normal_color->g / 255.0, (float)normal_color->b / 255.0);
	bsmod.hovering.color = *(bs_RGBA*)(data + BSGFX_LO_SUBPASS_0_OUT_COLOR);
	bsmod.hovering.index = data[BSGFX_LO_SUBPASS_0_OUT_INDEX];
	bsmod.hovering.flags = data[BSGFX_LO_SUBPASS_0_OUT_FLAGS];
}

void bsmod_editSelectedType() {
	if (bsmod.selected_type == BSGFX_TYPE_UNDEFINED)
		return;

	bsgfx_Type* type = bsgfx_getType(bsmod.selected_type);

	if (bsmod_keyDownOnce(BS_KEY_DELETE)) {
		bsmod_deleteSelected(bsmod.selected_type);
	}
}

void bsmod_selectHoveringTypes() {
	if (bsmod.hovering.billboard) {
		return;
	}

	bool select_tile = false;
	if (bsmod_leftClickOnce()) {
		if (bsmod.selected_type == BSGFX_TYPE_PRIMITIVE && bsmod.selected_ids.count && bsmod.hovering.primitive == bsmod_firstSelectedId(BSMOD_IDS)) {
			select_tile = true;
		}
		else if (bsmod.selected_type == BSGFX_TYPE_TILE) {
			if (bsmod_keyDown(BS_KEY_ALT) && !bsmod_keyDown(BS_KEY_LEFT_CONTROL))
				select_tile = true;
			if (bsmod_keyDown(BS_KEY_LEFT_SHIFT))
				select_tile = true;
		}
	}

	if (select_tile && !bsmod_keyDown(BS_KEY_LEFT_SHIFT)) {
		if (bsmod.selected_tile_axis != bsmod.hovering.tile_axis)
			bsmod_deselectAll();

		bsmod.selected_tile_primitive = bsmod.hovering.primitive;
		bsmod.selected_tile_axis = bsmod.hovering.tile_axis;

		if (bsmod_isSelected(BSMOD_TILE_IDS, BSGFX_TYPE_TILE, bsmod.hovering.tile))
			bsmod_deselect(BSMOD_TILE_IDS, bsmod.hovering.tile);
		else
			bsmod_select(BSMOD_TILE_IDS, BSGFX_TYPE_TILE, bsmod.hovering.tile);
	}

	bsmod.hovering.tile_axis = bsmod.hovering.primitive = bsmod.hovering.tile = -1;

	if (bsmod.hovering.flags & BSGFX_ID_IS_PRIMITIVE) {
		bs_U32 tile = bsgfx_extract24(bsmod.hovering.index);
		bs_U32 axis = bsgfx_extract8(bsmod.hovering.index);

		for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
			bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);
			if (tile >= primitive->first_tile && tile < primitive->last_tile) {
				if (bsmod_leftClickOnce() && !select_tile) {
					bsmod_deselectAll();
					bsmod_select(BSMOD_IDS, BSGFX_TYPE_PRIMITIVE, i);
				}

				bsmod.hovering.primitive = i;
				bsmod.hovering.tile_axis = axis;
				bsmod.hovering.tile = tile;

				break;
			}
		}
	}

	if (bsmod.hovering.flags & BSGFX_ID_IS_PREFAB) {
		//bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, bsmod.hovering.index);

		if (bsmod_leftClickOnce()) {
			if (!bsmod_keyDown(BS_KEY_ALT) || bsmod_keyDown(BS_KEY_LEFT_CONTROL))
				bsmod_deselectAll();

			bsmod_select(BSMOD_IDS, BSGFX_TYPE_PREFAB, bsmod.hovering.instance_id);
		}
	}

	if (bsmod_leftClickOnce() && bsmod.selected_type == BSGFX_TYPE_TILE && bsmod_keyDown(BS_KEY_LEFT_SHIFT)) {
		bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, bsmod.selected_tile_primitive);

		int first_tile = bsmod_firstSelectedId(BSMOD_TILE_IDS);
		int first_tile_axis = bsgfx_tileAxis(primitive, first_tile);

		if (first_tile_axis == bsmod.hovering.tile_axis && bsmod.hovering.tile >= 0 && first_tile >= 0) {

			bs_ivec2 start = bsgfx_tileCoordinate(primitive, bsmod.hovering.tile_axis, bsmod.hovering.tile);
			bs_ivec2 end = bsgfx_tileCoordinate(primitive, first_tile_axis, first_tile);
			if (start.x != BS_I32_MAX && end.x != BS_I32_MAX) {
				if (start.x > end.x) {
					float temp = start.x;
					start.x = end.x;
					end.x = temp;
				}
				if (start.y > end.y) {
					float temp = start.y;
					start.y = end.y;
					end.y = temp;
				}

				for (int x = start.x; x < end.x + 1; x++) {
					for (int y = start.y; y < end.y + 1; y++) {
						int index = bsgfx_tileIndex(primitive, bsmod.hovering.tile_axis, x, y);

						bsmod_select(BSMOD_TILE_IDS, BSGFX_TYPE_TILE, index);
					}
				}
			}
		}
	}

	 /**
	  For each selected tile, get its primitive and select all tiles on that primitive
	  */
	if (bsmod.selected_type == BSGFX_TYPE_TILE && bsmod_keyDown(BS_KEY_LEFT_CONTROL) && bsmod_keyDownOnce(BS_KEY_A)) {
		static bs_List primitives_to_select = { .unit_size = sizeof(int) , .increment = 256 };
		bs_seekList(&primitives_to_select, 0);

		const int primitives_count = bsgfx_count(BSGFX_TYPE_PRIMITIVE);

		for (int i = 0; i < primitives_count; i++) {
			bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);

			for (int j = 0; j < bsmod.selected_tiles.count; j++) {
				int tile_id = *(int*)bs_fetchUnit(&bsmod.selected_tiles, j);

				if (tile_id >= primitive->first_tile && tile_id <= primitive->last_tile) {
					bs_pushBack(&primitives_to_select, &i);
					break;
				}
			}
		}

		for (int i = 0; i < primitives_to_select.count; i++) {
			int primitive_id = *(int*)bs_fetchUnit(&primitives_to_select, i);
			bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, primitive_id);

			for (int j = primitive->first_tile; j <= primitive->last_tile; j++) {
				int axis = bsgfx_tileAxis(primitive, j);
				if (axis == bsmod.selected_tile_axis)
					bsmod_select(BSMOD_TILE_IDS, BSGFX_TYPE_TILE, j);
			}

			break;
		}
	}

	if (bsmod_leftClickOnce()) {
		bsmod.ui_blocked = true;
	}

	//printf("%d, prim %d, tile %d\n", bsmod.hovering.index, bsmod.hovering.primitive, bsmod.hovering.tile);
}
