#include "Keys.hpp"
#include "ConsoleCommandBase.hpp"
#include "Engine.hpp"
#include "utility.hpp"


void QuitCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Quit       -> Quits the game");
}

void QuitCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    Engine::instance().quit();
}


void FullscreenCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Fullscreen -> Switches between fullscren and windowed modes");
    Engine::instance().console().print(Console::Info, "              If no argumens are given, toggles");
    Engine::instance().console().print(Console::Info, "              Otherwise accepts 1, 0, true or false");
}

void FullscreenCommand::execute(std::vector<std::string> args)
{
    // If no other args are specified assume toggle
    if (args.size() <= 0)
        Engine::instance().toggleFullscreen();
}


void ClearCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Clear -> Clears the console");
}

void ClearCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    Engine::instance().console().clear();
}

void WireframeCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Wireframe -> Sets the render mode to wireframe");
}

void WireframeCommand::execute(std::vector<std::string> args)
{
    if (args.size() == 0)
    {
        usage();
        return;
    }

    bool valid = false;
    bool wireFrame = zelda::utility::parseBool(args[0], valid);
    if(valid)
        Engine::instance().setWireframe(wireFrame);
    else
        Engine::instance().console().print(Console::Warning, "Invalid argument specified");
}


void PlayCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Play -> Plays a sound or music file from the resource manager");
}

void PlayCommand::execute(std::vector<std::string> args)
{
    if (args.size() != 0)
    {
        if (Engine::instance().resourceManager().soundExists(args[0]))
        {
            Engine::instance().resourceManager().playSound(args[0]);
            return;
        }

        if (Engine::instance().resourceManager().musicExists(args[0]))
        {
            Engine::instance().resourceManager().playMusic(args[0]);
            return;
        }

        Engine::instance().console().print(Console::Warning, "Sound or Music resource \"" + args[0] + "\" not found");
    }
}


void BindCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Bind -> Binds a an action to a key mouse/joy \nbutton or joy axis");
    Engine::instance().console().print(Console::Info, "bind <binding> <action>");
    Engine::instance().console().print(Console::Info, "bind list <keyboard,mouse,joy 0-7>\nLists the binding names for the spcified device");
}

void BindCommand::execute(std::vector<std::string> args)
{
    if (args.size() >= 2)
    {
        if (args[0] == "list")
        {
            if (args[1] == "keyboard")
            {
                for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
                    Engine::instance().console().print(Console::Info, "Key: " + KeyInfo[i].name);
            }

            return;
        }

        if (Engine::instance().config().bind(args[1], args[0]))
            Engine::instance().console().print(Console::Message, "Bound %s to %s", args[1].c_str(), args[0].c_str());
        else
            Engine::instance().console().print(Console::Info, "Invalid binding specified");
        return;
    }

    usage();
}


void UnbindCommand::usage()
{
    Engine::instance().console().print(Console::Info, "Unbind -> Unbinds the specified key");

}

void UnbindCommand::execute(std::vector<std::string> args)
{
    if (args.size() != 1)
    {
        usage();
        return;
    }

    Engine::instance().config().unbind(args[0]);
}


void RestartCommand::usage()
{

}

void RestartCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    Engine::instance().restart();
}
