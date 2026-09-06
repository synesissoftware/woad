/* /////////////////////////////////////////////////////////////////////////
 * File:    src/api.c
 *
 * Purpose: Implementation of the woad API - version, colour policy, stream
 *          inspection, and conditional sequence selection.
 *
 * Credit:  The truey/falsey classification of environment-variable values
 *          derives from 2be (https://github.com/synesissoftware/2be); see
 *          the PROVENANCE note above woad_is_truey_(), below.
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


/* /////////////////////////////////////////////////////////////////////////
 * includes
 *
 * All operating-system inclusions are confined to this translation unit, so
 * that <woad/woad.h> can remain free of them.
 */

#include <woad/woad.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <io.h>
# include <windows.h>

# ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#  define ENABLE_VIRTUAL_TERMINAL_PROCESSING                (0x0004)
# endif
#else

# include <unistd.h>
#endif


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

static char const   woad_empty_[] = "";


/* /////////////////////////////////////////////////////////////////////////
 * helper functions - strings
 */

static
int
woad_stricmp_(
    char const*     s1
,   char const*     s2
)
{
    for (; '\0' != *s1 && '\0' != *s2; ++s1, ++s2)
    {
        int const ch1 = tolower((unsigned char)*s1);
        int const ch2 = tolower((unsigned char)*s2);

        if (ch1 != ch2)
        {
            return (ch1 < ch2) ? -1 : +1;
        }
    }

    if ('\0' == *s1 && '\0' == *s2)
    {
        return 0;
    }

    return ('\0' == *s1) ? -1 : +1;
}

/* PROVENANCE: the truey/falsey classification below - both the terminology
 * and the core token sets - is taken from **2be**, the C implementation of
 * Synesis Information Systems' **to-be** family of libraries:
 *
 *   https://github.com/synesissoftware/2be
 *
 * 2be classifies a string as "truthy" when it is recognised as a
 * boolean-like token at all, thence as "truey" or "falsey" according to
 * which sense it carries - hence the names of the two functions here - and
 * its core sets are `{ "1", "on", "true", "yes" }` and
 * `{ "0", "false", "no", "off" }` (see `twob_string_is_truey()` and
 * `twob_string_is_falsey()`, and `src/truthy.c` therein).
 *
 * woad reimplements the classification locally, rather than taking a
 * dependency on 2be, solely to keep woad's efferent coupling at nil - a
 * colour-code library should not oblige its clients to acquire a second
 * library in order to read an environment variable. The debt is
 * acknowledged, not avoided: were woad ever to acquire another dependency
 * for other reasons, 2be would be the right way to do this.
 *
 * This implementation differs from 2be in three respects, all of them
 * narrowings or widenings for woad's specific purpose:
 *
 * - it adds the domain-specific tokens `"always"` and `"force"` (truey) and
 *   `"never"` and `"none"` (falsey), which express colour policy rather
 *   than a general boolean;
 * - it compares case-insensitively, in one pass, where 2be maintains
 *   separate precise-capitalisation and lowercase token sets;
 * - it does not trim the string before classifying it, since an
 *   environment variable's value is taken as given;
 */

static
int
woad_is_truey_(
    char const*     value
)
{
    static char const* const s_truey[] =
    {
        "1",
        "always",
        "force",
        "on",
        "true",
        "yes",
    };

    size_t i;

    for (i = 0; i != sizeof(s_truey) / sizeof(s_truey[0]); ++i)
    {
        if (0 == woad_stricmp_(value, s_truey[i]))
        {
            return 1;
        }
    }

    return 0;
}

static
int
woad_is_falsey_(
    char const*     value
)
{
    static char const* const s_falsey[] =
    {
        "0",
        "false",
        "never",
        "no",
        "none",
        "off",
    };

    size_t i;

    for (i = 0; i != sizeof(s_falsey) / sizeof(s_falsey[0]); ++i)
    {
        if (0 == woad_stricmp_(value, s_falsey[i]))
        {
            return 1;
        }
    }

    return 0;
}

static
char const*
woad_getenv_nonempty_(
    char const*     name
)
{
    char const* const value = getenv(name);

    if (NULL == value ||
        '\0' == value[0])
    {
        return NULL;
    }

    return value;
}


/* /////////////////////////////////////////////////////////////////////////
 * helper functions - environment
 */

