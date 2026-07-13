#include <basilisk-gfx.h>
#include <bsgfx_cache.h>

#include <assert.h>
#include <bs_internal.h>

static bs_List _bsgfx_subtype_data[BSGFX_MAX_NUM_SUBTYPES];

bs_List* bsgfx_subtypeInstances(int subtype) {
	return &_bsgfx_subtype_data[subtype];
}

void bsgfx_iniInstances() {
	if (bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)) {
		bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
		if (buffer->bind_set != BSGFX_SET_MESH_DATA || buffer->binding != BSGFX_BINDING_MESH_DATA)
			bs_bindBuffer(BSGFX_SET_MESH_DATA, BSGFX_BINDING_MESH_DATA, buffer);
		return;
	}

	bs_Buffer* buffer = bs_buffer(BS_BUFFER(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA, false), sizeof(bsgfx_InstanceMetadata), 
		BS_BUFFER_USAGE_STORAGE_BUFFER_BIT | BS_BUFFER_USAGE_TRANSFER_DST_BIT | BS_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		0)->buffer;

	if (!buffer) 
		return;

	bs_bindBuffer(BSGFX_SET_MESH_DATA, BSGFX_BINDING_MESH_DATA, buffer);
	bsgfx_InstanceMetadata* map = bs_mapBuffer(buffer, BS_U32_MAX);
}

void bsgfx_instanceType(int instance_type_id, int max_instance_count, int bind_set, int point) {
	if (_poser_->instance_buffers[instance_type_id]) return;
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	
	bs_Binding* binding = bs_queryBinding(bs_queryBindSet(bind_set), point);
	bs_BufferUsageFlags usage_flags = 0;
	bs_MemoryPropertyFlags memory_flags = BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	if (binding->type == BS_BIND_TYPE_UNIFORM_BUFFER)
		usage_flags |= BS_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	else if (binding->type == BS_BIND_TYPE_STORAGE_BUFFER)
		usage_flags |= BS_BUFFER_USAGE_STORAGE_BUFFER_BIT | BS_BUFFER_USAGE_TRANSFER_DST_BIT | BS_BUFFER_USAGE_TRANSFER_SRC_BIT;
	else bs_throwBasilisk(BSX_INVALID_TYPE);

	// TODO: wtf is this yo
	VkDeviceSize atom_size = 64;

	bs_U32 size = max_instance_count * sizeof(bsgfx_InstanceBuffer);
	VkDeviceSize aligned_size =
		(size + 64 - 1) & ~(atom_size - 1);

	_poser_->instance_buffers[instance_type_id] = bs_buffer(BS_BUFFER(-1, 0, 0), aligned_size, usage_flags, memory_flags, 0)->buffer;

	bs_bindBuffer(bind_set, point, _poser_->instance_buffers[instance_type_id]);
	bs_mapBuffer(_poser_->instance_buffers[instance_type_id], BS_U32_MAX);

	int num_swaps = (buffer->flags & BSI_BUFFER_SWAPS_BIT) ? bs_scope()->window->frames_in_flight : 1;
	for (int i = 0; i < num_swaps; i++) {
		bsgfx_InstanceMetadata* metadata = buffer->_[i].data;
		metadata->instance_types[instance_type_id].allocated = max_instance_count;
	}
}

void bsgfx_deleteSubtype(int subtype) {
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	int num_swaps = (buffer->flags & BSI_BUFFER_SWAPS_BIT) ? bs_scope()->window->frames_in_flight : 1;

	for (int i = 0; i < num_swaps; i++) {
		bsgfx_InstanceMetadata* metadata = buffer->_[i].data;
		struct bsgfx_InstanceTypeMetadata* type_metadata = metadata->instance_types + metadata->instance_subtypes[subtype].instance_type;

		for (int j = subtype; j < metadata->subtypes_count; j++)
			metadata->instance_subtypes[j] = metadata->instance_subtypes[j + 1];

		metadata->subtypes_count--;
	}
}

bs_Range bsgfx_subtypeRange(int subtype) {
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	int index = buffer->flags & BSI_BUFFER_SWAPS_BIT ? bs_currentSwap() : 0;
	bsgfx_InstanceMetadata* metadata = buffer->_[index].data;
	assert(subtype >= 0);
	assert(subtype < metadata->subtypes_count);
	bsgfx_InstanceSubtypeMetadata* subtype_metadata = metadata->instance_subtypes + subtype;

	return (bs_Range) { 
		.offset = subtype_metadata->index_offset, 
		.num = subtype_metadata->index_count, 
		.batch = bs_fetch(subtype_metadata->batch_source_id, subtype_metadata->batch_id)->batch
	};
}

int bsgfx_instanceCount(int subtype) {
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	int index = buffer->flags & BSI_BUFFER_SWAPS_BIT ? bs_currentSwap() : 0;

	bsgfx_InstanceMetadata* metadata = buffer->_[index].data;
	return metadata->instance_subtypes[subtype].instance_count;
}

