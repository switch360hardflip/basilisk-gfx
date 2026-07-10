
 /**
  bs_models.c
  */

#include <stdio.h>
#include <time.h>

#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <direct.h>

#include <basilisk-core.h>
#include <bs_internal.h>



  /*==============================================================================
   * GLTF Helpers 
   =============================================================================*/
 
static bs_mat4 bs_gltfMat4(bs_Model* model, bs_Json* root, int accessor, int i) {
    int buffer_view_id = bs_fetchJsonF(root, BS_JSON_NUMBER, "accessors[%d].bufferView", accessor).as_number;
    int offset = bs_fetchJsonF(root, BS_JSON_NUMBER, "bufferViews[%d].byteOffset", buffer_view_id).as_number;

    bs_mat4 m;
    memcpy(m.a, model->gltf + offset + i * sizeof(bs_mat4), sizeof(bs_mat4));

    return m;
}

static int* bs_gltfIntArray(bs_Model* model, bs_Json* root, int accessor, int* out_len, int num_components, bs_U32 size) {
    int buffer_view_id = bs_fetchJsonF(root, BS_JSON_NUMBER, "accessors[%d].bufferView", accessor).as_number;
    bs_Json buffer_view = bs_jsonRoot(root, bs_fetchJsonF(root, BS_JSON_OBJECT, "bufferViews[%d]", buffer_view_id).as_object);

    *out_len   = bs_fetchJson(&buffer_view, BS_JSON_NUMBER, "byteLength").as_number / size;
    int offset = bs_fetchJson(&buffer_view, BS_JSON_NUMBER, "byteOffset").as_number;

    return (int*)(model->gltf + offset);
}

static float* bs_gltfFloatArray(bs_Model* model, bs_Json* root, int accessor, int* out_len, int num_components) {
    int buffer_view_id = bs_fetchJsonF(root, BS_JSON_NUMBER, "accessors[%d].bufferView", accessor).as_number;
    int count = bs_fetchJsonF(root, BS_JSON_NUMBER, "accessors[%d].count", accessor).as_number;
    int offset = bs_fetchJsonF(root, BS_JSON_NUMBER, "bufferViews[%d].byteOffset", buffer_view_id).as_number;

    *out_len = count * num_components;

    return (float*)(model->gltf + offset);
}


  /*==============================================================================
   * GLTF Meshes / Primitives
   =============================================================================*/

static void bs_modelAttribData(bs_Json* root, bs_Primitive* primitive, int accessor, int num_components, int offset) {
    int num_floats = 0;
    float* a = bs_gltfFloatArray(primitive->parent->model, root, accessor, &num_floats, num_components);

    for (int i = 0; i < num_floats / num_components; i++, offset += primitive->vertex_size) {
        for (int j = 0; j < num_components; j++)
            primitive->vertices[offset + j] = a[i * num_components + j];
    }
}

static void bs_modelAttribDataI(
    bs_Json* root,
    bs_Primitive* primitive, 
    int accessor, 
    int num_components, 
    int offset, 
    unsigned int* out)
{
    int num_floats = 0;
    bs_U8* arr = (bs_U8*)bs_gltfFloatArray(primitive->parent->model, root, accessor, &num_floats, num_components);

    for (int i = 0; i < num_floats / num_components; i++, offset += primitive->vertex_size) {
        for (int j = 0; j < num_components; j++)
            out[offset + j] = arr[i * num_components];
    }
}

static bs_Primitive* bs_loadPrimitive(
    bs_Mesh* mesh, 
    bs_Primitive* primitive, 
    bs_Json* root,
    bs_Json* primitive_json)
{
    memset(primitive, 0, sizeof(bs_Primitive));

    bs_Json attributes       = bs_jsonRoot(root, bs_fetchJson(primitive_json, BS_JSON_OBJECT, "attributes").as_object);
    bs_JsonValue position    = bs_fetchJson(&attributes, BS_JSON_UNDEFINED, "POSITION");
    bs_JsonValue normal      = bs_fetchJson(&attributes, BS_JSON_UNDEFINED, "NORMAL");
    bs_JsonValue tex_coord   = bs_fetchJson(&attributes, BS_JSON_UNDEFINED, "TEXCOORD_0");
    bs_JsonValue joints      = bs_fetchJson(&attributes, BS_JSON_UNDEFINED, "JOINTS_0");
    bs_JsonValue weights     = bs_fetchJson(&attributes, BS_JSON_UNDEFINED, "WEIGHTS_0");

    primitive->aabb.max = bs_v3V1(-BS_FLT_MAX);
    primitive->aabb.min = bs_v3V1(BS_FLT_MAX);
    
    int num_floats = 0, vertex_size = 0;
    bs_gltfIntArray(mesh->model, root, position.as_number, &num_floats, 1, sizeof(bs_U32));

    if (position.found)  vertex_size += 3;
    if (normal.found)    vertex_size = (primitive->normal_offset = vertex_size) + 3;
    if (tex_coord.found) vertex_size = (primitive->texture_offset = vertex_size) + 2;
    if (joints.found)    vertex_size = (primitive->bone_offset = vertex_size) + 4;
    if (weights.found)   vertex_size = (primitive->weight_offset = vertex_size) + 4;

    primitive->material_id = bs_fetchJson(primitive_json, BS_JSON_UNDEFINED, "material").as_number;
    primitive->parent = mesh;
    primitive->vertex_size = vertex_size;
    primitive->num_vertices = num_floats / 3;
    primitive->vertices = bs_malloc(primitive->num_vertices * vertex_size * sizeof(float));

    if (position.found)  bs_modelAttribData(root, primitive, position.as_number, 3, 0);
    if (normal.found)    bs_modelAttribData(root, primitive, normal.as_number, 3, primitive->normal_offset);
    if (tex_coord.found) bs_modelAttribData(root, primitive, tex_coord.as_number, 2, primitive->texture_offset);
    if (joints.found)    bs_modelAttribDataI(root, primitive, joints.as_number, 4, primitive->bone_offset, primitive->vertices);
    if (weights.found)   bs_modelAttribData(root, primitive, weights.as_number, 4, primitive->weight_offset);

   /**
    Read indices
    */
    int accessor = bs_fetchJson(primitive_json, BS_JSON_NUMBER, "indices").as_number;

    bs_U16* arr = (bs_U16*)bs_gltfFloatArray(mesh->model, root, accessor, &primitive->num_indices, 1);
    primitive->indices = bs_calloc(primitive->num_indices, sizeof(int));
    for (int i = 0; i < primitive->num_indices; i++)
        primitive->indices[i] = arr[i];

    mesh->num_vertices += primitive->num_vertices;
    mesh->num_indices += primitive->num_indices;
    mesh->model->num_vertices += primitive->num_vertices;
    mesh->model->num_indices += primitive->num_indices;

    return primitive;
}

