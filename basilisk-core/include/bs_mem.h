
 /**
  bs_mem.h
  */

#ifndef BS_MEM_H
#define BS_MEM_H

#include <stdint.h>
#include <bs_types.h>
#include <bs_objects.h>

#define BS_STACK_LIST(type, c) \
    { .capacity = c, .data = _alloca(c * sizeof(type)), .unit_size = sizeof(type) }

BSAPI bs_Instance* bs_instance();
BSAPI bs_String* bs_stringBuilder();
BSAPI bs_Args* bs_args();
BSAPI bs_Features* bs_features();
BSAPI bs_Props* bs_props();
BSAPI bs_Settings* bs_settings();
BSAPI bs_Config* bs_config();

  /*==============================================================================
   * System
   *============================================================================*/

BSAPI void bs_system(char* s);
BSAPI void bs_systemAsync(char* s);
BSAPI void bs_systemV(const char* format, va_list args);
BSAPI void bs_systemF(const char* format, ...);



  /*==============================================================================
   * Strings
   *============================================================================*/

    BSAPI const char*
    bs_checkStringPool(
        bs_List* pool,
        char* string);

   /**
    Allocate an empty string
    @param old
    @param len - The length of the string, excluding NULL term.
    @returns Pointer to the created string
    */
    BSAPI bs_String* 
    bs_stringAlloc(
        bs_String* old,
        int len);

   /**
    Create a string type from an existing C string
    @param old
    @param data
    @param len 
    @returns Pointer to the created string
    */
    BSAPI bs_String* 
    bs_string(
        bs_String* old,
        char* data,
        int len);
    BSAPI bs_String* bs_emptyString(bs_String* old);

   /**
    Creates a string type from variadic arguments
    @param old
    @param format
    @param ...
    @returns Pointer to the created string
    */
    BSAPI bs_String* 
    bs_stringF(
        bs_String* old,
        const char* format, 
        ...);

    BSAPI bs_String* bs_stringV(bs_String* old, const char* format, va_list args);

    BSAPI void 
    bs_toUpper(
        char* string, 
        int len);

    BSAPI void 
    bs_toLower(
        char* string, 
        int len);

    BSAPI bs_U64
    bs_hash(
        unsigned char* data,
        size_t size);

    BSAPI bs_U64
    bs_stringHash(
        char* string);

    BSAPI bool 
    bs_startsWith(
        char* string, 
        const char* prefix);

    BSAPI bool 
    bs_endsWith(
        char* string, 
        const char* suffix);

    BSAPI char
    bs_lastChar(
        char* string,
        int len);

   /**
    @param string
    @param c
    @returns True if the character exists in the string, otherwise false
    */
    BSAPI bool
    bs_stringContainsChar(
        char* string,
        char c);

   /**
    @returns String containing the current working directory
    */
    BSAPI bs_String* 
    bs_workingDirectory();

    BSAPI void 
    bs_setWorkingDirectory(char* path);

   /**
    @returns String containing the path to the directory where the application executable is stored
    */
    BSAPI bs_String*
    bs_executablePath();

#ifdef _WIN32
    BSAPI char*
    bs_appdataPath();


  /*==============================================================================
   * Clipboard
   *============================================================================*/

   /**
    @param string
    @param len
    */
    BSAPI void 
    bs_copyToClipboard(
        char* string, 
        int len);

   /**
    @param string
    @param len
    */
    BSAPI void 
    bs_copyToClipboardV(
        const char* format, 
        va_list args);

   /**
    @param string
    @param len
    */
    BSAPI void 
    bs_copyToClipboardF(
        const char* format, 
        ...);

