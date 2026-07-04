#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>

#ifdef WIN32
#include <io.h>
#include <shlobj_core.h >
#else
#include <unistd.h>
#endif

#include <basilisk-core.h>s
#include <bs_internal.h>
#include <vulkan.h>

bs_List _bs_packages = { 
    .unit_size = sizeof(bs_Package), 
    .increment = 4
};

bs_List _bs_object_sources = {
    .unit_size = sizeof(bs_Object),
    .increment = 32,
};



  /*==============================================================================
   * Package Format (.bpak)
   *============================================================================*/

bs_List* bs_packages() {
    return &_bs_packages;
}

bs_List* bs_objectSources() {
    return &_bs_object_sources;
}

void bs_destroyResource(bs_Resource* resource) {
    bs_free(resource->data);
    bs_free(resource);
}

bs_Resource* bs_queryResource(int package_id, const char* name) {
    bs_Package* package = bs_fetchUnit(&_bs_packages, package_id);
    bs_U64 hash = bs_stringHash(name);

    for (int i = 0; i < package->resource_headers.count; i++) {
        bs_ResourceHeader* resource = bs_fetchUnitUnsafe(&package->resource_headers, i);
        if (resource->header.name_hash == hash)
            return resource->resource;
    }

    bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Resource \"%s\"", name);
    return NULL;
}

int bs_queryPackage(const char* name) {
    bs_U64 hash = bs_stringHash(name);

    for (int i = 0; i < _bs_packages.count; i++) {
        bs_Package* package = bs_fetchUnit(&_bs_packages, i);
        if (package->path_hash == hash)
            return i;
    }

    bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Package \"%s\"", name);
    return -1;
}

bs_Resource* bs_loadResource(int package_id, const char* resource_name, bs_U32 flags) {
    bs_Package* package = bs_fetchUnit(&_bs_packages, package_id);

    bs_U64 hash = bs_stringHash(resource_name);

    bs_ResourceHeader* existing = NULL;
    for (int i = 0; i < package->resource_headers.count; i++) {
        bs_ResourceHeader* resource = bs_fetchUnitUnsafe(&package->resource_headers, i);
        if (resource->header.name_hash == hash) {
            existing = resource;
            break;
        }
    }

    if (!existing) {
        bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Resource \"%s\" in package \"%s\"", resource_name, package->path);
        return NULL;
    }

    if (!existing->resource) {
        existing->resource = bs_calloc(1, sizeof(bs_Resource)); // todo fix
        existing->resource->hash = hash;
        existing->resource->name = resource_name;
    }
    //else
    //    bs_free(existing->resource->data);
    existing->resource->data = bs_loadFileChunkF(existing->header.offset, existing->header.size, "%s_%03d.bpak", package->path, (existing->header.chunk + 1));

    bs_infoF("Loaded resource \"%s\"\n", resource_name);

    return existing->resource;
}

int bs_loadPackage(const char* path) {
    char* path_dup = strdup(path);
    bs_U64 hash = bs_stringHash(path_dup);

    bs_Package* existing = NULL, * old = NULL;
    int id = -1;
    for (int i = 0; i < _bs_packages.count; i++) {
        bs_Package* package = bs_fetchUnitUnsafe(&_bs_packages, i);
        if (package->path_hash == hash) {
            old = package;
            id = i;
            bs_free(old->raw);
            old->raw = NULL;
            break;
        }
    }

    bs_String* raw = bs_loadFileF("%s.bpak", path_dup);
    if (!raw)
        return -1;

    existing = old;

    bs_List new_resources = bs_list(sizeof(bs_ResourceHeader), 32);
    bs_infoF("Loading package \"%s\"\n", path_dup);
    if (!existing) {
        id = _bs_packages.count;
        existing = bs_pushBack(&_bs_packages, &(bs_Package) {
            .path_hash = hash,
            .path = path_dup,
        });
    }

    existing->raw = raw;

    for (int i = 0; i < (raw->len - 1);) {
        bs_ResourceHeader* header = raw->value + i;
        i += sizeof(header->header);
        if (i >= raw->len) {
            bs_throwBasilisk(BSX_CORRUPTED);
            return -1;
        }

        char* name = raw->value + i;
        i += header->header.name_length + 1;
        if (i >= raw->len) {
            bs_throwBasilisk(BSX_CORRUPTED);
            return -1;
        }

        char* end = strchr(name, '\n');
        if (!end) {
            bs_throwBasilisk(BSX_CORRUPTED);
            return -1;
        }
        end[0] = '\0';

        bs_ResourceHeader* added = bs_pushBack(&new_resources, &(bs_ResourceHeader) {
            .header = header->header,
            .name = strdup(name),
        });

        if (old) {
            for (int j = 0; j < old->resource_headers.count; j++) {
                bs_ResourceHeader* existing_header = bs_fetchUnitUnsafe(&old->resource_headers, j);
                if (existing_header->header.name_hash == header->header.name_hash) {
                    added->resource = existing_header->resource;
                    break;
                }
            }
        }

        end[0] = '\n';
    }

    existing->resource_headers = new_resources;

    return id;
}



  /*==============================================================================
   * Objects
   *============================================================================*/

