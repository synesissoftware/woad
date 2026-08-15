/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/colour.c
 *
 * Purpose: Example program that emits a coloured status string.
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


int main(void)
{
    puts("the colour is " WOAD_FG_GREEN "green" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_RED "red" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_YELLOW "yellow" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_BLUE "blue" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_MAGENTA "magenta" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_CYAN "cyan" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_WHITE "white" WOAD_RESET ".");
    puts("the colour is " WOAD_FG_BRIGHT_BLACK "bright black" WOAD_RESET ".");

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */
