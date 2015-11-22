/* INFO **
** INFO */

/* Header guard */
#ifndef EX_TAIL_H_2473126350101375
#define EX_TAIL_H_2473126350101375 1

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
ex_tail(void     *const self,
        void     *const data,
        mc_Panic *const panic);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
ex_tail_panic(mc_Panic *const panic);

#endif /* EX_TAIL_H_2473126350101375 */
