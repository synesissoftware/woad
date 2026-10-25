/* /////////////////////////////////////////////////////////////////////////
 * File:    woad/iostreams.hpp
 *
 * Purpose: IOStreams-interface for woad.
 *
 * Created: 23rd October 2026
 * Updated: 26th October 2026
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
# define WOAD_VER_WOAD_HPP_IOSTREAMS_EDIT   4
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

/** \defgroup group__woad__iostreams IOStreams interface
 * \ingroup group__woad
 *
 * \brief [C++ only] Manipulators, inserters, and stream queries for
 *   standard IOStreams.
 *
 * A standard stream maps to the C stream with which it is associated:
 * `std::cout` and `std::wcout` map to `stdout`; `std::cerr`, `std::clog`,
 * and their wide counterparts map to `stderr`. Any other stream buffer
 * maps to `NULL`, so that the colour policy alone decides.
 *
 * Every manipulator and inserter emits its sequence only when colour is
 * warranted for the stream, per woad_seq_for_stream(). Each inserter is
 * overloaded to accept either a message or another inserter, so that
 * attributes combine, e.g. `woad::bold(woad::red("failed"))`.
 *
 * @{
 */

/** \brief Obtains the C stream that corresponds to a narrow IOStream.
 *
 * \param stm The stream to be mapped;
 *
 * \return `stdout` if \c stm uses the buffer of `std::cout`; `stderr` if it
 *   uses that of `std::cerr` or `std::clog`; otherwise `NULL`.
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

/** \brief Obtains the C stream that corresponds to a wide IOStream.
 *
 * \param stm The stream to be mapped;
 *
 * \return `stdout` if \c stm uses the buffer of `std::wcout`; `stderr` if it
 *   uses that of `std::wcerr` or `std::wclog`; otherwise `NULL`.
 */
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

/** \brief Indicates whether the stream is attached to a terminal.
 *
 * \param stm The stream to be tested;
 *
 * \retval true The stream maps to a C stream that is a terminal;
 * \retval false The stream does not map to a C stream, or the C stream is
 *   not a terminal;
 */
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

/** \brief Inserts a sequence when colour is warranted for the stream, and
 *   inserts nothing otherwise.
 *
 * The decision is that of woad_seq_for_stream(), so policy, terminal
 * attachment, and console capability all apply.
 *
 * \param iob The stream to which the sequence is to be applied. Streams
 *   that are neither `std::ostream` nor `std::wostream` are ignored;
 * \param seq The SGR sequence;
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

/** \brief IOStreams overload of woad_seq_for_stream().
 *
 * Global, so that WOAD_SEQ_FOR's call to woad_seq_for() selects this
 * overload for an iostream argument. The decision, including colour
 * policy, stays in woad_seq_for_stream().
 *
 * \param seq The SGR sequence. May be NULL, which is treated as `""`;
 * \param stm The stream. May be any IOStream;
 *
 * \return Either \c seq or `""`. Never NULL.
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

/** \defgroup group__woad__iostreams__manipulators IOStreams manipulators
 * \ingroup group__woad__iostreams
 *
 * \brief [C++ only] Manipulators that apply an attribute or colour to
 *   subsequent output, until reset with woad::reset().
 *
 * @{
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

/** \brief Manipulator that resets all attributes and colours, when colour
 *   is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(reset, WOAD_RESET)

/** \brief Manipulator that selects bold, when colour is warranted for the
 *   stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bold, WOAD_BOLD)

/** \brief Manipulator that selects blinking, when colour is warranted for
 *   the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(blinking, WOAD_BLINKING)

/** \brief Manipulator that selects black foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(black, WOAD_FG_BLACK)

/** \brief Manipulator that selects red foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(red, WOAD_FG_RED)

/** \brief Manipulator that selects green foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(green, WOAD_FG_GREEN)

/** \brief Manipulator that selects yellow foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(yellow, WOAD_FG_YELLOW)

/** \brief Manipulator that selects blue foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(blue, WOAD_FG_BLUE)

/** \brief Manipulator that selects magenta foreground colour, when colour
 *   is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(magenta, WOAD_FG_MAGENTA)

/** \brief Manipulator that selects cyan foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(cyan, WOAD_FG_CYAN)

/** \brief Manipulator that selects white foreground colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(white, WOAD_FG_WHITE)

/** \brief Manipulator that selects bright black foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_black, WOAD_FG_BRIGHT_BLACK)

/** \brief Manipulator that selects bright red foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_red, WOAD_FG_BRIGHT_RED)

/** \brief Manipulator that selects bright green foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_green, WOAD_FG_BRIGHT_GREEN)

/** \brief Manipulator that selects bright yellow foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_yellow, WOAD_FG_BRIGHT_YELLOW)

/** \brief Manipulator that selects bright blue foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_blue, WOAD_FG_BRIGHT_BLUE)

/** \brief Manipulator that selects bright magenta foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_magenta, WOAD_FG_BRIGHT_MAGENTA)

/** \brief Manipulator that selects bright cyan foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_cyan, WOAD_FG_BRIGHT_CYAN)

/** \brief Manipulator that selects bright white foreground colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bright_white, WOAD_FG_BRIGHT_WHITE)

/** \brief Manipulator that selects black background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_black, WOAD_BG_BLACK)

/** \brief Manipulator that selects red background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_red, WOAD_BG_RED)

/** \brief Manipulator that selects green background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_green, WOAD_BG_GREEN)

/** \brief Manipulator that selects yellow background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_yellow, WOAD_BG_YELLOW)

/** \brief Manipulator that selects blue background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_blue, WOAD_BG_BLUE)

/** \brief Manipulator that selects magenta background colour, when colour
 *   is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_magenta, WOAD_BG_MAGENTA)

/** \brief Manipulator that selects cyan background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_cyan, WOAD_BG_CYAN)

/** \brief Manipulator that selects white background colour, when colour is
 *   warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_white, WOAD_BG_WHITE)

/** \brief Manipulator that selects bright black background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_black, WOAD_BG_BRIGHT_BLACK)

/** \brief Manipulator that selects bright red background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_red, WOAD_BG_BRIGHT_RED)

/** \brief Manipulator that selects bright green background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_green, WOAD_BG_BRIGHT_GREEN)

/** \brief Manipulator that selects bright yellow background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_yellow, WOAD_BG_BRIGHT_YELLOW)

/** \brief Manipulator that selects bright blue background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_blue, WOAD_BG_BRIGHT_BLUE)

/** \brief Manipulator that selects bright magenta background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_magenta, WOAD_BG_BRIGHT_MAGENTA)

/** \brief Manipulator that selects bright cyan background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_cyan, WOAD_BG_BRIGHT_CYAN)

/** \brief Manipulator that selects bright white background colour, when
 *   colour is warranted for the stream.
 *
 * \param stm The stream to which the sequence is to be applied;
 *
 * \return A reference to \c stm.
 */
