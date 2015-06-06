/* INFO ************************************************************************
**                                                                            **
**                                  MeaCulpa                                  **
**                                  ========                                  **
**                                                                            **
**      Sophisticated, minimalistic and high-level error handling for C       **
**                       Version: 0.1.4.166 (20150606)                        **
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
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/

#include "MeaCulpa.h"

typedef struct
{
    size_t size;
    char  *chars;
} RandChars;


/* Returns: mc_Okay, mc_Fail, mc_StdMalloc, mc_NullPtr */
mc_Error
RandChars_new(RandChars **const self,
              size_t            count)
{
    (void)self;
    (void)count;
    // return (mc_Error)mc_FAIL(RandChars_new);
    return (mc_Error)mc_OKAY(RandChars_new);
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


/* Returns: mc_Okay, mc_Fail, mc_NullPtr:0, mc_NullPtr:1, mc_Memory */
mc_Error
RandChars_gen(RandChars  *const self,
              char      **const buffer)
{
    (void)self;
    (void)buffer;
#if   defined(E1a)
    return (mc_Error)mc_ERROR(mc_NullPtr, RandChars_gen);
#elif defined(E1b)
    return (mc_Error)mc_ERROR(mc_Memory, RandChars_gen);
#endif
}
mc_Error
RandChars_gen_TRY(mc_Error signal)
{
    static const char *const function = "RandChars_gen";
    static const char *const messages[] =
    {
        "1st argument `self` is NULL",
        "2nd argument `buffer` is NULL",
        "One of the arguments is NULL, but the error-index is invalid",
        "Size of `chars` member is too small",
        "This should have never happened...",
    };

    mc_Error status = mc_FAIL(RandChars_gen_TRY);
    if (mc_NOT_OWNER(RandChars_gen, signal))
        return status;

    status.error = mc_Okay;
    switch (signal.error)
    {
        case mc_Okay:
            break;

        case mc_NullPtr:
            switch (signal.index)
            {
                case 0:
                    mc_print(mc_NullPtr, function, 1, messages);
                    break;

                case 1:
                    mc_print(mc_NullPtr, function, 1, messages + 1);
                    break;

                default:
                    mc_print(mc_NullPtr, function, 1, messages + 2);
                    break;
            }
            break;

        case mc_Memory:
            mc_print(mc_Memory, function, 1, messages + 3);
            break;

        /* mc_Fail or any other */
        default:
            mc_print(signal.error, function, 1, messages + 4);
            break;
    }

    return status;
}


/* Returns: mc_Okay, mc_Fail, mc_StdRealloc, mc_NullPtr */
mc_Error
RandChars_grow(RandChars *self,
               size_t     size)
{
    (void)self;
    (void)size;
    return (mc_Error)mc_OKAY(RandChars_grow);
}
mc_Error
RandChars_grow_TRY(mc_Error signal)
{
    static const char *const function = "RandChars_grow";
    static const char *const messages[] =
    {
        "Cannot reallocate space for the `chars` member",
        "This should have never happened...",
    };
    mc_Error status = mc_FAIL(RandChars_grow_TRY);
    if (mc_NOT_OWNER(RandChars_grow, signal))
        return status;

    status.error = mc_Okay;
    switch (signal.error)
    {
        case mc_Okay:
            break;

        case mc_StdRealloc:
            mc_print(mc_StdRealloc, function, 1, messages);

        /* mc_Fail or any other */
        default:
            mc_print(signal.error, function, 1, messages + 1);
            break;
    }

    return status;
}


void
RandChars_del(RandChars **const self)
{
    (void)self;
    return;
}



/*----------------------------------------------------------------------------*/
// mc_Error
// my_RandChars_new_wrapper(RandChars **const self,
//                          size_t            count)
// {
//     mc_Error others,
//              signal = mc_FAIL(my_RandChars_new_wrapper);

//     others = RandChars_new();
//     if (mc_NOT_OKAY(others))
//         return others;

//     signal.error = mc_Okay;
//     return signal;
// }

// mc_Error
// my_RandChars_new_wrapper_TRY(mc_Error signal)
// {
//     mc_Error status = mc_FAIL(my_RandChars_new_wrapper_TRY));
//     static const char *const function = "my_RandChars_new_wrapper";
//     static const char *const messages =
//     {
//         "Cannot create new RandChars object",
//         "Unknown error",
//     };

//     if (mc_IS_OKAY(RandChars_new_TRY(signal)))
//     {
//         mc_print(mc_Okay, 1, function, messages);
//         status.error = mc_Okay;
//         return status;
//     }
//     else if (mc_IS_OWNER(my_RandChars_new_wrapper, signal))
//         return status;

//     status.error = mc_Okay;
//     switch (signal.error)
//     {
//         case mc_Okay:
//             break;

//         default:
//             mc_print(mc_Fail, 1, function, messages + 1);
//             break;
//     }

//     return status;
// }


// mc_Error
// my_RandChars_gen_wrapper(RandChars  *const self,
//                          char      **const buffer)
// {
//     mc_Error others,
//              signal = mc_FAIL(my_RandChars_gen_wrapper);

//     others = RandChars_gen();
//     if (mc_NOT_OKAY(others))
//         return others;

//     signal.error = mc_Okay;
//     return signal;
// }

// mc_Error
// my_RandChars_gen_wrapper_TRY(mc_Error signal)
// {
//     mc_Error status = mc_FAIL(my_RandChars_gen_wrapper_TRY));
//     return status;
// }

/*----------------------------------------------------------------------------*/
#ifdef E1a
int
main(void)
{
    /* Error object and error messages */
    mc_Error signal;
    static const char *const function = "main";
    static const char *const messages[] =
    {
        "Cannot create new RandChars object",
        "Cannot generate RandChars characters",
    };

    /* Create RandChars object */
    RandChars *rc;
    signal = RandChars_new(&rc, 5);
    if (mc_NOT_OKAY(signal))
    {
        RandChars_new_TRY(signal);
        mc_print(mc_Okay, function, 1, messages);
        goto New_Error;
    }

    /* Generate random characters */
    char *s;
    signal = RandChars_gen(rc, &s);
    if (mc_NOT_OKAY(signal))
    {
        RandChars_gen_TRY(signal);
        mc_print(mc_Okay, function, 1, messages + 1);
        goto Gen_Error;
    }

    /* If there was no error, print characters */
    printf("%s\n", s);

    /* Clean up and return */
    RandChars_del(&rc);
    return EXIT_SUCCESS;

    /* If there was an error */
    Gen_Error:
    New_Error:
        RandChars_del(&rc);
        return EXIT_FAILURE;
}
#endif /* E1a */

#ifdef E1b
int
main(void)
{
    /* Error object and error messages */
    mc_Error signal;
    static const char *const function = "main";
    static const char *const messages[] =
    {
        "Cannot create new RandChars object",
        "Cannot generate RandChars characters",
        "Cannot increase size of character buffer",
    };

    /* Create RandChars object */
    RandChars *rc;
    signal = RandChars_new(&rc, 0);
    if (mc_NOT_OKAY(signal))
    {
        RandChars_new_TRY(signal);
        mc_print(mc_Okay, function, 1, messages);
        goto New_Error;
    }

    /* Generate random characters */
    char *s;
    signal = RandChars_gen(rc, &s);
    switch (signal.error)
    {
        case mc_Okay:
            break;

        /* We chose to deal with only the case, when there is not
           enough memory in RandChars object, to generate chars */
        case mc_Memory:
            signal = RandChars_grow(rc, 5);
            if (mc_NOT_OKAY(signal))
            {
                RandChars_grow_TRY(signal);
                mc_print(mc_Okay, function, 1, messages + 2);
                goto Grow_Error;
            }

            signal = RandChars_gen(rc, &s);
            if (mc_NOT_OKAY(signal))
            {
                RandChars_gen_TRY(signal);
                mc_print(mc_Okay, function, 1, messages + 1);
                goto Gen_Error;
            }
            break;

        default:
            RandChars_gen_TRY(signal);
            mc_print(mc_Okay, function, 1, messages + 1);
            goto Gen_Error;
    }

    /* If there was no error, print characters */
    printf("%s\n", s);

    /* Clean up and return */
    RandChars_del(&rc);
    return EXIT_SUCCESS;

    /* If there was an error */
    Grow_Error:
    Gen_Error:
    New_Error:
        RandChars_del(&rc);
        return EXIT_FAILURE;
}
#endif /* E1b */

#ifdef E2
#endif /* E2 */
