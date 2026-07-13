#include <types/spawner/bsgfx_spawner.h>
#include <types/primitive/bsgfx_primitive.h>

#include <spawners/item/bsgfx_item.h>
#include <spawners/entity/bsgfx_entity.h>

#include <bsgfx_material.h>

bs_vec3 bsgfx_spawnPosition(bs_vec2 coords, float y) {
    return (bs_vec3) {
        .x = 2.0 * coords.x + 1.0,
        .z = 2.0 * (-coords.y - 1.0) + y + 1.0,
        .y = y,
    };
}

static void bsgfx_mapSpawner(bsgfx_RawSpawner* unmapped, bsgfx_Spawner* mapped) {
    *mapped = (bsgfx_Spawner){
        .position = unmapped->position,
        .spawn_type = unmapped->spawn_type,
        .spawn_name = unmapped->spawn_name,
        .flags = unmapped->flags,
        .spawn_periodicity = unmapped->spawn_periodicity,
        .material_id = unmapped->material_name ? bsgfx_queryMaterialHash(unmapped->material_name)->id : 0,
    };
}

// static void bsgfx_onSelectSpawner(int id) {
//     bsgfx_deselectAll();
// }

void bsgfx_loadSpawners(int package_id) {
    bs_except(BSX_FAILED_TO_QUERY);
    bsgfx_type(
        BSGFX_TYPE_SPAWNER, 
        package_id, 
        BSGFX_SPAWNER_VERSION,
        "spawners", "spawner",
        sizeof(bsgfx_RawSpawner), sizeof(bsgfx_Spawner), 
        bsgfx_mapSpawner, 
        0, 
        0, 
        0, 
        0);
    if (bs_caught())
        return;
   // bsgfx_onSelect(BSGFX_TYPE_SPAWNER, bsgfx_onSelectSpawner);
}

void bsgfx_spawn(bsgfx_Spawner* spawner) {
    return;
    switch (spawner->spawn_type) {
    case BSGFX_SPAWNER_ITEM:
        bs_throwBasilisk(BSX_NOT_IMPLEMENTED);
        /*
        bs_Model* model = $items();
        bs_Mesh* mesh = bs_queryMeshHash(model, spawner->spawn_name, NULL);
        bsgfx_spawnItem(mesh - model->meshes, bs_v3AddY(spawner->position, 2.0), 10.0, spawner->material_id);
        */
        break;
    case BSGFX_SPAWNER_ENTITY:
        bs_throwBasilisk(BSX_NOT_IMPLEMENTED);

       //bsgfx_Script* script = bs_queryHash(NULL, "lua", spawner->spawn_name, 0).model;
       //bsgfx_spawnEntity(script, spawner->position);
        break;
    }
}

/*
struct bsgfx_SpawnerMenuParams {
    int id;
    int clicked_spawner;
};

static void bsgfx_onClickSpawn(bool confirm, struct bsgfx_SpawnerMenuParams* params) {
    if (!confirm) return;

    bsgfx_Spawner* spawner = bsgfx_get(BSGFX_TYPE_SPAWNER, params->clicked_spawner);
    bsgfx_spawn(spawner);
}

static void bsgfx_onClickDeleteSpawner(bool confirm, struct bsgfx_SpawnerMenuParams* params) {
    if (!confirm) return;

    bsmod_delete(BSGFX_TYPE_SPAWNER, params->clicked_spawner);
}

static bool bsgfx_instanceSpawner(bs_vec3 position, int id, const char* material) {
    bsgfx_meshInstance(_bsgfx_subtypes_[BSGFX_SUBTYPE_BIPYRAMID], &(bsgfx_MeshInstance) {
        .transform = bs_rotate(bs_translate(BS_MAT4_IDENTITY, bs_v3AddY(position, 1.0)), bs_qFromDegreesV3(0.0, bs_elapsedTime() * 60.0, 0.0))
    }, 0, id, bsgfx_queryMaterial(material)->id);

    return _poser_->hovering.subtype == _bsgfx_subtypes_[BSGFX_SUBTYPE_BIPYRAMID] && _poser_->hovering.instance_id == id;
}

void bsgfx_instanceSpawners() {
    if (!_bsgfx_settings_.draw_spawners)
        return;

    const float rotation_speed = 30.0;
    bs_vec4 rotation = bs_qFromDegreesV3(0.0, bs_elapsedTime() * rotation_speed, 0.0);

    static struct bsgfx_SpawnerMenuParams params;

    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_SPAWNER); i++) {
        bsgfx_Spawner* spawner = bsgfx_get(BSGFX_TYPE_SPAWNER, i);

        char* name = NULL;
        int id = -1;
        bool hovering = false;
        switch (spawner->spawn_type) {
        case BSGFX_SPAWNER_ITEM:
            bs_Model* model = $items();
            bs_Mesh* mesh = bs_queryMeshHash(model, spawner->spawn_name, NULL);

            name = mesh->name;
           // hovering = bsgfx_instanceItemPreview(BSGFX_ITEM_SUBTYPE, spawner->position, mesh->name_hash, i);
            id = mesh - model->meshes;
            // bsgfx_obbInstance(&mesh->aabb, BS_BLACK, &transform);
            hovering = bsgfx_instanceSpawner(spawner->position, i, "red");
            break;
        case BSGFX_SPAWNER_ENTITY: 
            bsgfx_Script* script = bs_queryHash(NULL, "lua", spawner->spawn_name, 0).data;
            bsgfx_Script* scripts = bs_resource(BSGFX_RESOURCE_SCRIPT, 0);

            name = script->name;

           // hovering = bsgfx_instanceScriptPreview(spawner->position, script->name, false);
            id = script - scripts;
            hovering = bsgfx_instanceSpawner(spawner->position, i, "green");
            break;
        }
        
        if (hovering && !_poser_->menu_blocked) {
            //if (!bsgfx_quickMenuEnabled())
            //    bsgfx_instanceHint(bs_windowCursorPosition(), name);

            if (bs_leftClickOnce()) {
                bsmod_select(BSGFX_TYPE_SPAWNER, i);
            }

            if (bs_rightClickOnce()) {
                params = (struct bsgfx_SpawnerMenuParams){
                    .clicked_spawner = i,
                    .id = id,
                };

                bsgfx_enableRightClickMenu((bsgfx_RightClickMenu) {
                    .buttons = {
                        {
                            .name = "spawn",
                            .on_click = bsgfx_onClickSpawn,
                        },
                        {
                            .name = "delete",
                            .on_click = bsgfx_onClickDeleteSpawner,
                            .confirm = true,
                        }
                    }
                }, &params);
            }
        }

    }
}
*/