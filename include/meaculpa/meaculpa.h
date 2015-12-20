/* INFO **
** INFO */

/* Header guard */
#ifndef MC_MEACULPA_H_1954125576046949
#define MC_MEACULPA_H_1954125576046949 1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  type  : FILE */


/*----------------------------------------------------------------------------*/
typedef unsigned long long int mc_Error;


/*----------------------------------------------------------------------------*/
extern const mc_Error   mc_Error_NONE,
                        /* No error */
                        mc_Error_OKAY,
                        /* Generic error */
                        mc_Error_FAIL,

                        /* Implementation specific errors */
                        mc_Error_DEPRECATED,
                        mc_Error_EXPERIMENTAL,

                        /* Meaculpa specific errors */
                        mc_Error_UNKNOWN_ERROR,

                        /* Function invoking specific errors */
                        mc_Error_ARG_IS_NULL,
                        mc_Error_INVALID_VALUE,
                        mc_Error_INVALID_INDEX,
                        mc_Error_INVALID_KEY,
                        mc_Error_STOP_ITERATION,

                        /* Memory and instance management errors */
                        mc_Error_ALLOC_FAIL,
                        mc_Error_INI_FAIL,
                        mc_Error_FIN_FAIL,

                        /* File I/O specific errors */
                        mc_Error_FILE_ACCESS,
                        mc_Error_FILE_NOT_FOUND,
                        mc_Error_PERMISSION_FAIL,
                        mc_Error_END_OF_LINE,
                        mc_Error_END_OF_FILE,

                        /* Math specific errors */
                        mc_Error_ZERO_DIVISION,

                        /* Error components
                           WARN: DO NOT USE THESE COMPONENTS DIRECTLY! */
                        mc_Error__COMPONENT_1,
                        mc_Error__COMPONENT_2,
                        mc_Error__COMPONENT_3,
                        mc_Error__COMPONENT_4,
                        mc_Error__COMPONENT_5,
                        mc_Error__COMPONENT_6,
                        mc_Error__COMPONENT_7,
                        mc_Error__COMPONENT_8,
                        mc_Error__COMPONENT_9,
                        mc_Error__COMPONENT_10,
                        mc_Error__COMPONENT_11,
                        mc_Error__COMPONENT_12;


/*----------------------------------------------------------------------------*/
const char *
mc_Error_str(mc_Error error);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Error__put(mc_Error                error,
              const char *restrict    function,
              const char *restrict    file,
              int                     messages,
           /* const char *restrict */ ...);


/*----------------------------------------------------------------------------*/
void
mc_stream_ini(void);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_stream_fin(void);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Return:  mc_Error_OKAY
            mc_Error_ARG_IS_NULL */
mc_Error
mc_stream_set(FILE     *restrict stream,
              mc_Error           silenced);


/*----------------------------------------------------------------------------*/
#define MC_ERROR__COMPOSITE(E, I)   (mc_Error_##E | mc_Error__COMPONENT_##I)
#define mc_Error_OKAY(I)            MC_ERROR__COMPOSITE(OKAY, I)
#define mc_Error_FAIL(I)            MC_ERROR__COMPOSITE(FAIL, I)
#define mc_Error_DEPRECATED(I)      MC_ERROR__COMPOSITE(DEPRECATED, I)
#define mc_Error_EXPERIMENTAL(I)    MC_ERROR__COMPOSITE(EXPERIMENTAL, I)
#define mc_Error_UNKNOWN_ERROR(I)   MC_ERROR__COMPOSITE(UNKNOWN_ERROR, I)
#define mc_Error_ARG_IS_NULL(I)     MC_ERROR__COMPOSITE(ARG_IS_NULL, I)
#define mc_Error_INVALID_VALUE(I)   MC_ERROR__COMPOSITE(INVALID_VALUE, I)
#define mc_Error_INVALID_INDEX(I)   MC_ERROR__COMPOSITE(INVALID_INDEX, I)
#define mc_Error_INVALID_KEY(I)     MC_ERROR__COMPOSITE(INVALID_KEY, I)
#define mc_Error_STOP_ITERATION(I)  MC_ERROR__COMPOSITE(STOP_ITERATION, I)
#define mc_Error_ALLOC_FAIL(I)      MC_ERROR__COMPOSITE(ALLOC_FAIL, I)
#define mc_Error_INI_FAIL(I)        MC_ERROR__COMPOSITE(INI_FAIL, I)
#define mc_Error_FIN_FAIL(I)        MC_ERROR__COMPOSITE(FIN_FAIL, I)
#define mc_Error_FILE_ACCESS(I)     MC_ERROR__COMPOSITE(FILE_ACCESS, I)
#define mc_Error_FILE_NOT_FOUND(I)  MC_ERROR__COMPOSITE(FILE_NOT_FOUND, I)
#define mc_Error_PERMISSION_FAIL(I) MC_ERROR__COMPOSITE(PERMISSION_FAIL, I)
#define mc_Error_END_OF_LINE(I)     MC_ERROR__COMPOSITE(END_OF_LINE, I)
#define mc_Error_END_OF_FILE(I)     MC_ERROR__COMPOSITE(END_OF_FILE, I)
#define mc_Error_ZERO_DIVISION(I)   MC_ERROR__COMPOSITE(ZERO_DIVISION, I)
#define mc_Error_put(E, ...)        mc_Error__put(E, __func__,                 \
                                                     __FILE__,                 \
                                                     __VA_ARGS__)

#endif /* MC_MEACULPA_H_1954125576046949 */
