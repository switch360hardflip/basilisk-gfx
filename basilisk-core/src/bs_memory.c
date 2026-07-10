#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#ifdef WIN32
#include <io.h>
#include <shlobj_core.h>
#else
#include <unistd.h>
#endif

#include <basilisk-core.h>s
#include <bs_internal.h>
#include <vulkan.h>

bs_Instance* _bs_instance;
bs_IO _bs_io;
bs_Swapchain* _bs_swapchain_;
int _bs_image_index_ = 0;



  /*==============================================================================
   * System
   *============================================================================*/

static DWORD WINAPI bs_systemCallback(char* input) {
    char output[512];

    FILE* fp = _popen(input, "r");
    if (!fp)
        bs_throwF("Failed to run script");

    while (fgets(output, sizeof(output), fp) != NULL)
        bs_infoF("%s", output);

    _pclose(fp);
    free(input);
    return 0;
}

BSAPI void _bs_system(char* s) {
    bs_systemCallback(s);
}

BSAPI void _bs_systemAsync(char* s) {
    CreateThread(NULL, 0, bs_systemCallback, s, 0, NULL);
}



  /*==============================================================================
   * Cross-Platform/Wrapper C Standard Functions
   *============================================================================*/

BSAPI bs_U32 _bs_alignUp(bs_U32 value, bs_U32 alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

BSAPI char* _bs_strsep(char** stringp, const char* delim) {
#ifdef _WIN32
    if (*stringp == NULL) { return NULL; }
    char* token_start = *stringp;
    *stringp = strpbrk(token_start, delim);
    if (*stringp) {
        **stringp = '\0';
        (*stringp)++;
    }
    return token_start;
#else
    return strsep(stringp, delim);
#endif
}

BSAPI size_t _bs_strnlen(const char* src, size_t n) {
    size_t len = 0;
    while (len < n && src[len])
        len++;
    return len;
}

BSAPI char* _bs_strndup(const char* s, size_t n) {
    size_t len = bs_strnlen(s, n);
    char* p = malloc(len + 1);
    if (p) {
        memcpy(p, s, len);
        p[len] = '\0';
    }
    return p;
}

BSAPI void* _bs_memmem(
    const void* haystack, bs_U32 haystack_len,
    const void* const needle, const bs_U32 needle_len) 
{
    if (haystack == NULL) return NULL;
    if (haystack_len == 0) return NULL;
    if (needle == NULL) return NULL;
    if (needle_len == 0) return NULL;

    for (const char *h = haystack; haystack_len >= needle_len;
       ++h, --haystack_len) 
    {
        if (memcmp(h, needle, needle_len) == 0) 
            return (void *)h;
    }

    return NULL;
}

BSAPI void* _bs_free(void* p) {
    free(p);
    return NULL;
}

/**
 if BSX_FAILED_TO_ALLOCATE occurs and we don't have cmd_log=true it'll try to allocate again which might fail
 bs_infoF uses the C lib's realloc so it won't be an infinite loop or anything
 dunno if i care thoughh :|

 ... wait so what was the problem again? am i retarded?
 */

 /**
  malloc
  */
BSAPI void* _val_bs_malloc(bs_U64 size) {
    BS_VALIDATE(size != 0, NULL,);
    return bs_malloc(size);
}

BSAPI void* _bs_malloc(bs_U64 size) {
    void* p = malloc(size);

    if (!p) {
        bs_criticalF("malloc(%lld) returned NULL\n", size);
        return NULL;
    }

    return p;
}

 /**
  calloc
  */
BSAPI void* _val_bs_calloc(bs_U64 num_units, bs_U64 unit_size) {
    BS_VALIDATE(num_units > 0, NULL,);
    BS_VALIDATE(unit_size > 0, NULL,);

    return bs_calloc(num_units, unit_size);
}

BSAPI void* _bs_calloc(bs_U64 num_units, bs_U64 unit_size) {
    void* p = calloc(num_units, unit_size);

    if (!p) {
        bs_criticalF("calloc(%lld, %lld) returned NULL\n", num_units, unit_size);
        return NULL;
    }

    return p;
}

/**
 realloc
 */
BSAPI void* _val_bs_realloc(void* p, bs_U64 size) {
    BS_VALIDATE(size != 0, NULL, );
    return bs_realloc(p, size);
}

BSAPI void* _bs_realloc(void* p, bs_U64 size) {
    p = realloc(p, size);

    if (!p) {
        bs_warnF("realloc(%lld) returned NULL\n", size);
        return NULL;
    }

    return p;
}

BSAPI void* _bs_fetchUnit(bs_List* list, bs_U32 offset) {
    return ((bs_U8*)list->data) + offset * list->unit_size;
}



  /*==============================================================================
   * Strings
   *============================================================================*/

#define BS_STRING_OVERHEAD (128)

BSAPI const char* _bs_checkStringPool(bs_List* pool, char* string) {
    bs_U64 hash = bs_stringHash(string);

    for (int i = 0; i < pool->count; i++) {
        bs_StringPoolEntry* entry = bs_fetchUnit(pool, i);
        if (hash == entry->hash)
            return entry->string;
    }

    bs_StringPoolEntry* entry = bs_pushBack(pool, NULL);
    entry->hash = hash;
    entry->string = strdup(string);

    return entry->string;
}

BSAPI bs_String* _bs_stringAlloc(bs_String* old, int len) {
    bs_String* data;

    if (old) {
        if (old->capacity < len)
            data = bs_realloc(old, sizeof(bs_String) + len + 1);
        else {
            len = old->capacity;
            data = old;
        }
    }
    else 
        data = bs_malloc(sizeof(bs_String) + len + 1);

    memset(data, 0, sizeof(bs_String));
    data->capacity = len;
    data->value[0] = '\0';
    return data;
}

BSAPI bs_String* _bs_string(bs_String* old, char* str, int len) {
    if (len < 0)
        len = strlen(str);
    bs_String* data = bs_stringAlloc(old, len);
    if (str)
        memcpy(data->value, str, len);
    data->value[len] = '\0';
    data->len = len;
    return data;
}

BSAPI bs_String* _bs_emptyString(bs_String* old) {
    return bs_string(old, "", 0);
}

BSAPI void _bs_shortenString(bs_String* string, int len) {
    string->len -= len;
    string->value[string->len] = '\0';
}

BSAPI bs_String* _bs_appendString(bs_String* string, char* append, int len) {
    if (!string)
        string = bs_stringAlloc(NULL, len + BS_STRING_OVERHEAD);
    else if ((string->capacity - string->len) < len + 1) {
        string->capacity = string->len + len + BS_STRING_OVERHEAD;
        string = bs_realloc(string, sizeof(bs_String) + string->capacity + 1);
    }
    strncat(string->value, append, len);
    string->len += len;

    return string;
}

BSAPI bs_String* _bs_appendChar(bs_String* string, char c) {
    if (!string)
        string = bs_stringAlloc(NULL, BS_STRING_OVERHEAD);
    else if ((string->capacity - string->len) < 1) {
        string->capacity += BS_STRING_OVERHEAD;
        string = bs_realloc(string, sizeof(bs_String) + string->capacity + 1);
    }

    string->value[string->len++] = c;
    string->value[string->len] = '\0';
    return string;
}

 /**
  Remove last N chars
  */
BSAPI void _val_bs_removeLastNChars(bs_String* string, int n) {
    BS_VALIDATE(string->len >= n,,);
}

BSAPI void _bs_removeLastNChars(bs_String* string, int n) {
    string->len -= n;
    string->value[string->len] = '\0';
}

 /**
  Remove char range
  */
BSAPI void _val_bs_removeCharRange(bs_String* string, int start, int count) {
    BS_VALIDATE(start >= 0,,);
    BS_VALIDATE(count >= 0,,);
    BS_VALIDATE(start < string->len,,);

    return bs_removeCharRange(string, start, count);
}

BSAPI void _bs_removeCharRange(bs_String* string, int start, int count) {
    if (start + count > string->len)
        count = string->len - start;

    memmove(
        string->value + start,
        string->value + start + count,
        string->len - (start + count) + 1);

    string->len -= count;
}

 /**
  Insert char
  */
BSAPI bs_String* _val_bs_insertChar(bs_String* string, int index, char c) {
    BS_VALIDATE(index >= 0, NULL,,);
    BS_VALIDATE(index <= string->len, NULL,,);

    return bs_insertChar(string, index, c);
}

BSAPI bs_String* _bs_insertChar(bs_String* string, int index, char c) {
    if (!string)
        string = bs_stringAlloc(NULL, BS_STRING_OVERHEAD);

    if ((string->capacity - string->len) < 1) {
        string->capacity += BS_STRING_OVERHEAD;
        string = bs_realloc(string, sizeof(bs_String) + string->capacity + 1);
    }

    memmove(
        string->value + index + 1,
        string->value + index,
        string->len - index + 1
    );

    string->value[index] = c;
    string->len++;

    return string;
}

BSAPI bs_String* _bs_appendPath(bs_String* string, char* path, int path_len) {
    if (path_len == 0 || string->value[string->len - 1] != '/')
        string = bs_appendChar(string, '/');
    return bs_appendString(string, path, path_len);
}

BSAPI void _bs_replaceCharOccurrences(char* string, int string_len, char a, char b) {
    for (int i = 0; i < string_len; i++) {
        if (string[i] == a) 
            string[i] = b;
    }
}

bs_String* bs_stringV(bs_String* old, const char* format, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    int len = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (len < 0)
        bs_throwErrno(format);

    bs_String* string = bs_stringAlloc(old, len);
    string->len = len;

    vsnprintf(string->value, len + 1, format, args);

    return string;
}

bs_String* bs_stringF(bs_String* old, const char* format, ...) {
    va_list args;
    va_start(args, format);

    bs_String* string = bs_stringV(old, format, args);

    va_end(args);
    return string;
}

 /**
  To lowercase
  */
BSAPI void _val_bs_toLower(char* string, int len) {
    BS_VALIDATE(len <= 0,,);
    return bs_toLower(string, len);
}

BSAPI void _bs_toLower(char* string, int len) {
    for (int i = 0; i < len; i++)
        string[i] = (char)tolower((unsigned char)string[i]);
}

 /**
  To uppercase
  */
BSAPI void _val_bs_toUpper(char* string, int len) {
    BS_VALIDATE(len <= 0,,);
    return bs_toUpper(string, len);
}
BSAPI void _bs_toUpper(char* string, int len) {
    for (int i = 0; i < len; i++)
        string[i] = (char)toupper((unsigned char)string[i]);
}

BSAPI bs_U64 _bs_hash(unsigned char* data, size_t size) {
    bs_U64 hash = 0xcbf29ce484222325;

    for (int i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= 0x100000001b3;
    }

    return hash;
}

BSAPI bs_U64 _bs_stringHash(char* string) {
    bs_U64 hash = 0xcbf29ce484222325;

    for (int i = 0; i < strlen(string); i++) {
        hash ^= ((unsigned char*)string)[i];
        hash *= 0x100000001b3;
    }

    return hash;
}

BSAPI bool _bs_startsWith(char* string, const char* prefix) {
    while (*prefix && *string == *prefix) ++string, ++prefix;
    return *prefix == 0;
}

BSAPI bool _bs_endsWith(const char* string, const char* suffix) {
    size_t string_len = strlen(string);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > string_len)
        return false;

    return strncmp(string + string_len - suffix_len, suffix, suffix_len) == 0;
}