int bsgfx_subtypeCount(int instance_type_id) {
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	int index = buffer->flags & BSI_BUFFER_SWAPS_BIT ? bs_currentSwap() : 0;

	bsgfx_InstanceMetadata* metadata = buffer->_[index].data;
	return metadata->instance_types[instance_type_id].subtype_count;
}

const int* bsgfx_subtypes() {
	return _bsgfx_subtypes_;
}

int bsgfx_subtype(int instance_type_id, bs_Batch* batch, bs_U32 flags, bs_Range range) {
	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	int num_swaps = (buffer->flags & BSI_BUFFER_SWAPS_BIT) ? bs_scope()->window->frames_in_flight : 1;

	if (!_poser_->instance_buffers[instance_type_id])
		bs_throwBasiliskF(BSX_UNINITIALIZED, "Cannot create subtype, instance type %d needs to be created first\n", instance_type_id);

	int existing = -1;

//	if (key != 0) {
//		existing = bsgfx_querySubtypeNull(instance_type_id, key);
//		if (existing != -1)
//			bs_throwBasiliskF(BSX_DUPLICATE, "Subtypes (%d, %d)", instance_type_id, key);
//
//		if (bs_fetch(batch_id)->batch != range.batch) {
//			bs_throwBasiliskF(BSX_MISMATCH, "Subtype (%d, %d)\nBatch %s != Batch %s",
//				instance_type_id, key, bs_idName(batch_id), (range.batch->head.id ? bs_idName(range.batch->head.id) : NULL));
//			return -1;
//		}
//	}

	int subtype = -1;
	for (int i = 0; i < num_swaps; i++) {
		bsgfx_InstanceMetadata* metadata = buffer->_[i].data;
		assert(metadata->subtypes_count < BSGFX_MAX_NUM_SUBTYPES);
		subtype = existing == -1 ? metadata->subtypes_count : existing;
		metadata->instance_subtypes[subtype].instance_type = instance_type_id;
		metadata->instance_subtypes[subtype].index_offset = range.offset;
		metadata->instance_subtypes[subtype].index_count = range.num;
		metadata->instance_subtypes[subtype].batch_source_id = batch->head.source_id;
		metadata->instance_subtypes[subtype].batch_id = batch->head.id;
		metadata->instance_subtypes[subtype].flags = flags;

		if (existing == -1) {
			if (i == 0)
				_bsgfx_subtype_data[metadata->subtypes_count] = bs_list(sizeof(bsgfx_InstanceBuffer), 32);
			metadata->subtypes_count++;
			metadata->instance_types[instance_type_id].subtype_count++;
		}
	}

	return subtype;
}

int bsgfx_instance(int subtype, const char* data, int data_size, bs_U32 flags, unsigned int bone_index, int id, int material) {
	if (subtype == -1)
		return -1;
	assert(flags <= 65536);
	assert(material <= 65536);

	bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	assert(_poser_->instances_ticked == false);

	bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

	int instance_type = metadata->instance_subtypes[subtype].instance_type;
	if (!_poser_->instance_buffers[instance_type]) {
		bs_warnF("Instance type %d has not been created or has no subtypes!\n", instance_type);
		return -1;
	}

	bsgfx_InstanceBuffer* instance_buffer = bs_bufferMap(_poser_->instance_buffers[instance_type]);

	if (!instance_buffer) {
		bs_warnF("Instance buffer of type %d is unmapped\n", instance_type);
		return -1;
	}

	if (metadata->instance_types[instance_type].count >= metadata->instance_types[instance_type].allocated) {
		bs_warnF("Instance buffer of type %d is maxed out\n", instance_type);
		return -1;
	}

	flags |= (1 << instance_type);
	int out_index = bsgfx_index(flags, metadata->instance_types[instance_type].count);
	metadata->instance_subtypes[subtype].instance_count++;
	metadata->instance_types[instance_type].count++;

	bsgfx_InstanceBuffer instance;
	memcpy(instance.data, data, data_size);
	instance.header.id = id;
	instance.header.flags = flags;
	instance.header.material = material;
	instance.header.subtype = subtype;
	instance.header.bone_index = bone_index;

	bs_pushBack(&_bsgfx_subtype_data[subtype], &instance);

	return metadata->instance_subtypes[subtype].instance_count - 1;
}

void bsgfx_deleteInstance() {
}

static int bsgfx_compareInstanceBuffers(const bsgfx_InstanceBuffer* a, const bsgfx_InstanceBuffer* b) {
	if (a->header.subtype != b->header.subtype)
		return (a->header.subtype < b->header.subtype) ? -1 : 1;
	return 0;
}

int bsgfx_instanceSubtype(int subtype) {

}

