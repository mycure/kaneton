/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/arch/ia32-virtual/core/thread.h
 *
 * created       renaud voltz   [tue apr  4 03:10:52 2006]
 * updated       matthieu bucchianeri   [fri jul 28 18:00:52 2006]
 */

#ifndef IA32_CORE_THREAD_H
#define IA32_CORE_THREAD_H       1

/*
 * ---------- dependencies ----------------------------------------------------
 */

//#include <core/thread.h>

/*                                                                [cut] k4   */

/*
 *
 */

typedef struct
{
  t_ia32_tss*		tss;
}			am_thread;

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct
{
  t_ia32_context	context;
  union
  {
    t_x87_state		x87;
    t_sse_state		sse;
  }			u;
}			ao_thread;

/*
 * ---------- macro functions -------------------------------------------------
 */

#define         machdep_include_thread()				\
  extern d_thread                thread_dispatch

#define         machdep_call_thread(_function_, _args_...)		\
  (									\
    {									\
      t_error   _r_ = ERROR_NONE;					\
									\
      if (thread_dispatch._function_ != NULL)				\
        _r_ = thread_dispatch._function_(_args_);			\
									\
      _r_;								\
    }									\
  )

#define         machdep_data_m_thread()					\
  am_thread		machdep

#define         machdep_data_o_thread()					\
  ao_thread		machdep

/*                                                               [cut] /k4   */

#endif