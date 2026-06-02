
 /**
  bs_core.h
  Buffers, batches, renderer, queues
  */

#ifndef BS_CORE_H
#define BS_CORE_H

#include <bs_types.h>
#include <windows.h>



 /*==============================================================================
  * Macros
  *============================================================================*/

#define BS_VERTEX_DECLARATION_STRUCTURE(count)  \
    struct {                                    \
        bs_Batch* batch;                        \
        bs_U32* offset;                         \
        bool populated;                         \
        int attributes_count;                   \
        struct bs_VertexDeclarationAttribute {  \
            int source_size;                    \
            int source_offset;                  \
            int destination_offset;             \
            const char* attribute_name;         \
        } attributes[count];                    \
    }

typedef BS_VERTEX_DECLARATION_STRUCTURE() bs_VertexDeclaration;

BSAPI void
bs_batchVertex(
    bs_VertexDeclaration* declaration,
    const unsigned char* src);

#define BS_VERTEX_DECLARATION_QUALIFIER(type, name) type name;
#define BS_VERTEX_ATTRIBUTE_QUALIFIER(type, name) {     \
    .source_size = sizeof(type),                        \
    .attribute_name = #name,                            \
},

#define BS_VERTEX_DECLARATION(declaration_name, batch, offset_p, ...)                               \
    typedef BS_VERTEX_DECLARATION_STRUCTURE(__VA_NARG__(__VA_ARGS__) / 2) bs_VertexDeclaration2;    \
    bs_VertexDeclaration2 declaration_name = {                                               \
        .attributes_count = __VA_NARG__(__VA_ARGS__) / 2,                                           \
        .attributes = {                                                                             \
            __VA_FOR_2__(BS_VERTEX_ATTRIBUTE_QUALIFIER, __VA_ARGS__)                                \
        }                                                                                           \
    };                                                                                              \
    typedef struct {                                                                                \
        __VA_FOR_2__(BS_VERTEX_DECLARATION_QUALIFIER, __VA_ARGS__)                                  \
    } bs_Vertex;                                                                                   \
    declaration_name.batch = batch;                                                                 \
    declaration_name.offset = offset_p;                                                             \
    if (!declaration_name.populated)                                                                \
        bs_populateVertexDeclaration(&declaration_name, batch->attributes, batch->attributes_count)

BSAPI void 
bs_populateVertexDeclaration(
    bs_VertexDeclaration* declaration, 
    bs_Attribute* attributes, 
    int attributes_count);

#define BS_NUM_CUBE_VERTICES 36
#define BS_NUM_CUBE_INDICES 36

#define BS_NUM_SLOPE_VERTICES 24
#define BS_NUM_SLOPE_INDICES 24

