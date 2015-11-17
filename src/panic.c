/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  const : stderr
    func  : fputs */
#include <stdbool.h>
/*  type  : bool
    const : false */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include "meaculpa/error.h"
/*  type  : mc_Error */
#include "meaculpa/panic.h"
/*  type  : mc_Panic
            mc_PanicData
            mc_PanicFunc */


/*----------------------------------------------------------------------------*/
mc_Error
mc_Panic__ini_data(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   const char   *const owner_str,
                   int                 error,
                   mc_PanicData        data)
{
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            /* Store passed values */
            self->owner        = owner;
            self->type         = mc_Panic_DATA;
            self->data         = data;
            return self->error = error;

    #ifdef MC_SAFE
        }
        return error;
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_func(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   const char   *const owner_str,
                   int                 error,
                   mc_PanicFunc        func)
{
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            /* Store passed values */
            self->owner        = owner;
            self->type         = mc_Panic_FUNC;
            self->func         = func;
            return self->error = error;

    #ifdef MC_SAFE
        }
        return error;
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_owner(mc_Panic *const self)
{
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
            return NULL;
    #endif
    return self->owner;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_owner(mc_Panic     *const self,
                    mc_PanicFunc        owner)
{
    #ifdef MC_SAFE
        if (self)
    #endif
            self->owner = owner;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
mc_Panic__owned_by(mc_Panic     *const self,
                   mc_PanicFunc        owner)
{
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
            return false;
    #endif
    return self->owner == owner;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_get_error(mc_Panic *const self)
{
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
            return mc_Error_FAIL;
    #endif
    return self->error;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_set_error(mc_Panic *const self,
                   int             error)
{
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
    #endif
            return self->error = error;
    #ifdef MC_SAFE
        return error;
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicData
mc_Panic_get_data(mc_Panic *const self)
{
    #ifdef MC_SAFE
        /* If self is not NULL or
           if the stored pointer is not a data pointer */
        if (!self ||
            self->type != mc_Panic_DATA)
            return NULL;
    #endif
    return self->data;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_data(mc_Panic     *const self,
                   mc_PanicData        data)
{
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            self->type = mc_Panic_DATA;
            self->data = data;
    #ifdef MC_SAFE
        }
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_func(mc_Panic *const self)
{
    #ifdef MC_SAFE
        /* If self is not NULL or
           if the stored pointer is not a data pointer */
        if (!self ||
            self->type != mc_Panic_FUNC)
            return NULL;
    #endif
    return self->func;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_func(mc_Panic     *const self,
                   mc_PanicFunc        func)
{
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            self->type = mc_Panic_FUNC;
            self->func = func;
    #ifdef MC_SAFE
        }
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic_ffput(mc_Panic     *const self,
               const char *(*const err_str)(int),
               const char   *const message,
               FILE         *const stream)
{
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
        {
            fputs("An error occured: mc_Error_ARG_IS_NULL\n"
                  "In function: mc_Panic_put\n"
                  "1st argument is NULL: mc_Panic *const self\n", stream);
            return;
        }
    #endif

    /* If there is no error */
    if (!self->error)
        return;

    /* If this is where the error occured */
    // if (is_origin)
    // {
        fputs("An error occured: ", stream);
        fputs(err_str(self->error), stream);
        fputs("\n", stream);
    // }

    /* If function is not NULL */
    // if (function)
    // {
    //     fputs("In function: ", stream);
    //     fputs(function, stream);
    //     fputs("\n", stream);
    // }

    /* If message is not NULL */
    if (message)
    {
        fputs(message, stream);
        fputs("\n", stream);
    }
}
