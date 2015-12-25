/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  func  : printf */
#include <stdlib.h>
/*  const : EXIT_SUCCESS
            EXIT_FAILURE */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include <meaculpa/meaculpa.h>
/*  type  : mc_Error
    macro : mc_Error_put
    const : mc_MUTE_NONE
            mc_OKAY
            mc_ARG_IS_NULL
            mc_ZERO_DIVISION */

/*  This example is about handling errors. Meaculpa's mc_Error_put macro is a
    nice and powerful tool, that can be used to print out full back-trace
    messages. But what if, the caller can recover from one of the errors? Why
    print an error message then? Meaculpa is based on bit-masking techniques, so
    it is very easy and cheap to pass a 'muted' flag, and specify which errors
    should be muted explicitly. (Or one can use the wildcards: mc_MUTE_NONE to
    let all errors print their messages or mc_MUTE_ALL to prevent any errors to
    print their error messages.) Remember though, that even if an error is
    muted, that won't effect the return value of a function -- the error will
    still be there, but its message won't be printed.

    The first function is called 'divide', which takes two ints, divides them
    and assigns the result to an output-argument. If the output argument (which
    is a pointer to int) is NULL, it won't do anything, but it will return back
    mc_ARG_IS_NULL, and if this error is not muted, it will also print the
    error-message as well. If the divisor is 0, it will return mc_ZERO_DIVISON,
    and if this error is not muted, it will also print the error-message as
    well.

    The second function, the 'div_default' is a wrapper around the first one. It
    takes one more argument 'on_zero'. This function will handle the
    mc_ZERO_DIVISION error if it returned from 'divide', and it will assign to
    the 'quotient' whatever value is passed to 'on_zero'. However if the
    'quotient' is NULL, it will let this error to be "raised". */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline mc_Error
divide(int           dividend,
       int           divisor,
       int *restrict quotient,
       mc_Error      muted);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline mc_Error
div_default(int           dividend,
            int           divisor,
            int *restrict quotient,
            int           on_zero,
            mc_Error      muted);



/*----------------------------------------------------------------------------*/
int
main(void)
{
    int      i,
             result = 0;
    mc_Error error;

    for (i=0; i<5; i++)
    {
        div_default(5, i, &result, 0, mc_MUTE_NONE);
        printf("result = %d\n", result);
    }

    for (i=0; i<5; i++)
    {
        if ((error = div_default(5, i, NULL, 0, mc_MUTE_NONE)))
        {
            mc_Error_put(error, 1, "Cannot divide 5 / i");
            break;
        }
        printf("result = %d\n", result);
    }
    return EXIT_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Return : mc_OKAY
            mc_ARG_IS_NULL */
static inline mc_Error
div_default(int           dividend,
            int           divisor,
            int *restrict quotient,
            int           on_zero,
            mc_Error      muted)
{
    mc_Error error;
    if ((error = divide(dividend, divisor, quotient, muted | mc_ZERO_DIVISION)))
    {
        if (error == mc_ZERO_DIVISION)
        {
            *quotient = on_zero;
            return mc_OKAY;
        }

        if (~muted & error)
            mc_Error_put(error, 1, "Cannot divide the integers");
    }
    return error;
}



/*----------------------------------------------------------------------------*/
/* Return : mc_OKAY
            mc_ARG_IS_NULL
            mc_ZERO_DIVISION */
static inline mc_Error
divide(int           dividend,
       int           divisor,
       int *restrict quotient,
       mc_Error      muted)
{
    if (!quotient)
    {
        if (~muted & mc_ARG_IS_NULL)
            mc_Error_put(mc_ARG_IS_NULL, -1,
                         "3rd argument 'int *restrict quotient' is NULL");
        return mc_ARG_IS_NULL;
    }

    if (!divisor)
    {
        if (~muted & mc_ZERO_DIVISION)
            mc_Error_put(mc_ZERO_DIVISION, -1, "divisor is 0");
        return mc_ZERO_DIVISION;
    }

    *quotient = dividend/divisor;
    return mc_OKAY;
}
