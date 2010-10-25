/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kaneton.STABLE/test/engine/engine.h
 *
 * created       julien quintard   [sun oct 17 15:43:58 2010]
 * updated       julien quintard   [mon oct 25 15:24:14 2010]
 */

#ifndef TEST_ENGINE_ENGINE_H
#define TEST_ENGINE_ENGINE_H	1

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

/*
 * ---------- macro-functions -------------------------------------------------
 */

#define TEST_ENTER()							\

#define TEST_ERROR(_message_)						\
  do									\
    {									\
      printf("%s", (_message_));					\
									\
      TEST_LEAVE();							\
    } while (0)

#define TEST_LEAVE()							\
  return;

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      task.c
 *      thread.c
 */

/*
 * task.c
 */

t_error			test_engine_task_spawn(t_class		class,
					       i_task*		task);


/*
 * thread.c
 */

int		test_engine_thread_spawn(i_task			task,
					 t_prior		prior,
					 t_vaddr		entry,
					 i_thread*		thread);


/*
 * eop
 */

#endif
