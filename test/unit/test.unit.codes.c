/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.codes.c
 *
 * Purpose: Unit tests for woad SGR colour codes.
 *
 * Created: 16th August 2026
 * Updated: 16th August 2026
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
#include <string.h>


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


static int
is_csi_sgr_(
    char const* code
)
{
    size_t const n = strlen(code);

    if (n < 3)
    {
        return 0;
    }

    if (0 != strncmp(code, "\033[", 2))
    {
        return 0;
    }

    if ('m' != code[n - 1])
    {
        return 0;
    }

    return 1;
}


int main(void)
{
    struct code_entry_t
    {
        char const* name;
        char const* code;
    };

    struct code_entry_t const codes[] =
    {
        { "WOAD_RESET",             WOAD_RESET             },
        { "WOAD_FG_BLACK",          WOAD_FG_BLACK          },
        { "WOAD_FG_RED",            WOAD_FG_RED            },
        { "WOAD_FG_GREEN",          WOAD_FG_GREEN          },
        { "WOAD_FG_YELLOW",         WOAD_FG_YELLOW         },
        { "WOAD_FG_BLUE",           WOAD_FG_BLUE           },
        { "WOAD_FG_MAGENTA",        WOAD_FG_MAGENTA        },
        { "WOAD_FG_CYAN",           WOAD_FG_CYAN           },
        { "WOAD_FG_WHITE",          WOAD_FG_WHITE          },
        { "WOAD_FG_BRIGHT_BLACK",   WOAD_FG_BRIGHT_BLACK   },
        { "WOAD_FG_BRIGHT_RED",     WOAD_FG_BRIGHT_RED     },
        { "WOAD_FG_BRIGHT_GREEN",   WOAD_FG_BRIGHT_GREEN   },
        { "WOAD_FG_BRIGHT_YELLOW",  WOAD_FG_BRIGHT_YELLOW  },
        { "WOAD_FG_BRIGHT_BLUE",    WOAD_FG_BRIGHT_BLUE    },
        { "WOAD_FG_BRIGHT_MAGENTA", WOAD_FG_BRIGHT_MAGENTA },
        { "WOAD_FG_BRIGHT_CYAN",    WOAD_FG_BRIGHT_CYAN    },
        { "WOAD_FG_BRIGHT_WHITE",   WOAD_FG_BRIGHT_WHITE   },
        { "WOAD_BG_BLACK",          WOAD_BG_BLACK          },
        { "WOAD_BG_RED",            WOAD_BG_RED            },
        { "WOAD_BG_GREEN",          WOAD_BG_GREEN          },
        { "WOAD_BG_YELLOW",         WOAD_BG_YELLOW         },
        { "WOAD_BG_BLUE",           WOAD_BG_BLUE           },
        { "WOAD_BG_MAGENTA",        WOAD_BG_MAGENTA        },
        { "WOAD_BG_CYAN",           WOAD_BG_CYAN           },
        { "WOAD_BG_WHITE",          WOAD_BG_WHITE          },
        { "WOAD_BG_BRIGHT_BLACK",   WOAD_BG_BRIGHT_BLACK   },
        { "WOAD_BG_BRIGHT_RED",     WOAD_BG_BRIGHT_RED     },
        { "WOAD_BG_BRIGHT_GREEN",   WOAD_BG_BRIGHT_GREEN   },
        { "WOAD_BG_BRIGHT_YELLOW",  WOAD_BG_BRIGHT_YELLOW  },
        { "WOAD_BG_BRIGHT_BLUE",    WOAD_BG_BRIGHT_BLUE    },
        { "WOAD_BG_BRIGHT_MAGENTA", WOAD_BG_BRIGHT_MAGENTA },
        { "WOAD_BG_BRIGHT_CYAN",    WOAD_BG_BRIGHT_CYAN    },
        { "WOAD_BG_BRIGHT_WHITE",   WOAD_BG_BRIGHT_WHITE   },
    };
    size_t i;
    char const* const concatenated = WOAD_FG_GREEN "ok" WOAD_RESET;

    ASSERT_TRUE_(0 == strcmp(WOAD_RESET, "\033[0m"));
    ASSERT_TRUE_(0 == strcmp(WOAD_FG_RED, "\033[31m"));
    ASSERT_TRUE_(0 == strcmp(WOAD_BG_BLUE, "\033[44m"));

    for (i = 0; i != sizeof(codes) / sizeof(codes[0]); ++i)
    {
        if (!is_csi_sgr_(codes[i].code))
        {
            fprintf(
                stderr
            ,   "%s:%d: assertion failed: %s is not a CSI SGR sequence\n"
            ,   __FILE__
            ,   __LINE__
            ,   codes[i].name
            );

            return EXIT_FAILURE;
        }
    }

    ASSERT_TRUE_(NULL != strstr(concatenated, "ok"));
    ASSERT_TRUE_(concatenated == strstr(concatenated, WOAD_FG_GREEN));

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
