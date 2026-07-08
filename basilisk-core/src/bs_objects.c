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



  /*==============================================================================
   * Package Format (.bpak)
   *============================================================================*/

BSAPI bs_List* _bs_packages() {
    static bs_List _bs_packages_ = {
        .unit_size = sizeof(bs_Package),
        .increment = 4
    };

    return &_bs_packages_;
}

BSAPI bs_List* _bs_objectSources() {
    static bs_List _bs_object_sources_ = {
        .unit_size = sizeof(bs_Object),
        .increment = 32,
    };

    return &_bs_object_sources_;
}

BSAPI void _bs_destroyResource(bs_Resource* resource) {
    bs_free(resource->data);
    bs_free(resource);
}

#define BS_FAILED_TO_QUERY(format, ...) \
    bs_warnF("Failed to query:\n" format __VA_OPT__("\n",) __VA_ARGS__)

BSAPI bs_Result _bs_queryResource(int package_id, const char* name, bs_Resource** out) {
    bs_Package* package = bs_fetchUnit(_bs_packages(), package_id);
    bs_U64 hash = bs_stringHash(name);

    for (int i = 0; i < package->resource_headers.count; i++) {
        bs_ResourceHeader* resource = bs_fetchUnit(&package->resource_headers, i);
        if (resource->header.name_hash == hash) {
            *out = resource->resource;
            return BS_RESULT_OK;
        }
    }

    *out = NULL;

    BS_FAILED_TO_QUERY("Resource \"%s\"", name);
    return BS_RESULT_FAILED_TO_QUERY;
}

BSAPI bs_Result _bs_queryPackage(const char* name, int* out) {
    bs_U64 hash = bs_stringHash(name);

    for (int i = 0; i < _bs_packages()->count; i++) {
        bs_Package* package = bs_fetchUnit(_bs_packages(), i);
        if (package->path_hash == hash) {
            *out = i;
            return i;
        }
    }

    *out = -1;
    BS_FAILED_TO_QUERY("Package \"%s\"", name);
    return BS_RESULT_FAILED_TO_QUERY;
}

 BSAPI bs_Result _bs_loadResource(int package_id, const char* resource_name, bs_U32 flags, bs_Resource** out) {
    bs_Package* package = bs_fetchUnit(_bs_packages(), package_id);

    bs_U64 hash = bs_stringHash(resource_name);

    bs_ResourceHeader* existing = NULL;
    for (int i = 0; i < package->resource_headers.count; i++) {
        bs_ResourceHeader* resource = bs_fetchUnit(&package->resource_headers, i);
        if (resource->header.name_hash == hash) {
            existing = resource;
            break;
        }
    }

    if (!existing) {
        bs_warnF("Failed to query resource \"%s\" in package \"%s\"\n", resource_name, package->path);
        return BS_RESULT_FAILED_TO_QUERY;
    }

    if (!existing->resource) {
        existing->resource = bs_calloc(1, sizeof(bs_Resource)); // TODO: don't allocate each resource individually
        existing->resource->hash = hash;
        existing->resource->name = resource_name;
    }
    //else
    //    bs_free(existing->resource->data);
    bs_Result result = bs_loadFileChunkF(
        existing->header.offset, 
        existing->header.size, 
        &existing->resource->data, 
        "%s_%03d.bpak", 
        package->path, (existing->header.chunk + 1)
    );

    if (result == BS_RESULT_OK) {
        bs_infoF("Loaded resource \"%s\"\n", resource_name);
        *out = existing->resource;
    }

    return BS_RESULT_OK;
}

