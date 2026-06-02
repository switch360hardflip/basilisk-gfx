
 /**
  MIT License
  
  Copyright (c) 2026 HexryRo <hexryro@gmail.com>
  
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

#ifndef BS_TYPES_H
#define BS_TYPES_H

#include <bs_enums.h>
#define BSAPI _declspec(dllexport)
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    BS_RESULT_OK,

    BS_RESULT_GENERAL_ERROR,
    BS_RESULT_INTEGER_OVERFLOW,
    BS_RESULT_INVALID_BASE64_PADDING,
    BS_RESULT_INVALID_PARAM,
    BS_RESULT_INVALID_TYPE,
    BS_RESULT_CORRUPTED,
    BS_RESULT_NOT_SUPPORTED,
    BS_RESULT_NOT_IMPLEMENTED,
    BS_RESULT_FAILED_TO_QUERY,

} bs_Result;



  /*==============================================================================
   * Macros
   *============================================================================*/

    #define BS_ENUM_GEN(ENUM) ENUM,
    #define BS_STRING_GEN(STRING) #STRING,

   /**
    Number of variadic args
    */
#ifndef __VA_NARG__
    #define __VA_NARG__(...) \
         BS_ARGS_COUNT_(__VA_ARGS__,BS_RSEQ_N())
    #define BS_ARGS_COUNT_(...) \
         BS_ARG_N(__VA_ARGS__)
    #define BS_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
    #define BS_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0
