/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/kaneton/core/include/region.h
 *
 * created       julien quintard   [wed jun  6 13:40:54 2007]
 * updated       julien quintard   [sun nov 28 20:50:35 2010]
 */

#ifndef CORE_REGION_H
#define CORE_REGION_H			1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <core/types.h>
#include <core/error.h>
#include <core/id.h>

#include <machine/machine.h>

/*
 * ---------- macros ----------------------------------------------------------
 */

/*
 * flags
 */

#define REGION_OPTION_NONE		0
#define REGION_OPTION_FORCE		(1 << 0)
#define REGION_OPTION_USER		(0 << 1)
#define REGION_OPTION_PRIVILEGED	(1 << 1)
#define REGION_OPTION_LOCAL		(0 << 2)
#define REGION_OPTION_GLOBAL		(1 << 2)

#define REGION_OPTION_INVALID		(~((1 << 2) | (1 << 1) | (1 << 0)))

/*
 * ---------- algorithms ------------------------------------------------------
 */

#define REGION_ALGORITHM_FIT_FIRST	(1 << 0)
#define REGION_ALGORITHM_FIT_BEST	(1 << 1)

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * region object
 *
 * a region object is identified by a regid.
 *
 * the identifier is in fact identical to the virtual address of the region.
 */

typedef struct
{
  i_region			id;

  i_segment			segment;

  t_vaddr			address;
  t_paddr			offset;
  t_vsize			size;
  t_options			options;

  machine_data(o_region);
}				o_region;

/*
 * region manager
 */

typedef struct
{
  o_id				id;

  t_vaddr			base;
  t_vsize			size;

  machine_data(m_region);
}				m_region;

/*
 * the region architecture-dependent interface
 */

typedef struct
{
  t_error			(*region_show)(i_as,
					       i_region);
  t_error			(*region_inject)(i_as,
						 o_region*,
						 i_region*);
  t_error			(*region_split)(i_as,
						i_region,
						t_vsize,
						i_region*,
						i_region*);
  t_error			(*region_resize)(i_as,
						 i_region,
						 t_vsize,
						 i_region*);
  t_error			(*region_coalesce)(i_as,
						   i_region,
						   i_region,
						   i_region*);
  t_error			(*region_reserve)(i_as,
						  i_segment,
						  t_paddr,
						  t_options,
						  t_vaddr,
						  t_vsize,
						  i_region*);
  t_error			(*region_release)(i_as,
						  i_region);
  t_error			(*region_flush)(i_as);
  t_error			(*region_initialize)(t_vaddr,
						     t_vsize);
  t_error			(*region_clean)(void);
}				d_region;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/region/region-fit.c
 */

/*
 * ../../core/region/region-fit.c
 */

t_error			region_show(i_as			asid,
				    i_region			regid);

t_error			region_dump(i_as		asid);

t_error			region_fit_first(i_as			asid,
					 t_vsize		size,
					 t_vaddr*		address);

t_error			region_space(i_as		asid,
				     t_vsize		size,
				     t_vaddr*		address);

t_error			region_inject(i_as		asid,
				      o_region*		o,
				      i_region*		regid);

t_error			region_split(i_as			asid,
				     i_region			regid,
				     t_vsize			size,
				     i_region*			left,
				     i_region*			right);

t_error			region_resize(i_as			as,
				      i_region			old,
				      t_vsize			size,
				      i_region*			new);

t_error			region_coalesce(i_as		asid,
					i_region	left,
					i_region	right,
					i_region*	regid);

t_error			region_reserve(i_as			asid,
				       i_segment		segid,
				       t_paddr			offset,
				       t_options		options,
				       t_vaddr			address,
				       t_vsize			size,
				       i_region*		regid);

t_error			region_release(i_as			asid,
				       i_region			regid);

t_error			region_flush(i_as			asid);

t_error			region_exist(i_as			asid,
				     i_region			regid);

t_error			region_get(i_as				asid,
				   i_region			regid,
				   o_region**			o);

t_error			region_initialize(t_vaddr		base,
					  t_vsize		size);

t_error			region_clean(void);


/*
 * eop
 */

#endif
