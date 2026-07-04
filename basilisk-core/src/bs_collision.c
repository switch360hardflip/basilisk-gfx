#include <basilisk-core.h>
#include <bs_internal.h>
#include <float.h>
#include <assert.h>

BSAPI bool _bs_rectangleVsPoint(bs_vec2 position, bs_vec2 dimensions, bs_vec2 point) {
    return
        position.x < point.x &&
        point.x < (position.x + dimensions.x) &&
        position.y < point.y &&
        point.y < (position.y + dimensions.y);
}

BSAPI bool _bs_rectangleVsPointAbs(bs_vec2 position, bs_vec2 dimensions, bs_vec2 point) {
    float min_x = (dimensions.x >= 0) ? position.x : position.x + dimensions.x;
    float max_x = (dimensions.x >= 0) ? position.x + dimensions.x : position.x;
    float min_y = (dimensions.y >= 0) ? position.y : position.y + dimensions.y;
    float max_y = (dimensions.y >= 0) ? position.y + dimensions.y : position.y;

    return point.x >= min_x && point.x <= max_x &&
        point.y >= min_y && point.y <= max_y;
}

BSAPI bs_Ray _bs_ray(bs_vec3 start, bs_vec3 direction, float length) {
    return (bs_Ray) {
        .origin = start,
        .direction = direction,
        .length = length,
    };
}

/*
    Fast Ray-Box Intersection
    by Andrew Woo
    from "Graphics Gems", Academic Press, 1990
*/

BSAPI bool _bs_rayVsObb(const bs_Ray* ray, bs_vec3 position, bs_vec4 rotation, bs_vec3 scale, bs_vec3* coordinate, bs_vec3* normal, int* plane) {
    char inside = true;
    char quadrant[3];
    register int i;
    int which_plane;
    double max_t[3];
    bs_vec3 candidate_plane = { 0.0 };

    const int RIGHT = 0, LEFT = 1, MIDDLE = 2;

    bs_mat4 transform = BS_MAT4_IDENTITY;
    bs_mat3 rotation_matrix;
    bs_mat3 rotation_matrix_inverse;

    bs_m4Translate(&transform, &position, &transform);
    bs_m4Rotate(&transform, &rotation, &transform);

    bs_m3FromQ(&rotation, &rotation_matrix);
    bs_m3Inverse(&rotation_matrix, &rotation_matrix_inverse);

    bs_vec3 origin;
    bs_vec3 direction;
    bs_vec3 min, max;

    bs_vec3 s;
    bs_v3Sub(&ray->origin, &position, &s);

    bs_m3MulV3(&rotation_matrix_inverse, &s, &origin);
    bs_m3MulV3(&rotation_matrix_inverse, &ray->direction, &direction);

    bs_v3MulV1(&scale, -1.0, &min);
    max = scale;

    // find candidate planes; this loop can be avoided if
    // rays cast all from the eye(assume perpsective view)
    for (i = 0; i < 3; i++) {
        if (origin.a[i] < min.a[i]) {
            quadrant[i] = LEFT;
            candidate_plane.a[i] = min.a[i];
            inside = false;
        }
        else if (origin.a[i] > max.a[i]) {
            quadrant[i] = RIGHT;
            candidate_plane.a[i] = max.a[i];
            inside = false;
        }
        else
            quadrant[i] = MIDDLE;
    }

    // ray origin inside bounding box
    if (inside) {
        if (coordinate)
            *coordinate = ray->origin;

        if (normal)
            bs_v3MulV1(&ray->direction, -1.0, normal);

        return true;
    }

    // calculate T distances to candidate planes
    for (i = 0; i < 3; i++) {
        if (quadrant[i] != MIDDLE && direction.a[i] != 0.0)
            max_t[i] = (candidate_plane.a[i] - origin.a[i]) / direction.a[i];
        else
            max_t[i] = -1.0;
    }

    // get largest of the maxT's for final choice of intersection
    which_plane = 0;
    for (i = 1; i < 3; i++) {
        if (max_t[which_plane] < max_t[i])
            which_plane = i;
    }

    if (normal) {
        bs_vec3 normals[3] = { bs_v3(1, 0, 0), bs_v3(0, 1, 0), bs_v3(0, 0, 1) };
        *normal = normals[which_plane];
        *normal = bs_m3MulV3(rotation_matrix, *normal);

        if (quadrant[which_plane] == LEFT)
            bs_v3MulV1(normal, -1.0, normal);
        bs_v3Normalize(normal);
    }

    bs_vec3 coord;
    for (i = 0; i < 3; i++) {
        if (which_plane != i) {
            coord.a[i] = origin.a[i] + max_t[which_plane] * direction.a[i];
            if (coord.a[i] < min.a[i] || coord.a[i] > max.a[i])
                return false;
        }
        else
            coord.a[i] = candidate_plane.a[i];
    }

    if (coordinate)
        *coordinate = bs_m4MulV3(transform, coord);

    if (plane)
        *plane = which_plane;

    return true;
}

