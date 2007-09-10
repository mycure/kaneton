/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/check/kaneton/bootloader/arch/ia32-virtual/common/common.h
 *
 * created       matthieu bucchianeri   [wed jul 19 18:35:35 2006]
 * updated       matthieu bucchianeri   [wed nov  1 16:09:18 2006]
 */

#ifndef CHECK_BOOTLOADER_COMMON_H_
# define CHECK_BOOTLOADER_COMMON_H_

# include <libc/libc.h>
# include <kaneton.h>
# include "../../../../../check/common/common.h"

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../01/01.c
 *      ../02/02.c
 *      ../03/03.c
 *      ../04/04.c
 */

/*
 * common.c
 */

void		check_ia32_virtual_common(void);

void		check_bootloader_tests(void);


/*
 * ../01/01.c
 */

void		check_ia32_virtual_01(void);


/*
 * ../02/02.c
 */

void			check_ia32_virtual_02(void);


/*
 * ../03/03.c
 */

void		check_ia32_virtual_03(void);


/*
 * ../04/04.c
 */

void		check_ia32_virtual_04(void);


/*
 * eop
 */

#endif /* !CHECK_BOOTLOADER_COMMON_H_ */
