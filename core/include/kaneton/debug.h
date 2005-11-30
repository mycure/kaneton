/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/mycure/kaneton/core/include/kaneton/debug.h
 *
 * created       julien quintard   [mon nov 28 19:37:01 2005]
 * updated       julien quintard   [mon nov 28 19:37:14 2005]
 */

#ifndef KANETON_DEBUG_H
#define KANETON_DEBUG_H		1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define DEBUG_PARAMS		(1 << 0)
#define DEBUG_AS		(1 << 1)
#define DEBUG_SEGMENT		(1 << 2)
#define DEBUG_SET		(1 << 3)
#define DEBUG_STATS		(1 << 4)

#define DEBUG								\
  (CONF_DEBUG)

/*
 * ---------- types -----------------------------------------------------------
 */

typedef	struct		s_debug_function
{
	char		*f_name;
	int		(*f)(char *str);
}			debug_function_t;


typedef struct			s_serial_data
{
  t_uint32			size;
  t_uint32			magic;
  t_uint32			crc;
  t_uint8*			data;
}				t_serial_data;


/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../kaneton/debug/debug.c
 */

/*
 * ../../kaneton/debug/debug.c
 */

t_error			debug_init(void);

t_error			debug_recv();

t_error			debug_recv_cmd();

t_error			debug_exec_cmd_tab(t_serial_data *cmd_tab);

t_error			debug_clean(void);


/*
 * eop
 */

#endif
