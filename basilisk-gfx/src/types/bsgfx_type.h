
 /**
  bsgfx_type.h
  
  typically used for known-sized arrays
  */

#ifndef BSGFX_TYPE_H
#define BSGFX_TYPE_H

#include <basilisk-gfx.h>
#include <stdbool.h>
#include <bsgfx_cache.h>

#define BSGFX_TYPE_MAGIC 0x65707974
#define BSGFX_FOLIAGE_VERSION 1




  /*==============================================================================
   * Constants 
   =============================================================================*/

	typedef enum {
		BSGFX_TYPE_UNDEFINED = -1,

		BSGFX_TYPE_PRIMITIVE,
		BSGFX_TYPE_TILE,
		BSGFX_TYPE_PREFAB,
		BSGFX_TYPE_FOLIAGE,
		BSGFX_TYPE_SPAWNER,
		BSGFX_TYPE_LIGHT,

		BSGFX_TYPE_COUNT
	} bsgfx_TypeId;



  /*==============================================================================
   * Structures
   =============================================================================*/

	typedef struct {
		bs_U32 magic;
		bs_U32 version;
		int flexible_count;
		int count;

		int accessors[];
	} bsgfx_TypeHeader;

	typedef struct bsgfx_Type bsgfx_Type;
	extern struct bsgfx_Type {
		int version;

		int capacity;
		int flexible_capacity;

		int count;
		int flexible_count;

		int package_id;

		const char* plural;
		const char* singular;

		void(*mapper)(void*, void*);

		int* unmapped_accessors;
		int* mapped_accessors;

		unsigned char* mapped;
		unsigned char* unmapped;

		int mapped_unit_size;
		int unmapped_unit_size;

		int unmapped_flexible_offset;
		int mapped_flexible_offset;

		int unmapped_flexible_size;
		int mapped_flexible_size;
	} bsgfx_types[BSGFX_TYPE_COUNT];



  /*==============================================================================
   * Functions
   =============================================================================*/

//#define BSGFX_TYPE(type, capacity, plural, singular, t1, t2) \
//    bsgfx_type(type, #plural, #singular, &bsgfx_##plural##_capacity, &bsgfx_##plural##_count, sizeof(t1)); \
//    bsgfx_##plural = bs_realloc(bsgfx_##plural, bsgfx_##plural##_capacity * sizeof(t2));

	// paranoid schizophrenic macros
#define BSGFX_TYPE_STRUCTURE(type, name) type name;
#define BSGFX_TYPE_FIELD_NAMES(type, name) #name,
#define BSGFX_TYPE_NAMES(type, name) #type,
#define BSGFX_TYPE_SIZES(type, name) sizeof(type),
#define BSGFX_UNMAPPED_TYPE \
	struct { \
		BSGFX_TYPE_FIELDS(BSGFX_TYPE_STRUCTURE) \
	} \

//#define BSGFX_TYPE_FORMAT(type, capacity, plural, singular, mapper, t, flexible) \
//	bsgfx_type(\
//		type,\
//		#plural,\
//		#singular,\
//		&bsgfx_##plural##_capacity,\
//		&bsgfx_##plural##_count,\
//        sizeof(struct { BSGFX_TYPE_FIELDS(BSGFX_TYPE_STRUCTURE) }), \
//		sizeof(t), \
//        (const char* []) { BSGFX_TYPE_FIELDS(BSGFX_TYPE_FIELD_NAMES) }, \
//        (const char* []) { BSGFX_TYPE_FIELDS(BSGFX_TYPE_NAMES) }, \
//        (size_t[]) { BSGFX_TYPE_FIELDS(BSGFX_TYPE_SIZES) },\
//		mapper,\
//		flexible);

	BSGFXAPI void
	bsgfx_type(
		bsgfx_TypeId id,
		int package_id,
		int version,
		const char* plural,
		const char* singular,
		size_t unmapped_size,
		size_t mapped_size,
		void(*mapper)(void*, void*),
		size_t unmapped_flexible_offset,
		size_t mapped_flexible_offset,
		size_t unmapped_flexible_size,
		size_t mapped_flexible_size);

	BSGFXAPI void
	bsgfx_map(
		bsgfx_TypeId type, 
		int id);

	BSGFXAPI void
	bsgfx_remap(
		bsgfx_TypeId type_id);

	BSGFXAPI int bsgfx_count(bsgfx_TypeId type_id);
	BSGFXAPI void* bsgfx_get(bsgfx_TypeId type_id, bs_U32 id);
	BSGFXAPI bsgfx_Type* bsgfx_getType(bsgfx_TypeId type_id);
	BSGFXAPI int bsgfx_id(bsgfx_TypeId type_id, unsigned char* p);
	BSGFXAPI int bsgfx_rawId(bsgfx_TypeId type_id, unsigned char* p);
	BSGFXAPI void* bsgfx_getRaw(bsgfx_TypeId type_id, int id);
	BSGFXAPI int bsgfx_flexibleCount(bsgfx_TypeId type_id, int id);
	//void
	//bsgfx_addFlexible(
	//	bsgfx_TypeId type_id,
	//	int id,
	//	void* data);

   /**
    Create an object of a given type
    @param id
	@param data - Can be NULL
	@returns - Safe pointer to the object that was added
    */
	BSGFXAPI void*
	bsmod_add(
		bsgfx_TypeId id,
		void* data);

	BSGFXAPI bsgfx_TypeId
	bsmod_queryType(
		const char* plural);

#endif