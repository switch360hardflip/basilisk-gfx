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

#define BS_V2_DEGREES(v) BS_V2(bs_degrees(v.x), bs_degrees(v.y))
#define BS_V3_DEGREES(v) BS_V3(bs_degrees(v.x), bs_degrees(v.y), bs_degrees(v.z))

#define BS_V2_RADIANS(v) BS_V2(bs_radians(v.x), bs_radians(v.y))
#define BS_V3_RADIANS(v) BS_V3(bs_radians(v.x), bs_radians(v.y), bs_radians(v.z))

#endif 