/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/versions/main.c
 *
 * Purpose: Prints woad composite version.
 *
 * Created: 17th September 2026
 * Updated: 27th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>


static void
version(
    FILE*       stm
,   char const* prefix
,   char const* libname
,   char const* macroname
,   unsigned    libver
)
{
    fprintf(
        stm
    ,   "%s%s%s%s: %sv%s%s (%s%s%s = %s0x%08x%s)\n"
    ,   prefix
    ,   WOAD_FG_BLUE_FOR(stm)
    ,   libname
    ,   WOAD_RESET_FOR(stm)
    ,   WOAD_FG_GREEN_FOR(stm)
    ,   woad_version_string()
    ,   WOAD_RESET_FOR(stm)
    ,   WOAD_FG_CYAN_FOR(stm)
    ,   macroname
    ,   WOAD_RESET_FOR(stm)
    ,   WOAD_FG_GREEN_FOR(stm)
    ,   libver
    ,   WOAD_RESET_FOR(stm)
    );
}


int main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);

    {
        unsigned const libver = (unsigned)WOAD_VER;

        version(stdout, "", "woad", "WOAD_VER", libver);
    }

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

