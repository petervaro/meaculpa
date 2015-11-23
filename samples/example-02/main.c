/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdlib.h>
/*  const : EXIT_SUCCESS
            EXIT_FAILURE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include <meaculpa/meaculpa.h>
/*  type  : mc_Error
            mc_Panic
    const : mc_Error_UNKNOWN_OWNER
    func  : mc_Panic_ini_data
            mc_Panic_get_data
            mc_Panic_fin
            mc_Panic_put */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include example headers */
#include "head.h"
/*  func  : ex_head
            ex_head_panic */


/*----------------------------------------------------------------------------*/
int
main(void)
{
    mc_Panic  panic;
    void     *p = NULL;

    if (ex_head(p, &panic))
    {
        if (!ex_head_panic(&panic))
        {
            mc_Panic_ini_data(&panic, main, mc_Error_UNKNOWN_OWNER);
            mc_Panic_put(&panic, main, "Panic has no owner");
            return EXIT_FAILURE;
        }
        mc_Panic_put(&panic, main);
        return EXIT_FAILURE;
    }
    mc_Panic_fin(&panic);
    return EXIT_SUCCESS;
}
