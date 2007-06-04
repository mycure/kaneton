/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/include/core/debug.h
 *
 * created       julien quintard   [mon nov 28 19:37:01 2005]
 * updated       matthieu bucchianeri   [sat may  5 17:10:54 2007]
 */

#ifndef CORE_DEBUG_H
#define CORE_DEBUG_H		1

/*
 * ---------- types -----------------------------------------------------------
 */

typedef struct s_serial_data	t_serial_data;
typedef struct s_serial_buffer	t_serial_buffer;

/*
 * ---------- macros ----------------------------------------------------------
 */

#define DEBUG_PARAMS		(1 << 0)
#define DEBUG_AS		(1 << 1)
#define DEBUG_SEGMENT		(1 << 2)
#define DEBUG_SET		(1 << 3)
#define DEBUG_STATS		(1 << 4)
#define DEBUG_REGION		(1 << 5)
#define DEBUG_TASK		(1 << 6)
#define DEBUG_THREAD		(1 << 7)
#define DEBUG_SCHEDULE		(1 << 8)
#define DEBUG_MESSAGE		(1 << 9)
#define DEBUG_CAPABILITY	(1 << 10)
#define DEBUG_INTERFACE		(1 << 11)
#define DEBUG_EVENT		(1 << 12)
#define DEBUG_TIME		(1 << 13)
#define DEBUG_CPU		(1 << 14)
#define DEBUG_IO		(1 << 15)
#define DEBUG_MAP		(1 << 16)

#define DEBUG								\
  (CONF_DEBUG)

/*                                                                [cut] k1   */

#define ASSERT(_test_)							\
  if (!(_test_))							\
    {									\
      printf("assert(%s) failed at %s:%d\n",				\
             #_test_, __FILE__, __LINE__);				\
									\
      while (1)								\
	;								\
    }

/*                                                                [cut] /k1  */

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <arch/machdep/machdep.h>

/*
 * ---------- types -----------------------------------------------------------
 */

/*                                                                [cut] k1   */

struct				s_serial_data
{
  t_uint32			size;
  t_uint32			magic;
  t_uint32			crc;
  t_uint8*			data;
};

struct				s_serial_buffer
{
 char*				name;
 void*				data;
};

/*                                                               [cut] /k1   */

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      ../../core/debug/debug.c
 *      ../../core/debug/cons-text.c
 *      ../../core/debug/serial.c
 *      ../../core/debug/gdb.c
 */

/*
 * ../../core/debug/debug.c
 */

t_error			debug_init(void);

t_error			debug_clean(void);


/*
 * ../../core/debug/cons-text.c
 */

void			cons_clear(void);

void			cons_scroll(t_uint16			lines);

void			cons_attr(t_uint8			attr);

int			cons_print_char(char			c);

void			cons_print_string(char*			string);

void			cons_msg(char				indicator,
				 char*				fmt,
				 ...);

t_error			cons_init(void);

t_error			cons_clean(void);


/*
 * ../../core/debug/serial.c
 */

void			serial_read(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32 			size);

void			serial_write(t_uint32			com_port,
				     t_uint8*			data,
				     t_uint32			size);

int			serial_send(t_uint32			com_port,
				    t_uint8*			data,
				    t_uint32			size);

int			serial_recv(t_uint32			com_port,
				    t_serial_data		*rdata);

int	serial_put(char c);

void			serial_init(t_uint32			com_port,
				    t_uint8			baud_rate,
				    t_uint8			bit_type,
				    t_uint8			fifo_type);


/*
 * ../../core/debug/gdb.c
 */

void			gdb_handler(t_uint32 needless);

t_error		gdb_command(void);

t_uint32		gdb_extract(t_uint8*	buffer,
				    t_uint32	sz);

int			gdb_read_reg(t_uint8*		buffer);

int		gdb_write_reg(t_uint8*		buffer);

int		gdb_read_mem(t_uint8*		buffer);

int		gdb_write_mem(t_uint8*		buffer);

int			gdb_step(t_uint8*		buffer);

int		gdb_continue(t_uint8*		buffer);

int		gdb_unset_break(t_uint8*	buffer);

int		gdb_set_break(t_uint8*		buffer);

int			gdb_status(t_uint8*		buffer);

t_error		gdb_init(void);

t_error		gdb_clean(void);


/*
 * eop
 */

#endif
