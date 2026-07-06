
 /**
  MIT License
  
  Copyright (c) 2026 switch360hardflip <switch360hardflip@gmail.com>
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */ 

 /**
  This file was generated from basilisk-gfx.com

  It is not recommended to make changes to this file as it will be lost if
  the code is regenerated.
  */
        
#include <bs_internal.h>
#include <basilisk-core.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/vec4.h>

bs_List* bs_packages()
{
    return next.bs_packages();
}

bs_List* bs_objectSources()
{
    return next.bs_objectSources();
}

bs_vec2 bs_v2()
{
    return next.bs_v2();
}

void bs_v2Add(
    bs_vec2* a, 
    bs_vec2* b, 
    bs_vec2* out)
{
    glm_vec2_add(a->a, b->a, out->a);
}

void bs_v2Sub(
    bs_vec2* a, 
    bs_vec2* b, 
    bs_vec2* out)
{
    glm_vec2_sub(a->a, b->a, out->a);
}

void bs_v2Mul(
    bs_vec2* a, 
    bs_vec2* b, 
    bs_vec2* out)
{
    glm_vec2_mul(a->a, b->a, out->a);
}

void bs_v2Div(
    bs_vec2* a, 
    bs_vec2* b, 
    bs_vec2* out)
{
    glm_vec2_div(a->a, b->a, out->a);
}

void bs_v2MulV1(
    bs_vec2* v, 
    float s, 
    bs_vec2* out)
{
    glm_vec2_scale(v->a, s, out->a);
}

void bs_v2DivV1(
    bs_vec2* v, 
    float s, 
    bs_vec2* out)
{
    glm_vec2_scale(v->a, 1.0 / s, out->a);
}

float bs_v2Dot(
    bs_vec2* a, 
    bs_vec2* b)
{
    glm_vec2_dot(a->a, b->a, out->a);
}

float bs_v2Normalize(
    bs_vec2* v)
{
    glm_vec2_normalize(v->a);
}

float bs_v2NormalizeTo(
    const bs_vec2* v, 
    bs_vec2* out)
{
    glm_vec2_normalize_to(v->a, out->a);
}

bs_vec3 bs_v3()
{
    return next.bs_v3();
}

void bs_v3Add(
    bs_vec3* a, 
    bs_vec3* b, 
    bs_vec3* out)
{
    glm_vec3_add(a->a, b->a, out->a);
}

void bs_v3Sub(
    bs_vec3* a, 
    bs_vec3* b, 
    bs_vec3* out)
{
    glm_vec3_sub(a->a, b->a, out->a);
}

void bs_v3Mul(
    bs_vec3* a, 
    bs_vec3* b, 
    bs_vec3* out)
{
    glm_vec3_mul(a->a, b->a, out->a);
}

void bs_v3Div(
    bs_vec3* a, 
    bs_vec3* b, 
    bs_vec3* out)
{
    glm_vec3_div(a->a, b->a, out->a);
}

void bs_v3MulV1(
    bs_vec3* v, 
    float s, 
    bs_vec3* out)
{
    glm_vec3_scale(v->a, s, out->a);
}

void bs_v3DivV1(
    bs_vec3* v, 
    float s, 
    bs_vec3* out)
{
    glm_vec3_scale(v->a, 1.0 / s, out->a);
}

float bs_v3Dot(
    bs_vec3* a, 
    bs_vec3* b)
{
    glm_vec3_dot(a->a, b->a, out->a);
}

float bs_v3Normalize(
    bs_vec3* v)
{
    glm_vec3_normalize(v->a);
}

float bs_v3NormalizeTo(
    const bs_vec3* v, 
    bs_vec3* out)
{
    glm_vec3_normalize_to(v->a, out->a);
}

bs_vec4 bs_v4()
{
    return next.bs_v4();
}

void bs_v4Add(
    bs_vec4* a, 
    bs_vec4* b, 
    bs_vec4* out)
{
    glm_vec4_add(a->a, b->a, out->a);
}

void bs_v4Sub(
    bs_vec4* a, 
    bs_vec4* b, 
    bs_vec4* out)
{
    glm_vec4_sub(a->a, b->a, out->a);
}

void bs_v4Mul(
    bs_vec4* a, 
    bs_vec4* b, 
    bs_vec4* out)
{
    glm_vec4_mul(a->a, b->a, out->a);
}

void bs_v4Div(
    bs_vec4* a, 
    bs_vec4* b, 
    bs_vec4* out)
{
    glm_vec4_div(a->a, b->a, out->a);
}

void bs_v4MulV1(
    bs_vec4* v, 
    float s, 
    bs_vec4* out)
{
    glm_vec4_scale(v->a, s, out->a);
}

void bs_v4DivV1(
    bs_vec4* v, 
    float s, 
    bs_vec4* out)
{
    glm_vec4_scale(v->a, 1.0 / s, out->a);
}

float bs_v4Dot(
    bs_vec4* a, 
    bs_vec4* b)
{
    glm_vec4_dot(a->a, b->a, out->a);
}

float bs_v4Normalize(
    bs_vec4* v)
{
    glm_vec4_normalize(v->a);
}

float bs_v4NormalizeTo(
    const bs_vec4* v, 
    bs_vec4* out)
{
    glm_vec4_normalize_to(v->a, out->a);
}

bs_Quad bs_quad(
    bs_vec3 position, 
    bs_vec2 dimensions)
{
    return next.bs_quad(position, dimensions);
}

bs_Result bs_convertVulkanResult(
    int code)
{
    return next.bs_convertVulkanResult(code);
}

bs_Result bs_convertLastError()
{
    return next.bs_convertLastError();
}

bs_Result bs_convertErrno()
{
    return next.bs_convertErrno();
}

const char* bs_serializeErrno()
{
    return next.bs_serializeErrno();
}

bs_Result bs_playSound(
    bs_Sound* sound, 
    float volume)
{
    return next.bs_playSound(sound, volume);
}

bs_Result bs_sound(
    bs_Resource* resource, 
    bs_U32 flags)
{
    return next.bs_sound(resource, flags);
}

bs_Result bs_iniAudio()
{
    return next.bs_iniAudio();
}

bs_Result bs_ray(
    bs_vec3 start, 
    bs_vec3 direction, 
    float length, 
    bs_Ray* ray)
{
    return next.bs_ray(start, direction, length, ray);
}

bs_Result bs_rayVsObb(
    const bs_Ray* ray, 
    bs_vec3 position, 
    bs_vec4 rotation, 
    bs_vec3 scale, 
    bs_RayVsObb* result)
{
    return next.bs_rayVsObb(ray, position, rotation, scale, result);
}

bs_Result bs_sphereVsPoint(
    bs_vec3 center, 
    float radius, 
    bs_vec3 point, 
    bs_SphereVsPoint* result)
{
    return next.bs_sphereVsPoint(center, radius, point, result);
}

bs_Result bs_sphereVsBox(
    bs_vec3 center, 
    float radius, 
    bs_vec3 position, 
    bs_vec4 rotation, 
    bs_vec3 scale, 
    bs_SphereVsBox* result)
{
    return next.bs_sphereVsBox(center, radius, position, rotation, scale, result);
}

bs_Result bs_rectangleVsPoint(
    bs_vec2 position, 
    bs_vec2 dimensions, 
    bs_vec2 point, 
    bs_RectangleVsPoint* result)
{
    return next.bs_rectangleVsPoint(position, dimensions, point, result);
}

bs_Result bs_rectangleVsPointAbs(
    bs_vec2 position, 
    bs_vec2 dimensions, 
    bs_vec2 point, 
    bs_RectangleVsPoint* result)
{
    return next.bs_rectangleVsPointAbs(position, dimensions, point, result);
}

bs_Result bs_lineVsLine(
    bs_vec2 l1_start, 
    bs_vec2 l1_end, 
    bs_vec2 l2_start, 
    bs_vec2 l2_end, 
    bs_LineVsLine* result)
{
    return next.bs_lineVsLine(l1_start, l1_end, l2_start, l2_end, result);
}

void bs_populateVertexDeclaration(
    bs_VertexDeclaration* declaration, 
    bs_Attribute* attributes, 
    int attributes_count)
{
    return next.bs_populateVertexDeclaration(declaration, attributes, attributes_count);
}

bs_Swapchain* bs_swapchain()
{
    return next.bs_swapchain();
}

int bs_currentSwap()
{
    return next.bs_currentSwap();
}

bs_Result bs_beginComment(
    char* value, 
    int value_length)
{
    return next.bs_beginComment(value, value_length);
}

bs_Result bs_beginCommentV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_beginComment(string->value, string->len);
}

bs_Result bs_beginCommentF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_beginCommentV(format, args);
    va_end(args);
    return _return;
}

bs_Result bs_endComment()
{
    return next.bs_endComment();
}

int bs_swapCount()
{
    return next.bs_swapCount();
}

bs_Image* bs_swapchainImage()
{
    return next.bs_swapchainImage();
}

bs_Result bs_clearStencil(
    bs_U32 index, 
    bs_ivec2 resolution, 
    bs_U32 value)
{
    return next.bs_clearStencil(index, resolution, value);
}

bs_Result bs_clearDepth(
    bs_U32 index, 
    bs_ivec2 dim, 
    float value)
{
    return next.bs_clearDepth(index, dim, value);
}

bs_Result bs_clearDepthStencil(
    bs_U32 index, 
    bs_ivec2 dim, 
    float depth_value, 
    bs_U32 stencil_value)
{
    return next.bs_clearDepthStencil(index, dim, depth_value, stencil_value);
}

bs_Result bs_clearColor(
    bs_U32 index, 
    bs_ivec2 dim, 
    bs_RGBA color)
{
    return next.bs_clearColor(index, dim, color);
}

