
 /**
  MIT License
  
  Copyright (c) 2026 switch360hardflip <switch360hardflip@gmail.com>
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */ 

#include <bsmod_internal.h>
#include <assert.h>

#include <spirv_cross_c.h>
#include <glslang/Include/glslang_c_interface.h>
#include <glslang/Public/resource_limits_c.h>

typedef struct bsmod_Reflection bsmod_Reflection;
struct bsmod_Reflection {
	spvc_compiler compiler;
	spvc_reflected_resource* data;
	size_t num;
};

static spvc_context _bsmod_compiler_context_;

BSMODAPI void _bsmod_onCompileShader(bsmod_TrackParams params) {
	bs_Result result;

	if (!bs_fileExists(params.path, strlen(params.path)))
		return;

	char* resource_name = bs_fileName(params.path);
	static bs_String* variadic;
	variadic = bs_stringF(variadic, "shaders/%s", resource_name);

	bs_logF("Compiling \"%s\" as \"%s\"\n", params.path, variadic->value);

	bool success = _bsmod_compileShader(params.path, variadic->value, params.package);
	if (success) {

		_bsmod_packBindings();
		_bsmod_updateBindings();
		if (params.compile_references)
			_bsmod_updateShaderReferences();
		//bs_loadBindings(_bsmod_.bsgfx_package, "bindings");

		if (bs_instance()->instance) {
			int package;
			result = bs_loadPackage(params.package, &package);
			if (result != BS_RESULT_OK)
				return;

			bs_shader(package, variadic->value, 0, NULL);
		}
	}

	if (params.compile_references && (bs_fileExtensionIs(params.path, "glsl") || bs_fileExtensionIs(params.path, "h"))) { // header file, compile all references
		bs_Json json;
		result = bs_loadJson(&json, BS_CONSTANT_STRING("project/woc/references.json")); // TODO: remove woc
		if (result != BS_RESULT_OK)
			return;

		bs_JsonValue files = bs_fetchJsonF(&json, BS_JSON_UNDEFINED, "%.*s", strlen(params.path) - 5, params.path);

		if (files.size > 0)
			bs_infoF(BS_PRINT_CYAN "Compiling %d reference(s)\n" BS_PRINT_RESET, files.size);

		for (int i = 0; i < files.size; i++) {
			char* file = files.as_array.as_strings[i];

			_bsmod_onCompileShader((bsmod_TrackParams) {
				.prefix = params.prefix,
				.package = params.package,
				.path = file,
			});
		}
	}
}

static bsmod_Reflection _bsmod_reflectionData(bs_U32* spirv, bs_U32 size, spvc_resource_type type) {
	bsmod_Reflection reflection = { 0 };

	spvc_parsed_ir ir = NULL;
	spvc_resources resources = NULL;

	spvc_context_parse_spirv(_bsmod_compiler_context_, spirv, size, &ir);
	spvc_context_create_compiler(_bsmod_compiler_context_, SPVC_BACKEND_GLSL, ir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP, &reflection.compiler);
	spvc_compiler_create_shader_resources(reflection.compiler, &resources);
	spvc_resources_get_resource_list_for_type(resources, type, &reflection.data, &reflection.num);

	return reflection;
}

