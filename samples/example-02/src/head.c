/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <stdbool.h>
/*  type  : bool
    const : true
            false */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include "meaculpa/meaculpa.h"
/*  type  : mc_Error
            mc_Panic
    const : mc_Error_OKAY
            mc_Error_DEPRECATED
            mc_Error_ARG_IS_NULL
    func  : mc_Panic_ini_data
            mc_Panic_owned_by
            mc_Panic_get_data
            mc_Panic_get_error
            mc_Panic_put */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include example headers */
#include "body.h"
/*  func  : ex_body
            ex_body_panic */
#include "head.h"


/*----------------------------------------------------------------------------*/
mc_Error
ex_head(void     *const self,
        mc_Panic *const panic)
{
    static const char *const ERR_SELF = "'void *const self' is NULL";

    void *p = NULL;
    switch (ex_body(p, 12, 45.99, panic))
    {
        case mc_Error_OKAY:
        case mc_Error_DEPRECATED:
            break;
        default:
            return mc_Panic_get_error(panic);
    }

    if (!self)
        return mc_Panic_ini_data(panic, ex_head, mc_Error_ARG_IS_NULL, ERR_SELF);

    return mc_Error_OKAY;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
ex_head_panic(mc_Panic *const panic)
{
    if (ex_body_panic(panic))
    {
        mc_Panic_put(panic, ex_head);
        return true;
    }
    if (!mc_Panic_owned_by(panic, ex_head))
        return false;

    mc_Panic_put(panic, ex_head, (const char *const)mc_Panic_get_data(panic));
    return true;
}
