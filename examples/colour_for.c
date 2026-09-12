/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/colour_for.c
 *
 * Purpose: Example program that emits colour conditionally, according to
 *          whether the destination stream is attached to a terminal, and to
 *          the colour policy expressed in the environment.
 *
 *          Run it twice to see the difference:
 *
 *            ./colour_for
 *            ./colour_for | cat
 *
 *          and then again with the policy overridden:
 *
 *            WOAD_COLOUR=always ./colour_for | cat
 *            WOAD_COLOUR=never ./colour_for
 *
 * Created: 27th August 2026
 * Updated: 27th August 2026
 *
 * Home:    https://github.com/synesissoftware/woad/
 *
 * Copyright (c) 2026, Matthew Wilson and Synesis Information Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer;
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution;
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    fprintf(
        stderr
    ,   "%s%sthis line is white-on-red if, and only if, stderr warrants colour%s\n"
    ,   WOAD_BG_RED_FOR(stderr)
    ,   WOAD_FG_BRIGHT_WHITE_FOR(stderr)
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