int bs_configureSource(bs_ObjectType type, int count, const char** names) {
    bs_ObjectSource source = {
        .type = type,
        .ids = bs_calloc(count, sizeof(struct bs_ObjectId)),
        .ids_count = count,
    };

    for (int i = 0; i < source.ids_count; i++) {
        source.ids[i].name = names[i];
        source.ids[i].name_hash = bs_stringHash(source.ids[i].name);
    }

    bs_pushBack(&_bs_object_sources, &source);

    return _bs_object_sources.count - 1;
}

bool bs_exists(bs_U32 source_id, bs_U32 id) {
    assert(source_id < _bs_object_sources.count);
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);
    assert(id < source->ids_count);
    bs_Object* object = source->ids[id].object;

    return
        object &&
       // object->strikes == 0 &&
        object->head != NULL && 
        object->head->id >= 0;
}

bs_Object* bs_fetch(bs_U32 source_id, bs_U32 id) {
    assert(source_id < _bs_object_sources.count);
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);
    assert(id < source->ids_count);
    bs_Object* object = source->ids[id].object;
#ifdef _DEBUG
    if (!object || !object->head || object->head->id == -1) {
        bs_except(0); // force throw
        bs_throwBasiliskF(BSX_NOT_FOUND, "id = %d %s", id, bs_idName(source_id, id));
        return NULL;
    }
#endif
    return object;
}

bool bs_shouldLoadId(bs_U32 source_id, bs_U32 id) {
    return true;
    assert(source_id < _bs_object_sources.count);
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);
    assert(id < source->ids_count);
    bs_Object* object = source->ids[id].object;

    return object->flags & BS_OBJECT_SHOULD_LOAD;
}

static inline const char* bs_objectTypeName(bs_ObjectType type) {
    switch (type) {
    case BS_OBJECT_IMAGE: return "image";
    case BS_OBJECT_SAMPLER: return "sampler";
    case BS_OBJECT_BUFFER: return "buffer";
    case BS_OBJECT_QUEUE: return "queue";
    case BS_OBJECT_BATCH: return "batch";
    case BS_OBJECT_RENDERER: return "renderer";
    default: return "unknown object type";
    }
}

void bs_assertSourceIsType(int source_id, bs_ObjectType object_type) {
#ifdef _DEBUG
    bs_ObjectSource* source = bs_fetchUnit(&_bs_object_sources, source_id);

    if (source->type != object_type) {
        bs_except(0);
        bs_throwBasiliskF(BSX_ASSERTION, "Expected source %d to be of type \"%s\"", source_id, bs_objectTypeName(object_type));
    }
#endif
}

static inline void bs_logObjectCreated(bs_U32 source_id, bs_U32 id) {
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);
    bs_Object* object = source->ids[id].object;
    const char* object_type_name = bs_objectTypeName(source->type);

    if (_bs_args.color_log)
        bs_infoF(BS_PRINT_GREEN "+" BS_PRINT_RESET " (%d) %s " BS_PRINT_CYAN "%s\n" BS_PRINT_RESET, id, object_type_name, source->ids[id].name);
    else
        bs_infoF("+ (%d) %s %s\n", id, object_type_name, source->ids[id].name);
}