static const char* _bsmod_serializeBufferType(spvc_resource_type type) {
	bs_criticalF("%s: Not implemented", __func__);
	return NULL;
	/*
	bs_BindType bind_type;

	switch (type) {
	case SPVC_RESOURCE_TYPE_STORAGE_BUFFER: bind_type = BS_DESCRIPTOR_TYPE_STORAGE_BUFFER; break;
	case SPVC_RESOURCE_TYPE_UNIFORM_BUFFER: bind_type = BS_DESCRIPTOR_TYPE_UNIFORM_BUFFER; break;
	case SPVC_RESOURCE_TYPE_SEPARATE_IMAGE: bind_type = BS_DESCRIPTOR_TYPE_SAMPLED_IMAGE; break;
	case SPVC_RESOURCE_TYPE_SUBPASS_INPUT: bind_type = BS_DESCRIPTOR_TYPE_INPUT_ATTACHMENT; break;
	case SPVC_RESOURCE_TYPE_SEPARATE_SAMPLERS: bind_type = BS_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
	case SPVC_RESOURCE_TYPE_PUSH_CONSTANT: bind_type = BS_DESCRIPTOR_TYPE_PUSH_CONSTANT; break;
	case SPVC_RESOURCE_TYPE_SAMPLED_IMAGE: bind_type = BS_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; break;
	case SPVC_RESOURCE_TYPE_ACCELERATION_STRUCTURE: bind_type = BS_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE; break;
	case SPVC_RESOURCE_TYPE_STORAGE_IMAGE: bind_type = BS_DESCRIPTOR_TYPE_STORAGE_IMAGE; break;
	default:
	//	ls_throw("SPVC buffer type (%d) not supported\n", type);
		return NULL;
	}


	return bs_serializeBindType(bind_type);
	*/
}

static void _bsmod_saveBuffer(bs_Json* json, char* shader_type, const char* name, spvc_resource_type type, bs_U32* spirv, bs_U32 size, bool add_to_bindings) {
	bsmod_Reflection reflection = _bsmod_reflectionData(spirv, size, type);

	static bs_String* path;

	const char* type_string = _bsmod_serializeBufferType(type);
	for (int i = 0; i < reflection.num; i++) {
		spvc_reflected_resource* data = reflection.data + i;

		int set = spvc_compiler_get_decoration(reflection.compiler, data->id, SpvDecorationDescriptorSet);
		int point = spvc_compiler_get_decoration(reflection.compiler, data->id, SpvDecorationBinding);
		int block = spvc_compiler_get_decoration(reflection.compiler, data->id, SpvDecorationUniform);

		path = bs_stringF(path, "%d_%d", set, point);

	//	bs_ensureJsonF(json, bs_jsonObject(json), "%s[%d]", name, i);
		bs_ensureJsonF(json, bs_jsonValue(data->name), "%s[%d].name", name, i);
		bs_ensureJsonF(json, bs_jsonValue(set), "%s[%d].set", name, i);
		bs_ensureJsonF(json, bs_jsonValue(point), "%s[%d].point", name, i);
		bs_ensureJsonF(json, bs_jsonValue(type_string), "%s[%d].type", name, i);

		spvc_type spirv_type = spvc_compiler_get_type_handle(reflection.compiler, data->type_id);
		int count = spvc_type_get_num_array_dimensions(spirv_type);
		if (count == 1 && spvc_type_array_dimension_is_literal(spirv_type, 0)) { // nested arrays not supported atm, idk how to handle
			spvc_constant_id descriptor_count = spvc_type_get_array_dimension(spirv_type, 0);
			bs_ensureJsonF(json, bs_jsonValue(descriptor_count), "%s[%d].count", name, i);

			if (add_to_bindings)
				bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(descriptor_count), "%s.count", path->value);
		}
		else {
			bs_ensureJsonF(json, bs_jsonValue(1), "%s[%d].count", name, i);

			if (add_to_bindings)
				bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(1), "%s.count", path->value);
		}

		size_t struct_size = 0;
		spvc_compiler_get_declared_struct_size(reflection.compiler, spirv_type, &struct_size);
		bs_ensureJsonF(json, bs_jsonValue(struct_size), "%s[%d].size", name, i);

		if (add_to_bindings) {
			double size = bs_fetchJsonF(&_bsmod_.bindings_json, BS_JSON_UNDEFINED, "%s.size", path->value).as_number;
			if (size != 0.0 && size != struct_size) {
				bs_warnF("Existing binding (set: %d, point: %d) differs in size (old: %lld, new: %lld)\n", set, point, (size_t)size, (size_t)struct_size);
				//continue;
			}

			bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(data->name), "%s.name", path->value);
			bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(type_string), "%s.type", path->value);
			bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(struct_size), "%s.size", path->value);
			bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(set), "%s.set", path->value);
			bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(point), "%s.point", path->value);

			bs_JsonValue v = bs_fetchJsonF(&_bsmod_.bindings_json, BS_JSON_UNDEFINED, "%s.stages", path->value);
			bool found = false;
			for (int j = 0; j < v.size; j++) {
				char* stage = v.as_array.as_strings[j];
				if (strcmp(stage, shader_type) == 0) {
					found = true;
					break;
				}
			}

			if (!found)
				bs_ensureJsonF(&_bsmod_.bindings_json, bs_jsonValue(shader_type), "%s.stages[999]", path->value);
		}
	}
}

