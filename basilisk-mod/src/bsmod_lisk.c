/*
#include <bs_http.h>
#include <bs_json.h>

#include <bsgfx_scene.h>
#include <bsgfx_collider.h>
#include <types/bsgfx_type.h>
#include <resources/script/bsgfx_script.h>
#include <bsgfx_editor.h>
#include <bsgfx_settings.h>
#include <bsgfx.h>
#include <math.h>
#include <assert.h>
#include <types/primitive/bsgfx_primitive.h>
#include <bsmod.h>
#include <bsmod_type.h>

bs_Server bsmod_server = { 0 };

static inline bs_Json bsmod_message(const char* message) {
	bs_String* raw = bs_stringF(NULL, "{\n    \"Message\": \"%s\"\n}", message);
	bs_Json json = bs_json(raw->value, raw->len);
	bs_free(raw);
	return json;
}

static bs_Json bsmod_primitive(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	if (bsmod.selected_ids.count <= 0)
		return bsmod_message("No primitive is selected.");

	if (bsmod.selected_ids.count != 1)
		return bsmod_message("Too many primitives selected.");

	bs_Json json = bs_emptyJson();

	int primitive_id = bsmod_firstSelectedId(BSGFX_TYPE_PRIMITIVE);
	int indices[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
	bs_vec3 vertices[36] = {
		BS_V3(-1.0, 1.0, -1.0), BS_V3(-1.0, 1.0, 1.0), BS_V3(1.0, 1.0, -1.0),
		BS_V3(-1.0, 1.0, 1.0), BS_V3(1.0, 1.0, 1.0), BS_V3(1.0, 1.0, -1.0),

		BS_V3(-1.0, -1.0, 1.0), BS_V3(-1.0, -1.0, -1.0), BS_V3(1.0, -1.0, -1.0),
		BS_V3(1.0, -1.0, 1.0), BS_V3(-1.0, -1.0, 1.0), BS_V3(1.0, -1.0, -1.0),

		BS_V3(-1.0, -1.0, 1.0), BS_V3(-1.0, 1.0, 1.0), BS_V3(-1.0, 1.0, -1.0),
		BS_V3(-1.0, -1.0, -1.0), BS_V3(-1.0, -1.0, 1.0), BS_V3(-1.0, 1.0, -1.0),

		BS_V3(1.0, 1.0, 1.0),  BS_V3(1.0, -1.0, 1.0), BS_V3(1.0, 1.0, -1.0),
		BS_V3(1.0, -1.0, 1.0), BS_V3(1.0, -1.0, -1.0), BS_V3(1.0, 1.0, -1.0),

		BS_V3(-1.0, 1.0, 1.0), BS_V3(-1.0, -1.0, 1.0), BS_V3(1.0, 1.0, 1.0),
		BS_V3(-1.0, -1.0, 1.0), BS_V3(1.0, -1.0, 1.0), BS_V3(1.0, 1.0, 1.0),

		BS_V3(-1.0, -1.0, -1.0), BS_V3(-1.0, 1.0, -1.0), BS_V3(1.0, 1.0, -1.0),
		BS_V3(1.0, -1.0, -1.0), BS_V3(-1.0, -1.0, -1.0), BS_V3(1.0, 1.0, -1.0),
	};

	bs_vec3 normals[36] = {
		BS_V3(0.0, 1.0, 0.0),
		BS_V3(0.0, 1.0, 0.0),
		BS_V3(0.0, 1.0, 0.0),
		BS_V3(0.0, 1.0, 0.0),
		BS_V3(0.0, 1.0, 0.0),
		BS_V3(0.0, 1.0, 0.0),
		BS_V3(0.0, -1.0, 0.0),
		BS_V3(0.0, -1.0, 0.0),
		BS_V3(0.0, -1.0, 0.0),
		BS_V3(0.0, -1.0, 0.0),
		BS_V3(0.0, -1.0, 0.0),
		BS_V3(0.0, -1.0, 0.0),
		BS_V3(-1.0, 0.0, 0.0),
		BS_V3(-1.0, 0.0, 0.0),
		BS_V3(-1.0, 0.0, 0.0),
		BS_V3(-1.0, 0.0, 0.0),
		BS_V3(-1.0, 0.0, 0.0),
		BS_V3(-1.0, 0.0, 0.0),
		BS_V3(1.0, 0.0, 0.0),
		BS_V3(1.0, 0.0, 0.0),
		BS_V3(1.0, 0.0, 0.0),
		BS_V3(1.0, 0.0, 0.0),
		BS_V3(1.0, 0.0, 0.0),
		BS_V3(1.0, 0.0, 0.0),
		BS_V3(0.0, 0.0, 1.0),
		BS_V3(0.0, 0.0, 1.0),
		BS_V3(0.0, 0.0, 1.0),
		BS_V3(0.0, 0.0, 1.0),
		BS_V3(0.0, 0.0, 1.0),
		BS_V3(0.0, 0.0, 1.0),
		BS_V3(0.0, 0.0, -1.0),
		BS_V3(0.0, 0.0, -1.0),
		BS_V3(0.0, 0.0, -1.0),
		BS_V3(0.0, 0.0, -1.0),
		BS_V3(0.0, 0.0, -1.0),
		BS_V3(0.0, 0.0, -1.0),
	};

	bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, primitive_id);
	bs_mat4 transform = bs_transform(primitive->position, primitive->rotation, primitive->scale);

	for (int i = 0; i < 36; i++) {
		bs_mat3 normal_matrix = bs_m3(transform.v[0].xyz, transform.v[1].xyz, transform.v[2].xyz);
		normal_matrix = bs_m3Transpose(bs_m3Inverse(normal_matrix));

		bs_vec3 vertex = bs_m4MulV3(transform, vertices[i]);
		bs_vec3 normal = bs_v3Normalize(bs_m3MulV3(normal_matrix, normals[i]));

		bs_ensureJsonF(&json, bs_jsonValue(vertex.x), "vertices[%d]", i * 3 + 0);
		bs_ensureJsonF(&json, bs_jsonValue(vertex.y), "vertices[%d]", i * 3 + 1);
		bs_ensureJsonF(&json, bs_jsonValue(vertex.z), "vertices[%d]", i * 3 + 2);
		bs_ensureJsonF(&json, bs_jsonValue(normal.x), "normals[%d]", i * 3 + 0);
		bs_ensureJsonF(&json, bs_jsonValue(normal.y), "normals[%d]", i * 3 + 1);
		bs_ensureJsonF(&json, bs_jsonValue(normal.z), "normals[%d]", i * 3 + 2);
		bs_ensureJsonF(&json, bs_jsonValue(indices[i]), "indices[%d]", i);
	}

	char guid[37] = { 0 };
	bs_guidToString(&primitive->guid, guid);
	bs_ensureJson(&json, bs_jsonValue(guid), "guid");
	bs_ensureJson(&json, bs_jsonValueFromStringPointer(bs_charStringF("items/resources/levels/%s/foliage/", bsgfx_fetchLevel(poser()->current_level)->name)), "level");

	return json;
}

static bs_Json bsmod_onLoadType(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	char* plural = strrchr(endpoint->name, '/');
	assert(plural);
	plural++;

	bs_Scope backup = bs_enterSingle();

	bs_except(BSX_FAILED_TO_QUERY);
	int type_id = bsmod_queryType(plural);
	if (bs_caught())
		return bsmod_message("Type is not initialized\n");

	bsmod_loadType(type_id);

	bs_leaveSingle(&backup);

	return bs_emptyJson();
}

static bs_Json bsmod_onSaveType(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	char* plural = strrchr(endpoint->name, '/');
	assert(plural);
	plural++;

	bs_Scope backup = bs_enterSingle();

	bs_except(BSX_FAILED_TO_QUERY);
	int type_id = bsmod_queryType(plural);
	if (bs_caught())
		return bsmod_message("Type is not initialized\n");

	bsmod_saveType(type_id);

	bs_leaveSingle(&backup);

	return bs_emptyJson();
}

static bs_Json bsmod_onLoadModels(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bs_except(BS_ALL_EXCEPTIONS);
	bs_JsonValue array = bs_fetchJson(body, BS_JSON_ARRAY, "$");
	const char* exception = bs_exceptionName(bs_except(0));
	if (exception)
		return bsmod_message(exception);

	bs_Scope scope = bs_enterSingle();
	//bsgfx_loadModels(array.as_array.as_strings, array.size);
	bs_leaveSingle(&scope);

	return bs_emptyJson();
}

static bs_Json bsmod_onLoadScripts(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bs_except(BS_ALL_EXCEPTIONS);
	bs_JsonValue array = bs_fetchJson(body, BS_JSON_ARRAY, "$");
	const char* exception = bs_exceptionName(bs_except(0));
	if (exception)
		return bsmod_message(exception);

	bs_Scope scope = bs_enterSingle();

	bs_JsonValue value = bs_fetchJson(body, BS_JSON_ARRAY, "$");

	for (int i = 0; i < value.size; i++) {
		char* script_name = value.as_array.as_strings[i];
		bsgfx_Script* script = bs_query(bs_fileName(script_name), BS_QUERY_FORCE_CREATE).data;

		bsgfx_respawnScriptEntities(script);
	}

	if (value.size == 0) {
		for (int i = 0; i < bs_resourcesCount(BSGFX_RESOURCE_SCRIPT); i++) {
			bs_Resource* resource = bs_resource(BSGFX_RESOURCE_SCRIPT, i);
			bsgfx_Script* script = resource->data;
			script = bs_query(script->name, BS_QUERY_FORCE_CREATE).data;
			bsgfx_respawnScriptEntities(script);
		}
	}

	//bsgfx_loadScripts(array.as_array.as_strings, array.size);
	bs_leaveSingle(&scope);

	return bs_emptyJson();
}

static bs_Json bsmod_onLoadTextures(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bsmod.queue.load_textures = true;
	return bs_emptyJson();
}

static bs_Json bsmod_onLoadMaterials(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bsmod.queue.load_materials = true;
	return bs_emptyJson();
}

static bs_Json bsmod_onLoadShaders(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bs_JsonValue root = bs_fetchJson(body, BS_JSON_ARRAY, "$");
	for (int i = 0; i < root.size; i++)
		bs_query(root.as_array.as_strings[i], BS_QUERY_FORCE_CREATE | BS_QUERY_ALLOW_NULL);

	bs_free(root.as_array.as_strings);

	//bsgfx_debug.queue.load_shaders = true;
	return bs_emptyJson();
}

static bs_Json bsmod_onLoadAtlases(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bsmod.queue.load_atlas = true;
	return bs_emptyJson();
}

static bs_Json bsmod_onLoadFonts(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	bsmod.queue.load_fonts = true;
	return bs_emptyJson();
}

static bs_Json bsmod_toggleStairBit(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
	for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
		bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);
		if (primitive->collision & BSGFX_COLLISION_STAIRS)
			primitive->collision &= ~BSGFX_COLLISION_STAIRS;
		else
			primitive->collision |= BSGFX_COLLISION_STAIRS;
	}

	//if (bsgfx_primitives_count > 0)
	//	dust->queue_save_primitives = true;

	return bsmod_message("OK");
}

#define BSGFX_INFO(v, f, ...) {						\
	sprintf(format, f, ##__VA_ARGS__);				\
	bs_pushBack(&list, &(struct bsgfx_InfoTable) {		\
		.field = strdup(format),					\
		.value = bs_jsonValue(v),					\
		.reference = &v,							\
	});												\
}

//static bs_Json bsmod_postPrimitive(bs_Endpoint* endpoint, bs_List* params, bs_Json* body) {
//	bsmod_deselectAll();
//	bsgfx_createPrimitive(true);
//	return bs_emptyJson();
//}

// ini

void bsmod_tickLisk() {
	if (bsmod_server.buffer_capacity)
		bs_tickServer(&bsmod_server);
}

#define BSGFX_LISK_DEFAULT "lisk (../../..) !cwd "
void bsmod_lisk(bool async, char* format, ...) {
	va_list args;

	va_start(args, format);
	int len = vsnprintf(NULL, 0, format, args);
	va_end(args);

	char* result = malloc(len + sizeof(BSGFX_LISK_DEFAULT));
	assert(result);
	strcpy(result, BSGFX_LISK_DEFAULT);

	va_start(args, format);
	vsnprintf(result + sizeof(BSGFX_LISK_DEFAULT) - 1, len + 1, format, args);
	va_end(args);

	if (async)
		bs_systemAsync(result);
	else
		bs_system(result);
	free(result);
}

void bsmod_iniLisk() {
	if (bsmod_server.buffer_capacity)
		return;

	bsmod_server = bs_server("http://+:1157/");

	bs_endpoint(&bsmod_server, BS_GET, bsmod_primitive, "primitive");

	bs_endpoint(&bsmod_server, BS_GET, bsmod_onSaveType, "save/tiles");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadType, "load/tiles");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadType, "save/foliages");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadType, "load/foliages");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onSaveType, "save/primitives");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadType, "load/primitives");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onSaveType, "save/meshes");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadType, "load/meshes");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onSaveType, "save/sprites");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadType, "load/sprites");

	bs_endpoint(&bsmod_server, BS_PUT, bsmod_onLoadModels, "load/models");
	bs_endpoint(&bsmod_server, BS_PUT, bsmod_onLoadScripts, "load/scripts");

	bs_endpoint(&bsmod_server, BS_PUT, bsmod_onLoadShaders, "load/shaders");

	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadAtlases, "load/atlases");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadFonts, "load/fonts");

	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadMaterials, "load/materials");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_onLoadTextures, "load/textures");

	bs_endpoint(&bsmod_server, BS_PUT, bsmod_onSaveType, "saveType");
	bs_endpoint(&bsmod_server, BS_GET, bsmod_toggleStairBit, "toggleStairBit");
	//bs_endpoint(&bsmod_server, BS_POST, bsmod_postPrimitive, "primitive");

	bs_infoF("listening for lisk\n");
}

*/