bs_Result bs_stencilReference(
    bs_FaceType face, 
    bs_U32 reference)
{
    return next.bs_stencilReference(face, reference);
}

bs_Result bs_cull(
    bs_CullFlags flags)
{
    return next.bs_cull(flags);
}

bs_Result bs_setLineWidth(
    float width)
{
    return next.bs_setLineWidth(width);
}

int bs_batchSize(
    bs_Batch* batch)
{
    return next.bs_batchSize(batch);
}

void bs_render(
    bs_Batch* batch, 
    bs_Pipeline* pipeline, 
    bs_U32 vertex_offset, 
    bs_U32 vertex_count, 
    bs_U32 first_instance, 
    bs_U32 num_instances)
{
    return next.bs_render(batch, pipeline, vertex_offset, vertex_count, first_instance, num_instances);
}

bs_Result bs_barrier(
    bs_U32 dependency_flags, 
    bs_U32 src, 
    bs_U32 dst, 
    bs_U32 src_access, 
    bs_U32 dst_access)
{
    return next.bs_barrier(dependency_flags, src, dst, src_access, dst_access);
}

bs_Result bs_rayTrace(
    bs_RayTracer* ray_tracer, 
    bs_Pipeline* pipeline, 
    bs_U32 width, 
    bs_U32 height, 
    bs_U32 depth)
{
    return next.bs_rayTrace(ray_tracer, pipeline, width, height, depth);
}

bs_Result bs_rayTracer(
    bs_Object* object, 
    bs_U32 flags, 
    ...)
{
    return next.bs_rayTracer(object, flags, ...);
}

bs_Result bs_accelerateAabb(
    bs_RayTracer* ray_tracer, 
    bs_Aabb aabb)
{
    return next.bs_accelerateAabb(ray_tracer, aabb);
}

bs_Result bs_accelerateBatch(
    bs_RayTracer* ray_tracer, 
    bs_Batch* batch)
{
    return next.bs_accelerateBatch(ray_tracer, batch);
}

bs_Result bs_build(
    bs_RayTracer* ray_tracer)
{
    return next.bs_build(ray_tracer);
}

bs_Result bs_destroyRayTracer(
    bs_RayTracer* ray_tracer)
{
    return next.bs_destroyRayTracer(ray_tracer);
}

bs_Result bs_dispatchAsync(
    bs_Pipeline* pipeline, 
    bs_U32 x, 
    bs_U32 y, 
    bs_U32 z)
{
    return next.bs_dispatchAsync(pipeline, x, y, z);
}

int bs_bufferSwaps(
    bs_Buffer* buffer)
{
    return next.bs_bufferSwaps(buffer);
}

bs_Result bs_nameBuffer(
    bs_Buffer* buffer, 
    char* value, 
    int value_length)
{
    return next.bs_nameBuffer(buffer, value, value_length);
}

bs_Result bs_nameBufferV(
    bs_Buffer* buffer, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_nameBuffer(buffer, string->value, string->len);
}

bs_Result bs_nameBufferF(
    bs_Buffer* buffer, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_nameBufferV(buffer, format, args);
    va_end(args);
    return _return;
}

bs_Result bs_buffer(
    bs_Object* object, 
    bs_U32 num_bytes, 
    bs_BufferUsageFlags usage_flags, 
    bs_MemoryPropertyFlags memory_flags, 
    bs_BufferBits flags)
{
    return next.bs_buffer(object, num_bytes, usage_flags, memory_flags, flags);
}

char* bs_bufferMap(
    bs_Buffer* buffer)
{
    return next.bs_bufferMap(buffer);
}

bs_Result bs_mapBuffer(
    bs_Buffer* buffer, 
    bs_U32 num_bytes)
{
    return next.bs_mapBuffer(buffer, num_bytes);
}

bs_Result bs_unmapBuffer(
    bs_Buffer* buffer)
{
    return next.bs_unmapBuffer(buffer);
}

bs_Result bs_stageNull(
    bs_Buffer* buffer)
{
    return next.bs_stageNull(buffer);
}

bs_Result bs_stageList(
    bs_Buffer* buffer, 
    bs_List* list)
{
    return next.bs_stageList(buffer, list);
}

bs_Result bs_stageImage(
    bs_Buffer* buffer, 
    bs_Format format, 
    bs_ivec2 dim, 
    const char* data)
{
    return next.bs_stageImage(buffer, format, dim, data);
}

bs_Result bs_destroyBuffer(
    bs_Buffer* buffer)
{
    return next.bs_destroyBuffer(buffer);
}

bs_Result bs_copyAsync(
    bs_Buffer* src, 
    bs_Buffer* dst, 
    bs_U32 src_offset, 
    bs_U32 dst_offset, 
    bs_U32 num_bytes)
{
    return next.bs_copyAsync(src, dst, src_offset, dst_offset, num_bytes);
}

bs_Result bs_setBufferAsync(
    bs_Buffer* buffer, 
    bs_U32 offset, 
    bs_U32 num_bytes, 
    bs_U32 value)
{
    return next.bs_setBufferAsync(buffer, offset, num_bytes, value);
}

bs_Result bs_batch(
    bs_Object* object, 
    int index_size, 
    bs_Shader* vertex_shader, 
    bs_BatchBits flags)
{
    return next.bs_batch(object, index_size, vertex_shader, flags);
}

bs_Attribute* bs_queryAttribute(
    bs_Batch* batch, 
    bs_Batch* , 
    char* value, 
    int value_length)
{
    return next.bs_queryAttribute(batch, , value, value_length);
}

bs_Attribute* bs_queryAttributeV(
    bs_Batch* batch, 
    bs_Batch* , 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_queryAttribute(batch, , string->value, string->len);
}

bs_Attribute* bs_queryAttributeF(
    bs_Batch* batch, 
    bs_Batch* , 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Attribute* _return = bs_queryAttributeV(batch, , format, args);
    va_end(args);
    return _return;
}

bool bs_batchIsPushed(
    bs_Batch* batch)
{
    return next.bs_batchIsPushed(batch);
}

bool bs_batchIsIndexed(
    bs_Batch* batch)
{
    return next.bs_batchIsIndexed(batch);
}

bs_Result bs_minimizeBatch(
    bs_Batch* batch)
{
    return next.bs_minimizeBatch(batch);
}

bs_Result bs_pushBatch(
    bs_Batch* batch, 
    bs_U32 num_index_bytes, 
    bs_U32 num_vertex_bytes)
{
    return next.bs_pushBatch(batch, num_index_bytes, num_vertex_bytes);
}

bs_Result bs_unpushBatch(
    bs_Batch* batch)
{
    return next.bs_unpushBatch(batch);
}

bs_Result bs_destroyBatch(
    bs_Batch* batch)
{
    return next.bs_destroyBatch(batch);
}

bs_Result bs_recreateBatch(
    bs_Batch* batch)
{
    return next.bs_recreateBatch(batch);
}

bs_Result bs_ensureBatchSize(
    bs_Batch* batch, 
    bs_U32 num_indices, 
    bs_U32 num_vertices)
{
    return next.bs_ensureBatchSize(batch, num_indices, num_vertices);
}

void bs_batchVertex(
    bs_VertexDeclaration* declaration, 
    const unsigned char* src)
{
    return next.bs_batchVertex(declaration, src);
}

bs_Range bs_batchRange(
    bs_Batch* batch, 
    bs_U32 offset)
{
    return next.bs_batchRange(batch, offset);
}

void bs_pushIndex(
    bs_Batch* batch, 
    bs_U32 index)
{
    return next.bs_pushIndex(batch, index);
}

void bs_pushIndexV(
    bs_Batch* batch, 
    bs_U32 num_indices, 
    ...)
{
    return next.bs_pushIndexV(batch, num_indices, ...);
}

bs_Range bs_batchCube(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_RGBA color)
{
    return next.bs_batchCube(batch, offset, color);
}

bs_Range bs_pushCube(
    bs_Batch* batch, 
    bs_RGBA color)
{
    return next.bs_pushCube(batch, color);
}

bs_Range bs_batchCone(
    bs_Batch* batch, 
    bs_U32* offset, 
    int segments, 
    float height, 
    float radius, 
    bs_RGBA color)
{
    return next.bs_batchCone(batch, offset, segments, height, radius, color);
}

bs_Range bs_pushCone(
    bs_Batch* batch, 
    int segments, 
    float height, 
    float radius, 
    bs_RGBA color)
{
    return next.bs_pushCone(batch, segments, height, radius, color);
}

bs_Range bs_batchRectangle(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 position, 
    bs_vec2 dimensions, 
    bs_vec2 texture_offset, 
    bs_vec2 texture_coords, 
    bs_RGBA color)
{
    return next.bs_batchRectangle(batch, offset, position, dimensions, texture_offset, texture_coords, color);
}

bs_Range bs_pushRectangle(
    bs_Batch* batch, 
    bs_vec3 position, 
    bs_vec2 dimensions, 
    bs_vec2 texture_offset, 
    bs_vec2 texture_coords, 
    bs_RGBA color)
{
    return next.bs_pushRectangle(batch, position, dimensions, texture_offset, texture_coords, color);
}

bs_Range bs_batchQuad(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_Quad quad, 
    bs_RGBA color)
{
    return next.bs_batchQuad(batch, offset, quad, color);
}

bs_Range bs_pushQuad(
    bs_Batch* batch, 
    bs_Quad quad, 
    bs_RGBA color)
{
    return next.bs_pushQuad(batch, quad, color);
}

bs_Range bs_batchTriangle(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 a, 
    bs_vec3 b, 
    bs_vec3 c, 
    bs_RGBA color)
{
    return next.bs_batchTriangle(batch, offset, a, b, c, color);
}

