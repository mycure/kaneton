/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * as.c
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 03:04:40 2005   mycure
 * last update   Sat Nov 19 12:48:27 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc.h>
#include <kaneton.h>

/*
 * ---------- extern ----------------------------------------------------------
 */

extern m_as*		as;

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the address space manager interface.
 */

i_as			as_interface =
  {
    ia32_as_give,
    ia32_as_clone,
    ia32_as_reserve,
    ia32_as_release,
    ia32_as_init,
    ia32_as_clean
  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * XXX
 */

t_error			ia32_as_give(t_asid			asid,
				     t_tskid			tskid)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_clone(t_tskid			tskid,
				      t_asid			old,
				      t_asid*			new)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_release(t_asid			asid)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_init(void)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * XXX
 */

t_error			ia32_as_clean(void)
{
  AS_ENTER(as);

  /* XXX */

  AS_LEAVE(as, ERROR_NONE);
}