static void bs_loadMeshes(bs_Model* model, bs_Json* root) {
    bs_JsonValue meshes = bs_fetchJson(root, BS_JSON_ARRAY, "meshes");
    bs_JsonValue nodes = bs_fetchJson(root, BS_JSON_ARRAY, "nodes");

    model->meshes_count = meshes.size;
    model->meshes = bs_malloc(model->meshes_count * sizeof(bs_Mesh));

    for (int i = 0; i < nodes.size; i++) {
        bs_Json node = bs_jsonRoot(root, nodes.as_array.as_objects[i]);

        bs_JsonValue mesh_id = bs_fetchJson(&node, BS_JSON_UNDEFINED, "mesh");
        if (!mesh_id.found) 
            continue;

        int id = mesh_id.as_number;
        bs_Json mesh_json = bs_jsonRoot(root, meshes.as_array.as_objects[id]);

        bs_JsonValue primitives = bs_fetchJson(&mesh_json, BS_JSON_ARRAY, "primitives");

        bs_JsonValue translation = bs_fetchJson(&node, BS_JSON_UNDEFINED, "translation");
        bs_JsonValue rotation = bs_fetchJson(&node, BS_JSON_UNDEFINED, "rotation");
        bs_JsonValue scale = bs_fetchJson(&node, BS_JSON_UNDEFINED, "scale");

        bool has_translation = translation.found && translation.type & BS_JSON_ARRAY && translation.type & BS_JSON_NUMBER && translation.size == 3;
        bool has_rotation = rotation.found && rotation.type & BS_JSON_ARRAY && rotation.type & BS_JSON_NUMBER && rotation.size == 4;
        bool has_scale = scale.found && scale.type & BS_JSON_ARRAY && scale.type & BS_JSON_NUMBER && scale.size == 3;

        bs_Mesh* mesh = model->meshes + id;
        *mesh = (bs_Mesh) {
            .model = model,
            .aabb.max = bs_v3V1(-BS_FLT_MAX),
            .aabb.min = bs_v3V1(BS_FLT_MAX),
            .name = strdup(bs_fetchJson(&node, BS_JSON_STRING, "name").as_string),
            .primitives = bs_malloc(primitives.size * sizeof(bs_Primitive)),
            .primitives_count = primitives.size,
            .position = has_translation ? bs_v3(translation.as_array.as_numbers[0], translation.as_array.as_numbers[1], translation.as_array.as_numbers[2]) : bs_v3V1(0.0),
            .rotation = has_rotation ? bs_v4(rotation.as_array.as_numbers[0], rotation.as_array.as_numbers[1], rotation.as_array.as_numbers[2], rotation.as_array.as_numbers[3]) : BS_QUAT_IDENTITY,
            .scale = has_scale ? bs_v3(scale.as_array.as_numbers[0], scale.as_array.as_numbers[1], scale.as_array.as_numbers[2]) : bs_v3V1(1.0),
        };
        mesh->name_hash = bs_stringHash(mesh->name);

        for (int j = 0; j < mesh->primitives_count; j++) {
            bs_Json primitive_json = bs_jsonRoot(root, primitives.as_array.as_objects[j]);
            bs_loadPrimitive(mesh, mesh->primitives + j, root, &primitive_json);
        }

        model->primitives_count += mesh->primitives_count;
    }
}


  /*==============================================================================
   * GLTF Animation
   =============================================================================*/

