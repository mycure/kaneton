/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strncat.c
 * 
 * path          /home/mycure/kaneton/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:55:14 2005   mycure
 * last update   Sat Jun 18 15:52:05 2005   mycure
 */

#include <klibc/include/klibc.h>

char*			strncat(char*		dst,
				char*		src,
				size_t		n)
{
  u_int			i;
  u_int			j;

  for (i = 0; dst[i]; i++)
    ;

  for (j = 0; src[j] && j < n; j++, i++)
    dst[i] = src[j];
  dst[i] = 0;

  return (dst);
}