bs_Range bs_pushTriangle(
    bs_Batch* batch, 
    bs_vec3 a, 
    bs_vec3 b, 
    bs_vec3 c, 
    bs_RGBA color)
{
    return next.bs_pushTriangle(batch, a, b, c, color);
}

bs_Range bs_batchLine(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 start, 
    bs_vec3 end, 
    bs_RGBA color)
{
    return next.bs_batchLine(batch, offset, start, end, color);
}

bs_Range bs_pushLine(
    bs_Batch* batch, 
    bs_vec3 start, 
    bs_vec3 end, 
    bs_RGBA color)
{
    return next.bs_pushLine(batch, start, end, color);
}

bs_Range bs_batchRay(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_Ray* ray, 
    bs_RGBA color)
{
    return next.bs_batchRay(batch, offset, ray, color);
}

bs_Range bs_pushRay(
    bs_Batch* batch, 
    bs_Ray* ray, 
    bs_RGBA color)
{
    return next.bs_pushRay(batch, ray, color);
}

bs_Range bs_batchPoint(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 position, 
    bs_RGBA color)
{
    return next.bs_batchPoint(batch, offset, position, color);
}

bs_Range bs_pushPoint(
    bs_Batch* batch, 
    bs_vec3 position, 
    bs_RGBA color)
{
    return next.bs_pushPoint(batch, position, color);
}

bs_Range bs_batchAabb(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_Aabb* aabb, 
    bs_RGBA color)
{
    return next.bs_batchAabb(batch, offset, aabb, color);
}

bs_Range bs_pushAabb(
    bs_Batch* batch, 
    bs_Aabb* aabb, 
    bs_RGBA color)
{
    return next.bs_pushAabb(batch, aabb, color);
}

bs_Range bs_batchSphere(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 position, 
    float radius, 
    bs_U32 lats, 
    bs_U32 longs, 
    bs_RGBA color)
{
    return next.bs_batchSphere(batch, offset, position, radius, lats, longs, color);
}

bs_Range bs_pushSphere(
    bs_Batch* batch, 
    bs_vec3 position, 
    float radius, 
    bs_U32 lats, 
    bs_U32 longs, 
    bs_RGBA color)
{
    return next.bs_pushSphere(batch, position, radius, lats, longs, color);
}

bs_Range bs_batchPyramid(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 pos, 
    float width, 
    float height, 
    bs_RGBA color)
{
    return next.bs_batchPyramid(batch, offset, pos, width, height, color);
}

bs_Range bs_pushPyramid(
    bs_Batch* batch, 
    bs_vec3 pos, 
    float width, 
    float height, 
    bs_RGBA color)
{
    return next.bs_pushPyramid(batch, pos, width, height, color);
}

bs_Range bs_batchBipyramid(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_vec3 pos, 
    float width, 
    float height, 
    bs_RGBA color)
{
    return next.bs_batchBipyramid(batch, offset, pos, width, height, color);
}

bs_Range bs_pushBipyramid(
    bs_Batch* batch, 
    bs_vec3 pos, 
    float width, 
    float height, 
    bs_RGBA color)
{
    return next.bs_pushBipyramid(batch, pos, width, height, color);
}

bs_Range bs_batchPrimitive(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_Primitive* primitive)
{
    return next.bs_batchPrimitive(batch, offset, primitive);
}

bs_Range bs_pushPrimitive(
    bs_Batch* batch, 
    bs_Primitive* primitive)
{
    return next.bs_pushPrimitive(batch, primitive);
}

bs_Range bs_batchMesh(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_Mesh* mesh)
{
    return next.bs_batchMesh(batch, offset, mesh);
}

bs_Range bs_pushMesh(
    bs_Batch* batch, 
    bs_Mesh* mesh)
{
    return next.bs_pushMesh(batch, mesh);
}

bs_Range bs_batchModel(
    bs_Batch* batch, 
    bs_U32* offset, 
    bs_Model* model)
{
    return next.bs_batchModel(batch, offset, model);
}

bs_Range bs_pushModel(
    bs_Batch* batch, 
    bs_Model* model)
{
    return next.bs_pushModel(batch, model);
}

bs_Result bs_renderer(
    bs_Object* object, 
    bs_RendererBits flags)
{
    return next.bs_renderer(object, flags);
}

void bs_output(
    bs_Renderer* renderer, 
    bs_Output output)
{
    return next.bs_output(renderer, output);
}

void bs_input(
    bs_Renderer* renderer, 
    bs_Input input)
{
    return next.bs_input(renderer, input);
}

void bs_dependency(
    bs_Renderer* renderer, 
    bs_U32 src_subpass, 
    bs_U32 dst_subpass, 
    bs_DependencyFlags flags, 
    bs_PipelineStage src_stage, 
    bs_PipelineStage dst_stage, 
    bs_AccessMask src_access, 
    bs_AccessMask dst_access)
{
    return next.bs_dependency(renderer, src_subpass, dst_subpass, flags, src_stage, dst_stage, src_access, dst_access);
}

void bs_renderPass(
    bs_Renderer* renderer)
{
    return next.bs_renderPass(renderer);
}

void bs_framebuffer(
    bs_Renderer* renderer, 
    bs_ivec2 resolution)
{
    return next.bs_framebuffer(renderer, resolution);
}

void bs_runPass(
    bs_Renderer* renderer, 
    ...)
{
    return next.bs_runPass(renderer, ...);
}

bool bs_rendererIsDynamic(
    bs_Renderer* renderer)
{
    return next.bs_rendererIsDynamic(renderer);
}

void bs_beginRender(
    bs_Renderer* renderer)
{
    return next.bs_beginRender(renderer);
}

void bs_endRender(
    bs_Renderer* renderer)
{
    return next.bs_endRender(renderer);
}

void bs_destroyRenderer(
    bs_Renderer* renderer)
{
    return next.bs_destroyRenderer(renderer);
}

void bs_resizeRenderer(
    bs_Renderer* renderer, 
    bs_ivec2 resolution)
{
    return next.bs_resizeRenderer(renderer, resolution);
}

bs_Queue* bs_singleTimesQueue()
{
    return next.bs_singleTimesQueue();
}

bs_I32 bs_queueFamily(
    bs_QueueBits flags)
{
    return next.bs_queueFamily(flags);
}

bs_Result bs_present(
    bs_Queue* queue, 
    ...)
{
    return next.bs_present(queue, ...);
}

bs_Result bs_acquire()
{
    return next.bs_acquire();
}

void bs_awaitQueue(
    bs_Queue* queue, 
    bs_PipelineStage stage)
{
    return next.bs_awaitQueue(queue, stage);
}

void bs_awaitAcquisition()
{
    return next.bs_awaitAcquisition();
}

bs_Result bs_enqueue(
    bs_Queue* queue, 
    bs_Callback function)
{
    return next.bs_enqueue(queue, function);
}

int bs_imageIndex()
{
    return next.bs_imageIndex();
}

int bs_queueSwapsCount(
    bs_Queue* queue)
{
    return next.bs_queueSwapsCount(queue);
}

 bs_queue(
    bs_Object* object, 
    bs_QueueBits flags)
{
    return next.bs_queue(object, flags);
}

bs_Result bs_destroyQueue(
    bs_Queue* queue)
{
    return next.bs_destroyQueue(queue);
}

void bs_stallGPU()
{
    return next.bs_stallGPU();
}

void bs_stallQueue(
    bs_Queue* queue)
{
    return next.bs_stallQueue(queue);
}

bs_Result bs_stall(
    bs_Queue* queue)
{
    return next.bs_stall(queue);
}

bool bs_poll(
    bs_Queue* queue)
{
    return next.bs_poll(queue);
}

bs_Scope bs_enterSingle()
{
    return next.bs_enterSingle();
}

void bs_leaveSingle(
    bs_Scope* backup)
{
    return next.bs_leaveSingle(backup);
}

bs_Scope* bs_getScope()
{
    return next.bs_getScope();
}

void bs_setScope(
    bs_Scope* scope)
{
    return next.bs_setScope(scope);
}

void bs_runSingle(
    void (*function)())
{
    return next.bs_runSingle(void (*function)());
}

void bs_glyph(
    bs_TTF* ttf, 
    bs_U16 code)
{
    return next.bs_glyph(ttf, code);
}

void bs_ttf(
    bs_TTF* existing, 
    const char* path, 
    bs_U32 flags)
{
    return next.bs_ttf(existing, path, flags);
}

void bs_rasterizeGlyph(
    bs_TTF* font, 
    bs_Glyph* glyph, 
    int width, 
    int height, 
    char* out_bmp, 
    float scale)
{
    return next.bs_rasterizeGlyph(font, glyph, width, height, out_bmp, scale);
}

void bs_kern(
    bs_TTF* ttf)
{
    return next.bs_kern(ttf);
}

void bs_bindFont(
    bs_Font* font, 
    bs_Sampler* sampler, 
    int bind_set, 
    int bind_point)
{
    return next.bs_bindFont(font, sampler, bind_set, bind_point);
}

bs_vec2 bs_textDimensions(
    bs_Font* font, 
    char* name, 
    int length)
{
    return next.bs_textDimensions(font, name, length);
}

bs_Result bs_loadFont(
    bs_Object* object, 
    int package_id, 
    const char* resource_name, 
    const char* alphabet, 
    float spacing, 
    bs_U32 flags)
{
    return next.bs_loadFont(object, package_id, resource_name, alphabet, spacing, flags);
}

bs_Result bs_image(
    bs_Object* object, 
    bs_ivec2 dim, 
    int num_indices, 
    bs_Format format, 
    bs_U32 flags)
{
    return next.bs_image(object, dim, num_indices, format, flags);
}

