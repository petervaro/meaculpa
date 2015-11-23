/* INFO **
** INFO */

/* Header guards */
#ifndef MC_PANIC_H_6243867736469041
#define MC_PANIC_H_6243867736469041 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  type  : FILE */
#include <stdbool.h>
/*  type  : bool */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include <meaculpa/error.h>
/*  type  : mc_Error
    const : mc_Error_OKAY
    func  : mc_Error_str */


/*----------------------------------------------------------------------------*/
/* Helper types */
typedef const void  *mc_PanicData;
typedef void (*mc_PanicFunc)(void);


/*----------------------------------------------------------------------------*/
typedef struct
{
    mc_PanicFunc owner;
    union
    {
        mc_PanicData data;
        mc_PanicFunc func;
    };
    enum
    {
        mc_Panic_NONE = -1,
        mc_Panic_DATA,
        mc_Panic_FUNC,
    } type;
    int  error;
} mc_Panic;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__new_data(mc_Panic     **const self,
                   mc_PanicFunc         owner,
                   int                  error,
                   mc_PanicData         pointer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__new_func(mc_Panic     **const self,
                   mc_PanicFunc         owner,
                   int                  error,
                   mc_PanicFunc         pointer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_new_copy(mc_Panic  *const self,
                  mc_Panic **const panic);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_data(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   int                 error,
                   mc_PanicData        pointer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_func(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   int                 error,
                   mc_PanicFunc        pointer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_ini_copy(mc_Panic *const self,
                  mc_Panic *const panic);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_fin(mc_Panic *const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_del(mc_Panic **const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_get_error(mc_Panic *const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic_set_error(mc_Panic *const self,
                   int             error);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_owner(mc_Panic *const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_owner(mc_Panic     *const self,
                    mc_PanicFunc        owner);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
bool
mc_Panic__owned_by(mc_Panic     *const self,
                   mc_PanicFunc        owner);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicData
mc_Panic_get_data(mc_Panic *const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_data(mc_Panic     *const self,
                   mc_PanicData        pointer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_func(mc_Panic *const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_func(mc_Panic     *const self,
                   mc_PanicFunc        pointer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic_ffput(mc_Panic     *const self,
               mc_PanicFunc        owner,
               const char   *const location,
               const char *(*const err_str)(int),
               const char   *const message,
               FILE         *const stream);

#endif /* MC_PANIC_H_6243867736469041 */
