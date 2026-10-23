/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/c++/colour_for/main.cpp
 *
 * Purpose: Example program that emits colour conditionally, according to
 *          whether the destination stream is attached to a terminal, and to
 *          the colour policy expressed in the environment.
 *
 *          Run it twice to see the difference:
 *
 *            ./example.c++.colour_for
 *            ./example.c++.colour_for | cat
 *
 *          and then again with the policy overridden:
 *
 *            WOAD_COLOUR=always ./example.c++.colour_for | cat
 *            WOAD_COLOUR=never ./example.c++.colour_for
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


#include <woad/iostreams.hpp>

#include <stdio.h>
#include <stdlib.h>


static char const* policy_name(int policy)
{
    switch (policy)
    {
    case WOAD_COLOUR_POLICY_FORCE:

        return "force";
    case WOAD_COLOUR_POLICY_INHIBIT:

        return "inhibit";
    default:

        return "auto";
    }
}

int main(void)
{
    std::cout
        << "policy: " << policy_name(woad_colour_policy())
        << "; "
        << "std::cout is" << (woad::stream_is_tty(std::cout) ? "" : "not") << " a tty"
        << "std::cerr is" << (woad::stream_is_tty(std::cerr) ? "" : "not") << " a tty"
        << "; "
        << std::endl
        ;

    /* The colour-named conditional macros. */

    std::cout
        << WOAD_FG_GREEN_FOR(std::cout)
        << "this line is green if, and only if, stdout warrants colour"
        << WOAD_RESET_FOR(std::cout)
        << std::endl
        ;

    std::cout
        << WOAD_BOLD_FOR(std::cout)
        << "this line is bold if, and only if, stdout warrants colour"
        << WOAD_RESET_FOR(std::cout)
        << std::endl
        ;

    std::cout
        << WOAD_BLINKING_FOR(std::cout)
        << "this line blinks if, and only if, stdout warrants colour"
        << WOAD_RESET_FOR(std::cout)
        << std::endl
        ;

    std::cout
        << WOAD_BG_RED_FOR(std::cout)
        << WOAD_FG_BRIGHT_WHITE_FOR(std::cout)
        << "this line is white-on-red if, and only if, stderr warrants colour"
        << WOAD_RESET_FOR(std::cout)
        << std::endl
        ;

    /* The colour-parameterised conditional macros, which take the sequence
     * to be applied, and so may be driven by a variable.
     */

    std::cout
        << WOAD_FG_FOR(WOAD_FG_YELLOW, std::cout)
        << "this line is yellow if, and only if, stdout warrants colour"
        << WOAD_RESET_FOR(std::cout)
        << std::endl
        ;

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

