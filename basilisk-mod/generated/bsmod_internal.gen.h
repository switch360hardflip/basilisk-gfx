
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
        
#ifndef BSMOD_INTERNAL_GEN_H
#define BSMOD_INTERNAL_GEN_H

#include <basilisk-mod.h>
#include <windows.h>


typedef void(__stdcall* PFN_bsmod_copyHoveringDataToBuffer)();
typedef void(__stdcall* PFN_bsmod_onIni)();
typedef void(__stdcall* PFN_bsmod_onLateIni)();
typedef void(__stdcall* PFN_bsmod_onCreateQuadSubtypes)(bs_Range range);
typedef void(__stdcall* PFN_bsmod_onLoad)();
typedef void(__stdcall* PFN_bsmod_bindAtlases)();
typedef void(__stdcall* PFN_bsmod_onGfxRender)();
typedef void(__stdcall* PFN_bsmod_onPostRender)();
typedef void(__stdcall* PFN_bsmod_onTick)();
typedef void(__stdcall* PFN_bsmod_onMap)(bsgfx_TypeId type_id, int id);
typedef void(__stdcall* PFN_bsmod_onTrack)();
typedef bs_Result(__stdcall* PFN_bsmod_packBindings)();
typedef void(__stdcall* PFN_bsmod_onCompileShader)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_onLoadTTF)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_onConvertBMFont)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_onPackAtlas)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_onPackModels)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_onPackBinary)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_onPackTextureArray)(bsmod_TrackParams params);
typedef void(__stdcall* PFN_bsmod_packAtlasTexture)(bsmod_AtlasPacker* packer, char* name, bs_RGBA* data, int width, int height, int category);
typedef bs_Result(__stdcall* PFN_bsmod_packAtlas)(bsmod_AtlasPacker* packer, int width, int height, char* package, char* resource_name);
typedef bsmod_AtlasPacker(__stdcall* PFN_bsmod_createAtlasPacker)();
typedef bs_Result(__stdcall* PFN_bsmod_packImageDirectory)(char* directory_name, char* package_name, char* resource_name);
typedef bs_Result(__stdcall* PFN_bsmod_packBMFont)(char* package_name, char* bmfont_path, char* png_path, char* value, int value_length);
typedef (__stdcall* PFN_bsmod_scrollbar)(const int* scroll, bsgfx_Scrollbar* out);
typedef void(__stdcall* PFN_bsmod_dividerWidget)(float width, int indent, bsgfx_Widget* out);
typedef void(__stdcall* PFN_bsmod_iconWidget)(const bsgfx_AtlasCache* cache, float align_height, bs_vec3 offset, bs_U32 advance_flags, bsgfx_Widget* out);
typedef bs_List*(__stdcall* PFN_bsmod_packages)();
typedef bsmod_Package*(__stdcall* PFN_bsmod_queryPackage)(const char* name);
typedef bsmod_Package*(__stdcall* PFN_bsmod_ensurePackage)(const char* name);
typedef bs_Result(__stdcall* PFN_bsmod_iniPackage)(const char* package_name);
typedef bs_Result(__stdcall* PFN_bsmod_packResource)(bs_ResourceType type, unsigned char* data, size_t data_size, const char* package_name, char* resource_name, int resource_name_length);
typedef bs_Result(__stdcall* PFN_bsmod_savePackage)(const char* name);
typedef void(__stdcall* PFN_bsmod_loadShaderReferences)();
typedef void(__stdcall* PFN_bsmod_updateShaderReferences)();
typedef void(__stdcall* PFN_bsmod_iniCompiler)();
typedef bs_Result(__stdcall* PFN_bsmod_compileShader)(char* path, char* name, char* package);
typedef void(__stdcall* PFN_bsmod_updateBindings)();
typedef void(__stdcall* PFN_bsmod_iniLisk)();
typedef void(__stdcall* PFN_bsmod_tickLisk)();
typedef bs_Queue*(__stdcall* PFN_bsmod_onQueue)();
typedef void(__stdcall* PFN_bsmod_queueRasterize)(const char* package, const char* name, bs_Callback callback);
typedef void(__stdcall* PFN_bsmod_pollRasterizer)();
typedef void(__stdcall* PFN_bsmod_beginRasterize)(bs_ivec2 render_size, bs_ivec2 output_size);
typedef void(__stdcall* PFN_bsmod_endRasterize)();
typedef bs_Result(__stdcall* PFN_bsmod_rasterizeInstance)(bs_PipelineHash pipeline_hash, int subtype, int instance_id, int category, char* name, int width, int height, size_t push_constant_size, unsigned char* push_constant);
typedef void(__stdcall* PFN_bsmod_instanceTransform)();
typedef void(__stdcall* PFN_bsmod_selectHoveringTypes)();
typedef bsgfx_TypeId(__stdcall* PFN_bsmod_queryType)(const char* plural);
typedef void(__stdcall* PFN_bsmod_delete)(bsgfx_TypeId type_id, int id);
typedef void(__stdcall* PFN_bsmod_deleteSelected)(bsgfx_TypeId type_id);
typedef bs_Result(__stdcall* PFN_bsmod_saveType)(bsgfx_TypeId id, char* value, int value_length);
typedef void*(__stdcall* PFN_bsmod_add)(bsgfx_TypeId id, void* data);
typedef bool(__stdcall* PFN_bsmod_isSelected)(bs_List* list, bsgfx_TypeId type, int id);
typedef void(__stdcall* PFN_bsmod_select)(bs_List* list, bsgfx_TypeId type, int id);
typedef void(__stdcall* PFN_bsmod_deselectAll)();
typedef void(__stdcall* PFN_bsmod_deselectRange)(bs_List* list, int first, int count);
typedef void(__stdcall* PFN_bsmod_deselectIndex)(bs_List* list, int id);
typedef void(__stdcall* PFN_bsmod_deselect)(bs_List* list, int id);
typedef int(__stdcall* PFN_bsmod_firstSelectedId)(bs_List* list);
typedef int(__stdcall* PFN_bsmod_lastSelectedId)(bs_List* list);
typedef void*(__stdcall* PFN_bsmod_removeFlexible)(bsgfx_TypeId type_id, int id);
typedef void*(__stdcall* PFN_bsmod_addFlexible)(bsgfx_TypeId type_id, int id, void* data);
typedef void(__stdcall* PFN_bsmod_readHoveringOutputs)();
typedef void(__stdcall* PFN_bsmod_readHoveringInstanceData)();
typedef void(__stdcall* PFN_bsmod_readHoveringVertex)();
typedef void(__stdcall* PFN_bsmod_editSelectedType)();
typedef void(__stdcall* PFN_bsmod_ensureTypeVersionsAreUpToDate)(int package_id);
typedef void(__stdcall* PFN_bsmod_renderBillboards)();
typedef void(__stdcall* PFN_bsmod_instanceUI)();
typedef void(__stdcall* PFN_bsmod_pushVecNWidget)(bs_List* widgets, const char* name, bs_vec3 offset, float width, float* v, int n);
typedef void(__stdcall* PFN_bsmod_pushInputWidget)(bs_List* widgets, void* value, bs_vec3 offset, float width, int name_padding, bool skip_advance, bool is_last, bool has_buttons, const char* name);
typedef void(__stdcall* PFN_bsmod_instanceTileContextMenu)(bs_vec3 position, bs_vec2 size);
typedef void(__stdcall* PFN_bsmod_instanceContextMenu)(bs_List* widgets, bs_vec3 position, bs_vec2 size);
typedef void(__stdcall* PFN_bsmod_pushContextMenuButton)(bs_List* widgets, bs_vec2 menu_size, bs_vec3 icon_offset, bsgfx_AtlasCache* icon, const char* name, int indent, PFN_bsmod_PushContextMenuButton action, bool expandable);
typedef void(__stdcall* PFN_bsmod_instanceGridMenu)(bs_vec3 position, bs_vec2 dimensions);
typedef void(__stdcall* PFN_bsmod_instanceLightBillboards)();
typedef bool(__stdcall* PFN_bsmod_onAddLightTick)(bsgfx_ButtonParams params);
typedef void(__stdcall* PFN_bsmod_pushMaterialWidgets)(bs_List* widgets, bs_vec2 background_size);
typedef void(__stdcall* PFN_bsmod_onDragMaterial)(bsmod_DraggingParams params);
typedef void(__stdcall* PFN_bsmod_onClickMaterialMenu)(bsmod_GridClickParams params);
typedef void(__stdcall* PFN_bsmod_rasterizeMaterialIcons)();
typedef void(__stdcall* PFN_bsmod_pushPrefabMenuWidgets)(bs_List* widgets, bs_vec2 background_size);
typedef bool(__stdcall* PFN_bsmod_instancePrefabPreview)(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering);
typedef void(__stdcall* PFN_bsmod_onDragPrefab)(bsmod_DraggingParams params);
typedef void(__stdcall* PFN_bsmod_rasterizePrefabIcons)();
typedef bool(__stdcall* PFN_bsmod_instancePrimitivePreview)(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering);
typedef void(__stdcall* PFN_bsmod_onDragPrimitive)(bsmod_DraggingParams params);
typedef void(__stdcall* PFN_bsmod_rasterizePrimitiveIcons)();
typedef void(__stdcall* PFN_bsmod_setSideMenuTab)(int id, bsmod_GridClickParams params);
typedef void(__stdcall* PFN_bsmod_instanceSideMenu)(bs_vec3 position, bs_vec2 dimensions);
typedef void(__stdcall* PFN_bsmod_pushTileMenuWidgets)(bs_List* widgets, bs_vec2 background_size);
typedef bool(__stdcall* PFN_bsmod_instanceTilePreview)(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering);
typedef void(__stdcall* PFN_bsmod_onDragTile)(bsmod_DraggingParams params);

