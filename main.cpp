#include "application.h"

#include <iostream>

int main()
{
    Pyro::Application app;
    
    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    

    return 0;
}