static
int
woad_policy_from_environment_(void)
{
    char const* value;

    /* 1. woad's own variable, which overrides all others. */

    if (NULL == (value = woad_getenv_nonempty_("WOAD_COLOUR")))
    {
        value = woad_getenv_nonempty_("WOAD_COLOR");
    }

    if (NULL != value)
    {
        if (woad_is_truey_(value))
        {
            return WOAD_COLOUR_POLICY_FORCE;
        }

        if (woad_is_falsey_(value))
        {
            return WOAD_COLOUR_POLICY_INHIBIT;
        }

        /* "auto", or anything unrecognised, defers to the stream. */

        return WOAD_COLOUR_POLICY_AUTO;
    }

    /* 2. NO_COLOR - see https://no-color.org/ - which, being an explicit
     * instruction from the user, outranks the force-oriented variables.
     */

    if (NULL != woad_getenv_nonempty_("NO_COLOR"))
    {
        return WOAD_COLOUR_POLICY_INHIBIT;
    }

    /* 3. CLICOLOR_FORCE, as understood by the BSD/GNU tool conventions. */

    if (NULL != (value = woad_getenv_nonempty_("CLICOLOR_FORCE")))
    {
        if (0 != strcmp(value, "0"))
        {
            return WOAD_COLOUR_POLICY_FORCE;
        }
    }

    /* 4. CLICOLOR=0. */

    if (NULL != (value = woad_getenv_nonempty_("CLICOLOR")))
    {
        if (0 == strcmp(value, "0"))
        {
            return WOAD_COLOUR_POLICY_INHIBIT;
        }
    }

    return WOAD_COLOUR_POLICY_AUTO;
}

static
int
woad_term_is_dumb_from_environment_(void)
{
    char const* const value = woad_getenv_nonempty_("TERM");

    return (NULL != value && 0 == woad_stricmp_(value, "dumb")) ? 1 : 0;
}


/* /////////////////////////////////////////////////////////////////////////
 * cached determinations
 *
 * The environment inspection is a pure query, and is idempotent, so a plain
 * flag suffices: two threads racing to populate the cache compute the same
 * answer, and neither observes a torn value, since each datum is a single
 * `int`. Deliberately, no thread library is used, so that woad acquires no
 * efferent dependency beyond the C runtime.
 *
 * The Windows console determination is *not* a pure query - it may call
 * `SetConsoleMode()`, which mutates process-wide console state - and so is
 * elected to a single thread by compare-and-swap. (It degenerates to a pure
 * query when `WOAD_SET_CONSOLE_MODE` withholds permission to modify the
 * console, but the election is retained regardless, since the variable is
 * read within the elected determination.)
 */

static int          s_environment_determined;
static int          s_policy;
static int          s_term_is_dumb;

static
void
woad_ensure_environment_(void)
{
    if (!s_environment_determined)
    {
        s_policy = woad_policy_from_environment_();
        s_term_is_dumb = woad_term_is_dumb_from_environment_();

        s_environment_determined = 1;
    }
}

#ifdef _WIN32

/* Obtains the Windows OS build number via RtlGetVersion, or 0 on failure.
 */
static
unsigned
woad_windows_build_number_(void)
{
    typedef LONG (WINAPI *woad_RtlGetVersion_fn_t_)(OSVERSIONINFOW*);

    HMODULE const ntdll = GetModuleHandleW(L"ntdll.dll");

    if (NULL != ntdll)
    {
        /* Avoid -Wcast-function-type (MinGW -Werror): FARPROC is not a
         * compatible function-pointer type for a direct cast.
         */
        FARPROC const proc = GetProcAddress(ntdll, "RtlGetVersion");
        woad_RtlGetVersion_fn_t_ RtlGetVersion = NULL;

        if (NULL != proc)
        {
            memcpy(&RtlGetVersion, &proc, sizeof(RtlGetVersion));
        }

        if (NULL != RtlGetVersion)
        {
            OSVERSIONINFOW osvi;

            ZeroMemory(&osvi, sizeof(osvi));
            osvi.dwOSVersionInfoSize = sizeof(osvi);

            if (0 == RtlGetVersion(&osvi))
            {
                return osvi.dwBuildNumber;
            }
        }
    }

    return 0;
}

/* Determines whether woad is permitted to modify the console mode.
 *
 * Absence of the variable means permission: the modification is woad's
 * default behaviour, and the variable exists only so that it may be
 * withheld. A falsey value withholds it; any other value - including one
 * that is neither truey nor falsey - leaves the default in force, exactly as
 * an unrecognised WOAD_COLOUR leaves automatic detection in force.
 */
