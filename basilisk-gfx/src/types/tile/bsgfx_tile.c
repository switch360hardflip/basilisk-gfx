#include <types/tile/bsgfx_tile.h>
#include <types/primitive/bsgfx_primitive.h>
#include <../bsgfx_contracts.h>
#include <bsgfx.h>



  /*==============================================================================
   * Helpers
   *============================================================================*/

static const bsgfx_TileAxis bsgfx_tile_axes[6] = {
    {
        .normal = { -1,  0,  0 },
        .right = {  0,  0,  1 },
        .up = {  0,  1,  0 },
        .start_sign = { -1, -1, -1 },
        .width_axis = 2,
        .height_axis = 1
    },
    {
        .normal = {  1,  0,  0 },
        .right = {  0,  0, -1 },
        .up = {  0,  1,  0 },
        .start_sign = {  1, -1,  1 },
        .width_axis = 2,
        .height_axis = 1
    },
    {
        .normal = {  0, -1,  0 },
        .right = {  1,  0,  0 },
        .up = {  0,  0,  1 },
        .start_sign = { -1, -1, -1 },
        .width_axis = 0,
        .height_axis = 2
    },
    {
        .normal = {  0,  1,  0 },
        .right = {  1,  0,  0 },
        .up = {  0,  0, -1 },
        .start_sign = { -1,  1,  1 },
        .width_axis = 0,
        .height_axis = 2
    },
    {
        .normal = {  0,  0, -1 },
        .right = { -1,  0,  0 },
        .up = {  0,  1,  0 },
        .start_sign = {  1, -1, -1 },
        .width_axis = 0,
        .height_axis = 1
    },
    {
        .normal = {  0,  0,  1 },
        .right = {  1,  0,  0 },
        .up = {  0,  1,  0 },
        .start_sign = { -1, -1,  1 },
        .width_axis = 0,
        .height_axis = 1
    }
};

const bsgfx_TileAxis* bsgfx_tileAxes() {
    return bsgfx_tile_axes;
}

bs_ivec2 bsgfx_tileCoordinate(
    bsgfx_Primitive* primitive,
    int axis,
    int index)
{
    int local = index - primitive->first_tile;
    if (local < 0)
        return bs_iv2(BS_I32_MAX, BS_I32_MAX);

    for (int a = 0; a < axis; a++) {
        bsgfx_TileAxis* ax_prev = &bsgfx_tile_axes[a];

        int w_prev = (int)(primitive->scale.a[ax_prev->width_axis] * 2.0f);
        int h_prev = (int)(primitive->scale.a[ax_prev->height_axis] * 2.0f);

        local -= (w_prev * h_prev);
    }

    bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

    int width = (int)(primitive->scale.a[ax->width_axis] * 2.0f);

    return bs_iv2(local % width, local / width);
}

int bsgfx_tileAxis(bsgfx_Primitive* primitive, int index) {
    int local = index - primitive->first_tile;
    if (local < 0)
        return -1;

    for (int axis = 0; axis < 6; axis++) {
        bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

        int width = (int)(primitive->scale.a[ax->width_axis] * 2.0f);
        int height = (int)(primitive->scale.a[ax->height_axis] * 2.0f);

        int count = width * height;

        if (local < count)
            return axis;

        local -= count;
    }

    return -1;
}

bs_U32 bsgfx_tileIndex(
    bsgfx_Primitive* primitive,
    int axis,
    int x,
    int y)
{
    if (axis >= 6)
        return primitive->first_tile;

    float sx = primitive->scale.x;
    float sy = primitive->scale.y;
    float sz = primitive->scale.z;

    bs_vec3 scale_vec = { sx, sy, sz };

    bs_U32 index = primitive->first_tile;

    for (int a = 0; a < axis; a++) {
        bsgfx_TileAxis* ax_prev = &bsgfx_tile_axes[a];

        int w_prev = (int)(scale_vec.a[ax_prev->width_axis] * 2.0f);
        int h_prev = (int)(scale_vec.a[ax_prev->height_axis] * 2.0f);

        index += (w_prev * h_prev);
    }

    bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

    int width = (int)(scale_vec.a[ax->width_axis] * 2.0f);
    int height = (int)(scale_vec.a[ax->height_axis] * 2.0f);

    if (x < 0 || x >= width || y < 0 || y >= height)
        return index;

    index += (y * width + x);

    return index;
}

