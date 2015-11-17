/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdlib.h>
/*  const : EXIT_SUCCESS
            EXIT_FAILURE */
#include <stdbool.h>
/*  type  : bool
    const : true
            false */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include <meaculpa/meaculpa.h>
/*  type  : mc_Error
            mc_Panic
    const : mc_Error_OKAY
            mc_Error_ARG_IS_NULL
            mc_Error_UNKNOWN_OWNER
    macro : mc_Panic_put
            mc_Panic_ini_data
    func  : mc_Panic_owned_by
            mc_Panic_get_data */


/*----------------------------------------------------------------------------*/
static mc_Error
tail(void     *const self,
     void     *const data,
     mc_Panic *const panic)
{
    static const char *const ERR_SELF = "void *const self is NULL",
                      *const ERR_DATA = "void *const data is NULL";
    if (!self)
        return mc_Panic_ini_data(panic, tail, mc_Error_ARG_IS_NULL, ERR_SELF);
    if (!data)
        return mc_Panic_ini_data(panic, tail, mc_Error_ARG_IS_NULL, ERR_DATA);

    return mc_Error_OKAY;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static bool
tail_panic(mc_Panic *const panic)
{
    if (!mc_Panic_owned_by(panic, tail))
        return false;

    mc_Panic_put(panic, (const char *const)mc_Panic_get_data(panic));
    return true;
}


/*----------------------------------------------------------------------------*/
static mc_Error
body(void     *const self,
     int             value1,
     double          value2,
     mc_Panic *const panic)
{
    static const char *const ERR_FUNC = "'body' function is deprecated, "
                                        "use 'soul' function instead";

    // Do the job..

    return mc_Panic_ini_data(panic, body, mc_Error_DEPRECATED, ERR_FUNC);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static bool
body_panic(mc_Panic *const panic)
{
    return true;
}


/*----------------------------------------------------------------------------*/
static mc_Error
head(void     *const self,
     mc_Panic *const panic)
{
    static const char *const ERR_SELF = "'void *const self' is NULL";
    double *p;
    switch (body((void *)&p, 12, 45.99, panic))
    {
        case mc_Error_OKAY:
        case mc_Error_DEPRECATED:
            break;
        default:
            return mc_Panic_get_error(panic);
    }

    if (!self)
        return mc_Panic_ini_data(panic, head, mc_Error_ARG_IS_NULL, ERR_SELF);

    return mc_Error_OKAY;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static bool
head_panic(mc_Panic *const panic)
{
    return true;
}


/*----------------------------------------------------------------------------*/
int
main(void)
{
    #ifndef MC_FAST
        mc_Panic panic;
    #endif

    void *p;

    if (head(p, &panic))
    {
        #ifndef MC_FAST
            if (head_panic(&panic))
            {
                mc_Panic_ini_data(&panic, main, mc_Error_UNKNOWN_OWNER);
                mc_Panic_put(&panic, "Panic has no owner");
                return EXIT_FAILURE;
            }
        #endif
        mc_Panic_put(&panic, (const char *const)mc_Panic_get_data(&panic));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
