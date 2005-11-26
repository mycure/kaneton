/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/kaneton/region/region.c
 *
 * created       julien quintard   [wed nov 23 09:19:43 2005]
 * updated       julien quintard   [wed nov 23 09:21:47 2005]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * XXX give
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * XXX
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

// XXX machdep_include(region);

/*
 * ---------- extern ----------------------------------------------------------
 */

/*
 * the init variable, filled by the bootloader, containing in this case
 * the list of segments to mark used.
 */

extern t_init*		init;

/*
 * the kernel address space identifier.
 */

extern t_asid		kas;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the region manager structure.
 */

m_region*		region;

/*
 * ---------- functions -------------------------------------------------------
 */

/* XXX */