#define BS_SEARCH_CHANNEL(channel, animation_joint, time)                           \
    int closest = 0;                                                                \
    float smallest = BS_FLT_MAX;                                                    \
    for (int i = 0; i < animation_joint->channel##_count; i++) {                    \
        if (animation_joint->channel##[i].time < time) {                            \
            float diff = time - animation_joint->channel##[i].time;                 \
            if (diff < smallest) {                                                  \
                smallest = diff;                                                    \
                closest = i;                                                        \
            }                                                                       \
        }                                                                           \
    }

BSAPI bs_vec4 _bs_interpolateRotation(bs_AnimationBone* animation_joint, float time) {
    if (!animation_joint->rotations) return BS_QUAT_IDENTITY;
    BS_SEARCH_CHANNEL(rotations, animation_joint, time);
    return animation_joint->rotations[closest].value;
}

BSAPI bs_vec3 _bs_interpolateTranslation(bs_AnimationBone* animation_joint, float time) {
    if (!animation_joint->translations) return bs_v3V1(0);
    BS_SEARCH_CHANNEL(translations, animation_joint, time);
    return animation_joint->translations[closest].value;
}

BSAPI bs_vec3 _bs_interpolateScale(bs_AnimationBone* animation_joint, float time) {
    if (!animation_joint->scalings) return bs_v3V1(1);
    BS_SEARCH_CHANNEL(scalings, animation_joint, time);
    return animation_joint->scalings[closest].value;
}

BSAPI bs_mat4 _bs_boneTransform(bs_Armature* armature, bs_Bone* joint) {
    if (!joint) return BS_MAT4_IDENTITY;
    return armature->bones[joint->id].matrix;
}

BSAPI bs_vec3 _bs_bonePosition(bs_Armature* armature, bs_Bone* joint) {
    return bs_m4MulV4(bs_boneTransform(armature, joint), bs_v4(0.0, 0.0, 0.0, 1.0)).xyz;
}

BSAPI bs_mat4* _bs_transformBone(bs_Armature* armature, bs_Bone* bone, bs_mat4 transform) {
    bs_mat4 parent = (bone->parent_idx == -1) ? BS_MAT4_IDENTITY : armature->bones[bone->parent_idx].matrix;
    bs_mat4* destination = &armature->bones[bone->id].matrix;

    *destination = bs_m4Mul(bone->local_matrix, transform);
    *destination = bs_m4Mul(*destination, bone->bind_matrix_inverse);
    *destination = bs_m4Mul(parent, *destination);

    return destination;
}

BSAPI void _val_bs_blendPose(bs_Armature* armature, bs_Animation* animation_a, bs_Animation* animation_b, float factor, float time_a, float time_b) {
    BS_VALIDATE(!animation_b || animation_a->bones_count == animation_b->bones_count,,
        "Animation \"%s\" (%d bones) != \"%s\" (%d bones)",
        animation_a->name, animation_a->bones_count, animation_b ? animation_b->name : "NULL", animation_b ? animation_b->bones_count : 0);
    _bs_blendPose(armature, animation_a, animation_b, factor, time_a, time_b);
}

BSAPI void _bs_blendPose(bs_Armature* armature, bs_Animation* animation_a, bs_Animation* animation_b, float factor, float time_a, float time_b) {
    if (animation_b) {
        if (animation_a->bones_count != animation_b->bones_count) {
            bs_warnF("Animation \"%s\" (%d bones) != \"%s\" (%d bones)\n", animation_a->name, animation_a->bones_count, animation_b->name, animation_b->bones_count);
            return;
        }

        for (int i = 0; i < armature->bones_count; i++) {
            bs_AnimationBone* a = animation_a->bones + i;
            bs_AnimationBone* b = animation_b->bones + i;

            bs_vec3 translation = bs_v3Lerp(bs_interpolateTranslation(a, time_a), bs_interpolateTranslation(b, time_b), factor);
            bs_vec4 rotation = bs_slerp(bs_interpolateRotation(a, time_a), bs_interpolateRotation(b, time_b), factor);
            bs_vec3 scale = bs_v3Lerp(bs_interpolateScale(a, time_a), bs_interpolateScale(b, time_b), factor);

            bs_transformBone(armature, &armature->bones[i].bone, bs_transform(translation, rotation, scale));
        }
    }
    else {
        for (int i = 0; i < armature->bones_count; i++) {
            bs_AnimationBone* a = animation_a->bones + i;

            bs_vec3 translation = bs_interpolateTranslation(a, time_a);
            bs_vec4 rotation = bs_interpolateRotation(a, time_a);
            bs_vec3 scale = bs_interpolateScale(a, time_a);

            bs_transformBone(armature, &armature->bones[i].bone, bs_transform(translation, rotation, scale));
        }
    }
}

static inline bs_vec3 bs_worldSpaceJoint(bs_Armature* armature, int bone_id) {
    bs_mat4 bind_matrix = bs_m4Inverse(armature->bones[bone_id].bone.bind_matrix_inverse);
    return bs_m4MulV4(bs_m4Mul(armature->bones[bone_id].matrix, bind_matrix), bs_v4(0.0, 0.0, 0.0, 1.0)).xyz;
}

 /**
  pretty lazily implemented rn
  should test around with transforming the target instead of every joint
  */
