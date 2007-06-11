/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton/library/libc/include/libdata/alloc.h
 *
 * created       julien quintard   [sun jun 10 17:33:03 2007]
 * updated       julien quintard   [sun jun 10 18:02:08 2007]
 */

#ifndef LIBC_LIBDATA_ALLOC_H
#define LIBC_LIBDATA_ALLOC_H       	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <libsys/types.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * area block signature
 */

#define AREA_SIGNATURE		0xab121337

/*
 * chunk metadata signature
 */

#define CHUNK_SIGNATURE		0xBA153E01

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * chunk forward declaration.
 */

struct s_chunk;

/*
 * area
 */

typedef struct			s_area
{
  vsize_t			size;

  u_int32_t			signature;

  struct s_chunk*		first_free_chunk;

  struct s_area*		prev_area;
  struct s_area*		next_area;
}				t_area;

/*
 * chunk
 */

typedef struct			s_chunk
{
  vsize_t			size;

  u_int32_t			signature;

  struct s_chunk*		next_free;

  t_area*			area;
}				t_chunk;

/*
 * alloc global information
 */

/*
 * XXX must not use intra-kernel stuff!!! find another way or protect
 * these things with #ifdef ___kernel
 */
#include <core/types.h>
#include <core/id.h>

typedef t_error (*t_pfn_map_reserve)(i_as, t_opts, t_vsize, t_perms, t_vaddr*);
typedef t_error (*t_pfn_map_release)(i_as, t_vaddr);

typedef struct
{
  t_area*			areas;

  void*				lowest;
  void*				highest;

  t_pfn_map_reserve		map_reserve;
  t_pfn_map_release		map_release;
  i_as				asid;

  vaddr_t			reserve;

  u_int32_t			nalloc;
  u_int32_t			nfree;
}				t_alloc;

#endif