static
int
woad_may_set_console_mode_from_environment_(void)
{
    char const* const value = woad_getenv_nonempty_("WOAD_SET_CONSOLE_MODE");

    if (NULL == value)
    {
        return 1;
    }

    return woad_is_falsey_(value) ? 0 : 1;
}

/* Determines whether the given handle will interpret ANSI SGR sequences,
 * enabling virtual-terminal processing upon it if that is necessary and
 * permitted.
 *
 * \param may_set Whether SetConsoleMode() may be called. When 0, this
 *   degenerates to a pure query: a console upon which the caller has already
 *   enabled virtual-terminal processing is still reported as capable, but no
 *   console state is altered.
 */
static
int
woad_try_enable_console_vt_(
    HANDLE          h
,   int             may_set
)
{
    DWORD mode;

    if (NULL == h ||
        INVALID_HANDLE_VALUE == h)
    {
        return 0;
    }

    if (!GetConsoleMode(h, &mode))
    {
        return 0;
    }

    if (0 != (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        return 1;
    }

    if (!may_set)
    {
        return 0;
    }

    return SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) ? 1 : 0;
}

static
int
woad_detect_console_supports_ansi_(void)
{
    unsigned const build = woad_windows_build_number_();

    /* Windows 11 (22000+) typically accepts ANSI without opt-in.
     *
     * NOTE: WOAD_SET_CONSOLE_MODE is not consulted here, since nothing is
     * modified on this path: there is no permission to withhold.
     */
    if (build >= 22000u)
    {
        return 1;
    }
    else if (build >= 16257u ||
             0u == build)
    {
        /* Windows 10 builds that support VT processing, or unknown build
         * (best-effort): expand compatibility via SetConsoleMode, unless the
         * user has withheld permission to modify the console, in which case
         * the handles are only queried.
         */
        int const may_set = woad_may_set_console_mode_from_environment_();

        int const out_ok = woad_try_enable_console_vt_(GetStdHandle(STD_OUTPUT_HANDLE), may_set);
        int const err_ok = woad_try_enable_console_vt_(GetStdHandle(STD_ERROR_HANDLE), may_set);

        return (out_ok || err_ok) ? 1 : 0;
    }
    else
    {
        return 0;
    }
}

/* 0 => not started; 1 => in progress; 2 => complete. */
static LONG volatile s_console_state;
static int           s_console_supports_ansi;

static
int
woad_ensure_console_(void)
{
    /* Elect exactly one thread - the one that observes the initial 0 - to
     * perform the determination.
     */
    if (0 == InterlockedCompareExchange(&s_console_state, 1, 0))
    {
        s_console_supports_ansi = woad_detect_console_supports_ansi_();

        InterlockedExchange(&s_console_state, 2);
    }
    else
    {
        /* Another thread is performing, or has performed, the
         * determination; wait for it, so that no caller can observe a
         * half-made determination. (The comparand-equals-exchange form is
         * used simply as an atomic read.)
         */
        while (2 != InterlockedCompareExchange(&s_console_state, 2, 2))
        {
            Sleep(0);
        }
    }

    return s_console_supports_ansi;
}
#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////////////
 * helper functions - terminal attachment
 */

static
int
woad_isatty_fd_(
    int             fd
)
{
#ifdef _WIN32

    return _isatty(fd) ? 1 : 0;
#else

    return isatty(fd) ? 1 : 0;
#endif
}

static
int
woad_isatty_stream_(
    FILE*           stm
)
{
    if (NULL == stm)
    {
        return 0;
    }

#if 0 || \
    defined(_WIN32) || \
    (   1 && \
        defined(_POSIX_C_SOURCE) && \
        (_POSIX_C_SOURCE >= 1) && \
        1) || \
    defined(_BSD_SOURCE) || \
    defined(_DEFAULT_SOURCE) || \
    defined(_GNU_SOURCE) || \
    defined(_XOPEN_SOURCE) || \
    !defined(__STRICT_ANSI__) || \
    0

    /* `fileno()` is POSIX (and `_fileno()` its Windows spelling). Prefer it
     * when feature-test macros or a non-strict dialect indicate that the
     * declaration is visible.
     */

# ifdef _WIN32

    return woad_isatty_fd_(_fileno(stm));
# else

    return woad_isatty_fd_(fileno(stm));
# endif
#else

    /* Strict ISO C (e.g. -std=c11): glibc/musl typically do not declare
     * `fileno()`. Map the standard streams via the unistd.h constants
     * instead, and treat any other stream as not a terminal.
     */

    if (stdout == stm)
    {
        return woad_isatty_fd_(STDOUT_FILENO);
    }
    else if (stderr == stm)
    {
        return woad_isatty_fd_(STDERR_FILENO);
    }
    else if (stdin == stm)
    {
        return woad_isatty_fd_(STDIN_FILENO);
    }
    else
    {
        return 0;
    }
#endif
}

