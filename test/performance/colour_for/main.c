/* /////////////////////////////////////////////////////////////////////////
 * File:    test/performance/colour_for/main.c
 *
 * Purpose: Times the colour-for API. Every call probes its currency for
 *          terminal attachment, and that probe is paid on every call:
 *          isatty() or _isatty() for a stream or a descriptor,
 *          GetConsoleMode() for a Windows console handle, and
 *          GetStdHandle() plus GetConsoleMode() for a standard-handle
 *          identifier. The colour policy is applied after the probe, so
 *          force and inhibit still pay it. The environment walk is
 *          cached, as is the Windows console ANSI determination
 *          (RtlGetVersion(), and where the build requires it
 *          SetConsoleMode()), which is elected once per loaded copy of
 *          the library.
 *
 *          Probe rows are the baseline. Warm colour-for rows add the
 *          policy decision on top of the same probe. The Windows rows
 *          are the currencies that exist only there. Cold rows call
 *          woad_refresh() on every iteration, repeating work a normal
 *          process pays once. Refresh discards the cached answer only;
 *          a console mode already enabled stays enabled.
 *
 *          Results are for human attention. There is no timing budget.
 *          WOAD_PERF_WARM_ITERATIONS, WOAD_PERF_COLD_ENV_ITERATIONS, and
 *          WOAD_PERF_COLD_CONSOLE_ITERATIONS override the iteration
 *          counts. SIS_PERFTESTS_GROUPGAPS, set by
 *          run_all_performance_tests.sh --gap-groups, inserts a blank
 *          line between groups.
 *
 * Created: 7th October 2026
 * Updated: 14th October 2026
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


/* `setenv()` / `unsetenv()` and `clock_gettime()` are POSIX, and are not
 * declared by glibc under a strict ISO dialect, which is what this
 * project's tests are compiled as.
 */

#if !defined(_WIN32) && \
    !defined(_POSIX_C_SOURCE)
# define _POSIX_C_SOURCE                                    200809L
#endif

#ifdef WOAD_PERF_HAS_PLATFORMSTL
# include <platformstl/platformstl.h>
#endif

#include <woad/woad.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>
#else

# include <time.h>
#endif


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#ifdef NDEBUG

# define WARM_ITERS_DEFAULT_                                (1000000ull)
# define COLD_ENV_ITERS_DEFAULT_                            (50000ull)
# define COLD_CONSOLE_ITERS_DEFAULT_                        (20000ull)
#else

# define WARM_ITERS_DEFAULT_                                (100000ull)
# define COLD_ENV_ITERS_DEFAULT_                            (5000ull)
# define COLD_CONSOLE_ITERS_DEFAULT_                        (2000ull)
#endif

#define WARMUP_CALLS_                                       (32ull)


/* /////////////////////////////////////////////////////////////////////////
 * types
 */

typedef unsigned long long (*runner_fn_t)(unsigned long long iterations);


/* /////////////////////////////////////////////////////////////////////////
 * state
 *
 * The volatile sinks keep each iteration's result live. The clock is the
 * platform monotonic counter. When PlatformSTL is available, the preamble
 * names the host from its operating-system discriminations and
 * UNIXSTL_ARCH_LABEL_STRING / WINSTL_ARCH_LABEL_STRING.
 */

static FILE*                        s_file;
static char const* volatile         s_seq_sink;
static int volatile                 s_int_sink;
static int                          s_gap_groups;
static int                          s_have_group;

#ifdef _WIN32

static void*                        s_stdout_handle;
static unsigned long                s_stdout_id;
static unsigned long long           s_qpc_freq;
#endif


/* /////////////////////////////////////////////////////////////////////////
 * clock
 */

static int
init_clock_(void)
{
#ifdef _WIN32

    LARGE_INTEGER freq;

    if (!QueryPerformanceFrequency(&freq) ||
        0 == freq.QuadPart)
    {
        fprintf(stderr, "test.performance.colour_for: QueryPerformanceFrequency failed\n");

        return -1;
    }

    s_qpc_freq = (unsigned long long)freq.QuadPart;

    return 0;
#else

    struct timespec ts;

    if (0 != clock_gettime(CLOCK_MONOTONIC, &ts))
    {
        fprintf(stderr, "test.performance.colour_for: clock_gettime failed\n");

        return -1;
    }

    return 0;
#endif
}

