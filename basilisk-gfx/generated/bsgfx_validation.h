
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
        
#ifndef BSGFX_VALIDATION_H
#define BSGFX_VALIDATION_H

#include <basilisk-gfx.h>
#include <bsgfx_internal.h>
#include <windows.h>

static inline bsgfx_FunctionTable _val_bsgfx_getFunctions() {
    bsgfx_FunctionTable functions;

    HMODULE module = NULL;
    GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCSTR)(&_val_bsgfx_getFunctions),
        &module);

    functions.bsgfx_currentScene = (PFN_bsgfx_currentScene)GetProcAddress(module, "_val_bsgfx_currentScene");
    functions.bsgfx_loadScene = (PFN_bsgfx_loadScene)GetProcAddress(module, "_val_bsgfx_loadScene");
    functions.bsgfx_images = (PFN_bsgfx_images)GetProcAddress(module, "_val_bsgfx_images");
    functions.bsgfx_samplers = (PFN_bsgfx_samplers)GetProcAddress(module, "_val_bsgfx_samplers");
    functions.bsgfx_buffers = (PFN_bsgfx_buffers)GetProcAddress(module, "_val_bsgfx_buffers");
    functions.bsgfx_batches = (PFN_bsgfx_batches)GetProcAddress(module, "_val_bsgfx_batches");
    functions.bsgfx_renderers = (PFN_bsgfx_renderers)GetProcAddress(module, "_val_bsgfx_renderers");
    functions.bsgfx_queues = (PFN_bsgfx_queues)GetProcAddress(module, "_val_bsgfx_queues");
    functions.bsgfx_rayTracers = (PFN_bsgfx_rayTracers)GetProcAddress(module, "_val_bsgfx_rayTracers");
    functions.bsgfx_atlases = (PFN_bsgfx_atlases)GetProcAddress(module, "_val_bsgfx_atlases");
    functions.bsgfx_fonts = (PFN_bsgfx_fonts)GetProcAddress(module, "_val_bsgfx_fonts");
    functions.bsgfx_materialCategoryName = (PFN_bsgfx_materialCategoryName)GetProcAddress(module, "_val_bsgfx_materialCategoryName");
    functions.bsgfx_materials = (PFN_bsgfx_materials)GetProcAddress(module, "_val_bsgfx_materials");
    functions.bsgfx_queryMaterialHash = (PFN_bsgfx_queryMaterialHash)GetProcAddress(module, "_val_bsgfx_queryMaterialHash");
    functions.bsgfx_materialName = (PFN_bsgfx_materialName)GetProcAddress(module, "_val_bsgfx_materialName");
    functions.bsgfx_queryMaterial = (PFN_bsgfx_queryMaterial)GetProcAddress(module, "_val_bsgfx_queryMaterial");
    functions.bsgfx_materialHash = (PFN_bsgfx_materialHash)GetProcAddress(module, "_val_bsgfx_materialHash");
    functions.bsgfx_allocateMaterials = (PFN_bsgfx_allocateMaterials)GetProcAddress(module, "_val_bsgfx_allocateMaterials");
    functions.bsgfx_loadMaterials = (PFN_bsgfx_loadMaterials)GetProcAddress(module, "_val_bsgfx_loadMaterials");
    functions.bsgfx_material = (PFN_bsgfx_material)GetProcAddress(module, "_val_bsgfx_material");
    functions.bsgfx_highlightMaterial = (PFN_bsgfx_highlightMaterial)GetProcAddress(module, "_val_bsgfx_highlightMaterial");
    functions.bsgfx_unhighlightMaterial = (PFN_bsgfx_unhighlightMaterial)GetProcAddress(module, "_val_bsgfx_unhighlightMaterial");
    functions.bsgfx_tickMaterials = (PFN_bsgfx_tickMaterials)GetProcAddress(module, "_val_bsgfx_tickMaterials");
    functions.bsgfx_shadowPipe = (PFN_bsgfx_shadowPipe)GetProcAddress(module, "_val_bsgfx_shadowPipe");
    functions.bsgfx_pipeline = (PFN_bsgfx_pipeline)GetProcAddress(module, "_val_bsgfx_pipeline");
    functions.bsgfx_onDeviceLost = (PFN_bsgfx_onDeviceLost)GetProcAddress(module, "_val_bsgfx_onDeviceLost");
    functions.bsgfx_sweepCollisions = (PFN_bsgfx_sweepCollisions)GetProcAddress(module, "_val_bsgfx_sweepCollisions");
    functions.bsgfx_collider = (PFN_bsgfx_collider)GetProcAddress(module, "_val_bsgfx_collider");
    functions.bsgfx_applyCollisions = (PFN_bsgfx_applyCollisions)GetProcAddress(module, "_val_bsgfx_applyCollisions");
    functions.bsgfx_printCollisions = (PFN_bsgfx_printCollisions)GetProcAddress(module, "_val_bsgfx_printCollisions");
    functions.bsgfx_instanceSweepCollisions = (PFN_bsgfx_instanceSweepCollisions)GetProcAddress(module, "_val_bsgfx_instanceSweepCollisions");
    functions.bsgfx_instanceCollider = (PFN_bsgfx_instanceCollider)GetProcAddress(module, "_val_bsgfx_instanceCollider");
    functions.bsgfx_skeleton = (PFN_bsgfx_skeleton)GetProcAddress(module, "_val_bsgfx_skeleton");
    functions.bsgfx_animationFrame = (PFN_bsgfx_animationFrame)GetProcAddress(module, "_val_bsgfx_animationFrame");
    functions.bsgfx_animationFrameCount = (PFN_bsgfx_animationFrameCount)GetProcAddress(module, "_val_bsgfx_animationFrameCount");
    functions.bsgfx_applyAnimationVelocity = (PFN_bsgfx_applyAnimationVelocity)GetProcAddress(module, "_val_bsgfx_applyAnimationVelocity");
    functions.bsgfx_queueAnimation = (PFN_bsgfx_queueAnimation)GetProcAddress(module, "_val_bsgfx_queueAnimation");
    functions.bsgfx_runAnimator = (PFN_bsgfx_runAnimator)GetProcAddress(module, "_val_bsgfx_runAnimator");
    functions.bsgfx_animator = (PFN_bsgfx_animator)GetProcAddress(module, "_val_bsgfx_animator");
    functions.bsgfx_subtypeInstances = (PFN_bsgfx_subtypeInstances)GetProcAddress(module, "_val_bsgfx_subtypeInstances");
    functions.bsgfx_iniInstances = (PFN_bsgfx_iniInstances)GetProcAddress(module, "_val_bsgfx_iniInstances");
    functions.bsgfx_instanceType = (PFN_bsgfx_instanceType)GetProcAddress(module, "_val_bsgfx_instanceType");
    functions.bsgfx_subtypeRange = (PFN_bsgfx_subtypeRange)GetProcAddress(module, "_val_bsgfx_subtypeRange");
    functions.bsgfx_deleteSubtype = (PFN_bsgfx_deleteSubtype)GetProcAddress(module, "_val_bsgfx_deleteSubtype");
    functions.bsgfx_instanceCount = (PFN_bsgfx_instanceCount)GetProcAddress(module, "_val_bsgfx_instanceCount");
    functions.bsgfx_subtypeCount = (PFN_bsgfx_subtypeCount)GetProcAddress(module, "_val_bsgfx_subtypeCount");
    functions.bsgfx_subtypes = (PFN_bsgfx_subtypes)GetProcAddress(module, "_val_bsgfx_subtypes");
    functions.bsgfx_subtype = (PFN_bsgfx_subtype)GetProcAddress(module, "_val_bsgfx_subtype");
    functions.bsgfx_instance = (PFN_bsgfx_instance)GetProcAddress(module, "_val_bsgfx_instance");
    functions.bsgfx_tickInstances = (PFN_bsgfx_tickInstances)GetProcAddress(module, "_val_bsgfx_tickInstances");
    functions.bsgfx_subtypeHasFlag = (PFN_bsgfx_subtypeHasFlag)GetProcAddress(module, "_val_bsgfx_subtypeHasFlag");
    functions.bsgfx_renderSubtype = (PFN_bsgfx_renderSubtype)GetProcAddress(module, "_val_bsgfx_renderSubtype");
    functions.bsgfx_resetInstances = (PFN_bsgfx_resetInstances)GetProcAddress(module, "_val_bsgfx_resetInstances");
    functions.bsgfx_resetSubtype = (PFN_bsgfx_resetSubtype)GetProcAddress(module, "_val_bsgfx_resetSubtype");
    functions.bsgfx_instanceHiResMesh = (PFN_bsgfx_instanceHiResMesh)GetProcAddress(module, "_val_bsgfx_instanceHiResMesh");
    functions.bsgfx_renderFontSubtypes = (PFN_bsgfx_renderFontSubtypes)GetProcAddress(module, "_val_bsgfx_renderFontSubtypes");
    functions.bsgfx_settingsEditor = (PFN_bsgfx_settingsEditor)GetProcAddress(module, "_val_bsgfx_settingsEditor");
    functions.bsgfx_renderFineShadowVolumes = (PFN_bsgfx_renderFineShadowVolumes)GetProcAddress(module, "_val_bsgfx_renderFineShadowVolumes");
    functions.bsgfx_renderShadowVolumes = (PFN_bsgfx_renderShadowVolumes)GetProcAddress(module, "_val_bsgfx_renderShadowVolumes");
    functions.bsgfx_computeShadowVolumes = (PFN_bsgfx_computeShadowVolumes)GetProcAddress(module, "_val_bsgfx_computeShadowVolumes");
    functions.bsgfx_prefabModel = (PFN_bsgfx_prefabModel)GetProcAddress(module, "_val_bsgfx_prefabModel");
    functions.bsgfx_package = (PFN_bsgfx_package)GetProcAddress(module, "_val_bsgfx_package");
    functions.bsgfx_ini = (PFN_bsgfx_ini)GetProcAddress(module, "_val_bsgfx_ini");
    functions.bsgfx_bsmodDll = (PFN_bsgfx_bsmodDll)GetProcAddress(module, "_val_bsgfx_bsmodDll");
    functions.bsgfx_checkGFSDK = (PFN_bsgfx_checkGFSDK)GetProcAddress(module, "_val_bsgfx_checkGFSDK");
    functions.bsgfx_logGFSDK = (PFN_bsgfx_logGFSDK)GetProcAddress(module, "_val_bsgfx_logGFSDK");
    functions.bsgfx_app = (PFN_bsgfx_app)GetProcAddress(module, "_val_bsgfx_app");
    functions.bsgfx_callbacks = (PFN_bsgfx_callbacks)GetProcAddress(module, "_val_bsgfx_callbacks");
    functions.bsgfx_settings = (PFN_bsgfx_settings)GetProcAddress(module, "_val_bsgfx_settings");
    functions.poser = (PFN_poser)GetProcAddress(module, "_val_poser");
    functions.bsgfx_setCamera = (PFN_bsgfx_setCamera)GetProcAddress(module, "_val_bsgfx_setCamera");

    return functions;
}

#endif