BSAPI char _val_bs_lastChar(char* string, int len) {
    BS_VALIDATE(len > 0, '\0',,);
    return bs_lastChar(string, len);
}

BSAPI char _bs_lastChar(char* string, int len) {
    return string[len - 1];
}

BSAPI bool _bs_stringContainsChar(char* string, char c) {
    return strchr(string, c);
}

 /**
  Wide strings
  */
#ifdef _WIN32
BSAPI bs_Result _bs_widen(char* src, wchar_t* dst, bs_U32 dst_size) {
    int result = MultiByteToWideChar(CP_UTF8, 0, src, -1, dst, dst_size);
    if (result == 0) {
        bs_warnF("Failed to widen string \"%s\"\n", src);
        return bs_convertLastError();
    }

    return BS_RESULT_OK;
}

BSAPI bs_Result _bs_unwiden(wchar_t* src, char* dst, bs_U32 dst_size) {
    int result = WideCharToMultiByte(CP_UTF8, 0, src, -1, dst, dst_size, NULL, NULL);
    if (result == 0) {
        bs_warnF("Failed to unwiden string \"%s\"\n", src);
        return bs_convertLastError();
    }

    return BS_RESULT_OK;
}
#endif



  /*==============================================================================
   * List
   *============================================================================*/

BSAPI bool _bs_listContains(bs_List* list, void* data) {
    for (int i = 0; i < list->count; i++) {
        void* compare = bs_fetchUnit(list, i);

        if (memcmp(compare, data, list->unit_size) == 0)
            return true;
    }

    return false;
}

