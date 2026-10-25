/* /////////////////////////////////////////////////////////////////////////
 * File:    examples/iostreams/version/main.cpp
 *
 * Purpose: Example program that prints the woad version string, via both
 *          the version macros and the version functions.
 *
 * Created: 26th October 2026
 * Updated: 26th October 2026
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <woad/iostreams.hpp>

#include <iostream>

#include <stdlib.h>


int main(int /* argc */, char* /* argv */[])
{
    std::cout << "C++:" << std::endl;


    std::cout
        <<  '\t'
        <<  woad::blue("woad")
        <<  ": v"
        <<  woad::cyan("WOAD_VER")
        <<  " ("
        <<  woad::green(woad_version_string())
        <<  " = "
        <<  woad::green
        <<  "0x" << std::hex << std::setw(8) << std::setfill('0') << woad::version()
        <<  woad::reset
        <<  ")"
        << std::endl
        ;

    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

