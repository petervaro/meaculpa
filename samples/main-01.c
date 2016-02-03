/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  type  : FILE */
#include <stddef.h>
/*  type  : size_t */
#include <stdlib.h>
/*  const : EXIT_SUCCESS
            EXIT_FAILURE */
#include <string.h>
/*  func  : strlen
            memcpy */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include "meaculpa/meaculpa.h"
/*  type  : mc_Error
    const : mc_MUTE_NONE
            mc_MUTE_ALL
            mc_OKAY
            mc_FILE_ACCESS */

/*  This example demonstrates how the error markers should be used, and how to
    remark them. The example functions are not working in the sense, that they
    are missing their true functionalities -- this example focuses on the error
    handling and reporting aspects only.

    The first function called 'open_file' is pretty simple: in theory it takes a
    file and opens it in the defined mode -- basically it could be imagined as a
    wrapper around the fopen function from the standard library. The open_file
    is not really implemented, in this example it can only return the error
    mc_FILE_ACCESS.

    The second function, the 'duplicate_backups' is a bit trickier because this
    is the first time the error markers are used. In theory the function opens a
    file, reads its content and opens another file, and writes the content of
    the first file to the second one. It uses the 'open_file' function twice.
    (For the sake of the example, 'duplicate_backups' does not care about
    closing the previously opened file(s).) Because 'open_file' can return
    mc_FILE_ACCESS on error, the caller of 'duplicate_backups' won't be able to
    distinguish between the two scenarios (1. the error occured while tried to
    open the original file, or 2. the error occured while tried to open the
    duplicate file) if it would simply return the error of 'open_file'. Because
    of that, the function will "translate" the returned errors to marked ones:
    mc_FILE_ACCESS(1) and mc_FILE_ACCESS(2).

    The third function, the 'manage_backups' is the most complex one, as it
    calls 'duplicate_backups' twice. For the same reason as before, it has to
    translate the returned error into marked ones, in this case:
    mc_FILE_ACCESS(1), mc_FILE_ACCESS(2), mc_FILE_ACCESS(3) and
    mc_FILE_ACCESS(4), so the caller can decide what to do on each scenario. */


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
typedef enum
{
    FileMode_READ,
    FileMode_WRITE,
} FileMode;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline mc_Error
open_file(const char  *restrict name,
          FILE       **restrict file,
          FileMode              mode,
          mc_Error              muted);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline mc_Error
duplicate_backups(const char *name,
                  mc_Error    muted);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
static inline mc_Error
manage_backups(const char *name,
               mc_Error    muted);



/*----------------------------------------------------------------------------*/
int
main(int          argc,
     const char **argv)
{
    #ifdef MUTE_ALL
        #define MUTED mc_MUTE_ALL
    #else
        #define MUTED mc_MUTE_NONE
    #endif

    mc_Error  error;
    FILE     *stream;

    if (argc <= 1)
    {
        fputs("Missing first argument: output-stream\n", stderr);
        return EXIT_FAILURE;
    }

    if (!(stream = fopen(argv[1], "wb")))
    {
        fputs("Cannot open 'stream'\n", stderr);
        return EXIT_FAILURE;
    }

    mc_stream_ini();

    if ((error = manage_backups("example", MUTED)))
        if (~MUTED & error)
            mc_Error_put(error, 1, "Failed to manage backups");

    mc_stream_set(stream, mc_MUTE_NONE);

    if ((error = manage_backups("example", MUTED)))
        if (~MUTED & error)
            mc_Error_put(error, 1, "Failed to manage backups");

    mc_stream_fin();
    fclose(stream);
    return EXIT_SUCCESS;
}



/*----------------------------------------------------------------------------*/
/* Return : mc_OKAY
            mc_FILE_ACCESS(1) => log (original)
            mc_FILE_ACCESS(2) => log (duplicate)
            mc_FILE_ACCESS(3) => data (original)
            mc_FILE_ACCESS(4) => data (duplicate) */