BSAPI void* _val_bs_fetchUnit(bs_List* list, bs_U32 offset) {
    BS_VALIDATE(list->data != NULL, NULL,);
    BS_VALIDATE(offset < list->count, NULL, );

    return bs_fetchUnit(list, offset);
}

BSAPI void* _bs_fetchUnit(bs_List* list, bs_U32 offset) {
    return ((bs_U8*)list->data) + offset * list->unit_size;
}

BSAPI void* _val_bs_seekList(bs_List* list, bs_U32 offset) {
    BS_VALIDATE(list->data != NULL, NULL, );
    BS_VALIDATE(offset < list->capacity, NULL, );

    return bs_fetchUnit(list, offset);
}

BSAPI void* _bs_seekList(bs_List* list, bs_U32 offset) {
    return _bs_fetchUnit(list, offset);
}

BSAPI void* _val_bs_fetchLast(bs_List* list) {
    BS_VALIDATE(list->count > 0, NULL, );
    return bs_fetchLast(list);
}

BSAPI void* _bs_fetchLast(bs_List* list) {
    return bs_fetchUnit(list, list->count - 1);
}

BSAPI void _bs_ensureSize(bs_List* list, bs_U32 num_units) {
    if ((list->count + num_units) < list->capacity)
        return;

    bs_U32 prev_capacity = list->capacity;
    list->capacity += bs_imax(num_units, list->increment);
    list->data = bs_realloc(list->data, list->capacity * list->unit_size);

    bs_U32 size = (list->capacity - prev_capacity) * list->unit_size;

    bs_except(BSX_OUT_OF_BOUNDS);
    void* unit = bs_fetchUnitUnassigned(list, prev_capacity);
    if (unit)
        memset(unit, 0, size);
    bs_caught();
}