BSAPI void _bs_fabrik(bs_Armature* armature, int end_effector_id, bs_vec3 target, int chain_length, float* chain) {
    bs_Bone* bone = &armature->bones[end_effector_id].bone;
    bs_vec3 current = target;

    int num = 0;
    int last_bone_id = 0;

    for (num = 0; bone && bone->parent_idx != -1 && (num < chain_length); num++) {
        bs_vec3 position = bs_worldSpaceJoint(armature, bone->id);
        bs_vec3 direction = bs_v3Normalize(bs_v3Sub(position, current));

        armature->bones[num].ik_id = bone->id;
        armature->bones[num].ik_length = chain[num];
        armature->bones[num].ik_position = current;
        armature->bones[num].bone;
        current = bs_v3Add(current, bs_v3MulV1(direction, armature->bones[num].ik_length));

        last_bone_id = bone->id;
        bone = &armature->bones[bone->parent_idx].bone;
    }

    current = bs_worldSpaceJoint(armature, last_bone_id);
    for (int i = num - 1; i >= 0; i--) {
        bs_vec3 direction = bs_v3Normalize(bs_v3Sub(armature->bones[i].ik_position, current));

        bs_vec3 up = bs_v3(0, 1, 0);
        bs_vec3 axis = bs_v3Cross(up, direction);
        float dot = bs_v3Dot(up, direction);
        float w = sqrt(bs_v3MagnitudeSqrd(up) * bs_v3MagnitudeSqrd(direction)) + dot;
        bs_vec4 rotation = bs_qNormalize(bs_v4V3(axis, w));

        armature->bones[armature->bones[i].ik_id].matrix = bs_m4Mul(bs_transform(current, rotation, bs_v3V1(1)), armature->bones[armature->bones[i].ik_id].bone.bind_matrix_inverse);

        current = bs_v3Add(current, bs_v3MulV1(direction, armature->bones[i].ik_length));
    }
}

BSAPI void _bs_bindPose(bs_Armature* armature) {
    for (int i = 0; i < armature->bones_count; i++)
        armature->bones[i].matrix = BS_MAT4_IDENTITY;
}

#define BS_CHANNEL_INCREMENT_BY (16)
#define BS_SET_CHANNEL(channel, p)                                                                              \
    if (bone->channel##_count >= bone->channel##_allocated) {                                                   \
        bone->channel##_allocated += BS_CHANNEL_INCREMENT_BY;                                                   \
        bone->channel## = bs_realloc(bone->channel##, bone->channel##_allocated * sizeof(*bone->channel##));    \
    }                                                                                                           \
    bone->channel##[bone->channel##_count].value = p##;                                                         \
    bone->channel##[bone->channel##_count++].time = timestamp;

BSAPI void _bs_keyframePosition(bs_AnimationBone* bone, float timestamp, bs_vec3 translation) { 
    BS_SET_CHANNEL(translations, translation);
}

BSAPI void _bs_keyframeRotation(bs_AnimationBone* bone, float timestamp, bs_vec4 rotation) { 
    BS_SET_CHANNEL(rotations, rotation); 
}

BSAPI void _bs_keyframeScale(bs_AnimationBone* bone, float timestamp, bs_vec3 scale) { 
    BS_SET_CHANNEL(scalings, scale); 
}

static int bs_queryAnimationHash(bs_Model* model, char* name) {
    bs_U64 hash = bs_stringHash(name);

    for (int i = 0; i < model->animations_count; i++) {
        if (model->animation_hashes[i] == hash) {
            return i;
        }
    }

    bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Animation \"%s\"", name);
    return -1;
}