int bs_imageSwapsCount(
    bs_Image* image)
{
    return next.bs_imageSwapsCount(image);
}

void bs_transition(
    bs_Image* image, 
    int index, 
    bs_ImageLayout old_layout, 
    bs_ImageLayout new_layout)
{
    return next.bs_transition(image, index, old_layout, new_layout);
}

unsigned char* bs_inspectPng(
    const char* path, 
    bs_PngData* out_png_data)
{
    return next.bs_inspectPng(path, out_png_data);
}

unsigned char* bs_loadPngMemory(
    char* data, 
    size_t size, 
    int channels_count, 
    bs_PngData* out_png_data)
{
    return next.bs_loadPngMemory(data, size, channels_count, out_png_data);
}

unsigned char* bs_loadPng(
    const char* path, 
    int channels_count, 
    bs_PngData* out_png_data)
{
    return next.bs_loadPng(path, channels_count, out_png_data);
}

bs_Result bs_bitmapImage(
    bs_Object* existing_object, 
    unsigned char* image_data, 
    bs_ivec2 dim, 
    bs_Format format, 
    bs_ImageBits flags)
{
    return next.bs_bitmapImage(existing_object, image_data, dim, format, flags);
}

bs_Result bs_savePng(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* value, 
    int value_length)
{
    return next.bs_savePng(data, resolution, type, value, value_length);
}

bs_Result bs_savePngV(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_savePng(data, resolution, type, string->value, string->len);
}

bs_Result bs_savePngF(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_savePngV(data, resolution, type, format, args);
    va_end(args);
    return _return;
}

unsigned char* bs_encodePng(
    size_t* out_size, 
    const unsigned char* data, 
    bs_ivec2 size, 
    bs_PngType type, 
    char* value, 
    int value_length)
{
    return next.bs_encodePng(out_size, data, size, type, value, value_length);
}

unsigned char* bs_encodePngV(
    size_t* out_size, 
    const unsigned char* data, 
    bs_ivec2 size, 
    bs_PngType type, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_encodePng(out_size, data, size, type, string->value, string->len);
}

unsigned char* bs_encodePngF(
    size_t* out_size, 
    const unsigned char* data, 
    bs_ivec2 size, 
    bs_PngType type, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    unsigned char* _return = bs_encodePngV(out_size, data, size, type, format, args);
    va_end(args);
    return _return;
}

bs_Result bs_savePng(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* value, 
    int value_length)
{
    return next.bs_savePng(data, resolution, type, value, value_length);
}

bs_Result bs_savePngV(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_savePng(data, resolution, type, string->value, string->len);
}

bs_Result bs_savePngF(
    char* data, 
    bs_ivec2 resolution, 
    bs_PngType type, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_savePngV(data, resolution, type, format, args);
    va_end(args);
    return _return;
}

bs_Result bs_destroyImage(
    bs_Image* image)
{
    return next.bs_destroyImage(image);
}

bs_Result bs_resizeImage(
    bs_Image* image, 
    bs_ivec2 resolution, 
    int num_indices)
{
    return next.bs_resizeImage(image, resolution, num_indices);
}

bs_Result bs_queryImageIndexHash(
    bs_Image* image, 
    bs_U64 name_hash, 
    char* name)
{
    return next.bs_queryImageIndexHash(image, name_hash, name);
}

bs_Result bs_queryImageIndex(
    bs_Image* image, 
    char* name)
{
    return next.bs_queryImageIndex(image, name);
}

bs_Result bs_copyImageToBufferAsync(
    bs_Image* image, 
    bs_Buffer* buffer, 
    int image_index, 
    bs_ImageLayout layout, 
    bs_U64 buffer_offset, 
    bs_ivec2 offset, 
    bs_ivec2 resolution)
{
    return next.bs_copyImageToBufferAsync(image, buffer, image_index, layout, buffer_offset, offset, resolution);
}

bs_Result bs_copyBufferToImage(
    bs_Buffer* buffer, 
    bs_Image* image, 
    int index, 
    bs_ImageLayout layout)
{
    return next.bs_copyBufferToImage(buffer, image, index, layout);
}

bs_Result bs_blit(
    bs_BlitOperation operation)
{
    return next.bs_blit(operation);
}

bs_Result bs_loadImage(
    bs_Object* object, 
    int package_id, 
    bs_ImageBits flags, 
    char* value, 
    int value_length)
{
    return next.bs_loadImage(object, package_id, flags, value, value_length);
}

bs_Result bs_loadImageV(
    bs_Object* object, 
    int package_id, 
    bs_ImageBits flags, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_loadImage(object, package_id, flags, string->value, string->len);
}

bs_Result bs_loadImageF(
    bs_Object* object, 
    int package_id, 
    bs_ImageBits flags, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadImageV(object, package_id, flags, format, args);
    va_end(args);
    return _return;
}

bool bs_isStencilFormat(
    bs_Format format)
{
    return next.bs_isStencilFormat(format);
}

bool bs_isDepthFormat(
    bs_Format format)
{
    return next.bs_isDepthFormat(format);
}

bool bs_hasAlpha(
    bs_Format format)
{
    return next.bs_hasAlpha(format);
}

bs_Result bs_nameImage(
    bs_Image* image, 
    const char* name)
{
    return next.bs_nameImage(image, name);
}

bs_Result bs_destroySampler(
    bs_Sampler* sampler)
{
    return next.bs_destroySampler(sampler);
}

bs_Result bs_sampler(
    bs_Object* object, 
    bs_ImageFilter filter, 
    bs_SamplerBits flags)
{
    return next.bs_sampler(object, filter, flags);
}

bs_Result bs_loadAtlas(
    bs_Object* object, 
    int package_id, 
    bs_U32 flags, 
    char* value, 
    int value_length)
{
    return next.bs_loadAtlas(object, package_id, flags, value, value_length);
}

bs_Result bs_loadAtlasV(
    bs_Object* object, 
    int package_id, 
    bs_U32 flags, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_loadAtlas(object, package_id, flags, string->value, string->len);
}

bs_Result bs_loadAtlasF(
    bs_Object* object, 
    int package_id, 
    bs_U32 flags, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadAtlasV(object, package_id, flags, format, args);
    va_end(args);
    return _return;
}

bs_vec4 bs_atlasCoordinates(
    bs_Atlas* atlas, 
    int texture_id, 
    int frame)
{
    return next.bs_atlasCoordinates(atlas, texture_id, frame);
}

bs_vec4 bs_mirrorUV(
    bs_vec4 uv)
{
    return next.bs_mirrorUV(uv);
}

bs_vec4 bs_flipUV(
    bs_vec4 uv)
{
    return next.bs_flipUV(uv);
}

bs_vec2 bs_atlasSize(
    bs_Atlas* atlas, 
    int texture)
{
    return next.bs_atlasSize(atlas, texture);
}

int bs_queryAtlasHash(
    bs_Atlas* atlas, 
    bs_U64 hash, 
    const char* name)
{
    return next.bs_queryAtlasHash(atlas, hash, name);
}

int bs_queryAtlas(
    bs_Atlas* atlas, 
    const char* name)
{
    return next.bs_queryAtlas(atlas, name);
}

bs_Result bs_destroyAtlas(
    bs_Atlas* atlas)
{
    return next.bs_destroyAtlas(atlas);
}

void bs_splitAtlasTexture(
    bs_Atlas* atlas, 
    char* name, 
    int split)
{
    return next.bs_splitAtlasTexture(atlas, name, split);
}

bs_Result bs_loadAtlasMemory(
    bs_Object* object, 
    int package_id, 
    char* resource_name, 
    char* data, 
    bs_U32 flags)
{
    return next.bs_loadAtlasMemory(object, package_id, resource_name, data, flags);
}

bs_Result bs_loadAtlas(
    bs_Object* object, 
    int package_id, 
    const char* resource_name, 
    bs_U32 flags)
{
    return next.bs_loadAtlas(object, package_id, resource_name, flags);
}

void bs_parseArgs(
    int argc, 
    char* argv[])
{
    return next.bs_parseArgs(argc, argv[]);
}

bs_Args* bs_arguments()
{
    return next.bs_arguments();
}

void bs_ini()
{
    return next.bs_ini();
}

void bs_load(
    bs_Callback load_resources)
{
    return next.bs_load(load_resources);
}

void bs_queryProcedures(
    bs_Procedure* procedures, 
    int count, 
    void* dll_handle, 
    unsigned char* destination)
{
    return next.bs_queryProcedures(procedures, count, dll_handle, destination);
}

struct VkCommandBuffer_T* bsi_fetchCommands()
{
    return next.bsi_fetchCommands();
}

struct VkDevice_T* bsi_fetchDevice()
{
    return next.bsi_fetchDevice();
}

void bsi_resizeObjects()
{
    return next.bsi_resizeObjects();
}

void bsi_resetQueue(
    bs_Queue* queue)
{
    return next.bsi_resetQueue(queue);
}

void bsi_pushQueue(
    bs_Queue* queue)
{
    return next.bsi_pushQueue(queue);
}

bs_Result bsi_nameHandle(
    bs_U64 handle, 
    bs_U32 type, 
    char* value, 
    int value_length)
{
    return next.bsi_nameHandle(handle, type, value, value_length);
}

bs_Result bsi_nameHandleV(
    bs_U64 handle, 
    bs_U32 type, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bsi_nameHandle(handle, type, string->value, string->len);
}

bs_Result bsi_nameHandleF(
    bs_U64 handle, 
    bs_U32 type, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bsi_nameHandleV(handle, type, format, args);
    va_end(args);
    return _return;
}

