#ifndef BASILISK_MOD_H
#define BASILISK_MOD_H

#include <basilisk-mod.gen.h>
#include <bsmod_cache.h>

#define BSMOD_VALIDATE(condition, ret, format, ...)                  \
    if (!(condition)) {                                              \
        bs_warnF(BS_PRINT_COLOR("[MOD] [VAL]", BS_PRINT_RED) " %s: %s\n" __VA_OPT__(format) "\n", __func__, #condition __VA_OPT__(,) __VA_ARGS__); \
        return ret;                                                  \
    }

#endif
