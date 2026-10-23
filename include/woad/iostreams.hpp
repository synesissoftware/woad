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
# define WOAD_VER_WOAD_HPP_IOSTREAMS_EDIT   3
#endif /* !WOAD_DOCUMENTATION_SKIP_SECTION */


#include <woad/woad.h>

#include <cassert>
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

template<
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

template<
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

template<
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

/* Inserts seq when colour is warranted for the stream, and inserts nothing
 * otherwise. The decision is woad_seq_for_stream()'s, so policy, terminal
 * attachment, and console capability all apply.
 */
inline
void
apply_sequence(
    std::ios_base&  iob
,   char const*     seq
)
{
    if (std::ostream* stm = dynamic_cast<std::ostream*>(&iob))
    {
        *stm << woad_seq_for_stream(seq, representative_standard_c_stream_for(*stm));

        return;
    }

    if (std::wostream* stm = dynamic_cast<std::wostream*>(&iob))
    {
        *stm << woad_seq_for_stream(seq, representative_standard_c_stream_for(*stm));

        return;
    }
}


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

} // namespace woad


/* /////////////////////////////////////////////////////////////////////////
 * C API functions
 */

/* Global, so that WOAD_SEQ_FOR's call to woad_seq_for() selects this
 * overload for an iostream argument. The decision, including colour policy,
 * stays in woad_seq_for_stream().
 */

template<
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
 * namespace
 */

namespace woad {


/* /////////////////////////////////////////////////////////////////////////
 * IOStreams manipulators
 */

#define WOAD_DEFINE_MANIPULATOR_0_(manip_name, seq)         \
                                                            \
    inline                                                  \
    std::ios_base&                                          \
    manip_name(                                             \
        std::ios_base& stm                                  \
    )                                                       \
    {                                                       \
        woad::apply_sequence(stm, seq);                     \
                                                            \
        return stm;                                         \
    }

WOAD_DEFINE_MANIPULATOR_0_(reset, WOAD_RESET)

WOAD_DEFINE_MANIPULATOR_0_(bold, WOAD_BOLD)
WOAD_DEFINE_MANIPULATOR_0_(blinking, WOAD_BLINKING)

WOAD_DEFINE_MANIPULATOR_0_(black, WOAD_FG_BLACK)
WOAD_DEFINE_MANIPULATOR_0_(red, WOAD_FG_RED)
WOAD_DEFINE_MANIPULATOR_0_(green, WOAD_FG_GREEN)
WOAD_DEFINE_MANIPULATOR_0_(yellow, WOAD_FG_YELLOW)
WOAD_DEFINE_MANIPULATOR_0_(blue, WOAD_FG_BLUE)
WOAD_DEFINE_MANIPULATOR_0_(magenta, WOAD_FG_MAGENTA)
WOAD_DEFINE_MANIPULATOR_0_(cyan, WOAD_FG_CYAN)
WOAD_DEFINE_MANIPULATOR_0_(white, WOAD_FG_WHITE)

WOAD_DEFINE_MANIPULATOR_0_(bright_black, WOAD_FG_BRIGHT_BLACK)
WOAD_DEFINE_MANIPULATOR_0_(bright_red, WOAD_FG_BRIGHT_RED)
WOAD_DEFINE_MANIPULATOR_0_(bright_green, WOAD_FG_BRIGHT_GREEN)
WOAD_DEFINE_MANIPULATOR_0_(bright_yellow, WOAD_FG_BRIGHT_YELLOW)
WOAD_DEFINE_MANIPULATOR_0_(bright_blue, WOAD_FG_BRIGHT_BLUE)
WOAD_DEFINE_MANIPULATOR_0_(bright_magenta, WOAD_FG_BRIGHT_MAGENTA)
WOAD_DEFINE_MANIPULATOR_0_(bright_cyan, WOAD_FG_BRIGHT_CYAN)
WOAD_DEFINE_MANIPULATOR_0_(bright_white, WOAD_FG_BRIGHT_WHITE)

WOAD_DEFINE_MANIPULATOR_0_(bg_black, WOAD_BG_BLACK)
WOAD_DEFINE_MANIPULATOR_0_(bg_red, WOAD_BG_RED)
WOAD_DEFINE_MANIPULATOR_0_(bg_green, WOAD_BG_GREEN)
WOAD_DEFINE_MANIPULATOR_0_(bg_yellow, WOAD_BG_YELLOW)
WOAD_DEFINE_MANIPULATOR_0_(bg_blue, WOAD_BG_BLUE)
WOAD_DEFINE_MANIPULATOR_0_(bg_magenta, WOAD_BG_MAGENTA)
WOAD_DEFINE_MANIPULATOR_0_(bg_cyan, WOAD_BG_CYAN)
WOAD_DEFINE_MANIPULATOR_0_(bg_white, WOAD_BG_WHITE)

WOAD_DEFINE_MANIPULATOR_0_(bg_bright_black, WOAD_BG_BRIGHT_BLACK)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_red, WOAD_BG_BRIGHT_RED)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_green, WOAD_BG_BRIGHT_GREEN)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_yellow, WOAD_BG_BRIGHT_YELLOW)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_blue, WOAD_BG_BRIGHT_BLUE)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_magenta, WOAD_BG_BRIGHT_MAGENTA)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_cyan, WOAD_BG_BRIGHT_CYAN)
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_white, WOAD_BG_BRIGHT_WHITE)


/* /////////////////////////////////////////////////////////////////////////
 * inserters
 */

