
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
        
#include <basilisk-mod.h>
#include <bsmod_internal.gen.h>

static bsmod_FunctionTable next = { 0 };

void _preval_bsmod_setFunctions(bsmod_FunctionTable functions) {
    next = functions;
}

static void _preval_bsmod_copyHoveringDataToBuffer() {
    next.bsmod_copyHoveringDataToBuffer();
}

static void _preval_bsmod_onIni() {
    next.bsmod_onIni();
}

static void _preval_bsmod_onLateIni() {
    next.bsmod_onLateIni();
}

static void _preval_bsmod_onCreateQuadSubtypes(bs_Range range) {
    next.bsmod_onCreateQuadSubtypes(range);
}

static void _preval_bsmod_onLoad() {
    next.bsmod_onLoad();
}

static void _preval_bsmod_bindAtlases() {
    next.bsmod_bindAtlases();
}

static void _preval_bsmod_onGfxRender() {
    next.bsmod_onGfxRender();
}

static void _preval_bsmod_onPostRender() {
    next.bsmod_onPostRender();
}

static void _preval_bsmod_onTick() {
    next.bsmod_onTick();
}

static void _preval_bsmod_onMap(bsgfx_TypeId type_id, int id) {
    next.bsmod_onMap(type_id, id);
}

static void _preval_bsmod_onTrack() {
    next.bsmod_onTrack();
}

static bs_Result _preval_bsmod_packBindings() {
    return next.bsmod_packBindings();
}

static void _preval_bsmod_onCompileShader(bsmod_TrackParams params) {
    next.bsmod_onCompileShader(params);
}

static void _preval_bsmod_onLoadTTF(bsmod_TrackParams params) {
    next.bsmod_onLoadTTF(params);
}

static void _preval_bsmod_onConvertBMFont(bsmod_TrackParams params) {
    next.bsmod_onConvertBMFont(params);
}

static void _preval_bsmod_onPackAtlas(bsmod_TrackParams params) {
    next.bsmod_onPackAtlas(params);
}

static void _preval_bsmod_onPackModels(bsmod_TrackParams params) {
    next.bsmod_onPackModels(params);
}

static void _preval_bsmod_onPackBinary(bsmod_TrackParams params) {
    next.bsmod_onPackBinary(params);
}

static void _preval_bsmod_onPackTextureArray(bsmod_TrackParams params) {
    next.bsmod_onPackTextureArray(params);
}

static void _preval_bsmod_packAtlasTexture(bsmod_AtlasPacker* packer, char* name, bs_RGBA* data, int width, int height, int category) {
    BSMOD_VALIDATE(packer != NULL, ,);
    BSMOD_VALIDATE(name != NULL, ,);
    BSMOD_VALIDATE(data != NULL, ,);
    next.bsmod_packAtlasTexture(packer, name, data, width, height, category);
}

