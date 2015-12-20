/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  type  : FILE
    const : stderr
    func  : fputs */
#include <stdarg.h>
/*  type  : va_list
    macro : va_start
            va_arg
            va_end */
#include <stdlib.h>
/*  func  : abs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include posix headers */
#include <pthread.h>
/*  const : PTHREAD_MUTEX_INITIALIZER
    type  : pthread_mutex_t
    func  : pthread_mutex_init
            pthread_mutex_destroy
            pthread_mutex_lock
            pthread_mutex_unlock */

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
#include "meaculpa/meaculpa.h"
/*  type   : mc_Error
    macro  : mc_Error_put */


/*----------------------------------------------------------------------------*/
static FILE            *mc_STREAM        = NULL;
static pthread_mutex_t  mc_STREAM_ACCESS = PTHREAD_MUTEX_INITIALIZER;


/*----------------------------------------------------------------------------*/
/* Error constants and NONE silenced-flag */
const mc_Error  mc_Error_NONE            = 0ull,

                /* No error */
                mc_Error_OKAY            = 0ull,
                /* Generic error */
                mc_Error_FAIL            = 1ull << 0ull,

                /* Implementation specific errors */
                mc_Error_DEPRECATED      = 1ull << 1ull,
                mc_Error_EXPERIMENTAL    = 1ull << 2ull,

                /* Meaculpa specific errors */
                mc_Error_UNKNOWN_ERROR   = 1ull << 3ull,

                /* Function invoking specific errors */
                mc_Error_ARG_IS_NULL     = 1ull << 4ull,
                mc_Error_INVALID_VALUE   = 1ull << 5ull,
                mc_Error_INVALID_INDEX   = 1ull << 6ull,
                mc_Error_INVALID_KEY     = 1ull << 7ull,
                mc_Error_STOP_ITERATION  = 1ull << 8ull,

                /* Memory and instance management errors */
                mc_Error_ALLOC_FAIL      = 1ull << 9ull,
                mc_Error_INI_FAIL        = 1ull << 10ull,
                mc_Error_FIN_FAIL        = 1ull << 11ull,

                /* File I/O specific errors */
                mc_Error_FILE_ACCESS     = 1ull << 12ull,
                mc_Error_FILE_NOT_FOUND  = 1ull << 13ull,
                mc_Error_PERMISSION_FAIL = 1ull << 14ull,
                mc_Error_END_OF_LINE     = 1ull << 15ull,
                mc_Error_END_OF_FILE     = 1ull << 16ull,

                /* Math specific errors */
                mc_Error_ZERO_DIVISION   = 1ull << 17ull,

                /*
                    ... CUSTOM ERRORS GOES HERE...
                */

                /* Error components
                   NOTE: HAS TO BE THE LAST ONES! */
                mc_Error__COMPONENT_1    = 1ull << 52ull,
                mc_Error__COMPONENT_2    = 1ull << 53ull,
                mc_Error__COMPONENT_3    = 1ull << 54ull,
                mc_Error__COMPONENT_4    = 1ull << 55ull,
                mc_Error__COMPONENT_5    = 1ull << 56ull,
                mc_Error__COMPONENT_6    = 1ull << 57ull,
                mc_Error__COMPONENT_7    = 1ull << 58ull,
                mc_Error__COMPONENT_8    = 1ull << 59ull,
                mc_Error__COMPONENT_9    = 1ull << 60ull,
                mc_Error__COMPONENT_10   = 1ull << 61ull,
                mc_Error__COMPONENT_11   = 1ull << 62ull,
                mc_Error__COMPONENT_12   = 1ull << 63ull;


/*----------------------------------------------------------------------------*/
static const char *const ERRORS[] =
{
    "mc_Error_OKAY",
    "mc_Error_FAIL",
    "mc_Error_DEPRECATED",
    "mc_Error_EXPERIMENTAL",
    "mc_Error_UNKNOWN_ERROR",
    "mc_Error_ARG_IS_NULL",
    "mc_Error_INVALID_VALUE",
    "mc_Error_INVALID_INDEX",
    "mc_Error_INVALID_KEY",
    "mc_Error_STOP_ITERATION",
    "mc_Error_ALLOC_FAIL",
    "mc_Error_INI_FAIL",
    "mc_Error_FIN_FAIL",
    "mc_Error_FILE_ACCESS",
    "mc_Error_FILE_NOT_FOUND",
    "mc_Error_PERMISSION_FAIL",
    "mc_Error_END_OF_LINE",
    "mc_Error_END_OF_FILE",
    "mc_Error_ZERO_DIVISION",
};


