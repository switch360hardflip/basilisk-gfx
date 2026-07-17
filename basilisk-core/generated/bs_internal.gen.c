
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
        
#include <bs_internal.gen.h>
#include <stdio.h>

void _bs_beginCommentV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_beginComment(_formatted, _length);
}

void _bs_beginCommentF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_beginCommentV(format, args);
    va_end(args);
}

void _bs_nameBufferV(
    bs_Buffer* buffer, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_nameBuffer(buffer, _formatted, _length);
}

void _bs_nameBufferF(
    bs_Buffer* buffer, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_nameBufferV(buffer, format, args);
    va_end(args);
}

bs_Attribute* _bs_queryAttributeV(
    bs_Batch* batch, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_queryAttribute(batch, _formatted, _length);
}

bs_Attribute* _bs_queryAttributeF(
    bs_Batch* batch, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Attribute* _return = bs_queryAttributeV(batch, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_savePngV(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_savePng(data, resolution, type, _formatted, _length);
}

bs_Result _bs_savePngF(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_savePngV(data, resolution, type, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_encodePngV(
    size_t* out_size, 
    const unsigned char* data, 
    bs_ivec2 size, 
    bs_PngType type, 
    unsigned char** out, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_encodePng(out_size, data, size, type, out, _formatted, _length);
}

bs_Result _bs_encodePngF(
    size_t* out_size, 
    const unsigned char* data, 
    bs_ivec2 size, 
    bs_PngType type, 
    unsigned char** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_encodePngV(out_size, data, size, type, out, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_loadImageV(
    bs_Object* object, 
    int package_id, 
    bs_ImageBits flags, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_loadImage(object, package_id, flags, _formatted, _length);
}

bs_Result _bs_loadImageF(
    bs_Object* object, 
    int package_id, 
    bs_ImageBits flags, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadImageV(object, package_id, flags, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_loadAtlasV(
    bs_Object* object, 
    int package_id, 
    bs_U32 flags, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_loadAtlas(object, package_id, flags, _formatted, _length);
}

bs_Result _bs_loadAtlasF(
    bs_Object* object, 
    int package_id, 
    bs_U32 flags, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadAtlasV(object, package_id, flags, format, args);
    va_end(args);
    return _return;
}

void _bsi_nameHandleV(
    bs_U64 handle, 
    bs_U32 type, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bsi_nameHandle(handle, type, _formatted, _length);
}

void _bsi_nameHandleF(
    bs_U64 handle, 
    bs_U32 type, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bsi_nameHandleV(handle, type, format, args);
    va_end(args);
}

bs_Result _bs_loadJsonV(
    bs_Json* out_json, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_loadJson(out_json, _formatted, _length);
}

bs_Result _bs_loadJsonF(
    bs_Json* out_json, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadJsonV(out_json, format, args);
    va_end(args);
    return _return;
}

bs_JsonValue _bs_fetchJsonV(
    bs_Json* root, 
    bs_JsonType expect, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_fetchJson(root, expect, _formatted, _length);
}

bs_JsonValue _bs_fetchJsonF(
    bs_Json* root, 
    bs_JsonType expect, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_JsonValue _return = bs_fetchJsonV(root, expect, format, args);
    va_end(args);
    return _return;
}

void _bs_deleteJsonV(
    bs_Json* root, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_deleteJson(root, _formatted, _length);
}

void _bs_deleteJsonF(
    bs_Json* root, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_deleteJsonV(root, format, args);
    va_end(args);
}

bs_Result _bs_ensureJsonV(
    bs_Json* root, 
    bs_JsonValue value, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_ensureJson(root, value, _formatted, _length);
}

bs_Result _bs_ensureJsonF(
    bs_Json* root, 
    bs_JsonValue value, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_ensureJsonV(root, value, format, args);
    va_end(args);
    return _return;
}

char* _bs_logSectionV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_logSection(_formatted, _length);
}

char* _bs_logSectionF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_logSectionV(format, args);
    va_end(args);
    return _return;
}

char* _bs_logWithTimestampV(
    const char* type, 
    int type_len, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_logWithTimestamp(type, type_len, _formatted, _length);
}

char* _bs_logWithTimestampF(
    const char* type, 
    int type_len, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_logWithTimestampV(type, type_len, format, args);
    va_end(args);
    return _return;
}

char* _bs_logV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_log(_formatted, _length);
}

char* _bs_logF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_logV(format, args);
    va_end(args);
    return _return;
}

char* _bs_infoV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_info(_formatted, _length);
}

char* _bs_infoF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_infoV(format, args);
    va_end(args);
    return _return;
}

char* _bs_warnV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_warn(_formatted, _length);
}

char* _bs_warnF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_warnV(format, args);
    va_end(args);
    return _return;
}

void _bs_criticalV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_critical(_formatted, _length);
}

void _bs_criticalF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_criticalV(format, args);
    va_end(args);
}

void _bs_systemV(
    char* command, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_system(command, _formatted, _length);
}

void _bs_systemF(
    char* command, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_systemV(command, format, args);
    va_end(args);
}

bs_String* _bs_stringV(
    bs_String* old, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_string(old, _formatted, _length);
}