bs_vec3 bsgfx_tilePosition(bsgfx_Primitive* primitive, int axis, int x, int y) {
    bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

    bs_vec3 right = bs_qRotateVec3(&primitive->rotation, &ax->right);
    bs_vec3 up = bs_qRotateVec3(&primitive->rotation, &ax->up);

    bs_vec3 start = bs_qRotateVec3(&primitive->rotation, &(bs_vec3) {
        ax->start_sign.x * primitive->scale.x,
        ax->start_sign.y * primitive->scale.y,
        ax->start_sign.z * primitive->scale.z
    });

    bs_v3Add(&start, &primitive->position, &start);

    bs_vec3 tile_origin = start;
    bs_vec3 right_scaled;
    bs_v3MulV1(&right, (float)x, &right_scaled);
    bs_v3Add(&tile_origin, &right_scaled, &tile_origin);
    bs_vec3 up_scaled;
    bs_v3MulV1(&up, (float)y, &up_scaled);
    bs_v3Add(&tile_origin, &up_scaled, &tile_origin);

    return tile_origin;
}

bs_vec4 bsgfx_tileRotation(int axis) {
    bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

    bs_vec3 right = ax->right;
    bs_vec3 up = ax->normal;
    bs_vec3 forward;
    bs_v3Cross(&right, &up, &forward);

    bs_mat3 m = {
        right.x, up.x, forward.x,
        right.y, up.y, forward.y,
        right.z, up.z, forward.z
    };

    return bs_qFromMat3(m);
}

bs_vec3 bsgfx_tileEulerRotation(int axis) {
    switch (axis) {
    case 0: return bs_v3(0, 90, 0);
    case 1: return bs_v3(0, -90, 0);
    case 2: return bs_v3(90, 0, 0);
    case 3: return bs_v3(0, 0, 0);
    case 4: return bs_v3(0, 90, 0);
    case 5: return bs_v3(0, 0, 0);
    }

    return bs_v3(0, 0, 0);
}



  /*==============================================================================
   * Batching
   *============================================================================*/

void bsgfx_batchTile(bs_Batch* batch, bs_U32* offset, bs_Quad quad, bs_vec3 normal, bs_U32 index, int image_index) {
    BS_VERTEX_DECLARATION(
        declaration, batch, offset,
        bs_vec3, bs_Position,
        bs_vec3, bs_Normal,
        bs_vec3, bs_Texture,
        uint, bsgfx_Index
    );

    bs_batchVertex(&declaration, &(bs_Vertex) { quad.a, normal, bs_v3V2(quad.ca, image_index), index });
    bs_batchVertex(&declaration, &(bs_Vertex) { quad.b, normal, bs_v3V2(quad.cb, image_index), index });
    bs_batchVertex(&declaration, &(bs_Vertex) { quad.c, normal, bs_v3V2(quad.cc, image_index), index });
    bs_batchVertex(&declaration, &(bs_Vertex) { quad.d, normal, bs_v3V2(quad.cd, image_index), index });
}

bs_Range bsgfx_pushTile(bs_Batch* batch, bs_Quad quad, bs_vec3 normal, bs_U32 index, int image_index) {
    int index_offset = batch->indices.count;

    bs_ensureBatchSize(batch, 6, 4);
    bs_pushIndexV(batch, 6, 1, 2, 0, 2, 1, 3);
    bsgfx_batchTile(batch, &batch->vertices.count, quad, normal, index, image_index);

    return bs_batchRange(batch, index_offset);
}

