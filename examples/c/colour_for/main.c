/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/c/colour_for/main.c
 *
 * Purpose: Example program that emits colour conditionally, according to
 *          whether the destination stream is attached to a terminal, and to
 *          the colour policy expressed in the environment.
 *
 *          Run it twice to see the difference:
 *
 *            ./example.c.colour_for
 *            ./example.c.colour_for | cat
 *
 *          and then again with the policy overridden:
 *
 *            WOAD_COLOUR=always ./example.c.colour_for | cat
 *            WOAD_COLOUR=never ./example.c.colour_for
 *
 * Created: 27th August 2026
 * Updated: 14th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>


static char const* policy_name(int policy)
{
    switch (policy)
    {
    case WOAD_COLOUR_POLICY_FORCE:

        return "force";
    case WOAD_COLOUR_POLICY_INHIBIT:

        return "inhibit";
    default:

        return "auto";
    }
}

int main(void)
{
    printf(
        "policy: %s; stdout is%s a tty; stderr is%s a tty\n"
    ,   policy_name(woad_colour_policy())
    ,   woad_stream_is_tty(stdout) ? "" : " not"
    ,   woad_stream_is_tty(stderr) ? "" : " not"
    );

    /* The colour-named conditional macros. */

    printf(
        "%sthis line is green if, and only if, stdout warrants colour%s\n"
    ,   WOAD_FG_GREEN_FOR(stdout)
    ,   WOAD_RESET_FOR(stdout)
    );

    printf(
        "%sthis line is bold if, and only if, stdout warrants colour%s\n"
    ,   WOAD_BOLD_FOR(stdout)
    ,   WOAD_RESET_FOR(stdout)
    );

    printf(
        "%sthis line blinks if, and only if, stdout warrants colour%s\n"
    ,   WOAD_BLINKING_FOR(stdout)
    ,   WOAD_RESET_FOR(stdout)
    );

    fprintf(
        stderr
    ,   "'%s%sthis line is magenta-on-bright_black if, and only if, stderr warrants colour%s'\n"
    ,   WOAD_BG_BRIGHT_BLACK_FOR(stderr)
    ,   WOAD_FG_BRIGHT_MAGENTA_FOR(stderr)
    ,   WOAD_RESET_FOR(stderr)
    );

    /* The colour-parameterised conditional macros, which take the sequence
     * to be applied, and so may be driven by a variable.
     */

    printf(
        "%sthis line is yellow if, and only if, stdout warrants colour%s\n"
    ,   WOAD_FG_FOR(WOAD_FG_YELLOW, stdout)
    ,   WOAD_RESET_FOR(stdout)
    );

    /* File descriptors serve equally well as the unit of currency, in C11
     * and later, and in C++, by type-dispatch.
     */

#if defined(__cplusplus) || \
    (   defined(__STDC_VERSION__) && \
        __STDC_VERSION__ >= 201112L)

    printf(
        "%sthis line is cyan if, and only if, descriptor 1 warrants colour%s\n"
    ,   WOAD_SEQ_FOR(WOAD_FG_CYAN, 1)
    ,   WOAD_SEQ_FOR(WOAD_RESET, 1)
    );
#endif

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

