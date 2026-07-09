
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
#include <math.h>



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