#endif

    /**
     For (1 param)
     */
    #ifndef __VA_FOR__
    #define FE_0(WHAT)
    #define FE_1(WHAT, X) WHAT(X)
    #define FE_2(WHAT, X, ...) WHAT(X) FE_1(WHAT, __VA_ARGS__)
    #define FE_3(WHAT, X, ...) WHAT(X) FE_2(WHAT, __VA_ARGS__)
    #define FE_4(WHAT, X, ...) WHAT(X) FE_3(WHAT, __VA_ARGS__)
    #define FE_5(WHAT, X, ...) WHAT(X) FE_4(WHAT, __VA_ARGS__)
    #define FE_6(WHAT, X, ...) WHAT(X) FE_5(WHAT, __VA_ARGS__)
    #define FE_7(WHAT, X, ...) WHAT(X) FE_6(WHAT, __VA_ARGS__)
    #define FE_8(WHAT, X, ...) WHAT(X) FE_7(WHAT, __VA_ARGS__)
    #define FE_9(WHAT, X, ...) WHAT(X) FE_8(WHAT, __VA_ARGS__)
    #define FE_10(WHAT, X, ...) WHAT(X) FE_9(WHAT, __VA_ARGS__)
    #define FE_11(WHAT, X, ...) WHAT(X) FE_10(WHAT, __VA_ARGS__)
    #define FE_12(WHAT, X, ...) WHAT(X) FE_11(WHAT, __VA_ARGS__)
    #define FE_13(WHAT, X, ...) WHAT(X) FE_12(WHAT, __VA_ARGS__)
    #define FE_14(WHAT, X, ...) WHAT(X) FE_13(WHAT, __VA_ARGS__)
    #define FE_15(WHAT, X, ...) WHAT(X) FE_14(WHAT, __VA_ARGS__)
    #define FE_16(WHAT, X, ...) WHAT(X) FE_15(WHAT, __VA_ARGS__)

    #define GET_MACRO( \
         _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,NAME,...) NAME

    #define __VA_FOR__(action, ...) \
        GET_MACRO(_0, __VA_ARGS__, \
            FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9, \
            FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1,FE_0) \
        (action, __VA_ARGS__)
    #endif

     /**
      For (2 param)
      */
    #define FE2_0(WHAT)
    #define FE2_2(WHAT, a, b) WHAT(a, b)
    #define FE2_4(WHAT, a, b, ...) WHAT(a, b) FE2_2(WHAT, __VA_ARGS__)
    #define FE2_6(WHAT, a, b, ...) WHAT(a, b) FE2_4(WHAT, __VA_ARGS__)
    #define FE2_8(WHAT, a, b, ...) WHAT(a, b) FE2_6(WHAT, __VA_ARGS__)
    #define FE2_10(WHAT, a, b, ...) WHAT(a, b) FE2_8(WHAT, __VA_ARGS__)
    #define FE2_12(WHAT, a, b, ...) WHAT(a, b) FE2_10(WHAT, __VA_ARGS__)
    #define FE2_14(WHAT, a, b, ...) WHAT(a, b) FE2_12(WHAT, __VA_ARGS__)
    #define FE2_16(WHAT, a, b, ...) WHAT(a, b) FE2_14(WHAT, __VA_ARGS__)

    #define GET_FE2_MACRO( \
         _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,NAME,...) NAME

    #define __VA_FOR_2__(action, ...) \
        GET_FE2_MACRO(__VA_ARGS__, \
            FE2_16,FE2_16, \
            FE2_14,FE2_14, \
            FE2_12,FE2_12, \
            FE2_10,FE2_10, \
            FE2_8, FE2_8,  \
            FE2_6, FE2_6,  \
            FE2_4, FE2_4,  \
            FE2_2, FE2_2,  \
            FE2_0) \
        (action, __VA_ARGS__)


   /**
    Max number of objects
    */

    #define BS_MAX_NUM_BINDINGS                 32
    #define BS_MAX_NUM_BIND_SETS                32 // todo remove, should be 4 bound at once or something (vulkan guaranteed i think)
    #define BS_MAX_NUM_QUEUES                   8
    #define BS_MAX_NUM_SIGNALS                  8
    #define BS_MAX_NUM_WAITS                    8
    #define BS_MAX_NUM_SUBPASSES                6
    #define BS_MAX_NUM_SUBPASS_DEPENDENCIES     6
    #define BS_MAX_NUM_ATTACHMENTS              12

   /**
    Amount of time in nanoseconds it takes for the GPU to timeout
    */
    #define BS_TIMEOUT                          60000000000000

   /**
    Used for storing keystrokes
    */
    #define BS_KEYS_COUNT                         256
    #define BS_KEY_BYTES_COUNT                    ((BS_KEYS_COUNT + 8) / 8)

   /**
    Identity transformations
    */

    #define BS_MAT4_IDENTITY                    \
        (bs_mat4) {{                            \
            { 1.0, 0.0, 0.0, 0.0 },             \
            { 0.0, 1.0, 0.0, 0.0 },             \
            { 0.0, 0.0, 1.0, 0.0 },             \
            { 0.0, 0.0, 0.0, 1.0 }}}

    #define BS_MAT3_IDENTITY                    \
        (bs_mat3) {{                            \
            { 1.0, 0.0, 0.0 },                  \
            { 0.0, 1.0, 0.0 },                  \
            { 0.0, 0.0, 1.0 }}}

    #define BS_QUAT_IDENTITY                    \
        (bs_vec4) { 0.0, 0.0, 0.0, 1.0 }

   /**
    Max values
    */
    #define BS_I8_MIN                           (bs_I8 )(-127 - 1)
    #define BS_I16_MIN                          (bs_I16)(-32767 - 1)
    #define BS_I32_MIN                          (bs_I32)(-2147483647 - 1)
    #define BS_I64_MIN                          (bs_I64)(-9223372036854775807 - 1)
    #define BS_I8_MAX                           (bs_I8 )(127)
    #define BS_I16_MAX                          (bs_I16)(32767)
    #define BS_I32_MAX                          (bs_I32)(2147483647)
    #define BS_I64_MAX                          (bs_I64)(9223372036854775807)
    #define BS_U8_MAX                           (bs_U8 )(0xff)
    #define BS_U16_MAX                          (bs_U16)(0xffff)
    #define BS_U32_MAX                          (bs_U32)(0xffffffff)
    #define BS_U64_MAX                          (bs_U64)(0xffffffffffffffff)

   /**
    Command-line colors
    */
    #define BS_PRINT_BLACK                      "\033[0;30m"
    #define BS_PRINT_RED                        "\033[0;31m"
    #define BS_PRINT_RED_BRIGHT                 "\033[1;31m"
    #define BS_PRINT_GREEN                      "\033[1;32m"
    #define BS_PRINT_DARK_GREEN                 "\033[0;32m"
    #define BS_PRINT_YELLOW                     "\033[0;33m"
    #define BS_PRINT_YELLOW_BRIGHT              "\033[1;33m"
    #define BS_PRINT_BLUE                       "\033[0;34m"
    #define BS_PRINT_BLUE_BRIGHT                "\033[1;34m"
    #define BS_PRINT_MAGENTA                    "\033[0;35m"
    #define BS_PRINT_MAGENTA_BRIGHT             "\033[1;35m"
    #define BS_PRINT_CYAN                       "\033[0;36m"
    #define BS_PRINT_CYAN_BRIGHT                "\033[1;36m"
    #define BS_PRINT_WHITE                      "\033[0;37m"
    #define BS_PRINT_GRAY                       "\033[38;5;245m"
    #define BS_PRINT_RESET                      "\033[0m"

    #define BS_PRINT_COLOR(string, color) color string BS_PRINT_RESET

    #define BS_PRINT_BLACK_UNDERLINED           "\033[4;30m"
    #define BS_PRINT_RED_UNDERLINED             "\033[4;31m"
    #define BS_PRINT_GREEN_UNDERLINED           "\033[4;32m"
    #define BS_PRINT_YELLOW_UNDERLINED          "\033[4;33m"
    #define BS_PRINT_BLUE_UNDERLINED            "\033[4;34m"
    #define BS_PRINT_MAGENTA_UNDERLINED         "\033[4;35m"
    #define BS_PRINT_CYAN_UNDERLINED            "\033[4;36m"
    #define BS_PRINT_WHITE_UNDERLINED           "\033[4;37m"



  /*==============================================================================
   * Declarations
   *============================================================================*/

   /** Vectors (Declaration) */
    typedef union bs_vec2 bs_vec2;
    typedef union bs_vec3 bs_vec3;
    typedef union bs_vec4 bs_vec4;
    typedef union bs_ivec2 bs_ivec2;
    typedef union bs_ivec3 bs_ivec3;
    typedef union bs_ivec4 bs_ivec4;

   /** Matrices (Declaration) */
    typedef union bs_mat2 bs_mat2;
    typedef union bs_mat3 bs_mat3;
    typedef union bs_mat4 bs_mat4;
    typedef union bs_mat4x3 bs_mat4x3;

   /** Shapes (Declaration) */
    typedef struct bs_Quad bs_Quad;
    typedef struct bs_Plane bs_Plane;
    typedef struct bs_Box bs_Box;
    typedef struct bs_Ray bs_Ray;
    typedef struct bs_Aabb bs_Aabb;
    typedef struct bs_Sphere bs_Sphere;
    typedef struct bs_Rectangle bs_Rectangle;

   /** Colors (Declaration) */
    typedef union bs_RGBA bs_RGBA;
    typedef union bs_RGB bs_RGB;

   /** Types (Declaration) */
    typedef void (*bs_Callback)();
    //typedef struct bs_Vertex bs_Vertex;
    typedef struct bs_Timer bs_Timer;
    typedef struct bs_DateTime bs_DateTime;
    typedef struct bs_List bs_List;
    typedef struct bs_GUID bs_GUID;
    typedef struct bs_Range bs_Range;
    typedef struct bs_String bs_String;
    typedef struct bs_StringPoolEntry bs_StringPoolEntry;
    typedef struct bs_mat3SVD bs_mat3SVD;

   /** Object (Declaration) */
    typedef struct bs_Object bs_Object;
    typedef struct bs_Header bs_Header;
    typedef struct bs_Resource bs_Resource;
    typedef struct bs_ResourceHeader bs_ResourceHeader;
    typedef struct bs_ResourceHeaderData bs_ResourceHeaderData;
    typedef struct bs_ResourceParams bs_ResourceParam;
    typedef struct bs_Package bs_Package;

   /** Shader (Declaration) */
    typedef struct bs_AttributeType bs_AttributeType;
    typedef struct bs_Attribute bs_Attribute;
    typedef struct bs_Shader bs_Shader;

   /** Pipeline (Declaration) */
    typedef struct bs_Pipeline bs_Pipeline;
    typedef struct bs_PipelineHash bs_PipelineHash;
    typedef struct bs_RayTracePipelineHash bs_RayTracePipelineHash;

   /** Image (Declaration) */
    typedef struct bs_BlitOperation bs_BlitOperation;
    typedef struct bs_ImageIndex bs_ImageIndex;
    typedef struct bs_Image bs_Image;
    typedef struct bs_Sampler bs_Sampler;
    typedef struct bs_AtlasTexture bs_AtlasTexture;
    typedef struct bs_Atlas bs_Atlas;

   /** Buffer (Declaration) */
    typedef struct bs_Buffer bs_Buffer;

   /** Renderer (Declaration) */
    typedef struct bs_Input bs_Input;
    typedef struct bs_Output bs_Output;
    typedef struct bs_Renderer bs_Renderer;

   /** Batch (Declaration) */
    typedef struct bs_Batch bs_Batch;

   /** Queue (Declaration) */
    typedef struct bs_Queue bs_Queue;

   /** Ray Tracer (Declaration) */
    typedef struct bs_RayTracer bs_RayTracer;

   /** Animation (Declaration) */
    typedef struct bs_AnimationBone bs_AnimationBone;
    typedef struct bs_Animation bs_Animation;
    typedef struct bs_Bone bs_Bone;
    typedef struct bs_Armature bs_Armature;

   /** Model (Declaration) */
    typedef struct bs_Material bs_Material;
    typedef struct bs_Primitive bs_Primitive;
    typedef struct bs_Mesh bs_Mesh;
    typedef struct bs_Model bs_Model;

   /** TTF (Declaration) */
    typedef struct bs_TTF bs_TTF;
    typedef struct bs_Font bs_Font;

   /** Sound (Declaration) */
    typedef struct bs_Sound bs_Sound;

   /** JSON (Declaration) */
    typedef struct bs_JsonArray bs_JsonArray;
    typedef void* bs_JsonObject;
    typedef struct bs_Json bs_Json;
    typedef struct bs_JsonValue bs_JsonValue;
    typedef struct bs_JsonEnumeration bs_JsonEnumeration;

   /** Simulation (Declaration) */
    typedef struct bs_Simulation bs_Simulation;
    typedef struct bs_Contact bs_Contact;

   /** Http (Declaration) */
    typedef enum bs_EndpointType bs_EndpointType;
    typedef struct bs_Server bs_Server;
    typedef struct bs_Endpoint bs_Endpoint;

   /** Externals (Declaration) */
    typedef struct bs_IO bs_IO;
    typedef struct bs_Window bs_Window;
    typedef struct bs_ObjectSource bs_ObjectSource;
    typedef struct bs_Instance bs_Instance;
    typedef struct bs_Swapchain bs_Swapchain;
    typedef struct bs_Scope bs_Scope;
    typedef struct bs_Args bs_Args;
    typedef struct bs_Features bs_Features;
    typedef struct bs_Props bs_Props;
    typedef struct bs_Settings bs_Settings;

   /** Bindings (Declaration) */
    typedef struct bs_Descriptor bs_Descriptor;
    typedef struct bs_Binding bs_Binding;
    typedef struct bs_BindSet bs_BindSet;



  /*==============================================================================
   * Vectors
   *============================================================================*/

   /**
    These are unions so vectors can be accessed in different ways
    For example:
        bs_vec4 v = { 0 };

        float y_a = v.y;
        float y_b = v.a[1];
        bs_vec2 v2 = v.xy;
        bs_vec3 v3 = v.xyz;
    */

    union bs_vec2 {
        float a[2];

        struct {
            float x, y;
        };
    };

    union bs_vec3 {
        float a[3];

        struct {
            float x, y, z;
        };

        struct {
            bs_vec2 xy;
            float _;
        };

        struct {
            float __;
            bs_vec2 yz;
        };
    };

    union bs_vec4 {
        float a[4];

        struct {
            float x, y, z, w;
        };

        struct {
            bs_vec2 xy;
            bs_vec2 zw;
        };

        struct {
            float _;
            bs_vec2 yz;
            float __;
        };

        struct {
            bs_vec3 xyz;
            float ___;
        };
    };

    union bs_ivec2 {
        int a[2];

        struct {
            int x, y;
        };
    };

    union bs_ivec3 {
        int a[3];

        struct {
            int x, y, z;
        };

        struct {
            bs_ivec2 xy;
            int _;
        };

        struct {
            int __;
            bs_ivec2 yz;
        };
    };

    union bs_ivec4 {
        int a[4];

        struct {
            int x, y, z, w;
        };

        struct {
            bs_ivec2 xy;
            bs_ivec2 zw;
        };

        struct {
            int _;
            bs_ivec2 yz;
            int __;
        };

        struct {
            bs_ivec3 xyz;
            int ___;
        };
    };



  /*==============================================================================
   * Matrices
   *============================================================================*/

    union bs_mat2 {
        bs_vec2 v[2];
        float f[2 * 2];
        float a[2][2];
    };

    union bs_mat3 {
        bs_vec3 v[3];
        float f[3 * 3];
        float a[3][3];
    };

    union bs_mat4 {
        bs_vec4 v[4];
        float f[4 * 4];
        float a[4][4];
    };

    union bs_mat4x3 {
        bs_vec3 v[4];
        float f[4 * 3];
        float a[4][3];
    };



  /*==============================================================================
   * Shapes (Definition)
   *============================================================================*/

   /**
    Axis-Aligned Bounding Box
    */
    struct bs_Aabb {
        bs_vec3 min;
        bs_vec3 max;
    };

    struct bs_Sphere {
        bs_vec3 center;
        float radius;
    };

    struct bs_Rectangle {
        bs_vec2 position;
        bs_vec2 dimensions;
    };

    struct bs_Ray {
        bs_vec3 origin;
        bs_vec3 direction;
        float length;
    };

    struct bs_Quad {
        bs_vec3 a, b, c, d;     // positions
        bs_vec2 ca, cb, cc, cd; // coords
    };

    struct bs_Plane {
        bs_vec3 point;
        bs_vec3 normal;
    };

    struct bs_Box {
        bs_vec3 extent;
        bs_mat4* transform;
    };



  /*==============================================================================
   * Colors
   *============================================================================*/

   /**
    Can be accessed either as an r,g,b value between 0 and 255 or its hexadecimal value
    */
    
    #define BS_BLANK                            (bs_RGBA) {   0,   0,   0,   0 }
    #define BS_BLACK                            (bs_RGBA) {   0,   0,   0, 255 }
    #define BS_RED                              (bs_RGBA) { 255,   0,   0, 255 }
    #define BS_GREEN                            (bs_RGBA) {   0, 255,   0, 255 }
    #define BS_BLUE                             (bs_RGBA) {   0,   0, 255, 255 }
    #define BS_WHITE                            (bs_RGBA) { 255, 255, 255, 255 }
    #define BS_YELLOW                           (bs_RGBA) { 255, 255,   0, 255 }
    #define BS_MAGENTA                          (bs_RGBA) { 255,   0, 255, 255 }
    #define BS_CYAN                             (bs_RGBA) {   0, 255, 255, 255 }

    union bs_RGBA {
        struct {
            unsigned char r, g, b, a;
        };

        bs_U32 hex;
        unsigned char array[4];
    };

    union bs_RGB {
        struct {
            unsigned char r, g, b;
        };

        bs_U32 hex : 24;
    };



  /*==============================================================================
   * Types
   *============================================================================*/

    typedef struct {
        char* path;
        size_t size;
    } bs_FileInfo;

    typedef struct {
        const char* func;
        size_t size;
        bool is_required;
    } bs_Procedure;

    typedef union {
        struct {
            unsigned long low_part;
            long high_part;
        } DUMMYSTRUCTNAME;
        struct {
            unsigned long low_part;
            long high_part;
        } u;
        long long quad_part;
    } bs_BigInt;

    struct bs_Timer {
        bs_BigInt ticks_per_second;
        bs_BigInt tick_count;
        bs_BigInt last_tick_count;
        bs_U64 microseconds;
        double seconds;
    };

   /**
    Structure representing a date and time
    Current date and time can be retrieved with bs_dateTime()
    */
    struct bs_DateTime {
        unsigned int years;
        unsigned int months;
        unsigned int days;
        unsigned int hours;
        unsigned int minutes;
        unsigned int seconds;
        unsigned int milliseconds;
        unsigned int day_of_week;
    };

   /**
    128-bit GUID
    Randomly generated with bs_guid()
    Can be used to retrieve a readable XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX format with bs_guidToString(...)
    */
    struct bs_GUID {
        bs_U64 a, b;
    };

   /**
    Dynamic array of contiguously stored elements of equal size
    Created with bs_list(...)
    Append elements with bs_pushBack(...)
    */
    struct bs_List {
        int count;
        int unit_size;
        int capacity;
        int increment;
        bs_U8* data;
    };

   /**
    Singular value decomposition of a 3x3 matrix
    Created with bs_m3Svd(...)
    */
    struct bs_mat3SVD {
        bs_mat3 U;
        bs_vec3 S; // sigma
        bs_mat3 V;
    };

   /**
    Dynamic string that can be easily modified
    Created with bs_string(...), bs_stringF(...)
    */
    struct bs_String {
        int len;
        int capacity;
        char value[];
    };

    struct bs_StringPoolEntry {
        bs_U64 hash;
        const char* string;
    };

    struct bs_Range {
        bs_U32 offset;
        bs_U32 num;
        bs_Batch* batch;
    };

   // struct bs_Vertex {
   //     bs_vec4 position;
   //     bs_vec3 texture;
   //     bs_vec3 normal;
   //     bs_RGBA color;
   //     bs_ivec4 bone;
   //     bs_vec4 weight;
   // };



  /*==============================================================================
   * Object
   *============================================================================*/

    struct bs_Header {
        int id;
        int source_id;
    };


    enum {
        BS_OBJECT_SHOULD_LOAD = (1 << 0),
        BS_OBJECT_HAS_SWAPS_BIT = (1 << 1),
        BS_OBJECT_FORCE_DESTROY = (1 << 2),
        BS_OBJECT_ALREADY_EXISTS = (1 << 3),
        BS_OBJECT_WAS_CREATED = (1 << 4),
        BS_OBJECT_WAS_ALTERED = (1 << 5),
    };

   /**
    Generic type representing any object
    */
    struct bs_Object {
        bs_U32 flags;

        union {
            bs_Image* image;
            bs_Sampler* sampler;
            bs_Renderer* renderer;
            bs_Batch* batch;
            bs_Queue* queue;
            bs_Buffer* buffer;
            bs_Pipeline* pipeline;
            bs_RayTracer* ray_tracer;
            bs_Atlas* atlas;
            bs_Font* font;

            bs_Header* head;
        };
    };

    typedef enum {
        BS_RESOURCE_BINARY,
        BS_RESOURCE_IMAGE,
        BS_RESOURCE_MODEL,
        BS_RESOURCE_SHADER,
        BS_RESOURCE_SOUND,
        BS_RESOURCE_ATLAS,
        BS_RESOURCE_FONT,

        BS_RESOURCE_TYPE_COUNT,
    } bs_ResourceType;

    struct bs_ResourceParams {
        bs_U32 type;
        int unit_size;
        const char* name;
        void(*loader)(bs_Resource*, bs_U32);
    };

    struct bs_Resource {
        bs_U64 hash;
        char* name;
        bs_String* data;

        union {
            bs_Model* model;
            bs_Shader* shader;
            bs_Sound* sound;
            bs_Atlas* atlas;
            bs_Font* font;
            bs_Image* image;
        };
    };

    struct bs_ResourceHeader {
        struct bs_ResourceHeaderData {
            bs_U64 name_hash;
            int chunk;
            int offset;
            int size;
            int name_length;
        } header;

        char* name;
        bs_Resource* resource;
    };

    struct bs_Package {
        bs_U64 path_hash;
        char* path;

        bs_List resource_headers;

        bs_String* raw;
    };



  /*==============================================================================
   * Image (Definition)
   *============================================================================*/

    struct bs_BlitOperation {
        bs_Image* source;
        bs_Image* destination;
        bs_ImageLayout source_layout;
        bs_ImageLayout destination_layout;
        bs_ivec2 source_scale;
        bs_ivec2 destination_scale;
    };

    typedef bs_U32 bs_ImageBits;

    enum {
        BS_IMAGE_SWAPS_BIT                      = 1 <<  0,
        BS_IMAGE_SHADER_ACCESSIBLE_BIT          = 1 <<  4,
        BS_IMAGE_ATTACHMENT_BIT                 = 1 <<  5,
        BS_IMAGE_INPUT_ATTACHMENT_BIT           = 1 <<  6,
        BS_IMAGE_USAGE_TRANSFER_DST_BIT         = 1 <<  7,
        BS_IMAGE_USAGE_TRANSFER_SRC_BIT         = 1 <<  8,
        BS_IMAGE_AUTO_RESIZE_BIT                = 1 <<  9,
        BS_IMAGE_IS_BOUND                       = 1 << 10, // internal
        
        BS_IMAGE_USAGE_STORAGE_BIT              = 1 << 13,
    };

   /**
    Image type encompassing any vulkan image
    Created with bs_image(...), bs_loadPng(...), bs_depth(...), ...
    */
    struct bs_ImageIndex {
        char* name;
        bs_U64 name_hash;
    };

    struct bs_Image {
        bs_Header head;
        bs_ImageBits flags;

        bs_ivec2 dim;
        int num_indices; // > 0 if image array
        bs_ImageIndex* indices;

        enum VkFormat format;

        int bind_set;
        int bind_point;

        struct {
            struct VkImage_T* vk_image;
            struct VkImageView_T* view;
            struct VkDeviceMemory_T* memory;

            enum VkImageUsageFlags usage_flags;
            enum VkImageAspectFlags aspect_flags;
        }_[];
    };

   /**
    Image Format
    */
    typedef struct {
        bs_U32 magic;
        bs_U32 version;
        bs_U32 images_count;
        bs_U32 width;
        bs_U32 height;
        bs_U32 channels_count;

        bs_U32 reserved_0;
        bs_U32 reserved_1;
    } bs_BiffHeader;

    typedef struct {
        bs_U32 offset;
        bs_U32 size;
        bs_U32 name_length;

        bs_U32 reserved;
    } bs_BiffPointer;

   /**
    Atlas Format
    */
    typedef struct {
        bs_U32 magic;
        bs_U32 version;
        bs_U32 images_count;
        bs_U32 width;
        bs_U32 height;
        bs_U32 channels_count;
        bs_U32 binary_offset;

        bs_U32 reserved_0;
    } bs_BatlHeader;

    typedef struct {
        bs_U32 x;
        bs_U32 y;
        bs_U32 w;
        bs_U32 h;
        bs_U32 name_length;
        bs_U32 flags;
        int category;

        bs_U32 reserved_1;
    } bs_BatlPointer;

    enum {
        BS_ATLAS_FORCE_CREATE = 1 << 0,
    };