BSAPI bool _bs_sphereVsPoint(bs_vec3 center, float radius, bs_vec3 point) {
    int x1 = pow((point.x - center.x), 2);
    int y1 = pow((point.y - center.y), 2);
    int z1 = pow((point.z - center.z), 2);

    int dist = (x1 + y1 + z1);

    if (dist <= (radius * radius))
        return true;
    return false;
}

BSAPI bool _bs_sphereVsBox(bs_vec3 center, float radius, bs_vec3 position, bs_vec4 rotation, bs_vec3 scale, bs_vec3* point, bs_vec3* normal, float* penetration) {
    bs_mat4 transform = BS_MAT4_IDENTITY;
    bs_translateP(&transform, &position);
    bs_rotateP(&transform, &rotation);

    bs_vec3 relative_center = bs_m4MulV3(bs_m4Inverse(transform), center);

    if (bs_abs(relative_center.x) - radius > scale.x ||
        bs_abs(relative_center.y) - radius > scale.y ||
        bs_abs(relative_center.z) - radius > scale.z)
        return false;

    bs_vec3 closest_point = bs_v3V1(0.0);
    float distance;

    distance = relative_center.x;
    if (distance > scale.x) distance = scale.x;
    if (distance < -scale.x) distance = -scale.x;
    closest_point.x = distance;

    distance = relative_center.y;
    if (distance > scale.y) distance = scale.y;
    if (distance < -scale.y) distance = -scale.y;
    closest_point.y = distance;

    distance = relative_center.z;
    if (distance > scale.z) distance = scale.z;
    if (distance < -scale.z) distance = -scale.z;
    closest_point.z = distance;

    distance = bs_v3MagnitudeSqrd(bs_v3Sub(closest_point, relative_center));
    if (distance > radius * radius) return false;

    if (point) {
        *point = bs_m4MulV3(transform, closest_point);
        if (normal)
            *normal = bs_v3Normalize(bs_v3Sub(center, *point));
    }
    if (penetration)
        *penetration = radius - bs_sqrt(distance);

    return true;
}

BSAPI bool _bs_sphereVsSphere(bs_Sphere* a, bs_Sphere* b, bs_Contact* result) {
    bs_vec3 mid = bs_v3Sub(a->center, b->center);
    float magnitude = bs_v3Magnitude(mid);

    if (magnitude <= 0.0 || magnitude >= (a->radius + b->radius))
        return false;

    result->normal = bs_v3MulV1(mid, 1.0 / magnitude);
    result->point = bs_v3Add(a->center, bs_v3MulV1(mid, 0.5));
    result->penetration = a->radius + b->radius - magnitude;

    return true;
}

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

    bs_vec2 result;
    if (denom == 0.0) {
        result = bs_v2(NAN, NAN);

        if (out)
            *out = result;

        return false;
    }

    result.x = xnom / denom;
    result.y = ynom / denom;

    if (out)
        *out = result;

    if (!isfinite(result.x) || !isfinite(result.y))
        return false;

    return true;
}