static void _bsmod_saveAttributes(bs_Json* json, bs_U32* spirv, bs_U32 size) {
	bsmod_Reflection reflection = _bsmod_reflectionData(spirv, size, SPVC_RESOURCE_TYPE_STAGE_INPUT);

	for (int i = 0; i < reflection.num; i++) {
		spvc_reflected_resource* data = reflection.data + i;

		spvc_type type = spvc_compiler_get_type_handle(reflection.compiler, data->base_type_id);
		int num_units = spvc_type_get_vector_size(type);
		int location = spvc_compiler_get_decoration(reflection.compiler, data->id, SpvDecorationLocation);

	//	bs_ensureJsonF(json, bs_jsonObject(json), "attributes[%d]", i);
		bs_ensureJsonF(json, bs_jsonValue(data->name), "attributes[%d].name", i);
		bs_ensureJsonF(json, bs_jsonValue(location), "attributes[%d].location", i);
		bs_ensureJsonF(json, bs_jsonValue(num_units * sizeof(bs_U32)), "attributes[%d].size", i);
	}
}

static bs_Result _bsmod_saveReflection(char* path, char* package, char* name, bs_U32* spirv, bs_U32 size, glslang_stage_t stage) {
	/*
	bs_Result result = BS_RESULT_OK;

	bs_Json json;
	result = bs_json("{}", 2, &json);
	if (result != BS_RESULT_OK)
		return result;

	bs_ensureJsonMutable(&json);

	bs_ShaderType shader_type = 0;
	const char* type_name = NULL;
	switch (stage) {
	case GLSLANG_STAGE_VERTEX:
		_bsmod_saveAttributes(&json, spirv, size);
		shader_type = BS_VERTEX_SHADER;
		break;
	case GLSLANG_STAGE_FRAGMENT: shader_type = BS_FRAGMENT_SHADER; break;
	case GLSLANG_STAGE_COMPUTE: shader_type = BS_COMPUTE_SHADER; break;
	case GLSLANG_STAGE_GEOMETRY: shader_type = BS_GEOMETRY_SHADER; break;
	case GLSLANG_STAGE_RAYGEN: shader_type = BS_RAY_GEN_SHADER; break;
	case GLSLANG_STAGE_ANYHIT: shader_type = BS_ANY_HIT_SHADER; break;
	case GLSLANG_STAGE_INTERSECT: shader_type = BS_INTERSECTION_SHADER; break;
	case GLSLANG_STAGE_CLOSESTHIT: shader_type = BS_CLOSEST_HIT_SHADER; break;
	case GLSLANG_STAGE_MISS: shader_type = BS_MISS_SHADER; break;
	default:
		bs_warnF("Skipping %s, shader stage is unsupported\n", path);
		return BS_RESULT_OK;
	}
	type_name = bs_serializeShaderType(shader_type);

	_bsmod_saveBuffer(&json, type_name, "uniformBuffers", SPVC_RESOURCE_TYPE_UNIFORM_BUFFER, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "storageBuffers", SPVC_RESOURCE_TYPE_STORAGE_BUFFER, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "images", SPVC_RESOURCE_TYPE_SEPARATE_IMAGE, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "samplers", SPVC_RESOURCE_TYPE_SEPARATE_SAMPLERS, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "subpasses", SPVC_RESOURCE_TYPE_SUBPASS_INPUT, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "pushConstants", SPVC_RESOURCE_TYPE_PUSH_CONSTANT, spirv, size, false);
	_bsmod_saveBuffer(&json, type_name, "sampledImages", SPVC_RESOURCE_TYPE_SAMPLED_IMAGE, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "accelerationStructures", SPVC_RESOURCE_TYPE_ACCELERATION_STRUCTURE, spirv, size, true);
	_bsmod_saveBuffer(&json, type_name, "storageImages", SPVC_RESOURCE_TYPE_STORAGE_IMAGE, spirv, size, true);
	*/
	bs_warnF("Custom shader format not implemented yet\n");
	return BS_RESULT_NOT_IMPLEMENTED;
	/*
	size_t out_len = 0;
	char* base64 = NULL;
	if (result = bs_encodeBase64(spirv, size * sizeof(bs_U32), &out_len, false, &base64)) {
		bs_ensureJson(&json, bs_jsonValue(type_name), "type");
		bs_ensureJson(&json, bs_jsonValue(base64), "SPIR-V");
	}
	else {
		bs_destroyJson(&json);
		return result;
	}
	*/

	/*
	char* file_name = bs_fileName(path);
	char* ext = bs_fileExtension(path);
	strncpy(ext, "spv\0", 4);
	bs_saveFileF(spirv, size * sizeof(bs_U32), "raw/%s", file_name);
	strncpy(ext, "json", 4);
	*/

	/*
	if (ls_cmp.stages_to_delete & (1 << stage)) {
		ls_cmp.stages_to_delete &= ~(1 << stage);
		char* name = bs_fileName(path);
		assert(name);
		name[-1] = '\0'; // this is safe
		//if (bs_directoryExists(path))
		//	bs_deleteDirectoryContents(path);
		name[-1] = '/';
	}
	*/
	/*
	char* raw;
	result = bs_saveJson(&json, BS_JSON_PRETTY, &raw);
	if (result != BS_RESULT_OK)
		goto end;

	result = _bsmod_packResource(BS_RESOURCE_SHADER, raw, strlen(raw), package, name, strlen(name));
	bs_free(raw);
	if (result != BS_RESULT_OK)
		goto end;

end:
	bs_destroyJson(&json);

	return result;
	*/
}