bs_Procedures* bs_procedures()
{
    return next.bs_procedures();
}

bs_JsonEnumeration bs_beginEnumeration(
    bs_Json* json)
{
    return next.bs_beginEnumeration(json);
}

void bs_enumerateJson(
    bs_Json* json, 
    bs_JsonEnumeration* e)
{
    return next.bs_enumerateJson(json, e);
}

bs_Json bs_jsonRoot(
    bs_Json* json, 
    bs_JsonObject object)
{
    return next.bs_jsonRoot(json, object);
}

void bs_ensureJsonMutable(
    bs_Json* root)
{
    return next.bs_ensureJsonMutable(root);
}

bs_Json bs_jsonCopy(
    const bs_Json* root)
{
    return next.bs_jsonCopy(root);
}

char* bs_saveJson(
    bs_Json* json, 
    bs_SaveJsonBits flags)
{
    return next.bs_saveJson(json, flags);
}

bs_Json bs_emptyJson()
{
    return next.bs_emptyJson();
}

bs_Json bs_emptyJsonArray()
{
    return next.bs_emptyJsonArray();
}

bs_Result bs_json(
    char* raw, 
    int len, 
    bs_Json* out_json)
{
    return next.bs_json(raw, len, out_json);
}

bs_Result bs_loadJson(
    char* path, 
    bs_Json* out_json, 
    char* value, 
    int value_length)
{
    return next.bs_loadJson(path, out_json, value, value_length);
}

bs_Result bs_loadJsonV(
    char* path, 
    bs_Json* out_json, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_loadJson(path, out_json, string->value, string->len);
}

bs_Result bs_loadJsonF(
    char* path, 
    bs_Json* out_json, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadJsonV(path, out_json, format, args);
    va_end(args);
    return _return;
}

bs_Result bs_destroyJson(
    bs_Json* json)
{
    return next.bs_destroyJson(json);
}

bs_JsonValue bs_parseJsonValue(
    char* raw)
{
    return next.bs_parseJsonValue(raw);
}

bs_JsonValue bs_fetchJson(
    bs_Json* root, 
    bs_JsonType expect, 
    char* value, 
    int value_length)
{
    return next.bs_fetchJson(root, expect, value, value_length);
}

bs_JsonValue bs_fetchJsonV(
    bs_Json* root, 
    bs_JsonType expect, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_fetchJson(root, expect, string->value, string->len);
}

bs_JsonValue bs_fetchJsonF(
    bs_Json* root, 
    bs_JsonType expect, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_JsonValue _return = bs_fetchJsonV(root, expect, format, args);
    va_end(args);
    return _return;
}

void bs_deleteJson(
    bs_Json* root, 
    char* value, 
    int value_length)
{
    return next.bs_deleteJson(root, value, value_length);
}

void bs_deleteJsonV(
    bs_Json* root, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_deleteJson(root, string->value, string->len);
}

void bs_deleteJsonF(
    bs_Json* root, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_deleteJsonV(root, format, args);
    va_end(args);
}

bool bs_ensureJson(
    bs_Json* root, 
    bs_JsonValue value, 
    char* value, 
    int value_length)
{
    return next.bs_ensureJson(root, value, value, value_length);
}

bool bs_ensureJsonV(
    bs_Json* root, 
    bs_JsonValue value, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_ensureJson(root, value, string->value, string->len);
}

bool bs_ensureJsonF(
    bs_Json* root, 
    bs_JsonValue value, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bool _return = bs_ensureJsonV(root, value, format, args);
    va_end(args);
    return _return;
}

bs_JsonValue bs_jsonValueFromObject(
    bs_JsonObject x)
{
    return next.bs_jsonValueFromObject(x);
}

bs_JsonValue bs_jsonValueFromRoot(
    bs_Json x)
{
    return next.bs_jsonValueFromRoot(x);
}

bs_JsonValue bs_jsonValueFromBool(
    bool x)
{
    return next.bs_jsonValueFromBool(x);
}

bs_JsonValue bs_jsonValueFromInteger(
    int x)
{
    return next.bs_jsonValueFromInteger(x);
}

bs_JsonValue bs_jsonValueFromFloat(
    double x)
{
    return next.bs_jsonValueFromFloat(x);
}

bs_JsonValue bs_jsonValueFromString(
    char* x)
{
    return next.bs_jsonValueFromString(x);
}

bs_JsonValue bs_jsonValueFromStringPointer(
    char* x)
{
    return next.bs_jsonValueFromStringPointer(x);
}

bs_JsonValue bs_jsonValueFromDateTime(
    bs_DateTime x)
{
    return next.bs_jsonValueFromDateTime(x);
}

bs_JsonValue bs_jsonObject(
    bs_Json* json)
{
    return next.bs_jsonObject(json);
}

bs_JsonValue bs_jsonArray(
    bs_JsonType type, 
    char* data, 
    int count)
{
    return next.bs_jsonArray(type, data, count);
}

bs_JsonValue bs_jsonVec2(
    bs_vec2* vector)
{
    return next.bs_jsonVec2(vector);
}

bs_JsonValue bs_jsonVec3(
    bs_vec3* vector)
{
    return next.bs_jsonVec3(vector);
}

bs_JsonValue bs_jsonVec4(
    bs_vec4* vector)
{
    return next.bs_jsonVec4(vector);
}

bs_JsonValue bs_jsonRGBA(
    bs_RGBA color)
{
    return next.bs_jsonRGBA(color);
}

char* bs_logSection(
    char* value, 
    int value_length)
{
    return next.bs_logSection(value, value_length);
}

char* bs_logSectionV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_logSection(string->value, string->len);
}

char* bs_logSectionF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_logSectionV(format, args);
    va_end(args);
    return _return;
}

char* bs_logEndOfSection()
{
    return next.bs_logEndOfSection();
}

char* bs_logWithTimestamp(
    const char* type, 
    int type_len, 
    char* value, 
    int value_length)
{
    return next.bs_logWithTimestamp(type, type_len, value, value_length);
}

char* bs_logWithTimestampV(
    const char* type, 
    int type_len, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_logWithTimestamp(type, type_len, string->value, string->len);
}

char* bs_logWithTimestampF(
    const char* type, 
    int type_len, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_logWithTimestampV(type, type_len, format, args);
    va_end(args);
    return _return;
}

char* bs_log(
    char* value, 
    int value_length)
{
    return next.bs_log(value, value_length);
}

char* bs_logV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_log(string->value, string->len);
}

char* bs_logF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_logV(format, args);
    va_end(args);
    return _return;
}

char* bs_info(
    char* value, 
    int value_length)
{
    return next.bs_info(value, value_length);
}

char* bs_infoV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_info(string->value, string->len);
}

char* bs_infoF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_infoV(format, args);
    va_end(args);
    return _return;
}

char* bs_warn(
    char* value, 
    int value_length)
{
    return next.bs_warn(value, value_length);
}

char* bs_warnV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_warn(string->value, string->len);
}

char* bs_warnF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_warnV(format, args);
    va_end(args);
    return _return;
}

void bs_critical(
    char* value, 
    int value_length)
{
    return next.bs_critical(value, value_length);
}

void bs_criticalV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_critical(string->value, string->len);
}

void bs_criticalF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_criticalV(format, args);
    va_end(args);
}

void bs_logObjectDiff()
{
    return next.bs_logObjectDiff();
}

void bs_logUnchangedObjects()
{
    return next.bs_logUnchangedObjects();
}

void bs_logBindings()
{
    return next.bs_logBindings();
}

void bs_infoF4(
    bs_mat4* m)
{
    return next.bs_infoF4(m);
}

bs_Instance* bs_instance()
{
    return next.bs_instance();
}

bs_Args* bs_args()
{
    return next.bs_args();
}

bs_Features* bs_features()
{
    return next.bs_features();
}

bs_Props* bs_props()
{
    return next.bs_props();
}

bs_Settings* bs_settings()
{
    return next.bs_settings();
}

bs_Config* bs_config()
{
    return next.bs_config();
}

void bs_system(
    char* command, 
    char* value, 
    int value_length)
{
    return next.bs_system(command, value, value_length);
}

void bs_systemV(
    char* command, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_system(command, string->value, string->len);
}

void bs_systemF(
    char* command, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_systemV(command, format, args);
    va_end(args);
}

void bs_systemAsync(
    char* command, 
    char* value, 
    int value_length)
{
    return next.bs_systemAsync(command, value, value_length);
}

void bs_systemAsyncV(
    char* command, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_systemAsync(command, string->value, string->len);
}

void bs_systemAsyncF(
    char* command, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_systemAsyncV(command, format, args);
    va_end(args);
}

const char* bs_checkStringPool(
    bs_List* pool, 
    char* string)
{
    return next.bs_checkStringPool(pool, string);
}

bs_String* bs_stringAlloc(
    bs_String* old, 
    int len)
{
    return next.bs_stringAlloc(old, len);
}

bs_String* bs_string(
    bs_String* old, 
    char* data)
{
    return next.bs_string(old, data);
}

bs_String* bs_emptyString(
    bs_String* old)
{
    return next.bs_emptyString(old);
}

bs_String* bs_string(
    bs_String* old, 
    char* value)
{
    return next.bs_string(old, value);
}

bs_String* bs_stringN(
    bs_String* old, 
    char* value, 
    int value_length)
{
    return next.bs_stringN(old, value, value_length);
}

bs_String* bs_stringF(
    bs_String* old, 
    char* format, 
    ...)
{
    return next.bs_stringF(old, format, ...);
}

bs_String* bs_stringV(
    bs_String* old, 
    char* format, 
    va_list args)
{
    return next.bs_stringV(old, format, args);
}

