/* INFO ************************************************************************
**                                                                            **
**                                  MeaCulpa                                  **
**                                  ========                                  **
**                                                                            **
**      Sophisticated, minimalistic and high-level error handling for C       **
**                       Version: 0.1.4.096 (20150602)                        **
**                         File: MeaCulpa/MeaCulpa.c                          **
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

/* Include standard headers */
#include <stdio.h> /*
    const : stderr
    func  : fprintf
*/

/* Include MeaCulpa headers */
#include <MeaCulpa.h> /*
    const : mc_Okay
            mc_Fail
            mc_Depricated
            mc_Experimental
            mc_EOF
            mc_ZeroDiv
            mc_NullPtr
            mc_Value
            mc_Empty
            mc_Index
            mc_Key
            mc__Invalid__
*/

#define HINT_INDENT "       "
#define INDENT "    "
#define INDENTED    INDENT "%s\n"

/* Error type names */
static const char *const ERROR_TYPE_NAMES[] =
{
    [mc_Okay]         = "mc_Okay",
    [mc_Fail]         = "mc_Fail",
    [mc_Depricated]   = "mc_Depricated",
    [mc_Experimental] = "mc_Experimental",
    [mc_EOF]          = "mc_EOF",
    [mc_ZeroDiv]      = "mc_ZeroDiv",
    [mc_NullPtr]      = "mc_NullPtr",
    [mc_Value]        = "mc_Value",
    [mc_Empty]        = "mc_Empty",
    [mc_Index]        = "mc_Index",
    [mc_Key]          = "mc_Key",
};

/* Predefined error messages */
static const char *const ERROR_MESSAGES[] =
{
    [mc_Okay]         = "No error",
    [mc_Fail]         = "Unhandled error",
    [mc_Depricated]   = "This feature is deprecated",
    [mc_Experimental] = "This feature is experimental",
    [mc_EOF]          = "End of File",
    [mc_ZeroDiv]      = "Division by 0",
    [mc_NullPtr]      = "Argument is a pointer to NULL",
    [mc_Value]        = "Argument has an invalid value",
    [mc_Empty]        = "Container is empty",
    [mc_Index]        = "Index out of range",
    [mc_Key]          = "Key not found",
};

/* TODO: Create mc_printer => custom printer for user defined errors.
         Also, make mc_print only a wrapper around mc_printer */

/* TODO: Create optional colored output */

/* Print function */
void
mc_print(int                      error,
         const char *const        function,
         size_t                   line_count,
         const char *const *const lines)
{
    /* If print is not part of a trace-chain */
    if (error != mc_Okay)
        /* Start error */
        fprintf(stderr, "\nAn error occured:\n");

    /* If `function` argument is invalid */
    if (!function)
    {
        fprintf(stderr, INDENT "In function: mc_print()\n"
                        INDENT INDENT "Cannot print function name\n"
                        INDENT INDENT "(Hint: The 2nd argument 'function' "
                                      "is pointing to NULL)\n");
        return;
    }

    /* Print function name */
    fprintf(stderr, INDENT "In function: %s()\n", function);

    /* If `error` argument is invalid */
    if (error <  mc_Okay ||
        error >= mc__Invalid__)
    {
        fprintf(stderr, INDENT "In function: mc_print()\n"
                        INDENT INDENT "Cannot print predefined error message\n"
                        INDENT INDENT "(Hint: The 1st argument "
                        "`error`is not an `mc_ErrorType`)\n");
        return;
    }

    if (error != mc_Okay)
    {
        /* Print predefined messages */
        fprintf(stderr, INDENT INDENT "%s\n", ERROR_MESSAGES[error]);
        fprintf(stderr, INDENT INDENT "(Type: %s)\n",
                        ERROR_TYPE_NAMES[error]);
    }

    /* If `lines` argument is invalid */
    if (!lines && line_count)
    {
        fprintf(stderr, INDENT "In function: mc_print()\n"
                        INDENT INDENT "Cannot print user defined message\n"
                        INDENT INDENT "(Hint: 4th argument 'lines' "
                                      "is pointing to NULL)\n");
        return;
    }

    /* Print user defined messages */
    fprintf(stderr, INDENT INDENT "(Hint: %s", lines[0]);
    for (size_t i=1; i<line_count; i++)
        fprintf(stderr, "\n" INDENT INDENT HINT_INDENT "%s", lines[i]);
    fprintf(stderr, ")\n");
}