static bs_Json _bsmod_shader_references = { 0 };
BSMODAPI void _bsmod_loadShaderReferences() {
	bs_Result result = bs_loadJson(&_bsmod_shader_references, BS_CONSTANT_STRING("project/woc/references.json")); // TODO: remove woc
	if (result == BS_RESULT_OK)
		_bsmod_shader_references = bs_emptyJson();
}

BSMODAPI void _bsmod_updateShaderReferences() {
	bs_Result result;

	char* raw;
	result = bs_saveJson(&_bsmod_shader_references, BS_JSON_PRETTY, &raw);
	if (result != BS_RESULT_OK)
		return;

	result = bs_saveFile(raw, strlen(raw), BS_CONSTANT_STRING("project/woc/references.json")); //TODO: remove woc
	if (result != BS_RESULT_OK)
		return;
}

static void _bsmod_writeReferences(char* path, bs_String* glsl) {
	bs_String* include_path = bs_string(NULL, "", 0);
#define INCLUDE_SEARCH "#include "
	char* include = glsl->value;
	int length = glsl->len;
	for (int i = 0 ;; i++) {
		include = bs_memmem(include, length, INCLUDE_SEARCH, sizeof(INCLUDE_SEARCH) - 1);
		if (!include) break;

		// i mean these asserts shouldnt fire cause howsit gonna compile the shaderthing but yea
		include = strchr(include, '\"');
		assert(include);

		char* end = strchr(++include, '\"');
		assert(end);
		int len = end - include;

		include_path = bs_string(include_path, include, len);
		char* ext = bs_fileExtension(include_path->value);
		assert(ext);
		int name_len = (ext - 1) - include_path->value;

		if (memchr(include_path->value, '.', name_len))
			goto next;

		bs_JsonValue existing = bs_fetchJsonF(&_bsmod_shader_references, BS_JSON_UNDEFINED, "%.*s", name_len, include_path->value);
		int index = 99999;
		for (int j = 0; j < existing.size; j++) {
			if (strcmp(existing.as_array.as_strings[j], path) == 0)
				goto next;
		}

		bs_ensureJsonF(&_bsmod_shader_references, bs_jsonValue(path), "%.*s[%d]", name_len, include_path->value, index);

	next:
		include = strchr(end, '\n');
		length = strlen(include);
	}
	bs_free(include_path);
}

