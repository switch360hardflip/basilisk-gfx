
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
  bs_math.c

  Collection of math tools

  CGLM wrapper functions such as vector and matrix operations are 
  generated from basilisk-core.xml, cglm functions are called 
  directly from the trampoline, see generated/basilisk-core.c
  */

#include <basilisk-core.h>
#include <bs_internal.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <cglm/mat3.h>
#include <cglm/mat4.h>
#include <cglm/quat.h>
#include <cglm/cam.h>
#include <math.h>



  /*==============================================================================
   * Vectors
   =============================================================================*/

BSAPI void _bs_v2Mid(bs_vec2* a, bs_vec2* b, bs_vec2* out) {
    *out = bs_v2((a->x + b->x) / 2.0f, (a->y + b->y) / 2.0f);
}

BSAPI void _bs_v3Mid(bs_vec3* a, bs_vec3* b, bs_vec3* out) {
    *out = bs_v3((a->x + b->x) / 2.0f, (a->y + b->y) / 2.0f, (a->z + b->z) / 2.0f);
}



  /*==============================================================================
   * Bezier Curves
   =============================================================================*/

static float bs_cubicBezier(float p0, float p1, float p2, float p3, float t) {
    float curve;

    curve = powf(1.0f - t, 3.0f) * p0;
    curve += 3 * powf(1.0f - t, 2.0f) * t * p1;
    curve += 3 * powf(1.0f - t, 1.0f) * powf(t, 2.0f) * p2;
    curve += powf(t, 3.0f) * p3;

    return curve;
}

static float bs_quadBezier(float p0, float p1, float p2, float t) {
    float u = 1.0f - t;
    return u * u * p0 + 2.0f * u * t * p1 + t * t * p2;
}

static void bs_nCubicBezier(int n, const bs_vec4* p0, const bs_vec4* p1, const bs_vec4* p2, const bs_vec4* p3, bs_vec4* out, int out_length) {
    float t = 0.0f;
    float incr = 1.0f / (float)out_length;

    for (int i = 0; i < out_length; i++, t += incr) {
        for (int j = 0; j < n; j++)
            out[i].a[j] = bs_cubicBezier(p0->a[j], p1->a[j], p2->a[j], p3->a[j], t);
    }
}

static void bs_nQuadBezier(int n, const bs_vec4* p0, const bs_vec4* p1, const bs_vec4* p2, bs_vec4* out, int out_length) {
    float t = 0.0;
    float incr = 1.0 / (float)out_length;

    for (int i = 0; i < out_length; i++, t += incr) {
        for (int j = 0; j < n; j++)
            out[i].a[j] = bs_quadBezier(p0->x, p1->x, p2->x, t);
    }
}

BSAPI void _bs_v2CubicBezier(const bs_vec2* p0, const bs_vec2* p1, const bs_vec2* p2, const bs_vec2* p3, bs_vec2* out, int out_length) {
    bs_nCubicBezier(2, p0, p1, p2, p3, out, out_length);
}

BSAPI void _bs_v2QuadBezier(const bs_vec2* p0, const bs_vec2* p1, const bs_vec2* p2, const bs_vec2* p3, bs_vec2* out, int out_length) {
    bs_nQuadBezier(2, p0, p1, p2, out, out_length);
}

BSAPI void _bs_v3CubicBezier(const bs_vec3* p0, const bs_vec3* p1, const bs_vec3* p2, const bs_vec3* p3, bs_vec3* out, int out_length) {
    bs_nCubicBezier(3, p0, p1, p2, p3, out, out_length);
}

BSAPI void _bs_v3QuadBezier(const bs_vec3* p0, const bs_vec3* p1, const bs_vec3* p2, const bs_vec3* p3, bs_vec3* out, int out_length) {
    bs_nQuadBezier(3, p0, p1, p2, out, out_length);
}




  /*==============================================================================
   * Lines
   =============================================================================*/

// https://gist.github.com/TimSC/47203a0f5f15293d2099507ba5da44e6
static inline double bs_determinate(float a, float b, float c, float d) {
    return a * d - b * c;
}

BSAPI bool _bs_lineVsLine(bs_vec2 l1_start, bs_vec2 l1_end, bs_vec2 l2_start, bs_vec2 l2_end, bs_vec2* out) {
    double detL1 = bs_determinate(l1_start.x, l1_start.y, l1_end.x, l1_end.y);
    double detL2 = bs_determinate(l2_start.x, l2_start.y, l2_end.x, l2_end.y);
    double x1mx2 = l1_start.x - l1_end.x;
    double x3mx4 = l2_start.x - l2_end.x;
    double y1my2 = l1_start.y - l1_end.y;
    double y3my4 = l2_start.y - l2_end.y;

    double xnom = bs_determinate(detL1, x1mx2, detL2, x3mx4);
    double ynom = bs_determinate(detL1, y1my2, detL2, y3my4);
    double denom = bs_determinate(x1mx2, y1my2, x3mx4, y3my4);

    if (denom == 0.0) {
        if (out)
            *out = (bs_vec2){ NAN, NAN };

        return false;
    }

    bs_vec2 result = {
        .x = xnom / denom,
        .y = ynom / denom,
    };

    if (out)
        *out = result;

    if (!isfinite(result.x) || !isfinite(result.y))
        return false;

    return true;
}
