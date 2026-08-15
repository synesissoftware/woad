/* /////////////////////////////////////////////////////////////////////////
 * File:    woad/woad.h
 *
 * Purpose: Root header for woad (C-API). This skeleton provides library
 *          version macros only.
 *
 * Created: 15th August 2026
 * Updated: 15th August 2026
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
# define WOAD_VER_WOAD_H_WOAD_PATCH                     1
# define WOAD_VER_WOAD_H_WOAD_EDIT                      1
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
#define WOAD_VER_PATCH      0
#define WOAD_VER_ALPHABETA  0xFF

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


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* !WOAD_INCL_WOAD_H_WOAD */

/* ///////////////////////////// end of file //////////////////////////// */

