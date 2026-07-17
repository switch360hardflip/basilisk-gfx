
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

#ifndef BSMOD_INTERNAL_H
#define BSMOD_INTERNAL_H

#include <basilisk-mod.h>

static inline const char* bsmod_applicationContentPath() {
	return bs_fetchJson(&_bsmod_config_, BS_JSON_STRING, BS_CONSTANT_STRING("application.content_path")).as_string;
}

#define BSMOD_ENUM_GEN(ENUM) ENUM,
#define BSMOD_STRING_GEN(STRING) #STRING,



  /*==============================================================================
   * Images
   *============================================================================*/

#define BSMOD_IMAGE_IDS(X)									\
        X(BSMOD_IMAGE_DEPTH)                                \
        X(BSMOD_IMAGE_DEPTH_3D)                             \
        X(BSMOD_IMAGE_COLOR)

enum { BSMOD_IMAGE_IDS(BSMOD_ENUM_GEN) BSMOD_IMAGES_COUNT };
extern int _bsmod_images_;
#define BSMOD_IMAGES _bsmod_images_



  /*==============================================================================
   * Samplers
   *============================================================================*/

#define BSMOD_SAMPLER_IDS(X) \
	X(BSMOD_SAMPLER_PLACEHOLDER)

enum { BSMOD_SAMPLER_IDS(BSMOD_ENUM_GEN) BSMOD_SAMPLERS_COUNT };
extern int _bsmod_samplers_;
#define BSMOD_SAMPLERS _bsmod_samplers_



  /*==============================================================================
   * Buffers
   *============================================================================*/

#define BSMOD_BUFFER_IDS(X) \
	X(BSMOD_BUFFER_PLACEHOLDER)

enum { BSMOD_BUFFER_IDS(BSMOD_ENUM_GEN) BSMOD_BUFFERS_COUNT };
extern int _bsmod_buffers_;
#define BSMOD_BUFFERS _bsmod_buffers_



  /*==============================================================================
   * Batches
   *============================================================================*/

#define BSMOD_BATCH_IDS(X)									\
        X(BSMOD_BATCH_TILE)

enum { BSMOD_BATCH_IDS(BSMOD_ENUM_GEN) BSMOD_BATCHES_COUNT };
extern int _bsmod_batches_;
#define BSMOD_BATCHES _bsmod_batches_



  /*==============================================================================
   * Renderers
   *============================================================================*/

#define BSMOD_RENDERER_IDS(X)								\
        X(BSMOD_RENDERER)                                   \
        X(BSMOD_RENDERER_3D)

enum { BSMOD_RENDERER_IDS(BSMOD_ENUM_GEN) BSMOD_RENDERERS_COUNT };
extern int _bsmod_renderers_;
#define BSMOD_RENDERERS _bsmod_renderers_



  /*==============================================================================
   * Queues
   *============================================================================*/

#define BSMOD_QUEUE_IDS(X)									\
        X(BSMOD_QUEUE_GRAPHICS)                             \
        X(BSMOD_QUEUE_GRAPHICS_RASTERIZATION)

enum { BSMOD_QUEUE_IDS(BSMOD_ENUM_GEN) BSMOD_QUEUES_COUNT };
extern int _bsmod_queues_;
#define BSMOD_QUEUES _bsmod_queues_



  /*==============================================================================
   * Ray tracers
   *============================================================================*/

#define BSMOD_RAY_TRACER_IDS(X) \
	X(BSMOD_RAY_TRACER_PLACEHOLDER)

enum { BSMOD_RAY_TRACER_IDS(BSMOD_ENUM_GEN) BSMOD_RAY_TRACERS_COUNT };
extern int _bsmod_ray_tracers_;
#define BSMOD_RAY_TRACERS _bsmod_ray_tracers_



  /*==============================================================================
   * Atlases
   *============================================================================*/

#define BSMOD_ATLAS_IDS(X)                              \
        X(BSMOD_ATLAS_UI)                                   \
        X(BSMOD_ATLAS_MATERIAL_ICONS)                       \
        X(BSMOD_ATLAS_PRIMITIVE_ICONS)                      \
        X(BSMOD_ATLAS_PREFAB_ICONS)

enum { BSMOD_ATLAS_IDS(BSMOD_ENUM_GEN) BSMOD_ATLASES_COUNT };
extern int _bsmod_atlases_;
#define BSMOD_ATLASES _bsmod_atlases_



  /*==============================================================================
   * Fonts
   *============================================================================*/

#define BSMOD_FONT_IDS(X) \
	X(BSMOD_FONT_PLACEHOLDER)

enum { BSMOD_FONT_IDS(BSMOD_ENUM_GEN) BSMOD_FONTS_COUNT };
extern int _bsmod_fonts_;
#define BSMOD_FONTS _bsmod_fonts_

#endif