WOAD_DEFINE_MANIPULATOR_0_(bg_bright_white, WOAD_BG_BRIGHT_WHITE)


/** @} */


/* /////////////////////////////////////////////////////////////////////////
 * inserters
 */

/** \defgroup group__woad__iostreams__inserters IOStreams inserters
 * \ingroup group__woad__iostreams
 *
 * \brief [C++ only] Inserters that apply an attribute or colour to a single
 *   message, and then reset.
 *
 * @{
 */

/** \brief The type returned by the colour inserters.
 *
 * Holds a message (or another inserter) and a sequence; on insertion into
 * a stream it emits the sequence, the message, and a trailing reset, each
 * only when colour is warranted for that stream.
 *
 * \note Not intended to be created directly: use the inserters
 *   woad::bold(), woad::red(), and so on.
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
    /** \brief Creates an inserter for a message.
     *
     * \param msg The message. May be NULL, which is treated as `""`;
     * \param seq The SGR sequence. May be NULL, which is treated as `""`;
     */
    explicit
    woad_inserter_t(
        char const* msg
    ,   char const* seq
    )
        : inner(NULL)
        , msg(str_always_(msg))
        , seq(str_always_(seq))
    {}
    /** \brief Creates an inserter that adds a sequence to another inserter.
     *
     * \param inner The inserter to be wrapped. Must outlive this instance;
     * \param seq The SGR sequence. May be NULL, which is treated as `""`;
     */
    explicit
    woad_inserter_t(
        woad_inserter_t const&  inner
    ,   char const*             seq
    )
        : inner(&inner)
        , msg(NULL)
        , seq(str_always_(seq))
    {}

    /** \brief Inserts the sequences, the message, and a reset into a stream.
     *
     * \param stm The stream;
     * \param wi The inserter;
     *
     * \return A reference to \c stm.
     */
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