typedef struct {
    PFN_bsmod_copyHoveringDataToBuffer bsmod_copyHoveringDataToBuffer;
    PFN_bsmod_onIni bsmod_onIni;
    PFN_bsmod_onLateIni bsmod_onLateIni;
    PFN_bsmod_onCreateQuadSubtypes bsmod_onCreateQuadSubtypes;
    PFN_bsmod_onLoad bsmod_onLoad;
    PFN_bsmod_bindAtlases bsmod_bindAtlases;
    PFN_bsmod_onGfxRender bsmod_onGfxRender;
    PFN_bsmod_onPostRender bsmod_onPostRender;
    PFN_bsmod_onTick bsmod_onTick;
    PFN_bsmod_onMap bsmod_onMap;
    PFN_bsmod_onTrack bsmod_onTrack;
    PFN_bsmod_packBindings bsmod_packBindings;
    PFN_bsmod_onCompileShader bsmod_onCompileShader;
    PFN_bsmod_onLoadTTF bsmod_onLoadTTF;
    PFN_bsmod_onConvertBMFont bsmod_onConvertBMFont;
    PFN_bsmod_onPackAtlas bsmod_onPackAtlas;
    PFN_bsmod_onPackModels bsmod_onPackModels;
    PFN_bsmod_onPackBinary bsmod_onPackBinary;
    PFN_bsmod_onPackTextureArray bsmod_onPackTextureArray;
    PFN_bsmod_packAtlasTexture bsmod_packAtlasTexture;
    PFN_bsmod_packAtlas bsmod_packAtlas;
    PFN_bsmod_createAtlasPacker bsmod_createAtlasPacker;
    PFN_bsmod_packImageDirectory bsmod_packImageDirectory;
    PFN_bsmod_packBMFont bsmod_packBMFont;
    PFN_bsmod_scrollbar bsmod_scrollbar;
    PFN_bsmod_dividerWidget bsmod_dividerWidget;
    PFN_bsmod_iconWidget bsmod_iconWidget;
    PFN_bsmod_packages bsmod_packages;
    PFN_bsmod_queryPackage bsmod_queryPackage;
    PFN_bsmod_ensurePackage bsmod_ensurePackage;
    PFN_bsmod_iniPackage bsmod_iniPackage;
    PFN_bsmod_packResource bsmod_packResource;
    PFN_bsmod_savePackage bsmod_savePackage;
    PFN_bsmod_loadShaderReferences bsmod_loadShaderReferences;
    PFN_bsmod_updateShaderReferences bsmod_updateShaderReferences;
    PFN_bsmod_iniCompiler bsmod_iniCompiler;
    PFN_bsmod_compileShader bsmod_compileShader;
    PFN_bsmod_updateBindings bsmod_updateBindings;
    PFN_bsmod_iniLisk bsmod_iniLisk;
    PFN_bsmod_tickLisk bsmod_tickLisk;
    PFN_bsmod_onQueue bsmod_onQueue;
    PFN_bsmod_queueRasterize bsmod_queueRasterize;
    PFN_bsmod_pollRasterizer bsmod_pollRasterizer;
    PFN_bsmod_beginRasterize bsmod_beginRasterize;
    PFN_bsmod_endRasterize bsmod_endRasterize;
    PFN_bsmod_rasterizeInstance bsmod_rasterizeInstance;
    PFN_bsmod_instanceTransform bsmod_instanceTransform;
    PFN_bsmod_selectHoveringTypes bsmod_selectHoveringTypes;
    PFN_bsmod_queryType bsmod_queryType;
    PFN_bsmod_delete bsmod_delete;
    PFN_bsmod_deleteSelected bsmod_deleteSelected;
    PFN_bsmod_saveType bsmod_saveType;
    PFN_bsmod_add bsmod_add;
    PFN_bsmod_isSelected bsmod_isSelected;
    PFN_bsmod_select bsmod_select;
    PFN_bsmod_deselectAll bsmod_deselectAll;
    PFN_bsmod_deselectRange bsmod_deselectRange;
    PFN_bsmod_deselectIndex bsmod_deselectIndex;
    PFN_bsmod_deselect bsmod_deselect;
    PFN_bsmod_firstSelectedId bsmod_firstSelectedId;
    PFN_bsmod_lastSelectedId bsmod_lastSelectedId;
    PFN_bsmod_removeFlexible bsmod_removeFlexible;
    PFN_bsmod_addFlexible bsmod_addFlexible;
    PFN_bsmod_readHoveringOutputs bsmod_readHoveringOutputs;
    PFN_bsmod_readHoveringInstanceData bsmod_readHoveringInstanceData;
    PFN_bsmod_readHoveringVertex bsmod_readHoveringVertex;
    PFN_bsmod_editSelectedType bsmod_editSelectedType;
    PFN_bsmod_ensureTypeVersionsAreUpToDate bsmod_ensureTypeVersionsAreUpToDate;
    PFN_bsmod_renderBillboards bsmod_renderBillboards;
    PFN_bsmod_instanceUI bsmod_instanceUI;
    PFN_bsmod_pushVecNWidget bsmod_pushVecNWidget;
    PFN_bsmod_pushInputWidget bsmod_pushInputWidget;
    PFN_bsmod_instanceTileContextMenu bsmod_instanceTileContextMenu;
    PFN_bsmod_instanceContextMenu bsmod_instanceContextMenu;
    PFN_bsmod_pushContextMenuButton bsmod_pushContextMenuButton;
    PFN_bsmod_instanceGridMenu bsmod_instanceGridMenu;
    PFN_bsmod_instanceLightBillboards bsmod_instanceLightBillboards;
    PFN_bsmod_onAddLightTick bsmod_onAddLightTick;
    PFN_bsmod_pushMaterialWidgets bsmod_pushMaterialWidgets;
    PFN_bsmod_onDragMaterial bsmod_onDragMaterial;
    PFN_bsmod_onClickMaterialMenu bsmod_onClickMaterialMenu;
    PFN_bsmod_rasterizeMaterialIcons bsmod_rasterizeMaterialIcons;
    PFN_bsmod_pushPrefabMenuWidgets bsmod_pushPrefabMenuWidgets;
    PFN_bsmod_instancePrefabPreview bsmod_instancePrefabPreview;
    PFN_bsmod_onDragPrefab bsmod_onDragPrefab;
    PFN_bsmod_rasterizePrefabIcons bsmod_rasterizePrefabIcons;
    PFN_bsmod_instancePrimitivePreview bsmod_instancePrimitivePreview;
    PFN_bsmod_onDragPrimitive bsmod_onDragPrimitive;
    PFN_bsmod_rasterizePrimitiveIcons bsmod_rasterizePrimitiveIcons;
    PFN_bsmod_setSideMenuTab bsmod_setSideMenuTab;
    PFN_bsmod_instanceSideMenu bsmod_instanceSideMenu;
    PFN_bsmod_pushTileMenuWidgets bsmod_pushTileMenuWidgets;
    PFN_bsmod_instanceTilePreview bsmod_instanceTilePreview;
    PFN_bsmod_onDragTile bsmod_onDragTile;
} bsmod_FunctionTable;

