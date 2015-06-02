`MeaCulpa`
==========

What is `MeaCulpa`?
-------------------

A small and minimalistic error-handling library written in C. The main goal was
to fulfill the following requirements and create a(n):

  - (almost) zero-overhead,
  - stack-based (no dynamic allocations),
  - fully backtraced,
  - meaningful error-message generator,
  - thread-safe (locally handled),
  - type-safe,
  - extensible, flexible and modular,
  - truly encapsulated (higher level of abstraction),
  - maintainable,

error-handling library, which can be used in any C related projects. *(But of
course in the first place it was made to be used by all my personal projects.)*



Why another error-handling solution?
------------------------------------

C's error handling is explicit, which means that we have to decide what to do
immediately, right after calling a function by (most communly) checking its
return value.

This kind of explicitness is not necessarily a bad thing, but it makes quite
difficult to encapsulate the problem of error checking, or to backtrace the
error when it has happened during a nested function call chain. And it is
especially very hard to produce meaningful error messages when a function is
well designed and generic enough to be used in several different situations.
Even though this kind of help is almost mandatory to have better debugging
sessions and to bring 'A' class support for clients/users, and overall to
develop more stable and secure software.

While most higher-level languages support some sort of exception handling, those
also have their own costs too. For instance, if inside an exception handler
block two different functions can raise exceptions of the same type, it is kind
of difficult to distinguish between the two errors and inform the user properly.

Of course one possible solution in that case is to use two separate exception
handling blocks. Another one is to add extra informations to the exception
object itself and later on check for this added data once the exception has
caught. But these are almost exactly the same situations as explicitly and
immediately checking the two function calls capable of raising exceptions
separately.

*(However one good thing about exceptions is that they will be raised and most
likely print nice error-messages even without carefully handling them.)*

`MeaCulpa` tries to solve these questions without the need of a bloated,
process- and memory-expensive framework by also being loyal to the C philosophy
and traditions -- it is still explicit and lets the user to have full control.



Okay, these are the pros, but are there any cons?
-------------------------------------------------

Of course there are, there is no such thing as free meal, although there are
very cheap ones, and `MeaCulpa` is one of them.

One of the cons is, one has to be a bit more verbose then usual, because one has
to write the so called "try-function" counterpart of a single function as well.
Sure, it could bw cumbersome, and it needs focus and attention as well, but is
for the greateelr good. The code containing the try-functions would be a nice
experience to use for others, and it will also makes one lifes a hell lot easier
when it comes to debugging.

The other con is, that it means a small overhead, because some checking (for
exanple the owner matching) will run more than once, until one of the
try-functions finally find a match. This is the only processing overhead
`MeaCulpa` has compared to the traditional inplace return value checking.

The other "overhead" this library has is the size of the program code. It won't
effect the speed, but it will effect the size of the executable: it will be
bigger.

And last but not least, one has to be careful when defining the owner IDs to be
absolutely unique. It is not really a big deal, but if one misses it, it could
lead to unexpected errors, which are most likely handled, but not the expected
parts of the code will return them, and the backtrace messages won't be any help
either as they are heavily relying on the owner IDs uniqueness.



Yeah, but how does it work?
---------------------------

First of all, `MeaCulpa` (the whole library has pseudo-namespace, so all the
defined types, functions and macros has the `mc_` prefix before them) has an
error type which a struct. This type will be the return value of all functions
which wants to propagate their missbehaviour. The struct looks like this:

```C
typedef struct
{
    int error; /* WHAT?  The error returned by the function                 */
    int owner; /* WHO?   The function returned the error                    */
    int index; /* WHERE? The location of the error returned by the function */
} mc_Error;
```

The first two members are mandatory but the index can be omitted in most of the
cases. (This paper will discuss this later.)

The first member will be the type of the error, which specifies what went wrong,
or indicates if everything is fine. `MeaCulpa` provides predefined errors, but
the user can define custom ones as well, that's one of the reasons why the error
member has a "broad" type instead of the predefined error-types' type
the `mc_ErrorType`.

The predefined error-types are:

```C
typedef enum
{
    mc_OKAY,
    mc_FAIL,
    mc_STD_MALLOC,
    mc_STD_MEMCPY,
    mc_NULL_PTR_ARG,
    mc_EMPTY,
    mc_INDEX,
    ...
} mc_ErrorType;
```

(One can find more detailed informations about the meanings and conventional
usages of these error-types in the documentation.)

To be able to identify this error later (to build the error-stack on-the-fly) we
have to specify the second member, the `owner` as well, which has to be a unique
ID among the entire program. In almost all of the situations the owner should
be the function, who returned the error. For that reason we will use .......

A try-function has to be implemented as well, which will do the owner-matching
and the error-message printing as well.

For the error printing the try-function and we as well are using `MeaCulpa`'s
own printer function, which is thw following:

```C
void
mc_Error_print(int      error,
               size_t   line_count,
               char   **lines);
```

As you can see, it takes an error (`mc_Error.error`) and the number of lines we
would like to print, and the lines themselves. The message it will print is a
prettified error backtrace, with the library's predefined error message
corresponding to the passed error, and then our passed messages. There is
however exception: if we pass `mc_OKAY`, it will only print the messages we send
them. This is very useful, because we will use it, to build the backtrace.



First example
-------------

So let's take a look at an example. Let's say, we have a function from a library
which looks like this:

```C
mc_Error
SomeLib_new(void **const ptr
            int         *value);
```

Conventionally, this function can return `mc_OKAY` indicating that there was no
error, and `mc_FAIL` which means there was an unhandled error during the call of
this function. Besides the conventional return values, it can also return
`mc_STD_MALLOC` which means, that the `malloc` function declared in `stdlib.h`
returned a `NULL` pointer; and it can also return `mc_NULL_PTR_ARG` which means,
that the first argument `ptr` or the second one `value` is `NULL`.

It also comes with a try-function, which looks like this:

```C
mc_Error
SomeLib_Try_new(mc_Error);
```

As you can see, a try-function also returns an `mc_Error` which has to be two
values only: `mc_OKAY` which indicates, that the try-function matched the owner
and the proper actions took place; or `mc_FAIL` which means it could not find
the owner of the error.

Let's take a look at a very simple example, of how we may want to use this
function and its try-function:

```C
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/

#include <SomeLib.h> /*
    func  : SomeLib_new
            SomeLib_Try_new
            SomeLib_del
*/

int
main(void)
{
    /* Create an error signal object */
    mc_Error signal;

    /* Create data to be passed to SomeLib's functions */
    void *p;
    int   i = 12;

    signal = SomeLib_new(&ptr, &i);
    switch (signal.error)
    {
        /* If there was no error, we can move on */
        case mc_OKAY;
            break;

        /* We choose, to deal with only the NULL pointer argument problem,
           because we think we can handle it */
        case mc_NULL_PTR_ARG:
            ptr = NULL;
            break;

        /* If the call of this function returned mc_FAIL or mc_STD_MALLOC,
           we decided we can't handle those exception, so we pass the signal
           to the try-function. Hopefully it will print out a bunch of useful
           information we could use to understand the nature of the problem.
           After that, we print our own message, and quits our program with
           indicating it was a failure */
        default:
            SomeLib_Try_new(signal);
            /* We pass mc_OKAY to the printer function to indicate that this
               is not the place where the error occured */
            mc_Error_print(mc_OKAY, 1, "main(): Something went wrong...");
            return EXIT_FAILURE;
    }

    /* Let's clean-up the garbage */
    SomeLib_del(&ptr);

    return EXIT_SUCCESS;
}
```

Notice, that we did not care about the return value of the try-function. That's
because this was such a simple example. But if we use this function inside
another function, we have to be more careful than that!



Second example
--------------

