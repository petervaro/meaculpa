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
/* Error silencer wildcards */
const mc_Error   mc_MUTE_NONE =  0ull,
                 mc_MUTE_ALL  = ~0ull;


/*----------------------------------------------------------------------------*/
/* Error signals */
const mc_Error  mc_OKAY            = 0ull,

                /* Generic error */
                mc_FAIL            = 1ull << 0,

                /* Implementation specific errors */
                mc_DEPRECATED      = 1ull << 1,
                mc_EXPERIMENTAL    = 1ull << 2,

                /* Meaculpa specific errors */
                mc_UNKNOWN_ERROR   = 1ull << 3,

                /* Function invoking specific errors */
                mc_ARG_IS_NULL     = 1ull << 4,
                mc_INVALID_VALUE   = 1ull << 5,
                mc_INVALID_INDEX   = 1ull << 6,
                mc_INVALID_KEY     = 1ull << 7,
                mc_STOP_ITERATION  = 1ull << 8,

                /* Memory and instance management errors */
                mc_ALLOC_FAIL      = 1ull << 9,
                mc_INI_FAIL        = 1ull << 10,
                mc_FIN_FAIL        = 1ull << 11,

                /* File I/O specific errors */
                mc_FILE_ACCESS     = 1ull << 12,
                mc_FILE_NOT_FOUND  = 1ull << 13,
                mc_PERMISSION_FAIL = 1ull << 14,
                mc_END_OF_LINE     = 1ull << 15,
                mc_END_OF_FILE     = 1ull << 16,

                /* Math specific errors */
                mc_ZERO_DIVISION   = 1ull << 17,

                /*
                    ... CUSTOM ERRORS GOES HERE...
                */

                /* Error markers
                   NOTE: HAS TO BE THE LAST ONES! */
                mc__MARKER_16      = 1ull << 48,
                mc__MARKER_15      = 1ull << 49,
                mc__MARKER_14      = 1ull << 50,
                mc__MARKER_13      = 1ull << 51,
                mc__MARKER_12      = 1ull << 52,
                mc__MARKER_11      = 1ull << 53,
                mc__MARKER_10      = 1ull << 54,
                mc__MARKER_9       = 1ull << 55,
                mc__MARKER_8       = 1ull << 56,
                mc__MARKER_7       = 1ull << 57,
                mc__MARKER_6       = 1ull << 58,
                mc__MARKER_5       = 1ull << 59,
                mc__MARKER_4       = 1ull << 60,
                mc__MARKER_3       = 1ull << 61,
                mc__MARKER_2       = 1ull << 62,
                mc__MARKER_1       = 1ull << 63;


/*----------------------------------------------------------------------------*/
static const char *const mc_ERRORS[] =
{
    "mc_OKAY",
    "mc_FAIL",
    "mc_DEPRECATED",
    "mc_EXPERIMENTAL",
    "mc_UNKNOWN_ERROR",
    "mc_ARG_IS_NULL",
    "mc_INVALID_VALUE",
    "mc_INVALID_INDEX",
    "mc_INVALID_KEY",
    "mc_STOP_ITERATION",
    "mc_ALLOC_FAIL",
    "mc_INI_FAIL",
    "mc_FIN_FAIL",
    "mc_FILE_ACCESS",
    "mc_FILE_NOT_FOUND",
    "mc_PERMISSION_FAIL",
    "mc_END_OF_LINE",
    "mc_END_OF_FILE",
    "mc_ZERO_DIVISION",
};


/*----------------------------------------------------------------------------*/
const char *
mc_Error_str(mc_Error error)
{
    if (!error)                          return mc_ERRORS[0];
    else if (error & mc_FAIL)            return mc_ERRORS[1];
    else if (error & mc_DEPRECATED)      return mc_ERRORS[2];
    else if (error & mc_EXPERIMENTAL)    return mc_ERRORS[3];
    else if (error & mc_UNKNOWN_ERROR)   return mc_ERRORS[4];
    else if (error & mc_ARG_IS_NULL)     return mc_ERRORS[5];
    else if (error & mc_INVALID_VALUE)   return mc_ERRORS[6];
    else if (error & mc_INVALID_INDEX)   return mc_ERRORS[7];
    else if (error & mc_INVALID_KEY)     return mc_ERRORS[8];
    else if (error & mc_STOP_ITERATION)  return mc_ERRORS[9];
    else if (error & mc_ALLOC_FAIL)      return mc_ERRORS[10];
    else if (error & mc_INI_FAIL)        return mc_ERRORS[11];
    else if (error & mc_FIN_FAIL)        return mc_ERRORS[12];
    else if (error & mc_FILE_ACCESS)     return mc_ERRORS[13];
    else if (error & mc_FILE_NOT_FOUND)  return mc_ERRORS[14];
    else if (error & mc_PERMISSION_FAIL) return mc_ERRORS[15];
    else if (error & mc_END_OF_LINE)     return mc_ERRORS[16];
    else if (error & mc_END_OF_FILE)     return mc_ERRORS[17];
    else if (error & mc_ZERO_DIVISION)   return mc_ERRORS[18];
    else                                 return mc_ERRORS[4];
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
        if (~silenced & mc_ARG_IS_NULL)
            mc_Error_put(mc_ARG_IS_NULL, 1,
                         "1st argument 'FILE *restrict stream' is NULL");
        return mc_ARG_IS_NULL;
    }

    /* Lock access to stream */
    pthread_mutex_lock(&mc_STREAM_ACCESS);

    /* Set new stream */
    mc_STREAM = stream;

    /* Grant access to stream */
    pthread_mutex_unlock(&mc_STREAM_ACCESS);

    return mc_OKAY;
}