#define BS_ATLAS_TEXTURE_HAS_ALPHA (1 << 0)
#define BS_ATLAS_TEXTURE_IS_SOLID (1 << 1)
    struct bs_AtlasTexture {
        bs_vec4 coords;
        int x, y;
        int w, h;
        int split;
        bs_U32 flags;
    };

    struct bs_Atlas {
        bs_Header head;

        int count;

        //char* name;
        struct {
            char* name;
            bs_U64 name_hash;
            int category;
        }*unmapped;

        bs_AtlasTexture* mapped;

        bs_Image* image;
        bs_Buffer* buffer;

        struct {
            void* unused;
        }_[];
    };



  /*==============================================================================
   * Sampler (Definition)
   *============================================================================*/

    typedef bs_U32 bs_SamplerBits;

    enum {
        BS_SAMPLER_SWAPS_BIT                    = 1 << 0,
        BS_SAMPLER_MAG_NEAREST                  = 1 << 1,
        BS_SAMPLER_FORCE_CREATE                 = 1 << 2,
    };

   /**
    Used for binding an image so it can be sampled from in a fragment shader
    Created with bs_sampler(...)
    */
    struct bs_Sampler {
        bs_Header head;
        bs_SamplerBits flags;
        bs_ImageFilter filter;

        struct {
            struct VkSampler_T* vk_sampler;
        }_[];
    };



  /*==============================================================================
   * Pipeline (Definition)
   *============================================================================*/

    typedef struct {
        bs_StencilOperationType fail_op; // Stencil test failed
        bs_StencilOperationType pass_op; // Stencil test passed, depth test passed
        bs_StencilOperationType depth_fail_op; // Stencil test passed, depth test failed
        bs_ComparisonType compare_op;
        bs_U32 compare_mask;
        bs_U32 write_mask;
        bs_U32 reference;
    } bs_StencilOperation;

    typedef bs_U32 bs_PipelineFlags;
    enum {
        BS_PIPELINE_NEEDS_UPDATING = 1 << 1,
        BS_PIPELINE_FORCE_CREATE = 1 << 2,
    };

    typedef enum {
        BS_PIPELINE_GRAPHICS,
        BS_PIPELINE_COMPUTE,
        BS_PIPELINE_RAY_TRACE,

        BS_PIPELINE_TYPE_COUNT
    } bs_PipelineType;

   /**
    Represents both pipelines and compute pipelines
    Created with bs_pipeline(...), bs_computePipeline(...)
    */
    struct bs_Pipeline {
        bs_Header head;
        int shaders_count;
        bs_U64 hash;

        bs_PipelineType type;

        bs_PipelineFlags flags;
        bs_SlotBits bind_sets;
        bs_U32 shader_stages;

        int num_bind_sets;
        int constant_size;

        bs_String* name;

        struct VkPipeline_T* pipeline;
        struct VkPipelineLayout_T* layout;

        bs_Buffer* binding_table;

        struct { 
            bs_Shader* shader;
        }_[];
    };

    struct bs_PipelineHash {
        bs_PipelineFlags flags;
        int subpass;

        bs_Renderer* renderer;
        bs_Shader* shaders[2];
        bs_TopologyType topology_type;

        bs_BlendFactorType src_color_factor;
        bs_BlendFactorType dst_color_factor;
        bs_BlendFactorType src_alpha_factor;
        bs_BlendFactorType dst_alpha_factor;
        bs_BlendOperationType color_op;
        bs_BlendOperationType alpha_op;

        bs_StencilOperation stencil_front;
        bs_StencilOperation stencil_back;
        bs_CullType cull_type;
        bs_PolygonType polygon_type;

        bs_ComparisonType depth_comparison;

        struct {
            bool skip_write;
        } attachments[BS_MAX_NUM_ATTACHMENTS];

        bool restart_primitive;
        bool clamp_depth;
        bool skip_depth_test;
        bool skip_stencil_test;
        bool skip_depth_write;
        bool disable_blend;
    };

    struct bs_RayTracePipelineHash {
        bs_RayTracer* ray_tracer;
//        bs_Shader* ray_gen_shader;
//        bs_Shader* closest_hit_shader;
//        bs_Shader* miss_shader;
//        bs_Shader* intersection_shader;
//        bs_Shader* any_hit_shader;
    };



  /*==============================================================================
   * Shader (Definition)
   *============================================================================*/

    struct bs_AttributeType {
        const char* name;
        bs_U64 name_hash;
        bs_Format base_format;
    };

    struct bs_Attribute {
        char* name;
        bs_U64 name_hash;
        bs_U32 location;
        bs_U32 size;
        bs_U32 format;
        bs_U32 offset;
    };

    typedef bs_U32 bs_ShaderBits;
    enum {
        BS_SHADER_FORCE_CREATE              = 1 << 0,
        BS_SHADER_ENSURE_RETURN             = 1 << 1,
        BS_SHADER_KEEP_SPIRV                = 1 << 2,
    };

   /**
    Represents a fragment/vertex/geometry/compute shader
    Created with bs_shader(...)
    */
    struct bs_Shader {
        bs_Header head;
        bs_ShaderType type;
        bs_PipelineType pipeline_type;
        bs_SlotBits bind_sets;

        bs_U32* spirv;
        bs_U32 spirv_length;

        bs_Attribute* attributes;
        int num_attributes;
        int constant_size;

        bs_Resource* resource;

       // bs_String code;
        // const char* name;

        struct VkShaderModule_T* module;

        struct {
            void* _; // unused
        }_[];
    };



  /*==============================================================================
   * Buffer (Definition)
   *============================================================================*/

    typedef bs_U32 bs_BufferBits;
    enum {
        BSI_BUFFER_SWAPS_BIT     = 1 << 0,
        BSI_BUFFER_IS_BOUND     = 1 << 1, // internal
        /**
        Bind buffer after creation
        */
        BS_BUFFER_PRE_MAP           = 1 << 2,
        BS_BUFFER_IS_NAMED          = 1 << 3,
    };

   /**
    Holds data that can be accessed by shaders
    Created with bs_buffer(...)
    */
    struct bs_Buffer {
        bs_Header head;
        bs_BufferBits flags;

        bs_U32 bind_set;
        bs_U32 binding;

        bs_U32 num_bytes;

        bs_BufferUsageFlags usage_flags;
        bs_MemoryPropertyFlags memory_flags;

        struct {
            struct VkBuffer_T* vk_buffer;
            struct VkDeviceMemory_T* memory;
            char* data;
        }_[];
    };



  /*==============================================================================
   * Renderer (Definition)
   *============================================================================*/

   /**
    Hack to know if each subpass has depth
    */