```C
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/

#include <SomeLib.h> /*
    func  : SomeLib_new
            SomeLib_Try_new
            SomeLib_del
    const : SomeLib_Owner_new
*/

/* ..... THE OWNER ID IS DEFINED HERE ..... */

static mc_Error
my_wrapper_new(char **string,
               int   *value)
{
    mc_Error others,
             signal = {mc_FAIL, Owner_my_wrapper_new, 0};

    ...

    others = SomeLib_new(string, value);
    if (others.error != mc_OKAY)
        return others;

    if (!memcpy(*string, "wrapped", sizeof "wrapped"))
    {
        signal.error = mc_STD_MEMCPY;
        return signal;
    }

    ...

    return signal;
}

static mc_Error
Try_my_wrapper_new(mc_Error signal)
{
    /* This error object will be returned by the try-function */
    mc_Error status = {mc_FAIL, 0, 0};

    /* Check if this error belongs to the SomeLib's function.
       If so, it will do the rest for us, we can return a status
       indicating, that we have found the owner */
    if (SomeLib_Try_new(signal).error == mc_OKAY)
    {
        mc_Error_print(mc_OKAY, 1, "my_wrapper(): Internal error")
        status.error = mc_OKAY;
        return status;
    }

    /* Check if this error belongs to our my_wrapper function.
       If not, we have to indicate this, by returning a failed status */
    if (Owner_my_wrapper_new != signal.owner)
        return status;

    /* If this is our error, we have to deal with all the possible types */
    switch (signal.error)
    {
        case mc_OKAY:
            break;

        case mc_FAIL:
            mc_Error_print(mc_FAIL, 1, "my_wrapper(): Unhandled error");
            break;

        case mc_STD_MEMCPY:
            mc_Error_print(mc_STD_MEMCPY, 1,
                           "my_wrapper(): Cannot copy data to buffer");
            break;
    }

    /* After the error have been reported, we can inform
        the caller that we have owned this error */
    status.error = mc_OKAY;
    return status;
}


/* ..... THE DELETE WRAPPER'S CODE GOES HERE ..... */


int
main(void)
{
    /* Create an error signal object */
    mc_Error signal;

    /* Create data to be passed to our functions */
    char *s;
    int   i = 12;

    signal = my_wrapper_new(&s, &i);
    switch (signal.error)
    {
        case mc_OKAY;
            break;

        case mc_MEMCPY:
            s = "Default text";
            break;

        default:
            Try_my_wrapper(signal);
            mc_Error_print(mc_OKAY, 1, "main(): Something went wrong...");
            return EXIT_FAILURE;
    }

    /* Print what we have */
    printf("%s", s);

    /* Let's clean up the garbage */
    my_wrapper_del(&s);

    return EXIT_SUCCESS;
}
```



The index member
----------------

Because these examples were pretty simple, there was no need to use the third
member of the `mc_Error` object, the `index`. Think about a function, which has
several `malloc` calls in it. As we have a nice `mc_STD_MALLOC` error-type, we
can inform the caller, that one of the `malloc` havae failed. The question is,
when we are writing the try-function, how could we tell which call failed inside
the function?

This is where the `index` member comes in, as it ables us, to enumerate our
errors, which is sort of a position inside the function. Something like this:

```C
{
    mc_Error signal = {mc_FAIL, ..., 0};

    ...

    void *p;
    char *s;

    if (!p = malloc(SOME_SIZE))
    {
        signal.error = mc_STD_MALLOC;
        signal.index = 1;
        goto P_Alloc_Failed;
    }

    ...

    if (!s = malloc(sizeof "hello there"))
    {
        signal.error = mc_STD_MALLOC;
        signal.index = 2;
        goto S_Alloc_Failed;
    }

    ...

    S_Alloc_Failed:
        free(p);
    P_Alloc_Failed:
        return signal;
}
```

So inside our try-function we can do this:

```C
{
    ...

    switch (signal.error)
    {
        ...

        case mc_STD_MALLOC:
            switch (signal.index)
            {
                /* If p's allocation failed */
                case 1:
                    ...
                    break;

                /* If s' allocation failed */
                case 2:
                    ...
                    break;

                ...
            }
            break;

        ...
    }
```

The encapsulation, and reversed error propagation
-------------------------------------------------

- (encapsulation) All you have to worry about is the current function you are
  developing, and the calls to other functions inside the current one
- (reversed) The concept behind building a "fake" error-stack is, to deal with
  the error on the top-level and dig down and start printing from the lowest
  levels to build the stack.
