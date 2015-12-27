![meaculpa](img/logo.png?raw=true "logo")

- [What is `meaculpa`?](#what-is-meaculpa)
- [Why is it called `meaculpa`?](#why-is-it-called-meaculpa)
- [The rationale behind it](#the-rationale-behind-it)
- [The drawbacks](#the-drawbacks)
- [Dependencies](#dependencies)
- [Install utility](#install-utility)
- [End-user build and install](#end-user-build-and-install)
- [Developer build and install](#developer-build-and-install)
- [Uninstall](#uninstall)
- [Tutorials](#tutorials)
- [Documentation](#documentation)
- [License](#license)



What is `meaculpa`?
-------------------

`meaculpa` is a small, lightweight, simple, elegant, efficient, flexible,
extensible, type-safe, thread-safe, fully backtraced error handling and
reporting library and design pattern.

And it is capable of producing such beautiful outputs as the next one:

![colored-output](img/colored-output.png?raw=true "screenshot")



Why is it called `meaculpa`?
----------------------------

*Because `meaculpa` is all about errors!*

> Mea culpa is a Latin phrase that means "through my fault" and is an
> acknowledgement of having done wrong. [...]
>
> The expression is used also as an admission of having made a mistake that
> should have been avoided, and may be accompanied by beating the breast as in
> its use in a religious context.
>
> *&mdash; [Wikipedia](https://en.wikipedia.org/wiki/Mea_culpa)*



The rationale behind it
-----------------------

Error handling in C is explicit, which means, one has to check directly the
correctness of the returned value of a function. The returned value can be an
error signal or a mixture of valid values and error signals.

Eventhough this kind of explicitness is a very good thing at such a low-level as
C can work, it can make proper error-reporting a nightmare. How to tell exactly
where the error occured? How to handle some of the errors and recover from them,
while passing on the unrecoverable ones and at the same time printing only the
unhandled error messages out? Not to mention, that it would also be useful if
one could have a full backtraced message stack, when for example an error
occured in a deeply nested function call-chain!

It may sounds easy enough to implement an error-handling/reporting library as
that, but most of the existing solutions are reinventing the wheel, and they are
adding bloated and both process- and memory-expensive runtimes on top of the C
runtime. This kind of overhead is unacceptable, not to mention, that it is the
horror itself to work with them in a multithreaded environment.

`meaculpa` tries to solve the above mentioned problems, with an elegant and very
efficient way, which remains loyal to the philosophy of C &mdash; it is small,
still explicit and provides maximum control and flexibility to its users.



The drawbacks
-------------

Well, of course there is no such thing as free lunch, one has to pay for such
features. The real question is: how much should one pay?

With `meaculpa` the cost is very low. It returns `mc_Error`, which is an
unsigned integer type, capable of storing 2<sup>64</sup> - 1 values. That is, if
1 byte is 8 bits, than it is 8 bytes in the memory. In most 64-bit systems it is
the same size as a pointer. *(Note: the size of a pointer is not explicitly
defined in the standard, so this is just a rough approximation.)* So it will
hardly overflow on the stack &mdash; as returning a pointer won't do that either
&mdash; but the downside still remains: it is very likely that it will be larger
than a `char`, `bool`, `int` or `enum` typed values, which are very common error
signals in C.

The other place where one has to pay, is the mute-flags. To get full control
over the error riporting, one has to pass an `mc_Error` to a function which is
capable of returning an `mc_Error`. The cost here is not only the size of the
new argument, which is the same as in the previous paragraph, but it also means
the developer has to pass this value explicitly every time such function is
invoked. Of course this could be eliminated by defining variadic macros as
function wrappers, which allows the user to pass a flag by default if it is not
specified otherwise.

And the last costs are the extra `if` statements. One has to check wether an
error message should be printed or not. Although, this overhead can be removed
as well, as the main purpose of `meaculpa` is debugging, therefore a correct,
production-ready, and already tested/debugged code can remove the extra
checkings by placing `#ifdef`s at the right places.

All in all, the cost of `meaculpa` is very very small, especially compared to
other solutions &mdash; it is only a fraction of those!



Dependencies
------------

> **SUPPORTED PLATFORMS:** `meaculpa` is currently supported only on UNIX-like
> systems (Linux, BSD, OS X, etc.), but Windows support (via
> [mingw-w64](http://mingw-w64.org/doku.php)) is on its way!

<!-- -->
> **SUPPORTED STANDARDS:** `meaculpa` requires C99 or later (C11 recommended)

For end-users:

- [git](https://git-scm.com) *(2.6.4+)*
- [gcc](https://gcc.gnu.org) *(5.3.0+)* or
  [clang](http://clang.llvm.org) *(3.7.0+)*
- [ar](https://www.gnu.org/software/binutils) *(2.25.1)+*
- [bash](https://www.gnu.org/software/bash) *(4.3.42+)*
- [rainicorn](https://github.com/petervaro/rainicorn) *(0.1.2+)*

For developers:

- [git](https://git-scm.com) *(2.6.4+)*
- [gcc](https://gcc.gnu.org) *(5.3.0+)*
- [clang](http://clang.llvm.org) *(3.7.0+)*
- [ar](https://www.gnu.org/software/binutils) *(2.25.1)+*
- [bash](https://www.gnu.org/software/bash) *(4.3.42+)*
- [valgrind](http://valgrind.org) *(3.11.0+)*
- [clang-analyzer](http://clang-analyzer.llvm.org)  *(3.7.0+)*
- [tup](http://gittup.org/tup) *(0.7.3+)*
- [rainicorn](https://github.com/petervaro/rainicorn) *(0.1.2+)*



Install utility
---------------

The next three sections will demonstrate the default installation of `meaculpa`,
that is, using `gcc`, installing the headers at `/usr/local/include` and
installing libraries at `/usr/local/lib`. For further information and available
settings on how to use the included `install.sh` utility run:

```bash
bash install.sh --help
```



End-user build and install
--------------------------

```bash
$ git clone https://github.com/petervaro/meaculpa.git
$ cd meaculpa
$ bash install.sh
```



Developer build and install
---------------------------

```bash
$ git clone --recursive https://github.com/petervaro/meaculpa.git
$ cd meaculpa
$ bash tuplet/setup.sh
$ tup init
$ tup
$ bash install.sh
```


Uninstall
---------

```bash
$ bash install.sh --remove
```



Tutorials
---------

First include the necessary header files:

```C
#include <meaculpa/meaculpa.h>
```

The basic API design pattern of a function should be something like:

```
<error> <function-name> ( <inputs...>, <outputs...>, <mute-flags> )
```

that is, its return value will always be an error (`mc_Error` with `meaculpa`),
and all the outputs are going to be pointers. The last argument should be the
mute-flags, which will inform the functioin which errors should be printed and
which are not.

> **IMPORTANT:** The mute-flags won't change the return value of a function,
> they will only effect the output of the function's `mc_Error_put` calls.

So for example, the implementation of a divider function which divides two
`int`s, would look like this:

```C
mc_Error
divider(int       dividend,
        int       divisor,
        int      *quotient,
        mc_Error  muted)
{
    /* If the 'quotient' argument is NULL */
    if (!quotient)
    {
        /* If 'mc_ARG_IS_NULL' is not muted */
        if (~muted & mc_ARG_IS_NULL)
            /* Print the error message */
            mc_Error_put(mc_ARG_IS_NULL, -1,
                         "3rd argument 'int *quotient' is NULL");
        /* Return the error signal */
        return mc_ARG_IS_NULL;
    }

    /* If the 'divisor' argument is NULL */
    if (!divisor)
    {
        /* If 'mc_ZERO_DIVISION' is not muted */
        if (~muted & mc_ZERO_DIVISION)
            mc_Error_put(mc_ZERO_DIVISION, -1, "divisor is 0");
        /* Return the error signal */
        return mc_ZERO_DIVISION;
    }

    /* Set the output */
    *quotient = dividend/divisor;

    /* Indicate that everything went fine */
    return mc_OKAY;
}
```

The following things are happening:

1. The `divider` checks, if it can write to its output argument `quotient`, that
   is, the argument is not `NULL`.

2. If the argument `quotient` is `NULL`, then first the function checks wether
   the error it is going to return muted or not. This is the tricky part, as
   `meaculpa` is using
   [bit-masking](https://en.wikipedia.org/wiki/Mask_%28computing%29) technique
   for its error-handling/reporting. Therefore, first the function negates the
   argument `muted`, and then checks if `mc_ARG_IS_NULL` is in it. This
   expression will only be true, if `mc_ARG_IS_NULL` was not originally in the
   `muted` argument. Therefore if it was not in it, the function can print out
   the error message. After this the function will return the `mc_ARG_IS_NULL`
   wether the error was muted or not.

3. The `divider` checks if the argument `divisor` is `0` or not.

4. If the argument `divisor` is `0`, the function will check wether it should
   print an error message for `mc_ZERO_DIVISION` or not. It does the same way as
   it did before in the 2. point.  If the error is not muted it will print the
   error message, and then return the error signal itself.

5. If both arguments `quotient` and `divisor` are valid, the function will
   divide the argument `dividend` with the argument `divisor` and will write the
   result to the memory where the argument `quotient` is pointing to.

6. Finally it will return `mc_OKAY` indicating that everything went fine, it
   calculated what it had to, there were no errors occured.

The invocation of such function as the `divider` would look like this:

```C
int      result;
mc_Error error;

if ((error = divide(8, 2, &result, mc_MUTE_NONE)))
    mc_Error_put(error, 1, "Failed to divide 8 by 2");
else
    printf("The result is: %d\n", &result);
```

At the invocation the following things are happening:

1. The `divide` function is called, and the mute-flags is set to `mc_MUTE_NONE`
   which means, the function will print messages for all errors that can occure
   in it.

2. If everything goes fine, the return value of `divider` will be `mc_OKAY` the
   boolean value of which is always `false`. So first it is checked, that any
   errors occured during the call of `divider`.

3. If so, then an error message will be printed, explaining what exactly went
   wrong. This message will be part of the backtrace.

4. If not, the result of the division will be printed out.

> **NOTE:** For more examples, look at the [`samples` folder](samples)!



Documentation
-------------

**Types:**

- `mc_Error` is guaranteed to be a signed integer type that is capable of
  storing 2<sup>64</sup> - 1 values. This is the type of all errors, markers and
  mute-flags.

**Error mute-flag wildcard constants:**

- `mc_MUTE_NONE` allows all errors of a function to be printed out
- `mc_MUTE_ALL` disallows all errors of a function to be printed out

**Error constants:**

- `mc_OKAY` indicates that there were no errors occured during the call of a
  function

- `mc_FAIL` is a generic error, which should be used only in very special cases,
  where the function wants to indicate to its caller, that something unexpected
  happened

- `mc_UNKNOW_ERROR` should be used only in very special cases, when a function
  wants to indicate to its caller, that the returned value of an internal call
  cannot be recognized (maybe it is a badly used bare marker, or an error which
  is not defined)

- `mc_DEPRECATED` should be used, when the called function is deprecated. The
  deprecated function should return this value instead of `mc_OKAY`, and should
  never return `mc_OKAY`. If other errors occured it should return those errors
  instead

- `mc_EXPERIMENTAL` should be used, when the called function is experimental and
  can change its behaviour the future without any further notice. The
  experimental function should return this value instead of `mc_OKAY`, and
  should never return `mc_OKAY`. If other errors occured it should return those
  errors instead

- `mc_ARG_IS_NULL` should be returned, if an argument which happens to be a
  pointer is `NULL`, and that is not acceptable by the function

- `mc_INVALID_VALUE` should be used, when the passed argument cannot be used by
  the function

- `mc_INVALID_INDEX` should be used, when the function should use the passed
  argument as an index to a container/collection, and that index is out of range

- `mc_INVALID_KEY` should be used, when the function should use the passed
  argument as a key to container/collection, and that key is not in it

- `mc_STOP_ITERATION` should be used, when the function is performing an
  iteration on a complex type (container/collection) and there are no more
  values left in it

- `mc_ALLOC_FAIL` should be used, when the function should dynamically allocate
  memory, but that allocation failed

- `mc_INI_FAIL` should be used, when the initializer method failed

- `mc_FIN_FAIL` should be used, when the finalizer method failed

- `mc_FILE_ACCESS`

- `mc_FILE_NOT_FOUND` should be used, when the function is looking for a
  specific file, but it is noy available

- `mc_PERMISSION_FAIL`

- `mc_END_OF_FILE` should be used, when the function reaches `EOF`

- `mc_END_OF_LINE` should be used, when the function reaches `EOL`

- `mc_ZERO_DIVISION` should be used, when the function should should perform a
  division, but the divisor is `0`

**Marker constants:**

Each of the above listed errors (except `mc_OKAY`) can be *marked*. Marking is
only useful, if an error can be returned more than 1 different places from
a function, as it makes it easy to seperate between those places. It is
especially useful, if the caller can recover from at least one of them.

The notation is simple, it uses macros to overload the constant values:

```C
mc_ARG_IS_NULL(n)
```

where `n` can be `1..16`.

> **NOTE:** If a function can return more than one marked errors, then different
> markers should be used for each errors, otherwise it would be impossible to
> distinguish between the errors. For example: if a function can return
> `mc_ARG_IS_NULL` errors from 2 different places and `mc_INVALID_VALUE` errors
> from 3 different ones, then `mc_ARG_IS_NULL(1)`, `mc_ARG_IS_NULL(2)`,
> `mc_INVALID_VALUE(3)`, `mc_INVALID_VALUE(4)` and `mc_INVALID_VALUE(5)` should
> be used.

There are two implicit advantages of the markers: one can mute all the marked
errors of the same base, by simply passing or `|`-ing the error without any
markera as/to the mute-flags, and one can handle all marked errors of the same
base, without the need to explicitly check every marked version of it.

**`mc_Error` representation functions:**

```C
const char*
mc_Error_str(mc_Error error);
```
The function will return the string representation of an error as a `NULL`
terminated C-string. It does not care wether the error is marked or not, it will
return the unmarked representation of the error.
For example:

- for `mc_DEPRECATED` it will return `"mc_DEPRECATED"`
- for `mc_EXPERIMENTAL(12)` it will return `"mc_EXPERIMENTAL"`

```C
void
mc_Error_put(mc_Error error,
             int      messages,
          /* const char* */ ...);
```
This is a function-like macro wrapper around `mc_Error__put`, which should not
be called directly (as there is no real advantage there). It takes an error
constants, the number of messages, and the messages themselves. The messages
should be `NULL`-terminated C-strings. The messages count can be both negative
and positive (its absolute value tells how many strings have been passed). The
former indicates that the error is occured where this function is called, while
the latter means the error happened elsewhere and the current call is part of
the traceback calls. This indication will influence how the printed strings are
formatted.


**Stream management functions:**

```C
void
mc_stream_ini(void);
```
Initializes the stream-access mutex and the stream itself. It has to be called
before anything (most likely in the beginning of the `main` function) if: 1. the
program is running multiple threads and within those threads functions are using
`mc_Error_put` and/or `mc_stream_set`; or 2. the program wants to change the
default stream via `mc_stream_set`, thread count does not matter. If none of the
previously listed conditions are met, it is unnecessary to call this function,
though it is highly recommended for easier code maintainability in the future.

```C
void
mc_stream_fin(void);
```
Finalizes the stream-access mutex and the stream itself. It has to be called
after anything (most likely at the end of the `main` function) if
`mc_stream_ini` has been used before.

```C
mc_Error
mc_stream_set(FILE     *stream,
              mc_Error  muted);
```
Specifies the stream where `mc_Error_put` will print the error messages and the
traceback. This can be very useful, when the program should log these into an
external file, instead of printing them to the `stderr` stream for example. This
function can return:

- `mc_OKAY` if there was no error
- `mc_ARG_IS_NULL` if `FILE *stream` is `NULL`



License
-------

Copyright &copy; 2015-2016 **Peter Varo**

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program, most likely a file in the root directory, called 'LICENSE'.
If not, see <http://www.gnu.org/licenses>.