struct woad_inserter_t {
    woad_inserter_t const*  inner;
    char const*             msg;
    char const*             seq;

private:
    static
    char const*
    str_always_(
        char const* s
    )
    {
        return NULL == s ? "" : s;
    }

public:
    explicit
    woad_inserter_t(
        char const* msg
    ,   char const* seq
    )
        : inner(NULL)
        , msg(str_always_(msg))
        , seq(str_always_(seq))
    {}
    explicit
    woad_inserter_t(
        woad_inserter_t const&  inner
    ,   char const*             seq
    )
        : inner(&inner)
        , msg(NULL)
        , seq(str_always_(seq))
    {}

    template<
        typename T_char
    ,   typename T_traits
    >
    friend
    std::basic_ostream<T_char, T_traits>&
    operator <<(
        std::basic_ostream<T_char, T_traits>&   stm
    ,   woad_inserter_t const&                  wi
    )
    {
        /* Called once for a chain of concatenated inserters. Each sequence,
         * and the trailing reset, is emitted only when colour is warranted
         * for stm.
         */

        FILE* const cstm = representative_standard_c_stream_for(stm);

        for (woad_inserter_t const* wit = &wi; NULL != wit; wit = wit->inner)
        {
            stm << woad_seq_for_stream(wit->seq, cstm);

            if (NULL == wit->inner)
            {
                stm << wit->msg;
            }
        }

        stm << woad_seq_for_stream(WOAD_RESET, cstm);

        return stm;
    }
};

#define WOAD_DEFINE_INSERTER_1_(ins_name, seq)              \
                                                            \
    inline                                                  \
    woad_inserter_t                                         \
    ins_name(                                               \
        char const* msg                                     \
    )                                                       \
    {                                                       \
        return woad_inserter_t(msg, seq);                   \
    }                                                       \
    inline                                                  \
    woad_inserter_t                                         \
    ins_name(                                               \
        woad_inserter_t const& inner                        \
    )                                                       \
    {                                                       \
        return woad_inserter_t(inner, seq);                 \
    }


WOAD_DEFINE_INSERTER_1_(bold, WOAD_BOLD)
WOAD_DEFINE_INSERTER_1_(blinking, WOAD_BLINKING)

WOAD_DEFINE_INSERTER_1_(black, WOAD_FG_BLACK)
WOAD_DEFINE_INSERTER_1_(red, WOAD_FG_RED)
WOAD_DEFINE_INSERTER_1_(green, WOAD_FG_GREEN)
WOAD_DEFINE_INSERTER_1_(yellow, WOAD_FG_YELLOW)
WOAD_DEFINE_INSERTER_1_(blue, WOAD_FG_BLUE)
WOAD_DEFINE_INSERTER_1_(magenta, WOAD_FG_MAGENTA)
WOAD_DEFINE_INSERTER_1_(cyan, WOAD_FG_CYAN)
WOAD_DEFINE_INSERTER_1_(white, WOAD_FG_WHITE)

WOAD_DEFINE_INSERTER_1_(bright_black, WOAD_FG_BRIGHT_BLACK)
WOAD_DEFINE_INSERTER_1_(bright_red, WOAD_FG_BRIGHT_RED)
WOAD_DEFINE_INSERTER_1_(bright_green, WOAD_FG_BRIGHT_GREEN)
WOAD_DEFINE_INSERTER_1_(bright_yellow, WOAD_FG_BRIGHT_YELLOW)
WOAD_DEFINE_INSERTER_1_(bright_blue, WOAD_FG_BRIGHT_BLUE)
WOAD_DEFINE_INSERTER_1_(bright_magenta, WOAD_FG_BRIGHT_MAGENTA)
WOAD_DEFINE_INSERTER_1_(bright_cyan, WOAD_FG_BRIGHT_CYAN)
WOAD_DEFINE_INSERTER_1_(bright_white, WOAD_FG_BRIGHT_WHITE)

WOAD_DEFINE_INSERTER_1_(bg_black, WOAD_BG_BLACK)
WOAD_DEFINE_INSERTER_1_(bg_red, WOAD_BG_RED)
WOAD_DEFINE_INSERTER_1_(bg_green, WOAD_BG_GREEN)
WOAD_DEFINE_INSERTER_1_(bg_yellow, WOAD_BG_YELLOW)
WOAD_DEFINE_INSERTER_1_(bg_blue, WOAD_BG_BLUE)
WOAD_DEFINE_INSERTER_1_(bg_magenta, WOAD_BG_MAGENTA)
WOAD_DEFINE_INSERTER_1_(bg_cyan, WOAD_BG_CYAN)
WOAD_DEFINE_INSERTER_1_(bg_white, WOAD_BG_WHITE)

WOAD_DEFINE_INSERTER_1_(bg_bright_black, WOAD_BG_BRIGHT_BLACK)
WOAD_DEFINE_INSERTER_1_(bg_bright_red, WOAD_BG_BRIGHT_RED)
WOAD_DEFINE_INSERTER_1_(bg_bright_green, WOAD_BG_BRIGHT_GREEN)
WOAD_DEFINE_INSERTER_1_(bg_bright_yellow, WOAD_BG_BRIGHT_YELLOW)
WOAD_DEFINE_INSERTER_1_(bg_bright_blue, WOAD_BG_BRIGHT_BLUE)
WOAD_DEFINE_INSERTER_1_(bg_bright_magenta, WOAD_BG_BRIGHT_MAGENTA)
WOAD_DEFINE_INSERTER_1_(bg_bright_cyan, WOAD_BG_BRIGHT_CYAN)
WOAD_DEFINE_INSERTER_1_(bg_bright_white, WOAD_BG_BRIGHT_WHITE)


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

} // namespace woad


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#ifdef STLSOFT_PPF_pragma_once_SUPPORT
# pragma once
#endif /* STLSOFT_PPF_pragma_once_SUPPORT */

#endif /* !WOAD_INCL_WOAD_HPP_IOSTREAMS */

/* ///////////////////////////// end of file //////////////////////////// */