BSMODAPI bs_Result _bsmod_compileShader(char* path, char* name, char* package) {
	bs_Result result = BS_RESULT_OK;

	bs_String* glsl; 
	result = bs_loadFile(&glsl, path, strlen(path));
	if (result != BS_RESULT_OK)
		return result;

	char* ext = bs_fileExtension(path);
	//char* name = bs_fileName(path);
	
	_bsmod_writeReferences(path, glsl);

	glslang_stage_t stage = 0;
	if (strcmp(ext, "comp") == 0) stage = GLSLANG_STAGE_COMPUTE;
	else if (strcmp(ext, "geom") == 0) stage = GLSLANG_STAGE_GEOMETRY;
	else if (strcmp(ext, "tesc") == 0) stage = GLSLANG_STAGE_TESSCONTROL;
	else if (strcmp(ext, "tese") == 0) stage = GLSLANG_STAGE_TESSEVALUATION;
	else if (strcmp(ext, "vert") == 0) stage = GLSLANG_STAGE_VERTEX;
	else if (strcmp(ext, "frag") == 0) stage = GLSLANG_STAGE_FRAGMENT;
	else if (strcmp(ext, "rgen") == 0) stage = GLSLANG_STAGE_RAYGEN;
	else if (strcmp(ext, "rmiss") == 0) stage = GLSLANG_STAGE_MISS;
	else return BS_RESULT_GENERAL_ERROR;

	const glslang_input_t input = {
		.language = GLSLANG_SOURCE_GLSL,
		.stage = stage,
		.client = GLSLANG_CLIENT_VULKAN,
		.client_version = GLSLANG_TARGET_VULKAN_1_3,
		.target_language = GLSLANG_TARGET_SPV,
		.target_language_version = GLSLANG_TARGET_SPV_1_5,
		.code = glsl->value,
		.default_version = 450,
		.default_profile = GLSLANG_NO_PROFILE,
		.force_default_version_and_profile = false,
		.forward_compatible = false,
		.messages = GLSLANG_MSG_DEFAULT_BIT,
		.resource = glslang_default_resource(),
	};

	glslang_shader_t* shader = glslang_shader_create(&input);
	glslang_program_t* program = NULL;
	bs_U32* spirv = NULL;
	if (!glslang_shader_preprocess(shader, &input)) {
		bs_warnF("Failed to preprocess \"%s\"\n%s\n%s\n", path, glslang_shader_get_info_log(shader), glslang_shader_get_info_debug_log(shader)); // TODO: bsmod warn
		goto destroy;
	}

	if (!glslang_shader_parse(shader, &input)) {
		bs_warnF("Failed to parse \"%s\"\n%s\n%s\n", path, glslang_shader_get_info_log(shader), glslang_shader_get_info_debug_log(shader)); // TODO: bsmod warn
		goto destroy;
	}

	program = glslang_program_create();
	glslang_program_add_shader(program, shader);

	if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT)) {
		bs_warnF("Failed to link \"%s\"\n%s\n%s\n", path, glslang_shader_get_info_log(shader), glslang_shader_get_info_debug_log(shader)); // TODO: bsmod warn
		goto destroy;
	}

	glslang_spv_options_t options = {
		.generate_debug_info = true,
		.disable_optimizer = true,
	};
	glslang_program_SPIRV_generate_with_options(
		program,
		stage,
		&options
	);

	// glslang_program_SPIRV_generate(program, stage);
	bs_U64 size = glslang_program_SPIRV_get_size(program);
	spirv = malloc(size * sizeof(bs_U32));
	glslang_program_SPIRV_get(program, spirv);
	const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
	if (spirv_messages) bs_warnF("%s\b", spirv_messages);
	//ls_printBackground("  %s\n", result->value);
	//ls_Register* rand = ls_ensureRegister('?');

	result = _bsmod_saveReflection(path, package, name, spirv, size, stage);

