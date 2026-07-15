#ifndef BASILISK_GFX_H
#define BASILISK_GFX_H

#include <basilisk-gfx.gen.h>
#include <../bsgfx_contracts.h>

#define BSGFX_FOREACH_PROC(X)                                        \
    X(PFN_void, bsmod_onIni)                                         \
    X(PFN_void, bsmod_onLateIni)                                     \
    X(PFN_void, bsmod_onLoad)                                        \
    X(PFN_void, bsmod_copyHoveringDataToBuffer)                      \
    X(PFN_bsmod_onCreateSubtypes, bsmod_onCreateQuadSubtypes)        \
    X(PFN_bsmod_onQueue, bsmod_onQueue)                              \
    X(PFN_void, bsmod_onGfxRender)                                   \
    X(PFN_bsmod_isSelected, bsmod_isSelected)                        \
    X(PFN_bsmod_onMap, bsmod_onMap)                                  \
    X(PFN_void, bsmod_onTrack)

typedef struct {
    BSGFX_FOREACH_PROC(BSGFX_STRUCT_GEN)
} bsgfx_Procedures;
BSGFXAPI extern bsgfx_Procedures _bsgfx_procs_;

#endif