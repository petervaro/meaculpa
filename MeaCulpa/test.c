/* INFO ************************************************************************
**                                                                            **
**                                  MeaCulpa                                  **
**                                  ========                                  **
**                                                                            **
**      Sophisticated, minimalistic and high-level error handling for C       **
**                       Version: 0.1.4.069 (20150602)                        **
**                           File: MeaCulpa/test.c                            **
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

#include <stdio.h> /*
    func  : printf
*/
#include <stdlib.h> /*
    const : EXIT_SUCCESS
            EXIT_FAILURE
*/
#include "MeaCulpa.h"

mc_Error
fn1(void)
{
    return (mc_Error)mc_FAIL(fn1);
}

mc_Error
fn2(void)
{
    return (mc_Error)mc_OKAY(fn2);
}


int
main(void)
{
    mc_Error signal,
             signals[] = {fn1(), fn2()};

    for (size_t i=0; i<2; i++)
    {
        signal = signals[i];
        if (mc_IS_OWNER(fn1, signal))
        {
            switch (signal.error)
            {
                case mc_Okay:
                    break;

                case mc_Fail:
                    mc_print(mc_Fail, "fn1", 2, (const char*[]){"Test error",
                                                                "Another line"});
                    break;
            }
        }
        else if (mc_IS_OWNER(fn2, signal))
        {
            switch (signal.error)
            {
                case mc_Okay:
                    break;

                case mc_Fail:
                    mc_print(mc_Fail, "fn2", 2, (const char*[]){"Test error",
                                                                "Another line"});
                    break;
            }
        }
    }

    mc_print(55, NULL, 0, NULL);
    mc_print(55, "fake", 0, NULL);
    mc_print(55, "fake", 1, NULL);

    mc_print(mc_NullPtr, "intern", 1, (const char*[]){"1st argument is NULL"});
    mc_print(mc_Okay, "fake", 1, (const char*[]){"Invalid value"});
    mc_print(mc_Okay, "main", 1, (const char*[]){"Internal error in fake"});

    return EXIT_SUCCESS;
}