bs_Result bs_toUpper(
    char* string, 
    int len)
{
    return next.bs_toUpper(string, len);
}

bs_Result bs_toLower(
    char* string, 
    int len)
{
    return next.bs_toLower(string, len);
}

bs_U64 bs_hash(
    unsigned char* data, 
    size_t size)
{
    return next.bs_hash(data, size);
}

bs_U64 bs_stringHash(
    char* string)
{
    return next.bs_stringHash(string);
}

bool bs_startsWith(
    char* string, 
    const char* prefix)
{
    return next.bs_startsWith(string, prefix);
}

bool bs_endsWith(
    char* string, 
    const char* suffix)
{
    return next.bs_endsWith(string, suffix);
}

char bs_lastChar(
    char* string, 
    int len)
{
    return next.bs_lastChar(string, len);
}

bool bs_stringContainsChar(
    char* string, 
    char c)
{
    return next.bs_stringContainsChar(string, c);
}

bs_String* bs_workingDirectory()
{
    return next.bs_workingDirectory();
}

void bs_setWorkingDirectory(
    char* path)
{
    return next.bs_setWorkingDirectory(path);
}

bs_String* bs_executablePath()
{
    return next.bs_executablePath();
}

char* bs_appdataPath()
{
    return next.bs_appdataPath();
}

void bs_shortenString(
    bs_String* string, 
    int len)
{
    return next.bs_shortenString(string, len);
}

bs_String* bs_appendChar(
    bs_String* string, 
    char c)
{
    return next.bs_appendChar(string, c);
}

void bs_removeLastCharsCount(
    bs_String* string, 
    int n)
{
    return next.bs_removeLastCharsCount(string, n);
}

void bs_removeCharRange(
    bs_String* string, 
    int start, 
    int count)
{
    return next.bs_removeCharRange(string, start, count);
}

bs_String* bs_insertChar(
    bs_String* string, 
    int index, 
    char c)
{
    return next.bs_insertChar(string, index, c);
}

bs_String* bs_appendPath(
    bs_String* string, 
    char* path, 
    int path_len)
{
    return next.bs_appendPath(string, path, path_len);
}

void bs_replaceCharOccurrences(
    char* string, 
    int string_len, 
    char a, 
    char b)
{
    return next.bs_replaceCharOccurrences(string, string_len, a, b);
}

char* bs_strndup(
    const char* s, 
    size_t n)
{
    return next.bs_strndup(s, n);
}

size_t bs_strnlen(
    const char* src, 
    size_t n)
{
    return next.bs_strnlen(src, n);
}

char* bs_strsep(
    char** stringp, 
    const char* delim)
{
    return next.bs_strsep(stringp, delim);
}

void* bs_memmem(
    const void* haystack, 
    bs_U32 haystack_len, 
    const void* const needle, 
    const bs_U32 needle_len)
{
    return next.bs_memmem(haystack, haystack_len, needle, needle_len);
}

bs_U32 bs_alignUp(
    bs_U32 value, 
    bs_U32 alignment)
{
    return next.bs_alignUp(value, alignment);
}

void bs_widen(
    char* src, 
    wchar_t* dst, 
    bs_U32 dst_size)
{
    return next.bs_widen(src, dst, dst_size);
}

char* bs_charString(
    char* value, 
    int value_length)
{
    return next.bs_charString(value, value_length);
}

char* bs_charStringV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_charString(string->value, string->len);
}

char* bs_charStringF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    char* _return = bs_charStringV(format, args);
    va_end(args);
    return _return;
}

void* bs_free(
    void* p)
{
    return next.bs_free(p);
}

void* bs_malloc(
    bs_U64 size)
{
    return next.bs_malloc(size);
}

void* bs_calloc(
    bs_U64 num_units, 
    bs_U64 unit_size)
{
    return next.bs_calloc(num_units, unit_size);
}

void* bs_realloc(
    void* p, 
    bs_U64 size)
{
    return next.bs_realloc(p, size);
}

bool bs_listContains(
    bs_List* list, 
    void* data)
{
    return next.bs_listContains(list, data);
}

void* bs_fetchUnit(
    bs_List* list, 
    bs_U32 index)
{
    return next.bs_fetchUnit(list, index);
}

void* bs_fetchLast(
    bs_List* list)
{
    return next.bs_fetchLast(list);
}

void* bs_fetchLastNull(
    bs_List* list)
{
    return next.bs_fetchLastNull(list);
}

void bs_ensureSize(
    bs_List* list, 
    bs_U32 num_units)
{
    return next.bs_ensureSize(list, num_units);
}

void bs_erase(
    bs_List* list, 
    int index, 
    bs_U32 count)
{
    return next.bs_erase(list, index, count);
}

void* bs_pushBack(
    bs_List* list, 
    char* data)
{
    return next.bs_pushBack(list, data);
}

void* bs_pushBackList(
    bs_List* destination, 
    bs_List* source)
{
    return next.bs_pushBackList(destination, source);
}

void bs_destroyList(
    bs_List* list)
{
    return next.bs_destroyList(list);
}

void bs_seekList(
    bs_List* list, 
    bs_U32 unit_index)
{
    return next.bs_seekList(list, unit_index);
}

void bs_minimizeList(
    bs_List* list)
{
    return next.bs_minimizeList(list);
}

bs_List bs_list(
    int unit_size, 
    int increment)
{
    return next.bs_list(unit_size, increment);
}

void bs_guidToString(
    bs_GUID* guid, 
    char out[37])
{
    return next.bs_guidToString(guid, char out[37]);
}

bs_GUID bs_stringToGuid(
    const char* str)
{
    return next.bs_stringToGuid(str);
}

bool bs_sameGuid(
    bs_GUID* a, 
    bs_GUID* b)
{
    return next.bs_sameGuid(a, b);
}

bs_GUID bs_guid()
{
    return next.bs_guid();
}

bool bs_guidIsNull(
    bs_GUID* guid)
{
    return next.bs_guidIsNull(guid);
}

int bs_numDigits(
    int n)
{
    return next.bs_numDigits(n);
}

bool bs_directoryExists(
    char* path)
{
    return next.bs_directoryExists(path);
}

char* bs_fileExtension(
    const char* path)
{
    return next.bs_fileExtension(path);
}

bool bs_fileExtensionIs(
    const char* path, 
    char* extension)
{
    return next.bs_fileExtensionIs(path, extension);
}

char* bs_fileName(
    const char* path)
{
    return next.bs_fileName(path);
}

void bs_appendFile(
    const char* path, 
    const char* data)
{
    return next.bs_appendFile(path, data);
}

void bs_saveFile(
    char* data, 
    bs_U32 data_len, 
    char* value, 
    int value_length)
{
    return next.bs_saveFile(data, data_len, value, value_length);
}

void bs_saveFileV(
    char* data, 
    bs_U32 data_len, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_saveFile(data, data_len, string->value, string->len);
}

void bs_saveFileF(
    char* data, 
    bs_U32 data_len, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_saveFileV(data, data_len, format, args);
    va_end(args);
}

void bs_convertWin32Path(
    char* path, 
    int len, 
    char* value, 
    int value_length)
{
    return next.bs_convertWin32Path(path, len, value, value_length);
}

void bs_convertWin32PathV(
    char* path, 
    int len, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_convertWin32Path(path, len, string->value, string->len);
}

void bs_convertWin32PathF(
    char* path, 
    int len, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_convertWin32PathV(path, len, format, args);
    va_end(args);
}

void bs_ensureDirectory(
    char* value, 
    int value_length)
{
    return next.bs_ensureDirectory(value, value_length);
}

void bs_ensureDirectoryV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_ensureDirectory(string->value, string->len);
}

void bs_ensureDirectoryF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_ensureDirectoryV(format, args);
    va_end(args);
}

bs_DateTime bs_fileModifiedDate()
{
    return next.bs_fileModifiedDate();
}

void bs_setFileModifiedDate(
    const char* path, 
    bs_DateTime* date_time)
{
    return next.bs_setFileModifiedDate(path, date_time);
}

bs_DateTime bs_fileModifiedDate(
    char* value, 
    int value_length)
{
    return next.bs_fileModifiedDate(value, value_length);
}

bs_DateTime bs_fileModifiedDateV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_fileModifiedDate(string->value, string->len);
}

bs_DateTime bs_fileModifiedDateF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_DateTime _return = bs_fileModifiedDateV(format, args);
    va_end(args);
    return _return;
}

bs_String* bs_fullPath(
    bs_String* old, 
    const char* path, 
    int path_len)
{
    return next.bs_fullPath(old, path, path_len);
}

bool bs_fileExists(
    const char* path)
{
    return next.bs_fileExists(path);
}

bool bs_fileExists(
    char* value, 
    int value_length)
{
    return next.bs_fileExists(value, value_length);
}

bool bs_fileExistsV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_fileExists(string->value, string->len);
}

bool bs_fileExistsF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bool _return = bs_fileExistsV(format, args);
    va_end(args);
    return _return;
}

bs_I64 bs_toLong(
    const char* str)
{
    return next.bs_toLong(str);
}

bs_U64 bs_toULong(
    const char* str)
{
    return next.bs_toULong(str);
}

bs_F64 bs_toDouble(
    const char* str)
{
    return next.bs_toDouble(str);
}

bs_I64* bs_toLongNull(
    const char* str, 
    bs_I64* inout)
{
    return next.bs_toLongNull(str, inout);
}

bs_U64* bs_toULongNull(
    const char* str, 
    bs_U64* inout)
{
    return next.bs_toULongNull(str, inout);
}

bs_F64* bs_toDoubleNull(
    const char* str, 
    bs_F64* inout)
{
    return next.bs_toDoubleNull(str, inout);
}

