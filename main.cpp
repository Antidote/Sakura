#include "Engine.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Engine& engine = Engine::instance();
    engine.initialize(argc, argv, 640, 480, "Sakura ALPHA BUILD");
    return engine.run();
}
