
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

 /**
  This file was generated from basilisk-gfx.com

  It is not recommended to make changes to this file as it will be lost if
  the code is regenerated.
  */
        
#ifndef BSGFX_INTERNAL_GEN_H
#define BSGFX_INTERNAL_GEN_H

#include <basilisk-core.h>
#include <basilisk-gfx.h>
#include <windows.h>


typedef bsgfx_Scene*(__stdcall* PFN_bsgfx_currentScene)();
typedef void(__stdcall* PFN_bsgfx_loadScene)(const char* name);
typedef int(__stdcall* PFN_bsgfx_images)();
typedef int(__stdcall* PFN_bsgfx_samplers)();
typedef int(__stdcall* PFN_bsgfx_buffers)();
typedef int(__stdcall* PFN_bsgfx_batches)();
typedef int(__stdcall* PFN_bsgfx_renderers)();
typedef int(__stdcall* PFN_bsgfx_queues)();
typedef int(__stdcall* PFN_bsgfx_rayTracers)();
typedef int(__stdcall* PFN_bsgfx_atlases)();
typedef int(__stdcall* PFN_bsgfx_fonts)();
typedef bool(__stdcall* PFN_bsgfx_validateSubtype)(const char* library_name, int subtype);
typedef bool(__stdcall* PFN_bsgfx_validateInstanceType)(const char* library_name, int instance_type_id);
typedef const char*(__stdcall* PFN_bsgfx_materialCategoryName)(bsgfx_MaterialCategory category);
typedef bs_List*(__stdcall* PFN_bsgfx_materials)();
typedef bsgfx_Material*(__stdcall* PFN_bsgfx_queryMaterialHash)(bs_U64 hash);
typedef bsgfx_Material*(__stdcall* PFN_bsgfx_fetchMaterial)(int id);
typedef bsgfx_Material*(__stdcall* PFN_bsgfx_queryMaterial)(const char* name);
typedef void(__stdcall* PFN_bsgfx_loadMaterials)();
typedef bsgfx_Material*(__stdcall* PFN_bsgfx_material)(char* name, int name_length);
typedef void(__stdcall* PFN_bsgfx_highlightMaterial)(int material_id, bool auto_unhighlight);
typedef void(__stdcall* PFN_bsgfx_unhighlightMaterial)(int material_id);
typedef void(__stdcall* PFN_bsgfx_tickMaterials)();
typedef void(__stdcall* PFN_bsgfx_shadowPipe)();
typedef void(__stdcall* PFN_bsgfx_pipeline)();
typedef void(__stdcall* PFN_bsgfx_sweepCollisions)(float sweep_radius, const bs_vec3* position);
typedef bsgfx_Collider(__stdcall* PFN_bsgfx_collider)(bs_Aabb aabb, bs_vec3 scale, bs_ivec3 resolution, float sweep_radius);
typedef void(__stdcall* PFN_bsgfx_applyCollisions)(bsgfx_Collider* collider, bs_vec3 position, bs_vec3* velocity);
typedef void(__stdcall* PFN_bsgfx_printCollisions)(bsgfx_Collider* collider);
typedef void(__stdcall* PFN_bsgfx_instanceSweepCollisions)();
typedef void(__stdcall* PFN_bsgfx_instanceCollider)(bsgfx_Collider* collider, bs_vec3 position, bs_vec3* velocity);
typedef int(__stdcall* PFN_bsgfx_skeleton)(bs_Armature* armature);
typedef int(__stdcall* PFN_bsgfx_animationFrame)(bs_Animation* animation, float time, int num_frames);
typedef int(__stdcall* PFN_bsgfx_animationFrameCount)(bs_Animation* animation, float time_scale);
typedef void(__stdcall* PFN_bsgfx_applyAnimationVelocity)(bsgfx_Animator* animator, bs_vec3* velocity, bs_vec2 input);
typedef void(__stdcall* PFN_bsgfx_queueAnimation)(bsgfx_Animator* animator, int animation_id);
typedef void(__stdcall* PFN_bsgfx_runAnimator)(bsgfx_Animator* animator, bsgfx_AnimatorCallbacks callbacks);
typedef bsgfx_Animator(__stdcall* PFN_bsgfx_animator)(bs_Armature* armature, int resting_animation_id, int animations_count);
typedef bs_List*(__stdcall* PFN_bsgfx_subtypeInstances)(int subtype);
typedef bs_Result(__stdcall* PFN_bsgfx_iniInstances)();
typedef void(__stdcall* PFN_bsgfx_instanceType)(int type, int max_instance_count, int bind_set, int binding);
typedef bs_Range(__stdcall* PFN_bsgfx_subtypeRange)(int subtype);
typedef void(__stdcall* PFN_bsgfx_deleteSubtype)(int subtype);
typedef int(__stdcall* PFN_bsgfx_instanceCount)(int subtype);
typedef int(__stdcall* PFN_bsgfx_subtypeCount)(int instance_type_id);
typedef const int*(__stdcall* PFN_bsgfx_subtypes)();
typedef int(__stdcall* PFN_bsgfx_subtype)(int type, bs_Batch* batch, bs_U32 flags, bs_Range range);
typedef int(__stdcall* PFN_bsgfx_instance)(int subtype, const char* data, int data_size, bs_U32 flags, unsigned int bone_index, int id, int material);
typedef void(__stdcall* PFN_bsgfx_tickInstances)();
typedef bool(__stdcall* PFN_bsgfx_subtypeHasFlag)(int subtype, bs_U32 flag);
typedef void(__stdcall* PFN_bsgfx_renderSubtype)(int subtype, bs_Pipeline* pipeline);
typedef void(__stdcall* PFN_bsgfx_resetInstances)();
typedef void(__stdcall* PFN_bsgfx_resetSubtype)(int subtype);
typedef void(__stdcall* PFN_bsgfx_instanceHiResMesh)(bs_Mesh* mesh, const bs_vec3* position, const bs_vec4* rotation, float scale, int subtype_offset, bool origin_at_center);
typedef int(__stdcall* PFN_bsgfx_instanceMesh)(int subtype, const bsgfx_MeshInstance* data, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instanceBoneMesh)(int subtype, const bsgfx_BoneInstance* data, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instanceDepthlessLine)(bs_vec3 start, bs_vec3 end, bs_RGBA color);
typedef int(__stdcall* PFN_bsgfx_instanceLine)(bs_vec3 start, bs_vec3 end, bs_RGBA color);
typedef int(__stdcall* PFN_bsgfx_instanceRay)(const bs_Ray* ray, bs_RGBA color);
typedef void(__stdcall* PFN_bsgfx_instanceAabb)(const bs_Aabb* aabb, bs_RGBA color, bs_Range* out);
typedef int(__stdcall* PFN_bsgfx_instanceSphere)(bs_vec3 position, float radius);
typedef int(__stdcall* PFN_bsgfx_instanceCone)(bs_mat4 transform, float radius, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instancePoint)(bs_vec3 position, bs_RGBA color, float size);
typedef int(__stdcall* PFN_bsgfx_instanceQuad)(int subtype, bs_mat4x3 transform, bs_vec4 coords, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instanceAtlas)(int subtype, bs_mat4x3 transform, int texture, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instanceAtlasFlipped)(int subtype, bs_mat4x3 transform, int texture, bs_U32 flags, int id, int material);
typedef void(__stdcall* PFN_bsgfx_instanceText)(int subtype, bs_Font* font, bsgfx_Text* params, bs_vec2* out_text_size, char* value, int value_length);
typedef bs_mat4x3(__stdcall* PFN_bsgfx_matrix)(bs_vec3 position, bs_vec3 scale);
typedef void(__stdcall* PFN_bsgfx_renderFontSubtypes)();
typedef void(__stdcall* PFN_bsgfx_settingsEditor)(bs_List* result);
typedef void(__stdcall* PFN_bsgfx_renderFineShadowVolumes)();
typedef void(__stdcall* PFN_bsgfx_renderShadowVolumes)();
typedef void(__stdcall* PFN_bsgfx_computeShadowVolumes)();
typedef bs_Model*(__stdcall* PFN_bsgfx_prefabModel)();
typedef int(__stdcall* PFN_bsgfx_package)();
typedef void(__stdcall* PFN_bsgfx_ini)(const char* name, bs_U32 width, bs_U32 height, int argc, char* argv[]);
typedef HINSTANCE(__stdcall* PFN_bsgfx_bsmodDll)();
typedef void(__stdcall* PFN_bsgfx_checkGFSDK)(bs_U32 result);
typedef void(__stdcall* PFN_bsgfx_logGFSDK)(bs_U32 result);
typedef bsgfx_Application*(__stdcall* PFN_bsgfx_app)();
typedef bsgfx_Callbacks*(__stdcall* PFN_bsgfx_callbacks)();
typedef bsgfx_Settings*(__stdcall* PFN_bsgfx_settings)();
typedef struct Poser*(__stdcall* PFN_poser)();
typedef void(__stdcall* PFN_bsgfx_setCamera)(const bs_mat4* proj, const bs_mat4* view);
typedef void(__stdcall* PFN_bsgfx_type)(bsgfx_TypeId id, int package_id, int version, const char* plural, const char* singular, size_t unmapped_size, size_t mapped_size, PFN_bsgfx_TypeMapper mapper, size_t unmapped_flexible_offset, size_t mapped_flexible_offset, size_t unmapped_flexible_size, size_t mapped_flexible_size);
typedef void(__stdcall* PFN_bsgfx_map)(bsgfx_TypeId type, int id);
typedef void(__stdcall* PFN_bsgfx_remap)(bsgfx_TypeId type_id);
typedef int(__stdcall* PFN_bsgfx_count)(bsgfx_TypeId type_id);
typedef void*(__stdcall* PFN_bsgfx_get)(bsgfx_TypeId type_id, bs_U32 id);
typedef bsgfx_Type*(__stdcall* PFN_bsgfx_getType)(bsgfx_TypeId type_id);
typedef int(__stdcall* PFN_bsgfx_id)(bsgfx_TypeId type_id, unsigned char* p);
typedef int(__stdcall* PFN_bsgfx_rawId)(bsgfx_TypeId type_id, unsigned char* p);
typedef void*(__stdcall* PFN_bsgfx_getRaw)(bsgfx_TypeId type_id, int id);
typedef int(__stdcall* PFN_bsgfx_flexibleCount)(bsgfx_TypeId type_id, int id);
typedef void*(__stdcall* PFN_bsmod_add)(bsgfx_TypeId id, void* data);
typedef bsgfx_TypeId(__stdcall* PFN_bsmod_queryType)(const char* plural);
typedef void(__stdcall* PFN_bsgfx_loadFoliages)(int package_id);
typedef int(__stdcall* PFN_bsgfx_queryFoliage)(bs_GUID* guid);
typedef void(__stdcall* PFN_bsgfx_loadLights)(int package_id);
typedef void(__stdcall* PFN_bsgfx_computePrefabShadows)();
typedef void(__stdcall* PFN_bsgfx_renderPrefabShadowVolumes)();
typedef bsgfx_PrefabMetadata*(__stdcall* PFN_bsgfx_prefabMetadata)(int mesh_id);
typedef void(__stdcall* PFN_bsgfx_savePrefabs)();
typedef void(__stdcall* PFN_bsgfx_loadPrefabs)(int package_id, bs_Model* model);
typedef void(__stdcall* PFN_bsgfx_prefabTransform)(bsgfx_Prefab* prefab, bs_mat4* out);
typedef int(__stdcall* PFN_bsgfx_instancePrefabModel)(int mesh_id, bs_mat4 transform, bsgfx_PrefabSubtype prefab_subtype, int material_id);
typedef int(__stdcall* PFN_bsgfx_instancePrefab)(int id, bsgfx_PrefabSubtype prefab_subtype);
typedef void(__stdcall* PFN_bsgfx_instancePrefabs)();
typedef void(__stdcall* PFN_bsgfx_renderPrefabs)(bs_Pipeline* pipeline, int key_start);
typedef void(__stdcall* PFN_bsgfx_renderScenePrefabs)();
typedef void(__stdcall* PFN_bsgfx_renderPrefabPrimitives)(bs_Pipeline* pipeline, int key_start);
typedef bsgfx_RawPrefab*(__stdcall* PFN_bsgfx_tilePrefab)(bs_vec2 coords);
typedef int(__stdcall* PFN_bsgfx_queryPrefabId)(const bs_GUID* guid);
typedef int(__stdcall* PFN_bsgfx_closestPrefab)(bs_U64 mesh_name_hash, bs_vec3 position, float radius);
typedef void(__stdcall* PFN_bsgfx_primitivePosition)(const bsgfx_RawPrimitive* primitive, bs_vec3* out);
typedef void(__stdcall* PFN_bsgfx_loadPrimitives)(int package_id);
typedef int(__stdcall* PFN_bsgfx_primitiveSubtype)(bsgfx_PrimitiveType type);
typedef int(__stdcall* PFN_bsgfx_instancePrimitive)(int subtype, bs_mat4 transform, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_queryTilePrimitive)(int tile_id);
typedef void(__stdcall* PFN_bsgfx_instancePrimitives)();
typedef void(__stdcall* PFN_bsgfx_renderPrimitives)(bs_mat4 camera);
typedef int(__stdcall* PFN_bsgfx_queryPrimitive)(bs_GUID* guid);
typedef void(__stdcall* PFN_bsgfx_loadSpawners)(int package_id);
typedef void(__stdcall* PFN_bsgfx_spawn)(bsgfx_Spawner* spawner);
typedef void(__stdcall* PFN_bsgfx_instanceSpawners)();
typedef void(__stdcall* PFN_bsgfx_loadTiles)(int package_id, bool force_destroy);
typedef void(__stdcall* PFN_bsgfx_instanceTiles)();
typedef void(__stdcall* PFN_bsgfx_pushTile)(const bs_Batch* batch, bs_Quad quad, bs_vec3 normal, bs_U32 index, int image_index, bs_Range* out_range);
typedef void(__stdcall* PFN_bsgfx_batchTile)(const bs_Batch* batch, const bs_U32* offset, bs_Quad quad, bs_vec3 normal, bs_U32 index, int image_index);
typedef const bsgfx_TileAxis*(__stdcall* PFN_bsgfx_tileAxes)();
typedef void(__stdcall* PFN_bsgfx_tilePosition)(const bsgfx_Primitive* primitive, int axis, int x, int y, bs_vec3* out);
typedef void(__stdcall* PFN_bsgfx_tileRotation)(int axis, bs_vec4* out);
typedef void(__stdcall* PFN_bsgfx_tileEulerRotation)(int axis, bs_vec3* out);
typedef void(__stdcall* PFN_bsgfx_pushTileAt)(bs_Batch* batch, bsgfx_Primitive* primitive, int axis, int x, int y, bs_U32 index, int image_index, bs_U32* out);
typedef void(__stdcall* PFN_bsgfx_tileCoordinate)(bsgfx_Primitive* primitive, int axis, int index, bs_ivec2* out);
typedef void(__stdcall* PFN_bsgfx_tileAxis)(const bsgfx_Primitive* primitive, int index, int* out);
typedef void(__stdcall* PFN_bsgfx_tileIndex)(const bsgfx_Primitive* primitive, int axis, int x, int y, bs_U32* out);
typedef bool(__stdcall* PFN_bsgfx_instanceWidgets)(bsgfx_Menu menu, bsgfx_TitleBar* title_bar, bsgfx_MenuTabBar* tab_bar);
typedef void(__stdcall* PFN_bsgfx_renderColorPickers)();