static unsigned long long
now_ns_(void)
{
#ifdef _WIN32

    LARGE_INTEGER t;
    unsigned long long ticks;
    unsigned long long sec;
    unsigned long long rem;

    QueryPerformanceCounter(&t);

    ticks = (unsigned long long)t.QuadPart;
    sec = ticks / s_qpc_freq;
    rem = ticks % s_qpc_freq;

    return sec * 1000000000ull + (rem * 1000000000ull) / s_qpc_freq;
#else

    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (unsigned long long)ts.tv_sec * 1000000000ull
        + (unsigned long long)ts.tv_nsec;
#endif
}


/* /////////////////////////////////////////////////////////////////////////
 * formatting
 */

static void
format_u64_(
    char*               dest
,   size_t              dest_n
,   unsigned long long  v
)
{
    char raw[32];
    char grouped[48];
    int  n;
    int  i;
    int  g;

    n = snprintf(raw, sizeof(raw), "%llu", v);

    if (n < 0 ||
        (size_t)n >= sizeof(raw))
    {
        snprintf(dest, dest_n, "%llu", v);

        return;
    }

    grouped[sizeof(grouped) - 1] = '\0';
    i = (int)sizeof(grouped) - 1;
    g = 0;

    while (0 != n)
    {
        if (3 == g)
        {
            grouped[--i] = ',';
            g = 0;
        }

        grouped[--i] = raw[--n];
        ++g;
    }

    snprintf(dest, dest_n, "%s", grouped + i);
}

static void
format_ns_per_(
    char*               dest
,   size_t              dest_n
,   unsigned long long  elapsed_ns
,   unsigned long long  iterations
)
{
    unsigned long long tenths;
    char               whole[32];
    size_t             n;

    if (0 == dest_n)
    {
        return;
    }

    if (0 == iterations)
    {
        if (dest_n >= 4)
        {
            dest[0] = '0';
            dest[1] = '.';
            dest[2] = '0';
            dest[3] = '\0';
        }
        else
        {
            dest[0] = '\0';
        }

        return;
    }

    tenths = (elapsed_ns * 10ull + iterations / 2ull) / iterations;
    format_u64_(whole, sizeof(whole), tenths / 10ull);
    n = strlen(whole);

    /* whole + '.' + one digit + NUL; avoid snprintf so MinGW does not
     * diagnose a theoretical truncation of the concatenated form.
     */
    if (n + 3 > dest_n)
    {
        dest[0] = '\0';

        return;
    }

    memcpy(dest, whole, n);
    dest[n] = '.';
    dest[n + 1] = (char)('0' + (int)(tenths % 10ull));
    dest[n + 2] = '\0';
}

static unsigned long long
iters_from_env_(
    char const*         name
,   unsigned long long  fallback
)
{
    char const* const   value = getenv(name);
    char*               end = NULL;
    unsigned long long  v;

    if (NULL == value ||
        '\0' == value[0])
    {
        return fallback;
    }

    v = strtoull(value, &end, 10);

    if (end == value ||
        NULL == end ||
        '\0' != *end ||
        0 == v)
    {
        return fallback;
    }

    return v;
}


/* /////////////////////////////////////////////////////////////////////////
 * environment
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
clear_policy_env_(void)
{
    unset_env_("CLICOLOR");
    unset_env_("CLICOLOR_FORCE");
    unset_env_("NO_COLOR");
    unset_env_("WOAD_COLOR");
    unset_env_("WOAD_COLOUR");
    unset_env_("WOAD_SET_CONSOLE_MODE");
}

static void
use_policy_(
    char const*  token
)
{
    clear_policy_env_();
    set_env_("WOAD_COLOUR", token);
}


/* /////////////////////////////////////////////////////////////////////////
 * sinks
 *
 * `anchor` is the caller's accumulator. One volatile store per call keeps
 * the result live and gives every row the same store, so the difference
 * between rows is the call itself.
 */

#define SINK_SEQ_(anchor, expr)                             \
                                                            \
    do                                                      \
    {                                                       \
        s_seq_sink = (expr);                                \
        (anchor) += (unsigned char)s_seq_sink[0];           \
        (anchor) += (unsigned long long)((uintptr_t)s_seq_sink & 0xFFu); \
    } while (0)

