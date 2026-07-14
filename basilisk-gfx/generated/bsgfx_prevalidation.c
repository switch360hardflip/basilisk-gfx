
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

static bsgfx_FunctionTable next = { 0 };

void _preval_bsgfx_setFunctions(bsgfx_FunctionTable functions) {
    next = functions;
}

static bsgfx_Scene* _preval_bsgfx_currentScene() {
    return next.bsgfx_currentScene();
}

static void _preval_bsgfx_loadScene(const char* name) {
    if (name == NULL)
        return;

    return next.bsgfx_loadScene(name);
}

static int _preval_bsgfx_images() {
    return next.bsgfx_images();
}

static int _preval_bsgfx_samplers() {
    return next.bsgfx_samplers();
}

static int _preval_bsgfx_buffers() {
    return next.bsgfx_buffers();
}

static int _preval_bsgfx_batches() {
    return next.bsgfx_batches();
}

static int _preval_bsgfx_renderers() {
    return next.bsgfx_renderers();
}

static int _preval_bsgfx_queues() {
    return next.bsgfx_queues();
}

static int _preval_bsgfx_rayTracers() {
    return next.bsgfx_rayTracers();
}

static int _preval_bsgfx_atlases() {
    return next.bsgfx_atlases();
}

static int _preval_bsgfx_fonts() {
    return next.bsgfx_fonts();
}

static const char* _preval_bsgfx_materialCategoryName(bsgfx_MaterialCategory category) {
    return next.bsgfx_materialCategoryName(category);
}

static bs_List* _preval_bsgfx_materials() {
    return next.bsgfx_materials();
}

static bsgfx_Material* _preval_bsgfx_queryMaterialHash(bs_U64 hash) {
    return next.bsgfx_queryMaterialHash(hash);
}

static bsgfx_Material* _preval_bsgfx_fetchMaterial(int id) {
    return next.bsgfx_fetchMaterial(id);
}

static bsgfx_Material* _preval_bsgfx_queryMaterial(const char* name) {
    if (name == NULL)
        return NULL;

    return next.bsgfx_queryMaterial(name);
}

static void _preval_bsgfx_loadMaterials() {
    return next.bsgfx_loadMaterials();
}

static bsgfx_Material* _preval_bsgfx_material(char* name, int name_length) {
    if (name == NULL)
        return NULL;

    return next.bsgfx_material(name, name_length);
}

static void _preval_bsgfx_highlightMaterial(int material_id, bool auto_unhighlight) {
    return next.bsgfx_highlightMaterial(material_id, auto_unhighlight);
}

static void _preval_bsgfx_unhighlightMaterial(int material_id) {
    return next.bsgfx_unhighlightMaterial(material_id);
}

static void _preval_bsgfx_tickMaterials() {
    return next.bsgfx_tickMaterials();
}

static void _preval_bsgfx_shadowPipe() {
    return next.bsgfx_shadowPipe();
}

static void _preval_bsgfx_pipeline() {
    return next.bsgfx_pipeline();
}

static void _preval_bsgfx_onDeviceLost() {
    return next.bsgfx_onDeviceLost();
}

static void _preval_bsgfx_sweepCollisions(float sweep_radius, bs_vec3 position) {
    return next.bsgfx_sweepCollisions(sweep_radius, position);
}

static bsgfx_Collider _preval_bsgfx_collider(bs_Aabb aabb, bs_vec3 scale, bs_ivec3 resolution, float sweep_radius) {
    return next.bsgfx_collider(aabb, scale, resolution, sweep_radius);
}

static void _preval_bsgfx_applyCollisions(bsgfx_Collider* collider, bs_vec3 position, bs_vec3* velocity) {
    if (collider == NULL)
        return;

    if (velocity == NULL)
        return;

    return next.bsgfx_applyCollisions(collider, position, velocity);
}

static void _preval_bsgfx_printCollisions(bsgfx_Collider* collider) {
    if (collider == NULL)
        return;

    return next.bsgfx_printCollisions(collider);
}

static void _preval_bsgfx_instanceSweepCollisions() {
    return next.bsgfx_instanceSweepCollisions();
}

