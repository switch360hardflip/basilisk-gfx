#include <basilisk-gfx.h>
#include <bsgfx_cache.h>

#include <inttypes.h>

const char* _bsgfx_material_categories[BSGFX_MATERIAL_CATEGORY_COUNT] = {
    [BSGFX_MATERIAL_CATEGORY_NONE] = NULL,
    [BSGFX_MATERIAL_CATEGORY_UI_COLOR_SCHEME] = "BSMOD UI Color Scheme",
};

bs_List _bsgfx_materials = { .unit_size = sizeof(bsgfx_Material), .increment = 64 };



  /*==============================================================================
   * Materials
   *============================================================================*/

const char* bsgfx_materialCategoryName(bsgfx_MaterialCategory category) {
    return _bsgfx_material_categories[category];
}

bs_List* bsgfx_materials() {
    return &_bsgfx_materials;
}

bsgfx_Material* bsgfx_queryMaterialHash(bs_U64 hash) {
    for (int i = 0; i < _bsgfx_materials.count; i++) {
        bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, i);
        if (material->hash == hash)
            return material;
    }

    return NULL;
}

bsgfx_Material* bsgfx_queryMaterial(const char* name) {
    return bsgfx_queryMaterialHash(bs_stringHash(name));
}

char* bsgfx_materialName(int id) {
    bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, id);
    return material->name;
}

bs_U64 bsgfx_materialHash(int id) {
    bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, id);
    return material->hash;
}

bsgfx_Material* bsgfx_material(char* name, int name_length) {
    bs_Buffer* materials_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_MATERIALS)->buffer;
    bsgfx_MaterialContract* contract = bs_bufferMap(bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_MATERIALS)->buffer);

    bsgfx_Material* existing = bsgfx_queryMaterial(name);

    if (existing)
        return existing;

    return bs_pushBack(&_bsgfx_materials, &(bsgfx_Material) {
        .name = strdup(name),
        .hash = bs_stringHash(name),
        .contract = contract + _bsgfx_materials.count,
        .id = _bsgfx_materials.count,
        .json_index = -1,
    });
}

void bsgfx_allocateMaterials() {
    bs_Result result = bs_buffer(BS_BUFFER(BSGFX_BUFFERS, BSGFX_BUFFER_MATERIALS, false), BSGFX_MAX_MATERIALS_COUNT * sizeof(bsgfx_MaterialContract), 
        BS_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0);

    if (result != BS_RESULT_OK) {
        return;
    }

    bs_Buffer* materials_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_MATERIALS)->buffer;
    bsgfx_MaterialContract* materials = bs_mapBuffer(materials_buffer, BS_U32_MAX);
    bs_bindBuffer(BSGFX_SET_MATERIALS, BSGFX_BINDING_MATERIALS, materials_buffer);

    bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, bsgfx_material(BS_CONSTANT_STRING("blank"))->id);
    material->contract->color = bs_v4(1.0, 1.0, 1.0, 1.0);
}

void bsgfx_loadMaterials() {
    return;

    bs_Json json;
    bs_loadJson(&json, BS_CONSTANT_STRING("resources/materials.json"));

    bs_JsonValue root = bs_fetchJson(&json, BS_JSON_ARRAY, BS_CONSTANT_STRING("$"));
    
    for (int i = 0; i < root.size; i++) {
        bs_Json material_json = bs_jsonRoot(&json, root.as_array.as_objects[i]);

        bs_F64* color = bs_fetchJson(&material_json, BS_JSON_UNDEFINED, BS_CONSTANT_STRING("color[:4]")).as_array.as_numbers;
        bs_F64* tile_color = bs_fetchJson(&material_json, BS_JSON_UNDEFINED, BS_CONSTANT_STRING("tile_color[:4]")).as_array.as_numbers;
        bs_F64* tile_color_add = bs_fetchJson(&material_json, BS_JSON_UNDEFINED, BS_CONSTANT_STRING("tile_color_add[:4]")).as_array.as_numbers;

        int image_size = bs_fetchJson(&material_json, BS_JSON_UNDEFINED, BS_CONSTANT_STRING("image.size")).as_number;

        char* material_name = bs_fetchJson(&material_json, BS_JSON_STRING, BS_CONSTANT_STRING("name")).as_string;

        bsgfx_Material* material = bsgfx_material(material_name, strlen(material_name));
        material->json_index = i;
        material->editable = true;
        *material->contract = (bsgfx_MaterialContract){
            .color = color ? BSGFX_RGBA(color[0], color[1], color[2], color[3]) : (bs_vec4) { 1.0, 1.0, 1.0, 1.0 },
           // .image = image_size ? bsgfx_queryTexture(image_size, bs_fetchJson(&material_json, BS_JSON_STRING, "image.name").as_string) : 0,
           // .scale = image_size,
        };
    }

#ifndef _DEBUG
    // bs_unmapBuffer(materials_buffer);
#endif
    bs_destroyJson(&json);

    bs_infoF("Loaded %d materials\n", _bsgfx_materials.count);
}

// highlighting is kinda scuffed

void bsgfx_highlightMaterial(int material_id, bool auto_unhighlight) {
    bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, material_id);
    if (material->highlighted)
        return;

    bsgfx_MaterialContract* contract = bs_bufferMap(bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_MATERIALS)->buffer);
    contract += material_id;

    bs_v3Add(&contract->color.xyz, &(bs_vec3) { 0.25, 0.25, 0.25 }, &contract->color.xyz);

    material->highlighted = true;
    material->auto_unhighlight = auto_unhighlight;
}

void bsgfx_unhighlightMaterial(int material_id) {
    bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, material_id);
    bsgfx_MaterialContract* contract = bs_bufferMap(bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_MATERIALS)->buffer);
    contract += material_id;

    bs_v3Sub(&contract->color.xyz, &(bs_vec3) { 0.25, 0.25, 0.25 }, & contract->color.xyz);

    material->highlighted = false;
}

void bsgfx_tickMaterials() {
    for (int i = 0; i < _bsgfx_materials.count; i++) {
        bsgfx_Material* material = bs_fetchUnit(&_bsgfx_materials, i);
        if (material->highlighted && material->auto_unhighlight)
            bsgfx_unhighlightMaterial(i);
    }
}