#define SINK_INT_(anchor, expr)                             \
                                                            \
    do                                                      \
    {                                                       \
        s_int_sink = (expr);                                \
        (anchor) += (unsigned long long)s_int_sink;         \
    } while (0)


/* /////////////////////////////////////////////////////////////////////////
 * scenarios
 *
 * Policy rows share one function, so the code under the timer is
 * identical and only the library's cached policy differs.
 */

static unsigned long long
run_loop_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, (int)(i & 0xFFu));
    }

    return anchor;
}

static unsigned long long
run_stream_tty_file_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, woad_stream_is_tty(s_file));
    }

    return anchor;
}

static unsigned long long
run_stream_tty_stdout_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, woad_stream_is_tty(stdout));
    }

    return anchor;
}

static unsigned long long
run_fd_tty_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, woad_fd_is_tty(1));
    }

    return anchor;
}

static unsigned long long
run_seq_file_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_SEQ_(anchor, WOAD_SEQ_FOR(WOAD_FG_GREEN, s_file));
    }

    return anchor;
}

static unsigned long long
run_seq_stdout_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_SEQ_(anchor, WOAD_SEQ_FOR(WOAD_FG_GREEN, stdout));
    }

    return anchor;
}

static unsigned long long
run_seq_fd_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_SEQ_(anchor, WOAD_SEQ_FOR(WOAD_FG_GREEN, 1));
    }

    return anchor;
}

static unsigned long long
run_line_stdout_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_SEQ_(anchor, WOAD_FG_GREEN_FOR(stdout));
        SINK_SEQ_(anchor, WOAD_RESET_FOR(stdout));
    }

    return anchor;
}

static unsigned long long
run_console_warm_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, woad_console_supports_ansi());
    }

    return anchor;
}

static unsigned long long
run_console_cold_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        woad_refresh();
        SINK_INT_(anchor, woad_console_supports_ansi());
    }

    return anchor;
}

static unsigned long long
run_env_cold_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        woad_refresh();
        SINK_SEQ_(anchor, WOAD_SEQ_FOR(WOAD_FG_GREEN, s_file));
    }

    return anchor;
}

#ifdef _WIN32

static unsigned long long
run_handle_tty_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, woad_console_handle_is_tty(s_stdout_handle));
    }

    return anchor;
}

static unsigned long long
run_std_handle_tty_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_INT_(anchor, woad_std_handle_is_tty(s_stdout_id));
    }

    return anchor;
}

static unsigned long long
run_seq_handle_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_SEQ_(anchor, WOAD_SEQ_FOR(WOAD_FG_GREEN, s_stdout_handle));
    }

    return anchor;
}

static unsigned long long
run_seq_std_handle_(unsigned long long n)
{
    unsigned long long anchor = 0;
    unsigned long long i;

    for (i = 0; n != i; ++i)
    {
        SINK_SEQ_(anchor, WOAD_SEQ_FOR(WOAD_FG_GREEN, s_stdout_id));
    }

    return anchor;
}
#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////////////
 * reporting
 */

static void
report_(
    char const*         name
,   unsigned long long  iterations
,   unsigned long long  elapsed_ns
,   unsigned long long  anchor
)
{
    char iters[32];
    char total_us[32];
    char per[32];
    char anch[32];

    format_u64_(iters, sizeof(iters), iterations);
    format_u64_(total_us, sizeof(total_us), elapsed_ns / 1000ull);
    format_ns_per_(per, sizeof(per), elapsed_ns, iterations);
    format_u64_(anch, sizeof(anch), anchor);

    printf(
        "  %-52s  %14s  %14s  %14s  %14s\n"
    ,   name
    ,   iters
    ,   total_us
    ,   per
    ,   anch
    );
}

static void
begin_group_(
    char const*  title
)
{
    if (s_have_group &&
        s_gap_groups)
    {
        putchar('\n');
    }

    printf("  %s\n", title);
    s_have_group = 1;
}

static void
measure_(
    char const*         name
,   unsigned long long  iterations
,   unsigned long long  calls_per_iteration
,   runner_fn_t         fn
,   int                 warm
)
{
    unsigned long long anchor;
    unsigned long long t0;
    unsigned long long elapsed;

    if (warm)
    {
        (void)fn(WARMUP_CALLS_);
    }

    t0 = now_ns_();
    anchor = fn(iterations);
    elapsed = now_ns_() - t0;

    report_(name, iterations * calls_per_iteration, elapsed, anchor);
}


