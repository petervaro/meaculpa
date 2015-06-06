/* INFO ************************************************************************
**                                                                            **
**                                  MeaCulpa                                  **
**                                  ========                                  **
**                                                                            **
**      Sophisticated, minimalistic and high-level error handling for C       **
**                       Version: 0.1.6.185 (20150606)                        **
**                         File: MeaCulpa/MeaCulpa.h                          **
**                                                                            **
**               For more information about the project, visit                **
**                  <https://github.com/petervaro/MeaCulpa>.                  **
**                       Copyright (C) 2015 Peter Varo                        **
**                                                                            **
**  This program is free software: you can redistribute it and/or modify it   **
**   under the terms of the GNU General Public License as published by the    **
**       Free Software Foundation, either version 3 of the License, or        **
**                    (at your option) any later version.                     **
**                                                                            **
**    This program is distributed in the hope that it will be useful, but     **
**         WITHOUT ANY WARRANTY; without even the implied warranty of         **
**            MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            **
**            See the GNU General Public License for more details.            **
**                                                                            **
**     You should have received a copy of the GNU General Public License      **
**     along with this program, most likely a file in the root directory,     **
**        called 'LICENSE'. If not, see <http://www.gnu.org/licenses>.        **
**                                                                            **
************************************************************************ INFO */

/* Header guard */
#ifndef __MEA_CULPA_H_701077389399616__
#define __MEA_CULPA_H_701077389399616__

/* TODO: How would we solve the version differences? eg.: when a new enum is
         added to mc_ErrorTypes => static_assert? macro? etc? */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stddef.h> /*
    type  : size_t
*/
#include <stdio.h> /*
    const : stderr
    func  : fprintf
*/


/*----------------------------------------------------------------------------*/
/* Generic function pointer type */
typedef void (*mc_FnPtr)(void);



/*----------------------------------------------------------------------------*/
/* Error object */
typedef struct
{
    int      error; /* WHAT?  The error returned by the function */
    mc_FnPtr owner; /* WHO?   The function returned the error */
    int      index; /* WHERE? The location of the error in the function */
} mc_Error;



/*----------------------------------------------------------------------------*/
/* Helper macros for casting owner and matching ownership */
#define mc_OWNER(F) ((mc_FnPtr)&F)
#define mc_IS_OWNER(F, E)  (mc_OWNER(F) == (E).owner)
#define mc_NOT_OWNER(F, E) (mc_OWNER(F) != (E).owner)



/*----------------------------------------------------------------------------*/
/* Helper macros to initialize error-object */
#define mc_ERROR_VA(E, O, I, ...) {E, mc_OWNER(O), I}
#define mc_ERROR(...)             mc_ERROR_VA(__VA_ARGS__, 0, 0)



/*----------------------------------------------------------------------------*/
/* Predefined error types */
typedef enum
{
    /* No error OR matched ownership */
    mc_Okay,
    /* Uncaught error OR mismatched ownership */
    mc_Fail,

    /* Implementation errors */
    /* This feature is depricated */
    mc_Depricated,
    /* This feature is not stable */
    mc_Experimental,

    /* Standard based errors */
    /* Memory allocation failed */
    mc_StdMalloc,
    /* Clear memory allocation failed */
    mc_StdCalloc,
    /* Memory reallocation failed */
    mc_StdRealloc,

    /* Memory based error */
    /* There is not enough memory */
    mc_Memory,

    /* File I/O based errors */
    /* End-Of-File error */
    mc_EOF,

    /* Math based errors */
    mc_ZeroDiv,

    /* Function argument based errors */
    /* Argument points to NULL */
    mc_NullPtr,
    /* Argument's value is invalid */
    mc_Value,

    /* Container based errors */
    /* Container is empty */
    mc_Empty,
    /* Index out of range */
    mc_Index,
    /* Key is not found */
    mc_Key,

    /* THIS ENUM MUST BE THE LAST ONE
       DO NOT USE THIS ENUM WITH mc_print()
       (This is the last enum, which is used to
       identify the last valid enum) */
    mc__Invalid__
} mc_ErrorType;



