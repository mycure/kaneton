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

#include <klibc.h>
#include <kaneton.h>
#include "../common/common.h"

typedef struct
{
  t_uint32	ebp;
  t_uint32	edi;
  t_uint32	esi;
  t_uint32	edx;
  t_uint32	ecx;
  t_uint32	ebx;
  t_uint32	eax;
} __attribute__((packed)) t_check_ctx;

static volatile int	thrown = 0;

static void	check_int3(t_id	id)
{
  thrown = 1;
  asm volatile("	movl $0x10034a10, %eax		\n"
	       "	movl $0xf5000100, %ebx		\n"
	       "	movl $0xaaaaaaaa, %ecx		\n"
	       "	movl $0xffffffff, %edx		\n"
	       "	movl $0xa5a5a5a5, %esi		\n"
	       "	movl $0x5a5a5a5a, %edi		");
}

/*
 * reserve event onto int3 and throw an int3.
 */

void			check_event_context_01(void)
{
  static t_check_ctx*	ctx1;
  static t_check_ctx*	ctx2;
  static t_uint32	esp;

  TEST_ENTER();

  ASSERT(event_reserve(3,
		       EVENT_FUNCTION,
		       EVENT_HANDLER(check_int3)) == ERROR_NONE,
	 "cannot event_reserve\n");

  asm volatile("	pushl %%eax		\n"
	       "	pushl %%ebx		\n"
	       "	pushl %%ecx		\n"
	       "	pushl %%edx		\n"
	       "	pushl %%esi		\n"
	       "	pushl %%edi		\n"
	       "	pushl %%ebp		\n"
	       "	movl %%esp, %0		\n"
	       "	int $3			\n"
	       "	movl %%esp, %2		\n"
	       "	pushl %%eax		\n"
	       "	pushl %%ebx		\n"
	       "	pushl %%ecx		\n"
	       "	pushl %%edx		\n"
	       "	pushl %%esi		\n"
	       "	pushl %%edi		\n"
	       "	pushl %%ebp		\n"
	       "	movl %%esp, %1		"
	       : "=m" (ctx1), "=m" (ctx2), "=m" (esp));

  ASSERT(thrown == 1, " x Exception not caught\n");

  if (esp != ctx1)
    printf("esp differs\n");

  if (ctx1->eax != ctx2->eax)
    printf("eax differs\n");
  if (ctx1->ebx != ctx2->ebx)
    printf("ebx differs\n");
  if (ctx1->ecx != ctx2->ecx)
    printf("ecx differs\n");
  if (ctx1->edx != ctx2->edx)
    printf("edx differs\n");
  if (ctx1->esi != ctx2->esi)
    printf("esi differs\n");
  if (ctx1->edi != ctx2->edi)
    printf("edi differs\n");
  if (ctx1->ebp != ctx2->ebp)
    printf("ebp differs\n");

  TEST_LEAVE();
}