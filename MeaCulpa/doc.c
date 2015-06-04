/* INFO ************************************************************************
**                                                                            **
**                                  MeaCulpa                                  **
**                                  ========                                  **
**                                                                            **
**      Sophisticated, minimalistic and high-level error handling for C       **
**                       Version: 0.1.4.122 (20150604)                        **
**                            File: MeaCulpa/doc.c                            **
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

#include <stddef.h> /*
    type  : size_t
*/
#include <stdio.h> /*
    func  : printf
*/

#define FIRST_EXAMPLE
// #define SECOND_EXAMPLE

typedef struct
{
    char *chars;
} RandChars;

/* Returns: mc_Okay, mc_Fail, mc_StdMalloc, mc_NullPtr */
mc_Error
RandChars_new(RandChars **const self,
              size_t            count)
{
    (void)self;
    (void)count;
    // return (mc_Error)mc_FAIL(RandChars_new);
    return (mc_Error)mc_ERROR(mc_NullPtr, RandChars_new);
}
mc_Error
RandChars_new_TRY(mc_Error signal)
{
    static const char *const function = "RandChars_new";
    static const char *const messages[] =
    {
        "Internal error",
        "1st argument `self` is NULL",
        "This should have never happened...",
    };

    mc_Error status = mc_FAIL(RandChars_new_TRY);
    if (mc_NOT_OWNER(RandChars_new, signal))
        return status;

    status.error = mc_Okay;
    switch (signal.error)
    {
        case mc_Okay:
            break;

        case mc_StdMalloc:
            mc_print(mc_StdMalloc, function, 1, messages);
            break;

        case mc_NullPtr:
            mc_print(mc_NullPtr, function, 1, messages + 1);
            break;

        /* mc_Fail or any other */
        default:
            mc_print(signal.error, function, 1, messages + 2);
            break;
    }

    return status;
}


char *
RandChars_gen(RandChars *const self)
{
    (void)self;
    return NULL;
}


void
RandChars_del(RandChars **const self)
{
    (void)self;
    return;
}



/*----------------------------------------------------------------------------*/
int
main(void)
{
    static const char *const messages[] = {"Cannot create new RandChars"};
    mc_Error signal;
    RandChars *rc;

#ifdef FIRST_EXAMPLE
    signal = RandChars_new(&rc, 5);
    if (mc_NOT_OKAY(signal))
    {
        RandChars_del(&rc);
        RandChars_new_TRY(signal);
        mc_print(mc_Okay, "main", 1, messages);
        return EXIT_FAILURE;
    }

    printf("%s\n", RandChars_gen(rc));
#endif /* FIRST_EXAMPLE */

#ifdef SECOND_EXAMPLE
    char *s;

    signal = RandChars_new(&rc, 5);
    if (mc_NOT_OKAY(signal))
        switch (signal.error)
        {
            case mc_NullPtr:
                s = "abcde";
                break;

            default:
                RandChars_new_TRY(signal);
                mc_print(mc_Okay, "main", 1, messages);
                return EXIT_FAILURE;
        }
    else
        s = RandChars_gen(rc);

    printf("%s\n", s);
#endif /* SECOND_EXAMPLE */

    RandChars_del(&rc);
    return EXIT_SUCCESS;
}
