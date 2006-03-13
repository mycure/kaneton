/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/core/kaneton/arch/ia32-virtual/as.c
 *
 * created       julien quintard   [fri feb 11 03:04:40 2005]
 * updated       matthieu bucchianeri   [mon mar 13 19:17:01 2006]
 */

/*
 * ---------- information -----------------------------------------------------
 *
 * this file  implements dependent  code for as  manager on  ia32 with
 * paging architecture.
 *
 */

/*
 * ---------- assignments -----------------------------------------------------
 *
 * students will have to develop this entire part of the project. take
 * a look at the interface  structure declaration to fill it correctly
 * and to write needed functions.
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
extern t_tskid		ktask;
extern t_init*		init;

/*
 * ---------- globals ---------------------------------------------------------
 */


/*                                                                  [cut] k2 */

t_asid			kasid = ID_UNUSED;

/*                                                                 [cut] /k2 */

/*
 * the address space manager interface.
 */

i_as			as_interface =
  {

/*                                                                  [cut] k2 */

    ia32_as_show,
    NULL,
    NULL,
    NULL,
    NULL,
    ia32_as_reserve,
    ia32_as_release,
    NULL,
    NULL

/*                                                                 [cut] /k2 */

  };

/*
 * ---------- functions -------------------------------------------------------
 */

/*                                                                  [cut] k2 */

/** XXX  bouger ca ailleurs, que ce soit commun a region et as */
static t_error		ia32_as_create_page_table(o_as*		o,
						  t_uint16	pde,
						  t_directory	kpd,
						  void*		address)
{
  t_page		pg;
  t_table		pt;
  o_region		optreg;
  o_segment*		otbl;
  t_segid		segtbl;

  AS_ENTER(as);

  pt.rw = 1;
  pt.present = 1;
  pt.user = 0;
  pt.entries = address;

  pg.rw = 1;
  pg.present = 1;
  pg.user = 0;

  if (pd_add_table(&o->machdep.pd, pde, pt) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * d)
   */

  pg.addr = (void*)pt.entries;

  if (pd_get_table(&kpd, PDE_ENTRY((t_paddr)pg.addr), &pt) !=
      ERROR_NONE)
    {
      /*
       * e)
       */

      if (segment_reserve(kasid, PAGESZ, PERM_READ | PERM_WRITE,
			  &segtbl) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (segment_get(segtbl, &otbl) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      pt.entries = (void*)otbl->address;

      /*
       * f)
       */

      if (pd_add_table(&kpd, PDE_ENTRY((t_paddr)pg.addr), pt) !=
	  ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      optreg.address = (t_paddr)pg.addr;
      optreg.segid = (t_segid)optreg.address;
      optreg.offset = 0;
      optreg.size = PAGESZ;

      /*
       * g)
       */

      if (region_inject(kasid, &optreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);
    }

  /*
   * h)
   */

  pt.entries = ENTRY_ADDR(PD_MIRROR, PDE_ENTRY((t_paddr)pg.addr));

  if (pt_add_page(&pt, PTE_ENTRY((t_paddr)pg.addr), pg) !=
      ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  tlb_invalidate(pg.addr);

  AS_LEAVE(as, ERROR_NONE);
}


/*
 * this function displays architecture dependent data.
 *
 * steps:
 *
 * 1) get the address space object.
 * 2) display machdep data.
 */

t_error			ia32_as_show(t_asid			asid)
{
  o_as*			o;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  cons_msg('#', "  page directory base: 0x%x\n", *o->machdep.pd);

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function reserves an address space.
 *
 * steps:
 *
 * 1) get the as object.
 *   kernel task:
 *  a) get the page directory from the init structure.
 *  b) add the mirroring entry.
 *  c) manually reserve a region for the mirror entry.
 *  d) clean the page directory.
 *   normal task:
 *  a) reserve a segment for the directory.
 *  b) reserve a region for the directory in the kernel address space.
 *  c) build a new page directory for the as.
 */

