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
/* Error silencer wildcards */
extern const mc_Error   mc_MUTE_NONE,
                        mc_MUTE_ALL;


/*----------------------------------------------------------------------------*/
/* Error signals */
extern const mc_Error   mc_OKAY,

                        /* Generic error */
                        mc_FAIL,

                        /* Implementation specific errors */
                        mc_DEPRECATED,
                        mc_EXPERIMENTAL,

                        /* Meaculpa specific errors */
                        mc_UNKNOWN_ERROR,

                        /* Function invoking specific errors */
                        mc_ARG_IS_NULL,
                        mc_INVALID_VALUE,
                        mc_INVALID_INDEX,
                        mc_INVALID_KEY,
                        mc_STOP_ITERATION,

                        /* Memory and instance management errors */
                        mc_ALLOC_FAIL,
                        mc_INI_FAIL,
                        mc_FIN_FAIL,

                        /* File I/O specific errors */
                        mc_FILE_ACCESS,
                        mc_FILE_NOT_FOUND,
                        mc_PERMISSION_FAIL,
                        mc_END_OF_LINE,
                        mc_END_OF_FILE,

                        /* Math specific errors */
                        mc_ZERO_DIVISION,

                        /* Error markers
                           WARN: DO NOT USE THESE MARKERS DIRECTLY! */
                        mc__MARKER_16,
                        mc__MARKER_15,
                        mc__MARKER_14,
                        mc__MARKER_13,
                        mc__MARKER_12,
                        mc__MARKER_11,
                        mc__MARKER_10,
                        mc__MARKER_9,
                        mc__MARKER_8,
                        mc__MARKER_7,
                        mc__MARKER_6,
                        mc__MARKER_5,
                        mc__MARKER_4,
                        mc__MARKER_3,
                        mc__MARKER_2,
                        mc__MARKER_1;


/*----------------------------------------------------------------------------*/
/* TODO: **mc_Error_str_set**
         Consider turning mc_Error_str into a function pointer, which is set to
         the default str method at the beginning. The mc_Error_str_set would
         allow the user to specify another method which could be used to extend
         the number of errors (and therefore their representations) with custom
         user defined ones. The only drawback I see right now, is how to manage,
         if there are at least two libraries trying to work together and both
         are using meaculpa and trying to overload the mc_Error_str method with
         their own versions? Not to mention the overlapping error constants. One
         possible solution could be the explicit passing of mc_Error_str to
         mc_Error__put each time it is called.. so much overhead :( */
const char*
mc_Error_str(mc_Error error);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* TODO: **fput and sput**
         mc_Error_put  => output to mc_STREAM
         mc_Error_fput => output to a call-time specified stream
         mc_Error_sput => output to a call-time specified string-buffer
         Rationale: meaculpa should work in a threaded environment. If there are
         multiple threads printing to the same unbuffered stream, the output
         will be chaotic and the back-trace would be useless. But if the user
         has the option to print the back-trace into a buffer, which is used by
         a single thread, then at the end, when threads are joined or aborted
         the parent process could print out the whole back-trace at once from
         the buffer. Don't forget to add an option to use buffer as TTY or not
         (should the buffer store the ASCII escape sequences or not) */
void
mc_Error__put(mc_Error    error,
              const char *function,
              const char *file,
              int         messages,
           /* const char* */ ...);


/*----------------------------------------------------------------------------*/
void
mc_stream_ini(void);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void
mc_stream_fin(void);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Return : mc_Error_OKAY
            mc_Error_ARG_IS_NULL */
mc_Error
mc_stream_set(FILE     *stream,
              mc_Error  muted);


/*----------------------------------------------------------------------------*/
#define MC__MARKED(E, I)      (mc_##E | mc__MARKER_##I)
#define mc_OKAY(I)            MC__MARKED(OKAY, I)
#define mc_FAIL(I)            MC__MARKED(FAIL, I)
#define mc_DEPRECATED(I)      MC__MARKED(DEPRECATED, I)
#define mc_EXPERIMENTAL(I)    MC__MARKED(EXPERIMENTAL, I)
#define mc_UNKNOWN_ERROR(I)   MC__MARKED(UNKNOWN_ERROR, I)
#define mc_ARG_IS_NULL(I)     MC__MARKED(ARG_IS_NULL, I)
#define mc_INVALID_VALUE(I)   MC__MARKED(INVALID_VALUE, I)
#define mc_INVALID_INDEX(I)   MC__MARKED(INVALID_INDEX, I)
#define mc_INVALID_KEY(I)     MC__MARKED(INVALID_KEY, I)
#define mc_STOP_ITERATION(I)  MC__MARKED(STOP_ITERATION, I)
#define mc_ALLOC_FAIL(I)      MC__MARKED(ALLOC_FAIL, I)
#define mc_INI_FAIL(I)        MC__MARKED(INI_FAIL, I)
#define mc_FIN_FAIL(I)        MC__MARKED(FIN_FAIL, I)
#define mc_FILE_ACCESS(I)     MC__MARKED(FILE_ACCESS, I)
#define mc_FILE_NOT_FOUND(I)  MC__MARKED(FILE_NOT_FOUND, I)
#define mc_PERMISSION_FAIL(I) MC__MARKED(PERMISSION_FAIL, I)
#define mc_END_OF_LINE(I)     MC__MARKED(END_OF_LINE, I)
#define mc_END_OF_FILE(I)     MC__MARKED(END_OF_FILE, I)
#define mc_ZERO_DIVISION(I)   MC__MARKED(ZERO_DIVISION, I)
#define mc_Error_put(E, ...)  mc_Error__put(E, __func__, __FILE__, __VA_ARGS__)

/* TODO: **message-macros**
         Create predefined message composer macros */

#endif /* MC_MEACULPA_H_1954125576046949 */
