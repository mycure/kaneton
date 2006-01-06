/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/check/init/init.c
 *
 * created       matthieu bucchianeri   [tue dec 20 15:03:30 2005]
 * updated       matthieu bucchianeri   [tue jan  3 22:48:02 2006]
 */

#include <klibc.h>
#include "init.h"

void	check_check_init(void)
{
  /*
   * nothing to do here since this tests is only used to include code
   * for next steps
   */
  printf("init done.\n");
}

/*
 * starts ALL tests manually
 */

void	check_tests(void)
{
  check_check_init();
  check_set_tests();
//  check_malloc_tests();
//  check_printf_tests();
//  check_stats_tests();
//  check_id_tests();
  /* XXX continue tests */
}

/*
 * for leaks tracking
 */

void	check_leaks_init(t_leaks*	l)
{
  l->nb_alloc = alloc_nalloc();
  l->nb_free = alloc_nfree();
}

void	check_leaks_display(t_leaks*	l)
{
  if ((l->nb_alloc - l->nb_free) != (alloc_nalloc() - alloc_nfree()))
    printf("error: memory leaks detected: %u/%u -> %u/%u\n",
	   l->nb_alloc, l->nb_free, alloc_nalloc(), alloc_nfree());
}