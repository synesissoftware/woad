/* /////////////////////////////////////////////////////////////////////////
 * File:    test/unit/test.unit.version.c
 *
 * Purpose: Unit tests for woad version macros.
 *
 * Created: 15th August 2026
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


int main(void)
{
    unsigned long const expected_ver = (0ul
        | (0ul << 24)
        | (0ul << 16)
        | (1ul <<  8)
        | (0x42ul << 0)
    );

    ASSERT_TRUE_(0 == WOAD_VER_MAJOR);
    ASSERT_TRUE_(0 == WOAD_VER_MINOR);
    ASSERT_TRUE_(1 == WOAD_VER_PATCH);
    ASSERT_TRUE_(0x42 == WOAD_VER_ALPHABETA);
    ASSERT_TRUE_(WOAD_VER_REVISION == WOAD_VER_PATCH);
    ASSERT_TRUE_((unsigned long)WOAD_VER == expected_ver);
    ASSERT_TRUE_(0 == strcmp(WOAD_VER_STRING, "0.0.1"));

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