bs_String* _bs_stringF(
    bs_String* old, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_String* _return = bs_stringV(old, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_setWorkingDirectoryV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_setWorkingDirectory(_formatted, _length);
}

bs_Result _bs_setWorkingDirectoryF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_setWorkingDirectoryV(format, args);
    va_end(args);
    return _return;
}

char* _bs_charStringV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_charString(_formatted, _length);
}

char* _bs_charStringF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_charStringV(format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_saveFileV(
    char* data, 
    bs_U32 data_len, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_saveFile(data, data_len, _formatted, _length);
}

bs_Result _bs_saveFileF(
    char* data, 
    bs_U32 data_len, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_saveFileV(data, data_len, format, args);
    va_end(args);
    return _return;
}

void _bs_convertWin32PathV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_convertWin32Path(_formatted, _length);
}

void _bs_convertWin32PathF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_convertWin32PathV(format, args);
    va_end(args);
}

bs_Result _bs_ensureDirectoryV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_ensureDirectory(_formatted, _length);
}

bs_Result _bs_ensureDirectoryF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_ensureDirectoryV(format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_fileModifiedDateV(
    bs_DateTime* out, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_fileModifiedDate(out, _formatted, _length);
}

bs_Result _bs_fileModifiedDateF(
    bs_DateTime* out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_fileModifiedDateV(out, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_setFileModifiedDateV(
    bs_DateTime* date, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_setFileModifiedDate(date, _formatted, _length);
}

bs_Result _bs_setFileModifiedDateF(
    bs_DateTime* date, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_setFileModifiedDateV(date, format, args);
    va_end(args);
    return _return;
}

bool _bs_fileExistsV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_fileExists(_formatted, _length);
}

bool _bs_fileExistsF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bool _return = bs_fileExistsV(format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_loadResourceV(
    int package_id, 
    bs_U32 flags, 
    bs_Resource** out, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_loadResource(package_id, flags, out, _formatted, _length);
}

bs_Result _bs_loadResourceF(
    int package_id, 
    bs_U32 flags, 
    bs_Resource** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadResourceV(package_id, flags, out, format, args);
    va_end(args);
    return _return;
}

void _bs_titleWindowV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_titleWindow(_formatted, _length);
}

void _bs_titleWindowF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_titleWindowV(format, args);
    va_end(args);
}

void _bs_copyToClipboardV(
    bs_Timer* timer, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    bs_copyToClipboard(timer, _formatted, _length);
}

void _bs_copyToClipboardF(
    bs_Timer* timer, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_copyToClipboardV(timer, format, args);
    va_end(args);
}

bs_String* _bs_appendStringV(
    bs_String* destination, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_appendString(destination, _formatted, _length);
}

bs_String* _bs_appendStringF(
    bs_String* destination, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_String* _return = bs_appendStringV(destination, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_foreachFileV(
    bs_ForeachDocumentFunction x, 
    void* param, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_foreachFile(x, param, _formatted, _length);
}

bs_Result _bs_foreachFileF(
    bs_ForeachDocumentFunction x, 
    void* param, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_foreachFileV(x, param, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_foreachDirectoryV(
    bs_ForeachDocumentFunction x, 
    void* param, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_foreachDirectory(x, param, _formatted, _length);
}

bs_Result _bs_foreachDirectoryF(
    bs_ForeachDocumentFunction x, 
    void* param, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_foreachDirectoryV(x, param, format, args);
    va_end(args);
    return _return;
}

int _bs_numFilesV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_numFiles(_formatted, _length);
}

int _bs_numFilesF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    int _return = bs_numFilesV(format, args);
    va_end(args);
    return _return;
}

int _bs_numDirectoriesV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_numDirectories(_formatted, _length);
}

int _bs_numDirectoriesF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    int _return = bs_numDirectoriesV(format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_loadFileV(
    bs_String** out, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_loadFile(out, _formatted, _length);
}

bs_Result _bs_loadFileF(
    bs_String** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadFileV(out, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_loadFileChunkV(
    const char* path, 
    long offset, 
    size_t size, 
    bs_String** out, 
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_loadFileChunk(path, offset, size, out, _formatted, _length);
}

bs_Result _bs_loadFileChunkF(
    const char* path, 
    long offset, 
    size_t size, 
    bs_String** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadFileChunkV(path, offset, size, out, format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_deleteFileV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_deleteFile(_formatted, _length);
}

bs_Result _bs_deleteFileF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_deleteFileV(format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_deleteDirectoryContentsV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_deleteDirectoryContents(_formatted, _length);
}

bs_Result _bs_deleteDirectoryContentsF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_deleteDirectoryContentsV(format, args);
    va_end(args);
    return _return;
}

bs_Result _bs_deleteDirectoryV(
    char* format, 
    va_list args)
{
    int _length = bs_formatStringLength(format, args);
    char* _formatted = bs_alloca(_length + 1);
    vsnprintf(_formatted, _length + 1, format, args);
    return bs_deleteDirectory(_formatted, _length);
}

bs_Result _bs_deleteDirectoryF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_deleteDirectoryV(format, args);
    va_end(args);
    return _return;
}