bs_Resource* bs_model(
    int package_id, 
    const char* name, 
    bs_U32 flags)
{
    return next.bs_model(package_id, name, flags);
}

void bs_destroyModel(
    bs_Model* model)
{
    return next.bs_destroyModel(model);
}

bs_vec4 bs_interpolateRotation(
    bs_AnimationBone* animation_joint, 
    float time)
{
    return next.bs_interpolateRotation(animation_joint, time);
}

bs_vec3 bs_interpolateTranslation(
    bs_AnimationBone* animation_joint, 
    float time)
{
    return next.bs_interpolateTranslation(animation_joint, time);
}

bs_vec3 bs_interpolateScale(
    bs_AnimationBone* animation_joint, 
    float time)
{
    return next.bs_interpolateScale(animation_joint, time);
}

bs_mat4 bs_boneTransform(
    bs_Armature* armature, 
    bs_Bone* bone)
{
    return next.bs_boneTransform(armature, bone);
}

bs_vec3 bs_bonePosition(
    bs_Armature* armature, 
    bs_Bone* bone)
{
    return next.bs_bonePosition(armature, bone);
}

bs_mat4* bs_transformBone(
    bs_Armature* armature, 
    bs_Bone* bone, 
    bs_mat4 transform)
{
    return next.bs_transformBone(armature, bone, transform);
}

void bs_blendPose(
    bs_Armature* armature, 
    bs_Animation* animation_a, 
    bs_Animation* animation_b, 
    float factor, 
    float time_a, 
    float time_b)
{
    return next.bs_blendPose(armature, animation_a, animation_b, factor, time_a, time_b);
}

int bs_bone(
    bs_Armature* armature, 
    bs_mat4 local_transform, 
    int parent_id, 
    const char* name)
{
    return next.bs_bone(armature, local_transform, parent_id, name);
}

void bs_fabrik(
    bs_Armature* armature, 
    int end_effector_id, 
    bs_vec3 target, 
    int chain_length, 
    float* chain)
{
    return next.bs_fabrik(armature, end_effector_id, target, chain_length, chain);
}

void bs_bindPose(
    bs_Armature* armature)
{
    return next.bs_bindPose(armature);
}

void bs_keyframePosition(
    bs_AnimationBone* bone, 
    float timestamp, 
    bs_vec3 position)
{
    return next.bs_keyframePosition(bone, timestamp, position);
}

void bs_keyframeRotation(
    bs_AnimationBone* bone, 
    float timestamp, 
    bs_vec4 rotation)
{
    return next.bs_keyframeRotation(bone, timestamp, rotation);
}

void bs_keyframeScale(
    bs_AnimationBone* bone, 
    float timestamp, 
    bs_vec3 scale)
{
    return next.bs_keyframeScale(bone, timestamp, scale);
}

bs_Animation bs_loadAnimation(
    bs_Model* model, 
    const char* name)
{
    return next.bs_loadAnimation(model, name);
}

int bs_queryBoneId(
    bs_Armature* armature, 
    const char* name)
{
    return next.bs_queryBoneId(armature, name);
}

bs_Armature* bs_queryArmature(
    bs_Model* model, 
    const char* name)
{
    return next.bs_queryArmature(model, name);
}

bs_Bone* bs_queryBone(
    bs_Armature* armature, 
    const char* name)
{
    return next.bs_queryBone(armature, name);
}

bs_Mesh* bs_queryMesh(
    bs_Model* model, 
    const char * name)
{
    return next.bs_queryMesh(model, name);
}

bs_Mesh* bs_queryMeshHash(
    bs_Model* model, 
    bs_U64 hash, 
    const char* name)
{
    return next.bs_queryMeshHash(model, hash, name);
}

bs_Material* bs_queryMaterial(
    bs_Model* model, 
    const char* name)
{
    return next.bs_queryMaterial(model, name);
}

void bs_assertSourceIsType(
    int source_id, 
    bs_ObjectType object_type)
{
    return next.bs_assertSourceIsType(source_id, object_type);
}

const char* bs_idName(
    bs_U32 source_id, 
    bs_U32 id)
{
    return next.bs_idName(source_id, id);
}

bs_Object* bs_object(
    bs_U32 source_id, 
    bs_U32 id, 
    size_t size, 
    size_t flexible_size, 
    int flexible_count, 
    bs_U32 flags)
{
    return next.bs_object(source_id, id, size, flexible_size, flexible_count, flags);
}

bs_List* bs_packages()
{
    return next.bs_packages();
}

bs_List* bs_objectSources()
{
    return next.bs_objectSources();
}

void bs_destroyResource(
    bs_Resource* resource)
{
    return next.bs_destroyResource(resource);
}

bs_Result bs_queryResource(
    int package_id, 
    const char* name, 
    bs_Resource** out)
{
    return next.bs_queryResource(package_id, name, out);
}

bs_Result bs_queryPackage(
    const char* name, 
    int* out)
{
    return next.bs_queryPackage(name, out);
}

bs_Resource* bs_loadResource(
    int package_id, 
    const char* resource_name, 
    bs_U32 flags)
{
    return next.bs_loadResource(package_id, resource_name, flags);
}

int bs_loadPackage(
    const char* path)
{
    return next.bs_loadPackage(path);
}

int bs_configureSource(
    bs_ObjectType type, 
    int count, 
    const char** names)
{
    return next.bs_configureSource(type, count, names);
}

bool bs_exists(
    bs_U32 source_id, 
    bs_U32 id)
{
    return next.bs_exists(source_id, id);
}

bs_Object* bs_fetch(
    bs_U32 source_id, 
    bs_U32 id)
{
    return next.bs_fetch(source_id, id);
}

bool bs_shouldLoadId(
    bs_U32 source_id, 
    bs_U32 id)
{
    return next.bs_shouldLoadId(source_id, id);
}

bs_ShaderType bs_deserializeShaderType(
    const char* type_name)
{
    return next.bs_deserializeShaderType(type_name);
}

const char* bs_serializeShaderType(
    bs_ShaderType type)
{
    return next.bs_serializeShaderType(type);
}

bs_Result bs_shader(
    int package_id, 
    const char* name, 
    bs_U32 flags, 
    bs_Resource** out)
{
    return next.bs_shader(package_id, name, flags, out);
}

void bs_destroyShader(
    bs_Shader* shader)
{
    return next.bs_destroyShader(shader);
}

bs_Pipeline* bs_computePipeline(
    bs_Shader* compute_shader, 
    bs_PipelineFlags flags)
{
    return next.bs_computePipeline(compute_shader, flags);
}

void bs_destroyComputePipeline(
    bs_Pipeline* pipeline)
{
    return next.bs_destroyComputePipeline(pipeline);
}

bs_Pipeline* bs_queryPipeline(
    bs_U64 hash)
{
    return next.bs_queryPipeline(hash);
}

bs_U64 bs_pipelineHash(
    bs_PipelineHash* descriptor)
{
    return next.bs_pipelineHash(descriptor);
}

bs_Pipeline* bs_pipeline(
    bs_PipelineHash* descriptor)
{
    return next.bs_pipeline(descriptor);
}

void bs_destroyPipeline(
    bs_Pipeline* pipeline)
{
    return next.bs_destroyPipeline(pipeline);
}

void bs_pushConstant(
    bs_Pipeline* pipeline, 
    bs_U32 offset, 
    bs_U32 size, 
    void* data)
{
    return next.bs_pushConstant(pipeline, offset, size, data);
}

bs_Pipeline* bs_rayTracingPipeline(
    bs_RayTracePipelineHash* pipeline_hash)
{
    return next.bs_rayTracingPipeline(pipeline_hash);
}

bs_BindType bs_deserializeBindType(
    const char* string)
{
    return next.bs_deserializeBindType(string);
}

const char* bs_serializeBindType(
    bs_BindType type)
{
    return next.bs_serializeBindType(type);
}

void bs_loadBindings(
    int package_id, 
    const char* path)
{
    return next.bs_loadBindings(package_id, path);
}

void bs_bindImage(
    bs_U32 bind_set_slot, 
    bs_U32 binding_slot, 
    bs_Image* image, 
    bs_Sampler* sampler, 
    bs_ImageLayout layout)
{
    return next.bs_bindImage(bind_set_slot, binding_slot, image, sampler, layout);
}

void bs_bindImages(
    bs_U32 bind_set_slot, 
    bs_U32 slot, 
    bs_ImageDescriptor* images, 
    int images_count)
{
    return next.bs_bindImages(bind_set_slot, slot, images, images_count);
}

void bs_bindBuffer(
    bs_U32 bind_set_slot, 
    bs_U32 binding_slot, 
    bs_Buffer* buffer)
{
    return next.bs_bindBuffer(bind_set_slot, binding_slot, buffer);
}

void bs_bindAccelerationStructure(
    bs_U32 bind_set_slot, 
    bs_U32 slot, 
    bs_RayTracer* ray_tracer)
{
    return next.bs_bindAccelerationStructure(bind_set_slot, slot, ray_tracer);
}

void bs_pushDescriptors()
{
    return next.bs_pushDescriptors();
}

void bs_pushBindings()
{
    return next.bs_pushBindings();
}

bs_BindSet* bs_queryBindSet(
    bs_U32 id)
{
    return next.bs_queryBindSet(id);
}

bs_Binding* bs_queryBinding(
    bs_BindSet* bind_set, 
    bs_U32 id)
{
    return next.bs_queryBinding(bind_set, id);
}

void bs_configureAttribute(
    const char* name, 
    bs_Format base_format)
{
    return next.bs_configureAttribute(name, base_format);
}

bs_Window* bs_wnd()
{
    return next.bs_wnd();
}