#endif

   /**
    @param string
    @param len - New location of the NULL term.
    */
    BSAPI void
    bs_shortenString(
        bs_String* string,
        int len);

   /**
    Appends a string to the end of another
    @param string - String that will be modified
    @param append - String that will be appended
    @param len - Length of the string to append
    @returns Pointer to the newly created string
    */
    BSAPI bs_String*
    bs_appendString(
        bs_String* string, 
        char* append, 
        int len);

   /**
    Appends a format string to the end of another
    @param string - String that will be modified
    @param format - String that will be appended
    @param ... - 
    @returns Pointer to the newly created string
    */
    BSAPI bs_String*
    bs_appendStringF(
        bs_String* string,
        const char* format, 
        ...);

    BSAPI bs_String* bs_appendStringV(bs_String* string, const char* format, va_list args);

   /**
    Appends a char to the end of a string
    @param string - String that will be modified
    @param c - Char to append
    @returns Pointer to the newly created string
    */
    BSAPI bs_String*
    bs_appendChar(
        bs_String* string,
        char c);

   /**
    @param string
    @param n - Number of chars to remove from the end of the string
    @throws BSX_OUT_OF_BOUNDS - string->len < n
    */
    BSAPI void
    bs_removeLastNChars(
        bs_String* string,
        int n);

    BSAPI void bs_removeCharRange(bs_String* string, int start, int count);
    BSAPI bs_String* bs_insertChar(bs_String* string, int index, char c);

   /**
    Appends a string to the end of another, and adds a slash if it doesn't exist
    @param - string - String that will be modified
    @param - path - String that will be appended
    @param - path_len - Length of the string to append
    @returns Pointer to the newly created string
    */
    BSAPI bs_String*
    bs_appendPath(
        bs_String* string,
        char* path,
        int path_len);

   /**
    @param string
    @param string_len
    @param a
    @param b
    */
    BSAPI void
    bs_replaceCharOccurrences(
        char* string,
        int string_len,
        char a,
        char b);



  /*==============================================================================
   * Misc
   *============================================================================*/

    BSAPI char*
    bs_strndup(
        const char* s, 
        size_t n);

    BSAPI size_t 
    bs_strnlen(
        const char* src, 
        size_t n);

    BSAPI char*
    bs_strsep(
        char** stringp,
        const char* delim);

    BSAPI void* 
    bs_memmem(
        const void* haystack, bs_U32 haystack_len, 
        const void* const needle, const bs_U32 needle_len);

    BSAPI bs_U32 
    bs_alignUp(
        bs_U32 value,
        bs_U32 alignment);

    #ifdef _WIN32
   /**
    Convert 
    @param src
    @param dst
    @param dst_size
    */
    BSAPI void 
    bs_widen(
        char* src, 
        wchar_t* dst, 
        bs_U32 dst_size);
    #endif

    BSAPI char*
    bs_charStringV(
        const char* format,
        va_list args);

    BSAPI char*
    bs_charStringF(
        const char* format,
        ...);

   /**
    These aren't really the same as the C std functions but whatever
    They throw errors and shit
    Fuck you
    */

   /**
    Same as free(...)
    */
    BSAPI void* 
    bs_free(
        void* p);

   /**
    Same as malloc(...)
    */
    BSAPI void*
    bs_malloc(
        bs_U64 size);

   /**
    Same as calloc(...)
    */
    BSAPI void*
    bs_calloc(
        bs_U64 num_units,
        bs_U64 unit_size);

   /**
    Same as realloc(...)
    */
    BSAPI void*
    bs_realloc(
        void* p,
        bs_U64 size);


  /*==============================================================================
   * Lists
   *============================================================================*/

    BSAPI void*
    bs_fetchUnitUnsafe(
        bs_List* list,
        bs_U32 offset);

    BSAPI bool 
    bs_listContains(
        bs_List* list, 
        void* data);

   /**
    Retreives an item from a list
    @param list
    @param index
    @returns Pointer to the item in the list
    @throws BSX_UNINITIALIZED_LIST
    */
    BSAPI void*
    bs_fetchUnit(
        bs_List* list,
        bs_U32 index);

   /**
    Retreives the last item from a list
    @param list
    @returns Pointer to the last item in the list
    @throws BSX_EMPTY_LIST
    */
    BSAPI void*
    bs_fetchLast(
        bs_List* list);

   /**
    Retreives the last item from a list
    @param list
    @returns Pointer to the last item in the list or NULL if the list is empty
    */
    BSAPI void*
    bs_fetchLastNull(
        bs_List* list);

   /**
    Ensure that the list has enough memory, allocates new units if it doesn't
    @param list
    @param num_units
    */
    BSAPI void
    bs_ensureSize(
        bs_List* list,
        bs_U32 num_units);

    BSAPI void 
    bs_erase(
        bs_List* list, 
        int index,
        bs_U32 count);

   /**
    Append an element to the end of the list
    @param list
    @param data
    @returns Pointer to the last item in the list
    */
    BSAPI void*
    bs_pushBack(
        bs_List* list, 
        char* data);

    BSAPI void* 
    bs_pushBackUnsafe(
        bs_List* list, 
        char* data);

    BSAPI void*
    bs_pushBackList(
        bs_List* destination,
        bs_List* source);

   /**
    Deallocates a list and all of its resources
    @param list
    */
    BSAPI void 
    bs_destroyList(
        bs_List* list);

   /**
    Sets the list's reference position where operations will be performed
    @param list
    @param unit_index
    */
    BSAPI void
    bs_seekList(
        bs_List* list,
        bs_U32 unit_index);

   /**
    Reallocates the list to the used size
    @param batch
    */
    BSAPI void 
    bs_minimizeList(
        bs_List* list);

   /**
    Creates a list
    @param unit_size - Size of each unit in the list
    @param increment - Amount of units to increment by when adding to the list
    @returns List that was created
    */
    BSAPI bs_List
    bs_list(
        int unit_size,
        int increment);



  /*==============================================================================
   * GUIDs
   *============================================================================*/

   /**
    Get the string equivalent of a GUID (XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX)
    @param guid
    @param out - Pointer to a string of size 37
    */
    BSAPI void
    bs_guidToString(
        bs_GUID* guid,
        char out[37]);

   /**
    Convert a GUID string to a GUID type
    @param str
    @returns Converted GUID
    @throws BSX_INVALID_FORMAT
    */
    BSAPI bs_GUID
    bs_stringToGuid(
        const char* str);

   /**
    @param a
    @param b
    @returns True if the two GUIDs are the same
    */
    BSAPI bool
    bs_sameGuid(
        bs_GUID* a,
        bs_GUID* b);

   /**
    @returns Randomly generated GUID
    */
    BSAPI bs_GUID
    bs_guid();

   /**
    @param guid
    @returs True if the guid is just 0s, otherwise false
    */
    BSAPI bool
    bs_guidIsNull(
        bs_GUID* guid);



  /*==============================================================================
   * File I/O
   *============================================================================*/

   /**
    Iterate through files in a directory, recursive, not recommended for large directories
    @param directory - Directory to iterate through
    @param void x(char* file_name, void* params) - Function that will be called for every file in the directory
    @param param - Parameter to send into the function
    */
    BSAPI void
    bs_foreachFile(
        void(*x)(bs_FileInfo, void*),
        void* param,
        const char* directory);

    /**
    Iterate through directories in a directory, recursive, not recommended for large directories
    @param directory - Directory to iterate through
    @param void x(char* directory_name, void* params) - Function that will be called for every directory in the directory
    @param param - Parameter to send into the function
    */
    BSAPI void
    bs_foreachDirectory( 
        void(*x)(bs_FileInfo, void*),
        void* param,
        const char* directory);

    // todo docs
    BSAPI void
    bs_foreachFileF(
        void(*x)(bs_FileInfo, void*),
        void* param, 
        char* format, 
        ...);

    BSAPI void
    bs_foreachDirectoryF(
        void(*x)(bs_FileInfo, void*),
        void* param, 
        char* format, 
        ...);

   /**
    @param directory
    @returns Number of files in the directory
    */
    BSAPI int
    bs_numFiles(
        const char* directory);

   /**
    @param format
    @param ...
    @returns Number of files in the directory
    */
    BSAPI int
    bs_numFilesF(
        const char* format,
        ...);

   /**
    @param directory
    @returns Number of directories in the directory
    */
    BSAPI int
    bs_numDirectories(
        const char* directory);

   /**
    @param format
    @param ...
    @returns Number of directories in the directory
    */
    BSAPI int 
    bs_numDirectoriesF(
        const char* format, 
        ...);

    BSAPI int
    bs_numDigits(
        int n);

   /**
    Load a file from disk
    @param path
    @throws BSX_FILE_NOT_FOUND
    @throws BSX_PERMISSION_DENIED
    @returns File contents or NULL
    */
    BSAPI bs_String*
    bs_loadFile(
        const char* path);

   /**
    Load a file from disk with a variadic name
    @param format
    @param ...
    @throws BSX_FILE_NOT_FOUND
    @throws BSX_PERMISSION_DENIED
    @returns File contents or NULL
    */
    BSAPI bs_String*
    bs_loadFileF(
        const char* format,
        ...);

    BSAPI bs_String* 
    bs_loadFileChunk(
        long offset, 
        size_t size,
        const char* path);

    BSAPI bs_String* 
    bs_loadFileChunkF(
        long offset,
        size_t size,
        const char* format,
        ...);

   /**
    @param path
    @throws Win32 exceptions
    */
    BSAPI void 
    bs_deleteFile(
        const char* path);

   /**
    @param format
    @param ...
    @throws Win32 exceptions
    */
    BSAPI void 
    bs_deleteFileF(
        const char* format, 
        ...);

   /**
    @param path
    @throws Win32 exceptions
    */
    BSAPI void 
    bs_deleteDirectoryContents(
        const char* path);

   /**
    @param format
    @param ...
    @throws Win32 exceptions
    */
    BSAPI void 
    bs_deleteDirectoryContentsF(
        const char* format, 
        ...);

   /**
    @param path
    @throws Win32 exceptions
    */
    BSAPI void 
    bs_deleteDirectory(
        const char* path);

   /**
    @param format
    @param ...
    @throws Win32 exceptions
    */
    BSAPI void 
    bs_deleteDirectoryF(
        const char* format, 
        ...);

   /**
    @param path
    @returns True if the directory exists, otherwise false
    */
    BSAPI bool
    bs_directoryExists(
        char* path);

   /**
    @param path
    @returns Pointer to the end of the string after the last .
    */
    BSAPI char* 
    bs_fileExtension(
        const char* path);

    BSAPI bool
    bs_fileExtensionIs(
        const char* path,
        char* extension);

   /**
    @param path
    @returns Pointer to the end of the string after the last /
    */
    BSAPI char*
    bs_fileName(
        const char* path);

   /**
    Appends data to an existing file
    @param path
    @param data
    @throws BSX_PERMISSION_DENIED
    */
    BSAPI void
    bs_appendFile(
        const char* path, 
        const char* data);

    // todo make path the last parameter
   /**
    Saves data to an existing file
    @param path
    @param data
    @param data_len
    @throws BSX_PERMISSION_DENIED
    */
    BSAPI void
    bs_saveFile(
        const char* path, 
        char* data, 
        bs_U32 data_len);

   /**
    Saves data to an existing file with a variadic name
    @param data
    @param data_len
    @param format
    @param ...
    @throws BSX_PERMISSION_DENIED
    */
    BSAPI void
    bs_saveFileF(
        char* data,
        bs_U32 data_len,
        char* format,
        ...);



  /*==============================================================================
   * Paths
   *============================================================================*/

