/* INFO **
** INFO */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include standard headers */
#include <stdio.h>
/*  const : stderr
    func  : fputs */
#include <stdlib.h>
/*  const : EXIT_SUCCESS
            EXIT_FAILURE */
#include <errno.h>
/*  data  : errno */
#include <string.h>
/*  func  : strerror */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include rainicorn headers */
#include <rainicorn/rainicorn.h>

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Include meaculpa headers */
#include <meaculpa/meaculpa.h>
/*  type  : mc_Error
            mc_Panic
    const : mc_Error_UNKNOWN_OWNER
    func  : mc_Panic_ini_data
            mc_Panic_get_data
            mc_Panic_fin
            mc_Panic_put */

#define INDEX(I) fputs(RC_F(RC_YELLOW, "(" #I ")") RC_XFBS("\n"), stderr)

/*----------------------------------------------------------------------------*/
int
main(void)
{
    mc_Panic panic;
    mc_Panic_ini_data(&panic);

    FILE *log_file;
    if (!(log_file = fopen("/tmp/meaculpa-output", "w")))
    {
        mc_Panic_ini_data(&panic, main, mc_Error_FILE_ACCESS);
        mc_Panic_put(&panic, main, strerror(errno));
        return EXIT_FAILURE;
    }

    INDEX(1);
    mc_Panic_put(NULL);

    /* Nothing will happen as there is no error set */
    INDEX(2);
    mc_Panic_put(&panic);

    INDEX(3);
    mc_Panic_put(&panic, main);

    INDEX(4);
    mc_Panic_put(&panic, main, NULL);

    INDEX(5);
    mc_Panic_put(&panic, main, "Oh, snap!", log_file);

    /* Set error */
    mc_Panic_ini_data(&panic,
                      main,
                      mc_Error_EXPERIMENTAL,
                      "This test is experimental!");

    /**/
    INDEX(6);
    mc_Panic_put(&panic);

    INDEX(7);
    mc_Panic_put(&panic, main); /* No output => error=mc_Error_OKAY */

    INDEX(8);
    mc_Panic_put(&panic, main, NULL); /* No output => error=mc_Error_OKAY */

    INDEX(9);
    mc_Panic_put(&panic, main, "Oh, snap!", log_file);

    INDEX(10);
    mc_Panic_put(&panic, main, mc_Panic_get_data(&panic), log_file);


    fclose(log_file);
    mc_Panic_fin(&panic);
    return EXIT_SUCCESS;
}