static inline mc_Error
manage_backups(const char *name,
               mc_Error    muted)
{
    mc_Error    error,
                silenced;
    char        name_log[128],
                name_data[128];
    size_t      length = strlen(name);

    #define SUFFIX "-log"
    memcpy(name_log, name, length);
    memcpy(name_log + length, SUFFIX, sizeof SUFFIX);
    name_log[length + sizeof SUFFIX] = '\0';
    #undef SUFFIX

    silenced = mc_MUTE_NONE;
    silenced |= muted & mc_FILE_ACCESS(1);
    silenced |= muted & mc_FILE_ACCESS(2);
    if ((error = duplicate_backups(name_log, silenced)))
    {
        if (error == mc_FILE_ACCESS(1))
        {
            if (~silenced & error)
                mc_Error_put(error, 1, "Cannot read content of the log file");
            return error;
        }
        else if (error == mc_FILE_ACCESS(2))
        {
            if (~silenced & error)
                mc_Error_put(error, 1, "Cannot write to the log file backup");
            return error;
        }
    }

    #define SUFFIX "-data"
    memcpy(name_data, name, length);
    memcpy(name_data + length, SUFFIX, sizeof SUFFIX);
    name_data[length + sizeof SUFFIX] = '\0';
    #undef SUFFIX

    silenced = mc_MUTE_NONE;
    if (muted & mc_FILE_ACCESS(3))
        silenced |= mc_FILE_ACCESS(1);
    if (muted & mc_FILE_ACCESS(4))
        silenced |= mc_FILE_ACCESS(2);

    if ((error = duplicate_backups(name_data, silenced)))
    {
        if (error == mc_FILE_ACCESS(1))
        {
            if (~silenced & mc_FILE_ACCESS(3))
                mc_Error_put(error, 1, "Cannot read content of the data file");
            return mc_FILE_ACCESS(3);
        }
        else if (error == mc_FILE_ACCESS(2))
        {
            if (~silenced & mc_FILE_ACCESS(4))
                mc_Error_put(error, 1, "Cannot write to the data file backup");
            return mc_FILE_ACCESS(4);
        }
    }

    return mc_OKAY;
}



/*----------------------------------------------------------------------------*/
/* Return : mc_OKAY
            mc_FILE_ACCESS(1) => read original
            mc_FILE_ACCESS(2) => write duplicate */
static inline mc_Error
duplicate_backups(const char *name,
                  mc_Error    muted)
{
    mc_Error    error;
    FILE       *file_orig,
               *file_copy;
    char        name_orig[128],
                name_copy[128];
    size_t      length = strlen(name);

    #define SUFFIX "-orig"
    memcpy(name_orig, name, length);
    memcpy(name_orig + length, SUFFIX, sizeof SUFFIX);
    name_orig[length + sizeof SUFFIX] = '\0';
    #undef SUFFIX

    if ((error = open_file(name_orig, &file_orig, FileMode_READ, muted)))
    {
        if (error == mc_FILE_ACCESS)
        {
            if (~muted & mc_FILE_ACCESS(1))
                mc_Error_put(error, 1, "Cannot open original file");
            return mc_FILE_ACCESS(1);
        }
    }

    #define SUFFIX "-copy"
    memcpy(name_copy, name, length);
    memcpy(name_copy + length, SUFFIX, sizeof SUFFIX);
    name_copy[length + sizeof SUFFIX] = '\0';
    #undef SUFFIX

    if ((error = open_file(name_copy, &file_copy, FileMode_WRITE, muted)))
    {
        if (error == mc_FILE_ACCESS)
        {
            if (~muted & mc_FILE_ACCESS(2))
                mc_Error_put(error, 1, "Cannot open duplicate file");
            return mc_FILE_ACCESS(2);
        }
    }

    return mc_OKAY;
}



/*----------------------------------------------------------------------------*/
/* Return : mc_OKAY
            mc_FILE_ACCESS */
static inline mc_Error
open_file(const char  *restrict name,
          FILE       **restrict file,
          FileMode              mode,
          mc_Error              muted)
{
    (void)file;
    (void)mode;

    if (~muted & mc_FILE_ACCESS)
        mc_Error_put(mc_FILE_ACCESS, -3, "Cannot access file: '", name, "'");
    return mc_FILE_ACCESS;
}