#define BS_RENDERER_SUBPASS_HAS_DEPTH(flags, subpass) (flags & (1 << subpass))

    typedef bs_U32 bs_RendererBits;
    enum {
        BSI_RENDERER_SUBPASS_00_DEPTH_BIT   = (1 << 0), // internal
        BSI_RENDERER_SUBPASS_01_DEPTH_BIT   = (1 << 1), // internal
        BSI_RENDERER_SUBPASS_02_DEPTH_BIT   = (1 << 2), // internal
        BSI_RENDERER_SUBPASS_03_DEPTH_BIT   = (1 << 3), // internal
        BSI_RENDERER_SUBPASS_04_DEPTH_BIT   = (1 << 4), // internal
        BSI_RENDERER_SUBPASS_05_DEPTH_BIT   = (1 << 5), // internal

       /**
        Whether the renderer should automatically resize its framebuffer to match the swapchains dimensions
        */
        BS_RENDERER_AUTO_RESIZE_BIT         = (1 << 6),
        BSI_RENDERER_HAS_SWAPS_BIT          = (1 << 7),
    };
    
    typedef bs_U32 bs_OutputFlags;
    enum {
        BS_OUTPUT_SELF_DEPENDENCY = (1 << 0),
        BS_OUTPUT_EXTERNAL_DEPENDENCY = (1 << 1),
    };

   /**
    Image the renderer will write to
    */
    struct bs_Output {
        bs_OutputFlags flags;
        bs_U32 subpass;
        bs_U32 attachment;
        bs_LoadOp load_op;
        bs_StoreOp store_op;
        bs_Image* image;
        bs_ImageLayout old_layout;
        bs_ImageLayout new_layout;
    };

   /**
    Wait for the parent_subpass to finish writing before starting the next subpass
    */
    typedef bs_U32 bs_InputBits;
    enum {
        BS_INPUT_AWAIT_WRITE                = (1 << 0)
    };

   /**
    Input attachment
    After an output attachment has been written to, its contents can be read from in the next subpass
    */
    struct bs_Input {
        bs_InputBits flags;
        bs_U32 subpass;
        bs_U32 parent_subpass;
        bs_U32 attachment;
        bs_Image* image;
    };

   /**
    Used to setup the images that will be rendered into
    */
    struct bs_Renderer {
        bs_Header head;
        bs_RendererBits flags;

        bs_Input* inputs;
        bs_Output* outputs;
        struct VkSubpassDependency* dependencies;

        int num_inputs;
        int num_outputs;
        int num_subpasses;
        int num_dependencies;

        bs_ivec2 dim;

        struct VkRenderPass_T* render_pass;
        struct bs_RendererSwaps {
            struct VkFramebuffer_T* framebuffer;
        }_[];
    };



  /*==============================================================================
   * Batch (Definition)
   *============================================================================*/

    typedef bs_U32 bs_BatchBits;
    enum {
       /**
        Always recreate the batch when calling bs_batch(...)
        */
        BS_BATCH_FORCE_DESTROY              = (1 << 0),
        BS_BATCH_IS_PUSHED                  = (1 << 1),
        BS_BATCH_KEEP_DATA                  = (1 << 2),
        /**
        Whether the batch's vertex and index buffer should be double/triple buffered
        */
        BSI_BATCH_SWAPS_BIT                 = (1 << 3),
        BS_BATCH_RAY_TRACEABLE              = (1 << 4),
    };

   /**
    Collection of a vertex buffer, index buffer (optional) and staging buffer
    Created with bs_batch(...)
    */
    struct bs_Batch {
        bs_Header head;
        bs_BatchBits flags;

       /**
        Determines the way the vertex data is populated
        */

        bs_Attribute* attributes;
        bs_U32 attributes_count;
        bs_U32 attribute_flags;

       /**
        Created when batch is pushed
        */

        bs_Object* vertex_buffer;
        bs_Object* index_buffer;
        bs_Object* staging_buffer;

       /**
        Populated when adding geometry to the batch
        */

        bs_List vertices;
        bs_List indices;

        struct {
            void* _; // unused
        }_[];
    };



  /*==============================================================================
   * Queue (Definition)
   *============================================================================*/

    typedef bs_U32 bs_QueueBits;
    enum {
        BSI_QUEUE_SWAPS_BIT                  = (1 << 0),
        BS_QUEUE_GRAPHICS_BIT               = (1 << 1),
        BS_QUEUE_COMPUTE_BIT                = (1 << 2),
        BS_QUEUE_TRANSFER_BIT               = (1 << 3),
        BS_QUEUE_SINGLE_TIMES_BIT           = (1 << 4),
        BS_QUEUE_DONT_SIGNAL                = (1 << 5),
    };

   /**
    Work submitted to a renderer has to be compatible with the current queue
    Render calls have to be done in a graphics queue
    Dispatch calls have to be done in a compute queue
    Created with bs_queue(...)
    */
    struct bs_Queue {
        bs_Header head;
        bs_QueueBits flags;
        bs_U32 family;

        struct VkQueue_T* queue;
        struct {
            struct VkFence_T* fence;
            struct VkSemaphore_T* semaphore;
            struct VkCommandBuffer_T* command_buffer;
        }_[];
    };



  /*==============================================================================
   * Queue (Definition)
   *============================================================================*/

    struct bs_RayTracer {
        bs_Header head;

        bs_List aabbs;
        bs_List batches;

        struct VkAccelerationStructureKHR_T* BLAS;
        struct VkAccelerationStructureKHR_T* TLAS;

        bs_Buffer* BLAS_buffer;
        bs_Buffer* TLAS_buffer;

        bs_Buffer* BLAS_scratch_buffer;
        bs_Buffer* TLAS_scratch_buffer;

        bs_U32 record_size;
        bs_U32 groups_count;

        struct bs_ShaderGroup {
            bs_Shader* shader;
        }_[];
    };



    /*==============================================================================
     * JSON (Definition)
     *============================================================================*/

    struct bs_JsonArray {
        union {
            unsigned char* as_uchars;
            float* as_floats;
            int* as_ints;
            bs_F64* as_numbers;
            char** as_strings;
            bs_JsonObject* as_objects;
            bool* as_bools;

            bs_vec2* vec2;
            bs_vec3* vec3;
            bs_vec4* vec4;
        };
    };

    /**
     Root JSON object or array
     */
    struct bs_Json {
        bool is_mutable;
        union {
            bs_JsonObject as_object;
        };
        void* doc; // yyjson doc
    };

    struct bs_JsonValue {
        bool found;
        bs_JsonType type;
        int size;

        union bs_JsonValueUnion {
            bs_JsonArray as_array;
            bs_JsonObject as_object;
            bs_F64 as_number;
            char* as_string;
            bool as_bool;
        };
    };

    struct bs_JsonEnumeration {
        const char* key;
        bs_JsonValue value;

        struct {
            bs_U64 idx;
            bs_U64 max;
            void* cur;
            union {
                void* obj;
                struct {
                    void* mut_pre;
                    void* mut_obj;
                };
            };
        } iter;
    };



  /*==============================================================================
   * Model (Definition)
   *============================================================================*/

    typedef bs_U32 bs_ModelFlags;
    typedef enum {
        BS_MODEL_FORCE_DESTROY = 1 << 0,
        BS_MODEL_JUST_CREATED = 1 << 1,
        BS_MODEL_KEEP_JSON = 1 << 2,
    } bs_ModelFlag;

    struct bs_Material {
        char* name;
        bs_RGBA color;
    };

    struct bs_Primitive {
        float* vertices;
        int num_vertices;
        int vertex_size;

        int texture_offset;
        int normal_offset;
        int bone_offset;
        int weight_offset;
        int index_offset;

        bs_U32 num_indices;
        bs_U32* indices;

        int material_id;

        bs_Mesh* parent;
        bs_Aabb aabb;

        int* extra;
    };

    struct bs_Mesh {
        char* name;
        bs_U64 name_hash;

        bs_vec3 position;
        bs_vec4 rotation;
        bs_vec3 scale;

        bs_Primitive* primitives;
        int primitives_count;

        int num_vertices;
        int num_indices;

        bs_Model* model;
        bs_Aabb aabb;

        int* extra;
    };

   /**
    Root document representing a GLTF model
    */
    struct bs_Model {
        bs_Header head;
        bs_ModelFlags flags;

      //  bs_Animation* animations;
        bs_Armature* armatures;
        bs_Mesh* meshes;
        bs_Material* materials;

        bs_U64* animation_hashes;
        int animations_count;
        int armatures_count;
        int meshes_count;
        int materials_count;
        int primitives_count;

        int num_vertices;
        int num_indices;

        bs_Json json;
        bs_Aabb aabb;

        unsigned char* gltf;
        int* extra;

        struct {
            void* _; // unused
        }_[];
    };



  /*==============================================================================
   * Animation (Definition)
   *============================================================================*/

    struct bs_Bone {
        bs_U32 id;
        int parent_idx;
        int opposite_id;
        int name_length;
        char* name;
        bs_U64 name_hash;

        bs_mat4 local_matrix; // bind matrix * local
        bs_mat4 bind_matrix_inverse; // transformation into bone space (parent joint is origin)
    };

    typedef bs_U32 bs_ArmatureFlags;
    typedef enum {
        BS_ARMATURE_FORCE_DESTROY = 1 << 0,
    } bs_ArmatureFlag;

   /**
    Model's skeletal structure
    Created with bs_armature(...)
    Queried with bs_queryArmature(...)
    */
    struct bs_Armature {
        struct {
            float ik_length;
            int ik_id;
            bs_vec3 ik_direction;
            bs_vec3 ik_position;
            bs_mat4 matrix;
            bs_Bone bone;
        }* bones;

        int bones_count;
        int bones_allocated;

        char* name;

        struct {
            void*_;
        }_[];
    };

    struct bs_AnimationBone {
        struct {
            float time;
            bs_vec3 value;
        }* translations;

        struct {
            float time;
            bs_vec4 value;
        }* rotations;

        struct {
            float time;
            bs_vec3 value;
        }* scalings;

        bs_vec3 last_translation;
        bs_vec4 last_rotation;
        bs_vec3 last_scale;

        int translations_count, translations_allocated;
        int rotations_count, rotations_allocated;
        int scalings_count, scalings_allocated;
    };

    typedef bs_U32 bs_AnimationFlags;
    typedef enum {
        BS_ANIMATION_FORCE_DESTROY = 1 << 0,
    } bs_AnimationFlag;

   /**
    Created with bs_animation(...)
    */
    struct bs_Animation {
        char* name;

        bs_AnimationBone* bones;
        int bones_count;
        int bones_allocated;
        int frames_count;

        float length;
    };



  /*==============================================================================
   * Sound (Definition)
   *============================================================================*/

   /**
    Windows only (xaudio2)
    Created with bs_sound(...)
    */
    struct bs_Sound {
        bs_Header head;
        const char* name;

        void* data;
        void* xaudio;
        int size;

        struct {
            void* _; // unused
        }_[];
    };



  /*==============================================================================
   * TTF (Definition)
   *============================================================================*/

    typedef struct {
        bs_U16 advance_width;
        bs_I16 left_side_bearing;
    } bs_LongHorMetric;

    typedef struct {
        bs_I16 x, y;
        bool on_curve;
    } bs_GlyfPt;

    typedef struct {
        int index;
        union {
            bs_U16 code;
            char ascii;
        };
        bs_U16 num_points;
        bs_U16 num_contours;

        bs_I16 x_min, x_max;
        bs_I16 y_min, y_max;

        bs_U16* contours;
        bs_GlyfPt* coords;

        bs_vec2 tex_coord;
        bs_vec2 tex_offset;
        int width;
        int height;

        bs_U16 pairs_count;
        bs_U16 pairs_offset;

        bs_LongHorMetric long_hor_metric;
    } bs_Glyph;

    typedef struct {
        char* buf;
        int units_per_em;
        bs_I16 index_to_loc_format;
    } bs_Head;

    typedef struct {
        char* buf;
        bs_U16 num_glyphs;
    } bs_Maxp;

    typedef struct {
        char* buf;
        bs_U16 num_of_long_hor_metrics;
    } bs_Hhea;

    typedef struct {
        char* buf;
    } bs_Hmtx;

    typedef struct {
        char* buf;
    } bs_Loca;

    typedef struct {
        char* buf;
    } bs_Glyf;

    typedef struct {
        char* buf;
        bs_U32 offset;

        int num_subtables;
        char* subtable;

        bs_U16 format;
        bs_U16 length;
        bs_U16 seg_count_x2;
        bs_U16 search_range;
        bs_U16 entry_selector;
        bs_U16 range_shift;

        bs_U16* format_data;
        bs_U16* end_code;
        bs_U16* start_code;
        bs_I16* id_delta;
        bs_U16* id_range_offset;
    } bs_Cmap;

    typedef struct {
        int right_index;
        bs_U16 right;
        bs_I16 value;
    } bs_KerningPair;

   /**
    This will be replaced soon
    14kb??
    */
    struct bs_TTF {
        bs_Header header;
        bs_U32 offset;

       // char* atlas;
        //int atlas_size;

        bs_String* buffer;
        const char* name;
        const char* alphabet;
       // bs_Glyph lookup[256];
        bs_List glyphs; // bs_Glyph
        bs_List kerning_pairs; // bs_KerningPair

        //float scale;
        bs_U16 table_count;

        int x_offset;
        int y_offset;

        // tables
        bs_Head head;
        bs_Maxp maxp;
        bs_Hhea hhea;
        bs_Hmtx hmtx;
        bs_Loca loca;
       // bs_Cmap cmap;
        bs_Glyf glyf;

        struct {
            void* _; // unused
        }_[];
    };

   /**
    Font Format
    */
    typedef struct {
        bs_U32 magic;
        bs_U32 version;
        bs_U32 glyphs_count;
        bs_U32 kerning_pairs_count;
        bs_U32 line_height;
        bs_U32 units_per_em;
        bs_U32 size;
        bs_U32 batl_offset;

        char ascii_table[256];
    } bs_BfntHeader;

    typedef struct {
        int right;
        int value;
    } bs_BfntKerningPair;

    typedef struct {
        bs_U32 code;

        int y_offset;
        int advance_width;
        int left_side_bearing;

        bs_U32 kerning_pair_offset;
        bs_U32 kerning_pair_count;

        bs_U32 reserved_0;
        bs_U32 reserved_1;
    } bs_BfntGlyph;

    struct bs_Font {
        bs_Header head;

//        const char* shader_path;
        const char* alphabet;
        bs_Atlas* atlas;

        int min_y_shift;
        int size;
        int units_per_em;
        int height;
        float spacing;

        int pairs_count;
        struct {
            int right;
            float value;
        }*pairs;

        struct {
            int y_offset;
            float advance_width;
            float left_side_bearing;
            int kerning_pair_offset;
            int kerning_pair_count;
        }*glyphs;

        //bs_Shader* fragment_shader;
        char table[256];

        struct {
            void* unused;
        }_[];
    };




  /*==============================================================================
   * Http (Definition)
   *============================================================================*/

    enum bs_EndpointType {
        BS_ENDPOINT_NONE,

        BS_GET,
        BS_PUT,
        BS_POST,
    };

    struct bs_Endpoint {
        bs_EndpointType type;
        int name_len;
        const char* name;
        bs_Json(*callback)(bs_Endpoint*, bs_List*, bs_Json*);
    };

   /**
    Http server
    Created with bs_server(...)
    */
    struct bs_Server {
        bs_List endpoints;

#ifdef _WIN32
        void* queue;
        void* overlapped;
        char* buffer;
        int buffer_capacity;
        bool is_waiting;
#else
        int reserved;
#endif
    };



  /*==============================================================================
   * Simulation (Definition)
   *============================================================================*/

    struct bs_Simulation {
        float mass;
        float inverse_mass;
        float damping;
        float angular_damping;
        float gravity_scale;

        bs_vec3 velocity;
        bs_vec3 angular_velocity;
        // bs_vec3 acceleration;
        bs_vec3 acceleration_last;
        bs_vec3 force;
        bs_vec3 torque;
        bs_vec3 position;
        bs_vec4 rotation;

        bs_mat3 local_inverse_inertia;
        bs_mat3 world_inverse_inertia;
    };

    struct bs_Contact {
        bs_mat3 to_world;
        bs_vec3 normal;
        bs_vec3 point;
        bs_vec3 velocity;
        float delta_velocity;
        float penetration;

        bs_Simulation* involved[2];
        bs_vec3 relative_contact_positions[2]; // world space position relative to the center of each body
    };



  /*==============================================================================
   * Externals
   *============================================================================*/

    struct bs_Descriptor {
        union {
            struct {
                struct VkSampler_T* vk_sampler;
                struct VkImageView_T* vk_image_view;
                enum VkImageLayout vk_image_layout;
                bs_U32 padding;

                bs_Image* image;
                bs_Sampler* sampler;
            } as_image;

            struct {
                struct VkBuffer_T* vk_buffer;
                bs_U64 vk_offset;
                bs_U64 vk_range;

                bs_Buffer* buffer;
            } as_buffer;
        };

        int object_type;
        int reserved;
        int bind_set;
        int bind_point;
    };

    struct bs_Binding {
        bs_U32 stages;
        int slot;
        int set;
        int location;
        int descriptors_count;
        bool in_use;

        bs_BindType type;
        int size;

       // bs_Object object;
        char* name;
    };

    struct bs_BindSet {
        bs_U32 slot;
        bool needs_update;

        int bindings_count;
        int descriptors_count;
        int max_binding;
        int bound_descriptors_count;

        bs_Binding* bindings;
        bs_Descriptor* descriptors;

        struct VkDescriptorUpdateTemplate_T* update_template;
        struct VkDescriptorSet_T* set;
        struct VkDescriptorSetLayout_T* layout;
    };

    BSAPI extern struct bs_IO {
        bs_U64 exceptions;
        bs_U64 caught;

        bool left_clicked;
        bool right_clicked;
        bool middle_clicked;

        bool left_clicked_last;
        bool right_clicked_last;
        bool middle_clicked_last;

        bool disable_inputs;

        bs_String* executable;
        bs_String* cwd;
        bs_String* appdata;

        bs_String* log;

        bs_U8 
            hold_keys[BS_KEY_BYTES_COUNT],
            keys[BS_KEY_BYTES_COUNT],
            keys_old[BS_KEY_BYTES_COUNT],
            chars[BS_KEY_BYTES_COUNT],
            chars_old[BS_KEY_BYTES_COUNT];

        bs_I8 scroll, scroll_old; // -1, 0 or 1
    } _bs_io;

    typedef enum {
        BS_CURSOR_DEFAULT,
        BS_CURSOR_TEXT,

        BS_CURSOR_TYPE_COUNT
    } bs_CursorIcon;

    BSAPI extern struct bs_Window {
        const char* title;
        void* hwnd;

        bs_ivec2 dimensions;
        bs_Callback resize;
        bs_Callback destroy;
        bs_vec2 cursor;
        bs_vec2 screen_cursor;

        double time, time_old; // elapsed time in seconds
        double delta_time;
        double fixed_time;
        double fixed_interpolation;

        bool in_fixed;
        bool lock_cursor_position;
        bool active;

        struct {
            int id;
            bs_U64 handle;
        } cursor_icons[BS_CURSOR_TYPE_COUNT];

        bs_CursorIcon cursor_icon;
    } _bs_wnd;

    typedef enum {
        BS_OBJECT_IMAGE,
        BS_OBJECT_SAMPLER,
        BS_OBJECT_BUFFER,
        BS_OBJECT_QUEUE,
        BS_OBJECT_BATCH,
        BS_OBJECT_RENDERER,
        BS_OBJECT_RAY_TRACER,
        BS_OBJECT_FONT,
        BS_OBJECT_ATLAS,

        BS_OBJECT_TYPE_COUNT,
    } bs_ObjectType;

    BSAPI struct bs_ObjectSource {
      //  const char* name;
        struct bs_ObjectId {
            const char* name;
            bs_U64 name_hash;
            bs_Object* object;
        }* ids;
        int ids_count;
        bs_ObjectType type;
    };

    extern bs_List _bs_object_sources;

    // ugly
    BSAPI extern struct bs_Instance {
        bs_Queue* single_times_queue;

        bool paused;
        bool advance;
        bool alive;

        int bind_sets_count;
        int bindings_count;
        int descriptors_count;
        int max_bind_set;

        bs_BindSet* bind_sets;
        bs_Binding* bindings;
        bs_Descriptor* descriptors;

        struct {
            int* bindings;
            int bind_set;
        }* descriptor_lookup;

        bool descriptor_pool_needs_update;


        struct {
            enum {
                BS_SURFACE_TYPE_UNDEFINED,
                BS_SURFACE_TYPE_WIN32,
                BS_SURFACE_TYPE_WAYLAND,
                BS_SURFACE_TYPE_X11,
                BS_SURFACE_TYPE_HEADLESS,
            } surface_type;
        } extensions;

        struct VkDescriptorSet_T* sets[BS_MAX_NUM_BIND_SETS];
        struct VkDescriptorSetLayout_T* layouts[BS_MAX_NUM_BIND_SETS];

        struct VkInstance_T* instance;
        struct VkPhysicalDevice_T* physical_device;
        struct VkDevice_T* device;
        struct VkCommandPool_T* command_pool;
        struct VkSurfaceKHR_T* surface;
    }* _bs_instance;

    BSAPI extern struct bs_Bindings {

        bs_BindSet* bs_bind_sets;
        int bs_bind_sets_count;

    } _bs_bind;

    BSAPI extern struct bs_Swapchain {
        int id;
        int frame;
        bs_SwapchainBits flags;

        bool resized;
        bool image_acquired;

        bs_Object* image;

        struct VkSwapchainKHR_T* swapchain;
        enum VkColorSpaceKHR color_space;
        struct {
            // image available
            struct VkSemaphore_T* semaphore;
         //   struct VkFence_T* fence;
        }_[];
    }* _bs_swapchain;

    typedef struct bs_Config bs_Config;
    BSAPI extern struct bs_Config {
        bs_List attributes;
    } _bs_config;

   /**
    Contains info about current part of the render pass
    */
    BSAPI extern struct bs_Scope {
        bool has_begun;
        bs_Renderer* renderer;
        int subpass;
        bs_Queue* queue;
        struct VkSemaphore_T* wait_semaphores[BS_MAX_NUM_WAITS];
        bs_U32 wait_stages[BS_MAX_NUM_WAITS];
        bs_U32 wait_num;
    } bs_scope;

    extern int bs_image_index;

   /**
    Arguments
    */
    BSAPI extern struct bs_Args {
        bool send_bugs;
        bool cmd_log;
        bool skip_log;
        bool color_log;
        bool use_lisk;
        bool use_validation_layers;
        bool skip_log_info;
        bool track_changes;
    } _bs_args;


   /**
    Vulkan feature support queried at startup
    */
    BSAPI extern struct bs_Features {
        bool independent_blend;
        bool ray_tracing;
    } _bs_features;

    BSAPI extern struct bs_Props {
        bs_U32 shader_group_handle_size;
        bs_U32 shader_group_base_alignment;
        bs_U32 min_acceleration_structure_scratch_offset_alignment;
    } _bs_props;

   /**
    INTERNAL NOTE: change references if changing this from 1, 2, 3
    */
    typedef enum {
        BS_SWAPCHAIN_MODE_SINGLE = 1,
        BS_SWAPCHAIN_MODE_DOUBLE = 2,
        BS_SWAPCHAIN_MODE_TRIPLE = 3,

        BS_SWAPCHAIN_MODE_MAX = 3,
    } bs_SwapchainMode;

    BSAPI extern struct bs_Settings {
        bs_SwapchainMode frames_in_flight, buffer_count_min, frames_in_flight_max;
    } _bs_settings;

    BSAPI extern bs_List bs_pipelines[BS_PIPELINE_TYPE_COUNT];
    
    BSAPI extern bs_String* _bs_string_builder;

#endif