BSAPI bs_Result _bs_loadPackage(const char* path, int* out) {
    bs_Result result;

    char* path_dup = strdup(path);
    bs_U64 hash = bs_stringHash(path_dup);

    bs_Package* existing = NULL, * old = NULL;
    int id = -1;
    for (int i = 0; i < _bs_packages()->count; i++) {
        bs_Package* package = bs_fetchUnit(_bs_packages(), i);
        if (package->path_hash == hash) {
            old = package;
            id = i;
            bs_free(old->raw);
            old->raw = NULL;
            break;
        }
    }

    bs_String* raw;
    result = bs_loadFileF(&raw, "%s.bpak", path_dup);
    if (!raw)
        return result;

    existing = old;

    bs_List new_resources = bs_list(sizeof(bs_ResourceHeader), 32);
    bs_infoF("Loading package \"%s\"\n", path_dup);
    if (!existing) {
        id = _bs_packages()->count;
        existing = bs_pushBack(_bs_packages(), &(bs_Package) {
            .path_hash = hash,
            .path = path_dup,
        });
    }

    existing->raw = raw;

    for (int i = 0; i < (raw->len - 1);) {
        bs_ResourceHeader* header = raw->value + i;
        i += sizeof(header->header);
        if (i >= raw->len) {
            return BS_RESULT_CORRUPTED;
        }

        char* name = raw->value + i;
        i += header->header.name_length + 1;
        if (i >= raw->len) {
            return BS_RESULT_CORRUPTED;
        }

        char* end = strchr(name, '\n');
        if (!end) {
            return BS_RESULT_CORRUPTED;
        }
        end[0] = '\0';

        bs_ResourceHeader* added = bs_pushBack(&new_resources, &(bs_ResourceHeader) {
            .header = header->header,
            .name = strdup(name),
        });

        if (old) {
            for (int j = 0; j < old->resource_headers.count; j++) {
                bs_ResourceHeader* existing_header = bs_fetchUnit(&old->resource_headers, j);
                if (existing_header->header.name_hash == header->header.name_hash) {
                    added->resource = existing_header->resource;
                    break;
                }
            }
        }

        end[0] = '\n';
    }

    existing->resource_headers = new_resources;

    *out = id;
    return BS_RESULT_OK;
}



  /*==============================================================================
   * Objects
   *============================================================================*/

#define BS_VALIDATE_SOURCE(source_id, _return) \
    BS_VALIDATE(source_id < bs_objectSources()->count, _return,)

#define BS_VALIDATE_ID(id, _return) \
    BS_VALIDATE(id < ((bs_ObjectSource*)bs_fetchUnit(bs_objectSources(), source_id))->ids_count, _return,)

BSAPI int _bs_configureSource(bs_ObjectType type, int count, const char** names) {
    bs_ObjectSource source = {
        .type = type,
        .ids = bs_calloc(count, sizeof(struct bs_ObjectId)),
        .ids_count = count,
    };

    for (int i = 0; i < source.ids_count; i++) {
        source.ids[i].name = names[i];
        source.ids[i].name_hash = bs_stringHash(source.ids[i].name);
    }

    bs_List* object_sources = bs_objectSources();
    bs_pushBack(object_sources, &source);

    return object_sources->count - 1;
}

 /**
  bs_exists
  */
BSAPI bool _val_bs_exists(bs_U32 source_id, bs_U32 id) {
    BS_VALIDATE_SOURCE(source_id, false);
    BS_VALIDATE_ID(id, false);

    return bs_exists(source_id, id);
}

BSAPI bool _bs_exists(bs_U32 source_id, bs_U32 id) {
    bs_List* object_sources = bs_objectSources();
    bs_ObjectSource* source = bs_fetchUnit(object_sources, source_id);

    bs_Object* object = source->ids[id].object;

    return
        object &&
       // object->strikes == 0 &&
        object->head != NULL && 
        object->head->id >= 0;
}

 /**
  bs_fetch
  */
BSAPI bs_Object* _val_bs_fetch(bs_U32 source_id, bs_U32 id) {
    BS_VALIDATE_SOURCE(source_id, NULL);
    BS_VALIDATE_ID(id, NULL);

    return bs_fetch(source_id, id);
}

BSAPI bs_Object* _postval_bs_fetch(bs_U32 source_id, bs_U32 id, bs_Object* object) {
    BS_VALIDATE(object != NULL, NULL, "Object (%d, %d) has not been created yet", source_id, id);
    BS_VALIDATE(object->head != NULL, NULL, "Object (%d, %d)", source_id, id); // TODO: what does this mean
    BS_VALIDATE(object->head->id >= 0, NULL, "Object (%d, %d)", source_id, id); // TODO: what does this mean

    return object;
}

