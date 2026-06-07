#include <basilisk.h>
#include <bs_internal.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>
#include <math.h>



  /*==============================================================================
   * bs_vec2
   =============================================================================*/

void bs_v2(float x, float y, bs_vec2* out) {
    glm_vec2_copy((float[2]){x, y}, out->a);
}

void bs_v2V1(float s, bs_vec2* out) {
    glm_vec2_fill(out->a, s);
}

void bs_v2Iv2(bs_ivec2* v, bs_vec2* out) {
    out->x = (float)v->x;
    out->y = (float)v->y;
}

void bs_v2Add(bs_vec2* a, bs_vec2* b, bs_vec2* out) {
    glm_vec2_add(a->a, b->a, out->a);
}

void bs_v2Sub(bs_vec2* a, bs_vec2* b, bs_vec2* out) {
    glm_vec2_sub(a->a, b->a, out->a);
}

void bs_v2Mul(bs_vec2* a, bs_vec2* b, bs_vec2* out) {
    glm_vec2_mul(a->a, b->a, out->a);
}

void bs_v2Div(bs_vec2* a, bs_vec2* b, bs_vec2* out) {
    glm_vec2_div(a->a, b->a, out->a);
}

void bs_v2MulV1(bs_vec2* a, float s, bs_vec2* out) {
    glm_vec2_scale(a->a, s, out->a);
}

void bs_v2DivV1(bs_vec2* a, float s, bs_vec2* out) {
    glm_vec2_scale(a->a, 1.0f / s, out->a);
}

void bs_v2AddV1(bs_vec2* a, float s, bs_vec2* out) {
    glm_vec2_adds(a->a, s, out->a);
}

void bs_v2AddY(bs_vec2* a, float s, bs_vec2* out) {
    glm_vec2_copy(a->a, out->a);
    out->y += s;
}

float bs_v2Dot(bs_vec2* a, bs_vec2* b) {
    return glm_vec2_dot(a->a, b->a);
}

float bs_v2Magnitude(bs_vec2* a) {
    return glm_vec2_norm(a->a);
}

void bs_v2Normalize(bs_vec2* a, bs_vec2* out) {
    glm_vec2_normalize_to(a->a, out->a);
}

void bs_v2NormalizeV2(float x, float y, bs_vec2* out) {
    float len = sqrtf(x * x + y * y);
    out->x = x / len;
    out->y = y / len;
}



  /*==============================================================================
   * bs_vec3
   =============================================================================*/

void bs_v3(float x, float y, float z, bs_vec3* out) {
    glm_vec3_copy((float[3]){x, y, z}, out->a);
}

void bs_v3V1(float s, bs_vec3* out) {
    glm_vec3_fill(out->a, s);
}

void bs_v3V2(bs_vec2* v, float z, bs_vec3* out) {
    out->x = v->x;
    out->y = v->y;
    out->z = z;
}

void bs_v3Add(bs_vec3* a, bs_vec3* b, bs_vec3* out) {
    glm_vec3_add(a->a, b->a, out->a);
}

void bs_v3Sub(bs_vec3* a, bs_vec3* b, bs_vec3* out) {
    glm_vec3_sub(a->a, b->a, out->a);
}

void bs_v3Mul(bs_vec3* a, bs_vec3* b, bs_vec3* out) {
    glm_vec3_mul(a->a, b->a, out->a);
}

void bs_v3MulV1(bs_vec3* a, float s, bs_vec3* out) {
    glm_vec3_scale(a->a, s, out->a);
}

void bs_v3Div(bs_vec3* a, bs_vec3* b, bs_vec3* out) {
    glm_vec3_div(a->a, b->a, out->a);
}

void bs_v3DivV1(bs_vec3* a, float s, bs_vec3* out) {
    glm_vec3_scale(a->a, 1.0f / s, out->a);
}

float bs_v3Dot(bs_vec3* a, bs_vec3* b) {
    return glm_vec3_dot(a->a, b->a);
}

void bs_v3Cross(bs_vec3* a, bs_vec3* b, bs_vec3* out) {
    glm_vec3_cross(a->a, b->a, out->a);
}

void bs_v3Normalize(bs_vec3* a, bs_vec3* out) {
    glm_vec3_normalize_to(a->a, out->a);
}

void bs_v3NormalizeV3(float x, float y, float z, bs_vec3* out) {
    float len = sqrtf(x * x + y * y + z * z);
    out->x = x / len;
    out->y = y / len;
    out->z = z / len;
}

float bs_v3Magnitude(bs_vec3* a) {
    return glm_vec3_norm(a->a);
}

float bs_v3MagnitudeSqrd(bs_vec3* a) {
    return glm_vec3_norm2(a->a);
}

float bs_v3Dist(bs_vec3* a, bs_vec3* b) {
    return glm_vec3_distance(a->a, b->a);
}

void bs_v3Lerp(bs_vec3* a, bs_vec3* b, float t, bs_vec3* out) {
    glm_vec3_lerp(a->a, b->a, t, out->a);
}



  /*==============================================================================
   * bs_vec4
   =============================================================================*/

void bs_v4(float x, float y, float z, float w, bs_vec4* out) {
    glm_vec4_copy((float[4]){x, y, z, w}, out->a);
}

void bs_v4V1(float s, bs_vec4* out) {
    glm_vec4_fill(out->a, s);
}

void bs_v4V3(bs_vec3* v, float w, bs_vec4* out) {
    out->x = v->x;
    out->y = v->y;
    out->z = v->z;
    out->w = w;
}

void bs_v4Add(bs_vec4* a, bs_vec4* b, bs_vec4* out) {
    glm_vec4_add(a->a, b->a, out->a);
}

void bs_v4Sub(bs_vec4* a, bs_vec4* b, bs_vec4* out) {
    glm_vec4_sub(a->a, b->a, out->a);
}

void bs_v4Mul(bs_vec4* a, bs_vec4* b, bs_vec4* out) {
    glm_vec4_mul(a->a, b->a, out->a);
}

void bs_v4MulV1(bs_vec4* a, float s, bs_vec4* out) {
    glm_vec4_scale(a->a, s, out->a);
}

void bs_v4Div(bs_vec4* a, bs_vec4* b, bs_vec4* out) {
    glm_vec4_div(a->a, b->a, out->a);
}

void bs_v4DivV1(bs_vec4* a, float s, bs_vec4* out) {
    glm_vec4_scale(a->a, 1.0f / s, out->a);
}



  /*==============================================================================
   * Lines
   =============================================================================*/

// https://gist.github.com/TimSC/47203a0f5f15293d2099507ba5da44e6
static inline double bs_determinate(float a, float b, float c, float d) {
    return a * d - b * c;
}

bool bs_lineVsLine(bs_vec2 l1_start, bs_vec2 l1_end, bs_vec2 l2_start, bs_vec2 l2_end, bs_vec2* out) {
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
