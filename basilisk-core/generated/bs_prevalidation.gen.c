
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
  This file was generated from basilisk-gfx.com

  It is not recommended to make changes to this file as it will be lost if
  the code is regenerated.
  */
        
#include <basilisk-core.gen.h>
#include <bs_internal.h>

static bs_FunctionTable next = { 0 };

void _preval_bs_setFunctions(bs_FunctionTable functions) {
    next = functions;
}

static void _preval_bs_v2Add(const bs_vec2* a, const bs_vec2* b, bs_vec2* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Add(a, b, out);
}

static void _preval_bs_v2Sub(const bs_vec2* a, const bs_vec2* b, bs_vec2* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Sub(a, b, out);
}

static void _preval_bs_v2Mul(const bs_vec2* a, const bs_vec2* b, bs_vec2* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Mul(a, b, out);
}

static void _preval_bs_v2Div(const bs_vec2* a, const bs_vec2* b, bs_vec2* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Div(a, b, out);
}

static void _preval_bs_v2AddS(const bs_vec2* v, float s, bs_vec2* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2AddS(v, s, out);
}

static void _preval_bs_v2SubS(const bs_vec2* v, float s, bs_vec2* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2SubS(v, s, out);
}

static void _preval_bs_v2MulS(const bs_vec2* v, float s, bs_vec2* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2MulS(v, s, out);
}

static void _preval_bs_v2DivS(const bs_vec2* v, float s, bs_vec2* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2DivS(v, s, out);
}

static float _preval_bs_v2Dot(const bs_vec2* a, const bs_vec2* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v2Dot(a, b);
}

static float _preval_bs_v2Distance(const bs_vec2* a, const bs_vec2* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v2Distance(a, b);
}

static float _preval_bs_v2Magnitude(const bs_vec2* v) {
    if (v == NULL)
        return 0;

    return next.bs_v2Magnitude(v);
}

static float _preval_bs_v2MagnitudeSqrd(const bs_vec2* v) {
    if (v == NULL)
        return 0;

    return next.bs_v2MagnitudeSqrd(v);
}

static void _preval_bs_v2Normalize(const bs_vec2* v, bs_vec2* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Normalize(v, out);
}