/* /////////////////////////////////////////////////////////////////////////
 * platform
 *
 * Operating-system identity is PlatformSTL's: macOS, Linux, FreeBSD, and
 * so on. The architecture spelling is the STLSoft label string.
 */

static char const*
platform_name_(void)
{
#ifdef WOAD_PERF_HAS_PLATFORMSTL
# if defined(UNIXSTL_OS_IS_MACOSX)

    return "macOS";
# elif defined(UNIXSTL_OS_IS_AIX)

    return "AIX";
# elif defined(UNIXSTL_OS_IS_FREEBSD)

    return "FreeBSD";
# elif defined(UNIXSTL_OS_IS_HPUX)

    return "HP-UX";
# elif defined(UNIXSTL_OS_IS_LINUX)

    return "Linux";
# elif defined(UNIXSTL_OS_IS_NETBSD)

    return "NetBSD";
# elif defined(UNIXSTL_OS_IS_OPENBSD)

    return "OpenBSD";
# elif defined(UNIXSTL_OS_IS_SOLARIS)

    return "Solaris";
# elif defined(UNIXSTL_OS_IS_SUNOS)

    return "SunOS";
# elif defined(PLATFORMSTL_OS_IS_WINDOWS)

    return "Windows";
# elif defined(PLATFORMSTL_OS_IS_UNIX)

    return "Unix";
# else

    return "unknown";
# endif
#elif defined(_WIN32)

    return "Windows";
#else

    return "Unix";
#endif
}