/* Applies the policy to a terminal-attachment determination. */
static
int
woad_supports_colour_(
    int             is_tty
)
{
    woad_ensure_environment_();

    switch (s_policy)
    {
    case WOAD_COLOUR_POLICY_FORCE:

        return 1;
    case WOAD_COLOUR_POLICY_INHIBIT:

        return 0;
    default:

        break;
    }

    if (!is_tty)
    {
        return 0;
    }

    if (s_term_is_dumb)
    {
        return 0;
    }

    return woad_console_supports_ansi();
}


/* /////////////////////////////////////////////////////////////////////////
 * API functions - version
 */

unsigned long
woad_version(void)
{
    return (unsigned long)WOAD_VER;
}

char const*
woad_version_string(void)
{
    return WOAD_VER_STRING;
}


/* /////////////////////////////////////////////////////////////////////////
 * API functions - policy
 */

int
woad_colour_policy(void)
{
    woad_ensure_environment_();

    return s_policy;
}

void
woad_refresh(void)
{
    s_environment_determined = 0;

#ifdef _WIN32

    InterlockedExchange(&s_console_state, 0);
#endif
}

int
woad_console_supports_ansi(void)
{
#ifdef _WIN32

    return woad_ensure_console_();
#else

    return 1;
#endif
}


/* /////////////////////////////////////////////////////////////////////////
 * API functions - stream inspection
 */

int
woad_stream_is_tty(
    FILE*           stm
)
{
    return woad_isatty_stream_(stm);
}

int
woad_fd_is_tty(
    int             fd
)
{
    return woad_isatty_fd_(fd);
}

int
woad_stream_supports_colour(
    FILE*           stm
)
{
    return woad_supports_colour_(woad_isatty_stream_(stm));
}

int
woad_fd_supports_colour(
    int             fd
)
{
    return woad_supports_colour_(woad_isatty_fd_(fd));
}

#ifdef _WIN32

int
woad_handle_is_tty(
    void*           h
)
{
    DWORD mode;

    if (NULL == h ||
        INVALID_HANDLE_VALUE == (HANDLE)h)
    {
        return 0;
    }

    return GetConsoleMode((HANDLE)h, &mode) ? 1 : 0;
}

int
woad_std_handle_is_tty(
    unsigned long   id
)
{
    return woad_handle_is_tty((void*)GetStdHandle((DWORD)id));
}

int
woad_handle_supports_colour(
    void*           h
)
{
    return woad_supports_colour_(woad_handle_is_tty(h));
}

int
woad_std_handle_supports_colour(
    unsigned long   id
)
{
    return woad_supports_colour_(woad_std_handle_is_tty(id));
}
#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////////////
 * API functions - conditional sequences
 */

static
char const*
woad_seq_or_empty_(
    char const*     seq
,   int             supported
)
{
    if (!supported ||
        NULL == seq)
    {
        return woad_empty_;
    }

    return seq;
}

char const*
woad_seq_for_stream(
    char const*     seq
,   FILE*           stm
)
{
    return woad_seq_or_empty_(seq, woad_stream_supports_colour(stm));
}

char const*
woad_seq_for_fd(
    char const*     seq
,   int             fd
)
{
    return woad_seq_or_empty_(seq, woad_fd_supports_colour(fd));
}

#ifdef _WIN32

char const*
woad_seq_for_handle(
    char const*     seq
,   void*           h
)
{
    return woad_seq_or_empty_(seq, woad_handle_supports_colour(h));
}

char const*
woad_seq_for_std_handle(
    char const*     seq
,   unsigned long   id
)
{
    return woad_seq_or_empty_(seq, woad_std_handle_supports_colour(id));
}
#endif /* _WIN32 */


/* ///////////////////////////// end of file //////////////////////////// */

