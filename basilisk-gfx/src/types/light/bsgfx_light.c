#include <types/light/bsgfx_light.h>
#include <types/bsgfx_type.h>

static void bsgfx_mapLight(const bsgfx_RawLight* unmapped, bsgfx_Light* mapped) {
    *mapped = (bsgfx_Light) {
        .position = unmapped->position,
        .type = unmapped->type,
    };

    bs_vec4 q;
    bs_eulToQ(&unmapped->rotation, &q);
    bs_qRotateV3(&q, &BS_V3(0, -1, 0), &mapped->direction);
}

void bsgfx_loadLights(int package_id) {
    bsgfx_type(
        BSGFX_TYPE_LIGHT,
        package_id,
        BSGFX_LIGHT_VERSION,
        "lights", "light",
        sizeof(bsgfx_RawLight), sizeof(bsgfx_Light), bsgfx_mapLight,
        0, 0, 0, 0);
}