t_error			ia32_as_reserve(t_tskid			tskid,
					t_asid*			asid)
{
  o_as*			o;
  t_segid	        seg;
  t_regid		reg;
  o_segment*		oseg;
  o_segment		pt_seg;
  o_region		oreg;
  t_paddr		base;
  t_table		pt;
  t_uint32		i;
  t_uint32		pde_start;
  t_uint32		pde_end;
  t_uint32		pte_start;
  t_uint32		pte_end;
  t_uint32		pde;
  t_uint32		pte;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(*asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (tskid == ktask)
    {
      kasid = *asid;

      /*
       * a)
       */

      o->machdep.pd = (t_directory)init->segments[10].address;

      /*
       * b)
       */

      pt.present = 1;
      pt.rw = 1;
      pt.user = 0;
      pt.entries = (void*)o->machdep.pd;

      if (pd_add_table(&o->machdep.pd, PD_MIRROR, pt) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      oreg.address = (t_vaddr)ENTRY_ADDR(PD_MIRROR, 0);
      oreg.segid = (t_segid)o->machdep.pd;
      oreg.offset = 0;
      oreg.size = PT_MAX_ENTRIES * PAGESZ;

      if (region_inject(*asid, &oreg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * d)
       */

      pde_start = 0;
      pte_start = 0;

      for (i = 0; i < init->nregions + 1; i++)
	{
	  if (i != init->nregions)
	    {
	      pde_end = PDE_ENTRY(init->regions[i].address);
	      pte_end = PTE_ENTRY(init->regions[i].address);
	    }
	  else
	    {
	      pde_end = PD_MAX_ENTRIES;
	      pte_end = PT_MAX_ENTRIES;
	    }

	  for (pde = pde_start; pde <= pde_end; pde++)
	    {
	      if (pde != PD_MIRROR &&
		  pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
		{
		  for (pte = (pde == pde_start ? pte_start : 0);
		       pte < (pde == pde_end ? pte_end : PT_MAX_ENTRIES);
		       pte++)
		    {
		      pt_delete_page(&pt, pte);
		    }
		}
	    }

	  if (i != init->nregions)
	    {
	      pde_start = PDE_ENTRY(init->regions[i].address +
				    init->regions[i].size);
	      pte_start = PTE_ENTRY(init->regions[i].address +
				    init->regions[i].size);
	    }
	}

      /*
       * e)
       */

      for (i = 0; i < init->nregions; i++)
	{
	  pde_start = PDE_ENTRY(init->regions[i].address);
	  pde_end = PDE_ENTRY(init->regions[i].address +
			      init->regions[i].size);

	  for (pde = pde_start; pde <= pde_end; pde++)
	    {
	      if (pd_get_table(&o->machdep.pd, pde, &pt) == ERROR_NONE)
		{
		  seg = (t_segid)(t_uint32)pt.entries;

		  if (segment_get(*asid, seg) != ERROR_NONE)
		    {
		      pt_seg.address = pt.entries;
		      pt_seg.size = PAGESZ;
		      pt_seg.perms = PERM_READ | PERM_WRITE;

		      if (segment_inject(*asid, &pt_seg) != ERROR_NONE)
			AS_LEAVE(as, ERROR_UNKNOWN);

		      ia32_as_create_page_table(o, pde, o->machdep.pd,
						pt.entries);
		    }
		}
	    }
	}
    }
  else
    {
      /*
       * a)
       */

      if (segment_reserve(*asid, PAGESZ,
			  PERM_READ | PERM_WRITE, &seg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      if (segment_get(seg, &oseg) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      base = oseg->address;

      /*
       * b)
       */

      if (region_reserve(kasid, seg, 0, REGION_OPT_FORCE, seg, PAGESZ, &reg) !=
	  ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

      /*
       * c)
       */

      if (pd_build(base, &o->machdep.pd, 1) != ERROR_NONE)
	AS_LEAVE(as, ERROR_UNKNOWN);

    }

  AS_LEAVE(as, ERROR_NONE);
}

/*
 * this function releases an address space.
 *
 * steps:
 *
 * 1) get the as object.
 * 2) release page-directory segment and region.
 */

t_error			ia32_as_release(t_asid			asid)
{
  o_as*			o;
  t_segid		seg;
  t_regid		reg;

  AS_ENTER(as);

  /*
   * 1)
   */

  if (as_get(asid, &o) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  /*
   * 2)
   */

  seg = (t_segid)(t_uint32)(o->machdep.pd);
  reg = seg;

  if (region_release(reg, kasid) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  if (segment_release(seg) != ERROR_NONE)
    AS_LEAVE(as, ERROR_UNKNOWN);

  AS_LEAVE(as, ERROR_NONE);
}

/*                                                                 [cut] /k2 */
