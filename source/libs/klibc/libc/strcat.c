/*
 * copyright quintard julien
 * 
 * kaneton
 * 
 * strcat.c
 * 
 * path          /home/mycure/kaneton/src/libs/klibc/libc
 * 
 * made by mycure
 *         quintard julien   [quinta_j@epita.fr]
 * 
 * started on    Fri Feb 11 02:53:22 2005   mycure
 * last update   Sat Apr 23 16:58:53 2005   mycure
 */

#include <klibc/include/klibc.h>

char			*strcat(char		*s,
				const char	*append)
{
  u_int			i;

  for (i = 0; s[i]; i++)
    ;

  for (; s[i] && *append; i++, append++)
    s[i] = *append;
  s[i] = 0;

  return (s);
}
