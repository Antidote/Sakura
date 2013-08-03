#include <iostream>
#include <Engine.hpp>

int main()
{
    Engine& engine = Engine::instance();
    engine.initialize(640, 480, "Test");

    return engine.run();
}

