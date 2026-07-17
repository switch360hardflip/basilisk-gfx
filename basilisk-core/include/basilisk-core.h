
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

#ifndef BASILISK_CORE_H
#define BASILISK_CORE_H

#include <basilisk-core.gen.h>

#define BS_IV2(x, y) (bs_ivec2) { x, y }
#define BS_IV3(x, y, z) (bs_ivec3) { x, y, z }
#define BS_IV4(x, y, z, w) (bs_ivec4) { x, y, z, w }

#define BS_V2(x, y) (bs_vec2) { x, y }
#define BS_V3(x, y, z) (bs_vec3) { x, y, z }
#define BS_V4(x, y, z, w) (bs_vec4) { x, y, z, w }

#define BS_V2_TO_V3(v, s) (bs_vec3) { v.x, v.y, s }
#define BS_V2_TO_V4(v, s1, s2) (bs_vec4) { v.x, v.y, s1, s2 }
#define BS_V3_TO_V4(v, s) (bs_vec4) { v.x, v.y, v.z, s }

#define BS_IV2_TO_V2(v) (bs_vec2) { v.x, v.y }
#define BS_IV3_TO_V3(v) (bs_vec4) { v.x, v.y, v.z }
#define BS_IV4_TO_V4(v) (bs_vec4) { v.x, v.y, v.z, v.w }

#define BS_V2_TO_DEGREES(v) BS_V2(bs_degrees(v.x), _bs_degrees(v.y))
#define BS_V3_TO_DEGREES(v) BS_V3(bs_degrees(v.x), _bs_degrees(v.y), _bs_degrees(v.z))

#define BS_V2_TO_RADIANS(v) BS_V2(bs_radians(v.x), _bs_radians(v.y))
#define BS_V3_TO_RADIANS(v) BS_V3(bs_radians(v.x), _bs_radians(v.y), _bs_radians(v.z))

#define BS_V2_ADD(a, b) (bs_vec2) { a.x + b.x, a.y + b.y }
#define BS_V3_ADD(a, b) (bs_vec3) { a.x + b.x, a.y + b.y, a.z + b.z }
#define BS_V4_ADD(a, b) (bs_vec4) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }

#define BS_V2_SUB(a, b) (bs_vec2) { a.x - b.x, a.y - b.y }
#define BS_V3_SUB(a, b) (bs_vec3) { a.x - b.x, a.y - b.y, a.z - b.z }
#define BS_V4_SUB(a, b) (bs_vec4) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }

#define BS_V2_MUL(a, b) (bs_vec2) { a.x * b.x, a.y * b.y }
#define BS_V3_MUL(a, b) (bs_vec3) { a.x * b.x, a.y * b.y, a.z * b.z }
#define BS_V4_MUL(a, b) (bs_vec4) { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }

#define BS_V2_DIV(a, b) (bs_vec2) { a.x / b.x, a.y / b.y }
#define BS_V3_DIV(a, b) (bs_vec3) { a.x / b.x, a.y / b.y, a.z / b.z }
#define BS_V4_DIV(a, b) (bs_vec4) { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }

#define BS_V2_ADD_S(a, s) (bs_vec2) { a.x + s, a.y + s }
#define BS_V3_ADD_S(a, s) (bs_vec3) { a.x + s, a.y + s, a.z + s }
#define BS_V4_ADD_S(a, s) (bs_vec4) { a.x + s, a.y + s, a.z + s, a.w + s }

#define BS_V2_SUB_S(a, s) (bs_vec2) { a.x - s, a.y - s }
#define BS_V3_SUB_S(a, s) (bs_vec3) { a.x - s, a.y - s, a.z - s }
#define BS_V4_SUB_S(a, s) (bs_vec4) { a.x - s, a.y - s, a.z - s, a.w - s }

#define BS_V2_MUL_S(a, s) (bs_vec2) { a.x * s, a.y * s }
#define BS_V3_MUL_S(a, s) (bs_vec3) { a.x * s, a.y * s, a.z * s }
#define BS_V4_MUL_S(a, s) (bs_vec4) { a.x * s, a.y * s, a.z * s, a.w * s }

#define BS_V2_DIV_S(a, s) (bs_vec2) { a.x / s, a.y / s }
#define BS_V3_DIV_S(a, s) (bs_vec3) { a.x / s, a.y / s, a.z / s }
#define BS_V4_DIV_S(a, s) (bs_vec4) { a.x / s, a.y / s, a.z / s, a.w / s }

#define BS_V3_MAX(a, b) BS_V3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z)
#define BS_V3_MIN(a, b) BS_V3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z)

#endif 