static bs_Result _preval_bsmod_packAtlas(bsmod_AtlasPacker* packer, int width, int height, char* package, char* resource_name) {
    BSMOD_VALIDATE(packer != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(package != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(resource_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_packAtlas(packer, width, height, package, resource_name);
}

static bsmod_AtlasPacker _preval_bsmod_createAtlasPacker() {
    return next.bsmod_createAtlasPacker();
}

static bs_Result _preval_bsmod_packImageDirectory(char* directory_name, char* package_name, char* resource_name) {
    BSMOD_VALIDATE(directory_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(package_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(resource_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_packImageDirectory(directory_name, package_name, resource_name);
}

static bs_Result _preval_bsmod_packBMFont(char* package_name, char* bmfont_path, char* png_path, char* value, int value_length) {
    BSMOD_VALIDATE(package_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(bmfont_path != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(png_path != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(value != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_packBMFont(package_name, bmfont_path, png_path, value, value_length);
}

static  _preval_bsmod_scrollbar(const int* scroll, bsgfx_Scrollbar* out) {
    BSMOD_VALIDATE(scroll != NULL, 0,);
    BSMOD_VALIDATE(out != NULL, 0,);
    return next.bsmod_scrollbar(scroll, out);
}

static void _preval_bsmod_dividerWidget(float width, int indent, bsgfx_Widget* out) {
    BSMOD_VALIDATE(out != NULL, ,);
    next.bsmod_dividerWidget(width, indent, out);
}

static void _preval_bsmod_iconWidget(const bsgfx_AtlasCache* cache, float align_height, bs_vec3 offset, bs_U32 advance_flags, bsgfx_Widget* out) {
    BSMOD_VALIDATE(cache != NULL, ,);
    BSMOD_VALIDATE(out != NULL, ,);
    next.bsmod_iconWidget(cache, align_height, offset, advance_flags, out);
}

static bs_List* _preval_bsmod_packages() {
    return next.bsmod_packages();
}

static bsmod_Package* _preval_bsmod_queryPackage(const char* name) {
    BSMOD_VALIDATE(name != NULL, NULL,);
    return next.bsmod_queryPackage(name);
}

static bsmod_Package* _preval_bsmod_ensurePackage(const char* name) {
    BSMOD_VALIDATE(name != NULL, NULL,);
    return next.bsmod_ensurePackage(name);
}

static bs_Result _preval_bsmod_iniPackage(const char* package_name) {
    BSMOD_VALIDATE(package_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_iniPackage(package_name);
}

static bs_Result _preval_bsmod_packResource(bs_ResourceType type, unsigned char* data, size_t data_size, const char* package_name, char* resource_name, int resource_name_length) {
    BSMOD_VALIDATE(data != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(package_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(resource_name != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_packResource(type, data, data_size, package_name, resource_name, resource_name_length);
}

static bs_Result _preval_bsmod_savePackage(const char* name) {
    BSMOD_VALIDATE(name != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_savePackage(name);
}

static void _preval_bsmod_loadShaderReferences() {
    next.bsmod_loadShaderReferences();
}

static void _preval_bsmod_updateShaderReferences() {
    next.bsmod_updateShaderReferences();
}

static void _preval_bsmod_iniCompiler() {
    next.bsmod_iniCompiler();
}

static bs_Result _preval_bsmod_compileShader(char* path, char* name, char* package) {
    BSMOD_VALIDATE(path != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(name != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(package != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_compileShader(path, name, package);
}

static void _preval_bsmod_updateBindings() {
    next.bsmod_updateBindings();
}

static void _preval_bsmod_iniLisk() {
    next.bsmod_iniLisk();
}

static void _preval_bsmod_tickLisk() {
    next.bsmod_tickLisk();
}

static bs_Queue* _preval_bsmod_onQueue() {
    return next.bsmod_onQueue();
}

static void _preval_bsmod_queueRasterize(const char* package, const char* name, bs_Callback callback) {
    BSMOD_VALIDATE(package != NULL, ,);
    BSMOD_VALIDATE(name != NULL, ,);
    next.bsmod_queueRasterize(package, name, callback);
}

static void _preval_bsmod_pollRasterizer() {
    next.bsmod_pollRasterizer();
}

static void _preval_bsmod_beginRasterize(bs_ivec2 render_size, bs_ivec2 output_size) {
    next.bsmod_beginRasterize(render_size, output_size);
}

static void _preval_bsmod_endRasterize() {
    next.bsmod_endRasterize();
}

static bs_Result _preval_bsmod_rasterizeInstance(bs_PipelineHash pipeline_hash, int subtype, int instance_id, int category, char* name, int width, int height, size_t push_constant_size, unsigned char* push_constant) {
    BSMOD_VALIDATE(name != NULL, BS_RESULT_VALIDATION_ERROR,);
    BSMOD_VALIDATE(push_constant != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_rasterizeInstance(pipeline_hash, subtype, instance_id, category, name, width, height, push_constant_size, push_constant);
}

static void _preval_bsmod_instanceTransform() {
    next.bsmod_instanceTransform();
}

static void _preval_bsmod_selectHoveringTypes() {
    next.bsmod_selectHoveringTypes();
}

static bsgfx_TypeId _preval_bsmod_queryType(const char* plural) {
    BSMOD_VALIDATE(plural != NULL, 0,);
    return next.bsmod_queryType(plural);
}

static void _preval_bsmod_delete(bsgfx_TypeId type_id, int id) {
    next.bsmod_delete(type_id, id);
}

static void _preval_bsmod_deleteSelected(bsgfx_TypeId type_id) {
    next.bsmod_deleteSelected(type_id);
}

static bs_Result _preval_bsmod_saveType(bsgfx_TypeId id, char* value, int value_length) {
    BSMOD_VALIDATE(value != NULL, BS_RESULT_VALIDATION_ERROR,);
    return next.bsmod_saveType(id, value, value_length);
}

static void* _preval_bsmod_add(bsgfx_TypeId id, void* data) {
    BSMOD_VALIDATE(data != NULL, NULL,);
    return next.bsmod_add(id, data);
}

static bool _preval_bsmod_isSelected(bs_List* list, bsgfx_TypeId type, int id) {
    BSMOD_VALIDATE(list != NULL, false,);
    return next.bsmod_isSelected(list, type, id);
}

static void _preval_bsmod_select(bs_List* list, bsgfx_TypeId type, int id) {
    BSMOD_VALIDATE(list != NULL, ,);
    next.bsmod_select(list, type, id);
}

static void _preval_bsmod_deselectAll() {
    next.bsmod_deselectAll();
}

static void _preval_bsmod_deselectRange(bs_List* list, int first, int count) {
    BSMOD_VALIDATE(list != NULL, ,);
    next.bsmod_deselectRange(list, first, count);
}

static void _preval_bsmod_deselectIndex(bs_List* list, int id) {
    BSMOD_VALIDATE(list != NULL, ,);
    next.bsmod_deselectIndex(list, id);
}

static void _preval_bsmod_deselect(bs_List* list, int id) {
    BSMOD_VALIDATE(list != NULL, ,);
    next.bsmod_deselect(list, id);
}

static int _preval_bsmod_firstSelectedId(bs_List* list) {
    BSMOD_VALIDATE(list != NULL, 0,);
    return next.bsmod_firstSelectedId(list);
}

static int _preval_bsmod_lastSelectedId(bs_List* list) {
    BSMOD_VALIDATE(list != NULL, 0,);
    return next.bsmod_lastSelectedId(list);
}

static void* _preval_bsmod_removeFlexible(bsgfx_TypeId type_id, int id) {
    return next.bsmod_removeFlexible(type_id, id);
}

static void* _preval_bsmod_addFlexible(bsgfx_TypeId type_id, int id, void* data) {
    BSMOD_VALIDATE(data != NULL, NULL,);
    return next.bsmod_addFlexible(type_id, id, data);
}

static void _preval_bsmod_readHoveringOutputs() {
    next.bsmod_readHoveringOutputs();
}

static void _preval_bsmod_readHoveringInstanceData() {
    next.bsmod_readHoveringInstanceData();
}

static void _preval_bsmod_readHoveringVertex() {
    next.bsmod_readHoveringVertex();
}

static void _preval_bsmod_editSelectedType() {
    next.bsmod_editSelectedType();
}

static void _preval_bsmod_ensureTypeVersionsAreUpToDate(int package_id) {
    next.bsmod_ensureTypeVersionsAreUpToDate(package_id);
}

static void _preval_bsmod_renderBillboards() {
    next.bsmod_renderBillboards();
}

static void _preval_bsmod_instanceUI() {
    next.bsmod_instanceUI();
}

static void _preval_bsmod_pushVecNWidget(bs_List* widgets, const char* name, bs_vec3 offset, float width, float* v, int n) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    BSMOD_VALIDATE(name != NULL, ,);
    BSMOD_VALIDATE(v != NULL, ,);
    next.bsmod_pushVecNWidget(widgets, name, offset, width, v, n);
}

static void _preval_bsmod_pushInputWidget(bs_List* widgets, void* value, bs_vec3 offset, float width, int name_padding, bool skip_advance, bool is_last, bool has_buttons, const char* name) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    BSMOD_VALIDATE(value != NULL, ,);
    BSMOD_VALIDATE(name != NULL, ,);
    next.bsmod_pushInputWidget(widgets, value, offset, width, name_padding, skip_advance, is_last, has_buttons, name);
}

static void _preval_bsmod_instanceTileContextMenu(bs_vec3 position, bs_vec2 size) {
    next.bsmod_instanceTileContextMenu(position, size);
}

static void _preval_bsmod_instanceContextMenu(bs_List* widgets, bs_vec3 position, bs_vec2 size) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    next.bsmod_instanceContextMenu(widgets, position, size);
}

static void _preval_bsmod_pushContextMenuButton(bs_List* widgets, bs_vec2 menu_size, bs_vec3 icon_offset, bsgfx_AtlasCache* icon, const char* name, int indent, PFN_bsmod_PushContextMenuButton action, bool expandable) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    BSMOD_VALIDATE(icon != NULL, ,);
    BSMOD_VALIDATE(name != NULL, ,);
    next.bsmod_pushContextMenuButton(widgets, menu_size, icon_offset, icon, name, indent, action, expandable);
}

static void _preval_bsmod_instanceGridMenu(bs_vec3 position, bs_vec2 dimensions) {
    next.bsmod_instanceGridMenu(position, dimensions);
}

static void _preval_bsmod_instanceLightBillboards() {
    next.bsmod_instanceLightBillboards();
}

static bool _preval_bsmod_onAddLightTick(bsgfx_ButtonParams params) {
    return next.bsmod_onAddLightTick(params);
}

static void _preval_bsmod_pushMaterialWidgets(bs_List* widgets, bs_vec2 background_size) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    next.bsmod_pushMaterialWidgets(widgets, background_size);
}

static void _preval_bsmod_onDragMaterial(bsmod_DraggingParams params) {
    next.bsmod_onDragMaterial(params);
}

static void _preval_bsmod_onClickMaterialMenu(bsmod_GridClickParams params) {
    next.bsmod_onClickMaterialMenu(params);
}

static void _preval_bsmod_rasterizeMaterialIcons() {
    next.bsmod_rasterizeMaterialIcons();
}

static void _preval_bsmod_pushPrefabMenuWidgets(bs_List* widgets, bs_vec2 background_size) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    next.bsmod_pushPrefabMenuWidgets(widgets, background_size);
}

static bool _preval_bsmod_instancePrefabPreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    BSMOD_VALIDATE(widget != NULL, false,);
    BSMOD_VALIDATE(position != NULL, false,);
    return next.bsmod_instancePrefabPreview(widget, position, id, hovering);
}

static void _preval_bsmod_onDragPrefab(bsmod_DraggingParams params) {
    next.bsmod_onDragPrefab(params);
}

static void _preval_bsmod_rasterizePrefabIcons() {
    next.bsmod_rasterizePrefabIcons();
}

static bool _preval_bsmod_instancePrimitivePreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    BSMOD_VALIDATE(widget != NULL, false,);
    BSMOD_VALIDATE(position != NULL, false,);
    return next.bsmod_instancePrimitivePreview(widget, position, id, hovering);
}

static void _preval_bsmod_onDragPrimitive(bsmod_DraggingParams params) {
    next.bsmod_onDragPrimitive(params);
}

static void _preval_bsmod_rasterizePrimitiveIcons() {
    next.bsmod_rasterizePrimitiveIcons();
}

static void _preval_bsmod_setSideMenuTab(int id, bsmod_GridClickParams params) {
    next.bsmod_setSideMenuTab(id, params);
}

static void _preval_bsmod_instanceSideMenu(bs_vec3 position, bs_vec2 dimensions) {
    next.bsmod_instanceSideMenu(position, dimensions);
}

static void _preval_bsmod_pushTileMenuWidgets(bs_List* widgets, bs_vec2 background_size) {
    BSMOD_VALIDATE(widgets != NULL, ,);
    next.bsmod_pushTileMenuWidgets(widgets, background_size);
}

static bool _preval_bsmod_instanceTilePreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    BSMOD_VALIDATE(widget != NULL, false,);
    BSMOD_VALIDATE(position != NULL, false,);
    return next.bsmod_instanceTilePreview(widget, position, id, hovering);
}

static void _preval_bsmod_onDragTile(bsmod_DraggingParams params) {
    next.bsmod_onDragTile(params);
}

bsmod_FunctionTable _preval_bsmod_getFunctionTable() {
    bsmod_FunctionTable functions;

    functions.bsmod_copyHoveringDataToBuffer = _preval_bsmod_copyHoveringDataToBuffer;
    functions.bsmod_onIni = _preval_bsmod_onIni;
    functions.bsmod_onLateIni = _preval_bsmod_onLateIni;
    functions.bsmod_onCreateQuadSubtypes = _preval_bsmod_onCreateQuadSubtypes;
    functions.bsmod_onLoad = _preval_bsmod_onLoad;
    functions.bsmod_bindAtlases = _preval_bsmod_bindAtlases;
    functions.bsmod_onGfxRender = _preval_bsmod_onGfxRender;
    functions.bsmod_onPostRender = _preval_bsmod_onPostRender;
    functions.bsmod_onTick = _preval_bsmod_onTick;
    functions.bsmod_onMap = _preval_bsmod_onMap;
    functions.bsmod_onTrack = _preval_bsmod_onTrack;
    functions.bsmod_packBindings = _preval_bsmod_packBindings;
    functions.bsmod_onCompileShader = _preval_bsmod_onCompileShader;
    functions.bsmod_onLoadTTF = _preval_bsmod_onLoadTTF;
    functions.bsmod_onConvertBMFont = _preval_bsmod_onConvertBMFont;
    functions.bsmod_onPackAtlas = _preval_bsmod_onPackAtlas;
    functions.bsmod_onPackModels = _preval_bsmod_onPackModels;
    functions.bsmod_onPackBinary = _preval_bsmod_onPackBinary;
    functions.bsmod_onPackTextureArray = _preval_bsmod_onPackTextureArray;
    functions.bsmod_packAtlasTexture = _preval_bsmod_packAtlasTexture;
    functions.bsmod_packAtlas = _preval_bsmod_packAtlas;
    functions.bsmod_createAtlasPacker = _preval_bsmod_createAtlasPacker;
    functions.bsmod_packImageDirectory = _preval_bsmod_packImageDirectory;
    functions.bsmod_packBMFont = _preval_bsmod_packBMFont;
    functions.bsmod_scrollbar = _preval_bsmod_scrollbar;
    functions.bsmod_dividerWidget = _preval_bsmod_dividerWidget;
    functions.bsmod_iconWidget = _preval_bsmod_iconWidget;
    functions.bsmod_packages = _preval_bsmod_packages;
    functions.bsmod_queryPackage = _preval_bsmod_queryPackage;
    functions.bsmod_ensurePackage = _preval_bsmod_ensurePackage;
    functions.bsmod_iniPackage = _preval_bsmod_iniPackage;
    functions.bsmod_packResource = _preval_bsmod_packResource;
    functions.bsmod_savePackage = _preval_bsmod_savePackage;
    functions.bsmod_loadShaderReferences = _preval_bsmod_loadShaderReferences;
    functions.bsmod_updateShaderReferences = _preval_bsmod_updateShaderReferences;
    functions.bsmod_iniCompiler = _preval_bsmod_iniCompiler;
    functions.bsmod_compileShader = _preval_bsmod_compileShader;
    functions.bsmod_updateBindings = _preval_bsmod_updateBindings;
    functions.bsmod_iniLisk = _preval_bsmod_iniLisk;
    functions.bsmod_tickLisk = _preval_bsmod_tickLisk;
    functions.bsmod_onQueue = _preval_bsmod_onQueue;
    functions.bsmod_queueRasterize = _preval_bsmod_queueRasterize;
    functions.bsmod_pollRasterizer = _preval_bsmod_pollRasterizer;
    functions.bsmod_beginRasterize = _preval_bsmod_beginRasterize;
    functions.bsmod_endRasterize = _preval_bsmod_endRasterize;
    functions.bsmod_rasterizeInstance = _preval_bsmod_rasterizeInstance;
    functions.bsmod_instanceTransform = _preval_bsmod_instanceTransform;
    functions.bsmod_selectHoveringTypes = _preval_bsmod_selectHoveringTypes;
    functions.bsmod_queryType = _preval_bsmod_queryType;
    functions.bsmod_delete = _preval_bsmod_delete;
    functions.bsmod_deleteSelected = _preval_bsmod_deleteSelected;
    functions.bsmod_saveType = _preval_bsmod_saveType;
    functions.bsmod_add = _preval_bsmod_add;
    functions.bsmod_isSelected = _preval_bsmod_isSelected;
    functions.bsmod_select = _preval_bsmod_select;
    functions.bsmod_deselectAll = _preval_bsmod_deselectAll;
    functions.bsmod_deselectRange = _preval_bsmod_deselectRange;
    functions.bsmod_deselectIndex = _preval_bsmod_deselectIndex;
    functions.bsmod_deselect = _preval_bsmod_deselect;
    functions.bsmod_firstSelectedId = _preval_bsmod_firstSelectedId;
    functions.bsmod_lastSelectedId = _preval_bsmod_lastSelectedId;
    functions.bsmod_removeFlexible = _preval_bsmod_removeFlexible;
    functions.bsmod_addFlexible = _preval_bsmod_addFlexible;
    functions.bsmod_readHoveringOutputs = _preval_bsmod_readHoveringOutputs;
    functions.bsmod_readHoveringInstanceData = _preval_bsmod_readHoveringInstanceData;
    functions.bsmod_readHoveringVertex = _preval_bsmod_readHoveringVertex;
    functions.bsmod_editSelectedType = _preval_bsmod_editSelectedType;
    functions.bsmod_ensureTypeVersionsAreUpToDate = _preval_bsmod_ensureTypeVersionsAreUpToDate;
    functions.bsmod_renderBillboards = _preval_bsmod_renderBillboards;
    functions.bsmod_instanceUI = _preval_bsmod_instanceUI;
    functions.bsmod_pushVecNWidget = _preval_bsmod_pushVecNWidget;
    functions.bsmod_pushInputWidget = _preval_bsmod_pushInputWidget;
    functions.bsmod_instanceTileContextMenu = _preval_bsmod_instanceTileContextMenu;
    functions.bsmod_instanceContextMenu = _preval_bsmod_instanceContextMenu;
    functions.bsmod_pushContextMenuButton = _preval_bsmod_pushContextMenuButton;
    functions.bsmod_instanceGridMenu = _preval_bsmod_instanceGridMenu;
    functions.bsmod_instanceLightBillboards = _preval_bsmod_instanceLightBillboards;
    functions.bsmod_onAddLightTick = _preval_bsmod_onAddLightTick;
    functions.bsmod_pushMaterialWidgets = _preval_bsmod_pushMaterialWidgets;
    functions.bsmod_onDragMaterial = _preval_bsmod_onDragMaterial;
    functions.bsmod_onClickMaterialMenu = _preval_bsmod_onClickMaterialMenu;
    functions.bsmod_rasterizeMaterialIcons = _preval_bsmod_rasterizeMaterialIcons;
    functions.bsmod_pushPrefabMenuWidgets = _preval_bsmod_pushPrefabMenuWidgets;
    functions.bsmod_instancePrefabPreview = _preval_bsmod_instancePrefabPreview;
    functions.bsmod_onDragPrefab = _preval_bsmod_onDragPrefab;
    functions.bsmod_rasterizePrefabIcons = _preval_bsmod_rasterizePrefabIcons;
    functions.bsmod_instancePrimitivePreview = _preval_bsmod_instancePrimitivePreview;
    functions.bsmod_onDragPrimitive = _preval_bsmod_onDragPrimitive;
    functions.bsmod_rasterizePrimitiveIcons = _preval_bsmod_rasterizePrimitiveIcons;
    functions.bsmod_setSideMenuTab = _preval_bsmod_setSideMenuTab;
    functions.bsmod_instanceSideMenu = _preval_bsmod_instanceSideMenu;
    functions.bsmod_pushTileMenuWidgets = _preval_bsmod_pushTileMenuWidgets;
    functions.bsmod_instanceTilePreview = _preval_bsmod_instanceTilePreview;
    functions.bsmod_onDragTile = _preval_bsmod_onDragTile;

    return functions;
}