static inline void bs_logObjectUpdated(bs_U32 source_id, bs_U32 id) {
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);
    bs_Object* object = source->ids[id].object;
    const char* object_type_name = bs_objectTypeName(source->type);

    if (_bs_args.color_log)
        bs_infoF(BS_PRINT_YELLOW "/" BS_PRINT_RESET  " (%d) %s " BS_PRINT_CYAN "%s\n" BS_PRINT_RESET, id, object_type_name, source->ids[id].name);
    else
        bs_infoF("/ (%d) %s %s\n", id, object_type_name, source->ids[id].name);
}

static inline void bs_logObjectUnchanged(bs_U32 source_id, bs_U32 id) {
  //  if (_bs_args.color_log)
  //      bs_infoF(BS_PRINT_GRAY "~" BS_PRINT_RESET  " (%d) %s " "%s\n" BS_PRINT_RESET, id, bs_objectName(type), bs_idName(id));
  //  else
  //      bs_infoF("~ (%d) %s %s\n", id, bs_objectName(type), bs_idName(id));
}

const char* bs_idName(bs_U32 source_id, bs_U32 id) {
    assert(source_id < _bs_object_sources.count);
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);
    assert(id < source->ids_count);
    return source->ids[id].name;
}

static bs_Object* bs_allocateObject(size_t size, size_t flexible_size, int flexible_count, bs_U32 flags) {
    static int heaps_count;
    static struct {
        char* block;
        bs_U64 size;
    }* heaps = NULL;


    const size_t flexible_array_size = flexible_size * flexible_count;
    size_t new_size = size + flexible_array_size + sizeof(bs_Object);

    const int heap_size = 65536;
    if (!heaps || (heaps[heaps_count - 1].size + new_size) > heap_size) {
        heaps = bs_realloc(heaps, sizeof(*heaps) * ++heaps_count);
        heaps[heaps_count - 1].size = 0;
        heaps[heaps_count - 1].block = bs_malloc(heap_size);
    }

    unsigned char* result = (int*)(heaps[heaps_count - 1].block + heaps[heaps_count - 1].size);
    bs_Object object = {
        .head = result + sizeof(bs_Object),
        .flags = flags | BS_OBJECT_WAS_CREATED,
    };

    memcpy(result, &object, sizeof(bs_Object));
    memset(result + sizeof(bs_Object), 0, size);
    memset(result + sizeof(bs_Object) + size, 0, flexible_array_size);

    heaps[heaps_count - 1].size += new_size;

    return result;
}

static bs_Object* bs_update(bs_U32 source_id, bs_U32 id, int size, int swap_size) {
    bs_Object* object = bs_fetch(source_id, id);
    memset(object->head, 0, size + swap_size);
    //memcpy(object->head, data, size);
    object->head->id = id;
    object->head->source_id = source_id;
    object->flags |= BS_OBJECT_ALREADY_EXISTS | BS_OBJECT_WAS_ALTERED;
    object->flags &= ~BS_OBJECT_WAS_CREATED;

    bs_logObjectUpdated(source_id, id);

    return object;
}

bs_Object* bs_object(bs_U32 source_id, bs_U32 id, size_t size, size_t flexible_array_size, int flexible_count, bs_U32 flags) {
    if (source_id == BS_U32_MAX) {
        return bs_allocateObject(size, flexible_array_size, flexible_count, flags);
    }

    assert(source_id < _bs_object_sources.count);
    bs_ObjectSource* source = bs_fetchUnitUnsafe(&_bs_object_sources, source_id);

    assert(id < source->ids_count);
    bs_Object* result = source->ids[id].object;

    if (!bs_shouldLoadId(source_id, id))
        return NULL;

    if (result && result->head) {
        result->flags = flags | BS_OBJECT_ALREADY_EXISTS;
        if (flags & BS_OBJECT_FORCE_DESTROY)
            bs_logObjectUpdated(source, result);

        return result;
    }

    if (!bs_exists(source_id, id)) {
        result = source->ids[id].object = bs_allocateObject(size, flexible_array_size, flexible_count, flags);
        result->head->id = id;
        result->head->source_id = source_id;

        bs_logObjectCreated(source_id, id);

        return result;
    }
    else
        return bs_update(source_id, id, size, flexible_array_size);
}
