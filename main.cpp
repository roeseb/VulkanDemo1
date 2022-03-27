#include "demo.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
    ved::Demos::SimpleDemo *demo = new ved::Demos::SimpleDemo();

    try
    {
        demo->run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}