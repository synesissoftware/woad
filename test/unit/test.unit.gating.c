/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.gating.c
 *
 * Purpose: Unit tests for woad stream inspection, colour policy, and
 *          conditional sequence selection.
 *
 *          The determinations are made deterministic by using a temporary
 *          file - which is never a terminal - as the subject stream, and by
 *          driving the policy from the environment.
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


/* `setenv()` / `unsetenv()` are POSIX, and are not declared by glibc under
 * a strict ISO dialect, which is what this project's tests are compiled as.
 */
#if !defined(_WIN32) && \
    !defined(_POSIX_C_SOURCE)
# define _POSIX_C_SOURCE                                    200809L
#endif

#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * test infrastructure
 */

static int
fail_(
    char const*  expr
,   char const*  file
,   int          line
)
{
    fprintf(stderr, "%s:%d: assertion failed: %s\n", file, line, expr);

    return EXIT_FAILURE;
}

#define ASSERT_TRUE_(expr)                                  \
                                                            \
    do                                                      \
    {                                                       \
        if (!(expr))                                        \
        {                                                   \
            return fail_(#expr, __FILE__, __LINE__);        \
        }                                                   \
    } while (0)

#define ASSERT_SEQ_(expected, actual)                       \
                                                            \
    ASSERT_TRUE_(0 == strcmp((expected), (actual)))


/* /////////////////////////////////////////////////////////////////////////
 * environment manipulation
 */

static void
set_env_(
    char const*  name
,   char const*  value
)
{
#ifdef _WIN32

    (void)_putenv_s(name, value);
#else

    (void)setenv(name, value, 1);
#endif

    woad_refresh();
}

static void
unset_env_(
    char const*  name
)
{
#ifdef _WIN32

    (void)_putenv_s(name, "");
#else

    (void)unsetenv(name);
#endif

    woad_refresh();
}

static void
clear_all_env_(void)
{
    unset_env_("CLICOLOR");
    unset_env_("CLICOLOR_FORCE");
    unset_env_("NO_COLOR");
    unset_env_("WOAD_COLOR");
    unset_env_("WOAD_COLOUR");
    unset_env_("WOAD_SET_CONSOLE_MODE");
}


/* /////////////////////////////////////////////////////////////////////////
 * tests
 */

int main(void)
{
    FILE* const not_a_tty = tmpfile();

    ASSERT_TRUE_(NULL != not_a_tty);

    clear_all_env_();


    /* the library version is that of the header against which this test was
     * compiled
     */

    ASSERT_TRUE_((unsigned long)WOAD_VER == woad_version());
    ASSERT_SEQ_(WOAD_VER_STRING, woad_version_string());


    /* stream inspection */

    ASSERT_TRUE_(0 == woad_stream_is_tty(not_a_tty));
    ASSERT_TRUE_(0 == woad_stream_is_tty(NULL));


    /* WOAD_COLOUR_POLICY_AUTO: a non-terminal stream gets no colour */

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_AUTO == woad_colour_policy());
    ASSERT_TRUE_(0 == woad_stream_supports_colour(not_a_tty));
    ASSERT_SEQ_("", woad_seq_for_stream(WOAD_FG_RED, not_a_tty));
    ASSERT_SEQ_("", WOAD_FG_RED_FOR(not_a_tty));
    ASSERT_SEQ_("", WOAD_RESET_FOR(not_a_tty));


    /* WOAD_COLOUR forces colour, whatever the stream */

    set_env_("WOAD_COLOUR", "always");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_FORCE == woad_colour_policy());
    ASSERT_TRUE_(0 != woad_stream_supports_colour(not_a_tty));
    ASSERT_SEQ_(WOAD_FG_RED, woad_seq_for_stream(WOAD_FG_RED, not_a_tty));
    ASSERT_SEQ_(WOAD_FG_RED, WOAD_FG_RED_FOR(not_a_tty));
    ASSERT_SEQ_(WOAD_BG_BLACK, WOAD_BG_BLACK_FOR(not_a_tty));
    ASSERT_SEQ_(WOAD_BG_RED, WOAD_FG_FOR(WOAD_BG_RED, not_a_tty));
    ASSERT_SEQ_(WOAD_RESET, WOAD_RESET_FOR(not_a_tty));

    /* ... and the value is matched case-insensitively */

    set_env_("WOAD_COLOUR", "ALWAYS");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_FORCE == woad_colour_policy());

    /* ... and a NULL sequence still yields the empty string */

    ASSERT_SEQ_("", woad_seq_for_stream(NULL, not_a_tty));

    /* ... and a NULL stream is answered by the policy alone */

    ASSERT_SEQ_(WOAD_FG_RED, woad_seq_for_stream(WOAD_FG_RED, NULL));


    /* WOAD_COLOUR inhibits colour, whatever the stream */

    set_env_("WOAD_COLOUR", "never");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_INHIBIT == woad_colour_policy());
    ASSERT_TRUE_(0 == woad_stream_supports_colour(not_a_tty));
    ASSERT_SEQ_("", woad_seq_for_stream(WOAD_FG_RED, not_a_tty));
    ASSERT_SEQ_("", woad_seq_for_stream(WOAD_FG_RED, NULL));


    /* an unrecognised value defers to the stream */

    set_env_("WOAD_COLOUR", "banana");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_AUTO == woad_colour_policy());


    /* WOAD_COLOR is the accepted alternative spelling */

    clear_all_env_();
    set_env_("WOAD_COLOR", "1");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_FORCE == woad_colour_policy());


    /* NO_COLOR inhibits colour */

    clear_all_env_();
    set_env_("NO_COLOR", "1");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_INHIBIT == woad_colour_policy());


    /* CLICOLOR_FORCE forces colour, but is outranked by NO_COLOR */

    clear_all_env_();
    set_env_("CLICOLOR_FORCE", "1");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_FORCE == woad_colour_policy());

    set_env_("NO_COLOR", "1");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_INHIBIT == woad_colour_policy());

    /* ... and both are outranked by woad's own variable */

    set_env_("WOAD_COLOUR", "force");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_FORCE == woad_colour_policy());


    /* CLICOLOR=0 inhibits colour */

    clear_all_env_();
    set_env_("CLICOLOR", "0");

    ASSERT_TRUE_(WOAD_COLOUR_POLICY_INHIBIT == woad_colour_policy());


    /* the type-dispatched forms agree with the specific forms */

