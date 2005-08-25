/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * bcmp.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:49:47 2005   mycure
 * last update   Sun Jun 19 22:23:19 2005   mycure
 */

/*
 * ---------- includes --------------------------------------------------------
 */

#include <klibc/include/klibc.h>

/*
 * ---------- functions -------------------------------------------------------
 */

int			bcmp(const void*			b1,
			     const void*			b2,
			     size_t				length)
{
  const char*		p1 = b1;
  const char*		p2 = b2;

  if (length == 0)
    return(0);

  do
    {
      if (*p1++ != *p2++)
	break;
    } while (--length);
  
  return (length);
}