#include <bsgen_string.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define BSGEN_STRING_OVERHEAD (1024)

bsgen_String* bsgen_stringAlloc(bsgen_String* old, int len) {
    bsgen_String* data;

    if (old) {
        if (old->capacity < len)
            data = realloc(old, sizeof(bsgen_String) + len + 1);
        else {
            len = old->capacity;
            data = old;
        }
    }
    else
        data = malloc(sizeof(bsgen_String) + len + 1);

    memset(data, 0, sizeof(bsgen_String));
    data->capacity = len;
    data->value[0] = '\0';
    return data;
}

bsgen_String* bsgen_string(bsgen_String* old, char* str, int len) {
    if (len < 0)
        len = strlen(str);
    bsgen_String* data = bsgen_stringAlloc(old, len);
    if (str)
        memcpy(data->value, str, len);
    data->value[len] = '\0';
    data->len = len;
    return data;
}

bsgen_String* bsgen_emptyString(bsgen_String* old) {
    return bsgen_string(old, "", 0);
}

void bsgen_shortenString(bsgen_String* string, int len) {
    string->len -= len;
    string->value[string->len] = '\0';
}

bsgen_String* bsgen_appendString(bsgen_String* string, char* append, int len) {
    if (!string)
        string = bsgen_stringAlloc(NULL, len + BSGEN_STRING_OVERHEAD);
    else if ((string->capacity - string->len) < len + 1) {
        string->capacity = string->len + len + BSGEN_STRING_OVERHEAD;
        string = realloc(string, sizeof(bsgen_String) + string->capacity + 1);
    }
    strncat(string->value, append, len);
    string->len += len;

    return string;
}

bsgen_String* bsgen_appendStringV(bsgen_String* string, const char* format, va_list args) {
    static bsgen_String* formatted;
    formatted = bsgen_stringV(formatted, format, args);

    return bsgen_appendString(string, formatted->value, formatted->len);
}

bsgen_String* bsgen_appendStringF(bsgen_String* string, const char* format, ...) {
    va_list args;
    va_start(args, format);
    bsgen_String* result = bsgen_appendStringV(string, format, args);
    va_end(args);
    return result;
}

bsgen_String* bsgen_appendChar(bsgen_String* string, char c) {
    if (!string)
        string = bsgen_stringAlloc(NULL, BSGEN_STRING_OVERHEAD);
    else if ((string->capacity - string->len) < 1) {
        string->capacity += BSGEN_STRING_OVERHEAD;
        string = realloc(string, sizeof(bsgen_String) + string->capacity + 1);
    }

    string->value[string->len++] = c;
    string->value[string->len] = '\0';
    return string;
}

void bsgen_removeLastNChars(bsgen_String* string, int n) {
    assert(string->len >= n);
    string->len -= n;
    string->value[string->len] = '\0';
}

void bsgen_removeCharRange(bsgen_String* string, int start, int count) {
    assert(start >= 0);
    assert(count >= 0);
    assert(start < string->len);

    if (start + count > string->len)
        count = string->len - start;

    memmove(
        string->value + start,
        string->value + start + count,
        string->len - (start + count) + 1);

    string->len -= count;
}

bsgen_String* bsgen_insertChar(bsgen_String* string, int index, char c) {
    if (!string)
        string = bsgen_stringAlloc(NULL, BSGEN_STRING_OVERHEAD);
    
    assert(index >= 0);
    assert(index < string->len);

    if ((string->capacity - string->len) < 1) {
        string->capacity += BSGEN_STRING_OVERHEAD;
        string = realloc(string, sizeof(bsgen_String) + string->capacity + 1);
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

bsgen_String* bsgen_appendPath(bsgen_String* string, char* path, int path_len) {
    if (path_len == 0 || string->value[string->len - 1] != '/')
        string = bsgen_appendChar(string, '/');
    return bsgen_appendString(string, path, path_len);
}

void bsgen_replaceCharOccurrences(char* string, int string_len, char a, char b) {
    for (int i = 0; i < string_len; i++)
        if (string[i] == a) string[i] = b;
}

bsgen_String* bsgen_stringV(bsgen_String* old, const char* format, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    int len = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    assert(len >= 0);

    bsgen_String* string = bsgen_stringAlloc(old, len);
    string->len = len;

    vsnprintf(string->value, len + 1, format, args);

    return string;
}

bsgen_String* bsgen_stringF(bsgen_String* old, const char* format, ...) {
    va_list args;
    va_start(args, format);

    bsgen_String* string = bsgen_stringV(old, format, args);

    va_end(args);
    return string;
}
