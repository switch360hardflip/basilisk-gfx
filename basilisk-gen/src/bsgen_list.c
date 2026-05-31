#include <bsgen_list.h>
#include <string.h>
#include <stdlib.h>

#define BSGEN_MIN(a,b) (((a)<(b))?(a):(b))
#define BSGEN_MAX(a,b) (((a)>(b))?(a):(b))

void bsgen_ensureSize(bsgen_List* list, int num_units) {
    if ((list->count + num_units) < list->capacity)
        return;

    int prev_capacity = list->capacity;
    const int increment = 64;

    list->capacity += BSGEN_MAX(num_units, increment);
    list->data = realloc(list->data, list->capacity * list->unit_size);

    int size = (list->capacity - prev_capacity) * list->unit_size;

    void* unit = bsgen_fetchUnit(list, prev_capacity);
    if (unit)
        memset(unit, 0, size);
}

void* bsgen_pushBack(bsgen_List* list, unsigned char* data) {
    bsgen_ensureSize(list, 1);

    char* dest = bsgen_fetchUnit(list, list->count);
    if (data)
        memcpy(dest, data, list->unit_size);
    else memset(dest, 0, list->unit_size);

    list->count++;
    return dest;
}