typedef struct {
    PFN_bsgfx_currentScene bsgfx_currentScene;
    PFN_bsgfx_loadScene bsgfx_loadScene;
    PFN_bsgfx_images bsgfx_images;
    PFN_bsgfx_samplers bsgfx_samplers;
    PFN_bsgfx_buffers bsgfx_buffers;
    PFN_bsgfx_batches bsgfx_batches;
    PFN_bsgfx_renderers bsgfx_renderers;
    PFN_bsgfx_queues bsgfx_queues;
    PFN_bsgfx_rayTracers bsgfx_rayTracers;
    PFN_bsgfx_atlases bsgfx_atlases;
    PFN_bsgfx_fonts bsgfx_fonts;
    PFN_bsgfx_validateSubtype bsgfx_validateSubtype;
    PFN_bsgfx_validateInstanceType bsgfx_validateInstanceType;
    PFN_bsgfx_materialCategoryName bsgfx_materialCategoryName;
    PFN_bsgfx_materials bsgfx_materials;
    PFN_bsgfx_queryMaterialHash bsgfx_queryMaterialHash;
    PFN_bsgfx_fetchMaterial bsgfx_fetchMaterial;
    PFN_bsgfx_queryMaterial bsgfx_queryMaterial;
    PFN_bsgfx_loadMaterials bsgfx_loadMaterials;
    PFN_bsgfx_material bsgfx_material;
    PFN_bsgfx_highlightMaterial bsgfx_highlightMaterial;
    PFN_bsgfx_unhighlightMaterial bsgfx_unhighlightMaterial;
    PFN_bsgfx_tickMaterials bsgfx_tickMaterials;
    PFN_bsgfx_shadowPipe bsgfx_shadowPipe;
    PFN_bsgfx_pipeline bsgfx_pipeline;
    PFN_bsgfx_sweepCollisions bsgfx_sweepCollisions;
    PFN_bsgfx_collider bsgfx_collider;
    PFN_bsgfx_applyCollisions bsgfx_applyCollisions;
    PFN_bsgfx_printCollisions bsgfx_printCollisions;
    PFN_bsgfx_instanceSweepCollisions bsgfx_instanceSweepCollisions;
    PFN_bsgfx_instanceCollider bsgfx_instanceCollider;
    PFN_bsgfx_skeleton bsgfx_skeleton;
    PFN_bsgfx_animationFrame bsgfx_animationFrame;
    PFN_bsgfx_animationFrameCount bsgfx_animationFrameCount;
    PFN_bsgfx_applyAnimationVelocity bsgfx_applyAnimationVelocity;
    PFN_bsgfx_queueAnimation bsgfx_queueAnimation;
    PFN_bsgfx_runAnimator bsgfx_runAnimator;
    PFN_bsgfx_animator bsgfx_animator;
    PFN_bsgfx_subtypeInstances bsgfx_subtypeInstances;
    PFN_bsgfx_iniInstances bsgfx_iniInstances;
    PFN_bsgfx_instanceType bsgfx_instanceType;
    PFN_bsgfx_subtypeRange bsgfx_subtypeRange;
    PFN_bsgfx_deleteSubtype bsgfx_deleteSubtype;
    PFN_bsgfx_instanceCount bsgfx_instanceCount;
    PFN_bsgfx_subtypeCount bsgfx_subtypeCount;
    PFN_bsgfx_subtypes bsgfx_subtypes;
    PFN_bsgfx_subtype bsgfx_subtype;
    PFN_bsgfx_instance bsgfx_instance;
    PFN_bsgfx_tickInstances bsgfx_tickInstances;
    PFN_bsgfx_subtypeHasFlag bsgfx_subtypeHasFlag;
    PFN_bsgfx_renderSubtype bsgfx_renderSubtype;
    PFN_bsgfx_resetInstances bsgfx_resetInstances;
    PFN_bsgfx_resetSubtype bsgfx_resetSubtype;
    PFN_bsgfx_instanceHiResMesh bsgfx_instanceHiResMesh;
    PFN_bsgfx_instanceMesh bsgfx_instanceMesh;
    PFN_bsgfx_instanceBoneMesh bsgfx_instanceBoneMesh;
    PFN_bsgfx_instanceDepthlessLine bsgfx_instanceDepthlessLine;
    PFN_bsgfx_instanceLine bsgfx_instanceLine;
    PFN_bsgfx_instanceRay bsgfx_instanceRay;
    PFN_bsgfx_instanceAabb bsgfx_instanceAabb;
    PFN_bsgfx_instanceSphere bsgfx_instanceSphere;
    PFN_bsgfx_instanceCone bsgfx_instanceCone;
    PFN_bsgfx_instancePoint bsgfx_instancePoint;
    PFN_bsgfx_instanceQuad bsgfx_instanceQuad;
    PFN_bsgfx_instanceAtlas bsgfx_instanceAtlas;
    PFN_bsgfx_instanceAtlasFlipped bsgfx_instanceAtlasFlipped;
    PFN_bsgfx_instanceText bsgfx_instanceText;
    PFN_bsgfx_matrix bsgfx_matrix;
    PFN_bsgfx_renderFontSubtypes bsgfx_renderFontSubtypes;
    PFN_bsgfx_settingsEditor bsgfx_settingsEditor;
    PFN_bsgfx_renderFineShadowVolumes bsgfx_renderFineShadowVolumes;
    PFN_bsgfx_renderShadowVolumes bsgfx_renderShadowVolumes;
    PFN_bsgfx_computeShadowVolumes bsgfx_computeShadowVolumes;
    PFN_bsgfx_prefabModel bsgfx_prefabModel;
    PFN_bsgfx_package bsgfx_package;
    PFN_bsgfx_ini bsgfx_ini;
    PFN_bsgfx_bsmodDll bsgfx_bsmodDll;
    PFN_bsgfx_checkGFSDK bsgfx_checkGFSDK;
    PFN_bsgfx_logGFSDK bsgfx_logGFSDK;
    PFN_bsgfx_app bsgfx_app;
    PFN_bsgfx_callbacks bsgfx_callbacks;
    PFN_bsgfx_settings bsgfx_settings;
    PFN_poser poser;
    PFN_bsgfx_setCamera bsgfx_setCamera;
    PFN_bsgfx_type bsgfx_type;
    PFN_bsgfx_map bsgfx_map;
    PFN_bsgfx_remap bsgfx_remap;
    PFN_bsgfx_count bsgfx_count;
    PFN_bsgfx_get bsgfx_get;
    PFN_bsgfx_getType bsgfx_getType;
    PFN_bsgfx_id bsgfx_id;
    PFN_bsgfx_rawId bsgfx_rawId;
    PFN_bsgfx_getRaw bsgfx_getRaw;
    PFN_bsgfx_flexibleCount bsgfx_flexibleCount;
    PFN_bsmod_add bsmod_add;
    PFN_bsmod_queryType bsmod_queryType;
    PFN_bsgfx_loadFoliages bsgfx_loadFoliages;
    PFN_bsgfx_queryFoliage bsgfx_queryFoliage;
    PFN_bsgfx_loadLights bsgfx_loadLights;
    PFN_bsgfx_computePrefabShadows bsgfx_computePrefabShadows;
    PFN_bsgfx_renderPrefabShadowVolumes bsgfx_renderPrefabShadowVolumes;
    PFN_bsgfx_prefabMetadata bsgfx_prefabMetadata;
    PFN_bsgfx_savePrefabs bsgfx_savePrefabs;
    PFN_bsgfx_loadPrefabs bsgfx_loadPrefabs;
    PFN_bsgfx_prefabTransform bsgfx_prefabTransform;
    PFN_bsgfx_instancePrefabModel bsgfx_instancePrefabModel;
    PFN_bsgfx_instancePrefab bsgfx_instancePrefab;
    PFN_bsgfx_instancePrefabs bsgfx_instancePrefabs;
    PFN_bsgfx_renderPrefabs bsgfx_renderPrefabs;
    PFN_bsgfx_renderScenePrefabs bsgfx_renderScenePrefabs;
    PFN_bsgfx_renderPrefabPrimitives bsgfx_renderPrefabPrimitives;
    PFN_bsgfx_tilePrefab bsgfx_tilePrefab;
    PFN_bsgfx_queryPrefabId bsgfx_queryPrefabId;
    PFN_bsgfx_closestPrefab bsgfx_closestPrefab;
    PFN_bsgfx_primitivePosition bsgfx_primitivePosition;
    PFN_bsgfx_loadPrimitives bsgfx_loadPrimitives;
    PFN_bsgfx_primitiveSubtype bsgfx_primitiveSubtype;
    PFN_bsgfx_instancePrimitive bsgfx_instancePrimitive;
    PFN_bsgfx_queryTilePrimitive bsgfx_queryTilePrimitive;
    PFN_bsgfx_instancePrimitives bsgfx_instancePrimitives;
    PFN_bsgfx_renderPrimitives bsgfx_renderPrimitives;
    PFN_bsgfx_queryPrimitive bsgfx_queryPrimitive;
    PFN_bsgfx_loadSpawners bsgfx_loadSpawners;
    PFN_bsgfx_spawn bsgfx_spawn;
    PFN_bsgfx_instanceSpawners bsgfx_instanceSpawners;
    PFN_bsgfx_loadTiles bsgfx_loadTiles;
    PFN_bsgfx_instanceTiles bsgfx_instanceTiles;
    PFN_bsgfx_pushTile bsgfx_pushTile;
    PFN_bsgfx_batchTile bsgfx_batchTile;
    PFN_bsgfx_tileAxes bsgfx_tileAxes;
    PFN_bsgfx_tilePosition bsgfx_tilePosition;
    PFN_bsgfx_tileRotation bsgfx_tileRotation;
    PFN_bsgfx_tileEulerRotation bsgfx_tileEulerRotation;
    PFN_bsgfx_pushTileAt bsgfx_pushTileAt;
    PFN_bsgfx_tileCoordinate bsgfx_tileCoordinate;
    PFN_bsgfx_tileAxis bsgfx_tileAxis;
    PFN_bsgfx_tileIndex bsgfx_tileIndex;
    PFN_bsgfx_instanceWidgets bsgfx_instanceWidgets;
    PFN_bsgfx_renderColorPickers bsgfx_renderColorPickers;
} bsgfx_FunctionTable;

