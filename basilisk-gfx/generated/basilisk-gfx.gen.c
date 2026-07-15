
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
#include <bsgfx_internal.gen.h>
#include <math.h>
#include <stdio.h>

bsgfx_FunctionTable next = { 0 };

bsgfx_Scene* bsgfx_currentScene()
{
    return next.bsgfx_currentScene();
}

void bsgfx_loadScene(
    const char* name)
{
    return next.bsgfx_loadScene(name);
}

int bsgfx_images()
{
    return next.bsgfx_images();
}

int bsgfx_samplers()
{
    return next.bsgfx_samplers();
}

int bsgfx_buffers()
{
    return next.bsgfx_buffers();
}

int bsgfx_batches()
{
    return next.bsgfx_batches();
}

int bsgfx_renderers()
{
    return next.bsgfx_renderers();
}

int bsgfx_queues()
{
    return next.bsgfx_queues();
}

int bsgfx_rayTracers()
{
    return next.bsgfx_rayTracers();
}

int bsgfx_atlases()
{
    return next.bsgfx_atlases();
}

int bsgfx_fonts()
{
    return next.bsgfx_fonts();
}

const char* bsgfx_materialCategoryName(
    bsgfx_MaterialCategory category)
{
    return next.bsgfx_materialCategoryName(category);
}

bs_List* bsgfx_materials()
{
    return next.bsgfx_materials();
}

bsgfx_Material* bsgfx_queryMaterialHash(
    bs_U64 hash)
{
    return next.bsgfx_queryMaterialHash(hash);
}

bsgfx_Material* bsgfx_fetchMaterial(
    int id)
{
    return next.bsgfx_fetchMaterial(id);
}

bsgfx_Material* bsgfx_queryMaterial(
    const char* name)
{
    return next.bsgfx_queryMaterial(name);
}

void bsgfx_loadMaterials()
{
    return next.bsgfx_loadMaterials();
}

bsgfx_Material* bsgfx_material(
    char* name, 
    int name_length)
{
    return next.bsgfx_material(name, name_length);
}

bsgfx_Material* bsgfx_materialV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bsgfx_material(_formatted, _length);
}

bsgfx_Material* bsgfx_materialF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bsgfx_Material* _return = bsgfx_materialV(format, args);
    va_end(args);
    return _return;
}

void bsgfx_highlightMaterial(
    int material_id, 
    bool auto_unhighlight)
{
    return next.bsgfx_highlightMaterial(material_id, auto_unhighlight);
}

void bsgfx_unhighlightMaterial(
    int material_id)
{
    return next.bsgfx_unhighlightMaterial(material_id);
}

void bsgfx_tickMaterials()
{
    return next.bsgfx_tickMaterials();
}

void bsgfx_shadowPipe()
{
    return next.bsgfx_shadowPipe();
}

void bsgfx_pipeline()
{
    return next.bsgfx_pipeline();
}

void bsgfx_onDeviceLost()
{
    return next.bsgfx_onDeviceLost();
}

void bsgfx_sweepCollisions(
    float sweep_radius, 
    bs_vec3 position)
{
    return next.bsgfx_sweepCollisions(sweep_radius, position);
}

bsgfx_Collider bsgfx_collider(
    bs_Aabb aabb, 
    bs_vec3 scale, 
    bs_ivec3 resolution, 
    float sweep_radius)
{
    return next.bsgfx_collider(aabb, scale, resolution, sweep_radius);
}

void bsgfx_applyCollisions(
    bsgfx_Collider* collider, 
    bs_vec3 position, 
    bs_vec3* velocity)
{
    return next.bsgfx_applyCollisions(collider, position, velocity);
}

void bsgfx_printCollisions(
    bsgfx_Collider* collider)
{
    return next.bsgfx_printCollisions(collider);
}

void bsgfx_instanceSweepCollisions()
{
    return next.bsgfx_instanceSweepCollisions();
}

void bsgfx_instanceCollider(
    bsgfx_Collider* collider, 
    bs_vec3 position, 
    bs_vec3* velocity)
{
    return next.bsgfx_instanceCollider(collider, position, velocity);
}

int bsgfx_skeleton(
    bs_Armature* armature)
{
    return next.bsgfx_skeleton(armature);
}

int bsgfx_animationFrame(
    bs_Animation* animation, 
    float time, 
    int num_frames)
{
    return next.bsgfx_animationFrame(animation, time, num_frames);
}