static inline bsmod_FunctionTable _bsmod_getFunctions() {
    bsmod_FunctionTable functions;

    HMODULE module = NULL;
    GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCSTR)(&_bsmod_getFunctions),
        &module);

    functions.bsmod_copyHoveringDataToBuffer = (PFN_bsmod_copyHoveringDataToBuffer)GetProcAddress(module, "_bsmod_copyHoveringDataToBuffer");
    functions.bsmod_onIni = (PFN_bsmod_onIni)GetProcAddress(module, "_bsmod_onIni");
    functions.bsmod_onLateIni = (PFN_bsmod_onLateIni)GetProcAddress(module, "_bsmod_onLateIni");
    functions.bsmod_onCreateQuadSubtypes = (PFN_bsmod_onCreateQuadSubtypes)GetProcAddress(module, "_bsmod_onCreateQuadSubtypes");
    functions.bsmod_onLoad = (PFN_bsmod_onLoad)GetProcAddress(module, "_bsmod_onLoad");
    functions.bsmod_bindAtlases = (PFN_bsmod_bindAtlases)GetProcAddress(module, "_bsmod_bindAtlases");
    functions.bsmod_onGfxRender = (PFN_bsmod_onGfxRender)GetProcAddress(module, "_bsmod_onGfxRender");
    functions.bsmod_onPostRender = (PFN_bsmod_onPostRender)GetProcAddress(module, "_bsmod_onPostRender");
    functions.bsmod_onTick = (PFN_bsmod_onTick)GetProcAddress(module, "_bsmod_onTick");
    functions.bsmod_onMap = (PFN_bsmod_onMap)GetProcAddress(module, "_bsmod_onMap");
    functions.bsmod_onTrack = (PFN_bsmod_onTrack)GetProcAddress(module, "_bsmod_onTrack");
    functions.bsmod_packBindings = (PFN_bsmod_packBindings)GetProcAddress(module, "_bsmod_packBindings");
    functions.bsmod_onCompileShader = (PFN_bsmod_onCompileShader)GetProcAddress(module, "_bsmod_onCompileShader");
    functions.bsmod_onLoadTTF = (PFN_bsmod_onLoadTTF)GetProcAddress(module, "_bsmod_onLoadTTF");
    functions.bsmod_onConvertBMFont = (PFN_bsmod_onConvertBMFont)GetProcAddress(module, "_bsmod_onConvertBMFont");
    functions.bsmod_onPackAtlas = (PFN_bsmod_onPackAtlas)GetProcAddress(module, "_bsmod_onPackAtlas");
    functions.bsmod_onPackModels = (PFN_bsmod_onPackModels)GetProcAddress(module, "_bsmod_onPackModels");
    functions.bsmod_onPackBinary = (PFN_bsmod_onPackBinary)GetProcAddress(module, "_bsmod_onPackBinary");
    functions.bsmod_onPackTextureArray = (PFN_bsmod_onPackTextureArray)GetProcAddress(module, "_bsmod_onPackTextureArray");
    functions.bsmod_packAtlasTexture = (PFN_bsmod_packAtlasTexture)GetProcAddress(module, "_bsmod_packAtlasTexture");
    functions.bsmod_packAtlas = (PFN_bsmod_packAtlas)GetProcAddress(module, "_bsmod_packAtlas");
    functions.bsmod_createAtlasPacker = (PFN_bsmod_createAtlasPacker)GetProcAddress(module, "_bsmod_createAtlasPacker");
    functions.bsmod_packImageDirectory = (PFN_bsmod_packImageDirectory)GetProcAddress(module, "_bsmod_packImageDirectory");
    functions.bsmod_packBMFont = (PFN_bsmod_packBMFont)GetProcAddress(module, "_bsmod_packBMFont");
    functions.bsmod_scrollbar = (PFN_bsmod_scrollbar)GetProcAddress(module, "_bsmod_scrollbar");
    functions.bsmod_dividerWidget = (PFN_bsmod_dividerWidget)GetProcAddress(module, "_bsmod_dividerWidget");
    functions.bsmod_iconWidget = (PFN_bsmod_iconWidget)GetProcAddress(module, "_bsmod_iconWidget");
    functions.bsmod_packages = (PFN_bsmod_packages)GetProcAddress(module, "_bsmod_packages");
    functions.bsmod_queryPackage = (PFN_bsmod_queryPackage)GetProcAddress(module, "_bsmod_queryPackage");
    functions.bsmod_ensurePackage = (PFN_bsmod_ensurePackage)GetProcAddress(module, "_bsmod_ensurePackage");
    functions.bsmod_iniPackage = (PFN_bsmod_iniPackage)GetProcAddress(module, "_bsmod_iniPackage");
    functions.bsmod_packResource = (PFN_bsmod_packResource)GetProcAddress(module, "_bsmod_packResource");
    functions.bsmod_savePackage = (PFN_bsmod_savePackage)GetProcAddress(module, "_bsmod_savePackage");
    functions.bsmod_loadShaderReferences = (PFN_bsmod_loadShaderReferences)GetProcAddress(module, "_bsmod_loadShaderReferences");
    functions.bsmod_updateShaderReferences = (PFN_bsmod_updateShaderReferences)GetProcAddress(module, "_bsmod_updateShaderReferences");
    functions.bsmod_iniCompiler = (PFN_bsmod_iniCompiler)GetProcAddress(module, "_bsmod_iniCompiler");
    functions.bsmod_compileShader = (PFN_bsmod_compileShader)GetProcAddress(module, "_bsmod_compileShader");
    functions.bsmod_updateBindings = (PFN_bsmod_updateBindings)GetProcAddress(module, "_bsmod_updateBindings");
    functions.bsmod_iniLisk = (PFN_bsmod_iniLisk)GetProcAddress(module, "_bsmod_iniLisk");
    functions.bsmod_tickLisk = (PFN_bsmod_tickLisk)GetProcAddress(module, "_bsmod_tickLisk");
    functions.bsmod_onQueue = (PFN_bsmod_onQueue)GetProcAddress(module, "_bsmod_onQueue");
    functions.bsmod_queueRasterize = (PFN_bsmod_queueRasterize)GetProcAddress(module, "_bsmod_queueRasterize");
    functions.bsmod_pollRasterizer = (PFN_bsmod_pollRasterizer)GetProcAddress(module, "_bsmod_pollRasterizer");
    functions.bsmod_beginRasterize = (PFN_bsmod_beginRasterize)GetProcAddress(module, "_bsmod_beginRasterize");
    functions.bsmod_endRasterize = (PFN_bsmod_endRasterize)GetProcAddress(module, "_bsmod_endRasterize");
    functions.bsmod_rasterizeInstance = (PFN_bsmod_rasterizeInstance)GetProcAddress(module, "_bsmod_rasterizeInstance");
    functions.bsmod_instanceTransform = (PFN_bsmod_instanceTransform)GetProcAddress(module, "_bsmod_instanceTransform");
    functions.bsmod_selectHoveringTypes = (PFN_bsmod_selectHoveringTypes)GetProcAddress(module, "_bsmod_selectHoveringTypes");
    functions.bsmod_queryType = (PFN_bsmod_queryType)GetProcAddress(module, "_bsmod_queryType");
    functions.bsmod_delete = (PFN_bsmod_delete)GetProcAddress(module, "_bsmod_delete");
    functions.bsmod_deleteSelected = (PFN_bsmod_deleteSelected)GetProcAddress(module, "_bsmod_deleteSelected");
    functions.bsmod_saveType = (PFN_bsmod_saveType)GetProcAddress(module, "_bsmod_saveType");
    functions.bsmod_add = (PFN_bsmod_add)GetProcAddress(module, "_bsmod_add");
    functions.bsmod_isSelected = (PFN_bsmod_isSelected)GetProcAddress(module, "_bsmod_isSelected");
    functions.bsmod_select = (PFN_bsmod_select)GetProcAddress(module, "_bsmod_select");
    functions.bsmod_deselectAll = (PFN_bsmod_deselectAll)GetProcAddress(module, "_bsmod_deselectAll");
    functions.bsmod_deselectRange = (PFN_bsmod_deselectRange)GetProcAddress(module, "_bsmod_deselectRange");
    functions.bsmod_deselectIndex = (PFN_bsmod_deselectIndex)GetProcAddress(module, "_bsmod_deselectIndex");
    functions.bsmod_deselect = (PFN_bsmod_deselect)GetProcAddress(module, "_bsmod_deselect");
    functions.bsmod_firstSelectedId = (PFN_bsmod_firstSelectedId)GetProcAddress(module, "_bsmod_firstSelectedId");
    functions.bsmod_lastSelectedId = (PFN_bsmod_lastSelectedId)GetProcAddress(module, "_bsmod_lastSelectedId");
    functions.bsmod_removeFlexible = (PFN_bsmod_removeFlexible)GetProcAddress(module, "_bsmod_removeFlexible");
    functions.bsmod_addFlexible = (PFN_bsmod_addFlexible)GetProcAddress(module, "_bsmod_addFlexible");
    functions.bsmod_readHoveringOutputs = (PFN_bsmod_readHoveringOutputs)GetProcAddress(module, "_bsmod_readHoveringOutputs");
    functions.bsmod_readHoveringInstanceData = (PFN_bsmod_readHoveringInstanceData)GetProcAddress(module, "_bsmod_readHoveringInstanceData");
    functions.bsmod_readHoveringVertex = (PFN_bsmod_readHoveringVertex)GetProcAddress(module, "_bsmod_readHoveringVertex");
    functions.bsmod_editSelectedType = (PFN_bsmod_editSelectedType)GetProcAddress(module, "_bsmod_editSelectedType");
    functions.bsmod_ensureTypeVersionsAreUpToDate = (PFN_bsmod_ensureTypeVersionsAreUpToDate)GetProcAddress(module, "_bsmod_ensureTypeVersionsAreUpToDate");
    functions.bsmod_renderBillboards = (PFN_bsmod_renderBillboards)GetProcAddress(module, "_bsmod_renderBillboards");
    functions.bsmod_instanceUI = (PFN_bsmod_instanceUI)GetProcAddress(module, "_bsmod_instanceUI");
    functions.bsmod_pushVecNWidget = (PFN_bsmod_pushVecNWidget)GetProcAddress(module, "_bsmod_pushVecNWidget");
    functions.bsmod_pushInputWidget = (PFN_bsmod_pushInputWidget)GetProcAddress(module, "_bsmod_pushInputWidget");
    functions.bsmod_instanceTileContextMenu = (PFN_bsmod_instanceTileContextMenu)GetProcAddress(module, "_bsmod_instanceTileContextMenu");
    functions.bsmod_instanceContextMenu = (PFN_bsmod_instanceContextMenu)GetProcAddress(module, "_bsmod_instanceContextMenu");
    functions.bsmod_pushContextMenuButton = (PFN_bsmod_pushContextMenuButton)GetProcAddress(module, "_bsmod_pushContextMenuButton");
    functions.bsmod_instanceGridMenu = (PFN_bsmod_instanceGridMenu)GetProcAddress(module, "_bsmod_instanceGridMenu");
    functions.bsmod_instanceLightBillboards = (PFN_bsmod_instanceLightBillboards)GetProcAddress(module, "_bsmod_instanceLightBillboards");
    functions.bsmod_onAddLightTick = (PFN_bsmod_onAddLightTick)GetProcAddress(module, "_bsmod_onAddLightTick");
    functions.bsmod_pushMaterialWidgets = (PFN_bsmod_pushMaterialWidgets)GetProcAddress(module, "_bsmod_pushMaterialWidgets");
    functions.bsmod_onDragMaterial = (PFN_bsmod_onDragMaterial)GetProcAddress(module, "_bsmod_onDragMaterial");
    functions.bsmod_onClickMaterialMenu = (PFN_bsmod_onClickMaterialMenu)GetProcAddress(module, "_bsmod_onClickMaterialMenu");
    functions.bsmod_rasterizeMaterialIcons = (PFN_bsmod_rasterizeMaterialIcons)GetProcAddress(module, "_bsmod_rasterizeMaterialIcons");
    functions.bsmod_pushPrefabMenuWidgets = (PFN_bsmod_pushPrefabMenuWidgets)GetProcAddress(module, "_bsmod_pushPrefabMenuWidgets");
    functions.bsmod_instancePrefabPreview = (PFN_bsmod_instancePrefabPreview)GetProcAddress(module, "_bsmod_instancePrefabPreview");
    functions.bsmod_onDragPrefab = (PFN_bsmod_onDragPrefab)GetProcAddress(module, "_bsmod_onDragPrefab");
    functions.bsmod_rasterizePrefabIcons = (PFN_bsmod_rasterizePrefabIcons)GetProcAddress(module, "_bsmod_rasterizePrefabIcons");
    functions.bsmod_instancePrimitivePreview = (PFN_bsmod_instancePrimitivePreview)GetProcAddress(module, "_bsmod_instancePrimitivePreview");
    functions.bsmod_onDragPrimitive = (PFN_bsmod_onDragPrimitive)GetProcAddress(module, "_bsmod_onDragPrimitive");
    functions.bsmod_rasterizePrimitiveIcons = (PFN_bsmod_rasterizePrimitiveIcons)GetProcAddress(module, "_bsmod_rasterizePrimitiveIcons");
    functions.bsmod_setSideMenuTab = (PFN_bsmod_setSideMenuTab)GetProcAddress(module, "_bsmod_setSideMenuTab");
    functions.bsmod_instanceSideMenu = (PFN_bsmod_instanceSideMenu)GetProcAddress(module, "_bsmod_instanceSideMenu");
    functions.bsmod_pushTileMenuWidgets = (PFN_bsmod_pushTileMenuWidgets)GetProcAddress(module, "_bsmod_pushTileMenuWidgets");
    functions.bsmod_instanceTilePreview = (PFN_bsmod_instanceTilePreview)GetProcAddress(module, "_bsmod_instanceTilePreview");
    functions.bsmod_onDragTile = (PFN_bsmod_onDragTile)GetProcAddress(module, "_bsmod_onDragTile");

    return functions;
}

#endif
