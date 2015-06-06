![MeaCulpa](logo.png?raw=true "logo")

  - [What is `MeaCulpa`?](#what-is-meaculpa)
  - [Why another solution?](#why-another-solution)
  - [Are there any cons?](#are-there-any-cons)
  - [How can I try it?](#how-can-i-try-it)
  - [How should I use it?](#how-should-i-use-it)
  - [First example](#first-example)
  - [Second example](#second-example)
  - [The index member](#the-index-member)
  - [Custom error types](#custom-error-types)
  - [Summary](#summary)
  - [Predefined error types](#predefined-error-types)
  - [Public macros](#public-macros)
  - [Public types](#public-types)
  - [Public functions](#public-functions)



What is `MeaCulpa`?
-------------------

A small and minimalistic error-handling library written in C. The main goal was
to fulfill the following requirements and create a(n):

  - *(almost)* zero-overhead,
  - stack-based *(no dynamic allocations)*,
  - fully backtraced,
  - meaningful error-message generator,
  - thread-safe *(locally handled)*,
  - type-safe,
  - extensible, flexible and modular,
  - truly encapsulated *(higher level of abstraction)*,
  - maintainable,

error-handling library, which can be used in any C related projects. *(But of
course in the first place it was made to be used by all my personal projects.)*



Why another solution?
---------------------

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



Are there any cons?
-------------------

Well, there is no such thing as free lunch, so of course there are down sides.

The first drawback of using the library is that you have to be a bit more
verbose than usual, because you have to write the so called "try-function"
counterpart of a function. It could be cumbersome, and it needs focus and
attention to the details but it is for the greater good. The code containing
try-functions will be a pleasant experience to use for others and for the future
youself as well.

The other drawback is the very small processing-overhead as a side effect of
some extra checkings (for example the error-object's owner matching, which will
most likely run more than once, until one of the try-functions finally find a
match). Actually this is the only overhead `MeaCulpa` has compared to the
traditional return value checking.

Another weakness of this library can be the effect of the size of the program
code. It won't really effect the processing-speed, but it will effect the size
of the executable as it will be bigger.

In my opinion these issues are neglectable compared to the pros this approach
brings to the quality of the software.



How can I try it?
-----------------

First open a terminal, and make sure you have [`git`](https://git-scm.com) and
[`SCons`](http://scons.org) installed.

  - on Arch Linux:

  ```
  $ sudo pacman -S git scons
  ```

  - on Ubuntu:

  ```
  $ sudo apt-get install git scons
  ```

  - on Mac OS X *(You should consider using [`homebrew`](http://brew.sh))*:

  ```
  $ brew install git scons
  ```

Now you have to navigate to your downloads folder, and do the following:

```
$ cd downloads
$ git clone https://github.com/petervaro/MeaCulpa.git
$ cd MeaCulpa
$ sudo scons install
```

After the compilation and installation succeded, you should run the tests as
well, to make sure eveyrthing is working:

```
$ scons tests
```

If tests are running, you can start writing your own code using `MeaCulpa`. All
you have to do is to include the header:

```C
#include <MeaCulpa.h>
```

And pass the following options to your compiler command:

```
-I/usr/local/include -L/usr/local/lib -lMeaCulpa
```


How should I use it?
--------------------

*(NOTE: All the public types, functions and macros are using pseudo namespace,
the `mc_` prefix before their names.)*

First of all, `MeaCulpa` has an error type which is a struct. This is the type
that will be the return value of all functions that propagate their
missbehaviours. The struct is defined as follows:

```C
typedef struct
{
    int      error; /* WHAT?  The error returned by the function */
    mc_FnPtr owner; /* WHO?   The function returned the error */
    int      index; /* WHERE? The location of the error in the function */
} mc_Error;
```

The first two members are mandatory but the last one, `index`, can be omitted in
most of the cases. *(It will be discussed later on.)*

The first member will be the type of the error, which specifies what went wrong,
or indicates if everything was fine. `MeaCulpa` provides predefined errors, but
the user can define custom ones as well -- that's one of the reasons why the
`error` member has such a "broad" type as an `int` instead of the predefined
error-types' type the `mc_ErrorType`. Which type looks like this:

```C
typedef enum
{
    /* Generic errors */
    mc_Okay,
    mc_Fail,

    /* Implementation based errors */
    mc_Depricated,
    mc_Experimental,

    /* Standard based errors */
    mc_StdMalloc,
    mc_StdCalloc,
    mc_StdRealloc,

    /* Memory based errors */
    mc_Memory,

    /* File I/O based errors */
    mc_EOF,

    /* Mathematics based errors */
    mc_ZeroDiv,

    /* Function argument based errors */
    mc_NullPtr,
    mc_Value,

    /* Containers based errors */
    mc_Empty,
    mc_Index,
    mc_Key,
    ...
} mc_ErrorType;
```

*(More detailed informations about the meanings and conventional usages of these
error-types will be discussed later on.)*

To be able to identify the returned errors by the try-functions even long after
they actually returned, we have to specify the second member, the `owner`. Since
this identifier has to be unique among all the identifiers during the whole
lifetime of the program to be able to build an error-stack like behaviour, we
will use the functions themselves to identify them.

Unfortunately -- according to the standard -- pointers to functions are not
*ordinary* pointers, so they cannot be stored in a simple `void*`. That's why
`MeaCulpa` has a special type `mc_FnPtr`, which is capable of storing any kind
of function pointers.

*(NOTE: Very important, one should never call the stored function directly!)*

There is a convenient macro, which helps to cast the function to become a
storable `owner` inside the error object:

```C
mc_Error
my_beautiful_function(void)
{
    mc_Error signal = {mc_Okay, mc_OWNER(my_beautiful_function), 0};
    ...
}
```

Of course, there is an even more abstract function-like macro to create an error
object and set the values for us:

```C
mc_Error
my_beautiful_function(void)
{
    mc_Error signal = mc_ERROR(mc_Okay, my_beautiful_function);
    ...
}
```

`mc_ERROR` can even handle if the last, optional argument, the `index` is
omitted. *(It will set it to the default `0`.)*

We already talked about try-functions, which are needed to be implemented. These
functions will do the owner-matching and the error-message printing, and they
are responsible for the encapsulation/abstraction part of `MeaCulpa`.

To produce prettified, properly printed error messages inside a try-function we
will use `MeaCulpa`'s own printer function, which declared as follows:

```C
void
mc_print(const mc_ErrorType       error,
         const char *const        function,
         const size_t             line_count,
         const char *const *const lines);
```

The function takes an error (which is the `mc_Error.error`); the name of the
function, which returned the error; the number of extra lines (hints) the user
wants to print; and the lines themselves to provide more accurate informations
about the error.

There is a special circumstance though, when we want to print the extra hints
for error message, but as part of the backtrace sequence. In that case, we have
to pass `mc_Okay` as the `error` argument to this function, so it won't start a
new "stack", but instead "appends" the passed messages to the existing one.

Here is a sample output, how the error messages would look like:

```
An error occured:
  In function: my_internal_function
    mc_NullPtr: Argument is a pointer to NULL
    (Hint: 1st argument is NULL)
  In function: my_function
    (Hint: Internal error)
  In function: main
    (Hint: my_function failed)
```



First example
-------------

Let's take a look at our error-handling solution through practical examples.
For this we will "use" an imaginary 3rd-party library which also uses
`MeaCulpa`. The public type and all its methods from this library are the
followings:

```C
/*----------------------------------------------------------------------------*/
/* Base type */
typedef struct
{
    size_t size;
    char  *chars;
} RandChars;

/*----------------------------------------------------------------------------*/
/* Allocation and initialisation.
   Returns: mc_Okay, mc_Fail, mc_StdMalloc, mc_NullPtr */
mc_Error
RandChars_new(RandChars **const self,
              size_t            count);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Try-function of the new method */
mc_Error
RandChars_new_TRY(mc_Error signal);

/*----------------------------------------------------------------------------*/
/* Generates random characters
   Returns: mc_Okay, mc_Fail, mc_NullPtr:0, mc_NullPtr:1, mc_Memory */
mc_Error
RandChars_gen(RandChars  *const self,
              char      **const buffer);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Try-function of gen method */
mc_Error
RandChars_gen_TRY(mc_Error signal)

/*----------------------------------------------------------------------------*/
/* Reallocation if necessary.
   Returns: mc_Okay, mc_Fail, mc_StdRealloc, mc_NullPtr */
mc_Error
RandChars_grow(RandChars *self,
               size_t     size);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Try-function of grow method */
mc_Error
RandChars_grow_TRY(mc_Error signal);

/*----------------------------------------------------------------------------*/
/* Deallocation */
void
RandChars_del(RandChars **const self);
```

Conventionally all functions, which are returning `mc_Error` can and should
return `mc_Okay` indicating that there was no error and `mc_Fail` which means an
unhandled error happened during the call of the function. Besides the
conventional error-types, let's see the different other errors that these
functions can return.

`mc_StdMalloc` is returned by `RandChars_new` when the `malloc` function
declared in `stdlib.h` returned a `NULL` pointer.

`mc_StdRealloc` is returned by `RandChars_grow` when the `realloc` function
declared in `stdlib.h` returned a `NULL` pointer.

`mc_NullPtr` is returned by `RandChars_new`, `RandChars_gen` and
`RandChars_grow`, when their first argument `self` is `NULL`, or by
`RandChars_gen`, when its second argument `buffer` is `NULL`.

`mc_Memory` is returned by `RandChars_gen`, when the underlying buffer of a
`RandChars` object has not enough size to store the generated `char`s.

Here is our very simple example, of how we may want to use this function and
its try-function:

```C
#include <stdio.h> /*
    func  : printf
*/
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/

#include <RandChars.h> /*
    func  : RandChars_new
            RandChars_new_TRY
            RandChars_gen
            RandChars_gen_TRY
            RandChars_del
*/

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
```

As you can see, here we were very strict, any error occures in `RandChars_new`
(`mc_Fail`, `mc_StdMalloc` and `mc_NullPtr`) or in `RandChars_gen` (`mc_Fail`,
`mc_NullPtr` or `mc_Memory`) we want to interrupt our program. Therefore if the
first argument of `RandChars_gen` was `NULL` for some reason, the above code
would give us the following error messages and quits:

```
An error occured:
  In function: RandChars_gen
    mc_NullPtr: Argument is a pointer to NULL
    (Hint: 1st argument `self` is NULL)
  In function: main
    (Hint: Cannot generate RandChars characters)
```

Also notice, that we used a macro to check if the signal is `mc_Okay` or not.
Actually there are two of these convenient macros:

```C
#define mc_IS_OKAY(E)  ((E).error == mc_Okay)
#define mc_NOT_OKAY(E) ((E).error != mc_Okay)
```

Now, let's distinguish between the possible errors and let's try to recover from
one of them:

```C
#include <stdio.h> /*
    func  : printf
*/
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/

#include <RandChars.h> /*
    func  : RandChars_new
            RandChars_new_TRY
            RandChars_gen
            RandChars_gen_TRY
            RandChars_grow
            RandChars_grow_TRY
            RandChars_del
*/

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
```

Notice, that we did not check the return values of the try-functions. That's
only because this was such a simple example. But if we use these functions
inside other functions, we have to be more careful than that!



Second example
--------------

In this example, we will wrap the previously used `RandChars_new` and
`RandChars_gen` functions, and we will write our first try-functions to do so:

```C
#include <stdio.h> /*
    func  : printf
*/
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/

#include <RandChars.h> /*
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

Hopefully you have noticed, that the wrapper functions are using two separate
`mc_Error` objects: `others` and `signal`.

Also it is very important, that the try-functions has their own `mc_Error`
objects as well (`status`) which is indicates if these functions have matched
the error's owner or not.



The index member
----------------

You may have noticed, that the comment above the `RandChars_gen` defines two
errors of the same type:

```C
/* Generates random characters
   Returns: mc_Okay, mc_Fail, mc_NullPtr:0, mc_NullPtr:1 */
mc_Error
RandChars_gen(RandChars  *const self,
              char      **const buffer);
```

That's because the function takes two pointers as arguments, so it can propagate
different errors based on which one is not the value the function expected. And
it can do so, even if the two errors have the same type! Which is very useful,
as you could have seen, because we can ignore these extra informations, and only
act based on the error-type alone. (Which we did in the previous example.) This
kind of enumeration of errors makes it possible to print the following error
messages when the `self` argument is `NULL`:

```
An error occured:
  In function: RandChars_gen
    mc_NullPtr: Argument is a pointer to NULL
    (Hint: 1st argument `self` is NULL)
  In function: main
    (Hint: Cannot generate RandChars characters)
```

and prints these ones, when `buffer` is `NULL`:

```
An error occured:
  In function: RandChars_gen
    mc_NullPtr: Argument is a pointer to NULL
    (Hint: 2nd argument `buffer` is NULL)
  In function: main
    (Hint: Cannot generate RandChars characters)
```

So you can think of indices of errors as the positions of errors inside a
function. Here is the possible implementation of `RandChars_gen_TRY` function:


```C
mc_Error
RandChars_gen_TRY(mc_Error signal)
{
    static const char *const function = "RandChars_gen";
    static const char *const messages[] =
    {
        "1st argument `self` is NULL",
        "2nd argument `buffer` is NULL",
        "One of the arguments is NULL, but the error-index is invalid",
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

        /* mc_Fail or any other */
        default:
            mc_print(signal.error, function, 1, messages + 3);
            break;
    }

    return status;
}
```



Custom error types
------------------

As we mentioned before, you can easily extend the predefined error-types and
the error messages, if you need more fine tuned or custom errors inside your
project. All you need is to use `mc__Invalid__` to start your `enum` types and
the `mc_PRINT` macro to get formatted error messages.

```C
/* Include standard headers */
#include <stdlib.h> /*
    const : EXIT_SUCCESS
*/

/* Include MeaCulpa headers */
#include <MeaCulpa.h> /*
    enum  : mc_Okay
            mc_StdMalloc
            mc__Invalid__
    macro : mc_PRINT
            mc_print
*/

typedef enum
{
    MyError_X = mc__Invalid__,
    MyError_Y,
    MyError_Z,
    MyError__Invalid__,
} MyErrorType;

/* Error type names */
static const char *const MY_ERROR_TYPE_NAMES[] =
{
    [MyError_X] = "MyError_X",
    [MyError_Y] = "MyError_Y",
    [MyError_Z] = "MyError_Z",
};

/* Predefined error messages */
static const char *const MY_ERROR_MESSAGES[] =
{
    [MyError_X] = "X happened",
    [MyError_Y] = "Y happened",
    [MyError_Z] = "Z happened",
};

/* Custom printer */
void
MyError_print(const int                error,
              const char *const        function,
              const size_t             line_count,
              const char *const *const lines)
{
    if (error < mc__Invalid__)
    {
        mc_print(error, function, line_count, lines);
        return;
    }

    mc_PRINT(error,
             function,
             line_count,
             lines,
             "MyError_print",
             MyError_X,
             MyError__Invalid__,
             MY_ERROR_TYPE_NAMES,
             MY_ERROR_MESSAGES);
}

int
main(void)
{
    static const char *const messages[] =
    {
        "Oh, snap...",
        "Shoot!!!",
    };
    MyError_print(mc_StdMalloc, "main", 1, messages);
    MyError_print(MyError_X,    "main", 1, messages + 1);
    return EXIT_SUCCESS;
}
```

This will produce the following output:

```
An error occured:
  In function: main
    mc_StdMalloc: Memory allocation failed
    (Hint: Oh, snap...)

An error occured:
  In function: main
    MyError_X: X happened
    (Hint: Shoot!!!)
```



Summary
-------

As you can see, `MeaCulpa` is more of a "best-practice" which happens to have a
very small library to support the convenient use-cases. It can help you
encapsulate the error checking, so you only have to worry about the current
function and the functions you are calling inside it. As a result, it mimics
an error-stack like behaviour and produces beautiful, useful and proper
error-messages.



Predefined error types
----------------------

- `mc_Okay`: *(generic)*

    - It can indicate, that there were no error happened during the call of the
      function

    - It can indicate, (when it is returned by a try-function) that the
      ownership has been matched

    - It can also be used by passing it as the first argument to the `mc_print`
      function, indicating that the current error message is part of an error
      message sequence.

- `mc_Fail`: *(generic)*

    - It can indicate, that there was an unknown/unhandled error happened during
      the call of the function

    - It can indicate, (when it is returned by a try-function) that the
      ownership has mismatched

- `mc_Depricated`: *(implementation based)*

    - Indicates, that the function which returned this is depricated. (It is
      more like a warning)

- `mc_Experimental`: *(implementation based)*

    - Indicates, that the function which returned this is experimental. (It is
      more like a warning)

- `mc_StdMalloc`: *(standard based)*

    - Indicates, that the function `malloc` declared in `<stdlib.h>` in the C
      Standard returned `NULL`

- `mc_StdCalloc`: *(standard based)*

    - Indicates, that the function `calloc` declared in `<stdlib.h>` in the C
      Standard returned `NULL`

- `mc_StdRealloc`: *(standard based)*

    - Indicates, that the function `realloc` declared in `<stdlib.h>` in the C
      Standard returned `NULL`

- `mc_EOF`: *(file I/O based)*

    - Indicates, that the function reached the end of the file

- `mc_ZeroDiv`: *(math based)*

    - Indicates, that

- `mc_NullPtr`: *(function argument based)*

    - Indicates, that argument is pointing to `NULL`

- `mc_Value`: *(function argument based)*

    - Indicates, that argument has an invalid value

- `mc_Empty`: *(container based)*

    - Indicates, that the container is empty

- `mc_Index`: *(container based)*

    - Indicates, that index is out of range

- `mc_Key`: *(container based)*

    - Indicates, that key is not in the container



Public macros
-------------

- `mc_OWNER(function)`:

    - Casts the input `function` to an `mc_FnPtr` generic function pointer

- `mc_IS_OWNER(function, error)`:

    - Checks if `error.owner` is the `function`

- `mc_NOT_OWNER(function, error)`:

    - Checks if `error.owner` is not the `function`.

- `mc_ERROR(error, function [, index])`:

    - Creates a new `mc_Error` object. The `index` input is optional.

- `mc_OKAY(function [, index])`:

    - Creates a new `mc_Error` object, with `error` member set to `mc_Okay`. The
      `index` input is optional.

- `mc_FAIL(function [, index])`:

    - Creates a new `mc_Error` object, with `error` member set to `mc_Fail`. The
      `index` input is optional.

- `mc_IS_OKAY(error)`:

    - Checks if `error.error` is `mc_Okay`.

- `mc_NOT_OKAY(error)`:

    - Checks if `error.error` is not `mc_Okay`.

- `mc_PRINT(error, function, line_count, lines, PRINTER, MIN_VALID, MAX_VALID,
   TYPE_NAMES, MESSAGES)`:

    - Prints error messages. Don't use this macro alone, only inside a printer
      function (as macros are not type safe)
    - `error`, `function`, `lines_count` and `lines` are the arguments from the
      wrapper function (see `mc_print` for example)
    - PRINTER has to be a `char*` literal, `MIN_VALID` and `MAX_VALID` has to be
      `int`s, while `TYPE_NAMES` and `MESSAGES` has to be arrays of `char*`s



Public types
------------

- `mc_ErrorType`

- `mc_FnPtr`

- `mc_Error`



Public functions
----------------

```C
void
mc_print(const mc_ErrorType       error,
         const char *const        function,
         const size_t             line_count,
         const char *const *const lines);
```
