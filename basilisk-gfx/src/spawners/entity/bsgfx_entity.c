#include <bsgfx.h>
#include <bsgfx_collider.h>
#include <bsgfx_animator.h>

#include <spawners/entity/bsgfx_entity.h>

#include <objects/font/bsgfx_font.h>

#include <types/tile/bsgfx_tile.h>
#include <types/spawner/bsgfx_spawner.h>

#include <spawners/item/bsgfx_item.h>

/*
bs_List bsgfx_entities = { .unit_size = sizeof(bsgfx_Entity), .increment = 128 }; // todo this will implode the game after 128 entities due to dangling pointers

int bsgfx_queryEntityComponent(bsgfx_Entity* entity, const char* name) {
    for (int i = 0; i < BSGFX_COMPONENT_COUNT; i++) {
        if (strcmp(name, bsgfx_components[i].field_name) == 0)
            return i;

    }

    bs_throwBasilisk(BSX_FAILED_TO_QUERY);
    return -1;
}

int bsgfx_queryEntityField(bsgfx_Entity* entity, int component_id, const char* name) {
    for (int i = 0; i < bsgfx_components[component_id].count; i++) {
        if (strcmp(name, bsgfx_components[component_id].names[i]) == 0)
            return i;
    }

    bs_throwBasilisk(BSX_FAILED_TO_QUERY);
    return -1;
}

void bsgfx_readEntityComponent(bsgfx_Entity* entity, int component_id) {
    size_t component_offset = bsgfx_components[component_id].vk_offset;
    unsigned char** component = ((unsigned char*)entity) + component_offset;

    if (*component && bsgfx_enterField(bsgfx_components[component_id].field_name, -1)) {
        int field_count = *bsgfx_components[component_id].count;

        unsigned char* field = *component;
        for (int j = 0; j < field_count; j++) {
            const char* field_name = bsgfx_components[component_id].names[j];
            size_t field_size = bsgfx_components[component_id].sizes[j];
            bsgfx_LuaType field_type = bsgfx_components[component_id].types[j];

            bsgfx_LuaValue value = bsgfx_getValue(field_name, field_type, -1, NULL);
            if (bsgfx_luaIsPointerType(field_type)) {
                if (value.as_vec2)
                    memcpy(field, value.as_vec2, field_size);
            }
            else {
                if (field_type == BSGFX_LUA_NUMBER && field_size == sizeof(float)) {
                    float f = value.as_number;
                    memcpy(field, &f, field_size);
                }
                else
                    memcpy(field, &value.as_number, field_size);
            }

            field += bsgfx_components[component_id].sizes[j];
        }

        bsgfx_leaveField();
    }

}

void bsgfx_readEntityComponents(bsgfx_Entity* entity) {
    for (int i = 0; i < BSGFX_COMPONENT_COUNT; i++) {
        bsgfx_readEntityComponent(entity, i);
    }
}

void bsgfx_saveEntityComponent(bsgfx_Entity* entity, int component_id) {
    size_t component_offset = bsgfx_components[component_id].vk_offset;
    unsigned char** component = ((unsigned char*)entity) + component_offset;

    if (*component && bsgfx_enterField(bsgfx_components[component_id].field_name, -1)) {
        int field_count = *bsgfx_components[component_id].count;

        unsigned char* field = *component;
        for (int j = 0; j < field_count; j++) {
            const char* field_name = bsgfx_components[component_id].names[j];
            size_t field_size = bsgfx_components[component_id].sizes[j];
            bsgfx_LuaType field_type = bsgfx_components[component_id].types[j];

            bsgfx_pushSingle(field_type, field);
            duL_setField(-2, field_name);

            field += field_size;
        }

        bsgfx_leaveField();
    }
}

void bsgfx_saveEntityComponents(bsgfx_Entity* entity) {
    duL_set("id", entity->id, -2);

    for (int i = 0; i < BSGFX_COMPONENT_COUNT; i++) {
        bsgfx_saveEntityComponent(entity, i);
    }
}

static void bsgfx_allocateEntityFields(bsgfx_Entity* entity) {
    for (int i = 0; i < BSGFX_COMPONENT_COUNT; i++) {
        size_t component_offset = bsgfx_components[i].vk_offset;
        unsigned char** component = ((unsigned char*)entity) + component_offset;

        if (!*component && bsgfx_enterField(bsgfx_components[i].field_name, -1)) {
            *component = bs_calloc(1, bsgfx_components[i].size);
            bsgfx_leaveField();
        }
    }
}

void bsgfx_respawnEntity(bsgfx_Entity* entity, bs_vec3 position) {
    bsgfx_Script* script = entity->script;

    const char* old_table = bsgfx_currentTable();
    if (old_table) {
        old_table = strdup(old_table);
        bs_infoF("Temporarily leaving table \"%s\"\n", old_table);
        bsgfx_leaveTable();
    }

    if (bsgfx_enterTable(script->table)) {
        bs_infoF("Spawned entity with script \"%s\"\n", script->name);

        //duL_saveEntityData(entity);
        if (!bsgfx_call(script, "create", 0)) {
            entity->script->broken = true;
            bsgfx_leaveTable();
            return;
        }

        bsgfx_allocateEntityFields(entity);

        char* old_animation = NULL;
        if (entity->armature)
            old_animation = entity->armature->animation;

        bsgfx_readEntityComponents(entity);

        if (entity->controller) {
            entity->controller->position = position;
        }

        bs_Model* model = NULL;
        if (entity->model) {
            bs_U64 model_name_hash = bs_stringHash(entity->model->name);
            bs_throwBasilisk(BSX_NOT_IMPLEMENTED);
            //model = bs_queryHash(entity->model->name, NULL, model_name_hash, 0).model;

            if (entity->model->mesh) {
                bs_Mesh* mesh = bs_queryMesh(model, entity->model->mesh);
                entity->model->extent = bs_aabbExtent(&mesh->aabb);
            }
            else {
                entity->model->extent = bs_aabbExtent(&model->aabb);
            }
        }

        if (entity->armature) {
            if (old_animation)
                entity->armature->animation = old_animation;
        }

        if (entity->collider && entity->armature) {
            assert(model);
            bs_Armature* armature = bs_queryArmature(model, entity->armature->name);

            if (entity->armature->skeleton == -1)
                entity->armature->skeleton = 0;
        }

        bsgfx_leaveTable();
    }

    if (old_table) {
        bsgfx_enterTable(old_table);
        free(old_table);
    }
}

void bsgfx_respawnScriptEntities(bsgfx_Script* script) {
    for (int i = 0; i < bsgfx_entities.count; i++) {
        bsgfx_Entity* entity = bs_fetchUnit(&bsgfx_entities, i);
        if (entity->script == script)
            bsgfx_respawnEntity(entity, entity->controller ? entity->controller->position : bs_v3V1(0));
    }
}

void bsgfx_despawnEntity(int id) {
    bs_infoF("Despawned entity %d\n", id);

    for (int i = id; i < (bsgfx_entities.count - 1); i++) {
        bsgfx_Entity* this = bs_fetchUnit(&bsgfx_entities, i), * next = bs_fetchUnit(&bsgfx_entities, i + 1);
        *this = *next;
        this->id--;
    }
}

void bsgfx_spawnEntity(bsgfx_Script* script, bs_vec3 position) {
    // bsgfx_Script* script = bs_queryHash(NULL, "lua", spawner->spawn_name, 0).model;
    bsgfx_Entity* entity = bs_pushBack(&bsgfx_entities, &(bsgfx_Entity) {
        .inventory = bs_list(sizeof(bsgfx_EntityItem), 16),
        .id = bsgfx_entities.count,
        .script = script,
    });

    bsgfx_respawnEntity(entity, position);

    return entity;
}

void bsgfx_appendInventory(int entity_id, int item_id) {
    bsgfx_Entity* entity = bs_fetchUnit(&bsgfx_entities, entity_id);
    bsgfx_Item* item = bs_fetchUnit(&bsgfx_items, item_id);

    bs_pushBack(&entity->inventory, &(bsgfx_EntityItem) {
        .mesh_id = item->mesh,
    });

    bsgfx_despawnItem(item_id);
}

void bsgfx_entityFromInventory(const char* script_name, int entity_id, int inventory_id) {
    bsgfx_Entity* entity = bs_fetchUnit(&bsgfx_entities, entity_id);
    bs_throwBasilisk(BSX_NOT_IMPLEMENTED);
    bs_erase(&entity->inventory, inventory_id, 1);
  //  bsgfx_Script* script = bs_query(script_name, 0).data;

  //  bsgfx_spawnEntity(script, entity->controller->position);
}

static void bsgfx_applyPosition(bs_vec3* position, bs_vec3 velocity) {
    bs_v3Add(position, &velocity, position);
}

static void bsgfx_applyGravity(bs_vec3* velocity, float gravity) {
    const float terminal_velocity = 50.0;

    velocity->y -= bs_deltaTime() * gravity;
    velocity->y = bs_clamp(velocity->y, -terminal_velocity, terminal_velocity);
}

void bsgfx_fixedTickEntities() {
    for (int i = 0; i < bsgfx_entities.count; i++) {
        bsgfx_Entity* entity = bs_fetchUnit(&bsgfx_entities, i);
        bsgfx_Script* script = entity->script;

        if (script->broken)
            continue;

        if (bsgfx_enterTable(script->table)) {
            if (entity->controller)
                entity->controller->velocity = bs_v3V1(0);

            if (entity->collider && entity->controller)
                bsgfx_sweepCollisions(entity->collider->sweep_radius, entity->controller->position);

            bsgfx_saveEntityComponents(entity);
            bsgfx_readEntityComponents(entity);

            if (entity->model && entity->armature) {
                bs_U64 model_hash = bs_stringHash(entity->model->name);

                bs_throwBasilisk(BSX_NOT_IMPLEMENTED);

                //bs_Model* model = bs_queryHash(entity->model->name, NULL, model_hash, 0).model;
//                bs_Armature* armature = bs_queryArmature(model, entity->armature->name);

                if (entity->armature->skeleton >= 0) {
        //            bsgfx_runAnimator(model, armature, script->table, entity);
                }
            }

            if (entity->armature && bsgfx_enterField("armature", -1)) {
                duL_set("last_animation", entity->armature->last_animation, -2);
                bsgfx_leaveField();
            }
            bsgfx_readEntityComponents(entity);

            bsgfx_saveEntityComponents(entity);
            if (!bsgfx_call(script, "fixedTick", 0)) {
                entity->script->broken = true;
                bsgfx_leaveTable();
                continue;
            }
            bsgfx_readEntityComponents(entity);

            if (entity->controller) {
                bsgfx_applyGravity(&entity->controller->velocity, entity->controller->gravity);
                bsgfx_applyCollisions(entity);
                bsgfx_applyPosition(&entity->controller->position, entity->controller->velocity);
            }

            bsgfx_leaveTable();

        }
    }
}

void bsgfx_tickEntities() {
    for (int i = 0; i < bsgfx_entities.count; i++) {
        bsgfx_Entity* entity = bs_fetchUnit(&bsgfx_entities, i);
        bsgfx_Script* script = entity->script;

        if (script->broken)
            continue;

        if (bsgfx_enterTable(script->table)) {
            bsgfx_saveEntityComponents(entity);
            if (!bsgfx_call(script, "tick", 0)) {
                entity->script->broken = true;
                bsgfx_leaveTable();
                continue;
            }
            bsgfx_readEntityComponents(entity);

            bool override_transform = bsgfx_call(script, "transform", 1);
            bs_mat4 transform = override_transform ? *bsgfx_getValueReturnValue(BSGFX_LUA_MAT4, -1).as_mat4 : BS_MAT4_IDENTITY;

            if (entity->armature) {
                if (entity->armature->skeleton >= 0) {
                    bsgfx_runAnimatorUnfixed(entity);
                }
            }

            if (entity->model) {
                bs_U64 model_hash = bs_stringHash(entity->model->name);
                //bs_Model* model = bs_query(entity->model->name, 0).data;
                bs_Model* model = NULL;
                bs_throwBasilisk(BSX_NOT_IMPLEMENTED);

                if (entity->controller) {
                    if (!override_transform) {
                        transform = bs_translate(transform, entity->controller->position);
                        transform = bs_rotate(transform, entity->controller->rotation);
                    }
                }

                if (model->extra) {
                    bool override_instance = bsgfx_call(script, "instance", 0);
                    if (!override_instance) {
                        int skeleton = entity->armature ? entity->armature->skeleton : 0;
                        bsgfx_instance(
                            model->extra[0],
                            &transform,
                            sizeof(bs_mat4), 0, skeleton, -1, 0);

                        if (entity->inventory.count > 0 && entity->equipment) {
                            bs_Armature* armature = bs_queryArmature(model, entity->armature->name);
                            bs_except(BSX_FAILED_TO_QUERY);
                            // todo either rename this to "Hold.R" or something, or make it custom in lua
                            // havent decided yet
                            int bone_id = bs_queryBoneId(armature, "Board");
                            if (!bs_caught()) {
                                if (entity->equipment->holding_inventory_id != -1) {
                                    bsgfx_EntityItem* item = bs_fetchUnit(&entity->inventory, entity->equipment->holding_inventory_id);
                                    bs_Mesh* mesh = $items()->meshes + item->mesh_id;

                                    bs_mat4 bone_transform = bs_boneTransform(armature, &armature->bones[bone_id].bone);
                                    bs_mat4 item_transform;
                                    bs_m4Mul(&transform, &bone_transform, &item_transform);
                                    bsgfx_instance(
                                        mesh->extra[BSGFX_ITEM_SUBTYPE],
                                        &item_transform,
                                        sizeof(bs_mat4), 0, 0, -1, 0);
                                }
                            }
                        }
                    }
                }
            }

            bsgfx_leaveTable();
        }
    }
}

    */
