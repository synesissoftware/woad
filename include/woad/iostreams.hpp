/* /////////////////////////////////////////////////////////////////////////
 * File:    woad/iostreams.hpp
 *
 * Purpose: IOStreams-interface for woad.
 *
 * Created: 23rd October 2026
 * Updated: 23rd October 2026
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


/** \file woad/iostreams.hpp
 *
 * [C++ only] IOStreams-interface for woad.
 */

#ifndef WOAD_INCL_WOAD_HPP_IOSTREAMS
#define WOAD_INCL_WOAD_HPP_IOSTREAMS

#ifndef WOAD_DOCUMENTATION_SKIP_SECTION
# define WOAD_VER_WOAD_HPP_IOSTREAMS_MAJOR  0
# define WOAD_VER_WOAD_HPP_IOSTREAMS_MINOR  0
# define WOAD_VER_WOAD_HPP_IOSTREAMS_PATCH  0
# define WOAD_VER_WOAD_HPP_IOSTREAMS_EDIT   2
#endif /* !WOAD_DOCUMENTATION_SKIP_SECTION */


#include <woad/woad.h>

#include <iostream>
#include <iomanip>


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

namespace woad {


/* /////////////////////////////////////////////////////////////////////////
 * C++ API
 *
 * A standard iostream maps to the C stream with which it is associated.
 * cout and wcout map to stdout; cerr, clog, and their wide counterparts map
 * to stderr. Any other buffer maps to NULL, so the colour policy alone
 * decides.
 */

template <
    typename T_traits
>
inline
FILE*
representative_standard_c_stream_for(
    std::basic_ios<char, T_traits> const&       stm
)
{
    std::basic_streambuf<char, T_traits> *const rb = stm.rdbuf();

    if (false)
    {}
    else if (rb == std::cout.rdbuf())
    {
        return stdout;
    }
    else if (rb == std::cerr.rdbuf())
    {
        return stderr;
    }
    else if (rb == std::clog.rdbuf())
    {
        return stderr;
    }
    else
    {
        return NULL;
    }
}

template <
    typename T_traits
>
inline
FILE*
representative_standard_c_stream_for(
    std::basic_ios<wchar_t, T_traits> const&    stm
)
{
    std::basic_streambuf<wchar_t, T_traits> *const rb = stm.rdbuf();

    if (false)
    {}
    else if (rb == std::wcout.rdbuf())
    {
        return stdout;
    }
    else if (rb == std::wcerr.rdbuf())
    {
        return stderr;
    }
    else if (rb == std::wclog.rdbuf())
    {
        return stderr;
    }
    else
    {
        return NULL;
    }
}

template <
    typename T_char
,   typename T_traits
>
inline
bool
stream_is_tty(
    std::basic_ios<T_char, T_traits> const&     stm
)
{
    return 0 != woad_stream_is_tty(representative_standard_c_stream_for(stm));
}


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

} // namespace woad


/* Global, so that WOAD_SEQ_FOR's call to woad_seq_for() selects this
 * overload for an iostream argument. The decision, including colour policy,
 * stays in woad_seq_for_stream().
 */

template <
    typename T_char
,   typename T_traits
>
inline
char const*
woad_seq_for(
    char const*                                 seq
,   std::basic_ios<T_char, T_traits> const&     stm
)
{
    return woad_seq_for_stream(seq, woad::representative_standard_c_stream_for(stm));
}


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#ifdef STLSOFT_PPF_pragma_once_SUPPORT
# pragma once
#endif /* STLSOFT_PPF_pragma_once_SUPPORT */

#endif /* !WOAD_INCL_WOAD_HPP_IOSTREAMS */

/* ///////////////////////////// end of file //////////////////////////// */

