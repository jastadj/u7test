#include <cstdlib>

#include "u7.hpp"

int main(int argc, char *argv[])
{
    U7 *u7;
    u7 = U7::getInstance();
    u7->start();

    return 0;
}