BSAPI void* _bs_pushBackUnsafe(bs_List* list, char* data) {
    bs_U8* dest = bs_fetchUnitUnassigned(list, list->count);
    memcpy(dest, data, list->unit_size);
    list->count++;
    return dest;
}

BSAPI void* _bs_pushBack(bs_List* list, char* data) {
    bs_ensureSize(list, 1);

    char* dest = bs_fetchUnitUnassigned(list, list->count);
    if (data)
        memcpy(dest, data, list->unit_size);
    else memset(dest, 0, list->unit_size);

    list->count++;
    return dest;
}

BSAPI void* _bs_pushBackList(bs_List* destination, bs_List* source) {
    assert(destination);
    assert(source);
    assert(destination->unit_size == source->unit_size);

    bs_ensureSize(destination, source->count);

    bs_U8* dest = bs_fetchUnitUnassigned(destination, destination->count);
    for (int i = 0; i < source->count; i++) {
        void* data = bs_fetchUnit(source, i);
        memcpy(dest + i * destination->unit_size, data, destination->unit_size);
    }

    destination->count += source->count;
    return dest;
}

BSAPI bs_Result _bs_erase(bs_List* list, int index, bs_U32 count) {
    if ((index + count - 1) >= list->count) {
        bs_warnF("_bs_erase: index %d + count %d out of bounds (list->count = %d)\n", index, count, list->count);
        return BS_RESULT_OUT_OF_BOUNDS;
    }

    for (int i = index; i < (list->count - 1); i++) {
        unsigned char* this = list->data + i * list->unit_size;
        unsigned char* next = this + count * list->unit_size;

        memcpy(this, next, list->unit_size);
    }

    list->count -= count;
    return BS_RESULT_OK;
}

BSAPI void _bs_destroyList(bs_List* list) {
    bs_free(list->data);
    list->capacity = 0;
    list->data = NULL;
}

BSAPI void _bs_minimizeList(bs_List* list) {
    bs_U64 smallest = (list->count * list->unit_size);
    if (smallest < list->capacity) {
        list->data = bs_realloc(list->data, smallest);
        list->capacity = smallest;
    }
}

BSAPI bs_List _bs_list(int unit_size, int increment) {
    return (bs_List) {
        .unit_size = unit_size,
        .increment = increment,
    };
}



  /*==============================================================================
   * File I/O
   *============================================================================*/

    /**
     Iterate documents
     */
#ifdef _WIN32
static inline void bs_iterateDocuments(int is_file, int(*x)(bs_FileInfo, void*), void* param, const char* directory) {
    char path[MAX_PATH];
    path[0] = '\0';
    int directory_len = strlen(directory);
    strncat(path, directory, directory_len);
    if (directory[directory_len - 1] == '/') {
        strncat(path, "*", 1);
    }
    else {
        directory_len++;
        strncat(path, "/*", 2);
    }

    WIN32_FIND_DATA file_data;
    HANDLE handle = FindFirstFile(path, &file_data);
    if (handle == INVALID_HANDLE_VALUE) {
        if (bs_throwLastWin32Error(path))
            return;
    }

    do {
        if (is_file ? 
            !(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) : 
            (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
        {
            if (file_data.cFileName[0] == '.') continue;
            strcpy(path + directory_len, file_data.cFileName);

            x((bs_FileInfo) {
                .path = path,
                .size = ((size_t)file_data.nFileSizeHigh << 32) | file_data.nFileSizeLow,
            }, param);
        }
    } while (FindNextFile(handle, &file_data));
    FindClose(handle);
}

BSAPI void _bs_foreachFile(void(*x)(bs_FileInfo, void*), void* param, const char* directory) {
    bs_iterateDocuments(true, x, param, directory);
}

BSAPI void _bs_foreachDirectory(void(*x)(bs_FileInfo, void*), void* param, const char* directory) {
    bs_iterateDocuments(false, x, param, directory);
}

BSAPI void _bs_foreachFileF(void(*x)(bs_FileInfo, void*), void* param, char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);
    bs_iterateDocuments(true, x, param, path);
}

BSAPI void _bs_foreachDirectoryF(void(*x)(bs_FileInfo, void*), void* param, char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);
    bs_iterateDocuments(false, x, param, path);
}

   /**
    Document counting
    */

static inline int bs_increment(bs_FileInfo info, int* i) { (*i)++; }
BSAPI int _bs_numFiles(const char* directory) {
    // todo maybe better way to do this
    int i = 0;
    bs_foreachFile(bs_increment, &i, directory);
    return i;
}

BSAPI int _bs_numFilesF(const char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);
    return bs_numFiles(path);
}

