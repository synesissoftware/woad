/* /////////////////////////////////////////////////////////////////////////
 * File:    woad/woad.h
 *
 * Purpose: Root header for woad (C-API). Provides library version macros
 *          and fixed ANSI SGR colour sequences.
 *
 * Created: 15th August 2026
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


/** \file woad/woad.h
 *
 * [C, C++] Root header for woad (C-API).
 */

#ifndef WOAD_INCL_WOAD_H_WOAD
#define WOAD_INCL_WOAD_H_WOAD

#ifndef WOAD_DOCUMENTATION_SKIP_SECTION
# define WOAD_VER_WOAD_H_WOAD_MAJOR                     0
# define WOAD_VER_WOAD_H_WOAD_MINOR                     0
# define WOAD_VER_WOAD_H_WOAD_PATCH                     2
# define WOAD_VER_WOAD_H_WOAD_EDIT                      3
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
#define WOAD_VER_ALPHABETA  0x42

#define WOAD_VER \
    (0\
        |   (   WOAD_VER_MAJOR       << 24   ) \
        |   (   WOAD_VER_MINOR       << 16   ) \
        |   (   WOAD_VER_PATCH       <<  8   ) \
        |   (   WOAD_VER_ALPHABETA   <<  0   ) \
    )

#define WOAD_VER_REVISION                                   WOAD_VER_PATCH

#define WOAD_VER_STRINGIZE_(M, m, p)                        #M "." #m "." #p
#define WOAD_VER_STRINGIZE(M, m, p)                         WOAD_VER_STRINGIZE_(M, m, p)
#define WOAD_VER_STRING                                     WOAD_VER_STRINGIZE(WOAD_VER_MAJOR, WOAD_VER_MINOR, WOAD_VER_PATCH)


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
 * language
 */

#ifdef __cplusplus
extern "C"
{
#endif


/* /////////////////////////////////////////////////////////////////////////
 * API
 */



/* /////////////////////////////////////////////////////////////////////////
 * language
 */

#ifdef __cplusplus
} /* extern "C" */
#endif


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#ifdef STLSOFT_PPF_pragma_once_SUPPORT
# pragma once
#endif /* STLSOFT_PPF_pragma_once_SUPPORT */

#endif /* !WOAD_INCL_WOAD_H_WOAD */

/* ///////////////////////////// end of file //////////////////////////// */

