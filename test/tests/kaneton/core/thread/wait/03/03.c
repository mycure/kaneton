/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton
 *
 * file          /home/mycure/kane...t/tests/kaneton/core/thread/wait/03/03.c
 *
 * created       julien quintard   [sun oct 17 14:37:04 2010]
 * updated       julien quintard   [sun jan 30 21:10:19 2011]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <kaneton.h>

#include "03.h"

/*
 * ---------- externs ---------------------------------------------------------
 */

extern m_kernel			_kernel;

/*
 * ---------- globals ---------------------------------------------------------
 */

static volatile i_thread	thread1;
static volatile i_thread	thread2;
static volatile i_thread	thread3;

/*
 * ---------- test ------------------------------------------------------------
 */

void			test_core_thread_wait_03_thread_01(void)
{
  s_wait		wait;
  i_cpu			cpu;

  if (thread_wait(thread1, thread2, WAIT_STATE_START, &wait) != ERROR_OK)
    TEST_HANG("[thread_wait] error");

  if (WAIT_THREAD(&wait) != thread2)
    TEST_HANG("invalid waiting thread identifier");

  if (WAIT_CAUSE(&wait) != WAIT_STATE_START)
    TEST_HANG("invalid waiting cause");

  if (thread_sleep(thread1, 5000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  TEST_SIGNATURE(r0923jawodfe09);

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_stop(cpu) != ERROR_OK)
    TEST_HANG("[scheduler_stop] error");

  TEST_HANG("unreachable");
}

void			test_core_thread_wait_03_thread_02(void)
{
  TEST_SIGNATURE(3r2wfeawf9043);

  while (1)
    ;
}

void			test_core_thread_wait_03_thread_03(void)
{
  if (thread_sleep(thread3, 1000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  if (thread_start(thread2) != ERROR_OK)
    TEST_HANG("[thread_start] error");

  if (thread_sleep(thread3, 1000) != ERROR_OK)
    TEST_HANG("[thread_sleep] error");

  TEST_SIGNATURE(32w98ru30rwaf09i);

  while (1)
    ;
}

void			test_core_thread_wait_03(void)
{
  i_cpu			cpu;

  TEST_ENTER();

  /*
   * thread 1
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_wait_03_thread_01,
		     (i_thread*)&thread1) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread1) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * thread 2
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_wait_03_thread_02,
		     (i_thread*)&thread2) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  /*
   * thread 3
   */

  if (thread_reserve(_kernel.task,
		     THREAD_PRIORITY,
		     THREAD_STACK_ADDRESS_NONE,
                     THREAD_STACK_SIZE_LOW,
		     (t_vaddr)test_core_thread_wait_03_thread_03,
		     (i_thread*)&thread3) != ERROR_OK)
    TEST_ERROR("[thread_reserve] error");

  if (thread_start(thread3) != ERROR_OK)
    TEST_ERROR("[thread_start] error");

  /*
   * scheduler
   */

  if (cpu_current(&cpu) != ERROR_OK)
    TEST_HANG("[cpu_current] error");

  if (scheduler_start(cpu) != ERROR_OK)
    TEST_ERROR("[scheduler_start] error");

  if (event_enable() != ERROR_OK)
    TEST_ERROR("[event_enable] error");

  TEST_SIGNATURE(rw98uefskdq0aa);

  if (event_disable() != ERROR_OK)
    TEST_ERROR("[event_disable] error");

  TEST_LEAVE();
}