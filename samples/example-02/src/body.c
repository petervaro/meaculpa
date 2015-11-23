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
    const : mc_Error_DEPRECATED
    func  : mc_Panic_ini_data
            mc_Panic_owned_by
            mc_Panic_get_data
            mc_Panic_get_error
            mc_Panic_put */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include example headers */
#include "tail.h"
/*  func  : ex_tail
            ex_tail_panic */
#include "body.h"


/*----------------------------------------------------------------------------*/
mc_Error
ex_body(void     *const self,
        int             value1,
        double          value2,
        mc_Panic *const panic)
{
    static const char *const ERR_FUNC = "the function 'body' is "
                                        "deprecated, use 'soul' instead";
    (void)value1;
    (void)value2;

    void *p = NULL;
    if (ex_tail(p, self, panic))
        return mc_Panic_get_error(panic);

    return mc_Panic_ini_data(panic, ex_body, mc_Error_DEPRECATED, ERR_FUNC);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
ex_body_panic(mc_Panic *const panic)
{
    if (ex_tail_panic(panic))
    {
        mc_Panic_put(panic, ex_body);
        return true;
    }
    if (!mc_Panic_owned_by(panic, ex_body))
        return false;

    mc_Panic_put(panic, ex_body, (const char *const)mc_Panic_get_data(panic));
    return true;
}
