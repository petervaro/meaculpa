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
typedef void  *mc_PanicData;
typedef void (*mc_PanicFunc)(void);


/*----------------------------------------------------------------------------*/
#define FUNC_NAME (size_t)1024
typedef struct
{
    const char owner_str[FUNC_NAME];
    mc_PanicFunc owner;
    union
    {
        mc_PanicData data;
        mc_PanicFunc func;
    };
    enum
    {
        mc_Panic_DATA,
        mc_Panic_FUNC,
    } type;
    int  error;
} mc_Panic;
#undef FUNC_NAME


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_data(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   const char   *const owner_str,
                   int                 error,
                   mc_PanicData        data);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_Panic__ini_func(mc_Panic     *const self,
                   mc_PanicFunc        owner,
                   const char   *const owner_str,
                   int                 error,
                   mc_PanicFunc        func);
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
                   mc_PanicData        data);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_PanicFunc
mc_Panic_get_func(mc_Panic *const self);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic__set_func(mc_Panic     *const self,
                   mc_PanicFunc        func);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Panic_ffput(mc_Panic     *const self,
               const char *(*const err_str)(int),
               const char   *const message,
               FILE         *const stream);

#endif /* MC_PANIC_H_6243867736469041 */
