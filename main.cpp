#include <cstdlib>

// testing
#include <iostream>
#include <string>
#include "flx.hpp"

int main(int argc, char *argv[])
{

    std::string testfile("u7\\STATIC\\PALETTES.flx");

    // testing
    FLXFile testflx;
    if(testflx.open(testfile))
    {
        testflx.show();
    }
    else std::cout << "error opening " << testfile << std::endl;

    return 0;
}