static inline bsgfx_FunctionTable _bsgfx_getFunctions() {
    bsgfx_FunctionTable functions;

    HMODULE module = NULL;
    GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCSTR)(&_bsgfx_getFunctions),
        &module);

    functions.bsgfx_currentScene = (PFN_bsgfx_currentScene)GetProcAddress(module, "_bsgfx_currentScene");
    functions.bsgfx_loadScene = (PFN_bsgfx_loadScene)GetProcAddress(module, "_bsgfx_loadScene");
    functions.bsgfx_images = (PFN_bsgfx_images)GetProcAddress(module, "_bsgfx_images");
    functions.bsgfx_samplers = (PFN_bsgfx_samplers)GetProcAddress(module, "_bsgfx_samplers");
    functions.bsgfx_buffers = (PFN_bsgfx_buffers)GetProcAddress(module, "_bsgfx_buffers");
    functions.bsgfx_batches = (PFN_bsgfx_batches)GetProcAddress(module, "_bsgfx_batches");
    functions.bsgfx_renderers = (PFN_bsgfx_renderers)GetProcAddress(module, "_bsgfx_renderers");
    functions.bsgfx_queues = (PFN_bsgfx_queues)GetProcAddress(module, "_bsgfx_queues");
    functions.bsgfx_rayTracers = (PFN_bsgfx_rayTracers)GetProcAddress(module, "_bsgfx_rayTracers");
    functions.bsgfx_atlases = (PFN_bsgfx_atlases)GetProcAddress(module, "_bsgfx_atlases");
    functions.bsgfx_fonts = (PFN_bsgfx_fonts)GetProcAddress(module, "_bsgfx_fonts");
    functions.bsgfx_validateSubtype = (PFN_bsgfx_validateSubtype)GetProcAddress(module, "_bsgfx_validateSubtype");
    functions.bsgfx_validateInstanceType = (PFN_bsgfx_validateInstanceType)GetProcAddress(module, "_bsgfx_validateInstanceType");
    functions.bsgfx_materialCategoryName = (PFN_bsgfx_materialCategoryName)GetProcAddress(module, "_bsgfx_materialCategoryName");
    functions.bsgfx_materials = (PFN_bsgfx_materials)GetProcAddress(module, "_bsgfx_materials");
    functions.bsgfx_queryMaterialHash = (PFN_bsgfx_queryMaterialHash)GetProcAddress(module, "_bsgfx_queryMaterialHash");
    functions.bsgfx_fetchMaterial = (PFN_bsgfx_fetchMaterial)GetProcAddress(module, "_bsgfx_fetchMaterial");
    functions.bsgfx_queryMaterial = (PFN_bsgfx_queryMaterial)GetProcAddress(module, "_bsgfx_queryMaterial");
    functions.bsgfx_loadMaterials = (PFN_bsgfx_loadMaterials)GetProcAddress(module, "_bsgfx_loadMaterials");
    functions.bsgfx_material = (PFN_bsgfx_material)GetProcAddress(module, "_bsgfx_material");
    functions.bsgfx_highlightMaterial = (PFN_bsgfx_highlightMaterial)GetProcAddress(module, "_bsgfx_highlightMaterial");
    functions.bsgfx_unhighlightMaterial = (PFN_bsgfx_unhighlightMaterial)GetProcAddress(module, "_bsgfx_unhighlightMaterial");
    functions.bsgfx_tickMaterials = (PFN_bsgfx_tickMaterials)GetProcAddress(module, "_bsgfx_tickMaterials");
    functions.bsgfx_shadowPipe = (PFN_bsgfx_shadowPipe)GetProcAddress(module, "_bsgfx_shadowPipe");
    functions.bsgfx_pipeline = (PFN_bsgfx_pipeline)GetProcAddress(module, "_bsgfx_pipeline");
    functions.bsgfx_sweepCollisions = (PFN_bsgfx_sweepCollisions)GetProcAddress(module, "_bsgfx_sweepCollisions");
    functions.bsgfx_collider = (PFN_bsgfx_collider)GetProcAddress(module, "_bsgfx_collider");
    functions.bsgfx_applyCollisions = (PFN_bsgfx_applyCollisions)GetProcAddress(module, "_bsgfx_applyCollisions");
    functions.bsgfx_printCollisions = (PFN_bsgfx_printCollisions)GetProcAddress(module, "_bsgfx_printCollisions");
    functions.bsgfx_instanceSweepCollisions = (PFN_bsgfx_instanceSweepCollisions)GetProcAddress(module, "_bsgfx_instanceSweepCollisions");
    functions.bsgfx_instanceCollider = (PFN_bsgfx_instanceCollider)GetProcAddress(module, "_bsgfx_instanceCollider");
    functions.bsgfx_skeleton = (PFN_bsgfx_skeleton)GetProcAddress(module, "_bsgfx_skeleton");
    functions.bsgfx_animationFrame = (PFN_bsgfx_animationFrame)GetProcAddress(module, "_bsgfx_animationFrame");
    functions.bsgfx_animationFrameCount = (PFN_bsgfx_animationFrameCount)GetProcAddress(module, "_bsgfx_animationFrameCount");
    functions.bsgfx_applyAnimationVelocity = (PFN_bsgfx_applyAnimationVelocity)GetProcAddress(module, "_bsgfx_applyAnimationVelocity");
    functions.bsgfx_queueAnimation = (PFN_bsgfx_queueAnimation)GetProcAddress(module, "_bsgfx_queueAnimation");
    functions.bsgfx_runAnimator = (PFN_bsgfx_runAnimator)GetProcAddress(module, "_bsgfx_runAnimator");
    functions.bsgfx_animator = (PFN_bsgfx_animator)GetProcAddress(module, "_bsgfx_animator");
    functions.bsgfx_subtypeInstances = (PFN_bsgfx_subtypeInstances)GetProcAddress(module, "_bsgfx_subtypeInstances");
    functions.bsgfx_iniInstances = (PFN_bsgfx_iniInstances)GetProcAddress(module, "_bsgfx_iniInstances");
    functions.bsgfx_instanceType = (PFN_bsgfx_instanceType)GetProcAddress(module, "_bsgfx_instanceType");
    functions.bsgfx_subtypeRange = (PFN_bsgfx_subtypeRange)GetProcAddress(module, "_bsgfx_subtypeRange");
    functions.bsgfx_deleteSubtype = (PFN_bsgfx_deleteSubtype)GetProcAddress(module, "_bsgfx_deleteSubtype");
    functions.bsgfx_instanceCount = (PFN_bsgfx_instanceCount)GetProcAddress(module, "_bsgfx_instanceCount");
    functions.bsgfx_subtypeCount = (PFN_bsgfx_subtypeCount)GetProcAddress(module, "_bsgfx_subtypeCount");
    functions.bsgfx_subtypes = (PFN_bsgfx_subtypes)GetProcAddress(module, "_bsgfx_subtypes");
    functions.bsgfx_subtype = (PFN_bsgfx_subtype)GetProcAddress(module, "_bsgfx_subtype");
    functions.bsgfx_instance = (PFN_bsgfx_instance)GetProcAddress(module, "_bsgfx_instance");
    functions.bsgfx_tickInstances = (PFN_bsgfx_tickInstances)GetProcAddress(module, "_bsgfx_tickInstances");
    functions.bsgfx_subtypeHasFlag = (PFN_bsgfx_subtypeHasFlag)GetProcAddress(module, "_bsgfx_subtypeHasFlag");
    functions.bsgfx_renderSubtype = (PFN_bsgfx_renderSubtype)GetProcAddress(module, "_bsgfx_renderSubtype");
    functions.bsgfx_resetInstances = (PFN_bsgfx_resetInstances)GetProcAddress(module, "_bsgfx_resetInstances");
    functions.bsgfx_resetSubtype = (PFN_bsgfx_resetSubtype)GetProcAddress(module, "_bsgfx_resetSubtype");
    functions.bsgfx_instanceHiResMesh = (PFN_bsgfx_instanceHiResMesh)GetProcAddress(module, "_bsgfx_instanceHiResMesh");
    functions.bsgfx_instanceMesh = (PFN_bsgfx_instanceMesh)GetProcAddress(module, "_bsgfx_instanceMesh");
    functions.bsgfx_instanceBoneMesh = (PFN_bsgfx_instanceBoneMesh)GetProcAddress(module, "_bsgfx_instanceBoneMesh");
    functions.bsgfx_instanceDepthlessLine = (PFN_bsgfx_instanceDepthlessLine)GetProcAddress(module, "_bsgfx_instanceDepthlessLine");
    functions.bsgfx_instanceLine = (PFN_bsgfx_instanceLine)GetProcAddress(module, "_bsgfx_instanceLine");
    functions.bsgfx_instanceRay = (PFN_bsgfx_instanceRay)GetProcAddress(module, "_bsgfx_instanceRay");
    functions.bsgfx_instanceAabb = (PFN_bsgfx_instanceAabb)GetProcAddress(module, "_bsgfx_instanceAabb");
    functions.bsgfx_instanceSphere = (PFN_bsgfx_instanceSphere)GetProcAddress(module, "_bsgfx_instanceSphere");
    functions.bsgfx_instanceCone = (PFN_bsgfx_instanceCone)GetProcAddress(module, "_bsgfx_instanceCone");
    functions.bsgfx_instancePoint = (PFN_bsgfx_instancePoint)GetProcAddress(module, "_bsgfx_instancePoint");
    functions.bsgfx_instanceQuad = (PFN_bsgfx_instanceQuad)GetProcAddress(module, "_bsgfx_instanceQuad");
    functions.bsgfx_instanceAtlas = (PFN_bsgfx_instanceAtlas)GetProcAddress(module, "_bsgfx_instanceAtlas");
    functions.bsgfx_instanceAtlasFlipped = (PFN_bsgfx_instanceAtlasFlipped)GetProcAddress(module, "_bsgfx_instanceAtlasFlipped");
    functions.bsgfx_instanceText = (PFN_bsgfx_instanceText)GetProcAddress(module, "_bsgfx_instanceText");
    functions.bsgfx_matrix = (PFN_bsgfx_matrix)GetProcAddress(module, "_bsgfx_matrix");
    functions.bsgfx_renderFontSubtypes = (PFN_bsgfx_renderFontSubtypes)GetProcAddress(module, "_bsgfx_renderFontSubtypes");
    functions.bsgfx_settingsEditor = (PFN_bsgfx_settingsEditor)GetProcAddress(module, "_bsgfx_settingsEditor");
    functions.bsgfx_renderFineShadowVolumes = (PFN_bsgfx_renderFineShadowVolumes)GetProcAddress(module, "_bsgfx_renderFineShadowVolumes");
    functions.bsgfx_renderShadowVolumes = (PFN_bsgfx_renderShadowVolumes)GetProcAddress(module, "_bsgfx_renderShadowVolumes");
    functions.bsgfx_computeShadowVolumes = (PFN_bsgfx_computeShadowVolumes)GetProcAddress(module, "_bsgfx_computeShadowVolumes");
    functions.bsgfx_prefabModel = (PFN_bsgfx_prefabModel)GetProcAddress(module, "_bsgfx_prefabModel");
    functions.bsgfx_package = (PFN_bsgfx_package)GetProcAddress(module, "_bsgfx_package");
    functions.bsgfx_ini = (PFN_bsgfx_ini)GetProcAddress(module, "_bsgfx_ini");
    functions.bsgfx_bsmodDll = (PFN_bsgfx_bsmodDll)GetProcAddress(module, "_bsgfx_bsmodDll");
    functions.bsgfx_checkGFSDK = (PFN_bsgfx_checkGFSDK)GetProcAddress(module, "_bsgfx_checkGFSDK");
    functions.bsgfx_logGFSDK = (PFN_bsgfx_logGFSDK)GetProcAddress(module, "_bsgfx_logGFSDK");
    functions.bsgfx_app = (PFN_bsgfx_app)GetProcAddress(module, "_bsgfx_app");
    functions.bsgfx_callbacks = (PFN_bsgfx_callbacks)GetProcAddress(module, "_bsgfx_callbacks");
    functions.bsgfx_settings = (PFN_bsgfx_settings)GetProcAddress(module, "_bsgfx_settings");
    functions.poser = (PFN_poser)GetProcAddress(module, "_poser");
    functions.bsgfx_setCamera = (PFN_bsgfx_setCamera)GetProcAddress(module, "_bsgfx_setCamera");
    functions.bsgfx_type = (PFN_bsgfx_type)GetProcAddress(module, "_bsgfx_type");
    functions.bsgfx_map = (PFN_bsgfx_map)GetProcAddress(module, "_bsgfx_map");
    functions.bsgfx_remap = (PFN_bsgfx_remap)GetProcAddress(module, "_bsgfx_remap");
    functions.bsgfx_count = (PFN_bsgfx_count)GetProcAddress(module, "_bsgfx_count");
    functions.bsgfx_get = (PFN_bsgfx_get)GetProcAddress(module, "_bsgfx_get");
    functions.bsgfx_getType = (PFN_bsgfx_getType)GetProcAddress(module, "_bsgfx_getType");
    functions.bsgfx_id = (PFN_bsgfx_id)GetProcAddress(module, "_bsgfx_id");
    functions.bsgfx_rawId = (PFN_bsgfx_rawId)GetProcAddress(module, "_bsgfx_rawId");
    functions.bsgfx_getRaw = (PFN_bsgfx_getRaw)GetProcAddress(module, "_bsgfx_getRaw");
    functions.bsgfx_flexibleCount = (PFN_bsgfx_flexibleCount)GetProcAddress(module, "_bsgfx_flexibleCount");
    functions.bsmod_add = (PFN_bsmod_add)GetProcAddress(module, "_bsmod_add");
    functions.bsmod_queryType = (PFN_bsmod_queryType)GetProcAddress(module, "_bsmod_queryType");
    functions.bsgfx_loadFoliages = (PFN_bsgfx_loadFoliages)GetProcAddress(module, "_bsgfx_loadFoliages");
    functions.bsgfx_queryFoliage = (PFN_bsgfx_queryFoliage)GetProcAddress(module, "_bsgfx_queryFoliage");
    functions.bsgfx_loadLights = (PFN_bsgfx_loadLights)GetProcAddress(module, "_bsgfx_loadLights");
    functions.bsgfx_computePrefabShadows = (PFN_bsgfx_computePrefabShadows)GetProcAddress(module, "_bsgfx_computePrefabShadows");
    functions.bsgfx_renderPrefabShadowVolumes = (PFN_bsgfx_renderPrefabShadowVolumes)GetProcAddress(module, "_bsgfx_renderPrefabShadowVolumes");
    functions.bsgfx_prefabMetadata = (PFN_bsgfx_prefabMetadata)GetProcAddress(module, "_bsgfx_prefabMetadata");
    functions.bsgfx_savePrefabs = (PFN_bsgfx_savePrefabs)GetProcAddress(module, "_bsgfx_savePrefabs");
    functions.bsgfx_loadPrefabs = (PFN_bsgfx_loadPrefabs)GetProcAddress(module, "_bsgfx_loadPrefabs");
    functions.bsgfx_prefabTransform = (PFN_bsgfx_prefabTransform)GetProcAddress(module, "_bsgfx_prefabTransform");
    functions.bsgfx_instancePrefabModel = (PFN_bsgfx_instancePrefabModel)GetProcAddress(module, "_bsgfx_instancePrefabModel");
    functions.bsgfx_instancePrefab = (PFN_bsgfx_instancePrefab)GetProcAddress(module, "_bsgfx_instancePrefab");
    functions.bsgfx_instancePrefabs = (PFN_bsgfx_instancePrefabs)GetProcAddress(module, "_bsgfx_instancePrefabs");
    functions.bsgfx_renderPrefabs = (PFN_bsgfx_renderPrefabs)GetProcAddress(module, "_bsgfx_renderPrefabs");
    functions.bsgfx_renderScenePrefabs = (PFN_bsgfx_renderScenePrefabs)GetProcAddress(module, "_bsgfx_renderScenePrefabs");
    functions.bsgfx_renderPrefabPrimitives = (PFN_bsgfx_renderPrefabPrimitives)GetProcAddress(module, "_bsgfx_renderPrefabPrimitives");
    functions.bsgfx_tilePrefab = (PFN_bsgfx_tilePrefab)GetProcAddress(module, "_bsgfx_tilePrefab");
    functions.bsgfx_queryPrefabId = (PFN_bsgfx_queryPrefabId)GetProcAddress(module, "_bsgfx_queryPrefabId");
    functions.bsgfx_closestPrefab = (PFN_bsgfx_closestPrefab)GetProcAddress(module, "_bsgfx_closestPrefab");
    functions.bsgfx_primitivePosition = (PFN_bsgfx_primitivePosition)GetProcAddress(module, "_bsgfx_primitivePosition");
    functions.bsgfx_loadPrimitives = (PFN_bsgfx_loadPrimitives)GetProcAddress(module, "_bsgfx_loadPrimitives");
    functions.bsgfx_primitiveSubtype = (PFN_bsgfx_primitiveSubtype)GetProcAddress(module, "_bsgfx_primitiveSubtype");
    functions.bsgfx_instancePrimitive = (PFN_bsgfx_instancePrimitive)GetProcAddress(module, "_bsgfx_instancePrimitive");
    functions.bsgfx_queryTilePrimitive = (PFN_bsgfx_queryTilePrimitive)GetProcAddress(module, "_bsgfx_queryTilePrimitive");
    functions.bsgfx_instancePrimitives = (PFN_bsgfx_instancePrimitives)GetProcAddress(module, "_bsgfx_instancePrimitives");
    functions.bsgfx_renderPrimitives = (PFN_bsgfx_renderPrimitives)GetProcAddress(module, "_bsgfx_renderPrimitives");
    functions.bsgfx_queryPrimitive = (PFN_bsgfx_queryPrimitive)GetProcAddress(module, "_bsgfx_queryPrimitive");
    functions.bsgfx_loadSpawners = (PFN_bsgfx_loadSpawners)GetProcAddress(module, "_bsgfx_loadSpawners");
    functions.bsgfx_spawn = (PFN_bsgfx_spawn)GetProcAddress(module, "_bsgfx_spawn");
    functions.bsgfx_instanceSpawners = (PFN_bsgfx_instanceSpawners)GetProcAddress(module, "_bsgfx_instanceSpawners");
    functions.bsgfx_loadTiles = (PFN_bsgfx_loadTiles)GetProcAddress(module, "_bsgfx_loadTiles");
    functions.bsgfx_instanceTiles = (PFN_bsgfx_instanceTiles)GetProcAddress(module, "_bsgfx_instanceTiles");
    functions.bsgfx_pushTile = (PFN_bsgfx_pushTile)GetProcAddress(module, "_bsgfx_pushTile");
    functions.bsgfx_batchTile = (PFN_bsgfx_batchTile)GetProcAddress(module, "_bsgfx_batchTile");
    functions.bsgfx_tileAxes = (PFN_bsgfx_tileAxes)GetProcAddress(module, "_bsgfx_tileAxes");
    functions.bsgfx_tilePosition = (PFN_bsgfx_tilePosition)GetProcAddress(module, "_bsgfx_tilePosition");
    functions.bsgfx_tileRotation = (PFN_bsgfx_tileRotation)GetProcAddress(module, "_bsgfx_tileRotation");
    functions.bsgfx_tileEulerRotation = (PFN_bsgfx_tileEulerRotation)GetProcAddress(module, "_bsgfx_tileEulerRotation");
    functions.bsgfx_pushTileAt = (PFN_bsgfx_pushTileAt)GetProcAddress(module, "_bsgfx_pushTileAt");
    functions.bsgfx_tileCoordinate = (PFN_bsgfx_tileCoordinate)GetProcAddress(module, "_bsgfx_tileCoordinate");
    functions.bsgfx_tileAxis = (PFN_bsgfx_tileAxis)GetProcAddress(module, "_bsgfx_tileAxis");
    functions.bsgfx_tileIndex = (PFN_bsgfx_tileIndex)GetProcAddress(module, "_bsgfx_tileIndex");
    functions.bsgfx_instanceWidgets = (PFN_bsgfx_instanceWidgets)GetProcAddress(module, "_bsgfx_instanceWidgets");
    functions.bsgfx_renderColorPickers = (PFN_bsgfx_renderColorPickers)GetProcAddress(module, "_bsgfx_renderColorPickers");

    return functions;
}

#endif
