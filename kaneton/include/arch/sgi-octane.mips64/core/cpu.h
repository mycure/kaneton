/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/include/arch/sgi-octane.mips64/core/cpu.h
 *
 * created       Enguerrand RAYMOND   [tue oct 17 13:25:19 2006]
 * updated       Enguerrand RAYMOND   [tue oct 17 17:14:16 2006]
 */

#ifndef OCTANE_CORE_CPU_H
#define OCTANE_CORE_CPU_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

/* XXX #include <core/cpu.h>*/

/*
 * ---------- types -----------------------------------------------------------
 */

/*
 * ---------- macro functions -------------------------------------------------
 */

#define		machdep_include_cpu()					\
	extern d_cpu		cpu_dispatch

#define		machdep_call_cpu(_function_, _args_...)			\
  (									\
    {									\
      t_error	_r_ = ERROR_NONE;					\
									\
      if (cpu_dispatch._function_ != NULL)				\
        _r_ = cpu_dispatch._function_(_args_);				\
									\
      _r_;								\
    }									\
  )

#define		machdep_data_m_cpu()

#define		machdep_data_o_cpu()

#endif
