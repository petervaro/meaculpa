/* INFO **
** INFO */

/* Header guard */
#ifndef EX_HEAD_H_9285258603303980
#define EX_HEAD_H_9285258603303980 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdbool.h>
/*  type  : bool */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include "meaculpa/meaculpa.h"
/*  type  : mc_Error
            mc_Panic */


/*----------------------------------------------------------------------------*/
mc_Error
ex_head(void     *const self,
        mc_Panic *const panic);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
ex_head_panic(mc_Panic *const panic);

#endif /* EX_HEAD_H_9285258603303980 */
