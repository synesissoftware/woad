/* /////////////////////////////////////////////////////////////////////////
 * File:    woad/woad.h
 *
 * Purpose: Root header for woad (C-API). Provides fixed ANSI SGR colour
 *          sequence macros (usable without binding to the library),
 *          stream-conditional counterparts, and the library version and
 *          colour-policy API.
 *
 * Created: 15th August 2026
 * Updated: 12th September 2026
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


/** \file woad/woad.h
 *
 * [C, C++] Root header for woad (C-API).
 */

#ifndef WOAD_INCL_WOAD_H_WOAD
#define WOAD_INCL_WOAD_H_WOAD

#ifndef WOAD_DOCUMENTATION_SKIP_SECTION
# define WOAD_VER_WOAD_H_WOAD_MAJOR     0
# define WOAD_VER_WOAD_H_WOAD_MINOR     0
# define WOAD_VER_WOAD_H_WOAD_PATCH     4
# define WOAD_VER_WOAD_H_WOAD_EDIT      6
#endif /* !WOAD_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * version information
 */

/**
 * \def WOAD_VER_MAJOR
 * The major version number of the **woad** library
 *
 * \def WOAD_VER_MINOR
 * The minor version number of the **woad** library
 *
 * \def WOAD_VER_PATCH
 * The patch number of the **woad** library
 *
 * \def WOAD_VER_ALPHABETA
 * Pre-release marker in the low byte of WOAD_VER. `0xFF` for a numbered
 * (non-alpha, non-beta, non-rc) release.
 *
 * \def WOAD_VER
 * The composite version of the **woad** library
 *
 * \def WOAD_VER_STRING
 * The dotted version string of the **woad** library
 */

#define WOAD_VER_MAJOR      0
#define WOAD_VER_MINOR      0
#define WOAD_VER_PATCH      1
#define WOAD_VER_ALPHABETA  0x81

#define WOAD_VER \
    (0\
        |   (   WOAD_VER_MAJOR       << 24   ) \
        |   (   WOAD_VER_MINOR       << 16   ) \
        |   (   WOAD_VER_PATCH       <<  8   ) \
        |   (   WOAD_VER_ALPHABETA   <<  0   ) \
    )

#define WOAD_VER_REVISION                                   WOAD_VER_PATCH

#define WOAD_VER_STRINGIZE_(j, n, p)                        #j "." #n "." #p
#define WOAD_VER_STRINGIZE(j, n, p)                         WOAD_VER_STRINGIZE_(j, n, p)
#define WOAD_VER_STRING                                     WOAD_VER_STRINGIZE(WOAD_VER_MAJOR, WOAD_VER_MINOR, WOAD_VER_PATCH)


/* /////////////////////////////////////////////////////////////////////////
 * includes
 *
 * NOTE: <stdio.h> is the *only* inclusion, and is required solely for
 * `FILE`, which is the primary unit of currency of the stream-conditional
 * part of the API. In particular, this header never includes <windows.h>,
 * <io.h>, or <unistd.h>: the Windows handle currencies are expressed as
 * `void*` and `unsigned long` precisely so that no client translation unit
 * inherits operating-system headers (or their macro pollution) merely by
 * using woad.
 *
 * <stdio.h> is used (rather than <cstdio>) even in C++, so that `FILE` is
 * guaranteed to be available in the global namespace.
 */

#include <stdio.h>


/* /////////////////////////////////////////////////////////////////////////
 * SGR colour codes
 *
 * Fixed ANSI SGR sequences. These are always the colour codes; they do
 * not inspect TTY state or Windows console mode.
 */

/**
 * \def WOAD_RESET
 * Reset all attributes.
 */

#define WOAD_RESET                                          "\033[0m"

/**
 * \def WOAD_FG_BLACK
 * Foreground black.
 *
 * \def WOAD_FG_RED
 * Foreground red.
 *
 * \def WOAD_FG_GREEN
 * Foreground green.
 *
 * \def WOAD_FG_YELLOW
 * Foreground yellow.
 *
 * \def WOAD_FG_BLUE
 * Foreground blue.
 *
 * \def WOAD_FG_MAGENTA
 * Foreground magenta.
 *
 * \def WOAD_FG_CYAN
 * Foreground cyan.
 *
 * \def WOAD_FG_WHITE
 * Foreground white.
 */

