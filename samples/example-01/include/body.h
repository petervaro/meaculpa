/* INFO **
** INFO */

/* Header guard */
#ifndef EX_BODY_H_1434152090176464
#define EX_BODY_H_1434152090176464 1

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
ex_body(void     *const self,
        int             value1,
        double          value2,
        mc_Panic *const panic);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
ex_body_panic(mc_Panic *const panic);

#endif /* EX_BODY_H_1434152090176464 */