/*----------------------------------------------------------------------------*/
const char *
mc_Error_str(mc_Error error)
{
    if (!error)                                return ERRORS[0];
    else if (error & mc_Error_FAIL)            return ERRORS[1];
    else if (error & mc_Error_DEPRECATED)      return ERRORS[2];
    else if (error & mc_Error_EXPERIMENTAL)    return ERRORS[3];
    else if (error & mc_Error_UNKNOWN_ERROR)   return ERRORS[4];
    else if (error & mc_Error_ARG_IS_NULL)     return ERRORS[5];
    else if (error & mc_Error_INVALID_VALUE)   return ERRORS[6];
    else if (error & mc_Error_INVALID_INDEX)   return ERRORS[7];
    else if (error & mc_Error_INVALID_KEY)     return ERRORS[8];
    else if (error & mc_Error_STOP_ITERATION)  return ERRORS[9];
    else if (error & mc_Error_ALLOC_FAIL)      return ERRORS[10];
    else if (error & mc_Error_INI_FAIL)        return ERRORS[11];
    else if (error & mc_Error_FIN_FAIL)        return ERRORS[12];
    else if (error & mc_Error_FILE_ACCESS)     return ERRORS[13];
    else if (error & mc_Error_FILE_NOT_FOUND)  return ERRORS[14];
    else if (error & mc_Error_PERMISSION_FAIL) return ERRORS[15];
    else if (error & mc_Error_END_OF_LINE)     return ERRORS[16];
    else if (error & mc_Error_END_OF_FILE)     return ERRORS[17];
    else if (error & mc_Error_ZERO_DIVISION)   return ERRORS[18];
    else                                       return ERRORS[4];
}


/*----------------------------------------------------------------------------*/
void
mc_Error__put(mc_Error                error,
              const char *restrict    function,
              const char *restrict    file,
              int                     messages,
           /* const char *restrict */ ...)
{
    #ifndef MC_FAST
        FILE *restrict stream = mc_STREAM ? mc_STREAM : stderr;
    #else
        #define stream mc_STREAM
    #endif

    int     i;
    va_list args;

    /* Lock access to stream */
    pthread_mutex_lock(&mc_STREAM_ACCESS);

    /* If this is where the error occured */
    if (messages < 0)
    {
        fputs("\nRuntime error: "
              RC_FS(RC_RED, RC_BOLD, ""), stream);
        fputs(mc_Error_str(error), stream);

        /* If there are error messages passed */
        fputs(RC_XFS(" => "), stream);
        if (messages)
        {
            va_start(args, messages);
            fputs(RC_S(RC_BOLD, ""), stream);

            for (i=0; i<abs(messages); i++)
                fputs(va_arg(args, const char *restrict), stream);

            va_end(args);
        }
        else
            fputs(RC_FS(RC_BLACK, RC_BOLD, "[No error message specified]"),
                  stream);
        fputs(RC_XFBS("\n"), stream);

        /* If function name is not NULL */
        if (function)
        {
            fputs("While calling: "
                  RC_S(RC_BOLD, ""), stream);
            fputs(function, stream);
            fputs(RC_XFBS(" (File: "), stream);
            fputs(file, stream);
            fputs(")\n", stream);
        }
    }
    /* If the error returned by a function called inside this function */
    else if (function)
    {
        fputs(RC_F(RC_YELLOW, "> ")
              RC_XF("Called from: ")
              RC_FS(RC_MAGENTA, RC_BOLD, ""), stream);
        fputs(function, stream);

        /* If there are extra hints passed */
        if (messages)
        {
            va_start(args, messages);
            fputs(RC_FS(RC_MAGENTA, RC_NORMAL, " (Note: "), stream);

            for (i=0; i<abs(messages); i++)
                fputs(va_arg(args, const char *restrict), stream);

            fputs(")", stream);
            va_end(args);
        }

        fputs(RC_XFBS("\n"), stream);
    }

    /* Grant access to stream */
    pthread_mutex_unlock(&mc_STREAM_ACCESS);

    #ifdef MC_FAST
        #undef stream
    #endif
}


/*----------------------------------------------------------------------------*/
void
mc_stream_ini(void)
{
    /* Initialize access mutex */
    pthread_mutex_init(&mc_STREAM_ACCESS, NULL);

    /* Lock access to stream */
    pthread_mutex_lock(&mc_STREAM_ACCESS);

    /* Set default stream */
    mc_STREAM = stderr;

    /* Grant access to stream */
    pthread_mutex_unlock(&mc_STREAM_ACCESS);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_stream_fin(void)
{
    pthread_mutex_destroy(&mc_STREAM_ACCESS);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_stream_set(FILE     *restrict stream,
              mc_Error           silenced)
{
    /* If stream is NULL */
    if (!stream)
    {
        if (~silenced & mc_Error_ARG_IS_NULL)
            mc_Error_put(mc_Error_ARG_IS_NULL, 1, "1st argument 'FILE "
                                                  "*restrict stream' is NULL");
        return mc_Error_ARG_IS_NULL;
    }

    /* Lock access to stream */
    pthread_mutex_lock(&mc_STREAM_ACCESS);

    /* Set new stream */
    mc_STREAM = stream;

    /* Grant access to stream */
    pthread_mutex_unlock(&mc_STREAM_ACCESS);

    return mc_Error_OKAY;
}