static void _preval_bsgfx_instanceCollider(bsgfx_Collider* collider, bs_vec3 position, bs_vec3* velocity) {
    if (collider == NULL)
        return;

    if (velocity == NULL)
        return;

    return next.bsgfx_instanceCollider(collider, position, velocity);
}

static int _preval_bsgfx_skeleton(bs_Armature* armature) {
    if (armature == NULL)
        return 0;

    return next.bsgfx_skeleton(armature);
}

static int _preval_bsgfx_animationFrame(bs_Animation* animation, float time, int num_frames) {
    if (animation == NULL)
        return 0;

    return next.bsgfx_animationFrame(animation, time, num_frames);
}

static int _preval_bsgfx_animationFrameCount(bs_Animation* animation, float time_scale) {
    if (animation == NULL)
        return 0;

    return next.bsgfx_animationFrameCount(animation, time_scale);
}

static void _preval_bsgfx_applyAnimationVelocity(bsgfx_Animator* animator, bs_vec3* velocity, bs_vec2 input) {
    if (animator == NULL)
        return;

    if (velocity == NULL)
        return;

    return next.bsgfx_applyAnimationVelocity(animator, velocity, input);
}

static void _preval_bsgfx_queueAnimation(bsgfx_Animator* animator, int animation_id) {
    if (animator == NULL)
        return;

    return next.bsgfx_queueAnimation(animator, animation_id);
}

static void _preval_bsgfx_runAnimator(bsgfx_Animator* animator, bsgfx_AnimatorCallbacks callbacks) {
    if (animator == NULL)
        return;

    return next.bsgfx_runAnimator(animator, callbacks);
}

static bsgfx_Animator _preval_bsgfx_animator(bs_Armature* armature, int resting_animation_id, int animations_count) {
    if (armature == NULL)
        return (bsgfx_Animator) { 0 };

    return next.bsgfx_animator(armature, resting_animation_id, animations_count);
}

static bs_List* _preval_bsgfx_subtypeInstances(int subtype) {
    return next.bsgfx_subtypeInstances(subtype);
}

static bs_Result _preval_bsgfx_iniInstances() {
    return next.bsgfx_iniInstances();
}

static void _preval_bsgfx_instanceType(int type, int max_instance_count, int bind_set, int binding) {
    return next.bsgfx_instanceType(type, max_instance_count, bind_set, binding);
}

static bs_Range _preval_bsgfx_subtypeRange(int subtype) {
    return next.bsgfx_subtypeRange(subtype);
}

static void _preval_bsgfx_deleteSubtype(int subtype) {
    return next.bsgfx_deleteSubtype(subtype);
}

static int _preval_bsgfx_instanceCount(int subtype) {
    return next.bsgfx_instanceCount(subtype);
}

static int _preval_bsgfx_subtypeCount(int instance_type_id) {
    return next.bsgfx_subtypeCount(instance_type_id);
}

static const int* _preval_bsgfx_subtypes() {
    return next.bsgfx_subtypes();
}

static int _preval_bsgfx_subtype(int type, bs_Batch* batch, bs_U32 flags, bs_Range range) {
    if (batch == NULL)
        return 0;

    return next.bsgfx_subtype(type, batch, flags, range);
}

static int _preval_bsgfx_instance(int subtype, const char* data, int data_size, bs_U32 flags, unsigned int bone_index, int id, int material) {
    if (data == NULL)
        return 0;

    return next.bsgfx_instance(subtype, data, data_size, flags, bone_index, id, material);
}

static void _preval_bsgfx_tickInstances() {
    return next.bsgfx_tickInstances();
}

static bool _preval_bsgfx_subtypeHasFlag(int subtype, bs_U32 flag) {
    return next.bsgfx_subtypeHasFlag(subtype, flag);
}

static void _preval_bsgfx_renderSubtype(int subtype, bs_Pipeline* pipeline) {
    if (pipeline == NULL)
        return;

    return next.bsgfx_renderSubtype(subtype, pipeline);
}

static void _preval_bsgfx_resetInstances() {
    return next.bsgfx_resetInstances();
}

static void _preval_bsgfx_resetSubtype(int subtype) {
    return next.bsgfx_resetSubtype(subtype);
}

