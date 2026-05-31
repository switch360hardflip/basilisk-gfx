#ifndef BSGEN_STRING_H
#define BSGEN_STRING_H

#include <stdarg.h>

typedef struct {
    int len;
    int capacity;
    char value[];
} bsgen_String;

bsgen_String* 
bsgen_stringAlloc(
    bsgen_String* old, 
    int len);

bsgen_String* 
bsgen_string(
    bsgen_String* old,
    char* str, 
    int len);

bsgen_String* 
bsgen_emptyString(
    bsgen_String* old);

void 
bsgen_shortenString(
    bsgen_String* string, 
    int len);

bsgen_String* 
bsgen_appendString(
    bsgen_String* string, 
    char* append, 
    int len);

bsgen_String* 
bsgen_appendStringV(
    bsgen_String* string, 
    const char* format, 
    va_list args);

bsgen_String* 
bsgen_appendStringF(
    bsgen_String* string, 
    const char* format, ...);

bsgen_String* 
bsgen_appendChar(
    bsgen_String* string, 
    char c);

void 
bsgen_removeLastNChars(
    bsgen_String* string, 
    int n);

void 
bsgen_removeCharRange(
    bsgen_String* string, 
    int start, 
    int count);

bsgen_String* 
bsgen_insertChar(
    bsgen_String* string, 
    int index, 
    char c);

bsgen_String* 
bsgen_appendPath(
    bsgen_String* string, 
    char* path, 
    int path_len);

void 
bsgen_replaceCharOccurrences(
    char* string, 
    int string_len, 
    char a, 
    char b);

bsgen_String* 
bsgen_stringV(
    bsgen_String* old, 
    const char* format, 
    va_list args);

bsgen_String* 
bsgen_stringF(
    bsgen_String* old, 
    const char* format, ...);

#endif