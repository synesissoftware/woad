/* /////////////////////////////////////////////////////////////////////////
 * File:    src/woad.internal.mintty_pty.h
 *
 * Purpose: Recognise an MSYS2 / Cygwin (mintty, Git Bash) PTY pipe by the
 *          name Windows reports for it. Not a public header.
 *
 * Created: 5th November 2026
 * Updated: 5th November 2026
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


#ifndef _WOAD_INTERNAL_SRC_H_WOAD_INTERNAL_MINTTY_PTY_
#define _WOAD_INTERNAL_SRC_H_WOAD_INTERNAL_MINTTY_PTY_


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <wchar.h>


/* /////////////////////////////////////////////////////////////////////////
 * matcher
 *
 * The final path component (after the last '\\' or '/') must be:
 *
 *   (msys|cygwin)-<hash>-pty<digits>-(from|to)-master(-<suffix>)*
 *
 * <hash> and each <suffix> are non-empty and contain no '-'. This is the
 * name GetFileInformationByHandleEx(FileNameInfo) and NtQueryObject report
 * for a mintty PTY, including a "\Device\NamedPipe\" or "\\.\pipe\" prefix.
 * A native Cygwin / MSYS-linked build does not need it: its isatty() is
 * already correct.
 */

static
int
woad_mintty_starts_with_w_(
    wchar_t const*  s
,   wchar_t const*  prefix
)
{
    for (; L'\0' != *prefix; ++s, ++prefix)
    {
        if (*s != *prefix)
        {
            return 0;
        }
    }

    return 1;
}

static
int
woad_mintty_pty_name_is_terminal_(
    wchar_t const*  name
)
{
    wchar_t const* tail;
    wchar_t const* p;

    if (NULL == name ||
        L'\0' == name[0])
    {
        return 0;
    }

    tail = name;

    for (p = name; L'\0' != *p; ++p)
    {
        if (L'\\' == *p ||
            L'/' == *p)
        {
            tail = p + 1;
        }
    }

    if (0)
    {}
    else if (woad_mintty_starts_with_w_(tail, L"msys-"))
    {
        tail += 5;
    }
    else if (woad_mintty_starts_with_w_(tail, L"cygwin-"))
    {
        tail += 7;
    }
    else
    {
        return 0;
    }

    /* <hash>: one or more characters, none of them '-' */

    if (L'\0' == *tail ||
        L'-' == *tail)
    {
        return 0;
    }

    while (L'\0' != *tail &&
           L'-' != *tail)
    {
        ++tail;
    }

    if (L'-' != *tail)
    {
        return 0;
    }

    ++tail;

    /* pty<digits> */

    if (!woad_mintty_starts_with_w_(tail, L"pty"))
    {
        return 0;
    }

    tail += 3;

    if (*tail < L'0' ||
        *tail > L'9')
    {
        return 0;
    }

    while (*tail >= L'0' &&
           *tail <= L'9')
    {
        ++tail;
    }

    if (L'-' != *tail)
    {
        return 0;
    }

    ++tail;

    /* from-master or to-master */

    if (0)
    {}
    else if (woad_mintty_starts_with_w_(tail, L"from-master"))
    {
        tail += 11;
    }
    else if (woad_mintty_starts_with_w_(tail, L"to-master"))
    {
        tail += 9;
    }
    else
    {
        return 0;
    }

    /* optional -<suffix>, each suffix non-empty */

    while (L'-' == *tail)
    {
        ++tail;

        if (L'\0' == *tail ||
            L'-' == *tail)
        {
            return 0;
        }

        while (L'\0' != *tail &&
               L'-' != *tail)
        {
            ++tail;
        }
    }

    return L'\0' == *tail;
}


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#endif /* !_WOAD_INTERNAL_SRC_H_WOAD_INTERNAL_MINTTY_PTY_ */

/* ///////////////////////////// end of file //////////////////////////// */