#ifdef _WIN32
   /**
    Changes all / to \\
    @param path
    @parm len
    */
    BSAPI void
    bs_convertWin32Path(
        char* path,
        int len);
#endif

   /**
    Creates a directory if it doesn't exist
    @param path
    @throws BSX_PERMISSION_DENIED
    */
    BSAPI void
    bs_ensureDirectory(
        const char* path);

   /**
    Creates a directory if it doesn't exist
    @param format
    @param ...
    @throws BSX_PERMISSION_DENIED
    */
    BSAPI void
    bs_ensureDirectoryF(
        const char* format, ...);



  /*==============================================================================
   * Time
   *============================================================================*/

   /**
    @param path
    @returns Date and time the file was last modified
    */
    BSAPI bs_DateTime
    bs_fileModifiedDate(
        const char* path);

    BSAPI void
    bs_setFileModifiedDate(
        const char* path,
        bs_DateTime* date_time);

   /**
    @param format
    @param ...
    @returns Data and time the file was last modified
    */
    BSAPI bs_DateTime
    bs_fileModifiedDateF(
        const char* format,
        ...);

   /**
    @param old
    @param path - Relative path to the file
    @param path_len
    @returns New string containing the absolute path of the file
    */
    BSAPI bs_String*
    bs_fullPath(
        bs_String* old,
        const char* path,
        int path_len);

   /**
    @param path
    @returns True if the file exists, otherwise false
    */
    BSAPI bool
    bs_fileExists(
        const char* path);

   /**
    @param format
    @param ...
    @returns True if the file exists, otherwise false
    */
    BSAPI bool
    bs_fileExistsF(
        char* format,
        ...);

   /**
    @param str
    @returns Converted value
    @throws BSX_CONVERSION_FAILURE - String is not just a long
    */
    BSAPI bs_I64
    bs_toLong(
        const char* str);
   /**
    @param str
    @returns Converted value
    @throws BSX_NEGATIVE_ULONG - String contains a negative value
    @throws BSX_CONVERSION_FAILURE - String is not just an unsigned long
    */
    BSAPI bs_U64 
    bs_toULong(
        const char* str);

   /**
    @param str
    @returns Converted value
    @throws BSX_CONVERSION_FAILURE - String is not just a double
    */
    BSAPI bs_F64
    bs_toDouble(
        const char* str);

   /**
    @param str
    @param inout - Value to be converted
    @returns Converted value or NULL
    */
    BSAPI bs_I64* 
    bs_toLongNull(
        const char* str, 
        bs_I64* inout);

   /**
    @param str
    @param inout - Value to be converted
    @returns Converted value or NULL
    */
    BSAPI bs_U64* 
    bs_toULongNull(
        const char* str, 
        bs_U64* inout);
    
   /**
    @param str
    @param inout - Value to be converted
    @returns Converted value or NULL
    */
    BSAPI bs_F64* 
    bs_toDoubleNull(
        const char* str, 
        bs_F64* inout);

#endif