/* INFO **
** INFO */

/* Header guard */
#ifndef MC_ERROR_H_1954125576046949
#define MC_ERROR_H_1954125576046949 1

/*----------------------------------------------------------------------------*/
typedef enum
{
    /* No error */
    mc_Error_OKAY,
    /* Generic error */
    mc_Error_FAIL,

    /* Meaculpa specific errors */
    mc_Error_UNKNOWN_ERROR,
    mc_Error_UNKNOWN_OWNER,

    /* Implementation specific errors */
    mc_Error_DEPRECATED,
    mc_Error_EXPERIMENTAL,

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
    mc_Error_PERMISSION_FAIL,
    mc_Error_FILE_NOT_FOUND,
    mc_Error_END_OF_LINE,
    mc_Error_END_OF_FILE,

    /* Math specific errors */
    mc_Error_ZERO_DIVISION,

    /* Number of predefined mc_Errors. THIS HAS TO BE THE LAST MEMBER */
    mc_Error__COUNTS,
} mc_Error;


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const char*
mc_Error_str(mc_Error self);

#endif /* MC_ERROR_H_1954125576046949 */