/** \brief Inserter that renders its message in bold, when colour is
 *   warranted for the stream. May also wrap another inserter, to combine
 *   attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bold, WOAD_BOLD)

/** \brief Inserter that renders its message in blinking, when colour is
 *   warranted for the stream. May also wrap another inserter, to combine
 *   attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(blinking, WOAD_BLINKING)

/** \brief Inserter that renders its message in black foreground colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(black, WOAD_FG_BLACK)

/** \brief Inserter that renders its message in red foreground colour, when
 *   colour is warranted for the stream. May also wrap another inserter, to
 *   combine attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(red, WOAD_FG_RED)

/** \brief Inserter that renders its message in green foreground colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(green, WOAD_FG_GREEN)

/** \brief Inserter that renders its message in yellow foreground colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(yellow, WOAD_FG_YELLOW)

/** \brief Inserter that renders its message in blue foreground colour, when
 *   colour is warranted for the stream. May also wrap another inserter, to
 *   combine attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(blue, WOAD_FG_BLUE)

/** \brief Inserter that renders its message in magenta foreground colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(magenta, WOAD_FG_MAGENTA)

/** \brief Inserter that renders its message in cyan foreground colour, when
 *   colour is warranted for the stream. May also wrap another inserter, to
 *   combine attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(cyan, WOAD_FG_CYAN)

/** \brief Inserter that renders its message in white foreground colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(white, WOAD_FG_WHITE)

/** \brief Inserter that renders its message in bright black foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_black, WOAD_FG_BRIGHT_BLACK)

/** \brief Inserter that renders its message in bright red foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_red, WOAD_FG_BRIGHT_RED)

/** \brief Inserter that renders its message in bright green foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_green, WOAD_FG_BRIGHT_GREEN)

/** \brief Inserter that renders its message in bright yellow foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_yellow, WOAD_FG_BRIGHT_YELLOW)

/** \brief Inserter that renders its message in bright blue foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_blue, WOAD_FG_BRIGHT_BLUE)

/** \brief Inserter that renders its message in bright magenta foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_magenta, WOAD_FG_BRIGHT_MAGENTA)

/** \brief Inserter that renders its message in bright cyan foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_cyan, WOAD_FG_BRIGHT_CYAN)

/** \brief Inserter that renders its message in bright white foreground
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bright_white, WOAD_FG_BRIGHT_WHITE)

/** \brief Inserter that renders its message in black background colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_black, WOAD_BG_BLACK)

/** \brief Inserter that renders its message in red background colour, when
 *   colour is warranted for the stream. May also wrap another inserter, to
 *   combine attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_red, WOAD_BG_RED)

/** \brief Inserter that renders its message in green background colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_green, WOAD_BG_GREEN)

/** \brief Inserter that renders its message in yellow background colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_yellow, WOAD_BG_YELLOW)

/** \brief Inserter that renders its message in blue background colour, when
 *   colour is warranted for the stream. May also wrap another inserter, to
 *   combine attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_blue, WOAD_BG_BLUE)

/** \brief Inserter that renders its message in magenta background colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_magenta, WOAD_BG_MAGENTA)

/** \brief Inserter that renders its message in cyan background colour, when
 *   colour is warranted for the stream. May also wrap another inserter, to
 *   combine attributes (the overload taking a \c woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_cyan, WOAD_BG_CYAN)

/** \brief Inserter that renders its message in white background colour,
 *   when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_white, WOAD_BG_WHITE)

/** \brief Inserter that renders its message in bright black background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_black, WOAD_BG_BRIGHT_BLACK)

/** \brief Inserter that renders its message in bright red background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_red, WOAD_BG_BRIGHT_RED)

/** \brief Inserter that renders its message in bright green background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_green, WOAD_BG_BRIGHT_GREEN)

/** \brief Inserter that renders its message in bright yellow background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_yellow, WOAD_BG_BRIGHT_YELLOW)

/** \brief Inserter that renders its message in bright blue background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_blue, WOAD_BG_BRIGHT_BLUE)

/** \brief Inserter that renders its message in bright magenta background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_magenta, WOAD_BG_BRIGHT_MAGENTA)

/** \brief Inserter that renders its message in bright cyan background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_cyan, WOAD_BG_BRIGHT_CYAN)

/** \brief Inserter that renders its message in bright white background
 *   colour, when colour is warranted for the stream. May also wrap another
 *   inserter, to combine attributes (the overload taking a \c
 *   woad_inserter_t const&).
 *
 * \param msg The message to be inserted;
 *
 * \return An inserter, for insertion into a stream.
 */
WOAD_DEFINE_INSERTER_1_(bg_bright_white, WOAD_BG_BRIGHT_WHITE)


/* /////////////////////////////////////////////////////////////////////////
 * namespace
 */

/** @} */

/** @} */

} // namespace woad


/* /////////////////////////////////////////////////////////////////////////
 * inclusion
 */

#ifdef STLSOFT_PPF_pragma_once_SUPPORT
# pragma once
#endif /* STLSOFT_PPF_pragma_once_SUPPORT */

#endif /* !WOAD_INCL_WOAD_HPP_IOSTREAMS */

/* ///////////////////////////// end of file //////////////////////////// */