bs_U32 bsgfx_pushTileAt(
    bs_Batch* batch,
    bsgfx_Primitive* primitive,
    int axis,
    int x,
    int y,
    bs_U32 index,
    int image_index)
{
    bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

    bs_vec3 right = bs_qRotateVec3(&primitive->rotation, &ax->right);
    bs_vec3 up = bs_qRotateVec3(&primitive->rotation, &ax->up);
    bs_vec3 normal = bs_qRotateVec3(&primitive->rotation, &ax->normal);

    bs_vec3 start = bs_qRotateVec3(&primitive->rotation, &(bs_vec3) {
        ax->start_sign.x * primitive->scale.x,
        ax->start_sign.y * primitive->scale.y,
        ax->start_sign.z * primitive->scale.z
    });

    bs_v3Add(&start, &primitive->position, &start);

    bs_vec3 tile_origin = start;
    bs_vec3 right_scaled;
    bs_v3MulV1(&right, (float)x, &right_scaled);
    bs_v3Add(&tile_origin, &right_scaled, &tile_origin);
    bs_vec3 up_scaled;
    bs_v3MulV1(&up, (float)y, &up_scaled);
    bs_v3Add(&tile_origin, &up_scaled, &tile_origin);

    bs_vec3 a = tile_origin;
    bs_vec3 b;
    bs_v3Add(&a, &right, &b);
    bs_vec3 c;
    bs_v3Add(&a, &up, &c);
    bs_vec3 d;
    bs_v3Add(&c, &right, &d);

    bs_Quad quad = {
        .a = a, .b = b, .c = c, .d = d,
        .ca = { 0, 0 },
        .cb = { 1, 0 },
        .cc = { 0, 1 },
        .cd = { 1, 1 },
    };

    bs_U32 idx = bsgfx_index24(index, axis);
    bsgfx_pushTile(batch, quad, normal, idx, image_index);

    return index + 1;
}

static bs_U32 bsgfx_pushTileAxis(
    bs_Batch* batch,
    bsgfx_Primitive* primitive,
    int axis,
    bs_U32 index)
{
    bsgfx_TileAxis* ax = &bsgfx_tile_axes[axis];

    float sx = primitive->scale.x;
    float sy = primitive->scale.y;
    float sz = primitive->scale.z;

    bs_vec3 scale = { sx, sy, sz };

    int width = (int)(scale.a[ax->width_axis] * 2.0f);
    int height = (int)(scale.a[ax->height_axis] * 2.0f);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            index = bsgfx_pushTileAt(batch, primitive, axis, x, y, index, 0);
    }

    return index;
}



  /*==============================================================================
   * Loading Tile Resources
   *============================================================================*/

static void bsgfx_loadTileTextures(int package_id) {
    if (bsgfx_count(BSGFX_TYPE_PRIMITIVE) == 0) // todo better check
        return;

    bs_Object* tile_image = bs_loadImage(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_TILE, BS_OBJECT_FORCE_DESTROY), package_id, "textureArrays/tiles", BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_DST_BIT);
    if (!tile_image)
        return;

    // white image buffer
    bs_Buffer* staging_buffer = bs_buffer(BS_BUFFER(-1, 0, 0), BSGFX_TILE_SIZE.x * BSGFX_TILE_SIZE.y * 4,
        BS_BUFFER_USAGE_TRANSFER_SRC_BIT,
        BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0)->buffer;

    bs_mapBuffer(staging_buffer, BS_U32_MAX);

   // bs_foreachFile(bsgfx_loadTileTexture, &offset, "textures/tiles/");

    bs_bindImages(BSGFX_SET_34_24, BSGFX_BINDING_34_24, &(bs_ImageDescriptor) {
        .image = tile_image->image,
        .sampler = bs_fetch(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)->sampler,
        .layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    }, 1);

    bs_destroyBuffer(staging_buffer);

    bs_infoF("Loaded tile textures\n");
}

