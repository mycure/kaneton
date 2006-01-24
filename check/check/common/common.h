/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/check/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:21:50 2006]
 */

#ifndef CHECK_COMMON_H_
# define CHECK_COMMON_H_

# include <klibc.h>

# define TEST_ENTER							\
  t_leaks __memory_leaks;						\
  printf("Test %s\n", __FUNCTION__);					\
  check_leaks_init(&__memory_leaks)

# define TEST_LEAVE							\
  check_leaks_display(&__memory_leaks);					\
  printf("%s done.\n", __FUNCTION__)

typedef struct
{

  t_uint32	nb_alloc;
  t_uint32	nb_free;

}		t_leaks;

void	check_set_tests(void);
void	check_stats_tests(void);
void	check_malloc_tests(void);
void	check_printf_tests(void);
void	check_id_tests(void);
void	check_libia32_tests(void);

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 */

/*
 * common.c
 */

void	check_check_common(void);

void	check_tests(void);

void	check_leaks_init(t_leaks*	l);

void	check_leaks_display(t_leaks*	l);


/*
 * eop
 */

#endif /* !CHECK_COMMON_H_ */