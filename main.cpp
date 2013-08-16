#include "Engine.hpp"
#include <iostream>
#ifdef SFML_SYSTEM_LINUX
    #include <X11/Xlib.h>
#endif

int main(int argc, char* argv[])
{
#ifdef SFML_SYSTEM_LINUX
    XInitThreads();
#endif
    std::ios_base::sync_with_stdio(false);
    Engine& engine = Engine::instance();

    try
    {
        engine.initialize(argc, argv);
        return engine.run();
    }
    catch(...)
    {
        engine.console().print(Console::Fatal, "An unhandled exception has occurred");
        engine.setFullscreen(false);

        return 1;
    }
}
