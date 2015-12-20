/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <stdlib.h>
/*  const : EXIT_FAILURE
            EXIT_SUCCESS */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "meaculpa/meaculpa.h"
/*  type  : mc_Error
    const : mc_Error_OKAY
            mc_Error_DEPRECATED
    macro : mc_Error_put */

/*----------------------------------------------------------------------------*/
mc_Error
core(mc_Error silenced);

/*----------------------------------------------------------------------------*/
mc_Error
middle_man(mc_Error silenced);

/*----------------------------------------------------------------------------*/
mc_Error
wrapper(mc_Error silenced);


/*----------------------------------------------------------------------------*/
int
main(void)
{
    mc_stream_ini();

    mc_Error error;
    if ((error = wrapper(mc_Error_NONE)))
        mc_Error_put(error, 1, "Unexpected error in 'wrapper'");

    mc_stream_fin();
    return EXIT_SUCCESS;
}


/*----------------------------------------------------------------------------*/
mc_Error
wrapper(mc_Error silenced)
{
    mc_Error error;
    if ((error = middle_man(mc_Error_NONE)))
    {
        if (~silenced & error)
            mc_Error_put(error, 0);
        return error;
    }
    return mc_Error_OKAY;
}


/*----------------------------------------------------------------------------*/
mc_Error
middle_man(mc_Error silenced)
{
    mc_Error error;
    if ((error = core(mc_Error_NONE)))
    {
        if (~silenced & error)
            mc_Error_put(error, 1, "Cannot create X because 'core' failed");
        return error;
    }
    return mc_Error_OKAY;
}


/*----------------------------------------------------------------------------*/
mc_Error
core(mc_Error silenced)
{
    if (~silenced & mc_Error_DEPRECATED)
        mc_Error_put(mc_Error_DEPRECATED, -1, "This function is deprecated");
    return mc_Error_DEPRECATED;
}
