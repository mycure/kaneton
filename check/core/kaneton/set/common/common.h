/*
 * licence       kaneton licence
 *
 * project       kaneton
 *
 * file          /home/buckman/kaneton/kaneton/check/core/kaneton/set/common/common.h
 *
 * created       matthieu bucchianeri   [tue dec 20 15:04:37 2005]
 * updated       matthieu bucchianeri   [wed jan 18 19:07:24 2006]
 */

#ifndef CHECK_SET_COMMON_H_
# define CHECK_SET_COMMON_H_

# include <klibc.h>
# include <kaneton.h>
# include "../../../../check/common/common.h"

typedef void (*t_dispdata)(void *data);

/*
 * ---------- prototypes ------------------------------------------------------
 *
 *      common.c
 *      ../set_array/01/01.c
 *      ../set_array/02/02.c
 *      ../set_array/03/03.c
 *      ../set_array/04/04.c
 *      ../set_array/05/05.c
 *      ../set_array/06/06.c
 *      ../set_array/07/07.c
 *      ../set_array/08/08.c
 *      ../set_array/09/09.c
 *      ../set_array/10/10.c
 *      ../set_array/11/11.c
 *      ../set_array/12/12.c
 *      ../set_array/13/13.c
 *      ../set_ll/01/01.c
 *      ../set_ll/02/02.c
 *      ../set_ll/03/03.c
 *      ../set_ll/04/04.c
 *      ../set_ll/05/05.c
 *      ../set_ll/06/06.c
 *      ../set_ll/07/07.c
 *      ../set_ll/08/08.c
 *      ../set_ll/09/09.c
 *      ../set_ll/10/10.c
 *      ../set_ll/11/11.c
 *      ../set_ll/12/12.c
 *	../set_pipe/01/01.c
 *	../set_pipe/02/02.c
 *	../set_pipe/03/03.c
 */

/*
 * common.c
 */

void	check_set_common(void);

void	check_set_tests(void);

void		check_many_add(t_setid setid, int nb, ...);

void		check_many_remove(t_setid setid, int nb, ...);

void		check_display_set(t_setid setid, t_dispdata disp);


/*
 * ../set_array/01/01.c
 */

void		check_set_array_01(void);


/*
 * ../set_array/02/02.c
 */

void		check_set_array_02(void);


/*
 * ../set_array/03/03.c
 */

void		check_set_array_03(void);


/*
 * ../set_array/04/04.c
 */

void		check_set_array_04(void);


/*
 * ../set_array/05/05.c
 */

void		check_set_array_05(void);


/*
 * ../set_array/06/06.c
 */

void		check_set_array_06(void);


/*
 * ../set_array/07/07.c
 */

void		check_set_array_07(void);


/*
 * ../set_array/08/08.c
 */

void		check_set_array_08(void);


/*
 * ../set_array/09/09.c
 */

void		check_set_array_09(void);


/*
 * ../set_array/10/10.c
 */

void		check_set_array_10(void);


/*
 * ../set_array/11/11.c
 */

void		check_set_array_11(void);


/*
 * ../set_array/12/12.c
 */

void		check_set_array_12(void);


/*
 * ../set_array/13/13.c
 */

void		check_set_array_13(void);


/*
 * ../set_ll/01/01.c
 */

void		check_set_ll_01(void);


/*
 * ../set_ll/02/02.c
 */

void		check_set_ll_02(void);


/*
 * ../set_ll/03/03.c
 */

void		check_set_ll_03(void);


/*
 * ../set_ll/04/04.c
 */

void		check_set_ll_04(void);


/*
 * ../set_ll/05/05.c
 */

void		check_set_ll_05(void);


/*
 * ../set_ll/06/06.c
 */

void		check_set_ll_06(void);


/*
 * ../set_ll/07/07.c
 */

void		check_set_ll_07(void);


/*
 * ../set_ll/08/08.c
 */

void		check_set_ll_08(void);


/*
 * ../set_ll/09/09.c
 */

void		check_set_ll_09(void);


/*
 * ../set_ll/10/10.c
 */

void		check_set_ll_10(void);


/*
 * ../set_ll/11/11.c
 */

void		check_set_ll_11(void);


/*
 * ../set_ll/12/12.c
 */

void		check_set_ll_12(void);


/*
 * ../set_pipe/01/01.c
 */

void		check_set_pipe_01(void);


/*
 * ../set_pipe/02/02.c
 */

void		check_set_pipe_02(void);


/*
 * ../set_pipe/03/03.c
 */

void		check_set_pipe_03(void);


/*
 * eop
 */

#endif /* !CHECK_SET_COMMON_H_ */