#define WOAD_FG_BLACK                                       "\033[30m"
#define WOAD_FG_RED                                         "\033[31m"
#define WOAD_FG_GREEN                                       "\033[32m"
#define WOAD_FG_YELLOW                                      "\033[33m"
#define WOAD_FG_BLUE                                        "\033[34m"
#define WOAD_FG_MAGENTA                                     "\033[35m"
#define WOAD_FG_CYAN                                        "\033[36m"
#define WOAD_FG_WHITE                                       "\033[37m"

/**
 * \def WOAD_FG_BRIGHT_BLACK
 * Foreground bright black.
 *
 * \def WOAD_FG_BRIGHT_RED
 * Foreground bright red.
 *
 * \def WOAD_FG_BRIGHT_GREEN
 * Foreground bright green.
 *
 * \def WOAD_FG_BRIGHT_YELLOW
 * Foreground bright yellow.
 *
 * \def WOAD_FG_BRIGHT_BLUE
 * Foreground bright blue.
 *
 * \def WOAD_FG_BRIGHT_MAGENTA
 * Foreground bright magenta.
 *
 * \def WOAD_FG_BRIGHT_CYAN
 * Foreground bright cyan.
 *
 * \def WOAD_FG_BRIGHT_WHITE
 * Foreground bright white.
 */

#define WOAD_FG_BRIGHT_BLACK                                "\033[90m"
#define WOAD_FG_BRIGHT_RED                                  "\033[91m"
#define WOAD_FG_BRIGHT_GREEN                                "\033[92m"
#define WOAD_FG_BRIGHT_YELLOW                               "\033[93m"
#define WOAD_FG_BRIGHT_BLUE                                 "\033[94m"
#define WOAD_FG_BRIGHT_MAGENTA                              "\033[95m"
#define WOAD_FG_BRIGHT_CYAN                                 "\033[96m"
#define WOAD_FG_BRIGHT_WHITE                                "\033[97m"

/**
 * \def WOAD_BG_BLACK
 * Background black.
 *
 * \def WOAD_BG_RED
 * Background red.
 *
 * \def WOAD_BG_GREEN
 * Background green.
 *
 * \def WOAD_BG_YELLOW
 * Background yellow.
 *
 * \def WOAD_BG_BLUE
 * Background blue.
 *
 * \def WOAD_BG_MAGENTA
 * Background magenta.
 *
 * \def WOAD_BG_CYAN
 * Background cyan.
 *
 * \def WOAD_BG_WHITE
 * Background white.
 */

#define WOAD_BG_BLACK                                       "\033[40m"
#define WOAD_BG_RED                                         "\033[41m"
#define WOAD_BG_GREEN                                       "\033[42m"
#define WOAD_BG_YELLOW                                      "\033[43m"
#define WOAD_BG_BLUE                                        "\033[44m"
#define WOAD_BG_MAGENTA                                     "\033[45m"
#define WOAD_BG_CYAN                                        "\033[46m"
#define WOAD_BG_WHITE                                       "\033[47m"

/**
 * \def WOAD_BG_BRIGHT_BLACK
 * Background bright black.
 *
 * \def WOAD_BG_BRIGHT_RED
 * Background bright red.
 *
 * \def WOAD_BG_BRIGHT_GREEN
 * Background bright green.
 *
 * \def WOAD_BG_BRIGHT_YELLOW
 * Background bright yellow.
 *
 * \def WOAD_BG_BRIGHT_BLUE
 * Background bright blue.
 *
 * \def WOAD_BG_BRIGHT_MAGENTA
 * Background bright magenta.
 *
 * \def WOAD_BG_BRIGHT_CYAN
 * Background bright cyan.
 *
 * \def WOAD_BG_BRIGHT_WHITE
 * Background bright white.
 */

