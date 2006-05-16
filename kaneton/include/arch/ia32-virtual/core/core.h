/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/kaneton/include/arch/ia32-virtual/core/core.h
 *
 * created       julien quintard   [sat dec 17 17:13:18 2005]
 * updated       julien quintard   [fri apr  7 14:24:26 2006]
 */

#ifndef IA32_CORE_CORE_H
#define IA32_CORE_CORE_H	1

/*
 * ---------- macros ----------------------------------------------------------
 */

#define ___endian		ENDIAN_LITTLE
#define ___wordsz		WORDSZ_32

#define PAGESZ			4096

/*
 * ---------- includes --------------------------------------------------------
 */

#include <arch/machdep/core/as.h>
#include <arch/machdep/core/debug.h>
#include <arch/machdep/core/init.h>
#include <arch/machdep/core/region.h>
#include <arch/machdep/core/segment.h>
#include <arch/machdep/core/stats.h>
#include <arch/machdep/core/event.h>
#include <arch/machdep/core/timer.h>
#include <arch/machdep/core/thread.h>
#include <arch/machdep/core/task.h>

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../../../core/arch/machdep/as.c
 *      ../../../../core/arch/machdep/region.c
 *      ../../../../core/arch/machdep/segment.c
 *      ../../../../core/arch/machdep/event.c
 *	../../../../core/arch/machdep/timer.c
 *      ../../../../core/arch/machdep/task.c
 *      ../../../../core/arch/machdep/thread.c
 */

/*
 * ../../../../core/arch/machdep/as.c
 */

t_error			ia32_as_show(t_asid			asid);

t_error			ia32_as_reserve(i_task			tskid,
					t_asid*			asid);


/*
 * ../../../../core/arch/machdep/region.c
 */

t_vaddr			ia32_region_map(t_uint32		pte,
					t_paddr			p);

void			ia32_region_unmap(t_uint32		pte);

t_error			ia32_region_reserve(t_asid		asid,
					    t_segid		segid,
					    t_paddr		offset,
					    t_opts		opts,
					    t_vaddr		address,
					    t_vsize		size,
					    t_regid*		regid);

t_error			ia32_region_release(t_asid		asid,
					    t_regid		regid);

t_error			ia32_region_init(t_vaddr		start,
					 t_vsize		size);

t_error			ia32_region_clean(void);


/*
 * ../../../../core/arch/machdep/segment.c
 */

t_error			ia32_segment_read(t_segid		segid,
					  t_paddr		offs,
					  void*			buff,
					  t_psize		sz);

t_error			ia32_segment_write(t_segid		segid,
					   t_paddr		offs,
					   const void*		buff,
					   t_psize		sz);

t_error			ia32_segment_copy(t_segid		dst,
					  t_paddr		offsd,
					  t_segid		src,
					  t_paddr		offss,
					  t_psize		sz);

t_error			ia32_segment_init(void);

t_error			ia32_segment_clean(void);


/*
 * ../../../../core/arch/machdep/event.c
 */

t_error			ia32_event_reserve(i_event		id,
					   t_type		type,
					   u_event_handler	handler);

t_error			ia32_event_release(i_event		id);

t_error			ia32_event_init(void);

t_error			ia32_event_clean(void);

void			ia32_event_handler(t_uint32		id);

void                    ia32_kbd_handler(t_uint32		id);

void                    ia32_pf_handler(t_uint32		error_code);


/*
 * ../../../../core/arch/machdep/timer.c
 */

t_error			ia32_timer_init(void);


/*
 * ../../../../core/arch/machdep/task.c
 */

t_error			ia32_task_clone(i_task			old,
					i_task*			new);

t_error			ia32_task_reserve(t_class		class,
					  t_behav		behav,
					  t_prior		prior,
					  i_task*		tskid);

t_error			ia32_task_release(i_task		tskid);

t_error			ia32_task_init(void);

t_error			ia32_task_clean(void);


/*
 * ../../../../core/arch/machdep/thread.c
 */

t_error			ia32_thread_suspend(t_thrid		threadid);

t_error			ia32_thread_execute(t_thrid		threadid);

t_error			ia32_thread_clone(t_thrid		threadid);


/*
 * eop
 */

#endif
