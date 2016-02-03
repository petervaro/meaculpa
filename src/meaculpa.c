/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  type  : FILE
    const : stderr
    func  : fputs */
#include <stddef.h>
/*  const : NULL */
#include <stdarg.h>
/*  type  : va_list
    macro : va_start
            va_arg
            va_end */
#include <stdlib.h>
/*  func  : abs */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include threads headers */
#include <threads/threads.h>
/*  type  : mtx_t
    const : mtx_plain
    func  : mtx_init
            mtx_destroy
            mtx_lock
            mtx_unlock */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include syswrap headers */
#include <syswrap/io.h>
/*  func  : sw_isatty */

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
static FILE  *mc_STREAM = NULL;
static mtx_t  mc_STREAM_ACCESS;


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


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
const char*
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


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline void
mc_Error__put_tty(FILE       *stream,
                  mc_Error    error,
                  const char *function,
                  const char *file,
                  int         messages,
                  va_list     lines)
{
    /* Local variables */
    int i;

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
            fputs(RC_S(RC_BOLD, ""), stream);
            for (i=0; i<abs(messages); i++)
                fputs(va_arg(lines, const char *restrict), stream);
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
    /* If the error occured elsewhere, and this is just a propagation */
    else if (function)
    {
        fputs(RC_F(RC_YELLOW, "> ")
              RC_XF("Called from: ")
              RC_FS(RC_MAGENTA, RC_BOLD, ""), stream);
        fputs(function, stream);

        /* If there are extra hints passed */
        if (messages)
        {
            fputs(RC_FS(RC_MAGENTA, RC_NORMAL, " (Note: "), stream);
            for (i=0; i<abs(messages); i++)
                fputs(va_arg(lines, const char *restrict), stream);
            fputs(")", stream);
        }

        fputs(RC_XFBS("\n"), stream);
    }
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline void
mc_Error__put_etc(FILE       *stream,
                  mc_Error    error,
                  const char *function,
                  const char *file,
                  int         messages,
                  va_list     lines)
{
/* Local variables */
    int i;

    /* If this is where the error occured */
    if (messages < 0)
    {
        fputs("\nRuntime error: ", stream);
        fputs(mc_Error_str(error), stream);

        /* If there are error messages passed */
        fputs(" => ", stream);
        if (messages)
            for (i=0; i<abs(messages); i++)
                fputs(va_arg(lines, const char *restrict), stream);
        else
            fputs("[No error message specified]", stream);
        fputs("\n", stream);

        /* If function name is not NULL */
        if (function)
        {
            fputs("While calling: ", stream);
            fputs(function, stream);
            fputs(" (File: ", stream);
            fputs(file, stream);
            fputs(")\n", stream);
        }
    }
    /* If the error occured elsewhere, and this is just a propagation */
    else if (function)
    {
        fputs("> Called from: ", stream);
        fputs(function, stream);

        /* If there are extra hints passed */
        if (messages)
        {
            fputs(" (Note: ", stream);
            for (i=0; i<abs(messages); i++)
                fputs(va_arg(lines, const char *restrict), stream);
            fputs(")", stream);
        }

        fputs("\n", stream);
    }
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_Error__put(mc_Error    error,
              const char *function,
              const char *file,
              int         messages,
           /* const char* */ ...)
{
    /* Local variables */
    va_list lines;

    #ifndef MC_FAST
        /* If stream is not initialized yet */
        if (!mc_STREAM)
        {
            /* TODO: **warnings**
                     Consider to add runtime-warning option to meaculpa.
                     Warnings should be removed if FAST is defined. Otherwise
                     warning should be raised only if the error is safely
                     handled, but it should've never happenned */
            fputs("Warning: stream is uninitialized for mc_Error", stderr);
            mc_stream_ini();
        }
    #endif

    /* Lock access to stream */
    mtx_lock(&mc_STREAM_ACCESS);

    /* Initialize va_list */
    va_start(lines, messages);

    /* If stream is connected to a terminal */
    if (sw_isatty(mc_STREAM))
        mc_Error__put_tty(mc_STREAM, error, function, file, messages, lines);
    /* If stream is not connected to a terminal */
    else
        mc_Error__put_etc(mc_STREAM, error, function, file, messages, lines);

    /* Finalize va_list */
    va_end(lines);

    /* Grant access to stream */
    mtx_unlock(&mc_STREAM_ACCESS);
}


/*----------------------------------------------------------------------------*/
void
mc_stream_ini(void)
{
    /* Initialize access mutex */
    mtx_init(&mc_STREAM_ACCESS, mtx_plain);

    /* Lock access to stream */
    mtx_lock(&mc_STREAM_ACCESS);

    /* Set default stream */
    mc_STREAM = stderr;

    /* Grant access to stream */
    mtx_unlock(&mc_STREAM_ACCESS);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_stream_fin(void)
{
    mtx_destroy(&mc_STREAM_ACCESS);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
mc_Error
mc_stream_set(FILE     *stream,
              mc_Error  muted)
{
    /* If stream is NULL */
    if (!stream)
    {
        if (~muted & mc_ARG_IS_NULL)
            mc_Error_put(mc_ARG_IS_NULL, 1,
                         "1st argument 'FILE *restrict stream' is NULL");
        return mc_ARG_IS_NULL;
    }

    /* Lock access to stream */
    mtx_lock(&mc_STREAM_ACCESS);

    /* Set new stream */
    mc_STREAM = stream;

    /* Grant access to stream */
    mtx_unlock(&mc_STREAM_ACCESS);

    return mc_OKAY;
}