BSAPI int _bs_numDirectories(const char* directory) {
    int i = 0;
    bs_foreachDirectory(bs_increment, &i, directory);
    return i;
}

BSAPI int _bs_numDirectoriesF(const char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);
    return bs_numDirectories(path);
}

#else
BSAPI bs_Result _bs_foreachFile(int(*x)(bs_FileInfo, void*), const char* directory) { bs_warnF("_bs_foreachFile not implemented on this platform\n"); return BS_RESULT_NOT_IMPLEMENTED; }
BSAPI bs_Result _bs_foreachDirectory(int(*x)(bs_FileInfo, void*), const char* directory) { bs_warnF("_bs_foreachDirectory not implemented on this platform\n"); return BS_RESULT_NOT_IMPLEMENTED; }
BSAPI bs_Result _bs_foreachFileF(int(*x)(bs_FileInfo, void*), const char* format, ...) { bs_warnF("_bs_foreachFileF not implemented on this platform\n"); return BS_RESULT_NOT_IMPLEMENTED; }
BSAPI bs_Result _bs_foreachDirectoryF(int(*x)(bs_FileInfo, void*), const char* format, ...) { bs_warnF("_bs_foreachDirectoryF not implemented on this platform\n"); return BS_RESULT_NOT_IMPLEMENTED; }
#endif

   /**
    Document information
    */

BSAPI char* _bs_fileName(const char* path) {
    char* slash = strrchr(path, '/');
    if (!slash || slash == path) {
        //bs_throwBasiliskF(BSXI_INTERNAL | BSX_GENERAL, "Path %s does not have a file name", path);
        return path;
    }
    return slash + 1;
}

BSAPI char* _bs_fileExtension(const char* path) {
    char* dot = strrchr(path, '.');
    if (!dot || dot == path || dot[1] == '/' || dot[1] == '\\') {
        bs_warnF("Path %s does not have a file extension\n", path);
        return NULL;
    }
    return dot + 1;
}

BSAPI bool _bs_fileExtensionIs(const char* path, char* extension) {
    char* actual = bs_fileExtension(path);
    return strcmp(actual, extension) == 0;
}

#ifdef _WIN32
static inline bs_DateTime bs_fromSystemTime(SYSTEMTIME system_time) {
    return (bs_DateTime) {
        .years = system_time.wYear,
        .months = system_time.wMonth,
        .days = system_time.wDay,
        .hours = system_time.wHour,
        .minutes = system_time.wMinute,
        .seconds = system_time.wSecond,
        .milliseconds = system_time.wMilliseconds,
    };
}

static inline SYSTEMTIME bs_toSystemTime(bs_DateTime* date_time) {
    return (SYSTEMTIME) {
        .wYear = date_time->years,
        .wMonth = date_time->months,
        .wDay = date_time->days,
        .wHour = date_time->hours,
        .wMinute = date_time->minutes,
        .wSecond = date_time->seconds,
        .wMilliseconds = date_time->milliseconds,
    };
}

BSAPI bs_DateTime _bs_fileModifiedDate(const char* path) {
    HANDLE file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    FILETIME modified;
    DWORD error = GetLastError();

    if (file == INVALID_HANDLE_VALUE) {
        if (error == ERROR_SHARING_VIOLATION)
            return (bs_DateTime) { 0 };
        bs_throwWin32Error(error, path);
        return (bs_DateTime) { 0 };
    }
    if (!GetFileTime(file, NULL, NULL, &modified)) {
        bs_throwWin32Error(error, path);
        return (bs_DateTime) { 0 };
    }
    CloseHandle(file);

    SYSTEMTIME system_time = { 0 };
    FileTimeToSystemTime(&modified, &system_time);
    return bs_fromSystemTime(system_time);
}

BSAPI bs_Result _bs_setFileModifiedDate(const char* path, bs_DateTime* date_time) {
    HANDLE file = CreateFile(
        path,
        FILE_WRITE_ATTRIBUTES,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        bs_warnF("_bs_setFileModifiedDate(%s) -> CreateFile failed with error %lu\n", path, GetLastError());
        return bs_convertLastError();
    }

    SYSTEMTIME system_time = bs_toSystemTime(date_time);

    FILETIME file_time;
    if (!SystemTimeToFileTime(&system_time, &file_time)) {
        bs_warnF("_bs_setFileModifiedDate(%s) -> SystemTimeToFileTime failed with error %lu\n", path, GetLastError());
        CloseHandle(file);
        return bs_convertLastError();
    }

    if (!SetFileTime(file, NULL, NULL, &file_time)) {
        bs_warnF("_bs_setFileModifiedDate(%s) -> SetFileTime failed with error %lu\n", path, GetLastError());
        CloseHandle(file);
        return bs_convertLastError();
    }

    CloseHandle(file);
    return BS_RESULT_OK;
}