bs_IO* bs_io()
{
    return next.bs_io();
}

int bs_timeZoneBias()
{
    return next.bs_timeZoneBias();
}

bs_DateTime bs_dateTime()
{
    return next.bs_dateTime();
}

bs_I64 bs_totalSeconds(
    const bs_DateTime* date_time)
{
    return next.bs_totalSeconds(date_time);
}

bool bs_isLaterThan(
    const bs_DateTime* a, 
    const bs_DateTime* b)
{
    return next.bs_isLaterThan(a, b);
}

bs_vec2 bs_cursorPosition()
{
    return next.bs_cursorPosition();
}

bs_ivec2 bs_getWindowPosition()
{
    return next.bs_getWindowPosition();
}

bs_vec2 bs_screenCursorPosition()
{
    return next.bs_screenCursorPosition();
}

void bs_setCursorPosition(
    int x, 
    int y)
{
    return next.bs_setCursorPosition(x, y);
}

void bs_lockCursorPosition(
    bool value)
{
    return next.bs_lockCursorPosition(value);
}

void bs_disableUserInputs(
    bool value)
{
    return next.bs_disableUserInputs(value);
}

bool bs_middleClick()
{
    return next.bs_middleClick();
}

bool bs_middleClickOnce()
{
    return next.bs_middleClickOnce();
}

bool bs_middleClickUpOnce()
{
    return next.bs_middleClickUpOnce();
}

bool bs_leftClick()
{
    return next.bs_leftClick();
}

bool bs_rightClick()
{
    return next.bs_rightClick();
}

bool bs_rightClickOnce()
{
    return next.bs_rightClickOnce();
}

bool bs_leftClickOnce()
{
    return next.bs_leftClickOnce();
}

bool bs_rightClickUpOnce()
{
    return next.bs_rightClickUpOnce();
}

bool bs_leftClickUpOnce()
{
    return next.bs_leftClickUpOnce();
}

bool bs_keyDown(
    bs_U32 code)
{
    return next.bs_keyDown(code);
}

bool bs_keyHeld(
    bs_U32 code)
{
    return next.bs_keyHeld(code);
}

bool bs_keyDownOnce(
    bs_U32 code)
{
    return next.bs_keyDownOnce(code);
}

bool bs_keyUpOnce(
    bs_U32 code)
{
    return next.bs_keyUpOnce(code);
}

bool bs_charDown(
    unsigned char c)
{
    return next.bs_charDown(c);
}

bool bs_charDownOnce(
    unsigned char c)
{
    return next.bs_charDownOnce(c);
}

bool bs_charUpOnce(
    unsigned char c)
{
    return next.bs_charUpOnce(c);
}

int bs_scroll()
{
    return next.bs_scroll();
}

void bs_resize(
    bs_U32 width, 
    bs_U32 height)
{
    return next.bs_resize(width, height);
}

bs_ivec2 bs_screenDimensions()
{
    return next.bs_screenDimensions();
}

void bs_moveWindow(
    int x, 
    int y)
{
    return next.bs_moveWindow(x, y);
}

void bs_window(
    bs_U32 width, 
    bs_U32 height, 
    const char* title, 
    ...)
{
    return next.bs_window(width, height, title, ...);
}

void bs_tick(
    bs_Callback tick, 
    bs_Callback fixed_tick)
{
    return next.bs_tick(tick, fixed_tick);
}

void bs_exit()
{
    return next.bs_exit();
}

void bs_setCursor(
    bs_CursorIcon type)
{
    return next.bs_setCursor(type);
}

void bs_maximize()
{
    return next.bs_maximize();
}

void bs_minimize()
{
    return next.bs_minimize();
}

double bs_deltaTime()
{
    return next.bs_deltaTime();
}

void bs_pause()
{
    return next.bs_pause();
}

void bs_advance()
{
    return next.bs_advance();
}

double bs_elapsedTime()
{
    return next.bs_elapsedTime();
}

bs_ivec2 bs_resolution()
{
    return next.bs_resolution();
}

bs_ivec2 bs_queryResolution()
{
    return next.bs_queryResolution();
}

void bs_title(
    const char* title, 
    ...)
{
    return next.bs_title(title, ...);
}

bool bs_inFixedTick()
{
    return next.bs_inFixedTick();
}

bs_Timer bs_timer()
{
    return next.bs_timer();
}

void bs_checkTimer(
    bs_Timer* timer)
{
    return next.bs_checkTimer(timer);
}

void bs_copyToClipboard(
    bs_Timer* timer, 
    char* value, 
    int value_length)
{
    return next.bs_copyToClipboard(timer, value, value_length);
}

void bs_copyToClipboardV(
    bs_Timer* timer, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_copyToClipboard(timer, string->value, string->len);
}

void bs_copyToClipboardF(
    bs_Timer* timer, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_copyToClipboardV(timer, format, args);
    va_end(args);
}

bs_String* bs_appendString(
    bs_String* destination, 
    char* value, 
    int value_length)
{
    return next.bs_appendString(destination, value, value_length);
}

bs_String* bs_appendStringV(
    bs_String* destination, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_appendString(destination, string->value, string->len);
}

bs_String* bs_appendStringF(
    bs_String* destination, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_String* _return = bs_appendStringV(destination, format, args);
    va_end(args);
    return _return;
}

void bs_foreachFile(
    void(*x)(bs_FileInfo, void*), 
    void* param, 
    char* value, 
    int value_length)
{
    return next.bs_foreachFile(void(*x)(bs_FileInfo, void*), param, value, value_length);
}

void bs_foreachFileV(
    void(*x)(bs_FileInfo, void*), 
    void* param, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_foreachFile(void(*x)(bs_FileInfo, void*), param, string->value, string->len);
}

void bs_foreachFileF(
    void(*x)(bs_FileInfo, void*), 
    void* param, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_foreachFileV(void(*x)(bs_FileInfo, void*), param, format, args);
    va_end(args);
}

void bs_foreachDirectory(
    void(*x)(bs_FileInfo, void*), 
    void* param, 
    char* value, 
    int value_length)
{
    return next.bs_foreachDirectory(void(*x)(bs_FileInfo, void*), param, value, value_length);
}

void bs_foreachDirectoryV(
    void(*x)(bs_FileInfo, void*), 
    void* param, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_foreachDirectory(void(*x)(bs_FileInfo, void*), param, string->value, string->len);
}

void bs_foreachDirectoryF(
    void(*x)(bs_FileInfo, void*), 
    void* param, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_foreachDirectoryV(void(*x)(bs_FileInfo, void*), param, format, args);
    va_end(args);
}

int bs_numFiles(
    char* value, 
    int value_length)
{
    return next.bs_numFiles(value, value_length);
}

int bs_numFilesV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_numFiles(string->value, string->len);
}

int bs_numFilesF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    int _return = bs_numFilesV(format, args);
    va_end(args);
    return _return;
}

int bs_numDirectories(
    char* value, 
    int value_length)
{
    return next.bs_numDirectories(value, value_length);
}

int bs_numDirectoriesV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_numDirectories(string->value, string->len);
}

int bs_numDirectoriesF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    int _return = bs_numDirectoriesV(format, args);
    va_end(args);
    return _return;
}

bs_Result bs_loadFile(
    const char* path, 
    bs_String** out, 
    char* value, 
    int value_length)
{
    return next.bs_loadFile(path, out, value, value_length);
}

bs_Result bs_loadFileV(
    const char* path, 
    bs_String** out, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_loadFile(path, out, string->value, string->len);
}

bs_Result bs_loadFileF(
    const char* path, 
    bs_String** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadFileV(path, out, format, args);
    va_end(args);
    return _return;
}

bs_Result bs_loadFileChunk(
    const char* path, 
    long offset, 
    size_t size, 
    bs_String** out, 
    char* value, 
    int value_length)
{
    return next.bs_loadFileChunk(path, offset, size, out, value, value_length);
}

bs_Result bs_loadFileChunkV(
    const char* path, 
    long offset, 
    size_t size, 
    bs_String** out, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_loadFileChunk(path, offset, size, out, string->value, string->len);
}

bs_Result bs_loadFileChunkF(
    const char* path, 
    long offset, 
    size_t size, 
    bs_String** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_loadFileChunkV(path, offset, size, out, format, args);
    va_end(args);
    return _return;
}

bs_Result bs_deleteFile(
    const char* path, 
    bs_String** out, 
    char* value, 
    int value_length)
{
    return next.bs_deleteFile(path, out, value, value_length);
}

bs_Result bs_deleteFileV(
    const char* path, 
    bs_String** out, 
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_deleteFile(path, out, string->value, string->len);
}

bs_Result bs_deleteFileF(
    const char* path, 
    bs_String** out, 
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_Result _return = bs_deleteFileV(path, out, format, args);
    va_end(args);
    return _return;
}

void bs_deleteDirectoryContents(
    char* value, 
    int value_length)
{
    return next.bs_deleteDirectoryContents(value, value_length);
}

void bs_deleteDirectoryContentsV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_deleteDirectoryContents(string->value, string->len);
}

void bs_deleteDirectoryContentsF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_deleteDirectoryContentsV(format, args);
    va_end(args);
}

void bs_deleteDirectory(
    char* value, 
    int value_length)
{
    return next.bs_deleteDirectory(value, value_length);
}

void bs_deleteDirectoryV(
    char* format, 
    va_list args)
{
    static bs_String* string;
    string = bs_stringV(string, format, args);
    return bs_deleteDirectory(string->value, string->len);
}

void bs_deleteDirectoryF(
    char* format, 
    ...)
{
    va_list args;
    va_start(args, format);
    bs_deleteDirectoryV(format, args);
    va_end(args);
}