static void _preval_bsgfx_instanceHiResMesh(bs_Mesh* mesh, const bs_vec3* position, const bs_vec4* rotation, float scale, int subtype_offset, bool origin_at_center) {
    if (mesh == NULL)
        return;

    if (position == NULL)
        return;

    if (rotation == NULL)
        return;

    return next.bsgfx_instanceHiResMesh(mesh, position, rotation, scale, subtype_offset, origin_at_center);
}

static int _preval_bsgfx_instanceMesh(int subtype, const bsgfx_MeshInstance* data, bs_U32 flags, int id, int material) {
    if (data == NULL)
        return 0;

    return next.bsgfx_instanceMesh(subtype, data, flags, id, material);
}

static int _preval_bsgfx_instanceBoneMesh(int subtype, const bsgfx_BoneInstance* data, bs_U32 flags, int id, int material) {
    if (data == NULL)
        return 0;

    return next.bsgfx_instanceBoneMesh(subtype, data, flags, id, material);
}

static int _preval_bsgfx_instanceDepthlessLine(bs_vec3 start, bs_vec3 end, bs_RGBA color) {
    return next.bsgfx_instanceDepthlessLine(start, end, color);
}

static int _preval_bsgfx_instanceLine(bs_vec3 start, bs_vec3 end, bs_RGBA color) {
    return next.bsgfx_instanceLine(start, end, color);
}

static int _preval_bsgfx_instanceRay(const bs_Ray* ray, bs_RGBA color) {
    if (ray == NULL)
        return 0;

    return next.bsgfx_instanceRay(ray, color);
}

static bs_Range _preval_bsgfx_instanceAabb(const bs_Aabb* aabb, bs_RGBA color) {
    if (aabb == NULL)
        return 0;

    return next.bsgfx_instanceAabb(aabb, color);
}

static int _preval_bsgfx_instanceSphere(bs_vec3 position, float radius) {
    return next.bsgfx_instanceSphere(position, radius);
}

static int _preval_bsgfx_instanceCone(bs_mat4 transform, float radius, bs_U32 flags, int id, int material) {
    return next.bsgfx_instanceCone(transform, radius, flags, id, material);
}

static int _preval_bsgfx_instancePoint(bs_vec3 position, bs_RGBA color, float size) {
    return next.bsgfx_instancePoint(position, color, size);
}

static int _preval_bsgfx_instanceQuad(int subtype, bs_mat4x3 transform, bs_vec4 coords, bs_U32 flags, int id, int material) {
    return next.bsgfx_instanceQuad(subtype, transform, coords, flags, id, material);
}

static int _preval_bsgfx_instanceAtlas(int subtype, bs_mat4x3 transform, int texture, bs_U32 flags, int id, int material) {
    return next.bsgfx_instanceAtlas(subtype, transform, texture, flags, id, material);
}

static int _preval_bsgfx_instanceAtlasFlipped(int subtype, bs_mat4x3 transform, int texture, bs_U32 flags, int id, int material) {
    return next.bsgfx_instanceAtlasFlipped(subtype, transform, texture, flags, id, material);
}

static bs_Range _preval_bsgfx_instanceAtlas(const bs_mat4* transform, int segments, float radius, bs_RGBA color) {
    if (transform == NULL)
        return 0;

    return next.bsgfx_instanceAtlas(transform, segments, radius, color);
}

static bs_mat4x3 _preval_bsgfx_matrix(bs_vec3 position, bs_vec3 scale) {
    return next.bsgfx_matrix(position, scale);
}

static void _preval_bsgfx_renderFontSubtypes() {
    return next.bsgfx_renderFontSubtypes();
}

static void _preval_bsgfx_settingsEditor(bs_List* result) {
    if (result == NULL)
        return;

    return next.bsgfx_settingsEditor(result);
}

static void _preval_bsgfx_renderFineShadowVolumes() {
    return next.bsgfx_renderFineShadowVolumes();
}

static void _preval_bsgfx_renderShadowVolumes() {
    return next.bsgfx_renderShadowVolumes();
}

static void _preval_bsgfx_computeShadowVolumes() {
    return next.bsgfx_computeShadowVolumes();
}

static bs_Model* _preval_bsgfx_prefabModel() {
    return next.bsgfx_prefabModel();
}

static int _preval_bsgfx_package() {
    return next.bsgfx_package();
}

