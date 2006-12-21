/*
 * ---------- header ----------------------------------------------------------
 *
 * project       kaneton
 *
 * license       kaneton licence
 *
 * file          /home/enguerrand/kaneton/kaneton/bootloader/arch/sgi-o2.mips64/bootloader.c
 *
 * created       enguerrand raymond   [wed oct 18 14:21:40 2006]
 * updated       enguerrand raymond   [thu dec 21 16:19:26 2006]
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <libmipsr10000.h>
#include <klibc.h>
#include <kaneton.h>

#include "bootloader.h"
#include "init.h"

/*
 * ---------- globals ---------------------------------------------------------
 */

/*
 * the init variable.
 */

extern	t_init*		init;

/*
 * these three variable are globals to avoid them to be lost when the
 * bootloader changes its stack.
 *
 * indeed, before jumping on the kernel a new stack is created so the local
 * variables will no longer be accessible. putting these variables in
 * globals bypass this problem.
 */

void			(*kernel)(t_init*) = 0xffffffff8000f000;

/*
 * ---------- functions -------------------------------------------------------
 */

/*
 * a funny function which does nothing.
 *
 * this function is called when a fatal error occurs.
 */

void			bootloader_error(void)
{
  while (1)
    ;
}

/*
 * the bootloader entry point.
 *
 * steps:
 *
 * 1) initializes the console and displays welcome messages
 * 2) Initializes t_init structure
 * 3) Fill the t_init fields
 *    Set up 64 bits mode
 */

int			bootloader(void)
{
  /*
   * 1)
   */

  bootloader_cons_init();

  printf("\n");
  printf("                --- the kaneton microkernel project ---\n");
  printf("\n");

  /*
   * 2)
   */

  init = (t_init*)alloc(PAGESZ);

  /*
   * 3)
   */

  //bootloader_kernel_copy();

  init->mem = START_PHYSICAL_ADDR;
  init->memsz = bootloader_memory_size();

  init->kcode = (t_paddr)KERNEL_BASE;
  init->kcodesz = 0x64000;

  init->init = (t_paddr)init;
  init->initsz = PAGESZ;

  //kernel(init);

  printf("Set up 64 bits mode\n");

  /*
   *
   */
  asm("li $10, 0x300000e0\n\t"
      "mtc0 $10, $12\n\t");

  //kernel(init);

  bootloader_cons_msg('!', "error: kernel exited\n");

  bootloader_error();

  return (0);
}
