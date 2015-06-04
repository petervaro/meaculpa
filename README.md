![MeaCulpa](logo.svg?raw=true "logo")

  - [What is `MeaCulpa`?](#what-is-meaculpa)
  - [Why another solution?](#why-another-solution)
  - [Are there any cons?](#are-there-any-cons)
  - [How can I try it?](#how-can-i-try-it)
  - [How should I use it?](#how-should-i-use-it)
  - [First example](#first-example)
  - [Second example](#second-example)
  - [The index member](#the-index-member)
  - [Predefined error types](#predefined-error-types)



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
`MeaCulpa` has a special type `mc_FnPtr`, which is capable of string any kind of
function pointers.

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

`mc_Error` can even handle if the last, optional argument, the `index` is
omitted. *(It will set it to the default `0`.)*

We already talked about try-functions, which are needed to be implemented. These
functions will do the owner-matching and the error-message printing, and they
are responsible for the encapsulation/abstraction part of `MeaCulpa`.

To produce prettified, properly printed error messages inside a try-function we
will use `MeaCulpa`'s own printer function, which declared as follows:

```C
void
mc_print(const int                error,
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

Let's take a look at practical usages of our error-handling solution through
examples. For this we will "use" an imaginary 3rd-party library which also uses
`MeaCulpa`. The public type and all its methods from this library are the
followings:

```C
/* Base type */
typedef struct
{
    char *chars;
} RandChars;

/* Allocation and initialisation.
   Returns: mc_Okay, mc_Fail, mc_StdMalloc, mc_NullPtr */
mc_Error
RandChars_new(RandChars **const self,
              size_t            count);

/* Try-function of the new method */
mc_Error
RandChars_new_TRY(mc_Error signal);

/* Generates random characters */
char *
RandChars_gen(RandChars *const self);

/* Deallocation */
void
RandChars_del(RandChars **const self);
```

Conventionally, this function can return `mc_Okay` indicating that there was no
error; and `mc_Fail` which means an unhandled error happened during the call of
this function. Besides the conventional return values, it can also return
`mc_StdMalloc` which means, that the `malloc` function declared in `stdlib.h`
returned a `NULL` pointer; and it can also return `mc_NullPtr` which means,
that the first argument `self` is `NULL`.

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
            RandChars_del
*/

int
main(void)
{
    /* Error object and error messages */
    static const char *const messages[] = {"Cannot create new RandChars"};
    mc_Error signal;

    /* Create RandChars object */
    RandChars *rc;
    signal = RandChars_new(&rc, 5);
    /* If there was an error */
    if (mc_NOT_OKAY(signal))
    {
        /* Clean-up, print error messages and return */
        RandChars_del(&rc);
        RandChars_new_TRY(signal);
        mc_print(mc_Okay, "main", 1, messages);
        return EXIT_FAILURE;
    }

    /* If there was no error, generate random string and print it */
    printf("%s\n", RandChars_gen(rc));

    /* Clean up and return */
    RandChars_del(&rc);
    return EXIT_SUCCESS;
}
```

As you can see, here we were very strict, any error occures (`mc_Fail`,
`mc_StdMalloc` and `mc_NullPtr`) we want to interrupt our program. If the first
argument of `RandChar_new` was `NULL`, the above code would give us the
following error messages:

```
An error occured:
  In function: RandChars_new
    mc_NullPtr: Argument is a pointer to NULL
    (Hint: 1st argument `self` is NULL)
  In function: main
    (Hint: Cannot create new RandChar)
```

Now, let's distinguish between the errors:

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
            RandChars_del
*/

int
main(void)
{
    /* Error object and error messages */
    static const char *const messages[] = {"Cannot create new RandChars"};
    mc_Error signal;
    RandChars *rc;
    char *s;

    /* Create RandChars object */
    signal = RandChars_new(&rc, 5);
    /* If there was an error */
    if (mc_NOT_OKAY(signal))
        switch (signal.error)
        {
            /* We chose to deal with only the NULL pointer argument problem,
               so we assign a default value to our string */
            case mc_NullPtr:
                s = "abcde";
                break;

            default:
                RandChars_new_TRY(signal);
                mc_print(mc_Okay, "main", 1, messages);
                return EXIT_FAILURE;
        }
    /* If there was no error, generate random string */
    else
        s = RandChars_gen(rc);

    /* Print the string we have */
    printf("%s\n", s);

    /* Clean up and return */
    RandChars_del(&rc);
    return EXIT_SUCCESS;
}
```

Notice, that we did not check the return value of the try-function. That's only
because this was such a simple example. But if we use this function inside
another function, we have to be more careful than that!



Second example
--------------

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

Predefined error types
----------------------

...