static void _preval_bsgfx_ini(const char* name, bs_U32 width, bs_U32 height, int argc, char* argv) {
    if (name == NULL)
        return;

    if (argv == NULL)
        return;

    return next.bsgfx_ini(name, width, height, argc, argv);
}

static HINSTANCE _preval_bsgfx_bsmodDll() {
    return next.bsgfx_bsmodDll();
}

static void _preval_bsgfx_checkGFSDK(bs_U32 result) {
    return next.bsgfx_checkGFSDK(result);
}

static void _preval_bsgfx_logGFSDK(bs_U32 result) {
    return next.bsgfx_logGFSDK(result);
}

static bsgfx_Application* _preval_bsgfx_app() {
    return next.bsgfx_app();
}

static bsgfx_Callbacks* _preval_bsgfx_callbacks() {
    return next.bsgfx_callbacks();
}

static bsgfx_Settings* _preval_bsgfx_settings() {
    return next.bsgfx_settings();
}

static struct Poser* _preval_poser() {
    return next.poser();
}

static void _preval_bsgfx_setCamera(const bs_mat4* proj, const bs_mat4* view) {
    if (proj == NULL)
        return;

    if (view == NULL)
        return;

    return next.bsgfx_setCamera(proj, view);
}

bsgfx_FunctionTable _preval_bsgfx_getFunctionTable() {
    bsgfx_FunctionTable functions;

    functions.bsgfx_currentScene = _preval_bsgfx_currentScene;
    functions.bsgfx_loadScene = _preval_bsgfx_loadScene;
    functions.bsgfx_images = _preval_bsgfx_images;
    functions.bsgfx_samplers = _preval_bsgfx_samplers;
    functions.bsgfx_buffers = _preval_bsgfx_buffers;
    functions.bsgfx_batches = _preval_bsgfx_batches;
    functions.bsgfx_renderers = _preval_bsgfx_renderers;
    functions.bsgfx_queues = _preval_bsgfx_queues;
    functions.bsgfx_rayTracers = _preval_bsgfx_rayTracers;
    functions.bsgfx_atlases = _preval_bsgfx_atlases;
    functions.bsgfx_fonts = _preval_bsgfx_fonts;
    functions.bsgfx_materialCategoryName = _preval_bsgfx_materialCategoryName;
    functions.bsgfx_materials = _preval_bsgfx_materials;
    functions.bsgfx_queryMaterialHash = _preval_bsgfx_queryMaterialHash;
    functions.bsgfx_fetchMaterial = _preval_bsgfx_fetchMaterial;
    functions.bsgfx_queryMaterial = _preval_bsgfx_queryMaterial;
    functions.bsgfx_loadMaterials = _preval_bsgfx_loadMaterials;
    functions.bsgfx_material = _preval_bsgfx_material;
    functions.bsgfx_highlightMaterial = _preval_bsgfx_highlightMaterial;
    functions.bsgfx_unhighlightMaterial = _preval_bsgfx_unhighlightMaterial;
    functions.bsgfx_tickMaterials = _preval_bsgfx_tickMaterials;
    functions.bsgfx_shadowPipe = _preval_bsgfx_shadowPipe;
    functions.bsgfx_pipeline = _preval_bsgfx_pipeline;
    functions.bsgfx_onDeviceLost = _preval_bsgfx_onDeviceLost;
    functions.bsgfx_sweepCollisions = _preval_bsgfx_sweepCollisions;
    functions.bsgfx_collider = _preval_bsgfx_collider;
    functions.bsgfx_applyCollisions = _preval_bsgfx_applyCollisions;
    functions.bsgfx_printCollisions = _preval_bsgfx_printCollisions;
    functions.bsgfx_instanceSweepCollisions = _preval_bsgfx_instanceSweepCollisions;
    functions.bsgfx_instanceCollider = _preval_bsgfx_instanceCollider;
    functions.bsgfx_skeleton = _preval_bsgfx_skeleton;
    functions.bsgfx_animationFrame = _preval_bsgfx_animationFrame;
    functions.bsgfx_animationFrameCount = _preval_bsgfx_animationFrameCount;
    functions.bsgfx_applyAnimationVelocity = _preval_bsgfx_applyAnimationVelocity;
    functions.bsgfx_queueAnimation = _preval_bsgfx_queueAnimation;
    functions.bsgfx_runAnimator = _preval_bsgfx_runAnimator;
    functions.bsgfx_animator = _preval_bsgfx_animator;
    functions.bsgfx_subtypeInstances = _preval_bsgfx_subtypeInstances;
    functions.bsgfx_iniInstances = _preval_bsgfx_iniInstances;
    functions.bsgfx_instanceType = _preval_bsgfx_instanceType;
    functions.bsgfx_subtypeRange = _preval_bsgfx_subtypeRange;
    functions.bsgfx_deleteSubtype = _preval_bsgfx_deleteSubtype;
    functions.bsgfx_instanceCount = _preval_bsgfx_instanceCount;
    functions.bsgfx_subtypeCount = _preval_bsgfx_subtypeCount;
    functions.bsgfx_subtypes = _preval_bsgfx_subtypes;
    functions.bsgfx_subtype = _preval_bsgfx_subtype;
    functions.bsgfx_instance = _preval_bsgfx_instance;
    functions.bsgfx_tickInstances = _preval_bsgfx_tickInstances;
    functions.bsgfx_subtypeHasFlag = _preval_bsgfx_subtypeHasFlag;
    functions.bsgfx_renderSubtype = _preval_bsgfx_renderSubtype;
    functions.bsgfx_resetInstances = _preval_bsgfx_resetInstances;
    functions.bsgfx_resetSubtype = _preval_bsgfx_resetSubtype;
    functions.bsgfx_instanceHiResMesh = _preval_bsgfx_instanceHiResMesh;
    functions.bsgfx_instanceMesh = _preval_bsgfx_instanceMesh;
    functions.bsgfx_instanceBoneMesh = _preval_bsgfx_instanceBoneMesh;
    functions.bsgfx_instanceDepthlessLine = _preval_bsgfx_instanceDepthlessLine;
    functions.bsgfx_instanceLine = _preval_bsgfx_instanceLine;
    functions.bsgfx_instanceRay = _preval_bsgfx_instanceRay;
    functions.bsgfx_instanceAabb = _preval_bsgfx_instanceAabb;
    functions.bsgfx_instanceSphere = _preval_bsgfx_instanceSphere;
    functions.bsgfx_instanceCone = _preval_bsgfx_instanceCone;
    functions.bsgfx_instancePoint = _preval_bsgfx_instancePoint;
    functions.bsgfx_instanceQuad = _preval_bsgfx_instanceQuad;
    functions.bsgfx_instanceAtlas = _preval_bsgfx_instanceAtlas;
    functions.bsgfx_instanceAtlasFlipped = _preval_bsgfx_instanceAtlasFlipped;
    functions.bsgfx_instanceAtlas = _preval_bsgfx_instanceAtlas;
    functions.bsgfx_matrix = _preval_bsgfx_matrix;
    functions.bsgfx_renderFontSubtypes = _preval_bsgfx_renderFontSubtypes;
    functions.bsgfx_settingsEditor = _preval_bsgfx_settingsEditor;
    functions.bsgfx_renderFineShadowVolumes = _preval_bsgfx_renderFineShadowVolumes;
    functions.bsgfx_renderShadowVolumes = _preval_bsgfx_renderShadowVolumes;
    functions.bsgfx_computeShadowVolumes = _preval_bsgfx_computeShadowVolumes;
    functions.bsgfx_prefabModel = _preval_bsgfx_prefabModel;
    functions.bsgfx_package = _preval_bsgfx_package;
    functions.bsgfx_ini = _preval_bsgfx_ini;
    functions.bsgfx_bsmodDll = _preval_bsgfx_bsmodDll;
    functions.bsgfx_checkGFSDK = _preval_bsgfx_checkGFSDK;
    functions.bsgfx_logGFSDK = _preval_bsgfx_logGFSDK;
    functions.bsgfx_app = _preval_bsgfx_app;
    functions.bsgfx_callbacks = _preval_bsgfx_callbacks;
    functions.bsgfx_settings = _preval_bsgfx_settings;
    functions.poser = _preval_poser;
    functions.bsgfx_setCamera = _preval_bsgfx_setCamera;

    return functions;
}

