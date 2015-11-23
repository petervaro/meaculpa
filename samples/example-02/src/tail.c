/* INFO **
** INFO **/

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
            mc_Error_ARG_IS_NULL
    func  : mc_Panic_ini_data
            mc_Panic_owned_by
            mc_Panic_get_data
            mc_Panic_put */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include example headers */
#include "tail.h"


/*----------------------------------------------------------------------------*/
#ifdef EX_FAST
mc_Error
ex_tail(void     *const self,
        void     *const data)
{

}
#else
mc_Error
ex_tail(void     *const self,
        void     *const data,
        mc_Panic *const panic)
{
    static const char *const ERR_SELF = "void *const self is NULL";
    static const char *const ERR_DATA = "void *const data is NULL";

    if (!self)
        return mc_Panic_ini_data(panic, ex_tail, mc_Error_ARG_IS_NULL, ERR_SELF);
    if (!data)
        return mc_Panic_ini_data(panic, ex_tail, mc_Error_ARG_IS_NULL, ERR_DATA);

    return mc_Error_OKAY;
}
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
ex_tail_panic(mc_Panic *const panic)
{
    if (!mc_Panic_owned_by(panic, ex_tail))
        return false;

    mc_Panic_put(panic, ex_tail, (const char *const)mc_Panic_get_data(panic));
    return true;
}