destroy:
	if (glsl)
		bs_free(glsl);
//if (result)
//	bs_free(result);

	if (program)
		glslang_program_delete(program);
	if (shader)
		glslang_shader_delete(shader);
	if (spirv)
		free(spirv);
	return result;
}

static void ls_reflectionError(void* param, const char* error) {
	if (strcmp(error, "Declared struct in block cannot be empty.") == 0)
		return;

	bs_warnF("Reflection error: %s\n", error); // TODO: bsmod warn

	//ls_throw("%s\n", error);
}

typedef struct {
	char* name;
	char* source;
	const char* type;
	int size;
	int set;
	int point;
	bs_U32 shader_stages;
} ls_Binding;

static void ls_readBuffer(bs_Json* root, bs_List* bindings, char* path, const char* name) {
	bs_criticalF("%s: Not implemented", __func__);
	/*
	bs_JsonValue objects = bs_fetchJson(root, BS_JSON_UNDEFINED, name, strlen(name));

	if (!objects.found) 
		return;

	char* shader_type_string = bs_fetchJson(root, BS_JSON_STRING, BS_CONSTANT_STRING("type")).as_string;
	bs_ShaderType shader_type = bs_deserializeShaderType(shader_type_string);

	for (int i = 0; i < objects.size; i++) {
		bs_Json object = bs_jsonRoot(root, objects.as_array.as_objects[i]);

		char* name = bs_fetchJson(&object, BS_JSON_STRING, BS_CONSTANT_STRING("name")).as_string;
		bs_BindType type = bs_deserializeBindType(bs_fetchJson(&object, BS_JSON_STRING, BS_CONSTANT_STRING("type")).as_string);
		int set = bs_fetchJson(&object, BS_JSON_NUMBER, BS_CONSTANT_STRING("set")).as_number;
		int point = bs_fetchJson(&object, BS_JSON_NUMBER, BS_CONSTANT_STRING("point")).as_number;
		int size = bs_fetchJson(&object, BS_JSON_NUMBER, BS_CONSTANT_STRING("size")).as_number;

		if (type == BS_BIND_TYPE_PUSH_CONSTANT) {
			continue;
		}

		ls_Binding* existing = NULL;
		for (int j = 0; j < bindings->count; j++) {
			ls_Binding* binding = bs_fetchUnit(bindings, j);
			if (binding->set == set && binding->point == point) {
				existing = binding;
			}
		}

		if (!existing) {
			existing = bs_pushBack(bindings, &(ls_Binding) {
				.set = set,
					.point = point,
					.type = bs_serializeBindType(type),
			});
		}
		else {
			if (existing->size != size && existing->size != 0) {
				bs_warnF("Size mismatch in binding \"%s\" (%d, %d) between\n \"%s\" (%d) and\n \"%s\" (%d)\nPicking the larger size (%d)\n", name, set, point, path, size, existing->source, existing->size, size);
			}

			free(existing->name);
			free(existing->source);
		}

		existing->shader_stages |= shader_type;
		existing->name = strdup(name);
		existing->source = strdup(path);

		// bs_Binding* binding = bs_queryBinding(bs_queryBindSet(set), point);
		// if (binding) {
		//    // binding->stages |= shader->type;
		//  //   bs_pushDescriptorPools();
		// }
	}

	bs_free(objects.as_array.as_objects);
	*/
}

