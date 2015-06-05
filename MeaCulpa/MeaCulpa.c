/* INFO ************************************************************************
**                                                                            **
**                                  MeaCulpa                                  **
**                                  ========                                  **
**                                                                            **
**      Sophisticated, minimalistic and high-level error handling for C       **
**                       Version: 0.1.4.158 (20150605)                        **
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
/* Include MeaCulpa headers */
#include "MeaCulpa.h" /*
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
    macro : mc_PRINT
*/

/* Error type names */
static const char *const ERROR_TYPE_NAMES[] =
{
    [mc_Okay]         = "mc_Okay",
    [mc_Fail]         = "mc_Fail",
    [mc_Depricated]   = "mc_Depricated",
    [mc_Experimental] = "mc_Experimental",
    [mc_StdMalloc]    = "mc_StdMalloc",
    [mc_StdCalloc]    = "mc_StdCalloc",
    [mc_StdRealloc]   = "mc_StdRealloc",
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
    [mc_StdMalloc]    = "Memory allocation failed",
    [mc_StdCalloc]    = "Clear memory allocation failed",
    [mc_StdRealloc]   = "Memory reallocation failed",
    [mc_EOF]          = "End of File",
    [mc_ZeroDiv]      = "Division by 0",
    [mc_NullPtr]      = "Argument is a pointer to NULL",
    [mc_Value]        = "Argument has an invalid value",
    [mc_Empty]        = "Container is empty",
    [mc_Index]        = "Index out of range",
    [mc_Key]          = "Key not found",
};

/* TODO: Create optional colored output */

/* Print function */
void
mc_print(const mc_ErrorType       error,
         const char *const        function,
         const size_t             line_count,
         const char *const *const lines)
{
    mc_PRINT(error,
             function,
             line_count,
             lines,
             "mc_print",
             mc_Okay,
             mc__Invalid__,
             ERROR_TYPE_NAMES,
             ERROR_MESSAGES);
}