#if defined(__cplusplus) || \
    (   defined(__STDC_VERSION__) && \
        __STDC_VERSION__ >= 201112L)

    clear_all_env_();
    set_env_("WOAD_COLOUR", "always");

    ASSERT_SEQ_(WOAD_FG_RED, WOAD_SEQ_FOR(WOAD_FG_RED, not_a_tty));
    ASSERT_SEQ_(WOAD_FG_RED, woad_seq_for(WOAD_FG_RED, not_a_tty));
    ASSERT_SEQ_(woad_seq_for_fd(WOAD_FG_RED, 1), woad_seq_for(WOAD_FG_RED, 1));
    ASSERT_TRUE_(woad_fd_supports_colour(1) == woad_supports_colour(1));
    ASSERT_TRUE_(woad_stream_supports_colour(not_a_tty) == woad_supports_colour(not_a_tty));
#endif


    /* the console determination is stable across repeated calls */

    clear_all_env_();

    ASSERT_TRUE_(woad_console_supports_ansi() == woad_console_supports_ansi());


    /* WOAD_SET_CONSOLE_MODE withholds permission to modify the console.
     *
     * The determination it governs is Windows-only, and its outcome depends
     * on the console the test happens to be attached to, so what can be
     * asserted portably is that the variable is understood and harmless: the
     * determination remains stable and self-consistent whether permission is
     * withheld, granted, or unstated, and the colour policy - which the
     * variable has no business influencing - is unmoved by it.
     */

    set_env_("WOAD_SET_CONSOLE_MODE", "0");

    ASSERT_TRUE_(woad_console_supports_ansi() == woad_console_supports_ansi());
    ASSERT_TRUE_(WOAD_COLOUR_POLICY_AUTO == woad_colour_policy());

    set_env_("WOAD_SET_CONSOLE_MODE", "1");

    ASSERT_TRUE_(woad_console_supports_ansi() == woad_console_supports_ansi());
    ASSERT_TRUE_(WOAD_COLOUR_POLICY_AUTO == woad_colour_policy());

    /* an unrecognised value leaves the default - permission - in force, and
     * so cannot differ from the unset case
     */

    set_env_("WOAD_SET_CONSOLE_MODE", "banana");

    {
        int const with_nonsense = woad_console_supports_ansi();

        unset_env_("WOAD_SET_CONSOLE_MODE");

        ASSERT_TRUE_(with_nonsense == woad_console_supports_ansi());
    }

#ifndef _WIN32

    /* off Windows there is no console mode to modify, so the variable is
     * inert, and the determination is unconditionally affirmative
     */

    set_env_("WOAD_SET_CONSOLE_MODE", "never");

    ASSERT_TRUE_(1 == woad_console_supports_ansi());
#endif


    clear_all_env_();

    fclose(not_a_tty);

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