BSAPI bs_Animation _bs_loadAnimation(bs_Model* model, const char* name) {
    int animation_id = bs_queryAnimationHash(model, name);
    if (animation_id == -1)
        return (bs_Animation) { 0 };

    bs_Json* root = &model->json;
    bs_JsonValue animation_object = bs_fetchJsonF(root, BS_JSON_UNDEFINED, "animations[%d]", animation_id);
    bs_Json animation_root = bs_jsonRoot(root, animation_object.as_object);

    bs_JsonArray samplers = bs_fetchJson(&animation_root, BS_JSON_ARRAY, "samplers").as_array;
    bs_JsonValue channels_json = bs_fetchJson(&animation_root, BS_JSON_ARRAY, "channels");

    bs_Animation animation = {
        .name = strdup(bs_fetchJson(&animation_root, BS_JSON_STRING, "name").as_string),
        .bones_allocated = channels_json.size / 3,
    };

    if (channels_json.size > 0)
        animation.bones = bs_calloc(animation.bones_allocated, sizeof(bs_AnimationBone));

    for (int i = 0, last_node = -1; i < channels_json.size; i++) {
        bs_Json channel_json = bs_jsonRoot(root, channels_json.as_array.as_objects[i]);

        int sampler_id = bs_fetchJson(&channel_json, BS_JSON_NUMBER, "sampler").as_number;
        bs_Json sampler = bs_jsonRoot(root, samplers.as_objects[sampler_id]);
        int input = bs_fetchJson(&sampler, BS_JSON_NUMBER, "input").as_number;
        int output = bs_fetchJson(&sampler, BS_JSON_NUMBER, "output").as_number;
        char* path = bs_fetchJson(&channel_json, BS_JSON_STRING, "target.path").as_string;
        int node = bs_fetchJson(&channel_json, BS_JSON_NUMBER, "target.node").as_number;

        //char* name = bs_fetchJsonF(root, BS_JSON_STRING, "nodes[%d].name", node).as_string;
        int num_inputs = 0, num_outputs = 0;
        float* inputs = bs_gltfFloatArray(model, root, input, &num_inputs, 1);
        float* outputs = bs_gltfFloatArray(model, root, output, &num_outputs, 1);
        assert(num_inputs == num_outputs);

        if (last_node != -1 && last_node != node)
            animation.bones_count++;
        last_node = node;
        assert(animation.bones_count < animation.bones_allocated);

        animation.frames_count = BS_MAX(animation.frames_count, num_inputs);
        for (int j = 0; j < num_inputs; j++)
            animation.length = bs_max(animation.length, inputs[j]);

        int path_len = strlen(path);
        if ((sizeof("translation") - 1) == path_len && strncmp(path, "translation", path_len) == 0) {
            for (int j = 0; j < num_inputs; j++)
                bs_keyframePosition(animation.bones + animation.bones_count, inputs[j], ((bs_vec3*)outputs)[j]);
        }
        else if ((sizeof("rotation") - 1) == path_len && strncmp(path, "rotation", path_len) == 0) {
            for (int j = 0; j < num_inputs; j++)
                bs_keyframeRotation(animation.bones + animation.bones_count, inputs[j], ((bs_vec4*)outputs)[j]);
        }
        else if ((sizeof("scale") - 1) == path_len && strncmp(path, "scale", path_len) == 0) {
            for (int j = 0; j < num_inputs; j++)
                bs_keyframeScale(animation.bones + animation.bones_count, inputs[j], ((bs_vec3*)outputs)[j]);
        }
        else bs_throwBasilisk(BSXI_INTERNAL | BSX_CORRUPTED); // shouldnt happen
    }

    assert(animation.frames_count > 0);

    return animation;
}

static void bs_loadAnimations(bs_Model* model, bs_Json* root) {
    bs_JsonValue animations = bs_fetchJson(root, BS_JSON_UNDEFINED, "animations");
    if (!animations.found || animations.size == 0)
        return;

    model->animations_count = animations.size;
    model->animation_hashes = bs_calloc(animations.size, sizeof(bs_U64));
    // model->animations = bs_calloc(model->animations_count, sizeof(bs_Animation));

    for (int i = 0; i < model->animations_count; i++) {
        bs_Json animation_root = bs_jsonRoot(root, animations.as_array.as_objects[i]);
        char* name = bs_fetchJson(&animation_root, BS_JSON_STRING, "name").as_string;
        bs_U64 hash = bs_stringHash(name);

        model->animation_hashes[i] = hash;

        //bs_loadAnimation(root, &animation_root, model, model->animations + i);
    }
}


  /*==============================================================================
   * GLTF Armature
   =============================================================================*/

static inline void bs_setParentIds(bs_Armature* armature, bs_JsonArray* joints, bs_JsonValue* children, int i) {
    for (int j = 0; j < children->size; j++) {
        for (int k = 0; k < armature->bones_count; k++) {
            if (joints->as_numbers[k] == children->as_array.as_numbers[j]) {
                armature->bones[k].bone.parent_idx = i;
                break;
            }
        }
        children->as_array.as_numbers[j] = i;
    }
}

static inline void bs_setOppositeIds(bs_Armature* armature, bs_Bone* bone) {
    if (bone->name_length <= 2) return;
    if (bone->name[bone->name_length - 2] != '.') return;
    
    bs_except(BSX_FAILED_TO_QUERY);
    int opposite_id = -1;
    if (bone->name[bone->name_length - 1] == 'L') {
        bone->name[bone->name_length - 1] = 'R';
        opposite_id = bs_queryBoneId(armature, bone->name);
        bone->name[bone->name_length - 1] = 'L';
    }
    else if (bone->name[bone->name_length - 1] == 'R') {
        bone->name[bone->name_length - 1] = 'L';
        opposite_id = bs_queryBoneId(armature, bone->name);
        bone->name[bone->name_length - 1] = 'R';
    }
    if (!bs_caught())
        bone->opposite_id = opposite_id;
}

