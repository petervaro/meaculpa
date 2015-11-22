/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  const : stderr
    func  : fputs */
#include <stdlib.h>
/*  func  : malloc
            free */
#include <stdbool.h>
/*  type  : bool
    const : false */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include jemalloc headers */
#include <jemalloc/jemalloc.h>
/*  func  : malloc
            free*/

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include rainicorn headers */
#include <rainicorn/rainicorn.h>
/*  macro : RC_RED
            RC_BOLD
            RC_S
            RC_F
            RC_XFBS */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include "meaculpa/error.h"
/*  type  : mc_Error
    const : mc_Error_OKAY
            mc_Error_FAIL
            mc_Error_ALLOC_FAIL
            mc_Error_ARG_IS_NULL
            mc_Error_INVALID_VALUE */
#include "meaculpa/panic.h"
/*  type  : mc_Panic
            mc_PanicData
            mc_PanicFunc
    const : mc_Panic_NONE
            mc_Panic_DATA
            mc_Panic_FUNC */


/*----------------------------------------------------------------------------*/
mc_Error
mc_Panic__new_data(mc_Panic     **const self,
                   mc_PanicFunc         owner,
                   int                  error,
                   mc_PanicData         pointer)
{
    /* Temporary pointer for new panic object */
    mc_Panic *panic;

    /* If SAFE or BASE mode is defined */
    #ifndef MC_FAST
        /* If self is NULL */
        if (!self)
            return mc_Error_ARG_IS_NULL;
    #endif

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If anything goes wrong, self-pointer will be NULL */
        *self = NULL;
    #endif

    /* Allocate space for a new panic object */
    if (!(panic = malloc(sizeof(mc_Panic))))
        return mc_Error_ALLOC_FAIL;

    /* Initialize newly created panic object */
    mc_Panic__ini_data(panic, owner, error, pointer);

    /* If everything went fine, redirect self-pointer */
    *self = panic;
    return mc_Error_OKAY;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__new_func(mc_Panic     **const self,
                   mc_PanicFunc         owner,
                   int                  error,
                   mc_PanicFunc         pointer)
{
    /* Temporary pointer for new panic object */
    mc_Panic *panic;

    /* If SAFE or BASE mode is defined */
    #ifndef MC_FAST
        /* If self is NULL */
        if (!self)
            return mc_Error_ARG_IS_NULL;
    #endif

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If anything goes wrong, self-pointer will be NULL */
        *self = NULL;
    #endif

    /* Allocate space for a new panic object */
    if (!(panic = malloc(sizeof(mc_Panic))))
        return mc_Error_ALLOC_FAIL;

    /* Initialize newly created panic object */
    mc_Panic__ini_func(panic, owner, error, pointer);

    /* If everything went fine, redirect self-pointer */
    *self = panic;
    return mc_Error_OKAY;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_new_copy(mc_Panic  *const self,
                  mc_Panic **const target)
{
    /* Temporary pointer for new panic object */
    mc_Panic *panic;

    /* If SAFE or BASE mode is defined */
    #ifndef MC_FAST
        /* If target is NULL */
        if (!target)
            return mc_Error_ARG_IS_NULL;
    #endif

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If anything goes wrong, target will be NULL */
        *target = NULL;
    #endif

    /* If SAFE or BASE mode is defined */
    #ifndef MC_FAST
        /* If self is NULL */
        if (!self)
            return mc_Error_ARG_IS_NULL;
    #endif

    /* Allocate space for a new panic object */
    if (!(panic = malloc(sizeof(mc_Panic))))
        return mc_Error_ALLOC_FAIL;

    /* Initialize panic to be the same as self */
    mc_Panic_ini_copy(self, panic);

    /* If everything went fine, redirect target-pointer */
    *target = panic;
    return mc_Error_OKAY;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_data(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   int                 error,
                   mc_PanicData        pointer)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            /* Store passed values and return stored error */
            self->owner        = owner;
            self->type         = mc_Panic_DATA;
            self->data         = pointer;
            return (mc_Error)(self->error = error);

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        }
        /* If self is NULL, return passed error */
        return (mc_Error)error;
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_func(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   int                 error,
                   mc_PanicFunc        pointer)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            /* Store passed values and return stored error */
            self->owner        = owner;
            self->type         = mc_Panic_FUNC;
            self->func         = pointer;
            return (mc_Error)(self->error = error);

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        }
        /* If self is NULL, return passed error */
        return (mc_Error)error;
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_ini_copy(mc_Panic *const self,
                  mc_Panic *const panic)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is NULL or panic is NULL */
        if (!self ||
            !panic)
                return mc_Error_ARG_IS_NULL;
    #endif

    /* Copy all values from self to panic */
    panic->owner = self->owner;
    switch (panic->type = self->type)
    {
        case mc_Panic_DATA:
            panic->data = self->data;
            break;

        case mc_Panic_FUNC:
            panic->func = self->func;
            break;

        /* If invalid type-enum passed */
        default:
            panic->type = mc_Panic_NONE;
            return mc_Error_INVALID_VALUE;
    }
    /* Return stored error */
    return (mc_Error)(panic->error = self->error);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_fin(mc_Panic *const self)
{
    /* If SAFE or BASE mode is defined */
    #ifndef MC_FAST
        /* If self is NULL */
        if (!self)
            return mc_Error_ARG_IS_NULL;
    #endif

    /* Set all values in panic to default */
    self->owner = NULL;
    switch (self->type)
    {
        case mc_Panic_DATA:
            self->data = NULL;
            break;

        case mc_Panic_FUNC:
            self->func = NULL;
            break;

        default:
            return mc_Error_INVALID_VALUE;
    }
    self->type = mc_Panic_NONE;
    return self->error = mc_Error_OKAY;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_del(mc_Panic **const self)
{
    /* If SAFE or FAST is defined */
    #ifndef MC_FAST
        /* If self is NULL */
        if (!self)
            return mc_Error_ARG_IS_NULL;
    #endif

    /* Deallocate space for panic object */
    free(*self);

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        *self = NULL;
    #endif

    /* If everything went fine */
    return mc_Error_OKAY;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_owner(mc_Panic *const self)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
            return NULL;
    #endif

    /* If everything went fine, return the owner-pointer */
    return self->owner;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_owner(mc_Panic     *const self,
                    mc_PanicFunc        owner)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        if (self)
    #endif
            /* Set owner-pointer */
            self->owner = owner;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
mc_Panic__owned_by(mc_Panic     *const self,
                   mc_PanicFunc        owner)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
            return false;
    #endif

    /* Return wether this panic is owned by the passed owner */
    return self->owner == owner;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_get_error(mc_Panic *const self)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
            return mc_Error_FAIL;
    #endif

    /* Return stored error */
    return (mc_Error)(self->error);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_set_error(mc_Panic *const self,
                   int             error)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
    #endif
            /* Return passed and stored error */
            return (mc_Error)(self->error = error);

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* Return passed error */
        return (mc_Error)error;
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicData
mc_Panic_get_data(mc_Panic *const self)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL or the stored pointer is not a data-pointer */
        if (!self ||
            self->type != mc_Panic_DATA)
            return NULL;
    #endif

    /* If everything went fine, return stored data-pointer */
    return self->data;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_data(mc_Panic     *const self,
                   mc_PanicData        pointer)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            /* Set data-pointer */
            self->type = mc_Panic_DATA;
            self->data = pointer;

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        }
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_func(mc_Panic *const self)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL or the stored pointer is not a func-pointer */
        if (!self ||
            self->type != mc_Panic_FUNC)
            return NULL;
    #endif

    /* If everything went fine, return stored func-pointer */
    return self->func;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_func(mc_Panic     *const self,
                   mc_PanicFunc        pointer)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is not NULL */
        if (self)
        {
    #endif
            /* Set func-pointer */
            self->type = mc_Panic_FUNC;
            self->func = pointer;

    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        }
    #endif
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic_ffput(mc_Panic     *const self,
               mc_PanicFunc        owner,
               const char   *const location,
               const char *(*const err_str)(int),
               const char   *const message,
               FILE         *const stream)
{
    /* If SAFE mode is defined */
    #ifdef MC_SAFE
        /* If self is NULL */
        if (!self)
        {
            fputs(RC_S(RC_BOLD, "An error occured: ")
                  RC_F(RC_RED, "mc_Error_ARG_IS_NULL\n")
                  RC_XFBS("In function: mc_Panic_put\n")
                  "1st argument is NULL: mc_Panic *const self\n", stream);
            return;
        }
    #endif

    /* If there is no error */
    if (!self->error)
        return;

    /* If this is where the error occured */
    if (self->owner == owner)
    {
        fputs(RC_S(RC_BOLD, "An error occured: ")
              RC_F(RC_RED, ""), stream);
        fputs(err_str(self->error), stream);
        fputs(RC_XFBS("\n"), stream);
    }

    /* If function is not NULL */
    if (location)
    {
        fputs("In function: ", stream);
        fputs(location, stream);
        fputs("\n", stream);
    }

    /* If message is not NULL */
    if (message)
    {
        fputs(message, stream);
        fputs("\n", stream);
    }
}
