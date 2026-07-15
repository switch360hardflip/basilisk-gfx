
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
        
#include <basilisk-core.h>
#include <basilisk-gfx.h>
#include <windows.h>

#ifndef BSGFX_INTERNAL_GEN_H
#define BSGFX_INTERNAL_GEN_H

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
typedef void(__stdcall* PFN_bsgfx_onDeviceLost)();
typedef void(__stdcall* PFN_bsgfx_sweepCollisions)(float sweep_radius, bs_vec3 position);
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
typedef bs_Range(__stdcall* PFN_bsgfx_instanceAabb)(const bs_Aabb* aabb, bs_RGBA color);
typedef int(__stdcall* PFN_bsgfx_instanceSphere)(bs_vec3 position, float radius);
typedef int(__stdcall* PFN_bsgfx_instanceCone)(bs_mat4 transform, float radius, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instancePoint)(bs_vec3 position, bs_RGBA color, float size);
typedef int(__stdcall* PFN_bsgfx_instanceQuad)(int subtype, bs_mat4x3 transform, bs_vec4 coords, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instanceAtlas)(int subtype, bs_mat4x3 transform, int texture, bs_U32 flags, int id, int material);
typedef int(__stdcall* PFN_bsgfx_instanceAtlasFlipped)(int subtype, bs_mat4x3 transform, int texture, bs_U32 flags, int id, int material);
typedef bs_vec2(__stdcall* PFN_bsgfx_instanceText)(int subtype, bs_Font* font, bsgfx_Text* params, char* value, int value_length);
typedef bs_mat4x3(__stdcall* PFN_bsgfx_matrix)(bs_vec3 position, bs_vec3 scale);
typedef void(__stdcall* PFN_bsgfx_renderFontSubtypes)();
typedef void(__stdcall* PFN_bsgfx_settingsEditor)(bs_List* result);
typedef void(__stdcall* PFN_bsgfx_renderFineShadowVolumes)();
typedef void(__stdcall* PFN_bsgfx_renderShadowVolumes)();
typedef void(__stdcall* PFN_bsgfx_computeShadowVolumes)();
typedef bs_Model*(__stdcall* PFN_bsgfx_prefabModel)();
typedef int(__stdcall* PFN_bsgfx_package)();
typedef void(__stdcall* PFN_bsgfx_ini)(const char* name, bs_U32 width, bs_U32 height, int argc, char* argv);
typedef HINSTANCE(__stdcall* PFN_bsgfx_bsmodDll)();
typedef void(__stdcall* PFN_bsgfx_checkGFSDK)(bs_U32 result);
typedef void(__stdcall* PFN_bsgfx_logGFSDK)(bs_U32 result);
typedef bsgfx_Application*(__stdcall* PFN_bsgfx_app)();
typedef bsgfx_Callbacks*(__stdcall* PFN_bsgfx_callbacks)();
typedef bsgfx_Settings*(__stdcall* PFN_bsgfx_settings)();
typedef struct Poser*(__stdcall* PFN_poser)();
typedef void(__stdcall* PFN_bsgfx_setCamera)(const bs_mat4* proj, const bs_mat4* view);

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
    PFN_bsgfx_onDeviceLost bsgfx_onDeviceLost;
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
    functions.bsgfx_onDeviceLost = (PFN_bsgfx_onDeviceLost)GetProcAddress(module, "_bsgfx_onDeviceLost");
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

    return functions;
}

#endif
