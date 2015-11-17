/* INFO **
** INFO */

/* Include meaculpa headers */
#include "meaculpa/error.h"
/*  type  : mc_Error
            mc_Panic
    const : mc_Error_OKAY
            mc_Error_FAIL
            mc_Error_UNKNOWN_ERROR
            mc_Error_UNKNOWN_OWNER
            mc_Error_DEPRECATED
            mc_Error_EXPERIMENTAL
            mc_Error_ARG_IS_NULL
            mc_Error_INVALID_VALUE
            mc_Error_INVALID_INDEX
            mc_Error_INVALID_KEY
            mc_Error_STOP_ITERATION
            mc_Error_ALLOC_FAIL
            mc_Error_INI_FAIL
            mc_Error_FIN_FAIL
            mc_Error_PERMISSION_FAIL
            mc_Error_FILE_NOT_FOUND
            mc_Error_END_OF_LINE
            mc_Error_END_OF_FILE
            mc_Error_ZERO_DIVISION
            mc_Error__COUNTS */


/*----------------------------------------------------------------------------*/
static const char *const ERRORS[] =
{
    [mc_Error_OKAY]            = "mc_Error_OKAY",
    [mc_Error_FAIL]            = "mc_Error_FAIL",
    [mc_Error_UNKNOWN_ERROR]   = "mc_Error_UNKNOWN_ERROR",
    [mc_Error_UNKNOWN_OWNER]   = "mc_Error_UNKNOWN_OWNER",
    [mc_Error_DEPRECATED]      = "mc_Error_DEPRECATED",
    [mc_Error_EXPERIMENTAL]    = "mc_Error_EXPERIMENTAL",
    [mc_Error_ARG_IS_NULL]     = "mc_Error_ARG_IS_NULL",
    [mc_Error_INVALID_VALUE]   = "mc_Error_INVALID_VALUE",
    [mc_Error_INVALID_INDEX]   = "mc_Error_INVALID_INDEX",
    [mc_Error_INVALID_KEY]     = "mc_Error_INVALID_KEY",
    [mc_Error_STOP_ITERATION]  = "mc_Error_STOP_ITERATION",
    [mc_Error_ALLOC_FAIL]      = "mc_Error_ALLOC_FAIL",
    [mc_Error_INI_FAIL]        = "mc_Error_INI_FAIL",
    [mc_Error_FIN_FAIL]        = "mc_Error_FIN_FAIL",
    [mc_Error_PERMISSION_FAIL] = "mc_Error_PERMISSION_FAIL",
    [mc_Error_FILE_NOT_FOUND]  = "mc_Error_FILE_NOT_FOUND",
    [mc_Error_END_OF_LINE]     = "mc_Error_END_OF_LINE",
    [mc_Error_END_OF_FILE]     = "mc_Error_END_OF_FILE",
    [mc_Error_ZERO_DIVISION]   = "mc_Error_ZERO_DIVISION",
};


/*----------------------------------------------------------------------------*/
const char*
mc_Error_str(mc_Error self)
{
    #ifndef mc_FAST
        if (self <  mc_Error_OKAY ||
            self >= mc_Error__COUNTS)
                return ERRORS[mc_Error_UNKNOWN_ERROR];
    #endif
    return ERRORS[self];
}