int bsgfx_animationFrameCount(
    bs_Animation* animation, 
    float time_scale)
{
    return next.bsgfx_animationFrameCount(animation, time_scale);
}

void bsgfx_applyAnimationVelocity(
    bsgfx_Animator* animator, 
    bs_vec3* velocity, 
    bs_vec2 input)
{
    return next.bsgfx_applyAnimationVelocity(animator, velocity, input);
}

void bsgfx_queueAnimation(
    bsgfx_Animator* animator, 
    int animation_id)
{
    return next.bsgfx_queueAnimation(animator, animation_id);
}

void bsgfx_runAnimator(
    bsgfx_Animator* animator, 
    bsgfx_AnimatorCallbacks callbacks)
{
    return next.bsgfx_runAnimator(animator, callbacks);
}

bsgfx_Animator bsgfx_animator(
    bs_Armature* armature, 
    int resting_animation_id, 
    int animations_count)
{
    return next.bsgfx_animator(armature, resting_animation_id, animations_count);
}

bs_List* bsgfx_subtypeInstances(
    int subtype)
{
    return next.bsgfx_subtypeInstances(subtype);
}

bs_Result bsgfx_iniInstances()
{
    return next.bsgfx_iniInstances();
}

void bsgfx_instanceType(
    int type, 
    int max_instance_count, 
    int bind_set, 
    int binding)
{
    return next.bsgfx_instanceType(type, max_instance_count, bind_set, binding);
}

bs_Range bsgfx_subtypeRange(
    int subtype)
{
    return next.bsgfx_subtypeRange(subtype);
}

void bsgfx_deleteSubtype(
    int subtype)
{
    return next.bsgfx_deleteSubtype(subtype);
}

int bsgfx_instanceCount(
    int subtype)
{
    return next.bsgfx_instanceCount(subtype);
}

int bsgfx_subtypeCount(
    int instance_type_id)
{
    return next.bsgfx_subtypeCount(instance_type_id);
}

const int* bsgfx_subtypes()
{
    return next.bsgfx_subtypes();
}

int bsgfx_subtype(
    int type, 
    bs_Batch* batch, 
    bs_U32 flags, 
    bs_Range range)
{
    return next.bsgfx_subtype(type, batch, flags, range);
}

int bsgfx_instance(
    int subtype, 
    const char* data, 
    int data_size, 
    bs_U32 flags, 
    unsigned int bone_index, 
    int id, 
    int material)
{
    return next.bsgfx_instance(subtype, data, data_size, flags, bone_index, id, material);
}

void bsgfx_tickInstances()
{
    return next.bsgfx_tickInstances();
}

bool bsgfx_subtypeHasFlag(
    int subtype, 
    bs_U32 flag)
{
    return next.bsgfx_subtypeHasFlag(subtype, flag);
}

void bsgfx_renderSubtype(
    int subtype, 
    bs_Pipeline* pipeline)
{
    return next.bsgfx_renderSubtype(subtype, pipeline);
}

void bsgfx_resetInstances()
{
    return next.bsgfx_resetInstances();
}

void bsgfx_resetSubtype(
    int subtype)
{
    return next.bsgfx_resetSubtype(subtype);
}

void bsgfx_instanceHiResMesh(
    bs_Mesh* mesh, 
    const bs_vec3* position, 
    const bs_vec4* rotation, 
    float scale, 
    int subtype_offset, 
    bool origin_at_center)
{
    return next.bsgfx_instanceHiResMesh(mesh, position, rotation, scale, subtype_offset, origin_at_center);
}

int bsgfx_instanceMesh(
    int subtype, 
    const bsgfx_MeshInstance* data, 
    bs_U32 flags, 
    int id, 
    int material)
{
    return next.bsgfx_instanceMesh(subtype, data, flags, id, material);
}

int bsgfx_instanceBoneMesh(
    int subtype, 
    const bsgfx_BoneInstance* data, 
    bs_U32 flags, 
    int id, 
    int material)
{
    return next.bsgfx_instanceBoneMesh(subtype, data, flags, id, material);
}

int bsgfx_instanceDepthlessLine(
    bs_vec3 start, 
    bs_vec3 end, 
    bs_RGBA color)
{
    return next.bsgfx_instanceDepthlessLine(start, end, color);
}

int bsgfx_instanceLine(
    bs_vec3 start, 
    bs_vec3 end, 
    bs_RGBA color)
{
    return next.bsgfx_instanceLine(start, end, color);
}