static void bs_loadArmature(bs_Model* model, bs_Armature* armature, bs_Json* root, bs_Json* skin_root) {
    int inverse_bind_matrices_accessor = bs_fetchJson(skin_root, BS_JSON_NUMBER, "inverseBindMatrices").as_number;
    bs_JsonValue joints = bs_fetchJson(skin_root, BS_JSON_ARRAY, "joints");

    *armature = (bs_Armature){
        .name = strdup(bs_fetchJson(skin_root, BS_JSON_STRING, "name").as_string),
        .bones_count = joints.size,
        .bones_allocated = joints.size,
        .bones = bs_calloc(joints.size, sizeof(*armature->bones)),
    };

    for (int j = 0; j < armature->bones_count; j++)
        armature->bones[j].bone.parent_idx = armature->bones[j].bone.opposite_id = -1;

    for (int j = 0; j < armature->bones_count; j++) {
        int node_id = joints.as_array.as_numbers[j];
        bs_Json node = bs_jsonRoot(root, bs_fetchJsonF(root, BS_JSON_OBJECT, "nodes[%d]", node_id).as_object);
        bs_JsonValue children = bs_fetchJson(&node, BS_JSON_UNDEFINED, "children");

        bs_setParentIds(armature, &joints.as_array, &children, j);

        double* translation = bs_fetchJson(&node, BS_JSON_UNDEFINED, "translation").as_array.as_numbers;
        double* rotation = bs_fetchJson(&node, BS_JSON_UNDEFINED, "rotation").as_array.as_numbers;
        double* scale = bs_fetchJson(&node, BS_JSON_UNDEFINED, "scale").as_array.as_numbers;

        bs_vec3 translation_v = translation ? bs_v3(translation[0], translation[1], translation[2]) : bs_v3V1(0.0);
        bs_vec4 rotation_v = rotation ? bs_v4(rotation[0], rotation[1], rotation[2], rotation[3]) : BS_QUAT_IDENTITY;
        bs_vec3 scale_v = scale ? bs_v3(scale[0], scale[1], scale[2]) : bs_v3V1(1.0);

        bs_Bone* bone = &armature->bones[j].bone;
        bone->bind_matrix_inverse = bs_gltfMat4(model, root, inverse_bind_matrices_accessor, j);
        bs_mat4 bind_matrix = bs_m4Inverse(bone->bind_matrix_inverse);
        bone->local_matrix = bs_m4Mul(bind_matrix, bs_m4Inverse(bs_transform(translation_v, rotation_v, scale_v)));

        char* name = bs_fetchJson(&node, BS_JSON_STRING, "name").as_string;
        if (name) {
            bone->name_hash = bs_stringHash(name);
            bone->name = strdup(name);
            bone->name_length = bone->name ? strlen(bone->name) : 0;
        }
        bone->id = j;
    }

    for (int j = 0; j < armature->bones_count; j++) {
        bs_Bone* bone = &armature->bones[j].bone;
        bs_setOppositeIds(armature, bone);
    }
}

static void bs_loadArmatures(bs_Model* model, bs_Json* root) {
    bs_JsonValue skins = bs_fetchJson(root, BS_JSON_UNDEFINED, "skins");
    if (!skins.found || skins.size == 0)
        return;

    model->armatures_count = skins.size;
    if (model->armatures_count == 0)
        return;
    
    model->armatures = bs_malloc(model->armatures_count * sizeof(bs_Armature));

    for (int i = 0; i < model->armatures_count; i++) {
        bs_Armature* armature = model->armatures + i;
        bs_Json armature_json = bs_jsonRoot(root, skins.as_array.as_objects[i]);
        bs_loadArmature(model, armature, root, &armature_json);
    }
}


  /*==============================================================================
   * GLTF Materials
   =============================================================================*/

static void bs_loadMaterial(bs_Material* material, bs_Json* root) {
    material->name = strdup(bs_fetchJson(root, BS_JSON_STRING, "name").as_string);

    bs_JsonValue base_color_factor = bs_fetchJson(root, BS_JSON_UNDEFINED, "pbrMetallicRoughness.baseColorFactor");
    if (base_color_factor.found) {
        // asserts shouldnt ever fail
        assert(base_color_factor.size == 4);
        assert(base_color_factor.type & BS_JSON_NUMBER);

        material->color = bs_rgba(
            base_color_factor.as_array.as_numbers[0] * 255,
            base_color_factor.as_array.as_numbers[1] * 255, 
            base_color_factor.as_array.as_numbers[2] * 255, 
            base_color_factor.as_array.as_numbers[3] * 255);
    }
}

static void bs_loadMaterials(bs_Model* model, bs_Json* root) {
    bs_JsonValue materials = bs_fetchJson(root, BS_JSON_UNDEFINED, "materials");
    if (!materials.found || materials.size == 0)
        return;

    model->materials_count = materials.size;
    model->materials = bs_calloc(model->materials_count, sizeof(bs_Material));

    for (int i = 0; i < model->materials_count; i++) {
        bs_Json material_root = bs_jsonRoot(root, materials.as_array.as_objects[i]);
        bs_loadMaterial(model->materials + i, &material_root);
    }
}

  /*==============================================================================
   * GLTF Models
   =============================================================================*/

static inline bs_vec3 bs_v3MinBound(bs_vec3 a, bs_vec3 b) { return bs_v3(bs_min(a.x, b.x), bs_min(a.y, b.y), bs_min(a.z, b.z)); }
static inline bs_vec3 bs_v3MaxBound(bs_vec3 a, bs_vec3 b) { return bs_v3(bs_max(a.x, b.x), bs_max(a.y, b.y), bs_max(a.z, b.z)); }
static void bs_calculateModelBounds(bs_Model* model) {
    if (model->aabb.min.x != BS_FLT_MAX) return;

    for (int i = 0; i < model->meshes_count; i++) {
        bs_Mesh* mesh = model->meshes + i;

        for (int j = 0; j < mesh->primitives_count; j++) {
            bs_Primitive* prim = mesh->primitives + j;

            for (int k = 0; k < prim->num_vertices; k++) {
                bs_vec3 pos = *(bs_vec3*)(prim->vertices + k * prim->vertex_size);
                prim->aabb.min = bs_v3MinBound(prim->aabb.min, pos);
                prim->aabb.max = bs_v3MaxBound(prim->aabb.max, pos);
            }

            mesh->aabb.min = bs_v3MinBound(mesh->aabb.min, prim->aabb.min);
            mesh->aabb.max = bs_v3MaxBound(mesh->aabb.max, prim->aabb.max);
        }

        model->aabb.min = bs_v3MinBound(model->aabb.min, mesh->aabb.min);
        model->aabb.max = bs_v3MaxBound(model->aabb.max, mesh->aabb.max);
    }
}

