/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/versions/main.c
 *
 * Purpose: Prints woad composite version.
 *
 * Created: 17th September 2026
 * Updated: 7th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>


#define PROGRAM_NAME                                        "versions"


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
    ,   "%s%s%s%s: %sv%u.%u.%u.%u%s (%s%s%s = %s0x%08x%s)\n"
    ,   prefix
    ,   WOAD_FG_BLUE_FOR(stm)
    ,   libname
    ,   WOAD_RESET_FOR(stm)
    ,   WOAD_FG_GREEN_FOR(stm)
    ,   (libver >> 24) & 0xff
    ,   (libver >> 16) & 0xff
    ,   (libver >> 8) & 0xff
    ,   (libver >> 0) & 0xff
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