static void _preval_bs_v2Lerp(const bs_vec2* from, const bs_vec2* to, float t, bs_vec2* out) {
    if (from == NULL)
        return;

    if (to == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Lerp(from, to, t, out);
}

static void _preval_bs_v2Mid(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v2Mid(a, b, out);
}

static void _preval_bs_v3Add(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Add(a, b, out);
}

static void _preval_bs_v3Sub(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Sub(a, b, out);
}

static void _preval_bs_v3Mul(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Mul(a, b, out);
}

static void _preval_bs_v3Div(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Div(a, b, out);
}

static void _preval_bs_v3AddS(const bs_vec3* v, float s, bs_vec3* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3AddS(v, s, out);
}

static void _preval_bs_v3SubS(const bs_vec3* v, float s, bs_vec3* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3SubS(v, s, out);
}

static void _preval_bs_v3MulS(const bs_vec3* v, float s, bs_vec3* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3MulS(v, s, out);
}

static void _preval_bs_v3DivS(const bs_vec3* v, float s, bs_vec3* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3DivS(v, s, out);
}

static float _preval_bs_v3Dot(const bs_vec3* a, const bs_vec3* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v3Dot(a, b);
}

static float _preval_bs_v3Distance(const bs_vec3* a, const bs_vec3* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v3Distance(a, b);
}

static float _preval_bs_v3Magnitude(const bs_vec3* v) {
    if (v == NULL)
        return 0;

    return next.bs_v3Magnitude(v);
}

static float _preval_bs_v3MagnitudeSqrd(const bs_vec3* v) {
    if (v == NULL)
        return 0;

    return next.bs_v3MagnitudeSqrd(v);
}

static void _preval_bs_v3Normalize(const bs_vec3* v, bs_vec3* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Normalize(v, out);
}

static void _preval_bs_v3Lerp(const bs_vec3* from, const bs_vec3* to, float t, bs_vec3* out) {
    if (from == NULL)
        return;

    if (to == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Lerp(from, to, t, out);
}

static void _preval_bs_v3Mid(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Mid(a, b, out);
}

static void _preval_bs_v3Cross(const bs_vec3* a, const bs_vec3* b, bs_vec3* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v3Cross(a, b, out);
}

static float _preval_bs_v3Angle(const bs_vec3* a, const bs_vec3* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v3Angle(a, b);
}

static void _preval_bs_v4Add(const bs_vec4* a, const bs_vec4* b, bs_vec4* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4Add(a, b, out);
}

static void _preval_bs_v4Sub(const bs_vec4* a, const bs_vec4* b, bs_vec4* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4Sub(a, b, out);
}

static void _preval_bs_v4Mul(const bs_vec4* a, const bs_vec4* b, bs_vec4* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4Mul(a, b, out);
}

static void _preval_bs_v4Div(const bs_vec4* a, const bs_vec4* b, bs_vec4* out) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4Div(a, b, out);
}

static void _preval_bs_v4AddS(const bs_vec4* v, float s, bs_vec4* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4AddS(v, s, out);
}

static void _preval_bs_v4SubS(const bs_vec4* v, float s, bs_vec4* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4SubS(v, s, out);
}

static void _preval_bs_v4MulS(const bs_vec4* v, float s, bs_vec4* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4MulS(v, s, out);
}

static void _preval_bs_v4DivS(const bs_vec4* v, float s, bs_vec4* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4DivS(v, s, out);
}

static float _preval_bs_v4Dot(const bs_vec4* a, const bs_vec4* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v4Dot(a, b);
}

static float _preval_bs_v4Distance(const bs_vec4* a, const bs_vec4* b) {
    if (a == NULL)
        return 0;

    if (b == NULL)
        return 0;

    return next.bs_v4Distance(a, b);
}

static float _preval_bs_v4Magnitude(const bs_vec4* v) {
    if (v == NULL)
        return 0;

    return next.bs_v4Magnitude(v);
}

static float _preval_bs_v4MagnitudeSqrd(const bs_vec4* v) {
    if (v == NULL)
        return 0;

    return next.bs_v4MagnitudeSqrd(v);
}

static void _preval_bs_v4Normalize(const bs_vec4* v, bs_vec4* out) {
    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4Normalize(v, out);
}

static void _preval_bs_v4Lerp(const bs_vec4* from, const bs_vec4* to, float t, bs_vec4* out) {
    if (from == NULL)
        return;

    if (to == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_v4Lerp(from, to, t, out);
}

static void _preval_bs_m3Mul(const bs_mat3* a, const bs_mat3* b, const bs_mat3* result) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (result == NULL)
        return;

    return next.bs_m3Mul(a, b, result);
}

static void _preval_bs_m3Transpose(const bs_mat3* m, const bs_mat3* result) {
    if (m == NULL)
        return;

    if (result == NULL)
        return;

    return next.bs_m3Transpose(m, result);
}

static void _preval_bs_m3Inverse(const bs_mat3* m, const bs_mat3* result) {
    if (m == NULL)
        return;

    if (result == NULL)
        return;

    return next.bs_m3Inverse(m, result);
}

static void _preval_bs_m3MulV3(const bs_mat3* m, const bs_vec3* v, bs_vec3* out) {
    if (m == NULL)
        return;

    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m3MulV3(m, v, out);
}

static void _preval_bs_m4Mul(const bs_mat4* a, const bs_mat4* b, const bs_mat4* result) {
    if (a == NULL)
        return;

    if (b == NULL)
        return;

    if (result == NULL)
        return;

    return next.bs_m4Mul(a, b, result);
}

static void _preval_bs_m4Transpose(const bs_mat4* m, const bs_mat4* result) {
    if (m == NULL)
        return;

    if (result == NULL)
        return;

    return next.bs_m4Transpose(m, result);
}

static void _preval_bs_m4Inverse(const bs_mat4* m, const bs_mat4* result) {
    if (m == NULL)
        return;

    if (result == NULL)
        return;

    return next.bs_m4Inverse(m, result);
}

static bs_mat4x3 _preval_bs_m4x3(const bs_mat4* m) {
    if (m == NULL)
        return (bs_mat4x3) { 0 };

    return next.bs_m4x3(m);
}

static void _preval_bs_m4MulV3(const bs_mat4* m, const bs_vec3* v, bs_vec3* out) {
    if (m == NULL)
        return;

    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m4MulV3(m, v, out);
}

static void _preval_bs_m4MulV4(const bs_mat4* m, const bs_vec4* v, bs_vec4* out) {
    if (m == NULL)
        return;

    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m4MulV4(m, v, out);
}

static void _preval_bs_m4Translate(const bs_mat4* m, const bs_vec3* v, bs_mat4* out) {
    if (m == NULL)
        return;

    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m4Translate(m, v, out);
}

static void _preval_bs_m4Rotate(const bs_mat4* m, const bs_vec4* q, bs_mat4* out) {
    if (m == NULL)
        return;

    if (q == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m4Rotate(m, q, out);
}

static void _preval_bs_m4Scale(const bs_mat4* m, const bs_vec3* v, bs_mat4* out) {
    if (m == NULL)
        return;

    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m4Scale(m, v, out);
}

static void _preval_bs_m3ToQ(const bs_mat3* m, bs_vec4* out) {
    if (m == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m3ToQ(m, out);
}

static void _preval_bs_m4ToQ(const bs_mat4* m, bs_vec4* out) {
    if (m == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_m4ToQ(m, out);
}

static void _preval_bs_qToM3(const bs_vec4* q, bs_mat3* out) {
    if (q == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qToM3(q, out);
}

static void _preval_bs_qToM4(const bs_vec4* q, bs_mat4* out) {
    if (q == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qToM4(q, out);
}

static void _preval_bs_qNormalize(const bs_vec4* q, bs_vec4* out) {
    if (q == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qNormalize(q, out);
}

static void _preval_bs_qSlerp(const bs_vec4* from, const bs_vec4* to, float t, bs_vec4* out) {
    if (from == NULL)
        return;

    if (to == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qSlerp(from, to, t, out);
}

static void _preval_bs_qRotateV3(const bs_vec4* q, const bs_vec3* v, bs_vec3* out) {
    if (q == NULL)
        return;

    if (v == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qRotateV3(q, v, out);
}

static void _preval_bs_qLongSlerp(const bs_vec4* from, const bs_vec4* to, float t, bs_vec4* out) {
    if (from == NULL)
        return;

    if (to == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qLongSlerp(from, to, t, out);
}

static void _preval_bs_eulToQ(const bs_vec3* eul, bs_vec4* out) {
    if (eul == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_eulToQ(eul, out);
}

static void _preval_bs_qToEul(const bs_vec4* q, bs_vec3* out) {
    if (q == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_qToEul(q, out);
}

static void _preval_bs_orthographic(float left, float right, float bottom, float top, float near_z, float far_z, bs_mat4* out) {
    if (out == NULL)
        return;

    return next.bs_orthographic(left, right, bottom, top, near_z, far_z, out);
}

static void _preval_bs_perspective(float fov, float aspect, float near_z, float far_z, bs_mat4* out) {
    if (out == NULL)
        return;

    return next.bs_perspective(fov, aspect, near_z, far_z, out);
}

static void _preval_bs_lookAt(const bs_vec3* eye, const bs_vec3* center, const bs_vec3* up, bs_mat4* out) {
    if (eye == NULL)
        return;

    if (center == NULL)
        return;

    if (up == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_lookAt(eye, center, up, out);
}

static void _preval_bs_look(const bs_vec3* eye, const bs_vec3* direction, const bs_vec3* up, bs_mat4* out) {
    if (eye == NULL)
        return;

    if (direction == NULL)
        return;

    if (up == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_look(eye, direction, up, out);
}

static float _preval_bs_v2CubicBezier(const bs_vec2* p0, const bs_vec2* p1, const bs_vec2* p2, const bs_vec2* p3, bs_vec2* out, int out_length) {
    if (p0 == NULL)
        return 0;

    if (p1 == NULL)
        return 0;

    if (p2 == NULL)
        return 0;

    if (p3 == NULL)
        return 0;

    if (out == NULL)
        return 0;

    return next.bs_v2CubicBezier(p0, p1, p2, p3, out, out_length);
}

static float _preval_bs_v2QuadBezier(const bs_vec2* p0, const bs_vec2* p1, const bs_vec2* p2, bs_vec2* out, int out_length) {
    if (p0 == NULL)
        return 0;

    if (p1 == NULL)
        return 0;

    if (p2 == NULL)
        return 0;

    if (out == NULL)
        return 0;

    return next.bs_v2QuadBezier(p0, p1, p2, out, out_length);
}

static float _preval_bs_v3CubicBezier(const bs_vec3* p0, const bs_vec3* p1, const bs_vec3* p2, const bs_vec3* p3, bs_vec3* out, int out_length) {
    if (p0 == NULL)
        return 0;

    if (p1 == NULL)
        return 0;

    if (p2 == NULL)
        return 0;

    if (p3 == NULL)
        return 0;

    if (out == NULL)
        return 0;

    return next.bs_v3CubicBezier(p0, p1, p2, p3, out, out_length);
}

static float _preval_bs_v3QuadBezier(const bs_vec3* p0, const bs_vec3* p1, const bs_vec3* p2, bs_vec3* out, int out_length) {
    if (p0 == NULL)
        return 0;

    if (p1 == NULL)
        return 0;

    if (p2 == NULL)
        return 0;

    if (out == NULL)
        return 0;

    return next.bs_v3QuadBezier(p0, p1, p2, out, out_length);
}

static float _preval_bs_abs(float v) {
    return next.bs_abs(v);
}

static float _preval_bs_sin(float v) {
    return next.bs_sin(v);
}

static float _preval_bs_cos(float v) {
    return next.bs_cos(v);
}

static float _preval_bs_tan(float v) {
    return next.bs_tan(v);
}

static int _preval_bs_sign(float v) {
    return next.bs_sign(v);
}

static bs_Quad _preval_bs_quad(bs_vec3 position, bs_vec2 dimensions) {
    return next.bs_quad(position, dimensions);
}

static float _preval_bs_lerp(float from, float to, float t) {
    return next.bs_lerp(from, to, t);
}

static float _preval_bs_degrees(float radians) {
    return next.bs_degrees(radians);
}

static float _preval_bs_radians(float degrees) {
    return next.bs_radians(degrees);
}

static bs_vec3 _preval_bs_hsvToRgb(const bs_vec3* hsv) {
    if (hsv == NULL)
        return (bs_vec3) { 0 };

    return next.bs_hsvToRgb(hsv);
}

static bs_vec3 _preval_bs_rgbToHsv(const bs_vec3* rgb) {
    if (rgb == NULL)
        return (bs_vec3) { 0 };

    return next.bs_rgbToHsv(rgb);
}

static bs_Result _preval_bs_convertVulkanResult(int code) {
    return next.bs_convertVulkanResult(code);
}

static bs_Result _preval_bs_convertWin32Error(unsigned long code) {
    return next.bs_convertWin32Error(code);
}

static const char* _preval_bs_serializeWin32Error(unsigned long code) {
    return next.bs_serializeWin32Error(code);
}

static bs_Result _preval_bs_convertErrno() {
    return next.bs_convertErrno();
}

static const char* _preval_bs_serializeErrno() {
    return next.bs_serializeErrno();
}

static bs_Result _preval_bs_playSound(bs_Sound* sound, float volume) {
    if (sound == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_playSound(sound, volume);
}

static bs_Result _preval_bs_sound(bs_Resource* resource, bs_U32 flags) {
    if (resource == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_sound(resource, flags);
}

static bs_Result _preval_bs_iniAudio() {
    return next.bs_iniAudio();
}

static bs_Result _preval_bs_ray(bs_vec3 start, bs_vec3 direction, float length, bs_Ray* ray) {
    if (ray == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_ray(start, direction, length, ray);
}

static void _preval_bs_rayVsObb(const bs_Ray* ray, const bs_vec3* position, const bs_vec4* rotation, const bs_vec3* scale, bs_RayVsObb* out) {
    if (ray == NULL)
        return;

    if (position == NULL)
        return;

    if (rotation == NULL)
        return;

    if (scale == NULL)
        return;

    if (out == NULL)
        return;

    return next.bs_rayVsObb(ray, position, rotation, scale, out);
}

static void _preval_bs_sphereVsPoint(bs_vec3 center, float radius, bs_vec3 point, bs_SphereVsPoint* out) {
    if (out == NULL)
        return;

    return next.bs_sphereVsPoint(center, radius, point, out);
}

static bool _preval_bs_sphereVsObbTest(const bs_vec3* center, float radius, const bs_vec3* position, const bs_vec4* rotation, const bs_vec3* scale) {
    if (center == NULL)
        return false;

    if (position == NULL)
        return false;

    if (rotation == NULL)
        return false;

    if (scale == NULL)
        return false;

    return next.bs_sphereVsObbTest(center, radius, position, rotation, scale);
}

static bool _preval_bs_sphereVsObb(const bs_vec3* center, float radius, const bs_vec3* position, const bs_vec4* rotation, const bs_vec3* scale, bs_SphereVsBox* out) {
    if (center == NULL)
        return false;

    if (position == NULL)
        return false;

    if (rotation == NULL)
        return false;

    if (scale == NULL)
        return false;

    if (out == NULL)
        return false;

    return next.bs_sphereVsObb(center, radius, position, rotation, scale, out);
}

static bool _preval_bs_rectangleVsPoint(const bs_vec2* position, const bs_vec2* dimensions, const bs_vec2* point) {
    if (position == NULL)
        return false;

    if (dimensions == NULL)
        return false;

    if (point == NULL)
        return false;

    return next.bs_rectangleVsPoint(position, dimensions, point);
}

static void _preval_bs_lineVsLine(bs_vec2 l1_start, bs_vec2 l1_end, bs_vec2 l2_start, bs_vec2 l2_end, bs_LineVsLine* out) {
    if (out == NULL)
        return;

    return next.bs_lineVsLine(l1_start, l1_end, l2_start, l2_end, out);
}

static void _preval_bs_populateVertexDeclaration(bs_VertexDeclaration* declaration, bs_Attribute* attributes, int attributes_count) {
    if (declaration == NULL)
        return;

    if (attributes == NULL)
        return;

    if (!(attributes_count > 0))
        return;

    return next.bs_populateVertexDeclaration(declaration, attributes, attributes_count);
}

static int _preval_bs_currentSwap() {
    return next.bs_currentSwap();
}

static void _preval_bs_beginComment(char* value, int value_length) {
    if (value == NULL)
        return;

    return next.bs_beginComment(value, value_length);
}

static void _preval_bs_endComment() {
    return next.bs_endComment();
}

static bs_Image* _preval_bs_swapchainImage() {
    return next.bs_swapchainImage();
}

static bs_Result _preval_bs_clearStencil(bs_U32 index, bs_ivec2 resolution, bs_U32 value) {
    return next.bs_clearStencil(index, resolution, value);
}

static bs_Result _preval_bs_clearDepth(bs_U32 index, bs_ivec2 dim, float value) {
    return next.bs_clearDepth(index, dim, value);
}

static bs_Result _preval_bs_clearDepthStencil(bs_U32 index, bs_ivec2 dim, float depth_value, bs_U32 stencil_value) {
    return next.bs_clearDepthStencil(index, dim, depth_value, stencil_value);
}

static bs_Result _preval_bs_clearColor(bs_U32 index, bs_ivec2 dim, bs_RGBA color) {
    return next.bs_clearColor(index, dim, color);
}

static bs_Result _preval_bs_stencilReference(bs_FaceType face, bs_U32 reference) {
    return next.bs_stencilReference(face, reference);
}

static bs_Result _preval_bs_cull(bs_CullFlags flags) {
    return next.bs_cull(flags);
}

static bs_Result _preval_bs_setLineWidth(float width) {
    return next.bs_setLineWidth(width);
}

static int _preval_bs_batchSize(bs_Batch* batch) {
    if (batch == NULL)
        return 0;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return 0;

    return next.bs_batchSize(batch);
}

static void _preval_bs_render(bs_Batch* batch, bs_Pipeline* pipeline, bs_U32 vertex_offset, bs_U32 vertex_count, bs_U32 first_instance, bs_U32 num_instances) {
    if (batch == NULL)
        return;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return;

    if (pipeline == NULL)
        return;

    return next.bs_render(batch, pipeline, vertex_offset, vertex_count, first_instance, num_instances);
}

static bs_Result _preval_bs_barrier(bs_U32 dependency_flags, bs_U32 src, bs_U32 dst, bs_U32 src_access, bs_U32 dst_access) {
    return next.bs_barrier(dependency_flags, src, dst, src_access, dst_access);
}

static bs_Result _preval_bs_rayTrace(bs_RayTracer* ray_tracer, bs_Pipeline* pipeline, bs_U32 width, bs_U32 height, bs_U32 depth) {
    if (ray_tracer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (ray_tracer->head.source_id != BS_OBJECT_RAY_TRACER)
        return BS_RESULT_VALIDATION_ERROR;

    if (pipeline == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_rayTrace(ray_tracer, pipeline, width, height, depth);
}

static bs_Result _preval_bs_rayTracer(bs_Object* object, bs_U32 flags, bs_Shader* shaders, int shaders_count) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (shaders == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_rayTracer(object, flags, shaders, shaders_count);
}

static bs_Result _preval_bs_accelerateAabb(bs_RayTracer* ray_tracer, bs_Aabb aabb) {
    if (ray_tracer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (ray_tracer->head.source_id != BS_OBJECT_RAY_TRACER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_accelerateAabb(ray_tracer, aabb);
}

static bs_Result _preval_bs_accelerateBatch(bs_RayTracer* ray_tracer, bs_Batch* batch) {
    if (ray_tracer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (ray_tracer->head.source_id != BS_OBJECT_RAY_TRACER)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_accelerateBatch(ray_tracer, batch);
}

static bs_Result _preval_bs_build(bs_RayTracer* ray_tracer) {
    if (ray_tracer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (ray_tracer->head.source_id != BS_OBJECT_RAY_TRACER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_build(ray_tracer);
}

static bs_Result _preval_bs_destroyRayTracer(bs_RayTracer* ray_tracer) {
    if (ray_tracer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (ray_tracer->head.source_id != BS_OBJECT_RAY_TRACER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_destroyRayTracer(ray_tracer);
}

static bs_Result _preval_bs_dispatchAsync(bs_Pipeline* pipeline, bs_U32 x, bs_U32 y, bs_U32 z) {
    if (pipeline == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_dispatchAsync(pipeline, x, y, z);
}

static int _preval_bs_bufferSwaps(bs_Buffer* buffer) {
    if (buffer == NULL)
        return 0;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return 0;

    return next.bs_bufferSwaps(buffer);
}

static bs_Result _preval_bs_nameBuffer(bs_Buffer* buffer, char* value, int value_length) {
    if (buffer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_nameBuffer(buffer, value, value_length);
}

static bs_Result _preval_bs_buffer(bs_Object* object, bs_U32 num_bytes, bs_BufferUsageFlags usage_flags, bs_MemoryPropertyFlags memory_flags, bs_BufferBits flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_buffer(object, num_bytes, usage_flags, memory_flags, flags);
}

static bool _preval_bs_bufferIsMapped(bs_Buffer* buffer) {
    if (buffer == NULL)
        return false;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return false;

    return next.bs_bufferIsMapped(buffer);
}

static char* _preval_bs_bufferMap(bs_Buffer* buffer) {
    if (buffer == NULL)
        return NULL;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return NULL;

    return next.bs_bufferMap(buffer);
}

static bs_Result _preval_bs_mapBuffer(bs_Buffer* buffer, bs_U32 num_bytes) {
    if (buffer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_mapBuffer(buffer, num_bytes);
}

static bs_Result _preval_bs_unmapBuffer(bs_Buffer* buffer) {
    if (buffer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_unmapBuffer(buffer);
}

static void _preval_bs_stageNull(bs_Buffer* buffer) {
    if (buffer == NULL)
        return;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return;

    return next.bs_stageNull(buffer);
}

static void _preval_bs_stageList(bs_Buffer* buffer, bs_List* list) {
    if (buffer == NULL)
        return;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return;

    if (list == NULL)
        return;

    return next.bs_stageList(buffer, list);
}

static void _preval_bs_stageImage(bs_Buffer* buffer, bs_Format format, bs_ivec2 dim, const char* data) {
    if (buffer == NULL)
        return;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return;

    if (data == NULL)
        return;

    return next.bs_stageImage(buffer, format, dim, data);
}

static void _preval_bs_destroyBuffer(bs_Buffer* buffer) {
    if (buffer == NULL)
        return;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return;

    return next.bs_destroyBuffer(buffer);
}

static void _preval_bs_copyAsync(bs_Buffer* src, bs_Buffer* dst, bs_U32 src_offset, bs_U32 dst_offset, bs_U32 num_bytes) {
    if (src == NULL)
        return;

    if (src->head.source_id != BS_OBJECT_BUFFER)
        return;

    if (dst == NULL)
        return;

    if (dst->head.source_id != BS_OBJECT_BUFFER)
        return;

    return next.bs_copyAsync(src, dst, src_offset, dst_offset, num_bytes);
}

static void _preval_bs_setBufferAsync(bs_Buffer* buffer, bs_U32 offset, bs_U32 num_bytes, bs_U32 value) {
    if (buffer == NULL)
        return;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return;

    return next.bs_setBufferAsync(buffer, offset, num_bytes, value);
}

static bs_Result _preval_bs_batch(bs_Object* object, int index_size, bs_Shader* vertex_shader, bs_BatchBits flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (vertex_shader == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(index_size > 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_batch(object, index_size, vertex_shader, flags);
}

static bs_Attribute* _preval_bs_queryAttribute(bs_Batch* batch, char* value, int value_length) {
    if (batch == NULL)
        return NULL;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return NULL;

    if (value == NULL)
        return NULL;

    return next.bs_queryAttribute(batch, value, value_length);
}

static bool _preval_bs_batchIsPushed(bs_Batch* batch) {
    if (batch == NULL)
        return false;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return false;

    return next.bs_batchIsPushed(batch);
}

static bool _preval_bs_batchIsIndexed(bs_Batch* batch) {
    if (batch == NULL)
        return false;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return false;

    return next.bs_batchIsIndexed(batch);
}

static bs_Result _preval_bs_minimizeBatch(bs_Batch* batch) {
    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_minimizeBatch(batch);
}

static bs_Result _preval_bs_pushBatch(bs_Batch* batch, bs_U32 num_index_bytes, bs_U32 num_vertex_bytes) {
    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_pushBatch(batch, num_index_bytes, num_vertex_bytes);
}

static bs_Result _preval_bs_unpushBatch(bs_Batch* batch) {
    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_unpushBatch(batch);
}

static bs_Result _preval_bs_destroyBatch(bs_Batch* batch) {
    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_destroyBatch(batch);
}

static bs_Result _preval_bs_recreateBatch(bs_Batch* batch) {
    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_recreateBatch(batch);
}

static bs_Result _preval_bs_ensureBatchSize(bs_Batch* batch, bs_U32 num_indices, bs_U32 num_vertices) {
    if (batch == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_ensureBatchSize(batch, num_indices, num_vertices);
}

static void _preval_bs_batchVertex(bs_VertexDeclaration* declaration, const unsigned char* src) {
    if (declaration == NULL)
        return;

    if (src == NULL)
        return;

    return next.bs_batchVertex(declaration, src);
}

static bs_Range _preval_bs_batchRange(bs_Batch* batch, bs_U32 offset) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_batchRange(batch, offset);
}

static void _preval_bs_pushIndex(bs_Batch* batch, int index) {
    if (batch == NULL)
        return;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return;

    return next.bs_pushIndex(batch, index);
}

static void _preval_bs_pushIndices(bs_Batch* batch, int indices, bs_U32 indices_count) {
    if (batch == NULL)
        return;

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return;

    return next.bs_pushIndices(batch, indices, indices_count);
}

static bs_Range _preval_bs_batchCube(bs_Batch* batch, bs_U32* offset, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchCube(batch, offset, color);
}

static bs_Range _preval_bs_pushCube(bs_Batch* batch, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushCube(batch, color);
}

static bs_Range _preval_bs_batchCone(bs_Batch* batch, bs_U32* offset, int segments, float height, float radius, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchCone(batch, offset, segments, height, radius, color);
}

static bs_Range _preval_bs_pushCone(bs_Batch* batch, int segments, float height, float radius, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushCone(batch, segments, height, radius, color);
}

static bs_Range _preval_bs_batchRectangle(bs_Batch* batch, bs_U32* offset, bs_vec3 position, bs_vec2 dimensions, bs_vec2 texture_offset, bs_vec2 texture_coords, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchRectangle(batch, offset, position, dimensions, texture_offset, texture_coords, color);
}

static bs_Range _preval_bs_pushRectangle(bs_Batch* batch, bs_vec3 position, bs_vec2 dimensions, bs_vec2 texture_offset, bs_vec2 texture_coords, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushRectangle(batch, position, dimensions, texture_offset, texture_coords, color);
}

static bs_Range _preval_bs_batchQuad(bs_Batch* batch, bs_U32* offset, bs_Quad quad, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchQuad(batch, offset, quad, color);
}

static bs_Range _preval_bs_pushQuad(bs_Batch* batch, bs_Quad quad, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushQuad(batch, quad, color);
}

static bs_Range _preval_bs_batchTriangle(bs_Batch* batch, bs_U32* offset, bs_vec3 a, bs_vec3 b, bs_vec3 c, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchTriangle(batch, offset, a, b, c, color);
}

static bs_Range _preval_bs_pushTriangle(bs_Batch* batch, bs_vec3 a, bs_vec3 b, bs_vec3 c, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushTriangle(batch, a, b, c, color);
}

static bs_Range _preval_bs_batchLine(bs_Batch* batch, bs_U32* offset, bs_vec3 start, bs_vec3 end, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchLine(batch, offset, start, end, color);
}

static bs_Range _preval_bs_pushLine(bs_Batch* batch, bs_vec3 start, bs_vec3 end, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushLine(batch, start, end, color);
}

static bs_Range _preval_bs_batchRay(bs_Batch* batch, bs_U32* offset, bs_Ray* ray, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    if (ray == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchRay(batch, offset, ray, color);
}

static bs_Range _preval_bs_pushRay(bs_Batch* batch, bs_Ray* ray, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (ray == NULL)
        return (bs_Range) { 0 };

    return next.bs_pushRay(batch, ray, color);
}

static bs_Range _preval_bs_batchPoint(bs_Batch* batch, bs_U32* offset, bs_vec3 position, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchPoint(batch, offset, position, color);
}

static bs_Range _preval_bs_pushPoint(bs_Batch* batch, bs_vec3 position, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushPoint(batch, position, color);
}

static bs_Range _preval_bs_batchAabb(bs_Batch* batch, bs_U32* offset, bs_Aabb* aabb, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    if (aabb == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchAabb(batch, offset, aabb, color);
}

static bs_Range _preval_bs_pushAabb(bs_Batch* batch, bs_Aabb* aabb, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (aabb == NULL)
        return (bs_Range) { 0 };

    return next.bs_pushAabb(batch, aabb, color);
}

static bs_Range _preval_bs_batchSphere(bs_Batch* batch, bs_U32* offset, bs_vec3 position, float radius, bs_U32 lats, bs_U32 longs, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchSphere(batch, offset, position, radius, lats, longs, color);
}

static bs_Range _preval_bs_pushSphere(bs_Batch* batch, bs_vec3 position, float radius, bs_U32 lats, bs_U32 longs, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushSphere(batch, position, radius, lats, longs, color);
}

static bs_Range _preval_bs_batchPyramid(bs_Batch* batch, bs_U32* offset, bs_vec3 pos, float width, float height, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchPyramid(batch, offset, pos, width, height, color);
}

static bs_Range _preval_bs_pushPyramid(bs_Batch* batch, bs_vec3 pos, float width, float height, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushPyramid(batch, pos, width, height, color);
}

static bs_Range _preval_bs_batchBipyramid(bs_Batch* batch, bs_U32* offset, bs_vec3 pos, float width, float height, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchBipyramid(batch, offset, pos, width, height, color);
}

static bs_Range _preval_bs_pushBipyramid(bs_Batch* batch, bs_vec3 pos, float width, float height, bs_RGBA color) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    return next.bs_pushBipyramid(batch, pos, width, height, color);
}

static bs_Range _preval_bs_batchPrimitive(bs_Batch* batch, bs_U32* offset, bs_Primitive* primitive) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    if (primitive == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchPrimitive(batch, offset, primitive);
}

static bs_Range _preval_bs_pushPrimitive(bs_Batch* batch, bs_Primitive* primitive) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (primitive == NULL)
        return (bs_Range) { 0 };

    return next.bs_pushPrimitive(batch, primitive);
}

static bs_Range _preval_bs_batchMesh(bs_Batch* batch, bs_U32* offset, bs_Mesh* mesh) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    if (mesh == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchMesh(batch, offset, mesh);
}

static bs_Range _preval_bs_pushMesh(bs_Batch* batch, bs_Mesh* mesh) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (mesh == NULL)
        return (bs_Range) { 0 };

    return next.bs_pushMesh(batch, mesh);
}

static bs_Range _preval_bs_batchModel(bs_Batch* batch, bs_U32* offset, bs_Model* model) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (offset == NULL)
        return (bs_Range) { 0 };

    if (model == NULL)
        return (bs_Range) { 0 };

    return next.bs_batchModel(batch, offset, model);
}

static bs_Range _preval_bs_pushModel(bs_Batch* batch, bs_Model* model) {
    if (batch == NULL)
        return (bs_Range) { 0 };

    if (batch->head.source_id != BS_OBJECT_BATCH)
        return (bs_Range) { 0 };

    if (model == NULL)
        return (bs_Range) { 0 };

    return next.bs_pushModel(batch, model);
}

static int _preval_bs_rendererSwapsCount(bs_Renderer* renderer) {
    if (renderer == NULL)
        return 0;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return 0;

    return next.bs_rendererSwapsCount(renderer);
}

static bs_Result _preval_bs_renderer(bs_Object* object, bs_RendererBits flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_renderer(object, flags);
}

static void _preval_bs_output(bs_Renderer* renderer, bs_Output output) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_output(renderer, output);
}

static void _preval_bs_input(bs_Renderer* renderer, bs_Input input) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_input(renderer, input);
}

static void _preval_bs_dependency(bs_Renderer* renderer, bs_U32 src_subpass, bs_U32 dst_subpass, bs_DependencyFlags flags, bs_PipelineStage src_stage, bs_PipelineStage dst_stage, bs_AccessMask src_access, bs_AccessMask dst_access) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_dependency(renderer, src_subpass, dst_subpass, flags, src_stage, dst_stage, src_access, dst_access);
}

static void _preval_bs_renderPass(bs_Renderer* renderer) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_renderPass(renderer);
}

static bs_Result _preval_bs_framebuffer(bs_Renderer* renderer, bs_ivec2 resolution) {
    if (renderer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_framebuffer(renderer, resolution);
}

static void _preval_bs_runPass(bs_Renderer* renderer, bs_Callback callbacks, int callbacks_count) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_runPass(renderer, callbacks, callbacks_count);
}

static bool _preval_bs_rendererIsDynamic(bs_Renderer* renderer) {
    if (renderer == NULL)
        return false;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return false;

    return next.bs_rendererIsDynamic(renderer);
}

static void _preval_bs_beginRender(bs_Renderer* renderer) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_beginRender(renderer);
}

static void _preval_bs_endRender(bs_Renderer* renderer) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_endRender(renderer);
}

static void _preval_bs_destroyRenderer(bs_Renderer* renderer) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    return next.bs_destroyRenderer(renderer);
}

static void _preval_bs_resizeRenderer(bs_Renderer* renderer, bs_ivec2 resolution) {
    if (renderer == NULL)
        return;

    if (renderer->head.source_id != BS_OBJECT_RENDERER)
        return;

    if (!(resolution.y > 0))
        return;

    if (!(resolution.x > 0))
        return;

    return next.bs_resizeRenderer(renderer, resolution);
}

static bs_Queue* _preval_bs_singleTimesQueue() {
    return next.bs_singleTimesQueue();
}

static bs_I32 _preval_bs_queueFamily(bs_QueueBits flags) {
    return next.bs_queueFamily(flags);
}

static bs_Result _preval_bs_present(bs_Queue* queue, bs_Queue* wait_queues, int wait_queues_count) {
    if (queue == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return BS_RESULT_VALIDATION_ERROR;

    if (wait_queues == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (wait_queues->head.source_id != BS_OBJECT_QUEUE)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_present(queue, wait_queues, wait_queues_count);
}

static bs_Result _preval_bs_acquire() {
    return next.bs_acquire();
}

static int _preval_bs_queueSwap(bs_Queue* queue) {
    if (queue == NULL)
        return 0;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return 0;

    return next.bs_queueSwap(queue);
}

static void _preval_bs_awaitQueue(bs_Queue* queue, bs_PipelineStage stage) {
    if (queue == NULL)
        return;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return;

    return next.bs_awaitQueue(queue, stage);
}

static void _preval_bs_awaitAcquisition() {
    return next.bs_awaitAcquisition();
}

static bs_Result _preval_bs_enqueue(bs_Queue* queue, bs_Callback function) {
    if (queue == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_enqueue(queue, function);
}

static int _preval_bs_imageIndex() {
    return next.bs_imageIndex();
}

static int _preval_bs_queueSwapsCount(bs_Queue* queue) {
    if (queue == NULL)
        return 0;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return 0;

    return next.bs_queueSwapsCount(queue);
}

static bs_Result _preval_bs_queue(bs_Object* object, bs_QueueBits flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_queue(object, flags);
}

static void _preval_bs_destroyQueue(bs_Queue* queue) {
    if (queue == NULL)
        return;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return;

    return next.bs_destroyQueue(queue);
}

static void _preval_bs_stallGPU() {
    return next.bs_stallGPU();
}

static void _preval_bs_stallQueue(bs_Queue* queue) {
    if (queue == NULL)
        return;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return;

    return next.bs_stallQueue(queue);
}

static bs_Result _preval_bs_stall(bs_Queue* queue) {
    if (queue == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_stall(queue);
}

static bool _preval_bs_poll(bs_Queue* queue) {
    if (queue == NULL)
        return false;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return false;

    return next.bs_poll(queue);
}

static bs_Scope _preval_bs_enterSingle() {
    return next.bs_enterSingle();
}

static void _preval_bs_leaveSingle(bs_Scope* backup) {
    if (backup == NULL)
        return;

    return next.bs_leaveSingle(backup);
}

static bs_Scope* _preval_bs_getScope() {
    return next.bs_getScope();
}

static void _preval_bs_setScope(bs_Scope* scope) {
    if (scope == NULL)
        return;

    return next.bs_setScope(scope);
}

static void _preval_bs_runSingle(bs_Callback function) {
    return next.bs_runSingle(function);
}

static void _preval_bs_glyph(bs_TTF* ttf, bs_U16 code) {
    if (ttf == NULL)
        return;

    return next.bs_glyph(ttf, code);
}

static void _preval_bs_ttf(bs_TTF* existing, const char* path, bs_U32 flags) {
    if (existing == NULL)
        return;

    if (path == NULL)
        return;

    return next.bs_ttf(existing, path, flags);
}

static void _preval_bs_rasterizeGlyph(bs_TTF* font, bs_Glyph* glyph, int width, int height, char* out_bmp, float scale) {
    if (font == NULL)
        return;

    if (glyph == NULL)
        return;

    if (out_bmp == NULL)
        return;

    if (!(width > 0))
        return;

    if (!(height > 0))
        return;

    if (!(scale > 0.0f))
        return;

    return next.bs_rasterizeGlyph(font, glyph, width, height, out_bmp, scale);
}

static void _preval_bs_kern(bs_TTF* ttf) {
    if (ttf == NULL)
        return;

    return next.bs_kern(ttf);
}

static void _preval_bs_bindFont(bs_Font* font, bs_Sampler* sampler, int bind_set, int bind_point) {
    if (font == NULL)
        return;

    if (font->head.source_id != BS_OBJECT_FONT)
        return;

    if (sampler == NULL)
        return;

    if (sampler->head.source_id != BS_OBJECT_SAMPLER)
        return;

    if (!(bind_set >= 0))
        return;

    return next.bs_bindFont(font, sampler, bind_set, bind_point);
}

static bs_vec2 _preval_bs_textDimensions(bs_Font* font, char* name, int length) {
    if (font == NULL)
        return (bs_vec2) { 0 };

    if (font->head.source_id != BS_OBJECT_FONT)
        return (bs_vec2) { 0 };

    if (name == NULL)
        return (bs_vec2) { 0 };

    return next.bs_textDimensions(font, name, length);
}

static void _preval_bs_destroyFont(bs_Font* font) {
    if (font == NULL)
        return;

    if (font->head.source_id != BS_OBJECT_FONT)
        return;

    return next.bs_destroyFont(font);
}

static bs_Result _preval_bs_loadFont(bs_Object* object, int package_id, const char* resource_name, const char* alphabet, float spacing, bs_U32 flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (resource_name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (alphabet == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadFont(object, package_id, resource_name, alphabet, spacing, flags);
}

static bs_Result _preval_bs_image(bs_Object* object, bs_ivec2 dim, int num_indices, bs_Format format, bs_U32 flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(dim.y > 0))
        return BS_RESULT_VALIDATION_ERROR;

    if (!(dim.x > 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_image(object, dim, num_indices, format, flags);
}

static int _preval_bs_imageSwapsCount(bs_Image* image) {
    if (image == NULL)
        return 0;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return 0;

    return next.bs_imageSwapsCount(image);
}

static void _preval_bs_transition(bs_Image* image, int index, bs_ImageLayout old_layout, bs_ImageLayout new_layout) {
    if (image == NULL)
        return;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return;

    if (!(index >= 0))
        return;

    return next.bs_transition(image, index, old_layout, new_layout);
}

static unsigned char* _preval_bs_inspectPng(const char* path, bs_PngData* out_png_data) {
    if (path == NULL)
        return NULL;

    if (out_png_data == NULL)
        return NULL;

    return next.bs_inspectPng(path, out_png_data);
}

static unsigned char* _preval_bs_loadPngData(char* data, size_t size, int channels_count, bs_PngData* out_png_data) {
    if (data == NULL)
        return NULL;

    if (out_png_data == NULL)
        return NULL;

    return next.bs_loadPngData(data, size, channels_count, out_png_data);
}

static unsigned char* _preval_bs_loadPng(const char* path, int channels_count, bs_PngData* out_png_data) {
    if (path == NULL)
        return NULL;

    if (out_png_data == NULL)
        return NULL;

    return next.bs_loadPng(path, channels_count, out_png_data);
}

static bs_Result _preval_bs_bitmapImage(bs_Object* existing_object, unsigned char* image_data, bs_ivec2 dim, bs_Format format, bs_ImageBits flags) {
    if (existing_object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image_data == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bitmapImage(existing_object, image_data, dim, format, flags);
}

static bs_Result _preval_bs_savePng(char* data, bs_ivec2 resolution, bs_PngType type, char* value, int value_length) {
    if (data == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_savePng(data, resolution, type, value, value_length);
}

static unsigned char* _preval_bs_encodePng(size_t* out_size, const unsigned char* data, bs_ivec2 size, bs_PngType type, char* value, int value_length) {
    if (out_size == NULL)
        return NULL;

    if (data == NULL)
        return NULL;

    if (value == NULL)
        return NULL;

    return next.bs_encodePng(out_size, data, size, type, value, value_length);
}

static void _preval_bs_destroyImage(bs_Image* image) {
    if (image == NULL)
        return;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return;

    return next.bs_destroyImage(image);
}

static bs_Result _preval_bs_resizeImage(bs_Image* image, bs_ivec2 size, int indices_count) {
    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_resizeImage(image, size, indices_count);
}

static bs_Result _preval_bs_queryImageIndexHash(bs_Image* image, bs_U64 name_hash, int* out) {
    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_queryImageIndexHash(image, name_hash, out);
}

static bs_Result _preval_bs_queryImageIndex(bs_Image* image, char* name, int* out) {
    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_queryImageIndex(image, name, out);
}

static bs_Result _preval_bs_copyImageToBufferAsync(bs_Image* image, bs_Buffer* buffer, int image_index, bs_ImageLayout layout, bs_U64 buffer_offset, bs_ivec2 offset, bs_ivec2 resolution) {
    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(image_index >= 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_copyImageToBufferAsync(image, buffer, image_index, layout, buffer_offset, offset, resolution);
}

static bs_Result _preval_bs_copyBufferToImage(bs_Buffer* buffer, bs_Image* image, int index, bs_ImageLayout layout) {
    if (buffer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return BS_RESULT_VALIDATION_ERROR;

    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(index >= 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_copyBufferToImage(buffer, image, index, layout);
}

static bs_Result _preval_bs_blit(bs_BlitOperation operation) {
    return next.bs_blit(operation);
}

static bs_Result _preval_bs_loadImage(bs_Object* object, int package_id, bs_ImageBits flags, char* value, int value_length) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadImage(object, package_id, flags, value, value_length);
}

static bool _preval_bs_isStencilFormat(bs_Format format) {
    return next.bs_isStencilFormat(format);
}

static bool _preval_bs_isDepthFormat(bs_Format format) {
    return next.bs_isDepthFormat(format);
}

static bool _preval_bs_hasAlpha(bs_Format format) {
    return next.bs_hasAlpha(format);
}

static bs_Result _preval_bs_nameImage(bs_Image* image, const char* name) {
    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_nameImage(image, name);
}

static bs_Result _preval_bs_destroySampler(bs_Sampler* sampler) {
    if (sampler == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (sampler->head.source_id != BS_OBJECT_SAMPLER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_destroySampler(sampler);
}

static bs_Result _preval_bs_sampler(bs_Object* object, bs_ImageFilter filter, bs_SamplerBits flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_sampler(object, filter, flags);
}

static bs_Result _preval_bs_loadAtlas(bs_Object* object, int package_id, bs_U32 flags, char* value, int value_length) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadAtlas(object, package_id, flags, value, value_length);
}

static bs_vec4 _preval_bs_atlasCoordinates(bs_Atlas* atlas, int texture_id) {
    if (atlas == NULL)
        return (bs_vec4) { 0 };

    if (atlas->head.source_id != BS_OBJECT_ATLAS)
        return (bs_vec4) { 0 };

    return next.bs_atlasCoordinates(atlas, texture_id);
}

static bs_vec4 _preval_bs_mirrorUV(bs_vec4 uv) {
    return next.bs_mirrorUV(uv);
}

static bs_vec4 _preval_bs_flipUV(bs_vec4 uv) {
    return next.bs_flipUV(uv);
}

static bs_vec2 _preval_bs_atlasSize(bs_Atlas* atlas, int texture) {
    if (atlas == NULL)
        return (bs_vec2) { 0 };

    if (atlas->head.source_id != BS_OBJECT_ATLAS)
        return (bs_vec2) { 0 };

    return next.bs_atlasSize(atlas, texture);
}

static int _preval_bs_queryAtlasHash(bs_Atlas* atlas, bs_U64 hash) {
    if (atlas == NULL)
        return 0;

    if (atlas->head.source_id != BS_OBJECT_ATLAS)
        return 0;

    return next.bs_queryAtlasHash(atlas, hash);
}

static int _preval_bs_queryAtlas(bs_Atlas* atlas, const char* name) {
    if (atlas == NULL)
        return 0;

    if (atlas->head.source_id != BS_OBJECT_ATLAS)
        return 0;

    if (name == NULL)
        return 0;

    return next.bs_queryAtlas(atlas, name);
}

static bs_Result _preval_bs_destroyAtlas(bs_Atlas* atlas) {
    if (atlas == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (atlas->head.source_id != BS_OBJECT_ATLAS)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_destroyAtlas(atlas);
}

static bs_Result _preval_bs_loadAtlasMemory(bs_Object* object, int package_id, char* resource_name, char* data, bs_U32 flags) {
    if (object == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (resource_name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (data == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadAtlasMemory(object, package_id, resource_name, data, flags);
}

static void _preval_bs_parseArgs(int argc, char* argv) {
    if (argv == NULL)
        return;

    return next.bs_parseArgs(argc, argv);
}

static bs_Args* _preval_bs_arguments() {
    return next.bs_arguments();
}

static void _preval_bs_ini() {
    return next.bs_ini();
}

static void _preval_bs_load(bs_Callback load_resources) {
    return next.bs_load(load_resources);
}

static void _preval_bs_queryProcedures(bs_Procedure* procedures, int count, void* dll_handle, unsigned char* destination) {
    if (procedures == NULL)
        return;

    if (dll_handle == NULL)
        return;

    if (destination == NULL)
        return;

    return next.bs_queryProcedures(procedures, count, dll_handle, destination);
}

static struct VkCommandBuffer_T* _preval_bsi_fetchCommands() {
    return next.bsi_fetchCommands();
}

static struct VkDevice_T* _preval_bsi_fetchDevice() {
    return next.bsi_fetchDevice();
}

static void _preval_bsi_resizeObjects() {
    return next.bsi_resizeObjects();
}

static bs_Result _preval_bs_resetQueue(bs_Queue* queue) {
    if (queue == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_resetQueue(queue);
}

static bs_Result _preval_bs_pushQueue(bs_Queue* queue) {
    if (queue == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (queue->head.source_id != BS_OBJECT_QUEUE)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_pushQueue(queue);
}

static bs_Result _preval_bsi_nameHandle(bs_U64 handle, bs_U32 type, char* value, int value_length) {
    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(handle != 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bsi_nameHandle(handle, type, value, value_length);
}

static bs_JsonEnumeration _preval_bs_beginEnumeration(bs_Json* json) {
    if (json == NULL)
        return (bs_JsonEnumeration) { 0 };

    return next.bs_beginEnumeration(json);
}

static void _preval_bs_enumerateJson(bs_Json* json, bs_JsonEnumeration* e) {
    if (json == NULL)
        return;

    if (e == NULL)
        return;

    return next.bs_enumerateJson(json, e);
}

static bs_Json _preval_bs_jsonRoot(bs_Json* json, bs_JsonObject object) {
    if (json == NULL)
        return (bs_Json) { 0 };

    return next.bs_jsonRoot(json, object);
}

static void _preval_bs_ensureJsonMutable(bs_Json* root) {
    if (root == NULL)
        return;

    return next.bs_ensureJsonMutable(root);
}

static bs_Json _preval_bs_jsonCopy(const bs_Json* root) {
    if (root == NULL)
        return (bs_Json) { 0 };

    return next.bs_jsonCopy(root);
}

static bs_Result _preval_bs_saveJson(bs_Json* json, bs_SaveJsonBits flags, char** out) {
    if (json == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_saveJson(json, flags, out);
}

static bs_Json _preval_bs_emptyJson() {
    return next.bs_emptyJson();
}

static bs_Json _preval_bs_emptyJsonArray() {
    return next.bs_emptyJsonArray();
}

static bs_Result _preval_bs_json(char* raw, int len, bs_Json* out_json) {
    if (raw == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out_json == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(len >= 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_json(raw, len, out_json);
}

static bs_Result _preval_bs_loadJson(bs_Json* out_json, char* path, int path_length) {
    if (out_json == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadJson(out_json, path, path_length);
}

static void _preval_bs_destroyJson(bs_Json* json) {
    if (json == NULL)
        return;

    return next.bs_destroyJson(json);
}

static bs_JsonValue _preval_bs_parseJsonValue(char* raw) {
    if (raw == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_parseJsonValue(raw);
}

static bs_JsonValue _preval_bs_fetchJson(bs_Json* root, bs_JsonType expect, char* path, int path_length) {
    if (root == NULL)
        return (bs_JsonValue) { 0 };

    if (path == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_fetchJson(root, expect, path, path_length);
}

static void _preval_bs_deleteJson(bs_Json* root, char* path, int path_length) {
    if (root == NULL)
        return;

    if (path == NULL)
        return;

    return next.bs_deleteJson(root, path, path_length);
}

static bs_Result _preval_bs_ensureJson(bs_Json* root, bs_JsonValue value, char* path, int path_length) {
    if (root == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_ensureJson(root, value, path, path_length);
}

static bs_JsonValue _preval_bs_jsonValueFromObject(bs_JsonObject x) {
    return next.bs_jsonValueFromObject(x);
}

static bs_JsonValue _preval_bs_jsonValueFromRoot(bs_Json x) {
    return next.bs_jsonValueFromRoot(x);
}

static bs_JsonValue _preval_bs_jsonValueFromBool(bool x) {
    return next.bs_jsonValueFromBool(x);
}

static bs_JsonValue _preval_bs_jsonValueFromInteger(int x) {
    return next.bs_jsonValueFromInteger(x);
}

static bs_JsonValue _preval_bs_jsonValueFromFloat(double x) {
    return next.bs_jsonValueFromFloat(x);
}

static bs_JsonValue _preval_bs_jsonValueFromString(char* x) {
    if (x == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_jsonValueFromString(x);
}

static bs_JsonValue _preval_bs_jsonValueFromStringPointer(char* x) {
    if (x == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_jsonValueFromStringPointer(x);
}

static bs_JsonValue _preval_bs_jsonValueFromDateTime(bs_DateTime x) {
    return next.bs_jsonValueFromDateTime(x);
}

static bs_JsonValue _preval_bs_jsonObject(bs_Json* json) {
    if (json == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_jsonObject(json);
}

static bs_JsonValue _preval_bs_jsonArray(bs_JsonType type, char* data, int count) {
    if (data == NULL)
        return (bs_JsonValue) { 0 };

    if (!(count >= 0))
        return (bs_JsonValue) { 0 };

    return next.bs_jsonArray(type, data, count);
}

static bs_JsonValue _preval_bs_jsonVec2(bs_vec2* vector) {
    if (vector == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_jsonVec2(vector);
}

static bs_JsonValue _preval_bs_jsonVec3(bs_vec3* vector) {
    if (vector == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_jsonVec3(vector);
}

static bs_JsonValue _preval_bs_jsonVec4(bs_vec4* vector) {
    if (vector == NULL)
        return (bs_JsonValue) { 0 };

    return next.bs_jsonVec4(vector);
}

static bs_JsonValue _preval_bs_jsonRGBA(bs_RGBA color) {
    return next.bs_jsonRGBA(color);
}

static char* _preval_bs_logSection(char* value, int value_length) {
    if (value == NULL)
        return NULL;

    return next.bs_logSection(value, value_length);
}

static char* _preval_bs_logEndOfSection() {
    return next.bs_logEndOfSection();
}

static char* _preval_bs_logWithTimestamp(const char* type, int type_len, char* value, int value_length) {
    if (type == NULL)
        return NULL;

    if (value == NULL)
        return NULL;

    if (!(type_len >= 0))
        return NULL;

    return next.bs_logWithTimestamp(type, type_len, value, value_length);
}

static char* _preval_bs_log(char* value, int value_length) {
    if (value == NULL)
        return NULL;

    return next.bs_log(value, value_length);
}

static char* _preval_bs_info(char* value, int value_length) {
    if (value == NULL)
        return NULL;

    return next.bs_info(value, value_length);
}

static char* _preval_bs_warn(char* value, int value_length) {
    if (value == NULL)
        return NULL;

    return next.bs_warn(value, value_length);
}

static void _preval_bs_critical(char* value, int value_length) {
    if (value == NULL)
        return;

    return next.bs_critical(value, value_length);
}

static void _preval_bs_logObjectDiff() {
    return next.bs_logObjectDiff();
}

static void _preval_bs_logUnchangedObjects() {
    return next.bs_logUnchangedObjects();
}

static void _preval_bs_logBindings() {
    return next.bs_logBindings();
}

static void _preval_bs_infoF4(bs_mat4* m) {
    if (m == NULL)
        return;

    return next.bs_infoF4(m);
}

static bs_Instance* _preval_bs_instance() {
    return next.bs_instance();
}

static bs_Args* _preval_bs_args() {
    return next.bs_args();
}

static bs_Features* _preval_bs_features() {
    return next.bs_features();
}

static bs_Props* _preval_bs_props() {
    return next.bs_props();
}

static bs_Config* _preval_bs_config() {
    return next.bs_config();
}

static bs_Scope* _preval_bs_scope() {
    return next.bs_scope();
}

static void _preval_bs_system(char* command, char* value, int value_length) {
    if (command == NULL)
        return;

    if (value == NULL)
        return;

    return next.bs_system(command, value, value_length);
}

static void _preval_bs_createThread(bs_ThreadFunction function, void* param) {
    if (param == NULL)
        return;

    return next.bs_createThread(function, param);
}

static int _preval_bs_formatStringLength(const char* format, va_list args) {
    if (format == NULL)
        return 0;

    return next.bs_formatStringLength(format, args);
}

static const char* _preval_bs_checkStringPool(bs_List* pool, char* string) {
    if (pool == NULL)
        return NULL;

    if (string == NULL)
        return NULL;

    return next.bs_checkStringPool(pool, string);
}

static bs_String* _preval_bs_stringAlloc(bs_String* old, int len) {
    if (old == NULL)
        return NULL;

    if (!(len >= 0))
        return NULL;

    return next.bs_stringAlloc(old, len);
}

static bs_String* _preval_bs_emptyString(bs_String* old) {
    if (old == NULL)
        return NULL;

    return next.bs_emptyString(old);
}

static bs_String* _preval_bs_string(bs_String* old, char* value, int value_length) {
    if (old == NULL)
        return NULL;

    if (value == NULL)
        return NULL;

    return next.bs_string(old, value, value_length);
}

static bs_Result _preval_bs_toUpper(char* string, int len) {
    if (string == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(len >= 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_toUpper(string, len);
}

static bs_Result _preval_bs_toLower(char* string, int len) {
    if (string == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (!(len >= 0))
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_toLower(string, len);
}

static bs_U64 _preval_bs_hash(unsigned char* data, size_t size) {
    if (data == NULL)
        return 0;

    if (!(size > 0))
        return 0;

    return next.bs_hash(data, size);
}

static bs_U64 _preval_bs_stringHash(char* string) {
    if (string == NULL)
        return 0;

    return next.bs_stringHash(string);
}

static bool _preval_bs_startsWith(char* string, const char* prefix) {
    if (string == NULL)
        return false;

    if (prefix == NULL)
        return false;

    return next.bs_startsWith(string, prefix);
}

static bool _preval_bs_endsWith(char* string, const char* suffix) {
    if (string == NULL)
        return false;

    if (suffix == NULL)
        return false;

    return next.bs_endsWith(string, suffix);
}

static char _preval_bs_lastChar(char* string, int len) {
    if (string == NULL)
        return 0;

    if (!(len >= 0))
        return 0;

    return next.bs_lastChar(string, len);
}

static bool _preval_bs_stringContainsChar(char* string, char c) {
    if (string == NULL)
        return false;

    return next.bs_stringContainsChar(string, c);
}

static bs_String* _preval_bs_workingDirectory() {
    return next.bs_workingDirectory();
}

static void _preval_bs_setWorkingDirectory(char* path, int path_length) {
    if (path == NULL)
        return;

    return next.bs_setWorkingDirectory(path, path_length);
}

static bs_String* _preval_bs_executablePath() {
    return next.bs_executablePath();
}

static char* _preval_bs_appdataPath() {
    return next.bs_appdataPath();
}

static void _preval_bs_shortenString(bs_String* string, int len) {
    if (string == NULL)
        return;

    if (!(len >= 0))
        return;

    return next.bs_shortenString(string, len);
}

static bs_String* _preval_bs_appendChar(bs_String* string, char c) {
    if (string == NULL)
        return NULL;

    return next.bs_appendChar(string, c);
}

static void _preval_bs_removeLastCharsCount(bs_String* string, int n) {
    if (string == NULL)
        return;

    if (!(n <= string->len))
        return;

    if (!(n >= 0))
        return;

    return next.bs_removeLastCharsCount(string, n);
}

static void _preval_bs_removeCharRange(bs_String* string, int start, int count) {
    if (string == NULL)
        return;

    if (!(start >= 0))
        return;

    return next.bs_removeCharRange(string, start, count);
}

static bs_String* _preval_bs_insertChar(bs_String* string, int index, char c) {
    if (string == NULL)
        return NULL;

    return next.bs_insertChar(string, index, c);
}

static bs_String* _preval_bs_appendPath(bs_String* string, char* path, int path_len) {
    if (string == NULL)
        return NULL;

    if (path == NULL)
        return NULL;

    return next.bs_appendPath(string, path, path_len);
}

static void _preval_bs_replaceCharOccurrences(char* string, int string_len, char a, char b) {
    if (string == NULL)
        return;

    return next.bs_replaceCharOccurrences(string, string_len, a, b);
}

static char* _preval_bs_strndup(const char* s, size_t n) {
    if (s == NULL)
        return NULL;

    return next.bs_strndup(s, n);
}

static size_t _preval_bs_strnlen(const char* src, size_t n) {
    if (src == NULL)
        return 0;

    return next.bs_strnlen(src, n);
}

static char* _preval_bs_strsep(char** stringp, const char* delim) {
    if (stringp == NULL)
        return NULL;

    if (delim == NULL)
        return NULL;

    return next.bs_strsep(stringp, delim);
}

static void* _preval_bs_memmem(const void* haystack, bs_U32 haystack_len, const void* const needle, const bs_U32 needle_len) {
    if (haystack == NULL)
        return NULL;

    if (needle == NULL)
        return NULL;

    return next.bs_memmem(haystack, haystack_len, needle, needle_len);
}

static bs_U32 _preval_bs_alignUp(bs_U32 value, bs_U32 alignment) {
    return next.bs_alignUp(value, alignment);
}

static bs_Result _preval_bs_widen(char* src, wchar_t* dst, bs_U32 dst_size) {
    if (src == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (dst == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_widen(src, dst, dst_size);
}

static bs_Result _preval_bs_unwiden(wchar_t* src, char* dst, bs_U32 dst_size) {
    if (src == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (dst == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_unwiden(src, dst, dst_size);
}

static char* _preval_bs_charString(char* value, int value_length) {
    if (value == NULL)
        return NULL;

    return next.bs_charString(value, value_length);
}

static void* _preval_bs_free(void* p) {
    if (p == NULL)
        return NULL;

    return next.bs_free(p);
}

static void* _preval_bs_malloc(bs_U64 size) {
    return next.bs_malloc(size);
}

static void* _preval_bs_calloc(bs_U64 num_units, bs_U64 unit_size) {
    return next.bs_calloc(num_units, unit_size);
}

static void* _preval_bs_realloc(void* p, bs_U64 size) {
    if (p == NULL)
        return NULL;

    return next.bs_realloc(p, size);
}

static bool _preval_bs_listContains(bs_List* list, void* data) {
    if (list == NULL)
        return false;

    if (data == NULL)
        return false;

    return next.bs_listContains(list, data);
}

static void* _preval_bs_fetchUnit(bs_List* list, bs_U32 index) {
    if (list == NULL)
        return NULL;

    return next.bs_fetchUnit(list, index);
}

static void* _preval_bs_fetchLast(bs_List* list) {
    if (list == NULL)
        return NULL;

    return next.bs_fetchLast(list);
}

static void* _preval_bs_fetchLastNull(bs_List* list) {
    if (list == NULL)
        return NULL;

    return next.bs_fetchLastNull(list);
}

static void _preval_bs_ensureSize(bs_List* list, bs_U32 num_units) {
    if (list == NULL)
        return;

    return next.bs_ensureSize(list, num_units);
}

static void _preval_bs_erase(bs_List* list, int index, bs_U32 count) {
    if (list == NULL)
        return;

    return next.bs_erase(list, index, count);
}

static void* _preval_bs_pushBack(bs_List* list, char* data) {
    if (list == NULL)
        return NULL;

    if (data == NULL)
        return NULL;

    return next.bs_pushBack(list, data);
}

static void* _preval_bs_pushBackList(bs_List* source, bs_List* destination) {
    if (source == NULL)
        return NULL;

    if (destination == NULL)
        return NULL;

    return next.bs_pushBackList(source, destination);
}

static void _preval_bs_destroyList(bs_List* list) {
    if (list == NULL)
        return;

    return next.bs_destroyList(list);
}

static void _preval_bs_seekList(bs_List* list, bs_U32 unit_index) {
    if (list == NULL)
        return;

    return next.bs_seekList(list, unit_index);
}

static void _preval_bs_minimizeList(bs_List* list) {
    if (list == NULL)
        return;

    return next.bs_minimizeList(list);
}

static bs_List _preval_bs_list(int unit_size, int increment) {
    return next.bs_list(unit_size, increment);
}

static void _preval_bs_guidToString(bs_GUID* guid, char out) {
    if (guid == NULL)
        return;

    return next.bs_guidToString(guid, out);
}

static bs_GUID _preval_bs_stringToGuid(const char* str) {
    if (str == NULL)
        return (bs_GUID) { 0 };

    return next.bs_stringToGuid(str);
}

static bool _preval_bs_sameGuid(bs_GUID* a, bs_GUID* b) {
    if (a == NULL)
        return false;

    if (b == NULL)
        return false;

    return next.bs_sameGuid(a, b);
}

static bs_GUID _preval_bs_guid() {
    return next.bs_guid();
}

static bool _preval_bs_guidIsNull(bs_GUID* guid) {
    if (guid == NULL)
        return false;

    return next.bs_guidIsNull(guid);
}

static int _preval_bs_numDigits(int n) {
    return next.bs_numDigits(n);
}

static bool _preval_bs_directoryExists(char* path) {
    if (path == NULL)
        return false;

    return next.bs_directoryExists(path);
}

static char* _preval_bs_fileExtension(const char* path) {
    if (path == NULL)
        return NULL;

    return next.bs_fileExtension(path);
}

static bool _preval_bs_fileExtensionIs(const char* path, char* extension) {
    if (path == NULL)
        return false;

    if (extension == NULL)
        return false;

    return next.bs_fileExtensionIs(path, extension);
}

static char* _preval_bs_fileName(const char* path) {
    if (path == NULL)
        return NULL;

    return next.bs_fileName(path);
}

static void _preval_bs_appendFile(const char* path, const char* data) {
    if (path == NULL)
        return;

    if (data == NULL)
        return;

    return next.bs_appendFile(path, data);
}

static void _preval_bs_saveFile(char* data, bs_U32 data_len, char* path, int path_length) {
    if (data == NULL)
        return;

    if (path == NULL)
        return;

    return next.bs_saveFile(data, data_len, path, path_length);
}

static void _preval_bs_convertWin32Path(char* path, int path_length) {
    if (path == NULL)
        return;

    return next.bs_convertWin32Path(path, path_length);
}

static bs_Result _preval_bs_ensureDirectory(char* path, int path_length) {
    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_ensureDirectory(path, path_length);
}

static bs_Result _preval_bs_fileModifiedDate(bs_DateTime* out, char* path, int path_length) {
    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_fileModifiedDate(out, path, path_length);
}

static bs_Result _preval_bs_setFileModifiedDate(bs_DateTime* date, char* path, int path_length) {
    if (date == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_setFileModifiedDate(date, path, path_length);
}

static bs_String* _preval_bs_fullPath(bs_String* old, const char* path, int path_len) {
    if (old == NULL)
        return NULL;

    if (path == NULL)
        return NULL;

    return next.bs_fullPath(old, path, path_len);
}

static bool _preval_bs_fileExists(char* path, int path_length) {
    if (path == NULL)
        return false;

    return next.bs_fileExists(path, path_length);
}

static bs_I64 _preval_bs_toLong(const char* str) {
    if (str == NULL)
        return 0;

    return next.bs_toLong(str);
}

static bs_U64 _preval_bs_toULong(const char* str) {
    if (str == NULL)
        return 0;

    return next.bs_toULong(str);
}

static bs_F64 _preval_bs_toDouble(const char* str) {
    if (str == NULL)
        return 0;

    return next.bs_toDouble(str);
}

static bs_Result _preval_bs_model(int package_id, const char* name, bs_U32 flags, bs_Resource** out) {
    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_model(package_id, name, flags, out);
}

static void _preval_bs_destroyModel(bs_Model* model) {
    if (model == NULL)
        return;

    return next.bs_destroyModel(model);
}

static bs_vec4 _preval_bs_interpolateRotation(bs_AnimationBone* animation_joint, float time) {
    if (animation_joint == NULL)
        return (bs_vec4) { 0 };

    return next.bs_interpolateRotation(animation_joint, time);
}

static bs_vec3 _preval_bs_interpolateTranslation(bs_AnimationBone* animation_joint, float time) {
    if (animation_joint == NULL)
        return (bs_vec3) { 0 };

    return next.bs_interpolateTranslation(animation_joint, time);
}

static bs_vec3 _preval_bs_interpolateScale(bs_AnimationBone* animation_joint, float time) {
    if (animation_joint == NULL)
        return (bs_vec3) { 0 };

    return next.bs_interpolateScale(animation_joint, time);
}

static bs_mat4 _preval_bs_boneTransform(bs_Armature* armature, bs_Bone* bone) {
    if (armature == NULL)
        return (bs_mat4) { 0 };

    if (bone == NULL)
        return (bs_mat4) { 0 };

    return next.bs_boneTransform(armature, bone);
}

static bs_vec3 _preval_bs_bonePosition(bs_Armature* armature, bs_Bone* bone) {
    if (armature == NULL)
        return (bs_vec3) { 0 };

    if (bone == NULL)
        return (bs_vec3) { 0 };

    return next.bs_bonePosition(armature, bone);
}

static bs_mat4* _preval_bs_transformBone(bs_Armature* armature, bs_Bone* bone, const bs_mat4* transform) {
    if (armature == NULL)
        return NULL;

    if (bone == NULL)
        return NULL;

    if (transform == NULL)
        return NULL;

    return next.bs_transformBone(armature, bone, transform);
}

static void _preval_bs_blendPose(bs_Armature* armature, bs_Animation* animation_a, bs_Animation* animation_b, float factor, float time_a, float time_b) {
    if (armature == NULL)
        return;

    if (animation_a == NULL)
        return;

    if (animation_b == NULL)
        return;

    return next.bs_blendPose(armature, animation_a, animation_b, factor, time_a, time_b);
}

static int _preval_bs_bone(bs_Armature* armature, bs_mat4 local_transform, int parent_id, const char* name) {
    if (armature == NULL)
        return 0;

    if (name == NULL)
        return 0;

    return next.bs_bone(armature, local_transform, parent_id, name);
}

static void _preval_bs_fabrik(bs_Armature* armature, int end_effector_id, bs_vec3 target, int chain_length, float* chain) {
    if (armature == NULL)
        return;

    if (chain == NULL)
        return;

    return next.bs_fabrik(armature, end_effector_id, target, chain_length, chain);
}

static void _preval_bs_bindPose(bs_Armature* armature) {
    if (armature == NULL)
        return;

    return next.bs_bindPose(armature);
}

static void _preval_bs_keyframePosition(bs_AnimationBone* bone, float timestamp, bs_vec3 position) {
    if (bone == NULL)
        return;

    return next.bs_keyframePosition(bone, timestamp, position);
}

static void _preval_bs_keyframeRotation(bs_AnimationBone* bone, float timestamp, bs_vec4 rotation) {
    if (bone == NULL)
        return;

    return next.bs_keyframeRotation(bone, timestamp, rotation);
}

static void _preval_bs_keyframeScale(bs_AnimationBone* bone, float timestamp, bs_vec3 scale) {
    if (bone == NULL)
        return;

    return next.bs_keyframeScale(bone, timestamp, scale);
}

static bs_Result _preval_bs_loadAnimation(bs_Model* model, const char* name, bs_Animation* out) {
    if (model == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadAnimation(model, name, out);
}

static int _preval_bs_queryBoneId(bs_Armature* armature, const char* name) {
    if (armature == NULL)
        return 0;

    if (name == NULL)
        return 0;

    return next.bs_queryBoneId(armature, name);
}

static bs_Armature* _preval_bs_queryArmature(bs_Model* model, const char* name) {
    if (model == NULL)
        return NULL;

    if (name == NULL)
        return NULL;

    return next.bs_queryArmature(model, name);
}

static bs_Bone* _preval_bs_queryBone(bs_Armature* armature, const char* name) {
    if (armature == NULL)
        return NULL;

    if (name == NULL)
        return NULL;

    return next.bs_queryBone(armature, name);
}

static bs_Mesh* _preval_bs_queryMesh(bs_Model* model, const char * name) {
    if (model == NULL)
        return NULL;

    if (name == NULL)
        return NULL;

    return next.bs_queryMesh(model, name);
}

static bs_Mesh* _preval_bs_queryMeshHash(bs_Model* model, bs_U64 hash) {
    if (model == NULL)
        return NULL;

    return next.bs_queryMeshHash(model, hash);
}

static bs_Material* _preval_bs_queryMaterial(bs_Model* model, const char* name) {
    if (model == NULL)
        return NULL;

    if (name == NULL)
        return NULL;

    return next.bs_queryMaterial(model, name);
}

static const char* _preval_bs_idName(bs_U32 source_id, bs_U32 id) {
    return next.bs_idName(source_id, id);
}

static bs_Object* _preval_bs_object(bs_U32 source_id, bs_U32 id, size_t size, size_t flexible_size, int flexible_count, bs_U32 flags) {
    return next.bs_object(source_id, id, size, flexible_size, flexible_count, flags);
}

static bs_List* _preval_bs_packages() {
    return next.bs_packages();
}

static bs_List* _preval_bs_objectSources() {
    return next.bs_objectSources();
}

static void _preval_bs_destroyResource(bs_Resource* resource) {
    if (resource == NULL)
        return;

    return next.bs_destroyResource(resource);
}

static bs_Result _preval_bs_queryResource(int package_id, const char* name, bs_Resource** out) {
    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_queryResource(package_id, name, out);
}

static bs_Result _preval_bs_queryPackage(const char* name, int* out) {
    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_queryPackage(name, out);
}

static bs_Result _preval_bs_loadResource(int package_id, bs_U32 flags, bs_Resource** out, char* value, int value_length) {
    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadResource(package_id, flags, out, value, value_length);
}

static bs_Result _preval_bs_loadPackage(const char* path, int* out) {
    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadPackage(path, out);
}

static int _preval_bs_configureSource(bs_ObjectType type, int count, const char** names) {
    if (names == NULL)
        return 0;

    return next.bs_configureSource(type, count, names);
}

static bool _preval_bs_exists(bs_U32 source_id, bs_U32 id) {
    return next.bs_exists(source_id, id);
}

static bs_Object* _preval_bs_fetch(bs_U32 source_id, bs_U32 id) {
    return next.bs_fetch(source_id, id);
}

static bool _preval_bs_shouldLoadId(bs_U32 source_id, bs_U32 id) {
    return next.bs_shouldLoadId(source_id, id);
}

static bs_Result _preval_bs_shader(int package_id, const char* name, bs_U32 flags, bs_Resource** out) {
    if (name == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_shader(package_id, name, flags, out);
}

static void _preval_bs_destroyShader(bs_Shader* shader) {
    if (shader == NULL)
        return;

    return next.bs_destroyShader(shader);
}

static bs_Pipeline* _preval_bs_computePipeline(bs_Shader* compute_shader, bs_PipelineFlags flags) {
    if (compute_shader == NULL)
        return NULL;

    return next.bs_computePipeline(compute_shader, flags);
}

static void _preval_bs_destroyComputePipeline(bs_Pipeline* pipeline) {
    if (pipeline == NULL)
        return;

    return next.bs_destroyComputePipeline(pipeline);
}

static bs_Pipeline* _preval_bs_queryPipeline(bs_PipelineType type, bs_U64 hash) {
    return next.bs_queryPipeline(type, hash);
}

static bs_U64 _preval_bs_pipelineHash(bs_PipelineHash* descriptor) {
    if (descriptor == NULL)
        return 0;

    return next.bs_pipelineHash(descriptor);
}

static bs_Result _preval_bs_pipeline(bs_PipelineHash* descriptor, bs_Pipeline** out) {
    if (descriptor == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_pipeline(descriptor, out);
}

static void _preval_bs_destroyPipeline(bs_Pipeline* pipeline) {
    if (pipeline == NULL)
        return;

    return next.bs_destroyPipeline(pipeline);
}

static void _preval_bs_pushConstant(bs_Pipeline* pipeline, bs_U32 offset, bs_U32 size, void* data) {
    if (pipeline == NULL)
        return;

    if (data == NULL)
        return;

    return next.bs_pushConstant(pipeline, offset, size, data);
}

static bs_Result _preval_bs_rayTracingPipeline(bs_RayTracePipelineHash* pipeline_hash, bs_Pipeline** out) {
    if (pipeline_hash == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_rayTracingPipeline(pipeline_hash, out);
}

static void _preval_bs_loadBindings(int package_id, const char* path) {
    if (path == NULL)
        return;

    return next.bs_loadBindings(package_id, path);
}

static bs_Result _preval_bs_binding(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_Descriptor* descriptors, int descriptors_count, bs_Binding** out) {
    if (descriptors == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_binding(bind_set_slot, bind_point_slot, descriptors, descriptors_count, out);
}

static bs_Result _preval_bs_bindImage(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_Image* image, bs_Sampler* sampler, bs_ImageLayout layout) {
    if (image == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (image->head.source_id != BS_OBJECT_IMAGE)
        return BS_RESULT_VALIDATION_ERROR;

    if (sampler == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (sampler->head.source_id != BS_OBJECT_SAMPLER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bindImage(bind_set_slot, bind_point_slot, image, sampler, layout);
}

static bs_Result _preval_bs_bindImages(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_ImageDescriptor* images, int images_count) {
    if (images == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bindImages(bind_set_slot, bind_point_slot, images, images_count);
}

static bs_Result _preval_bs_bindBuffer(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_Buffer* buffer) {
    if (buffer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (buffer->head.source_id != BS_OBJECT_BUFFER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bindBuffer(bind_set_slot, bind_point_slot, buffer);
}

static bs_Result _preval_bs_bindBuffers(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_Buffer** buffers, int buffers_count) {
    if (buffers == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bindBuffers(bind_set_slot, bind_point_slot, buffers, buffers_count);
}

static bs_Result _preval_bs_bindAccelerationStructure(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_RayTracer* ray_tracer) {
    if (ray_tracer == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (ray_tracer->head.source_id != BS_OBJECT_RAY_TRACER)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bindAccelerationStructure(bind_set_slot, bind_point_slot, ray_tracer);
}

static bs_Result _preval_bs_bindAccelerationStructures(bs_U32 bind_set_slot, bs_U32 bind_point_slot, bs_RayTracer** ray_tracers, int ray_tracers_count) {
    if (ray_tracers == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_bindAccelerationStructures(bind_set_slot, bind_point_slot, ray_tracers, ray_tracers_count);
}

static void _preval_bs_pushDescriptors() {
    return next.bs_pushDescriptors();
}

static void _preval_bs_pushBindings() {
    return next.bs_pushBindings();
}

static bs_BindSet* _preval_bs_queryBindSet(bs_U32 id) {
    return next.bs_queryBindSet(id);
}

static bs_Binding* _preval_bs_queryBinding(const bs_BindSet* bind_set, bs_U32 id) {
    if (bind_set == NULL)
        return NULL;

    return next.bs_queryBinding(bind_set, id);
}

static void _preval_bs_configureAttribute(const char* name, bs_Format base_format) {
    if (name == NULL)
        return;

    return next.bs_configureAttribute(name, base_format);
}

static bs_Window* _preval_bs_wnd() {
    return next.bs_wnd();
}

static bs_IO* _preval_bs_io() {
    return next.bs_io();
}

static bs_Result _preval_bs_timeZoneBias(int* out) {
    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_timeZoneBias(out);
}

static bs_DateTime _preval_bs_dateTime() {
    return next.bs_dateTime();
}

static bs_I64 _preval_bs_totalSeconds(const bs_DateTime* date_time) {
    if (date_time == NULL)
        return 0;

    return next.bs_totalSeconds(date_time);
}

static bool _preval_bs_isLaterThan(const bs_DateTime* a, const bs_DateTime* b) {
    if (a == NULL)
        return false;

    if (b == NULL)
        return false;

    return next.bs_isLaterThan(a, b);
}

static bs_vec2 _preval_bs_cursorPosition() {
    return next.bs_cursorPosition();
}

static bs_ivec2 _preval_bs_windowPosition() {
    return next.bs_windowPosition();
}

static bs_vec2 _preval_bs_screenCursorPosition() {
    return next.bs_screenCursorPosition();
}

static void _preval_bs_lockCursorPosition(bs_Window* window, bool value) {
    if (window == NULL)
        return;

    return next.bs_lockCursorPosition(window, value);
}

static void _preval_bs_disableUserInputs(bool value) {
    return next.bs_disableUserInputs(value);
}

static bool _preval_bs_middleClick() {
    return next.bs_middleClick();
}

static bool _preval_bs_middleClickOnce() {
    return next.bs_middleClickOnce();
}

static bool _preval_bs_middleClickUpOnce() {
    return next.bs_middleClickUpOnce();
}

static bool _preval_bs_leftClick() {
    return next.bs_leftClick();
}

static bool _preval_bs_rightClick() {
    return next.bs_rightClick();
}

static bool _preval_bs_rightClickOnce() {
    return next.bs_rightClickOnce();
}

static bool _preval_bs_leftClickOnce() {
    return next.bs_leftClickOnce();
}

static bool _preval_bs_rightClickUpOnce() {
    return next.bs_rightClickUpOnce();
}

static bool _preval_bs_leftClickUpOnce() {
    return next.bs_leftClickUpOnce();
}

static bool _preval_bs_keyDown(bs_U32 code) {
    return next.bs_keyDown(code);
}

static bool _preval_bs_keyHeld(bs_U32 code) {
    return next.bs_keyHeld(code);
}

static bool _preval_bs_keyDownOnce(bs_U32 code) {
    return next.bs_keyDownOnce(code);
}

static bool _preval_bs_keyUpOnce(bs_U32 code) {
    return next.bs_keyUpOnce(code);
}

static bool _preval_bs_charDown(unsigned char c) {
    return next.bs_charDown(c);
}

static bool _preval_bs_charDownOnce(unsigned char c) {
    return next.bs_charDownOnce(c);
}

static bool _preval_bs_charUpOnce(unsigned char c) {
    return next.bs_charUpOnce(c);
}

static int _preval_bs_scroll() {
    return next.bs_scroll();
}

static void _preval_bs_resizeWindow(bs_U32 width, bs_U32 height) {
    return next.bs_resizeWindow(width, height);
}

static bs_ivec2 _preval_bs_screenDimensions() {
    return next.bs_screenDimensions();
}

static void _preval_bs_moveWindow(int x, int y) {
    return next.bs_moveWindow(x, y);
}

static void _preval_bs_window(bs_U32 width, bs_U32 height, const char* title) {
    if (title == NULL)
        return;

    return next.bs_window(width, height, title);
}

static void _preval_bs_tick(bs_Window* window, bs_Callback tick, bs_Callback fixed_tick) {
    if (window == NULL)
        return;

    return next.bs_tick(window, tick, fixed_tick);
}

static void _preval_bs_exit() {
    return next.bs_exit();
}

static void _preval_bs_setCursor(bs_CursorIcon type) {
    return next.bs_setCursor(type);
}

static void _preval_bs_maximize() {
    return next.bs_maximize();
}

static void _preval_bs_minimize() {
    return next.bs_minimize();
}

static double _preval_bs_deltaTime() {
    return next.bs_deltaTime();
}

static void _preval_bs_pause() {
    return next.bs_pause();
}

static void _preval_bs_advance() {
    return next.bs_advance();
}

static double _preval_bs_elapsedTime() {
    return next.bs_elapsedTime();
}

static bs_ivec2 _preval_bs_resolution() {
    return next.bs_resolution();
}

static void _preval_bs_titleWindow(char* name, int name_length) {
    if (name == NULL)
        return;

    return next.bs_titleWindow(name, name_length);
}

static bool _preval_bs_inFixedTick() {
    return next.bs_inFixedTick();
}

static void _preval_bs_setTargetFramerate(int fps) {
    return next.bs_setTargetFramerate(fps);
}

static bs_Timer _preval_bs_timer() {
    return next.bs_timer();
}

static void _preval_bs_checkTimer(bs_Timer* timer) {
    if (timer == NULL)
        return;

    return next.bs_checkTimer(timer);
}

static void _preval_bs_copyToClipboard(bs_Timer* timer, char* value, int value_length) {
    if (timer == NULL)
        return;

    if (value == NULL)
        return;

    return next.bs_copyToClipboard(timer, value, value_length);
}

static bs_String* _preval_bs_appendString(bs_String* destination, char* value, int value_length) {
    if (destination == NULL)
        return NULL;

    if (value == NULL)
        return NULL;

    return next.bs_appendString(destination, value, value_length);
}

static bs_Result _preval_bs_foreachFile(bs_ForeachDocumentFunction x, void* param, char* value, int value_length) {
    if (param == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_foreachFile(x, param, value, value_length);
}

static bs_Result _preval_bs_foreachDirectory(bs_ForeachDocumentFunction x, void* param, char* path, int path_length) {
    if (param == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_foreachDirectory(x, param, path, path_length);
}

static int _preval_bs_numFiles(char* value, int value_length) {
    if (value == NULL)
        return 0;

    return next.bs_numFiles(value, value_length);
}

static int _preval_bs_numDirectories(char* value, int value_length) {
    if (value == NULL)
        return 0;

    return next.bs_numDirectories(value, value_length);
}

static bs_Result _preval_bs_loadFile(bs_String** out, char* value, int value_length) {
    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadFile(out, value, value_length);
}

static bs_Result _preval_bs_loadFileChunk(const char* path, long offset, size_t size, bs_String** out, char* value, int value_length) {
    if (path == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (out == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_loadFileChunk(path, offset, size, out, value, value_length);
}

static bs_Result _preval_bs_deleteFile(char* value, int value_length) {
    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_deleteFile(value, value_length);
}

static bs_Result _preval_bs_deleteDirectoryContents(char* value, int value_length) {
    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_deleteDirectoryContents(value, value_length);
}

static bs_Result _preval_bs_deleteDirectory(char* value, int value_length) {
    if (value == NULL)
        return BS_RESULT_VALIDATION_ERROR;

    return next.bs_deleteDirectory(value, value_length);
}

static const char* _preval_bs_serializeJsonType(bs_JsonType e) {
    return next.bs_serializeJsonType(e);
}

static const char* _preval_bs_serializeShaderType(bs_ShaderType e) {
    return next.bs_serializeShaderType(e);
}

static bs_ShaderType _preval_bs_deserializeShaderType(const char* value) {
    if (value == NULL)
        return 0;

    return next.bs_deserializeShaderType(value);
}

static const char* _preval_bs_serializeBindType(bs_BindType e) {
    return next.bs_serializeBindType(e);
}

static bs_BindType _preval_bs_deserializeBindType(const char* value) {
    if (value == NULL)
        return 0;

    return next.bs_deserializeBindType(value);
}

bs_FunctionTable _preval_bs_getFunctionTable() {
    bs_FunctionTable functions;

    functions.bs_v2Add = _preval_bs_v2Add;
    functions.bs_v2Sub = _preval_bs_v2Sub;
    functions.bs_v2Mul = _preval_bs_v2Mul;
    functions.bs_v2Div = _preval_bs_v2Div;
    functions.bs_v2AddS = _preval_bs_v2AddS;
    functions.bs_v2SubS = _preval_bs_v2SubS;
    functions.bs_v2MulS = _preval_bs_v2MulS;
    functions.bs_v2DivS = _preval_bs_v2DivS;
    functions.bs_v2Dot = _preval_bs_v2Dot;
    functions.bs_v2Distance = _preval_bs_v2Distance;
    functions.bs_v2Magnitude = _preval_bs_v2Magnitude;
    functions.bs_v2MagnitudeSqrd = _preval_bs_v2MagnitudeSqrd;
    functions.bs_v2Normalize = _preval_bs_v2Normalize;
    functions.bs_v2Lerp = _preval_bs_v2Lerp;
    functions.bs_v2Mid = _preval_bs_v2Mid;
    functions.bs_v3Add = _preval_bs_v3Add;
    functions.bs_v3Sub = _preval_bs_v3Sub;
    functions.bs_v3Mul = _preval_bs_v3Mul;
    functions.bs_v3Div = _preval_bs_v3Div;
    functions.bs_v3AddS = _preval_bs_v3AddS;
    functions.bs_v3SubS = _preval_bs_v3SubS;
    functions.bs_v3MulS = _preval_bs_v3MulS;
    functions.bs_v3DivS = _preval_bs_v3DivS;
    functions.bs_v3Dot = _preval_bs_v3Dot;
    functions.bs_v3Distance = _preval_bs_v3Distance;
    functions.bs_v3Magnitude = _preval_bs_v3Magnitude;
    functions.bs_v3MagnitudeSqrd = _preval_bs_v3MagnitudeSqrd;
    functions.bs_v3Normalize = _preval_bs_v3Normalize;
    functions.bs_v3Lerp = _preval_bs_v3Lerp;
    functions.bs_v3Mid = _preval_bs_v3Mid;
    functions.bs_v3Cross = _preval_bs_v3Cross;
    functions.bs_v3Angle = _preval_bs_v3Angle;
    functions.bs_v4Add = _preval_bs_v4Add;
    functions.bs_v4Sub = _preval_bs_v4Sub;
    functions.bs_v4Mul = _preval_bs_v4Mul;
    functions.bs_v4Div = _preval_bs_v4Div;
    functions.bs_v4AddS = _preval_bs_v4AddS;
    functions.bs_v4SubS = _preval_bs_v4SubS;
    functions.bs_v4MulS = _preval_bs_v4MulS;
    functions.bs_v4DivS = _preval_bs_v4DivS;
    functions.bs_v4Dot = _preval_bs_v4Dot;
    functions.bs_v4Distance = _preval_bs_v4Distance;
    functions.bs_v4Magnitude = _preval_bs_v4Magnitude;
    functions.bs_v4MagnitudeSqrd = _preval_bs_v4MagnitudeSqrd;
    functions.bs_v4Normalize = _preval_bs_v4Normalize;
    functions.bs_v4Lerp = _preval_bs_v4Lerp;
    functions.bs_m3Mul = _preval_bs_m3Mul;
    functions.bs_m3Transpose = _preval_bs_m3Transpose;
    functions.bs_m3Inverse = _preval_bs_m3Inverse;
    functions.bs_m3MulV3 = _preval_bs_m3MulV3;
    functions.bs_m4Mul = _preval_bs_m4Mul;
    functions.bs_m4Transpose = _preval_bs_m4Transpose;
    functions.bs_m4Inverse = _preval_bs_m4Inverse;
    functions.bs_m4x3 = _preval_bs_m4x3;
    functions.bs_m4MulV3 = _preval_bs_m4MulV3;
    functions.bs_m4MulV4 = _preval_bs_m4MulV4;
    functions.bs_m4Translate = _preval_bs_m4Translate;
    functions.bs_m4Rotate = _preval_bs_m4Rotate;
    functions.bs_m4Scale = _preval_bs_m4Scale;
    functions.bs_m3ToQ = _preval_bs_m3ToQ;
    functions.bs_m4ToQ = _preval_bs_m4ToQ;
    functions.bs_qToM3 = _preval_bs_qToM3;
    functions.bs_qToM4 = _preval_bs_qToM4;
    functions.bs_qNormalize = _preval_bs_qNormalize;
    functions.bs_qSlerp = _preval_bs_qSlerp;
    functions.bs_qRotateV3 = _preval_bs_qRotateV3;
    functions.bs_qLongSlerp = _preval_bs_qLongSlerp;
    functions.bs_eulToQ = _preval_bs_eulToQ;
    functions.bs_qToEul = _preval_bs_qToEul;
    functions.bs_orthographic = _preval_bs_orthographic;
    functions.bs_perspective = _preval_bs_perspective;
    functions.bs_lookAt = _preval_bs_lookAt;
    functions.bs_look = _preval_bs_look;
    functions.bs_v2CubicBezier = _preval_bs_v2CubicBezier;
    functions.bs_v2QuadBezier = _preval_bs_v2QuadBezier;
    functions.bs_v3CubicBezier = _preval_bs_v3CubicBezier;
    functions.bs_v3QuadBezier = _preval_bs_v3QuadBezier;
    functions.bs_abs = _preval_bs_abs;
    functions.bs_sin = _preval_bs_sin;
    functions.bs_cos = _preval_bs_cos;
    functions.bs_tan = _preval_bs_tan;
    functions.bs_sign = _preval_bs_sign;
    functions.bs_quad = _preval_bs_quad;
    functions.bs_lerp = _preval_bs_lerp;
    functions.bs_degrees = _preval_bs_degrees;
    functions.bs_radians = _preval_bs_radians;
    functions.bs_hsvToRgb = _preval_bs_hsvToRgb;
    functions.bs_rgbToHsv = _preval_bs_rgbToHsv;
    functions.bs_convertVulkanResult = _preval_bs_convertVulkanResult;
    functions.bs_convertWin32Error = _preval_bs_convertWin32Error;
    functions.bs_serializeWin32Error = _preval_bs_serializeWin32Error;
    functions.bs_convertErrno = _preval_bs_convertErrno;
    functions.bs_serializeErrno = _preval_bs_serializeErrno;
    functions.bs_playSound = _preval_bs_playSound;
    functions.bs_sound = _preval_bs_sound;
    functions.bs_iniAudio = _preval_bs_iniAudio;
    functions.bs_ray = _preval_bs_ray;
    functions.bs_rayVsObb = _preval_bs_rayVsObb;
    functions.bs_sphereVsPoint = _preval_bs_sphereVsPoint;
    functions.bs_sphereVsObbTest = _preval_bs_sphereVsObbTest;
    functions.bs_sphereVsObb = _preval_bs_sphereVsObb;
    functions.bs_rectangleVsPoint = _preval_bs_rectangleVsPoint;
    functions.bs_lineVsLine = _preval_bs_lineVsLine;
    functions.bs_populateVertexDeclaration = _preval_bs_populateVertexDeclaration;
    functions.bs_currentSwap = _preval_bs_currentSwap;
    functions.bs_beginComment = _preval_bs_beginComment;
    functions.bs_endComment = _preval_bs_endComment;
    functions.bs_swapchainImage = _preval_bs_swapchainImage;
    functions.bs_clearStencil = _preval_bs_clearStencil;
    functions.bs_clearDepth = _preval_bs_clearDepth;
    functions.bs_clearDepthStencil = _preval_bs_clearDepthStencil;
    functions.bs_clearColor = _preval_bs_clearColor;
    functions.bs_stencilReference = _preval_bs_stencilReference;
    functions.bs_cull = _preval_bs_cull;
    functions.bs_setLineWidth = _preval_bs_setLineWidth;
    functions.bs_batchSize = _preval_bs_batchSize;
    functions.bs_render = _preval_bs_render;
    functions.bs_barrier = _preval_bs_barrier;
    functions.bs_rayTrace = _preval_bs_rayTrace;
    functions.bs_rayTracer = _preval_bs_rayTracer;
    functions.bs_accelerateAabb = _preval_bs_accelerateAabb;
    functions.bs_accelerateBatch = _preval_bs_accelerateBatch;
    functions.bs_build = _preval_bs_build;
    functions.bs_destroyRayTracer = _preval_bs_destroyRayTracer;
    functions.bs_dispatchAsync = _preval_bs_dispatchAsync;
    functions.bs_bufferSwaps = _preval_bs_bufferSwaps;
    functions.bs_nameBuffer = _preval_bs_nameBuffer;
    functions.bs_buffer = _preval_bs_buffer;
    functions.bs_bufferIsMapped = _preval_bs_bufferIsMapped;
    functions.bs_bufferMap = _preval_bs_bufferMap;
    functions.bs_mapBuffer = _preval_bs_mapBuffer;
    functions.bs_unmapBuffer = _preval_bs_unmapBuffer;
    functions.bs_stageNull = _preval_bs_stageNull;
    functions.bs_stageList = _preval_bs_stageList;
    functions.bs_stageImage = _preval_bs_stageImage;
    functions.bs_destroyBuffer = _preval_bs_destroyBuffer;
    functions.bs_copyAsync = _preval_bs_copyAsync;
    functions.bs_setBufferAsync = _preval_bs_setBufferAsync;
    functions.bs_batch = _preval_bs_batch;
    functions.bs_queryAttribute = _preval_bs_queryAttribute;
    functions.bs_batchIsPushed = _preval_bs_batchIsPushed;
    functions.bs_batchIsIndexed = _preval_bs_batchIsIndexed;
    functions.bs_minimizeBatch = _preval_bs_minimizeBatch;
    functions.bs_pushBatch = _preval_bs_pushBatch;
    functions.bs_unpushBatch = _preval_bs_unpushBatch;
    functions.bs_destroyBatch = _preval_bs_destroyBatch;
    functions.bs_recreateBatch = _preval_bs_recreateBatch;
    functions.bs_ensureBatchSize = _preval_bs_ensureBatchSize;
    functions.bs_batchVertex = _preval_bs_batchVertex;
    functions.bs_batchRange = _preval_bs_batchRange;
    functions.bs_pushIndex = _preval_bs_pushIndex;
    functions.bs_pushIndices = _preval_bs_pushIndices;
    functions.bs_batchCube = _preval_bs_batchCube;
    functions.bs_pushCube = _preval_bs_pushCube;
    functions.bs_batchCone = _preval_bs_batchCone;
    functions.bs_pushCone = _preval_bs_pushCone;
    functions.bs_batchRectangle = _preval_bs_batchRectangle;
    functions.bs_pushRectangle = _preval_bs_pushRectangle;
    functions.bs_batchQuad = _preval_bs_batchQuad;
    functions.bs_pushQuad = _preval_bs_pushQuad;
    functions.bs_batchTriangle = _preval_bs_batchTriangle;
    functions.bs_pushTriangle = _preval_bs_pushTriangle;
    functions.bs_batchLine = _preval_bs_batchLine;
    functions.bs_pushLine = _preval_bs_pushLine;
    functions.bs_batchRay = _preval_bs_batchRay;
    functions.bs_pushRay = _preval_bs_pushRay;
    functions.bs_batchPoint = _preval_bs_batchPoint;
    functions.bs_pushPoint = _preval_bs_pushPoint;
    functions.bs_batchAabb = _preval_bs_batchAabb;
    functions.bs_pushAabb = _preval_bs_pushAabb;
    functions.bs_batchSphere = _preval_bs_batchSphere;
    functions.bs_pushSphere = _preval_bs_pushSphere;
    functions.bs_batchPyramid = _preval_bs_batchPyramid;
    functions.bs_pushPyramid = _preval_bs_pushPyramid;
    functions.bs_batchBipyramid = _preval_bs_batchBipyramid;
    functions.bs_pushBipyramid = _preval_bs_pushBipyramid;
    functions.bs_batchPrimitive = _preval_bs_batchPrimitive;
    functions.bs_pushPrimitive = _preval_bs_pushPrimitive;
    functions.bs_batchMesh = _preval_bs_batchMesh;
    functions.bs_pushMesh = _preval_bs_pushMesh;
    functions.bs_batchModel = _preval_bs_batchModel;
    functions.bs_pushModel = _preval_bs_pushModel;
    functions.bs_rendererSwapsCount = _preval_bs_rendererSwapsCount;
    functions.bs_renderer = _preval_bs_renderer;
    functions.bs_output = _preval_bs_output;
    functions.bs_input = _preval_bs_input;
    functions.bs_dependency = _preval_bs_dependency;
    functions.bs_renderPass = _preval_bs_renderPass;
    functions.bs_framebuffer = _preval_bs_framebuffer;
    functions.bs_runPass = _preval_bs_runPass;
    functions.bs_rendererIsDynamic = _preval_bs_rendererIsDynamic;
    functions.bs_beginRender = _preval_bs_beginRender;
    functions.bs_endRender = _preval_bs_endRender;
    functions.bs_destroyRenderer = _preval_bs_destroyRenderer;
    functions.bs_resizeRenderer = _preval_bs_resizeRenderer;
    functions.bs_singleTimesQueue = _preval_bs_singleTimesQueue;
    functions.bs_queueFamily = _preval_bs_queueFamily;
    functions.bs_present = _preval_bs_present;
    functions.bs_acquire = _preval_bs_acquire;
    functions.bs_queueSwap = _preval_bs_queueSwap;
    functions.bs_awaitQueue = _preval_bs_awaitQueue;
    functions.bs_awaitAcquisition = _preval_bs_awaitAcquisition;
    functions.bs_enqueue = _preval_bs_enqueue;
    functions.bs_imageIndex = _preval_bs_imageIndex;
    functions.bs_queueSwapsCount = _preval_bs_queueSwapsCount;
    functions.bs_queue = _preval_bs_queue;
    functions.bs_destroyQueue = _preval_bs_destroyQueue;
    functions.bs_stallGPU = _preval_bs_stallGPU;
    functions.bs_stallQueue = _preval_bs_stallQueue;
    functions.bs_stall = _preval_bs_stall;
    functions.bs_poll = _preval_bs_poll;
    functions.bs_enterSingle = _preval_bs_enterSingle;
    functions.bs_leaveSingle = _preval_bs_leaveSingle;
    functions.bs_getScope = _preval_bs_getScope;
    functions.bs_setScope = _preval_bs_setScope;
    functions.bs_runSingle = _preval_bs_runSingle;
    functions.bs_glyph = _preval_bs_glyph;
    functions.bs_ttf = _preval_bs_ttf;
    functions.bs_rasterizeGlyph = _preval_bs_rasterizeGlyph;
    functions.bs_kern = _preval_bs_kern;
    functions.bs_bindFont = _preval_bs_bindFont;
    functions.bs_textDimensions = _preval_bs_textDimensions;
    functions.bs_destroyFont = _preval_bs_destroyFont;
    functions.bs_loadFont = _preval_bs_loadFont;
    functions.bs_image = _preval_bs_image;
    functions.bs_imageSwapsCount = _preval_bs_imageSwapsCount;
    functions.bs_transition = _preval_bs_transition;
    functions.bs_inspectPng = _preval_bs_inspectPng;
    functions.bs_loadPngData = _preval_bs_loadPngData;
    functions.bs_loadPng = _preval_bs_loadPng;
    functions.bs_bitmapImage = _preval_bs_bitmapImage;
    functions.bs_savePng = _preval_bs_savePng;
    functions.bs_encodePng = _preval_bs_encodePng;
    functions.bs_destroyImage = _preval_bs_destroyImage;
    functions.bs_resizeImage = _preval_bs_resizeImage;
    functions.bs_queryImageIndexHash = _preval_bs_queryImageIndexHash;
    functions.bs_queryImageIndex = _preval_bs_queryImageIndex;
    functions.bs_copyImageToBufferAsync = _preval_bs_copyImageToBufferAsync;
    functions.bs_copyBufferToImage = _preval_bs_copyBufferToImage;
    functions.bs_blit = _preval_bs_blit;
    functions.bs_loadImage = _preval_bs_loadImage;
    functions.bs_isStencilFormat = _preval_bs_isStencilFormat;
    functions.bs_isDepthFormat = _preval_bs_isDepthFormat;
    functions.bs_hasAlpha = _preval_bs_hasAlpha;
    functions.bs_nameImage = _preval_bs_nameImage;
    functions.bs_destroySampler = _preval_bs_destroySampler;
    functions.bs_sampler = _preval_bs_sampler;
    functions.bs_loadAtlas = _preval_bs_loadAtlas;
    functions.bs_atlasCoordinates = _preval_bs_atlasCoordinates;
    functions.bs_mirrorUV = _preval_bs_mirrorUV;
    functions.bs_flipUV = _preval_bs_flipUV;
    functions.bs_atlasSize = _preval_bs_atlasSize;
    functions.bs_queryAtlasHash = _preval_bs_queryAtlasHash;
    functions.bs_queryAtlas = _preval_bs_queryAtlas;
    functions.bs_destroyAtlas = _preval_bs_destroyAtlas;
    functions.bs_loadAtlasMemory = _preval_bs_loadAtlasMemory;
    functions.bs_parseArgs = _preval_bs_parseArgs;
    functions.bs_arguments = _preval_bs_arguments;
    functions.bs_ini = _preval_bs_ini;
    functions.bs_load = _preval_bs_load;
    functions.bs_queryProcedures = _preval_bs_queryProcedures;
    functions.bsi_fetchCommands = _preval_bsi_fetchCommands;
    functions.bsi_fetchDevice = _preval_bsi_fetchDevice;
    functions.bsi_resizeObjects = _preval_bsi_resizeObjects;
    functions.bs_resetQueue = _preval_bs_resetQueue;
    functions.bs_pushQueue = _preval_bs_pushQueue;
    functions.bsi_nameHandle = _preval_bsi_nameHandle;
    functions.bs_beginEnumeration = _preval_bs_beginEnumeration;
    functions.bs_enumerateJson = _preval_bs_enumerateJson;
    functions.bs_jsonRoot = _preval_bs_jsonRoot;
    functions.bs_ensureJsonMutable = _preval_bs_ensureJsonMutable;
    functions.bs_jsonCopy = _preval_bs_jsonCopy;
    functions.bs_saveJson = _preval_bs_saveJson;
    functions.bs_emptyJson = _preval_bs_emptyJson;
    functions.bs_emptyJsonArray = _preval_bs_emptyJsonArray;
    functions.bs_json = _preval_bs_json;
    functions.bs_loadJson = _preval_bs_loadJson;
    functions.bs_destroyJson = _preval_bs_destroyJson;
    functions.bs_parseJsonValue = _preval_bs_parseJsonValue;
    functions.bs_fetchJson = _preval_bs_fetchJson;
    functions.bs_deleteJson = _preval_bs_deleteJson;
    functions.bs_ensureJson = _preval_bs_ensureJson;
    functions.bs_jsonValueFromObject = _preval_bs_jsonValueFromObject;
    functions.bs_jsonValueFromRoot = _preval_bs_jsonValueFromRoot;
    functions.bs_jsonValueFromBool = _preval_bs_jsonValueFromBool;
    functions.bs_jsonValueFromInteger = _preval_bs_jsonValueFromInteger;
    functions.bs_jsonValueFromFloat = _preval_bs_jsonValueFromFloat;
    functions.bs_jsonValueFromString = _preval_bs_jsonValueFromString;
    functions.bs_jsonValueFromStringPointer = _preval_bs_jsonValueFromStringPointer;
    functions.bs_jsonValueFromDateTime = _preval_bs_jsonValueFromDateTime;
    functions.bs_jsonObject = _preval_bs_jsonObject;
    functions.bs_jsonArray = _preval_bs_jsonArray;
    functions.bs_jsonVec2 = _preval_bs_jsonVec2;
    functions.bs_jsonVec3 = _preval_bs_jsonVec3;
    functions.bs_jsonVec4 = _preval_bs_jsonVec4;
    functions.bs_jsonRGBA = _preval_bs_jsonRGBA;
    functions.bs_logSection = _preval_bs_logSection;
    functions.bs_logEndOfSection = _preval_bs_logEndOfSection;
    functions.bs_logWithTimestamp = _preval_bs_logWithTimestamp;
    functions.bs_log = _preval_bs_log;
    functions.bs_info = _preval_bs_info;
    functions.bs_warn = _preval_bs_warn;
    functions.bs_critical = _preval_bs_critical;
    functions.bs_logObjectDiff = _preval_bs_logObjectDiff;
    functions.bs_logUnchangedObjects = _preval_bs_logUnchangedObjects;
    functions.bs_logBindings = _preval_bs_logBindings;
    functions.bs_infoF4 = _preval_bs_infoF4;
    functions.bs_instance = _preval_bs_instance;
    functions.bs_args = _preval_bs_args;
    functions.bs_features = _preval_bs_features;
    functions.bs_props = _preval_bs_props;
    functions.bs_config = _preval_bs_config;
    functions.bs_scope = _preval_bs_scope;
    functions.bs_system = _preval_bs_system;
    functions.bs_createThread = _preval_bs_createThread;
    functions.bs_formatStringLength = _preval_bs_formatStringLength;
    functions.bs_checkStringPool = _preval_bs_checkStringPool;
    functions.bs_stringAlloc = _preval_bs_stringAlloc;
    functions.bs_emptyString = _preval_bs_emptyString;
    functions.bs_string = _preval_bs_string;
    functions.bs_toUpper = _preval_bs_toUpper;
    functions.bs_toLower = _preval_bs_toLower;
    functions.bs_hash = _preval_bs_hash;
    functions.bs_stringHash = _preval_bs_stringHash;
    functions.bs_startsWith = _preval_bs_startsWith;
    functions.bs_endsWith = _preval_bs_endsWith;
    functions.bs_lastChar = _preval_bs_lastChar;
    functions.bs_stringContainsChar = _preval_bs_stringContainsChar;
    functions.bs_workingDirectory = _preval_bs_workingDirectory;
    functions.bs_setWorkingDirectory = _preval_bs_setWorkingDirectory;
    functions.bs_executablePath = _preval_bs_executablePath;
    functions.bs_appdataPath = _preval_bs_appdataPath;
    functions.bs_shortenString = _preval_bs_shortenString;
    functions.bs_appendChar = _preval_bs_appendChar;
    functions.bs_removeLastCharsCount = _preval_bs_removeLastCharsCount;
    functions.bs_removeCharRange = _preval_bs_removeCharRange;
    functions.bs_insertChar = _preval_bs_insertChar;
    functions.bs_appendPath = _preval_bs_appendPath;
    functions.bs_replaceCharOccurrences = _preval_bs_replaceCharOccurrences;
    functions.bs_strndup = _preval_bs_strndup;
    functions.bs_strnlen = _preval_bs_strnlen;
    functions.bs_strsep = _preval_bs_strsep;
    functions.bs_memmem = _preval_bs_memmem;
    functions.bs_alignUp = _preval_bs_alignUp;
    functions.bs_widen = _preval_bs_widen;
    functions.bs_unwiden = _preval_bs_unwiden;
    functions.bs_charString = _preval_bs_charString;
    functions.bs_free = _preval_bs_free;
    functions.bs_malloc = _preval_bs_malloc;
    functions.bs_calloc = _preval_bs_calloc;
    functions.bs_realloc = _preval_bs_realloc;
    functions.bs_listContains = _preval_bs_listContains;
    functions.bs_fetchUnit = _preval_bs_fetchUnit;
    functions.bs_fetchLast = _preval_bs_fetchLast;
    functions.bs_fetchLastNull = _preval_bs_fetchLastNull;
    functions.bs_ensureSize = _preval_bs_ensureSize;
    functions.bs_erase = _preval_bs_erase;
    functions.bs_pushBack = _preval_bs_pushBack;
    functions.bs_pushBackList = _preval_bs_pushBackList;
    functions.bs_destroyList = _preval_bs_destroyList;
    functions.bs_seekList = _preval_bs_seekList;
    functions.bs_minimizeList = _preval_bs_minimizeList;
    functions.bs_list = _preval_bs_list;
    functions.bs_guidToString = _preval_bs_guidToString;
    functions.bs_stringToGuid = _preval_bs_stringToGuid;
    functions.bs_sameGuid = _preval_bs_sameGuid;
    functions.bs_guid = _preval_bs_guid;
    functions.bs_guidIsNull = _preval_bs_guidIsNull;
    functions.bs_numDigits = _preval_bs_numDigits;
    functions.bs_directoryExists = _preval_bs_directoryExists;
    functions.bs_fileExtension = _preval_bs_fileExtension;
    functions.bs_fileExtensionIs = _preval_bs_fileExtensionIs;
    functions.bs_fileName = _preval_bs_fileName;
    functions.bs_appendFile = _preval_bs_appendFile;
    functions.bs_saveFile = _preval_bs_saveFile;
    functions.bs_convertWin32Path = _preval_bs_convertWin32Path;
    functions.bs_ensureDirectory = _preval_bs_ensureDirectory;
    functions.bs_fileModifiedDate = _preval_bs_fileModifiedDate;
    functions.bs_setFileModifiedDate = _preval_bs_setFileModifiedDate;
    functions.bs_fullPath = _preval_bs_fullPath;
    functions.bs_fileExists = _preval_bs_fileExists;
    functions.bs_toLong = _preval_bs_toLong;
    functions.bs_toULong = _preval_bs_toULong;
    functions.bs_toDouble = _preval_bs_toDouble;
    functions.bs_model = _preval_bs_model;
    functions.bs_destroyModel = _preval_bs_destroyModel;
    functions.bs_interpolateRotation = _preval_bs_interpolateRotation;
    functions.bs_interpolateTranslation = _preval_bs_interpolateTranslation;
    functions.bs_interpolateScale = _preval_bs_interpolateScale;
    functions.bs_boneTransform = _preval_bs_boneTransform;
    functions.bs_bonePosition = _preval_bs_bonePosition;
    functions.bs_transformBone = _preval_bs_transformBone;
    functions.bs_blendPose = _preval_bs_blendPose;
    functions.bs_bone = _preval_bs_bone;
    functions.bs_fabrik = _preval_bs_fabrik;
    functions.bs_bindPose = _preval_bs_bindPose;
    functions.bs_keyframePosition = _preval_bs_keyframePosition;
    functions.bs_keyframeRotation = _preval_bs_keyframeRotation;
    functions.bs_keyframeScale = _preval_bs_keyframeScale;
    functions.bs_loadAnimation = _preval_bs_loadAnimation;
    functions.bs_queryBoneId = _preval_bs_queryBoneId;
    functions.bs_queryArmature = _preval_bs_queryArmature;
    functions.bs_queryBone = _preval_bs_queryBone;
    functions.bs_queryMesh = _preval_bs_queryMesh;
    functions.bs_queryMeshHash = _preval_bs_queryMeshHash;
    functions.bs_queryMaterial = _preval_bs_queryMaterial;
    functions.bs_idName = _preval_bs_idName;
    functions.bs_object = _preval_bs_object;
    functions.bs_packages = _preval_bs_packages;
    functions.bs_objectSources = _preval_bs_objectSources;
    functions.bs_destroyResource = _preval_bs_destroyResource;
    functions.bs_queryResource = _preval_bs_queryResource;
    functions.bs_queryPackage = _preval_bs_queryPackage;
    functions.bs_loadResource = _preval_bs_loadResource;
    functions.bs_loadPackage = _preval_bs_loadPackage;
    functions.bs_configureSource = _preval_bs_configureSource;
    functions.bs_exists = _preval_bs_exists;
    functions.bs_fetch = _preval_bs_fetch;
    functions.bs_shouldLoadId = _preval_bs_shouldLoadId;
    functions.bs_shader = _preval_bs_shader;
    functions.bs_destroyShader = _preval_bs_destroyShader;
    functions.bs_computePipeline = _preval_bs_computePipeline;
    functions.bs_destroyComputePipeline = _preval_bs_destroyComputePipeline;
    functions.bs_queryPipeline = _preval_bs_queryPipeline;
    functions.bs_pipelineHash = _preval_bs_pipelineHash;
    functions.bs_pipeline = _preval_bs_pipeline;
    functions.bs_destroyPipeline = _preval_bs_destroyPipeline;
    functions.bs_pushConstant = _preval_bs_pushConstant;
    functions.bs_rayTracingPipeline = _preval_bs_rayTracingPipeline;
    functions.bs_loadBindings = _preval_bs_loadBindings;
    functions.bs_binding = _preval_bs_binding;
    functions.bs_bindImage = _preval_bs_bindImage;
    functions.bs_bindImages = _preval_bs_bindImages;
    functions.bs_bindBuffer = _preval_bs_bindBuffer;
    functions.bs_bindBuffers = _preval_bs_bindBuffers;
    functions.bs_bindAccelerationStructure = _preval_bs_bindAccelerationStructure;
    functions.bs_bindAccelerationStructures = _preval_bs_bindAccelerationStructures;
    functions.bs_pushDescriptors = _preval_bs_pushDescriptors;
    functions.bs_pushBindings = _preval_bs_pushBindings;
    functions.bs_queryBindSet = _preval_bs_queryBindSet;
    functions.bs_queryBinding = _preval_bs_queryBinding;
    functions.bs_configureAttribute = _preval_bs_configureAttribute;
    functions.bs_wnd = _preval_bs_wnd;
    functions.bs_io = _preval_bs_io;
    functions.bs_timeZoneBias = _preval_bs_timeZoneBias;
    functions.bs_dateTime = _preval_bs_dateTime;
    functions.bs_totalSeconds = _preval_bs_totalSeconds;
    functions.bs_isLaterThan = _preval_bs_isLaterThan;
    functions.bs_cursorPosition = _preval_bs_cursorPosition;
    functions.bs_windowPosition = _preval_bs_windowPosition;
    functions.bs_screenCursorPosition = _preval_bs_screenCursorPosition;
    functions.bs_lockCursorPosition = _preval_bs_lockCursorPosition;
    functions.bs_disableUserInputs = _preval_bs_disableUserInputs;
    functions.bs_middleClick = _preval_bs_middleClick;
    functions.bs_middleClickOnce = _preval_bs_middleClickOnce;
    functions.bs_middleClickUpOnce = _preval_bs_middleClickUpOnce;
    functions.bs_leftClick = _preval_bs_leftClick;
    functions.bs_rightClick = _preval_bs_rightClick;
    functions.bs_rightClickOnce = _preval_bs_rightClickOnce;
    functions.bs_leftClickOnce = _preval_bs_leftClickOnce;
    functions.bs_rightClickUpOnce = _preval_bs_rightClickUpOnce;
    functions.bs_leftClickUpOnce = _preval_bs_leftClickUpOnce;
    functions.bs_keyDown = _preval_bs_keyDown;
    functions.bs_keyHeld = _preval_bs_keyHeld;
    functions.bs_keyDownOnce = _preval_bs_keyDownOnce;
    functions.bs_keyUpOnce = _preval_bs_keyUpOnce;
    functions.bs_charDown = _preval_bs_charDown;
    functions.bs_charDownOnce = _preval_bs_charDownOnce;
    functions.bs_charUpOnce = _preval_bs_charUpOnce;
    functions.bs_scroll = _preval_bs_scroll;
    functions.bs_resizeWindow = _preval_bs_resizeWindow;
    functions.bs_screenDimensions = _preval_bs_screenDimensions;
    functions.bs_moveWindow = _preval_bs_moveWindow;
    functions.bs_window = _preval_bs_window;
    functions.bs_tick = _preval_bs_tick;
    functions.bs_exit = _preval_bs_exit;
    functions.bs_setCursor = _preval_bs_setCursor;
    functions.bs_maximize = _preval_bs_maximize;
    functions.bs_minimize = _preval_bs_minimize;
    functions.bs_deltaTime = _preval_bs_deltaTime;
    functions.bs_pause = _preval_bs_pause;
    functions.bs_advance = _preval_bs_advance;
    functions.bs_elapsedTime = _preval_bs_elapsedTime;
    functions.bs_resolution = _preval_bs_resolution;
    functions.bs_titleWindow = _preval_bs_titleWindow;
    functions.bs_inFixedTick = _preval_bs_inFixedTick;
    functions.bs_setTargetFramerate = _preval_bs_setTargetFramerate;
    functions.bs_timer = _preval_bs_timer;
    functions.bs_checkTimer = _preval_bs_checkTimer;
    functions.bs_copyToClipboard = _preval_bs_copyToClipboard;
    functions.bs_appendString = _preval_bs_appendString;
    functions.bs_foreachFile = _preval_bs_foreachFile;
    functions.bs_foreachDirectory = _preval_bs_foreachDirectory;
    functions.bs_numFiles = _preval_bs_numFiles;
    functions.bs_numDirectories = _preval_bs_numDirectories;
    functions.bs_loadFile = _preval_bs_loadFile;
    functions.bs_loadFileChunk = _preval_bs_loadFileChunk;
    functions.bs_deleteFile = _preval_bs_deleteFile;
    functions.bs_deleteDirectoryContents = _preval_bs_deleteDirectoryContents;
    functions.bs_deleteDirectory = _preval_bs_deleteDirectory;
    functions.bs_serializeJsonType = _preval_bs_serializeJsonType;
    functions.bs_serializeShaderType = _preval_bs_serializeShaderType;
    functions.bs_deserializeShaderType = _preval_bs_deserializeShaderType;
    functions.bs_serializeBindType = _preval_bs_serializeBindType;
    functions.bs_deserializeBindType = _preval_bs_deserializeBindType;

    return functions;
}