BSAPI bs_Object* _bs_fetch(bs_U32 source_id, bs_U32 id) {
    bs_List* object_sources = bs_objectSources();
    bs_ObjectSource* source = bs_fetchUnit(object_sources, source_id);
    bs_Object* object = source->ids[id].object;

    return object;
}

 /**
  */
BSAPI bool _bs_shouldLoadId(bs_U32 source_id, bs_U32 id) {
    return true;

   // assert(source_id < _bs_object_sources.count);
   // bs_ObjectSource* source = bs_fetchUnit(&_bs_object_sources, source_id);
   // assert(id < source->ids_count);
   // bs_Object* object = source->ids[id].object;

   // return object->flags & BS_OBJECT_SHOULD_LOAD;
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

static inline void bs_logObjectCreated(bs_U32 source_id, bs_U32 id) {
    bs_List* object_sources = bs_objectSources();
    bs_ObjectSource* source = bs_fetchUnit(object_sources, source_id);
    bs_Object* object = source->ids[id].object;
    const char* object_type_name = bs_objectTypeName(source->type);

    if (_bs_args_.color_log)
        bs_infoF(BS_PRINT_GREEN "+" BS_PRINT_RESET " (%d) %s " BS_PRINT_CYAN "%s\n" BS_PRINT_RESET, id, object_type_name, source->ids[id].name);
    else
        bs_infoF("+ (%d) %s %s\n", id, object_type_name, source->ids[id].name);
}

static inline void bs_logObjectUpdated(bs_U32 source_id, bs_U32 id) {
    bs_List* object_sources = bs_objectSources();
    bs_ObjectSource* source = bs_fetchUnit(object_sources, source_id);
    bs_Object* object = source->ids[id].object;
    const char* object_type_name = bs_objectTypeName(source->type);

    if (_bs_args_.color_log)
        bs_infoF(BS_PRINT_YELLOW "/" BS_PRINT_RESET  " (%d) %s " BS_PRINT_CYAN "%s\n" BS_PRINT_RESET, id, object_type_name, source->ids[id].name);
    else
        bs_infoF("/ (%d) %s %s\n", id, object_type_name, source->ids[id].name);
}

static inline void bs_logObjectUnchanged(bs_U32 source_id, bs_U32 id) {
  //  if (_bs_args_.color_log)
  //      bs_infoF(BS_PRINT_GRAY "~" BS_PRINT_RESET  " (%d) %s " "%s\n" BS_PRINT_RESET, id, bs_objectName(type), bs_idName(id));
  //  else
  //      bs_infoF("~ (%d) %s %s\n", id, bs_objectName(type), bs_idName(id));
}

static inline bs_ObjectSource* bs_fetchObjectSource(bs_U32 source_id) {
    bs_List* object_sources = bs_objectSources();
    bs_ObjectSource* source = bs_fetchUnit(object_sources, source_id);
    return source;
}

BSAPI const char* _val_bs_idName(bs_U32 source_id, bs_U32 id) {
    BS_VALIDATE_SOURCE(source_id, NULL);
    BS_VALIDATE_ID(id, NULL);

    return bs_idName(source_id, id);
}

BSAPI const char* _bs_idName(bs_U32 source_id, bs_U32 id) {
    bs_ObjectSource* source = bs_fetchObjectSource(source_id);
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

BSAPI bs_Object* _val_bs_object(bs_U32 source_id, bs_U32 id, size_t size, size_t flexible_array_size, int flexible_count, bs_U32 flags) {
    BS_VALIDATE_SOURCE(source_id, NULL);
    BS_VALIDATE_ID(id, NULL);

    return bs_object(source_id, id, size, flexible_array_size, flexible_count, flags);
}

BSAPI bs_Object* _bs_object(bs_U32 source_id, bs_U32 id, size_t size, size_t flexible_array_size, int flexible_count, bs_U32 flags) {
    if (source_id == BS_U32_MAX) {
        return bs_allocateObject(size, flexible_array_size, flexible_count, flags);
    }

    bs_ObjectSource* source = bs_fetchObjectSource(source_id);

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