BSAPI void _bs_destroyModel(bs_Model* model) {
    for (int i = 0; i < model->meshes_count; i++) {
        bs_Mesh* mesh = model->meshes + i;
        for (int j = 0; j < mesh->primitives_count; j++) {
            bs_Primitive* primitive = mesh->primitives + j;

            primitive->indices = bs_free(primitive->indices);
            primitive->vertices = bs_free(primitive->vertices);
        }

        mesh->name = bs_free(mesh->name);
        mesh->primitives = bs_free(mesh->primitives);
    }

    model->armatures = bs_free(model->armatures);
    model->meshes = bs_free(model->meshes);
}

BSAPI bs_Resource* _bs_model(int package_id, const char* name, bs_U32 flags) {
    bs_Resource* resource = bs_loadResource(package_id, name, flags);

    struct {
        bs_U32 magic;
        bs_U32 version;
        bs_U32 length;
    }* header = resource->data->value;
    assert(header->magic == 0x46546C67);
    
    bs_Json json = { 0 };
    unsigned char* gltf = NULL;

    int offset = sizeof(*header);
    while (offset < header->length) {
        struct {
            bs_U32 length;
            bs_U32 type;
            char data[];
        }* chunk = resource->data->value + offset;

        if (chunk->type == 0x4E4F534A) { // json
            assert(json.doc == NULL);
            json = bs_json(resource->data->value + offset + sizeof(*chunk), chunk->length);
        }
        else if (chunk->type == 0x004E4942) { // bin
            gltf = resource->data->value + offset + sizeof(*chunk);
        }
        else {
            bs_warnF("_bs_model: unknown chunk type 0x%08X\n", chunk->type);
            return NULL;
        }

        offset += chunk->length + sizeof(*chunk);
    }

    if (!json.doc) {
        bs_warnF("_bs_model: missing JSON chunk\n");
        return NULL;
    }
    
    bs_Model model = {
        .aabb.max = bs_v3V1(-BS_FLT_MAX),
        .aabb.min = bs_v3V1(BS_FLT_MAX),
        .json = json,
        .flags = BS_MODEL_JUST_CREATED,
        .gltf = gltf,
    };

    bs_loadMeshes(&model, &json);
    bs_loadArmatures(&model, &json);
    bs_loadAnimations(&model, &json);
    bs_loadMaterials(&model, &json);

    bs_calculateModelBounds(&model);
   // if (!(flags & BS_QUERY_MODEL_KEEP_JSON))
   //     bs_destroyJson(&json);

    resource->model = bs_malloc(sizeof(bs_Model)); // todo dont do this
    memcpy(resource->model, &model, sizeof(bs_Model));

    return resource;
}


 /*==============================================================================
  * Armature
   =============================================================================*/

/*
bs_Object bs_armature(int id, bs_ArmatureFlags flags) {
    if (!id) 
        bs_throwBasilisk(BSXI_INTERNAL | BSX_NOT_IMPLEMENTED);
    if (bs_shouldLoadId(id)) {
        bs_Object existing = bs_fetchNull(id);
        if (existing.armature && !(flags & BS_ARMATURE_FORCE_DESTROY))
            return existing;
    } else return (bs_Object) { 0 };

    bs_Object object = BS_ENSURE(id, ((bs_Armature) {
        .name = "armature",
    }), 0, 0);

    return object;
}
*/

BSAPI int _val_bs_bone(bs_Armature* armature, bs_mat4 local_transform, int parent_id, const char* name) {
    BS_VALIDATE(armature != NULL, -1,
        "armature is NULL");
    return _bs_bone(armature, local_transform, parent_id, name);
}

BSAPI int _bs_bone(bs_Armature* armature, bs_mat4 local_transform, int parent_id, const char* name) {
    if (!armature) {
        bs_warnF("_bs_bone: armature is NULL\n");
        return -1;
    }

    bs_mat4 bind_matrix = BS_MAT4_IDENTITY;

    if (armature->bones_count >= armature->bones_allocated) {
        const int increment_by = 16;
        armature->bones_allocated += increment_by;
        armature->bones = bs_realloc(armature->bones, armature->bones_allocated * sizeof(*armature->bones));
    }

    armature->bones[armature->bones_count].ik_id = 0;
    armature->bones[armature->bones_count].ik_position = bs_v3V1(0);
    armature->bones[armature->bones_count].bone = (bs_Bone) {
        .bind_matrix_inverse = bind_matrix,
        .local_matrix = bs_m4Mul(bind_matrix, local_transform),
        .id = armature->bones_count,
        .name = name,
        .parent_idx = parent_id,
    };

    bs_transformBone(armature, &armature->bones[armature->bones_count].bone, BS_MAT4_IDENTITY);

    return armature->bones_count++;
}