static char const*
architecture_name_(void)
{
#ifdef WOAD_PERF_HAS_PLATFORMSTL
# if defined(PLATFORMSTL_OS_IS_UNIX)

    return UNIXSTL_ARCH_LABEL_STRING;
# elif defined(PLATFORMSTL_OS_IS_WINDOWS)

    return WINSTL_ARCH_LABEL_STRING;
# else

    return "unknown";
# endif
#else

    return "unspecified";
#endif
}


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(void)
{
    unsigned long long warm_iters;
    unsigned long long cold_env_iters;
    unsigned long long cold_console_iters;
    char const*        term;
    char               warm_text[32];
    char               cold_env_text[32];
    char               cold_console_text[32];

    if (0 != init_clock_())
    {
        return EXIT_FAILURE;
    }

    s_file = tmpfile();

    if (NULL == s_file)
    {
        fprintf(stderr, "test.performance.colour_for: tmpfile failed\n");

        return EXIT_FAILURE;
    }

    if (0 != woad_stream_is_tty(s_file))
    {
        fprintf(stderr, "test.performance.colour_for: temporary file is a tty\n");
        fclose(s_file);

        return EXIT_FAILURE;
    }

#ifdef _WIN32

    s_stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    s_stdout_id = (unsigned long)STD_OUTPUT_HANDLE;
#endif

    warm_iters = iters_from_env_("WOAD_PERF_WARM_ITERATIONS", WARM_ITERS_DEFAULT_);
    cold_env_iters = iters_from_env_("WOAD_PERF_COLD_ENV_ITERATIONS", COLD_ENV_ITERS_DEFAULT_);
    cold_console_iters = iters_from_env_("WOAD_PERF_COLD_CONSOLE_ITERATIONS", COLD_CONSOLE_ITERS_DEFAULT_);

#ifndef _WIN32

    /* Off Windows the console answer is a constant. Time it with the warm
     * count so the row is comparable with the cached Windows row.
     */

    if (NULL == getenv("WOAD_PERF_COLD_CONSOLE_ITERATIONS"))
    {
        cold_console_iters = warm_iters;
    }
#endif

    {
        char const* const gaps = getenv("SIS_PERFTESTS_GROUPGAPS");

        s_gap_groups = (NULL != gaps && '\0' != gaps[0] && 0 != strcmp(gaps, "0"));
    }

    term = getenv("TERM");

    format_u64_(warm_text, sizeof(warm_text), warm_iters);
    format_u64_(cold_env_text, sizeof(cold_env_text), cold_env_iters);
    format_u64_(cold_console_text, sizeof(cold_console_text), cold_console_iters);

    printf("woad colour-for costs\n");
    printf("  platform             : %s (%s)\n", platform_name_(), architecture_name_());
#ifdef NDEBUG

    printf("  build                : release\n");
#else

    printf("  build                : debug\n");
#endif
    printf("  stdout is a tty      : %s\n", woad_stream_is_tty(stdout) ? "yes" : "no");
    printf("  stderr is a tty      : %s\n", woad_stream_is_tty(stderr) ? "yes" : "no");
    printf("  TERM                 : %s\n", (NULL != term) ? term : "(unset)");
    printf("  warm iterations      : %s\n", warm_text);
    printf("  cold-env iterations  : %s\n", cold_env_text);
    printf("  cold-console iterations : %s\n", cold_console_text);
    printf("\n");
    printf("  Each colour-for call probes terminal attachment before the policy\n");
    printf("  is applied, so force and inhibit still pay that probe. The\n");
    printf("  environment walk is cached. On Windows the console ANSI\n");
    printf("  determination is cached after the first call. A cold row calls\n");
    printf("  woad_refresh() on every iteration, which discards the cached\n");
    printf("  answer; a console mode already enabled stays enabled.\n");
    printf("\n");
    printf(
        "  %-52s  %14s  %14s  %14s  %14s\n"
    ,   "scenario"
    ,   "calls"
    ,   "total (us)"
    ,   "ns/call"
    ,   "anchor"
    );

    use_policy_("auto");

    begin_group_("terminal probe");
    measure_("loop overhead", warm_iters, 1, run_loop_, 1);
    measure_("woad_stream_is_tty(file)", warm_iters, 1, run_stream_tty_file_, 1);
    measure_("woad_stream_is_tty(stdout)", warm_iters, 1, run_stream_tty_stdout_, 1);
    measure_("woad_fd_is_tty(1)", warm_iters, 1, run_fd_tty_, 1);
#ifdef _WIN32

    measure_("woad_console_handle_is_tty(stdout)", warm_iters, 1, run_handle_tty_, 1);
    measure_("woad_std_handle_is_tty(STD_OUTPUT_HANDLE)", warm_iters, 1, run_std_handle_tty_, 1);
#endif

    begin_group_("colour-for, warm");
    use_policy_("always");
    measure_("WOAD_SEQ_FOR(file), force", warm_iters, 1, run_seq_file_, 1);
    use_policy_("never");
    measure_("WOAD_SEQ_FOR(file), inhibit", warm_iters, 1, run_seq_file_, 1);
    use_policy_("auto");
    measure_("WOAD_SEQ_FOR(file), auto", warm_iters, 1, run_seq_file_, 1);
    measure_("WOAD_SEQ_FOR(stdout), auto", warm_iters, 1, run_seq_stdout_, 1);
    measure_("WOAD_SEQ_FOR(fd 1), auto", warm_iters, 1, run_seq_fd_, 1);
    measure_("FG_GREEN_FOR + RESET_FOR (stdout), auto", warm_iters, 2, run_line_stdout_, 1);

#ifdef _WIN32

    begin_group_("Windows currencies, warm");
    use_policy_("always");
    measure_("WOAD_SEQ_FOR(handle), force", warm_iters, 1, run_seq_handle_, 1);
    use_policy_("auto");
    measure_("WOAD_SEQ_FOR(handle), auto", warm_iters, 1, run_seq_handle_, 1);
    use_policy_("always");
    measure_("WOAD_SEQ_FOR(std-handle), force", warm_iters, 1, run_seq_std_handle_, 1);
    use_policy_("auto");
    measure_("WOAD_SEQ_FOR(std-handle), auto", warm_iters, 1, run_seq_std_handle_, 1);
#endif

    begin_group_("cached determinations");
    use_policy_("auto");
    measure_("woad_console_supports_ansi, warm", warm_iters, 1, run_console_warm_, 1);
    measure_("woad_console_supports_ansi, cold", cold_console_iters, 1, run_console_cold_, 0);
    use_policy_("always");
    measure_("WOAD_SEQ_FOR(file), force, cold environment", cold_env_iters, 1, run_env_cold_, 0);

    fclose(s_file);

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