static void bsgfx_mapTile(const bsgfx_RawTile* unmapped, bsgfx_Tile* mapped) {
    const int tile_count = bsgfx_count(BSGFX_TYPE_TILE);

    bs_Image* image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_TILE)->image;

    *mapped = (bsgfx_Tile){
        .coords = unmapped->coords,
        .flags = unmapped->flags,
        .index = 0,
        .axis = unmapped->axis,
        .image_index = bs_queryImageIndexHash(image, unmapped->texture_hash, NULL),
        // .primitive = unmapped->primitive,
        // .tile_type = unmapped->tile_type,
    };

    bs_except(BSX_FAILED_TO_QUERY);
    int primitive_id = bsgfx_queryPrimitive(&unmapped->primitive);
    if (!bs_caught()) {
        bsgfx_RawPrimitive* raw_primitive = bsgfx_getRaw(BSGFX_TYPE_PRIMITIVE, primitive_id);
        bsgfx_RawPrimitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, primitive_id);
        mapped->index = bsgfx_tileIndex(primitive, mapped->axis, mapped->coords.x, mapped->coords.y);
    }
}

void bsgfx_loadTiles(int package_id, bool force_destroy) {
    bsgfx_loadTileTextures(package_id);
    bs_except(BSX_FAILED_TO_QUERY);

    bsgfx_type(
        BSGFX_TYPE_TILE,
        package_id,
        BSGFX_TILE_VERSION,
        "tiles", "tile",
        sizeof(bsgfx_RawTile), sizeof(bsgfx_Tile), bsgfx_mapTile,
        0, 0, 0, 0);
    bs_except(0);

    bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_PRIMITIVE_TILES, force_destroy ? BS_OBJECT_FORCE_DESTROY : 0), sizeof(bs_U32), $vs_bsgfx_tile_static(), 0)->batch;
    bs_except(BSX_NOT_FOUND);
    bs_Batch* batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_PRIMITIVE_TILES)->batch;
    if (bs_except(0))
        return;
    if (bs_batchIsPushed(batch))
        bs_unpushBatch(batch);

    int red_material = $red_material()->id;

    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_TILE); i++) {
        bsgfx_RawTile* raw_tile = bsgfx_getRaw(BSGFX_TYPE_TILE, i);
        bsgfx_Tile* tile = bsgfx_get(BSGFX_TYPE_TILE, i);
        int primitive_id = bsgfx_queryPrimitive(&raw_tile->primitive);
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, primitive_id);

        bsgfx_pushTileAt(batch, primitive, tile->axis, tile->coords.x, tile->coords.y, tile->index, tile->image_index);
    }

    /*
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);

        if (primitive->type != BSGFX_PRIMITIVE_TYPE_BOX)
            continue;

        primitive->first_tile = index;
        for (int j = 0; j < bsgfx_count(BSGFX_TYPE_TILE); j++) {
            bsgfx_RawTile* raw_tile = bsgfx_getRaw(BSGFX_TYPE_TILE, j);
            bsgfx_Tile* tile = bsgfx_get(BSGFX_TYPE_TILE, j);
            int tile_primitive_id = bsgfx_queryPrimitive(&raw_tile->primitive);
            if (tile_primitive_id != i)
                continue;

            bsgfx_pushTileAt(batch, primitive, tile->axis, tile->coords.x, tile->coords.y, tile->index, tile->image_index);
        }
        primitive->last_tile = index;
    }
    */

    bs_U32 index = 0;
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);

        if (primitive->type != BSGFX_PRIMITIVE_TYPE_BOX)
            continue;

        for (int j = 0; j < 6; j++) {
            index = bsgfx_pushTileAxis(batch, primitive, j, index);
        }
    }
    bs_pushBatch(batch, BS_U32_MAX, BS_U32_MAX);
}

void bsgfx_instanceTiles() {

}
