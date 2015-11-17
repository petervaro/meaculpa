/* INFO **
** INFO */

/* Header guard */
#ifndef MC_MEA_CULPA_H_9596858801811053
#define MC_MEA_CULPA_H_9596858801811053 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  const : stderr */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include <meaculpa/error.h>
/*  type  : mc_Error
    const : mc_Error_OKAY
    func  : mc_Error_str */
#include <meaculpa/panic.h>
/*  type  : mc_Panic
            mc_PanicData
            mc_PanicFunc
    func  : mc_Panic__ini_data
            mc_Panic__ini_func
            mc_Panic__set_owner
            mc_Panic__owned_by
            mc_Panic__set_data
            mc_Panic__set_func
            mc_Panic_fput */

/* Helper macros
   P : Panic
   O : Owner
   E : Error
   U : User pointer
   T : Type
   M : Message
   S : Stream
   F : Function */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MC__PANIC_INI_DATA(P, O, E, U) \
    mc_Panic__ini_data(P, (mc_PanicFunc)O, #O, E, (mc_PanicData)P)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MC__PANIC_INI_FUNC(P, O, E, U) \
    mc_Panic__ini_func(P, (mc_PanicFunc)O, #O, E, (mc_PanicFunc)U)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MC__PANIC_VA1(T, P) MC__PANIC_INI_##T(P, NULL, mc_Error_OKAY, NULL)
#define MC__PANIC_VA2(T, P, O) MC__PANIC_INI_##T(P, NULL, mc_Error_OKAY, NULL)
#define MC__PANIC_VA3(T, P, O, E) MC__PANIC_INI_##T(P, O, E, NULL)
#define MC__PANIC_VA4(T, P, O, E, U) MC__PANIC_INI_##T(P, O, E, U)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MC__PANIC_VA_DISPATCHER(_1, _2, _3, _4, M, ...) M
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define MC__PANIC_VA(T, ...)                                                   \
    MC__PANIC_VA_DISPATCHER(__VA_ARGS__,                                       \
                            MC__PANIC_VA4,                                     \
                            MC__PANIC_VA3,                                     \
                            MC__PANIC_VA2,                                     \
                            MC__PANIC_VA1)(T, __VA_ARGS__)


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_ini_data(...) MC__PANIC_VA(DATA, __VA_ARGS__)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_ini_func(...) MC__PANIC_VA(FUNC, __VA_ARGS__)


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_set_data(P, U)  mc_Panic__set_data(P, (mc_PanicData)U)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_set_func(P, U)  mc_Panic__set_func(P, (mc_PanicFunc)U)


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_put(P, M) \
    mc_Panic_ffput(P, (const char *(*)(int))mc_Error_str, M, stderr)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_fput(P, M, S) \
    mc_Panic_ffput(P, (const char *(*)(int))mc_Error_str, M, S)


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_set_owner(P, O) mc_Panic__set_owner(P, (mc_PanicFunc)O)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#define mc_Panic_owned_by(P, F) mc_Panic__owned_by(P, (mc_PanicFunc)F)

#endif /* MC_MEA_CULPA_H_9596858801811053 */