BSAPI void _bs_destroyAnimation(bs_Animation* animation) {
    animation->bones_count = 0;
    animation->bones = NULL;
    bs_free(animation->bones);
}

/*
bs_Object bs_animation(bs_Armature* armature, int id, const char* name, bs_AnimationFlags flags) {
    if (!id) 
        bs_throwBasilisk(BSXI_INTERNAL | BSX_NOT_IMPLEMENTED);
    if (!armature) 
        bs_throwBasilisk(BSXI_INTERNAL | BSX_INVALID_PARAM);

    if (bs_shouldLoadId(id)) {
        bs_Object existing = bs_fetchNull(id);

        if (existing.animation) {
            if (flags & BS_ANIMATION_FORCE_DESTROY)
                bs_destroyAnimation(existing.animation);
            else return existing;
        }

    } else return (bs_Object) { 0 };

    bs_Object object = BS_ENSURE(id, ((bs_Animation) {
        .name = name,
        .bones = bs_calloc(armature->bones_count, sizeof(bs_AnimationBone)),
        .bones_count = armature->bones_count,
    }), 0, 0);

    return object;
}
*/

 /*==============================================================================
  * Queries
  =============================================================================*/

BSAPI bs_Mesh* _bs_queryMeshHash(bs_Model* model, bs_U64 hash, const char* name) {
    for (int i = 0; i < model->meshes_count; i++) {
        bs_Mesh* mesh = model->meshes + i;
        if (mesh->name_hash == hash)
            return mesh;
    }

    bs_warnF("Mesh \"%s\" not found\n", name);
    return NULL;
}

BSAPI bs_Mesh* _bs_queryMesh(bs_Model* model, const char* name) {
    return bs_queryMeshHash(model, bs_stringHash(name), name);
}

/*
bs_Object bs_queryArmature(bs_Model* model, int id, const char* name, bs_ArmatureFlags flags) {
    if (!id) 
        bs_throwBasilisk(BSXI_INTERNAL | BSX_NOT_IMPLEMENTED);
    if (bs_shouldLoadId(id)) {
    }
    else return (bs_Object) { 0 };

    for(int i = 0; i < model->armatures_count; i++) {
        bs_Armature* armature = model->armatures + i;
        if (strcmp(name, armature->name) == 0) {
            armature->head.id = id;

            bs_Object object = {
                .type = BS_ARMATURE,
                .armature = armature,
            };

            bs_map(id, &object);
            return object;
        }
    }
    
    bs_throwBasiliskF(BSXI_INTERNAL | BSX_FAILED_TO_QUERY, "Armature \"%s\"", name);
    return (bs_Object) { 0 };
}
*/

BSAPI bs_Armature* _bs_queryArmature(bs_Model* model, const char* name) {
    for (int i = 0; i < model->armatures_count; i++) {
        if (strcmp(name, model->armatures[i].name) == 0)
            return model->armatures + i;
    }

    bs_warnF("Armature \"%s\" not found\n", name);
    return NULL;
}

BSAPI int _bs_queryBoneId(bs_Armature* armature, const char* name) {
    bs_U64 name_hash = bs_stringHash(name);
    for (int i = 0; i < armature->bones_count; i++) {
        if (armature->bones[i].bone.name_hash == name_hash)
            return i;
    }

    bs_warnF("Bone \"%s\" not found\n", name);
    return -1;
}

BSAPI bs_Bone* _bs_queryBone(bs_Armature* armature, const char* name) {
    int id = bs_queryBoneId(armature, name);
    return id == -1 ? NULL : &armature->bones[id].bone;
}

/*
bs_Object bs_queryAnimation(bs_Model* model, int id, const char* name, bs_AnimationFlags flags) {
    if (!id) 
        bs_throwBasilisk(BSXI_INTERNAL | BSX_NOT_IMPLEMENTED);

    if (bs_shouldLoadId(id)) {
        bs_Object existing = bs_fetchNull(id);
        if (existing.animation) {
            if (!(flags & BS_ANIMATION_FORCE_DESTROY))
                return existing;
        }
    }
    else return (bs_Object) { 0 };

    for(int i = 0; i < model->animations_count; i++) {
        bs_Animation* animation = model->animations + i;
        if (strcmp(name, animation->name) == 0) {
            animation->head.id = id;

            bs_map(id, &object);
            return object;
        }
    }

    bs_throwBasiliskF(BSXI_INTERNAL | BSX_FAILED_TO_QUERY, "Animation \"%s\"", name);
    return (bs_Object) { 0 };
}

*/
BSAPI bs_Material* _bs_queryMaterial(bs_Model* model, const char* name) {
    for (int i = 0; i < model->materials_count; i++) {
        if (strcmp(name, model->materials[i].name) == 0)
            return model->materials + i;
    }

    bs_warnF("Material \"%s\" not found\n", name);
    return NULL;
}