BSAPI bs_DateTime _bs_fileModifiedDateF(const char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);
    return _bs_fileModifiedDate(path);
}

#else
BSAPI bs_DateTime _bs_fileModifiedDate(const char* path) {
    bs_warnF("_bs_fileModifiedDate not implemented on this platform\n");
    return (bs_DateTime) { 0 };
}
BSAPI bs_DateTime _bs_fileModifiedDateF(const char* format, ...) {
    bs_warnF("_bs_fileModifiedDateF not implemented on this platform\n");
    return (bs_DateTime) { 0 };
}
#endif


BSAPI bool _bs_fileExists(const char* path) {
#ifdef _WIN32
    return _access(path, 0) == 0;
#else
    return access(path, 0) == 0;
#endif
}

BSAPI bool _bs_fileExistsF(char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);

    return _bs_fileExists(path);
}

BSAPI bs_String* _bs_executablePath() { return _bs_io_.executable; }
BSAPI void _bs_findRelativePath();
BSAPI bs_String* _bs_workingDirectory() {
    if (!_bs_io_.cwd) _bs_findRelativePath();
    return _bs_io_.cwd;
}

BSAPI void _bs_setWorkingDirectory(char* path) {
    SetCurrentDirectory(path);
    _bs_findRelativePath();
}

   /**
    Loading documents
    */

 /** TODO: More checks needed in here */
static bs_String* bs_loadFileFromHandle(FILE* file) {
    fseek(file, 0, SEEK_END);
    long len = ftell(file) + 1;
    fseek(file, 0, SEEK_SET);

    bs_String* string = bs_stringAlloc(NULL, len);
    string->len = len;

    fread(string->value, 1, len, file);
    fclose(file);

    string->value[len - 1] = '\0';
    return string;
}

BSAPI bs_Result _bs_loadFile(const char* path, bs_String** out) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        bs_warnF("bs_loadFile(%s, ...) -> fopen(...) -> errno %d (%s)\n", path, errno, bs_serializeErrno());
        return bs_convertErrno();
    }

    *out = bs_loadFileFromHandle(file);
    return BS_RESULT_OK;
}

BSAPI bs_Result _bs_loadFileChunk(long offset, size_t size, const char* path, bs_String** out) {
    FILE* file = fopen(path, "rb");

    if (!file) {
        bs_warnF("bs_loadFileChunk(%s, ...) -> fopen(...) -> errno %d (%s)\n", path, errno, bs_serializeErrno());
        return bs_convertErrno();
    }
    
    if (fseek(file, offset, SEEK_SET) != 0) {
        fclose(file);
        return bs_convertErrno();
    }

    bs_String* buffer = bs_stringAlloc(NULL, size);

    size_t bytes_read = fread(buffer->value, 1, size, file);
    buffer->len = bytes_read;

    if (bytes_read != size) {
        bs_warnF("Read %lld/%lld bytes from \"%s\" at offset %lld\n", bytes_read, size, path, offset);
        return BS_RESULT_FAILED_TO_READ;
    }

    fclose(file);

    return buffer;
}

   /**
    Deleting documents
    */

BSAPI bs_Result _bs_deleteFile(const char* path) {
#ifdef _WIN32
    if (!DeleteFile(path)) {
        bs_warnF("_bs_deleteFile(%s) failed with error %lu\n", path, GetLastError());
        return bs_convertLastError();
    }
    bs_infoF("Deleted file %s\n", path);
    return BS_RESULT_OK;
#else
    bs_warnF("_bs_deleteFile not implemented on this platform\n");
    return BS_RESULT_NOT_IMPLEMENTED;
#endif
}

static int bs_doDeleteFile(char* path, void* param) {
    _bs_deleteFile(path);
    return 0;
}

BSAPI void _bs_deleteDirectoryContents(const char* path) {
#ifdef _WIN32
    _bs_foreachFile(bs_doDeleteFile, NULL, path);
#else
    bs_warnF("_bs_deleteDirectoryContents not implemented on this platform\n");
#endif
}

BSAPI void _bs_deleteDirectory(const char* path) {
#ifdef _WIN32
    _bs_deleteDirectoryContents(path);
    RemoveDirectory(path);
#else
    bs_warnF("_bs_deleteDirectory not implemented on this platform\n");
#endif
}

BSAPI bool _bs_directoryExists(const char* path) {
#ifdef _WIN32
    return _bs_fileExists(path);
#else
    bs_warnF("_bs_directoryExists not implemented on this platform\n");
    return false;
#endif
}

   /**
    Saving documents
    */

BSAPI bs_Result _bs_appendToFile(const char* path, const char* data) {
    FILE* file = fopen(path, "ab");
    if (!file) {
        bs_warnF("_bs_appendToFile(%s) -> fopen -> errno %d (%s)\n", path, errno, bs_serializeErrno());
        return bs_convertErrno();
    }

    fputs(data, file);
    fclose(file);
    return BS_RESULT_OK;
}

