#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/Keys.hpp"
#include "Sakura/Core/ConsoleCommandBase.hpp"
#include "Sakura/Core/CVar.hpp"
#include <utility.hpp>

namespace Sakura
{
namespace Core
{
std::string QuitCommand::usage() const
{
    return "Quits the game";
}

void QuitCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    sEngineRef().quit();
}


std::string FullscreenCommand::usage() const
{
    return "Switches between fullscren and windowed modes\n"
           "              If no argumens are given, toggles\n"
           "              Otherwise accepts 1, 0, true or false";
}

void FullscreenCommand::execute(std::vector<std::string> args)
{
    // If no other args are specified assume toggle
    if (args.size() <= 0)
        sEngineRef().toggleFullscreen();
}


std::string ClearCommand::usage() const
{
    return "Clears the console";
}

void ClearCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    sEngineRef().console().clear();
}

std::string WireframeCommand::usage() const
{
    return "Sets the render mode to wireframe";
}

void WireframeCommand::execute(std::vector<std::string> args)
{
    if (args.size() == 0)
    {
        usage();
        return;
    }

    bool valid = false;
    bool wireFrame = zelda::utility::parseBool(args[0], &valid);
    if(valid)
        sEngineRef().setWireframe(wireFrame);
    else
        sEngineRef().console().print(Console::Warning, "Invalid argument specified");
}


std::string PlayCommand::usage() const
{
    return "Plays a sound or music file from the resource manager";
}

void PlayCommand::execute(std::vector<std::string> args)
{
    if (args.size() != 0)
    {
        if (sEngineRef().resourceManager().soundExists(args[0]))
        {
            sEngineRef().resourceManager().playSound(args[0]);
            return;
        }

        if (sEngineRef().resourceManager().songExists(args[0]))
        {
            sEngineRef().resourceManager().playSong(args[0]);
            return;
        }

        sEngineRef().console().print(Console::Warning, "Sound or Music resource \"" + args[0] + "\" not found");
    }
}


std::string BindCommand::usage() const
{
    return "Binds a an action to a key mouse/joy \nbutton or joy axis\n"
           "bind <binding> <action>\n"
           "bind list <keyboard,mouse,joy 0-7>\nLists the binding names for the spcified device";
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
                    sEngineRef().console().print(Console::Info, "Key: " + KeyInfo[i].name);
            }

            return;
        }

        CVar* binding = sEngineRef().cvarManager().findCVar(args[1]);

        if (binding && binding->isBinding())
        {
            if (binding->tryBinding(args[0]))
                sEngineRef().console().print(Console::Message, "Bound %s to %s", args[1].c_str(), args[0].c_str());
            else
                sEngineRef().console().print(Console::Info, "Invalid binding specified");
        }
        else
        {
            sEngineRef().console().print(Console::Info, "Specified impulse %s does not exist", args[1].c_str());
        }
        return;
    }

    usage();
}


std::string UnbindCommand::usage() const
{
    return "Unbinds the specified key";

}

void UnbindCommand::execute(std::vector<std::string> args)
{
    if (args.size() != 1)
    {
        sEngineRef().console().print(Console::Info, "unbind %s", usage().c_str());
        return;
    }

    CVar* binding = sEngineRef().cvarManager().findCVar(args[0]);
    if (binding && binding->isBinding())
        binding->clearBindings();
    else
        sEngineRef().console().print(Console::Info, "Specified impulse %s does not exist", args[0].c_str());
}


std::string RestartCommand::usage() const
{
    return "Restarts the engine";
}

void RestartCommand::execute(std::vector<std::string> args)
{
    UNUSED(args);
    sEngineRef().restart();
}
}
}
