/* /////////////////////////////////////////////////////////////////////////
 * File:    src/woad.windows_constructs.h
 *
 * Purpose: Windows API-specific constructs.
 *
 * Created: 4th November 2026
 * Updated: 4th November 2026
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


#ifndef _WOAD_INTERNAL_SRC_H_WOAD_WINDOWS_CONSTRUCTS_
#define _WOAD_INTERNAL_SRC_H_WOAD_WINDOWS_CONSTRUCTS_


/* /////////////////////////////////////////////////////////////////////////
 * compatiblity
 */

#ifndef _WIN32
# error requires Windows
#endif

#if !defined(_WIN32_WINNT) ||\
    _WIN32_WINNT < 0x0600
# undef _WIN32_WINNT
# define _WIN32_WINNT                                       0x0600
#endif

#if !defined(WINVER) ||\
    WINVER < 0x0600
# undef WINVER
# define WINVER                                             0x0600
#endif


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#include <io.h>
#include <windows.h>


/* /////////////////////////////////////////////////////////////////////////
 * macros
 */

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
# define ENABLE_VIRTUAL_TERMINAL_PROCESSING                 (0x0004)
#endif


/* /////////////////////////////////////////////////////////////////////////
 * compatiblity
 */

#if 1 &&\
    !defined(__CYGWIN__) &&\
    !defined(__MSYS__) &&\
    1

# define WOAD_PROBE_MINTTY_
#else

# ifdef WOAD_PROBE_MINTTY_
#  error WOAD_PROBE_MINTTY_ must not be defined
# endif
#endif

/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#endif /* !_WOAD_INTERNAL_SRC_H_WOAD_WINDOWS_CONSTRUCTS_ */

/* ///////////////////////////// end of file //////////////////////////// */

