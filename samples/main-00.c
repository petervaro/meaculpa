/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include <stdio.h>
/*  type  : FILE
    value : stderr
    func  : fputs
            fopen
            fclose */
#include <stdlib.h>
/*  const : EXIT_FAILURE
            EXIT_SUCCESS */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "meaculpa/meaculpa.h"
/*  type  : mc_Error
    const : mc_MUTE_NONE
            mc_OKAY
            mc_DEPRECATED
    macro : mc_Error_put */

/*----------------------------------------------------------------------------*/
static inline mc_Error
core(mc_Error muted);

/*----------------------------------------------------------------------------*/
static inline mc_Error
middle_man(mc_Error muted);

/*----------------------------------------------------------------------------*/
static inline mc_Error
wrapper(mc_Error muted);


/*----------------------------------------------------------------------------*/
int
main(int          argc,
     const char **argv)
{
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

    if ((error = wrapper(mc_MUTE_NONE)))
        mc_Error_put(error, 1, "Unexpected error in 'wrapper'");

    mc_stream_set(stream, mc_MUTE_NONE);

    if ((error = wrapper(mc_MUTE_NONE)))
        mc_Error_put(error, 1, "Unexpected error in 'wrapper'");

    mc_stream_fin();
    fclose(stream);
    return EXIT_SUCCESS;
}


/*----------------------------------------------------------------------------*/
static inline mc_Error
wrapper(mc_Error muted)
{
    mc_Error error;
    if ((error = middle_man(mc_MUTE_NONE)))
    {
        if (~muted & error)
            mc_Error_put(error, 0);
        return error;
    }
    return mc_OKAY;
}


/*----------------------------------------------------------------------------*/
static inline mc_Error
middle_man(mc_Error muted)
{
    mc_Error error;
    if ((error = core(mc_MUTE_NONE)))
    {
        if (~muted & error)
            mc_Error_put(error, 1, "Cannot create X because 'core' failed");
        return error;
    }
    return mc_OKAY;
}


/*----------------------------------------------------------------------------*/
static inline mc_Error
core(mc_Error muted)
{
    if (~muted & mc_DEPRECATED)
        mc_Error_put(mc_DEPRECATED, -1, "This function is deprecated");
    return mc_DEPRECATED;
}