/*----------------------------------------------------------------------------*/
/* TODO: make convenient macros for all mc_ErrorTypes */
/* Helper macros to initialize convenient error-objects */
#define mc_OKAY_VA(O, I, ...) mc_ERROR(mc_Okay, O, I)
#define mc_OKAY(...)          mc_OKAY_VA(__VA_ARGS__, 0, 0)
#define mc_FAIL_VA(O, I, ...) mc_ERROR(mc_Fail, O, I)
#define mc_FAIL(...)          mc_FAIL_VA(__VA_ARGS__, 0, 0)



/*----------------------------------------------------------------------------*/
/* Helper macros to check if there was an error */
#define mc_IS_OKAY(E)  ((E).error == mc_Okay)
#define mc_NOT_OKAY(E) ((E).error != mc_Okay)



/*----------------------------------------------------------------------------*/
/* Generic error printing macro */
/* TODO: Create optional colored output using `prism.h` */
#define _HINT_IND "       "
#define _IND      "  "
#define mc_PRINT(error,                                                        \
                 function,                                                     \
                 line_count,                                                   \
                 lines,                                                        \
                 PRINTER,                                                      \
                 MIN_VALID,                                                    \
                 MAX_VALID,                                                    \
                 TYPE_NAMES,                                                   \
                 MESSAGES)                                                     \
    do                                                                         \
    {                                                                          \
        /* If print is not part of a trace-chain */                            \
        if (error != mc_Okay)                                                  \
            /* Start error */                                                  \
            fprintf(stderr, "\nAn error occured:\n");                          \
        /* If `function` argument is invalid */                                \
        if (!function)                                                         \
        {                                                                      \
            fprintf(stderr, _IND "In function: " PRINTER "\n"                  \
                            _IND _IND "Cannot print function name\n"           \
                            _IND _IND "(Hint: The 2nd argument 'function' "    \
                                          "is pointing to NULL)\n");           \
            return;                                                            \
        }                                                                      \
        /* Print function name */                                              \
        fprintf(stderr, _IND "In function: %s\n", function);                   \
        /* If `error` argument is invalid */                                   \
        if (error <  MIN_VALID ||                                              \
            error >= MAX_VALID)                                                \
        {                                                                      \
            fprintf(stderr, _IND "In function: " PRINTER "\n"                  \
                            _IND _IND "Cannot print predefined error message\n"\
                            _IND _IND "(Hint: The 1st argument "               \
                            "`error`is not an `mc_ErrorType`)\n");             \
            return;                                                            \
        }                                                                      \
        if (error != mc_Okay)                                                  \
            /* Print predefined messages */                                    \
            fprintf(stderr, _IND _IND "%s: %s\n", TYPE_NAMES[error],           \
                                                      MESSAGES[error]);        \
        /* If `lines` argument is invalid */                                   \
        if (!lines && line_count)                                              \
        {                                                                      \
            fprintf(stderr, _IND "In function: " PRINTER "\n"                  \
                            _IND _IND "Cannot print user defined message\n"    \
                            _IND _IND "(Hint: 4th argument 'lines' "           \
                                          "is pointing to NULL)\n");           \
            return;                                                            \
        }                                                                      \
        /* Print user defined messages */                                      \
        fprintf(stderr, _IND _IND "(Hint: %s", lines[0]);                      \
        for (size_t i=1; i<line_count; i++)                                    \
            fprintf(stderr, "\n" _IND _IND _HINT_IND "%s", lines[i]);          \
        fprintf(stderr, ")\n");                                                \
    }                                                                          \
    while (0)



/*----------------------------------------------------------------------------*/
/* Print function */
void
mc_print(const mc_ErrorType       error,
         const char *const        function,
         const size_t             line_count,
         const char *const *const lines);

#endif /* __MEA_CULPA_H_701077389399616__ */