BSAPI bs_Swapchain* bs_swapchain();
    BSAPI int bs_currentSwap();
    BSAPI void
    bs_beginComment(
        const char* format, ...);

    BSAPI void
    bs_endComment();

    BSAPI int
    bs_swapCount();

    BSAPI bs_Image* 
    bs_swapchainImage();



  /*==============================================================================
   * Logic
   *============================================================================*/

   /**
    Sets all pixels of the current renderer's stencil buffer to a given value
    @param index - Index of the renderer's stencil output attachment
    @param resolution - The resolution to clear
    @param value
    */
    BSAPI void
    bs_clearStencil(
        bs_U32 index, 
        bs_ivec2 resolution,
        bs_U32 value);

   /**
    Sets all pixels of the current renderer's depth buffer to a given value
    @param index - Index of the renderer's depth output attachment
    @param resolution - The resolution to clear
    @param value
    */
    BSAPI void
    bs_clearDepth(
        bs_U32 index, 
        bs_ivec2 dim,
        float value);

   /**
    Sets all pixels of the current renderer's depth/stencil buffer to a given value
    @param index - Index of the renderer's depth/stencil output attachment
    @param resolution - The resolution to clear
    @param depth_value
    @param stencil_value
    */
    BSAPI void
    bs_clearDepthStencil(
        bs_U32 index,
        bs_ivec2 dim, 
        float depth_value, 
        bs_U32 stencil_value);

   /**
    Sets all pixels of a renderer's color buffer to black
    @param index - Index of the renderer's output attachment
    @param resolution - The resolution to clear
    @param color
    */
    BSAPI void
    bs_clearColor(
        bs_U32 index, 
        bs_ivec2 dim,
        bs_RGBA color);

    BSAPI void
    bs_stencilReference(
        bs_FaceType face, 
        bs_U32 reference);

   /*
    Changes the cull mode
    @param flags
    */
    BSAPI void
    bs_cull(
        bs_CullFlags flags);

    BSAPI void
    bs_setLineWidth(
        float width);

    // todo docs
    BSAPI int
    bs_batchSize(
        bs_Batch* batch);

   /**
    Draw batch vertices to the screen
    @param batch
    @param pipeline - Vertex/fragment shader to use for rendering the vertices
    @param vertex_offset
    @param vertex_count
    @param first_instance - Offset to start incrementing instance ids
    @param num_instances - Amount of instances of this range to draw
    @throws BSX_INVALID_PARAM - Batch or pipeline is NULL
    */
    BSAPI void
    bs_render(
        bs_Batch* batch,
        bs_Pipeline* pipeline,
        bs_U32 vertex_offset, 
        bs_U32 vertex_count,
        bs_U32 first_instance,
        bs_U32 num_instances);

   /**
    Synchronizes memory between calls
    @param flags
    @param src - Pipeline stage where memory is stored
    @param dst - Pipeline stage where memory is used
    @param src_access - Access mask where memory is stored
    @param dst_access - Access mask where memory is used
    */
    BSAPI void
    bs_barrier(
        bs_U32 dependency_flags,
        bs_U32 src,
        bs_U32 dst,
        bs_U32 src_access,
        bs_U32 dst_access);
    BSAPI void bs_testBarrier();
    BSAPI void bs_rayTrace(bs_RayTracer* ray_tracer, bs_Pipeline* pipeline, bs_U32 width, bs_U32 height, bs_U32 depth);
    BSAPI bs_Result bs_rayTracer(bs_Object* object, bs_U32 flags, ...);
    BSAPI void bs_accelerateAabb(bs_RayTracer* ray_tracer, bs_Aabb aabb);
    BSAPI void bs_accelerateBatch(bs_RayTracer* tracer, bs_Batch* batch);
    BSAPI bs_Result bs_build(bs_RayTracer* ray_tracer);
    BSAPI void bs_destroyRayTracer(bs_RayTracer* tracer);
   /**
    Dispatches work to a compute pipeline
    @param pipeline
    @param x - Number of workgroups
    @param y - Number of workgroups
    @param z - Number of workgroups
    @throws BSX_INVALID_PARAM - Pipeline is NULL
    */
    BSAPI void
    bs_dispatchAsync(
        bs_Pipeline* pipeline,
        bs_U32 x,
        bs_U32 y,
        bs_U32 z);



  /*==============================================================================
   * Buffer
   *============================================================================*/

   /**
    @param buffer
    @return Number of swaps the buffer has allocated
    */
    BSAPI int
    bs_bufferSwaps(
        bs_Buffer* buffer);

   /**
    @todo
    */
    BSAPI void
    bs_nameBuffer(
        bs_Buffer* buffer, 
        const char* name);

    BSAPI bs_Result
    bs_buffer(
        bs_Object* object, 
        bs_U32 num_bytes, 
        bs_BufferUsageFlags usage_flags, 
        bs_MemoryPropertyFlags memory_flags, 
        bs_BufferBits flags);

   /**
    @param buffer
    @return Pointer to the buffer's mapped data
    @throws BSX_UNMAPPED_BUFFER if buffer is not mapped
    */
    BSAPI char*
    bs_bufferMap(
        bs_Buffer* buffer);

   /**
    Creates storage for the buffer that can be edited from host side
    @param buffer
    @param num_bytes - size of the buffer to map, or BS_U32_MAX to map the entire buffer
    @return Pointer to the buffer's mapped data
    @throws BSX_INVALID_PARAM - buffer is NULL
    @throws BSX_OUT_OF_BOUNDS - num_bytes is greater than the buffer's allocated size
    */ 
    BSAPI char*
    bs_mapBuffer(
        bs_Buffer* buffer, 
        bs_U32 num_bytes);

   /**
    Deallocates the buffer map
    @param buffer
    @throws BSX_INVALID_PARAM - buffer is NULL
    */
    BSAPI void
    bs_unmapBuffer(
        bs_Buffer* buffer);

   /**
    Zeroes the entire buffer map
    @param buffer
    */
    BSAPI void
    bs_stageNull(
        bs_Buffer* buffer);

   /**
    Copies a list into the buffer map
    @param buffer
    @throws BSX_OUT_OF_BOUNDS - list size is greater than the buffer size
    */
    BSAPI void
    bs_stageList(
        bs_Buffer* buffer, 
        bs_List* list);
    
   /**
    Copies image data into the buffer map
    @throws BSX_OUT_OF_BOUNDS - image size is greater than the buffer size
    */
    BSAPI void
    bs_stageImage(
        bs_Buffer* buffer, 
        bs_Format format,
        bs_ivec2 dim, 
        const char* data);

   /**
    Unmaps and destroys all resources belonging to the buffer
    @param buffer
    @throws BSX_INVALID_PARAM - buffer is NULL
    */
    BSAPI void
    bs_destroyBuffer(
        bs_Buffer* buffer);

   /**
    Copies data from one buffer into another
    @param src - Source buffer
    @param dst - Destination buffer
    @param src_offset - Offset in bytes to begin in the source buffer 
    @param dst_offset - Offset in bytes to begin in the destination buffer
    @param num_bytes - Number of bytes to copy, or BS_U32_MAX to copy everything
    @throws BSX_OUT_OF_BOUNDS - src buffer size is greater than the dst buffer size
    */
    BSAPI void
    bs_copyAsync(
        bs_Buffer* src, 
        bs_Buffer* dst,
        bs_U32 src_offset, 
        bs_U32 dst_offset,
        bs_U32 num_bytes);

   /**
    Sets the first x bytes to the specified value
    @param buffer
    @param offset - Offset in bytes to begin in the buffer
    @param num_bytes - Number of bytes to copy, or BS_U32_MAX to copy everything
    @param value - The value to be set in the buffer
    */
    BSAPI void
    bs_setBufferAsync(
        bs_Buffer* buffer,
        bs_U32 offset,
        bs_U32 num_bytes,
        bs_U32 value);



  /*==============================================================================
   * Batch
   *============================================================================*/

   /**
    @param object
    @param index_size - Size of one index in bytes, usually sizeof(int), or 0 to not use indices
    @param vertex_shader - The vertex shader to retrieve vertex attributes from
    @param flags
    @returns Object containing the created batch
    */
    BSAPI bs_Object*
    bs_batch(
        bs_Object* object,
        int index_size,
        bs_Shader* vertex_shader,
        bs_BatchBits flags);

    BSAPI bs_Attribute*
    bs_queryAttribute(
        bs_Batch* batch,
        char* name);
    
   /**
    @param batch
    @returns true if the batch has been pushed before, otherwise false
    @throws BSX_INVALID_PARAM - batch is NULL
    */
    BSAPI bool
    bs_batchIsPushed(
        bs_Batch* batch);
    
   /**
    @param batch
    @returns true if the batch uses indices, otherwise false
    @throws BSX_INVALID_PARAM - batch is NULL
    */
    BSAPI bool
    bs_batchIsIndexed(
        bs_Batch* batch);
    
   /**
    Reallocate the vertices and indices lists to their used sizes
    @param batch
    */
    BSAPI void
    bs_minimizeBatch(
        bs_Batch* batch);

   /**
    Creates a vertex, index and staging buffer for the batch from vertex and index data populated with bs_push... functions
    @param batch 
    @param num_index_bytes - number of indices to push
    @param num_vertex_bytes - number of vertices to push
    @throws BSX_UNKNOWN_TYPE_SIZE - the batch's index unit size is not 0, 2 or 4
    @throws BSX_EMPTY_LIST - there are 0 indices and the batch has an index buffer
    @throws BSX_INVALID_PARAM - the batch is NULL
    */
    BSAPI void
    bs_pushBatch(
        bs_Batch* batch,
        bs_U32 num_index_bytes,
        bs_U32 num_vertex_bytes);

    BSAPI void
    bs_unpushBatch(
        bs_Batch* batch);
    
   /**
    @param batch
    @throws BSX_INVALID_PARAM - the batch or its resources is NULL
    */
    BSAPI void
    bs_destroyBatch(
        bs_Batch* batch);
    
    /**
    @param batch
    @throws BSX_INVALID_PARAM - the batch or its resources is NULL
    */
    BSAPI void
    bs_recreateBatch(
        bs_Batch* batch);

   /**
    Ensures that the batch has room for a certain amount of indices and vertices
    @param batch
    @param num_indices
    @param num_vertices
    */
    BSAPI void
    bs_ensureBatchSize(
        bs_Batch* batch, 
        bs_U32 num_indices,
        bs_U32 num_vertices);

    BSAPI bs_Range bs_batchRange(bs_Batch* batch, bs_U32 offset);



  /*==============================================================================
   * Batch Inserts
   *============================================================================*/

   /**
    Appends an index to the batch index array

    Does not check if the batch has room for it first
    @param batch
    @param indexs
    */
    BSAPI void
    bs_pushIndex(
        bs_Batch* batch,
        bs_U32 index);

   /**
    Appends a variadic list of indices to the batch index array

    Does not check if the batch has room for it first
    @param batch
    @param num_indices - number of indices to add
    @param ... - list of integers
    */
    BSAPI void
    bs_pushIndexV(
        bs_Batch* batch,
        bs_U32 num_indices,
        ...);

   /**
    Appends a cube to the end of the batch vertex and index array
    @param batch
    @param color
    @returns location of the cube in the batch
    */
    BSAPI bs_Range
    bs_pushCube(
        bs_Batch* batch, 
        bs_RGBA color);

    BSAPI bs_Range
    bs_pushCone(
        bs_Batch* batch,
        int segments,
        float height,
        float radius,
        bs_RGBA color);

   /**
    Appends a rectangle to the end of the batch vertex and index array
    Calls bs_pushQuad(...) internally
    @param batch
    @param position
    @param dimensions
    @param texture_offset
    @param texture_coords
    @param color
    @returns location of the rectangle in the batch
    */
    BSAPI bs_Range
    bs_pushRectangle(
        bs_Batch* batch,
        bs_vec3 position,
        bs_vec2 dimensions,
        bs_vec2 texture_offset,
        bs_vec2 texture_coords,
        bs_RGBA color);

   /**
    Appends a quad to the end of the batch vertex and index array
    @param batch
    @param quad
    @param color
    @returns location of the quad in the batch
    */
    BSAPI bs_Range
    bs_pushQuad(
        bs_Batch* batch, 
        bs_Quad quad, 
        bs_RGBA color);

   /**
    Appends a triangle to the end of the batch vertex and index array
    @param batch
    @param a
    @param b
    @param c
    @param color
    @returns location of the triangle in the batch
    */
    BSAPI bs_Range
    bs_pushTriangle(
        bs_Batch* batch, 
        bs_vec3 a, 
        bs_vec3 b, 
        bs_vec3 c, 
        bs_RGBA color);

   /**
    Appends a line to the end of the batch vertex and index array
    @param batch
    @param start
    @param end
    @param color
    @returns location of the line in the batch
    */
    BSAPI bs_Range
    bs_pushLine(
        bs_Batch* batch, 
        bs_vec3 start, 
        bs_vec3 end, 
        bs_RGBA color);

   /**
    Appends a ray to the end of the batch vertex and index array
    @param batch
    @param ray
    @param color
    @returns location of the ray in the batch
    */
    BSAPI bs_Range
    bs_pushRay(
        bs_Batch* batch, 
        bs_Ray* ray, 
        bs_RGBA color);

   /**
    Appends a point to the end of the batch vertex and index array
    @param batch
    @param position
    @param color
    @returns location of the point in the batch
    */
    BSAPI bs_Range
    bs_pushPoint(
        bs_Batch* batch, 
        bs_vec3 position, 
        bs_RGBA color);

   /**
    Appends an axis-aligned bounding box to the end of the batch vertex and index array
    @param batch
    @param aabb
    @param color
    @returns location of the AABB in the batch
    */
    BSAPI bs_Range
    bs_pushAabb(
        bs_Batch* batch, 
        bs_Aabb* aabb, 
        bs_RGBA color);

   /**
    Appends a sphere to the end of the batch vertex and index array
    @param batch
    @param sphere
    @param lats
    @param longs
    @param color
    @returns location of the sphere in the batch
    */
    BSAPI bs_Range
    bs_pushSphere(
        bs_Batch* batch, 
        bs_vec3 position, 
        float radius,
        bs_U32 lats, 
        bs_U32 longs, 
        bs_RGBA color);

    BSAPI bs_Range bs_pushPyramid(bs_Batch* batch, bs_vec3 pos, float width, float height, bs_RGBA color);
    BSAPI bs_Range bs_pushBipyramid(bs_Batch* batch, bs_vec3 pos, float width, float height, bs_RGBA color);

   /**
    Appends a primitive to the end of the batch vertex and index array
    @param batch
    @param primitive
    @returns location of the primitive in the batch
    */
    BSAPI bs_Range
    bs_pushPrimitive(
        bs_Batch* batch,
        bs_Primitive* primitive);

   /**
    Appends a mesh to the end of the batch vertex and index array
    @param batch
    @param mesh
    @returns location of the mesh in the batch
    */
    BSAPI bs_Range
    bs_pushMesh(
        bs_Batch* batch,
        bs_Mesh* mesh);

   /**
    Appends a model to the end of the batch vertex and index array
    @param batch
    @param model
    @returns location of the model in the batch
    */
    BSAPI bs_Range
    bs_pushModel(
        bs_Batch* batch,
        bs_Model* model);

    void bs_batchSphere(bs_Batch* batch, bs_U32* offset, bs_vec3 position, float radius, bs_U32 lats, bs_U32 longs, bs_RGBA color);
    void bs_batchAabb(bs_Batch* batch, bs_U32* offset, bs_Aabb* aabb, bs_RGBA color);
    void bs_batchPoint(bs_Batch* batch, bs_U32* offset, bs_vec3 pos, bs_RGBA color);
    void bs_batchLine(bs_Batch* batch, bs_U32* offset, bs_vec3 a, bs_vec3 b, bs_RGBA color);
    void bs_batchTriangle(bs_Batch* batch, bs_U32* offset, bs_vec3 a, bs_vec3 b, bs_vec3 c, bs_RGBA color);
    void bs_batchQuad(bs_Batch* batch, bs_U32* offset, bs_Quad quad, bs_RGBA color);
    void bs_batchBipyramid(bs_Batch* batch, bs_U32* offset, bs_vec3 pos, float width, float height, bs_RGBA color);
    void bs_batchPyramid(bs_Batch* batch, bs_U32* offset, bs_vec3 pos, float width, float height, bs_RGBA color);
    void bs_batchCone(bs_Batch* batch, bs_U32* offset, bs_RGBA color, int segments, float height, float radius);



  /*==============================================================================
   * Renderer
   *============================================================================*/

   /**
    @param object
    @param flags
    @returns Object containing the created renderer
    */
    BSAPI bs_Object*
    bs_renderer(
        bs_Object* object,
        bs_RendererBits flags);
   /**
    Instruct the subpass to write to an image
    @param renderer
    @param output
    @throws BSX_OUT_OF_BOUNDS - total number of outputs exceeds BS_MAX_NUM_ATTACHMENTS
    */
    BSAPI void
    bs_output(
        bs_Renderer* renderer,
        bs_Output output);

   /**
    Instruct the renderer to use a previous subpass's output attachment as an input attachment to a later subpass
    @param renderer
    @param input
    @throws BSX_OUT_OF_BOUNDS - total number of inputs exceeds BS_MAX_NUM_ATTACHMENTS
    */
    BSAPI void
    bs_input(
        bs_Renderer* renderer,
        bs_Input input);

    BSAPI void
    bs_dependency(
        bs_Renderer* renderer,
        bs_U32 src_subpass,
        bs_U32 dst_subpass,
        bs_DependencyFlags flags,
        bs_PipelineStage src_stage,
        bs_PipelineStage dst_stage,
        bs_AccessMask src_access,
        bs_AccessMask dst_access);

   /**
    Create a render pass for a renderer, this is called after the renderer's attachments are created
    @param renderer
    @throws BSX_INVALID_PARAM - renderer is NULL
    */
    BSAPI void
    bs_renderPass(
        bs_Renderer* renderer);

   /**
    Create a framebuffer for a renderer
    @param renderer
    @param resolution - resolution of the framebuffer in pixels
    @throws BSX_INVALID_PARAM - renderer is NULL
    */
    BSAPI void
    bs_framebuffer(
        bs_Renderer* renderer, 
        bs_ivec2 resolution);

   /**
    Run the render pass and all subpasses
    @param renderer
    @param ... - Variadic list of pointers to parameterless void functions for each subpass. List is ended with NULL
    @throws BSX_INVALID_PARAM - renderer is NULL
    */
    BSAPI void
    bs_runPass(
        bs_Renderer* renderer, 
        ...);

    BSAPI bool 
    bs_rendererIsDynamic(
        bs_Renderer* renderer);

    BSAPI void 
    bs_beginRender(
        bs_Renderer* renderer);

    BSAPI void 
    bs_endRender(
        bs_Renderer* renderer);

   /**
    Deallocate a renderer and all of its resources
    @param renderer
    @throws BSX_INVALID_PARAM - renderer is NULL
    */
    BSAPI void
    bs_destroyRenderer(
        bs_Renderer* renderer);

   /**
    Resize a renderer's framebuffer, this will destroy and recreate the framebuffer
    @param renderer
    @param resolution - New framebuffer resolution in pixels
    @throws BSX_INVALID_PARAM - renderer is NULL
    */
    BSAPI void
    bs_resizeRenderer(
        bs_Renderer* renderer, 
        bs_ivec2 resolution);



  /*==============================================================================
   * Queue
   *============================================================================*/

    BSAPI bs_Queue*
    bs_singleTimesQueue();

   /**
    Queries a Vulkan queue family, probably only for internal use
    @param flag
    @returns Index of the queue family or -1 if it wasn't found
    */
    BSAPI bs_I32
    bs_queueFamily(
        bs_QueueBits flags);

   /**
    Queue image for presentation, should be done at the end of the rendering pipeline
    @param queue
    @param ... - Variadic list of queues to wait on before presenting. List is ended with NULL
    */
    BSAPI void
    bs_present(
        bs_Queue* queue, 
        ...);

   /**
    Acquire the swapchain image, needs to be done before rendering
    @throws BSX_FAILED_TO_ACQUIRE
    */
    BSAPI void
    bs_acquire();

   /**
    Wait for another queue to finish submission before enqueueing
    @param queue
    @param stage
    */
    BSAPI void
    bs_awaitQueue(
        bs_Queue* queue,
        bs_PipelineStage stage);

   /**
    Wait for the swapchain to finish acquiring an image before enqueuing
    */
    BSAPI void
    bs_awaitAcquisition();

   /**
    Submit commands to a queue
    @param queue
    @param function - Pointer to a parameterless void function where the commands will be queued
    */
    BSAPI void
    bs_enqueue(
        bs_Queue* queue,
        bs_Callback function);

    BSAPI int bs_imageIndex();
    BSAPI int 
    bs_queueSwapsCount(
        bs_Queue* queue);

   /**
    Create a queue object
    @param id - Object id the queue will be assigned to
    @param flags
    @returns Object containing the created queue
    */
    BSAPI bs_Object*
    bs_queue(
        bs_Object* object,
        bs_QueueBits flags);

   /**
    Deallocate a queue and all of its resources
    @param queue
    @throws BSX_INVALID_PARAM - queue is NULL
    */
    BSAPI void
    bs_destroyQueue(
        bs_Queue* queue);

    BSAPI void
    bs_stallGPU();

    BSAPI void
    bs_stallQueue(
        bs_Queue* queue);

   /**
    Blocks code execution until all GPU operations on the specified queue has completed
    @param queue
    */
    BSAPI bool
    bs_stall(
        bs_Queue* queue);

    BSAPI bool bs_poll(bs_Queue* queue);

    BSAPI bs_Scope bs_enterSingle();
    BSAPI void bs_leaveSingle(bs_Scope* backup);
    BSAPI bs_Scope* bs_getScope();
    BSAPI void bs_setScope(bs_Scope* scope);

   /**
    Runs asynchronous code and waits for its execution immediately
    @param function - Parameterless void function where asynchronous operations will be executed
    */
    BSAPI void
    bs_runSingle(
        void (*function)());

#endif