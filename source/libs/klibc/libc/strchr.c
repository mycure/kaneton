/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strchr.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:36 2005   mycure
 * last update   Sat Apr 23 16:59:02 2005   mycure
 */

#include <klibc/include/klibc.h>

char			*strchr(const char	*p,
				int		ch)
{
  u_int			i;

  for (i = 0; p[i]; i++)
    if (p[i] == ch)
      return ((char *) p + i);

  return (NULL);
}
