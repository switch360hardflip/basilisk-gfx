#ifndef BSMOD_CACHE_H
#define BSMOD_CACHE_H

#include <bsgfx_cache.h>
#include <basilisk-mod.h>

#define BSGFX_PACKAGE _bsmod_.package



  /*==============================================================================
   * Shader Cache
   *============================================================================*/

BSGFX_CACHE_FS(fs_bsmod_rasterize)
BSGFX_CACHE_FS(fs_bsmod_material_icon)
BSGFX_CACHE_FS(fs_bsmod_billboard)

BSGFX_CACHE_VS(vs_bsmod_mesh_instanced)
BSGFX_CACHE_VS(vs_bsmod_billboard)



  /*==============================================================================
   * Atlas Cache
   *============================================================================*/

	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_MATERIAL_ICONS, test0)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, grid_hover)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, white)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, title_bar_close)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, scrollbar_button)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, material)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, instance)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, increment)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, circle)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, prohibition)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, expand)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, light)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, light_add)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, sun)
	BSGFX_CACHE_ATLAS_QUERY(_bsmod_atlases, BSMOD_ATLAS_UI, sun_add)

#undef BSGFX_PACKAGE
#endif