BSAPI bs_Result _bs_saveFile(const char* path, char* data, bs_U32 data_len) {
    FILE* file = fopen(path, "wb");
    if (!file) {
        bs_warnF("_bs_saveFile(%s) -> fopen -> errno %d (%s)\n", path, errno, bs_serializeErrno());
        return bs_convertErrno();
    }

    if (data)
        fwrite(data, data_len, 1, file);
    fclose(file);

    if (!_bs_args_.skip_log_info)
        bs_infoF("Saved %d bytes to %s\n", data_len, path);
    return BS_RESULT_OK;
}

BSAPI bs_String* _bs_fullPath(bs_String* old, const char* path, int path_len) {
    bs_String* cwd = bs_workingDirectory();
    bs_String* relative_path = bs_stringAlloc(old, path_len + cwd->len);
    relative_path = bs_appendString(relative_path, cwd->value, cwd->len);
    relative_path = bs_appendPath(relative_path, path, path_len);
    return relative_path;
}



  /*==============================================================================
   * GUID
   *============================================================================*/

BSAPI void _bs_guidToString(bs_GUID* guid, char out[37]) {
#ifdef _WIN32
    OLECHAR str[39] = { 0 };
    int len = StringFromGUID2(guid, str, 39);

    for (int i = 1; i < len - 2; i++)
        out[i - 1] = str[i];
    out[36] = '\0';

#else
    bs_warnF("_bs_guidToString not implemented on this platform\n");
#endif
}

BSAPI bs_GUID _bs_stringToGuid(const char* str) {
#ifdef _WIN32
    bs_GUID guid;
    wchar_t wstr[39];
    wchar_t formatted[39];

    mbstowcs(wstr, str, 37);
    swprintf(formatted, 39, L"{%s}", wstr);

    HRESULT hr = CLSIDFromString(formatted, &guid);
    if (FAILED(hr)) {
        bs_warnF("_bs_stringToGuid(%s) -> CLSIDFromString failed with HRESULT %lx\n", str, hr);
        return (bs_GUID) { 0 };
    }

    return guid;
#else
    bs_warnF("_bs_stringToGuid not implemented on this platform\n");
    return (bs_GUID) { 0 };
#endif
}

BSAPI bool _bs_sameGuid(bs_GUID* a, bs_GUID* b) {
    return a->a == b->a && a->b == b->b;
}

BSAPI bs_GUID _bs_guid() {
    bs_GUID guid = { 0 };
#ifdef _WIN32
    HRESULT hr = CoCreateGuid(&guid);
    if (FAILED(hr)) {
        bs_warnF("_bs_guid -> CoCreateGuid failed with HRESULT %lx\n", hr);
        return (bs_GUID) { 0 };
    }
#else
    bs_warnF("_bs_guid not implemented on this platform\n");
    return (bs_GUID) { 0 };
#endif
    return guid;
}

BSAPI bool _bs_guidIsNull(bs_GUID* guid) {
    bs_GUID empty = { 0 };
    return guid->a == 0 && guid->b == 0;
}



  /*==============================================================================
   * 
   *============================================================================*/

BSAPI int _bs_numDigits(int n) {
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}

BSAPI bs_I64 _bs_toLong(const char* str) {
    char* o = NULL;
    bs_I64 v = strtol(str, &o, 10);
    if (str == o) {
        bs_warnF("Failed to convert \"%s\" to Long\n", str);
        return 0;
    }
    return v;
}

BSAPI bs_U64 _bs_toULong(const char* str) {
    char* o = NULL;
    bs_I64 v = strtol(str, &o, 10);
    if (v < 0) {
        bs_warnF("Expected unsigned value for \"%s\"\n", str);
        return 0;
    }
    if (str == o) {
        bs_warnF("Failed to convert \"%s\" to ULong\n", str);
        return 0;
    }
    return v;
}

BSAPI bs_F64 _bs_toDouble(const char *str) {
    char* o = NULL;
    bs_F64 v = strtod(str, &o);
    if (str == o) {
        bs_warnF("Failed to convert \"%s\" to Double\n", str);
        return 0.0;
    }
    return v;
}

BSAPI bs_I64* _bs_toLongNull(const char* str, bs_I64* out) {
    char* o = NULL;
    *out = strtol(str, &o, 10);
    return str == *out ? NULL : out;
}

BSAPI bs_U64* _bs_toULongNull(const char* str, bs_U64* out) {
    char* o = NULL;
    *out = strtol(str, &o, 10);
    return (*out < 0 || str == o) ? NULL : out;
}

BSAPI bs_F64* _bs_toDoubleNull(const char* str, bs_F64* out) {
    char* o = NULL;
    *out = strtod(str, &o);
    return str == o ? NULL : out;
}



  /*==============================================================================
   * Windows Specific
   *============================================================================*/

#ifdef _WIN32