#define WOAD_BG_BRIGHT_BLACK                                "\033[100m"
#define WOAD_BG_BRIGHT_RED                                  "\033[101m"
#define WOAD_BG_BRIGHT_GREEN                                "\033[102m"
#define WOAD_BG_BRIGHT_YELLOW                               "\033[103m"
#define WOAD_BG_BRIGHT_BLUE                                 "\033[104m"
#define WOAD_BG_BRIGHT_MAGENTA                              "\033[105m"
#define WOAD_BG_BRIGHT_CYAN                                 "\033[106m"
#define WOAD_BG_BRIGHT_WHITE                                "\033[107m"


/* /////////////////////////////////////////////////////////////////////////
 * implementation constructs
 */

#ifndef WOAD_DOCUMENTATION_SKIP_SECTION

# ifndef WOAD_CALL
#  ifdef __cplusplus
#   define WOAD_CALL(rt)                                    extern "C" rt
#  else

#   define WOAD_CALL(rt)                                    rt
#  endif
# endif /* !WOAD_CALL */

/* Type-dispatched wrappers are available in C++ (by overloading) and in C11
 * and later (by `_Generic`). In earlier C dialects the wrappers accept
 * `FILE*` only, and the other stream currencies must be addressed by naming
 * the specific function.
 */
# if 1 && \
     !defined(__cplusplus) && \
     defined(__STDC_VERSION__) && \
     __STDC_VERSION__ >= 201112L && \
     1

#  define WOAD_HAS_C_GENERIC_
# endif
#endif /* !WOAD_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * colour policy
 */

/**
 * \def WOAD_COLOUR_POLICY_AUTO
 * Colour is selected per-stream, according to whether the stream is
 * attached to a terminal that is known to understand ANSI SGR sequences.
 *
 * \def WOAD_COLOUR_POLICY_FORCE
 * Colour is emitted for every stream, regardless of terminal attachment.
 *
 * \def WOAD_COLOUR_POLICY_INHIBIT
 * Colour is emitted for no stream, regardless of terminal attachment.
 */

#define WOAD_COLOUR_POLICY_AUTO                             (0)
#define WOAD_COLOUR_POLICY_FORCE                            (1)
#define WOAD_COLOUR_POLICY_INHIBIT                          (2)


