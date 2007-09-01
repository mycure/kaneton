/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/arch/ia32-virtual/kaneton/region/01/01.c
 *
 * created       matthieu bucchianeri   [sun apr  2 19:04:14 2006]
 * updated       matthieu bucchianeri   [sun apr  9 17:41:04 2006]
 */

#include <libc.h>
#include <kaneton.h>
#include "../common/common.h"

static volatile int executed = 0;
static volatile int esp = 0;

static void	thread1(void)
{
  t_uint32	cs;

  asm volatile("	movl %%esp, esp		\n"
	       "	movl %%cs, %0		"
	       : "=a" (cs));

  printf ("Ring %d\n", cs & 0x3);

  executed = 1;

  while (1)
    ;
}

/*
 * XXX
 */

void		check_task_service_01(void)
{
  i_task	tsk;
  t_id		id;
  t_uint32	start;
  t_thread_context	ctx;

  TEST_ENTER();

  ASSERT(check_task_create(TASK_CLASS_SERVICE, &tsk) == 0,
	"error creating task\n");

  ASSERT(check_thread_create(tsk, THREAD_PRIOR, (t_vaddr)thread1, &id) == 0,
	 "error creating thread\n");

  CLI();
  ASSERT(task_state(tsk, SCHEDULER_STATE_RUN) == ERROR_NONE,
	 "cannot start task\n");
  STI();

  start = check_cmos_sec();
  while ((start + 3) % 60 != check_cmos_sec() && !executed)
    ;

  ASSERT(executed == 1, "Thread not executed\n")

  ASSERT(thread_store(id, &ctx) == ERROR_NONE,
	 "error in thread_store\n");
  ASSERT(ctx.sp + 4096 > esp && ctx.sp - 4096 < esp, "Bad stack\n");

  TEST_LEAVE();
}