int bsgfx_instanceRay(
    const bs_Ray* ray, 
    bs_RGBA color)
{
    return next.bsgfx_instanceRay(ray, color);
}

bs_Range bsgfx_instanceAabb(
    const bs_Aabb* aabb, 
    bs_RGBA color)
{
    return next.bsgfx_instanceAabb(aabb, color);
}

int bsgfx_instanceSphere(
    bs_vec3 position, 
    float radius)
{
    return next.bsgfx_instanceSphere(position, radius);
}

int bsgfx_instanceCone(
    bs_mat4 transform, 
    float radius, 
    bs_U32 flags, 
    int id, 
    int material)
{
    return next.bsgfx_instanceCone(transform, radius, flags, id, material);
}

int bsgfx_instancePoint(
    bs_vec3 position, 
    bs_RGBA color, 
    float size)
{
    return next.bsgfx_instancePoint(position, color, size);
}

int bsgfx_instanceQuad(
    int subtype, 
    bs_mat4x3 transform, 
    bs_vec4 coords, 
    bs_U32 flags, 
    int id, 
    int material)
{
    return next.bsgfx_instanceQuad(subtype, transform, coords, flags, id, material);
}

int bsgfx_instanceAtlas(
    int subtype, 
    bs_mat4x3 transform, 
    int texture, 
    bs_U32 flags, 
    int id, 
    int material)
{
    return next.bsgfx_instanceAtlas(subtype, transform, texture, flags, id, material);
}

int bsgfx_instanceAtlasFlipped(
    int subtype, 
    bs_mat4x3 transform, 
    int texture, 
    bs_U32 flags, 
    int id, 
    int material)
{
    return next.bsgfx_instanceAtlasFlipped(subtype, transform, texture, flags, id, material);
}

bs_vec2 bsgfx_instanceText(
    int subtype, 
    bs_Font* font, 
    bsgfx_Text* params, 
    char* value, 
    int value_length)
{
    return next.bsgfx_instanceText(subtype, font, params, value, value_length);
}

bs_vec2 bsgfx_instanceTextV(
    int subtype, 
    bs_Font* font, 
    bsgfx_Text* params, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bsgfx_instanceText(subtype, font, params, _formatted, _length);
}

bs_vec2 bsgfx_instanceTextF(
    int subtype, 
    bs_Font* font, 
    bsgfx_Text* params, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_vec2 _return = bsgfx_instanceTextV(subtype, font, params, format, args);
    va_end(args);
    return _return;
}

bs_mat4x3 bsgfx_matrix(
    bs_vec3 position, 
    bs_vec3 scale)
{
    return next.bsgfx_matrix(position, scale);
}

void bsgfx_renderFontSubtypes()
{
    return next.bsgfx_renderFontSubtypes();
}

void bsgfx_settingsEditor(
    bs_List* result)
{
    return next.bsgfx_settingsEditor(result);
}

void bsgfx_renderFineShadowVolumes()
{
    return next.bsgfx_renderFineShadowVolumes();
}

void bsgfx_renderShadowVolumes()
{
    return next.bsgfx_renderShadowVolumes();
}

void bsgfx_computeShadowVolumes()
{
    return next.bsgfx_computeShadowVolumes();
}

bs_Model* bsgfx_prefabModel()
{
    return next.bsgfx_prefabModel();
}

int bsgfx_package()
{
    return next.bsgfx_package();
}

void bsgfx_ini(
    const char* name, 
    bs_U32 width, 
    bs_U32 height, 
    int argc, 
    char* argv[])
{
    return next.bsgfx_ini(name, width, height, argc, argv);
}

HINSTANCE bsgfx_bsmodDll()
{
    return next.bsgfx_bsmodDll();
}

void bsgfx_checkGFSDK(
    bs_U32 result)
{
    return next.bsgfx_checkGFSDK(result);
}

void bsgfx_logGFSDK(
    bs_U32 result)
{
    return next.bsgfx_logGFSDK(result);
}

bsgfx_Application* bsgfx_app()
{
    return next.bsgfx_app();
}

bsgfx_Callbacks* bsgfx_callbacks()
{
    return next.bsgfx_callbacks();
}

bsgfx_Settings* bsgfx_settings()
{
    return next.bsgfx_settings();
}

struct Poser* poser()
{
    return next.poser();
}

void bsgfx_setCamera(
    const bs_mat4* proj, 
    const bs_mat4* view)
{
    return next.bsgfx_setCamera(proj, view);
}