/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
extern "C"
{
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

/* /////////////////////////////////////////////////////////////////
 * API functions - version
 */

/** Obtains the **woad** version, as the composite value WOAD_VER.
 *
 * \return The composite version, in which the major version occupies bits
 *   24-31, the minor version bits 16-23, the patch bits 8-15, and the
 *   pre-release marker bits 0-7.
 *
 * \note Unlike WOAD_VER, which is the version of the header against which
 *   the calling translation unit was compiled, this reports the version of
 *   the library implementation to which the caller is bound, and is
 *   therefore usable as an ABI probe.
 */
WOAD_CALL(unsigned long)
woad_version(void);

/** Obtains the **woad** version, as a dotted version string.
 *
 * \return A nul-terminated string of the form `"M.m.p"`. Never NULL, and
 *   never requires release by the caller.
 *
 * \note See woad_version() regarding header version vs library version.
 */
WOAD_CALL(char const*)
woad_version_string(void);


/* /////////////////////////////////////////////////////////////////
 * API functions - policy
 */

/** Obtains the prevailing colour policy, as determined from the
 * environment.
 *
 * The environment is examined once, upon first need, and the result is
 * cached for the lifetime of the process (see woad_refresh()). The
 * following variables are consulted, in the following order of precedence:
 *
 * 1. `WOAD_COLOUR`, or, if that is unset, `WOAD_COLOR` - a truthy value
 *    (`"1"`, `"always"`, `"force"`, `"on"`, `"true"`, `"yes"`) forces
 *    colour; a falsey value (`"0"`, `"never"`, `"none"`, `"off"`,
 *    `"false"`, `"no"`) inhibits it; `"auto"` (or any unrecognised value)
 *    selects WOAD_COLOUR_POLICY_AUTO. Comparison is case-insensitive;
 * 2. `NO_COLOR` - set and non-empty inhibits colour (see
 *    https://no-color.org/);
 * 3. `CLICOLOR_FORCE` - set, non-empty, and not `"0"` forces colour;
 * 4. `CLICOLOR` - set to `"0"` inhibits colour;
 *
 * \return One of WOAD_COLOUR_POLICY_AUTO, WOAD_COLOUR_POLICY_FORCE, or
 *   WOAD_COLOUR_POLICY_INHIBIT.
 */
WOAD_CALL(int)
woad_colour_policy(void);

/** Discards all cached determinations - colour policy, `TERM` inspection,
 * and (on Windows) console capability - so that the next query
 * re-evaluates them.
 *
 * \note This exists for the benefit of test programs, and of the rare
 *   client that manipulates its own environment at runtime. It is not
 *   necessary in normal use, and it is not safe to call concurrently with
 *   other **woad** functions.
 */
WOAD_CALL(void)
woad_refresh(void);

/** Determines whether the attached console is known to interpret ANSI SGR
 * sequences.
 *
 * On Windows, this inspects the operating-system build number and, for
 * builds that require it, attempts to enable virtual-terminal processing on
 * the standard output and error handles. That determination - and the
 * `SetConsoleMode()` side effect it entails - is performed exactly once per
 * loaded copy of the library, however many times this is called and from
 * however many threads. On all other platforms it returns 1.
 *
 * The `SetConsoleMode()` call may be withheld by setting
 * `WOAD_SET_CONSOLE_MODE` to a falsey value (`"0"`, `"never"`, `"none"`,
 * `"off"`, `"false"`, `"no"`), for the caller who requires that no library
 * alter the console it has configured. The determination then degenerates
 * to a pure query: a console upon which virtual-terminal processing is
 * already enabled is still reported as capable, and one upon which it is
 * not is reported as incapable rather than being modified. Absence of the
 * variable grants permission, as does any value that is not falsey. The
 * variable is consulted only on Windows, and only on those builds for
 * which the modification would otherwise be attempted.
 *
 * \return Non-zero if ANSI SGR sequences may be emitted; 0 otherwise.
 */
WOAD_CALL(int)
woad_console_supports_ansi(void);


/* /////////////////////////////////////////////////////////////////
 * API functions - stream inspection
 */

/** Determines whether the given stream is attached to a terminal.
 *
 * \param stm The stream to be tested. May be NULL, in which case 0 is
 *   returned;
 *
 * \return Non-zero if the stream is attached to a terminal; 0 otherwise.
 *
 * \note This exists so that client code need never choose between
 *   `isatty()`, `_isatty()`, `fileno()`, and the various feature-test
 *   macros that determine which of them are visible.
 */
WOAD_CALL(int)
woad_stream_is_tty(
    FILE*           stm
);

/** Determines whether the given file descriptor is attached to a terminal.
 *
 * \param fd The file descriptor to be tested, e.g. 1 for standard output,
 *   2 for standard error;
 *
 * \return Non-zero if the descriptor is attached to a terminal; 0
 *   otherwise.
 */
WOAD_CALL(int)
woad_fd_is_tty(
    int             fd
);

/** Determines whether colour should be emitted for the given stream,
 * combining the prevailing colour policy, the terminal attachment of the
 * stream, and the console's ANSI capability.
 *
 * \param stm The stream to be tested. May be NULL, in which case the
 *   result is that of the policy alone;
 *
 * \return Non-zero if colour should be emitted; 0 otherwise.
 */
WOAD_CALL(int)
woad_stream_supports_colour(
    FILE*           stm
);

/** Descriptor-based equivalent of woad_stream_supports_colour().
 *
 * \param fd The file descriptor to be tested;
 *
 * \return Non-zero if colour should be emitted; 0 otherwise.
 */
WOAD_CALL(int)
woad_fd_supports_colour(
    int             fd
);

#if defined(_WIN32) || \
    defined(WOAD_DOCUMENTATION_SKIP_SECTION)

/** [Windows-only] Determines whether the given console handle is attached
 * to a terminal.
 *
 * \param h The handle to be tested, as obtained from, e.g.,
 *   `GetStdHandle()`. Declared as `void*` rather than `HANDLE` so that
 *   this header need not include <windows.h>;
 *
 * \return Non-zero if the handle refers to a console; 0 otherwise.
 */
WOAD_CALL(int)
woad_console_handle_is_tty(
    void*           h
);

/** [Windows-only] Determines whether the identified standard handle is
 * attached to a terminal.
 *
 * \param id The standard-handle identifier, e.g. `STD_OUTPUT_HANDLE` or
 *   `STD_ERROR_HANDLE`. Declared as `unsigned long` rather than `DWORD` so
 *   that this header need not include <windows.h>;
 *
 * \return Non-zero if the handle refers to a console; 0 otherwise.
 */
WOAD_CALL(int)
woad_std_handle_is_tty(
    unsigned long   id
);

/** [Windows-only] Handle-based equivalent of
 * woad_stream_supports_colour().
 */
WOAD_CALL(int)
woad_console_handle_supports_colour(
    void*           h
);

/** [Windows-only] Standard-handle-based equivalent of
 * woad_stream_supports_colour().
 */
WOAD_CALL(int)
woad_std_handle_supports_colour(
    unsigned long   id
);
#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////
 * API functions - conditional sequences
 */

/** Obtains the given SGR sequence if colour should be emitted for the
 * given stream, or the empty string if not.
 *
 * \param seq The SGR sequence, e.g. WOAD_FG_RED. May be NULL, which is
 *   treated as the empty string;
 * \param stm The stream for which the sequence is intended. May be NULL;
 *
 * \return Either \c seq or `""`. Never NULL, and never requires release by
 *   the caller: the returned pointer is either the caller's own argument or
 *   a string literal, and so is valid for as long as \c seq is.
 */
WOAD_CALL(char const*)
woad_seq_for_stream(
    char const*     seq
,   FILE*           stm
);

/** Descriptor-based equivalent of woad_seq_for_stream(). */
WOAD_CALL(char const*)
woad_seq_for_fd(
    char const*     seq
,   int             fd
);

#if defined(_WIN32) || \
    defined(WOAD_DOCUMENTATION_SKIP_SECTION)

/** [Windows-only] Handle-based equivalent of woad_seq_for_stream(). */
WOAD_CALL(char const*)
woad_seq_for_handle(
    char const*     seq
,   void*           h
);

/** [Windows-only] Standard-handle-based equivalent of
 * woad_seq_for_stream().
 */
WOAD_CALL(char const*)
woad_seq_for_std_handle(
    char const*     seq
,   unsigned long   id
);
#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * type-dispatched wrappers
 *
 * `woad_seq_for()` and `woad_supports_colour()` accept any of the stream
 * currencies - a `FILE*`, a file descriptor, and, on Windows, a console
 * handle or a standard-handle identifier - so that client code need not
 * select the right function name for the currency it happens to hold.
 *
 * In C++ this is achieved by overloading; in C11 and later by `_Generic`;
 * in earlier C dialects the wrappers are `FILE*`-only, and the other
 * currencies remain available under their specific names.
 */

#ifdef __cplusplus

inline
char const*
woad_seq_for(
    char const*     seq
,   FILE*           stm
)
{
    return woad_seq_for_stream(seq, stm);
}

inline
char const*
woad_seq_for(
    char const*     seq
,   int             fd
)
{
    return woad_seq_for_fd(seq, fd);
}

inline
int
woad_supports_colour(
    FILE*           stm
)
{
    return woad_stream_supports_colour(stm);
}

inline
int
woad_supports_colour(
    int             fd
)
{
    return woad_fd_supports_colour(fd);
}

# ifdef _WIN32

inline
char const*
woad_seq_for(
    char const*     seq
,   void*           h
)
{
    return woad_seq_for_handle(seq, h);
}

inline
char const*
woad_seq_for(
    char const*     seq
,   unsigned long   id
)
{
    return woad_seq_for_std_handle(seq, id);
}

inline
int
woad_supports_colour(
    void*           h
)
{
    return woad_console_handle_supports_colour(h);
}

inline
int
woad_supports_colour(
    unsigned long   id
)
{
    return woad_std_handle_supports_colour(id);
}
# endif /* _WIN32 */
#elif defined(WOAD_HAS_C_GENERIC_)

# ifdef _WIN32

#  define woad_seq_for(seq, stm)                            \
                                                            \
    (_Generic((stm)                                         \
    ,   FILE*           :   woad_seq_for_stream             \
    ,   int             :   woad_seq_for_fd                 \
    ,   void*           :   woad_seq_for_handle             \
    ,   unsigned long   :   woad_seq_for_std_handle         \
    )((seq), (stm)))

#  define woad_supports_colour(stm)                             \
                                                                \
    (_Generic((stm)                                             \
    ,   FILE*           :   woad_stream_supports_colour         \
    ,   int             :   woad_fd_supports_colour             \
    ,   void*           :   woad_console_handle_supports_colour \
    ,   unsigned long   :   woad_std_handle_supports_colour     \
    )(stm))
# else

#  define woad_seq_for(seq, stm)                            \
                                                            \
    (_Generic((stm)                                         \
    ,   FILE*           :   woad_seq_for_stream             \
    ,   int             :   woad_seq_for_fd                 \
    )((seq), (stm)))

#  define woad_supports_colour(stm)                         \
                                                            \
    (_Generic((stm)                                         \
    ,   FILE*           :   woad_stream_supports_colour     \
    ,   int             :   woad_fd_supports_colour         \
    )(stm))
# endif /* _WIN32 */
#else /* ? dispatch */

# define woad_seq_for(seq, stm)                             woad_seq_for_stream((seq), (stm))
# define woad_supports_colour(stm)                          woad_stream_supports_colour(stm)
#endif /* dispatch */


/* /////////////////////////////////////////////////////////////////////////
 * conditional SGR colour codes
 *
 * Function-like counterparts of the object-like macros above. Each yields
 * the sequence when colour is appropriate for the nominated stream, and the
 * empty string when it is not, and so must be used as a `%s` argument
 * rather than by string-literal concatenation.
 */

/**
 * \def WOAD_SEQ_FOR(seq, stm)
 * Yields \c seq for \c stm, or `""`. The general form, of which all the
 * macros below are conveniences.
 *
 * \def WOAD_FG_FOR(clr, stm)
 * Yields the foreground sequence \c clr for \c stm, or `""`.
 *
 * \def WOAD_BG_FOR(clr, stm)
 * Yields the background sequence \c clr for \c stm, or `""`.
 *
 * \def WOAD_RESET_FOR(stm)
 * Yields WOAD_RESET for \c stm, or `""`.
 *
 * \note WOAD_FG_FOR() and WOAD_BG_FOR() are synonyms of WOAD_SEQ_FOR():
 *   since the sequence passed already carries its own SGR parameter, the
 *   distinction is one of expressing intent at the call site, not of
 *   behaviour.
 */

#define WOAD_SEQ_FOR(seq, stm)                              woad_seq_for((seq), (stm))
#define WOAD_FG_FOR(clr, stm)                               WOAD_SEQ_FOR(clr, stm)
#define WOAD_BG_FOR(clr, stm)                               WOAD_SEQ_FOR(clr, stm)
#define WOAD_RESET_FOR(stm)                                 WOAD_SEQ_FOR(WOAD_RESET, stm)

/**
 * \def WOAD_FG_BLACK_FOR
 * Yields WOAD_FG_BLACK for the given stream, or `""`.
 *
 * \def WOAD_FG_RED_FOR
 * Yields WOAD_FG_RED for the given stream, or `""`.
 *
 * \def WOAD_FG_GREEN_FOR
 * Yields WOAD_FG_GREEN for the given stream, or `""`.
 *
 * \def WOAD_FG_YELLOW_FOR
 * Yields WOAD_FG_YELLOW for the given stream, or `""`.
 *
 * \def WOAD_FG_BLUE_FOR
 * Yields WOAD_FG_BLUE for the given stream, or `""`.
 *
 * \def WOAD_FG_MAGENTA_FOR
 * Yields WOAD_FG_MAGENTA for the given stream, or `""`.
 *
 * \def WOAD_FG_CYAN_FOR
 * Yields WOAD_FG_CYAN for the given stream, or `""`.
 *
 * \def WOAD_FG_WHITE_FOR
 * Yields WOAD_FG_WHITE for the given stream, or `""`.
 */

#define WOAD_FG_BLACK_FOR(stm)                              WOAD_SEQ_FOR(WOAD_FG_BLACK, stm)
#define WOAD_FG_RED_FOR(stm)                                WOAD_SEQ_FOR(WOAD_FG_RED, stm)
#define WOAD_FG_GREEN_FOR(stm)                              WOAD_SEQ_FOR(WOAD_FG_GREEN, stm)
#define WOAD_FG_YELLOW_FOR(stm)                             WOAD_SEQ_FOR(WOAD_FG_YELLOW, stm)
#define WOAD_FG_BLUE_FOR(stm)                               WOAD_SEQ_FOR(WOAD_FG_BLUE, stm)
#define WOAD_FG_MAGENTA_FOR(stm)                            WOAD_SEQ_FOR(WOAD_FG_MAGENTA, stm)
#define WOAD_FG_CYAN_FOR(stm)                               WOAD_SEQ_FOR(WOAD_FG_CYAN, stm)
#define WOAD_FG_WHITE_FOR(stm)                              WOAD_SEQ_FOR(WOAD_FG_WHITE, stm)

/**
 * \def WOAD_FG_BRIGHT_BLACK_FOR
 * Yields WOAD_FG_BRIGHT_BLACK for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_RED_FOR
 * Yields WOAD_FG_BRIGHT_RED for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_GREEN_FOR
 * Yields WOAD_FG_BRIGHT_GREEN for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_YELLOW_FOR
 * Yields WOAD_FG_BRIGHT_YELLOW for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_BLUE_FOR
 * Yields WOAD_FG_BRIGHT_BLUE for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_MAGENTA_FOR
 * Yields WOAD_FG_BRIGHT_MAGENTA for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_CYAN_FOR
 * Yields WOAD_FG_BRIGHT_CYAN for the given stream, or `""`.
 *
 * \def WOAD_FG_BRIGHT_WHITE_FOR
 * Yields WOAD_FG_BRIGHT_WHITE for the given stream, or `""`.
 */

#define WOAD_FG_BRIGHT_BLACK_FOR(stm)                       WOAD_SEQ_FOR(WOAD_FG_BRIGHT_BLACK, stm)
#define WOAD_FG_BRIGHT_RED_FOR(stm)                         WOAD_SEQ_FOR(WOAD_FG_BRIGHT_RED, stm)
#define WOAD_FG_BRIGHT_GREEN_FOR(stm)                       WOAD_SEQ_FOR(WOAD_FG_BRIGHT_GREEN, stm)
#define WOAD_FG_BRIGHT_YELLOW_FOR(stm)                      WOAD_SEQ_FOR(WOAD_FG_BRIGHT_YELLOW, stm)
#define WOAD_FG_BRIGHT_BLUE_FOR(stm)                        WOAD_SEQ_FOR(WOAD_FG_BRIGHT_BLUE, stm)
#define WOAD_FG_BRIGHT_MAGENTA_FOR(stm)                     WOAD_SEQ_FOR(WOAD_FG_BRIGHT_MAGENTA, stm)
#define WOAD_FG_BRIGHT_CYAN_FOR(stm)                        WOAD_SEQ_FOR(WOAD_FG_BRIGHT_CYAN, stm)
#define WOAD_FG_BRIGHT_WHITE_FOR(stm)                       WOAD_SEQ_FOR(WOAD_FG_BRIGHT_WHITE, stm)

/**
 * \def WOAD_BG_BLACK_FOR
 * Yields WOAD_BG_BLACK for the given stream, or `""`.
 *
 * \def WOAD_BG_RED_FOR
 * Yields WOAD_BG_RED for the given stream, or `""`.
 *
 * \def WOAD_BG_GREEN_FOR
 * Yields WOAD_BG_GREEN for the given stream, or `""`.
 *
 * \def WOAD_BG_YELLOW_FOR
 * Yields WOAD_BG_YELLOW for the given stream, or `""`.
 *
 * \def WOAD_BG_BLUE_FOR
 * Yields WOAD_BG_BLUE for the given stream, or `""`.
 *
 * \def WOAD_BG_MAGENTA_FOR
 * Yields WOAD_BG_MAGENTA for the given stream, or `""`.
 *
 * \def WOAD_BG_CYAN_FOR
 * Yields WOAD_BG_CYAN for the given stream, or `""`.
 *
 * \def WOAD_BG_WHITE_FOR
 * Yields WOAD_BG_WHITE for the given stream, or `""`.
 */

#define WOAD_BG_BLACK_FOR(stm)                              WOAD_SEQ_FOR(WOAD_BG_BLACK, stm)
#define WOAD_BG_RED_FOR(stm)                                WOAD_SEQ_FOR(WOAD_BG_RED, stm)
#define WOAD_BG_GREEN_FOR(stm)                              WOAD_SEQ_FOR(WOAD_BG_GREEN, stm)
#define WOAD_BG_YELLOW_FOR(stm)                             WOAD_SEQ_FOR(WOAD_BG_YELLOW, stm)
#define WOAD_BG_BLUE_FOR(stm)                               WOAD_SEQ_FOR(WOAD_BG_BLUE, stm)
#define WOAD_BG_MAGENTA_FOR(stm)                            WOAD_SEQ_FOR(WOAD_BG_MAGENTA, stm)
#define WOAD_BG_CYAN_FOR(stm)                               WOAD_SEQ_FOR(WOAD_BG_CYAN, stm)
#define WOAD_BG_WHITE_FOR(stm)                              WOAD_SEQ_FOR(WOAD_BG_WHITE, stm)

/**
 * \def WOAD_BG_BRIGHT_BLACK_FOR
 * Yields WOAD_BG_BRIGHT_BLACK for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_RED_FOR
 * Yields WOAD_BG_BRIGHT_RED for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_GREEN_FOR
 * Yields WOAD_BG_BRIGHT_GREEN for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_YELLOW_FOR
 * Yields WOAD_BG_BRIGHT_YELLOW for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_BLUE_FOR
 * Yields WOAD_BG_BRIGHT_BLUE for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_MAGENTA_FOR
 * Yields WOAD_BG_BRIGHT_MAGENTA for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_CYAN_FOR
 * Yields WOAD_BG_BRIGHT_CYAN for the given stream, or `""`.
 *
 * \def WOAD_BG_BRIGHT_WHITE_FOR
 * Yields WOAD_BG_BRIGHT_WHITE for the given stream, or `""`.
 */

#define WOAD_BG_BRIGHT_BLACK_FOR(stm)                       WOAD_SEQ_FOR(WOAD_BG_BRIGHT_BLACK, stm)
#define WOAD_BG_BRIGHT_RED_FOR(stm)                         WOAD_SEQ_FOR(WOAD_BG_BRIGHT_RED, stm)
#define WOAD_BG_BRIGHT_GREEN_FOR(stm)                       WOAD_SEQ_FOR(WOAD_BG_BRIGHT_GREEN, stm)
#define WOAD_BG_BRIGHT_YELLOW_FOR(stm)                      WOAD_SEQ_FOR(WOAD_BG_BRIGHT_YELLOW, stm)
#define WOAD_BG_BRIGHT_BLUE_FOR(stm)                        WOAD_SEQ_FOR(WOAD_BG_BRIGHT_BLUE, stm)
#define WOAD_BG_BRIGHT_MAGENTA_FOR(stm)                     WOAD_SEQ_FOR(WOAD_BG_BRIGHT_MAGENTA, stm)
#define WOAD_BG_BRIGHT_CYAN_FOR(stm)                        WOAD_SEQ_FOR(WOAD_BG_BRIGHT_CYAN, stm)
#define WOAD_BG_BRIGHT_WHITE_FOR(stm)                       WOAD_SEQ_FOR(WOAD_BG_BRIGHT_WHITE, stm)


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#ifdef STLSOFT_PPF_pragma_once_SUPPORT
# pragma once
#endif /* STLSOFT_PPF_pragma_once_SUPPORT */

#endif /* !WOAD_INCL_WOAD_H_WOAD */

/* ///////////////////////////// end of file //////////////////////////// */