void bsgfx_tickInstances() {
	if (!bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA))
		return;

	bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	if (!metadata_buffer) return;
	bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

	for (int i = 0; i < BSGFX_INSTANCE_TYPE_COUNT; i++) {
		struct bsgfx_InstanceTypeMetadata* type_metadata = metadata->instance_types + i;
		if (type_metadata->count == 0) continue;

		bsgfx_InstanceBuffer* first_instance = bs_bufferMap(_poser_->instance_buffers[i]);

		for (int j = 0, offset = 0; j < metadata->subtypes_count; j++) {
			if (metadata->instance_subtypes[j].instance_type != i) continue;

			struct bsgfx_InstanceSubtypeMetadata* subtype_metadata = metadata->instance_subtypes + j;
			bsgfx_InstanceBuffer* instance = first_instance + offset;

			memcpy(instance, _bsgfx_subtype_data[j].data, _bsgfx_subtype_data[j].count * sizeof(bsgfx_InstanceBuffer));
			_bsgfx_subtype_data[j].count = 0;
			metadata->instance_subtypes[j].instance_offset = offset;
			offset += metadata->instance_subtypes[j].instance_count;
		}

		bs_bindBuffer(_poser_->instance_buffers[i]->bind_set, _poser_->instance_buffers[i]->binding, _poser_->instance_buffers[i]);
	}

	bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	bs_bindBuffer(BSGFX_SET_MESH_DATA, BSGFX_BINDING_MESH_DATA, buffer);
}

bool bsgfx_subtypeHasFlag(int subtype, bs_U32 flag) {
	if (subtype < 0) return false;
	bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	if (!metadata_buffer) return false;
	bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);
	return metadata->instance_subtypes[subtype].flags & flag;
}

void bsgfx_renderSubtype(int subtype, bs_Pipeline* pipeline) {
	if (subtype < 0) return;
	bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	if (!metadata_buffer) return;
	bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

	bs_render(
		bs_fetch(metadata->instance_subtypes[subtype].batch_source_id, metadata->instance_subtypes[subtype].batch_id)->batch, pipeline,
		metadata->instance_subtypes[subtype].index_offset, 
		metadata->instance_subtypes[subtype].index_count,
		metadata->instance_subtypes[subtype].instance_offset,
		metadata->instance_subtypes[subtype].instance_count);
}

void bsgfx_resetSubtype(int subtype) {
	bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	if (!metadata_buffer) return;
	bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

	struct bsgfx_InstanceSubtypeMetadata* subtype_metadata = metadata->instance_subtypes + subtype;
	struct bsgfx_InstanceTypeMetadata* type_metadata = metadata->instance_types + subtype_metadata->instance_type;

	bsgfx_InstanceBuffer* instance = bs_bufferMap(_poser_->instance_buffers[subtype_metadata->instance_type]);
	int count = type_metadata->count;

	for (int i = 0; i < count; i++) {
		if (instance[i].header.subtype == subtype) {
			for (int j = i; j < count - 1; j++)
				instance[j] = instance[j + 1];

			type_metadata->count--;
		}
	}

	subtype_metadata->instance_count = 0;

	assert(type_metadata->count >= 0);
}

void bsgfx_resetInstances() {
	bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
	if (!metadata_buffer) return;
	bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

	for (int i = 0; i < metadata->subtypes_count; i++) {
		// metadata->instance_types[metadata->instance_subtypes[i].instance_type].count -= metadata->instance_subtypes[i].instance_count;
		metadata->instance_subtypes[i].instance_count = 0;
	}
	for (int i = 0; i < BSGFX_INSTANCE_TYPE_COUNT; i++)
		metadata->instance_types[i].count = 0;

	//for (int i = 0; i < BSGFX_INSTANCE_TYPE_COUNT; i++) {
	//	assert(metadata->instance_types[i].count >= 0);
	//}

	_poser_->instances_ticked = false;
}

void bsgfx_destroyInstanceTypes() { // todo
}

void bsgfx_instanceHiResMesh(bs_Mesh* mesh, bs_vec3 position, bs_vec4 rotation, float scale, int subtype_offset, bool origin_at_center){
	bs_vec3 min = mesh->aabb.min;
	bs_vec3 max = mesh->aabb.max;
	bs_vec3 size;
	bs_v3Sub(&max, &min, &size);

	float max_dim = BS_MAX(BS_MAX(size.x, size.y), size.z);

	float s = scale / max_dim;
	bs_vec3 sc = { s, s, s };

	bs_vec3 center;
	bs_v3Add(&min, &max, &center);
	bs_v3MulS(&center, 0.5f, &center);

	bs_mat4 transform = bs_transform(position, rotation, sc);

	if (!origin_at_center) {
		bs_vec3 offset;
		bs_v3MulS(&center, -1, &offset);
		bs_translateP(&transform, &offset);
	}

	int mesh_subtype = mesh->extra[subtype_offset];
	bsgfx_instance(mesh_subtype, &transform, sizeof(bs_mat4), 0, 0, -1, 0);
}