static void _bsmod_readShaderBindings(char* path, bs_List* bindings) {
	bs_Json metadata;
	bs_loadJson(&metadata, path, strlen(path));

	ls_readBuffer(&metadata, bindings, path, "$.uniformBuffers");
	ls_readBuffer(&metadata, bindings, path, "$.storageBuffers");
	ls_readBuffer(&metadata, bindings, path, "$.images");
	ls_readBuffer(&metadata, bindings, path, "$.samplers");
	ls_readBuffer(&metadata, bindings, path, "$.sampledImages");
	ls_readBuffer(&metadata, bindings, path, "$.subpasses");
	ls_readBuffer(&metadata, bindings, path, "$.pushConstants");
	ls_readBuffer(&metadata, bindings, path, "$.accelerationStructures");
	ls_readBuffer(&metadata, bindings, path, "$.storageImages");

	bs_destroyJson(&metadata);
}

BSMODAPI void _bsmod_updateBindings() {
	/*
	bs_String* bindings_path = bs_stringF(NULL, BSMOD_COMPILE_OUTPUT_PATH "/bindings.json");

	// todo correct filenames
	bs_List bindings = bs_list(sizeof(ls_Binding), 32);
	bs_except(BSX_NOT_FOUND);
	bs_foreachFile(_bsmod_readShaderBindings, &bindings, "resources/shaders/vert");
	bs_except(BSX_NOT_FOUND);
	bs_foreachFile(_bsmod_readShaderBindings, &bindings, "resources/shaders/geom");
	bs_except(BSX_NOT_FOUND);
	bs_foreachFile(_bsmod_readShaderBindings, &bindings, "resources/shaders/frag");
	bs_except(BSX_NOT_FOUND);
	bs_foreachFile(_bsmod_readShaderBindings, &bindings, "resources/shaders/comp");
	bs_except(BSX_NOT_FOUND);
	bs_foreachFile(_bsmod_readShaderBindings, &bindings, "resources/shaders/rgen");
	bs_except(BSX_NOT_FOUND);
	bs_foreachFile(_bsmod_readShaderBindings, &bindings, "resources/shaders/rmiss");
	bs_caught();

	bs_Json json = bs_emptyJsonArray();
	for (int i = 0; i < bindings.count; i++) {
		ls_Binding* binding = bs_fetchUnit(&bindings, i);

		bs_ensureJsonF(&json, bs_jsonValue(binding->name), "$[%d].name", i);
		bs_ensureJsonF(&json, bs_jsonValue(binding->type), "$[%d].type", i);
		bs_ensureJsonF(&json, bs_jsonValue(binding->size), "$[%d].size", i);
		bs_ensureJsonF(&json, bs_jsonValue(binding->set), "$[%d].set", i);
		bs_ensureJsonF(&json, bs_jsonValue(binding->point), "$[%d].point", i);

		for (int j = 0; j < 32; j++) {
			if (binding->shader_stages & (1 << j))
				bs_ensureJsonF(&json, bs_jsonValue(bs_serializeShaderType(1 << j)), "$[%d].stages[999]", i);
		}
	}

	char* raw = bs_saveJson(&json, BS_JSON_PRETTY);
	bs_saveFile(bindings_path->value, raw, strlen(raw));
	bs_free(raw);
	bs_free(bindings_path);
	*/
}

BSMODAPI void _bsmod_iniCompiler() {
	spvc_context_create(&_bsmod_compiler_context_);
	spvc_context_set_error_callback(_bsmod_compiler_context_, ls_reflectionError, NULL);
	glslang_initialize_process();
}

BSMODAPI void _bsmod_destroyCompiler() {
	glslang_finalize_process();
	spvc_context_destroy(_bsmod_compiler_context_);
}