#ifndef BSGEN_LIST_H
#define BSGEN_LIST_H

#include <assert.h>

#define BSGEN_LIST(type) { .unit_size = sizeof(type) }

typedef struct {
    int count;
    int unit_size;
    int capacity;
    unsigned char* data;
} bsgen_List;

static inline void* bsgen_fetchUnit(bsgen_List* list, int offset) {
    assert(offset < list->capacity);
    return list->data + offset * list->unit_size;
}

void 
bsgen_ensureSize(
    bsgen_List* list, 
    int num_units);

void*
bsgen_pushBack(
    bsgen_List* list,
    unsigned char* data);

#endif