BSAPI void _bs_convertWin32Path(char* path, int len) {
    for (int i = 0; i < len; i++)
        path[i] = path[i] == '/' ? '\\' : path[i];
}

// todo document that this will alter the path from / to \\ for win32
// todo document limits
BSAPI void _bs_ensureDirectory(char* path) {
    _bs_convertWin32Path(path, strlen(path));
    DWORD file_attributes = GetFileAttributes(path);

    if (file_attributes == INVALID_FILE_ATTRIBUTES) {
        char* last = strrchr(path, '\\');

        if (last && last[1] != '\0') {
            last[0] = '\0';
            _bs_ensureDirectory(path);
            last[0] = '\\';
        }

        if (strrchr(path, '.')) // skip files
            return;

        if (!CreateDirectory(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
            bs_warnF("_bs_ensureDirectory(%s) -> CreateDirectory failed with error %lu\n", path, GetLastError());
    }
    else {
        bool is_directory_or_junction =
            ((file_attributes & FILE_ATTRIBUTE_DIRECTORY)     != 0) ||
            ((file_attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

       // if (!is_directory_or_junction)
       //     bs_throwLastWin32Error(path);
    }
}

BSAPI void _bs_ensureDirectoryF(const char* format, ...) {
    char path[256];
    int path_len = 0;
    BS_PARSE_FORMAT(format, path, path_len);
    _bs_ensureDirectory(path);
}

BSAPI void _bs_findExecutablePaths() {
    char executable_path[MAX_PATH];
    int len = GetModuleFileName(NULL, executable_path, MAX_PATH);

    _bs_io_.executable = bs_string(NULL, executable_path, len);
    char* exe_path = _bs_io_.executable->value;

    int i = 0;
    for (; i < len; i++) {
        char v = exe_path[i];
        exe_path[i] = v == '\\' ? '/' : v;
    }

    for (i = len - 1; i >= 0; i--) {
        if (exe_path[i] == '/') {
            if (i != len - 1) exe_path[i + 1] = '\0';
            break;
        }
    }

    len -= len - i - 1;
    _bs_io_.executable->len = len;
    bs_infoF("Executable path = (\"%s\")\n", exe_path);
}

BSAPI void _bs_findRelativePath() {
    char path[MAX_PATH]; // todo check if this can be more than max path
    int len = GetCurrentDirectory(MAX_PATH, path);
    if (len == 0) {
        bs_warnF("_bs_findRelativePath -> GetCurrentDirectory failed with error %lu\n", GetLastError());
        return;
    }

    _bs_io_.cwd = bs_string(NULL, path, len);
    for (int i = 0; i < len; i++) {
        char c = _bs_io_.cwd->value[i];
        _bs_io_.cwd->value[i] = c == '\\' ? '/' : c;
    }
}

BSAPI char* _bs_appdataPath() {
    if (_bs_io_.appdata)
        return _bs_io_.appdata->value;

    PWSTR wpath = NULL;
    HRESULT result = SHGetKnownFolderPath(&FOLDERID_RoamingAppData, 0, NULL, &wpath);

    if (result != S_OK) {
        bs_warnF("_bs_appdataPath -> SHGetKnownFolderPath failed with HRESULT %lx\n", result);
        return NULL;
    }

    int len = lstrlenW(wpath);
    _bs_io_.appdata = bs_string(_bs_io_.appdata, NULL, len);
    bs_unwiden(wpath, _bs_io_.appdata->value, len + 1);
    _bs_io_.appdata->len = len;

    bs_replaceCharOccurrences(_bs_io_.appdata->value, len, '\\', '/');

    return _bs_io_.appdata->value;
}



  /*==============================================================================
   * Clipboard
   *============================================================================*/

BSAPI void _bs_copyToClipboard(char* s, int len) {
    len++;
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(mem), s, len);
    GlobalUnlock(mem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, mem);
    CloseClipboard();

    bs_infoF("Copied %s to the clipboard\n", s);
}

BSAPI void _bs_copyToClipboardV(const char* format, va_list args) {
    char buffer[256];
    int len = vsnprintf(buffer, 256, format, args);
    assert(len < 256);
    _bs_copyToClipboard(buffer, len);
}

BSAPI void _bs_copyToClipboardF(const char* format, ...) {
    va_list args;
    va_start(args, format);
    _bs_copyToClipboardV(format, args);
    va_end(args);
}

#else
BSAPI char* _bs_executableDirectory() { bs_warnF("_bs_executableDirectory not implemented on this platform\n"); return NULL; }
BSAPI void _bs_findExecutablePaths() { bs_warnF("_bs_findExecutablePaths not implemented on this platform\n"); }
BSAPI void _bs_findCurrentWorkingDirectory() { bs_warnF("_bs_findCurrentWorkingDirectory not implemented on this platform\n"); }
BSAPI void _bs_ensureDirectory(const char* path) { bs_warnF("_bs_ensureDirectory not implemented on this platform\n"); }
#endif