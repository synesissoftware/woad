/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/c++/colour/main.cpp
 *
 * Purpose: Example program that emits coloured / attributed status strings.
 *
 * Created: 26th October 2026
 * Updated: 26th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <woad/woad.h>

#include <stdio.h>
#include <stdlib.h>


int main(int /* argc */, char* /* argv */[])
{
    puts("C++:");

    puts("\tthe colour is " WOAD_FG_GREEN "green" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_RED "red" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_YELLOW "yellow" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_BLUE "blue" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_MAGENTA "magenta" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_CYAN "cyan" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_WHITE "white" WOAD_RESET ".");
    puts("\tthe colour is " WOAD_FG_BRIGHT_BLACK "bright black" WOAD_RESET ".");
    puts("\tthe style is " WOAD_BOLD WOAD_FG_RED "bold red" WOAD_RESET ".");
    puts("\tthe style is " WOAD_BLINKING WOAD_FG_YELLOW "blinking yellow" WOAD_RESET ".");

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

