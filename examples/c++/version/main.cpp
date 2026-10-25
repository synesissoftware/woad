/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/c++/version/main.cpp
 *
 * Purpose: Example program that prints the woad version string, via both
 *          the version macros and the version functions.
 *
 * Created: 26th October 2026
 * Updated: 26th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>


int main(int /* argc */, char* /* argv */[])
{
    puts("C++:");

    printf(
        "\t%swoad%s: v%s%s%s (%s%s%s = %s0x%08lx%s)\n"
    ,   WOAD_FG_BLUE_FOR(stdout)
    ,   WOAD_RESET_FOR(stdout)
    ,   WOAD_FG_GREEN_FOR(stdout)
    ,   woad_version_string()
    ,   WOAD_RESET_FOR(stdout)
    ,   WOAD_FG_CYAN_FOR(stdout)
    ,   "WOAD_VER"
    ,   WOAD_RESET_FOR(stdout)
    ,   WOAD_FG_GREEN_FOR(stdout)
    ,   woad_version()
    ,   WOAD_RESET_FOR(stdout)
    );

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

