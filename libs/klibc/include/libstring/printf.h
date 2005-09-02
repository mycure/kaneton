/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * printf.h
 * 
 * path          /home/mycure/kaneton
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:32:39 2005   mycure
 * last update   Thu Sep  1 17:28:58 2005   mycure
 */

#ifndef LIBSTRING_PRINTF_H
#define LIBSTRING_PRINTF_H	1

/*
 * ---------- dependencies ----------------------------------------------------
 */

#include <klibc/include/libsys/types.h>

/*
 * ---------- defines ---------------------------------------------------------
 */

#define PRINTF_LADJUST		0x1
#define PRINTF_ZPAD		0x2
#define PRINTF_DOT		0x4

/*
 * ---------- macros ----------------------------------------------------------
 */

#define PRINTF_SWAP(_i1_, _i2_)						\
  {									\
    int		_tmp_;							\
									\
    _tmp_ = (_i1_);							\
    (_i1_) = (_i2_);							\
    (_i2_) = _tmp_;							\
  }

/* XXX */
#define fprintf(_stream_, _args_...) printf(_args_)
/* XXX */

/*
 * ---------- types -----------------------------------------------------------
 */

typedef int			(*t_printf_char_fn)(char	c);
typedef void			(*t_printf_attr_fn)(u_int8_t	attr);

#endif
