#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/Keys.hpp"
#include "Sakura/Core/Config.hpp"
#include <IOException.hpp>
#include <utility.hpp>
#include <sstream>

namespace Sakura
{
namespace Core
{
Config::Config()
{
}

Config::~Config()
{
}

void Config::initialize(const std::string& configFile)
{
    try
    {
        m_configFile = new zelda::io::TextStream(configFile, zelda::io::TextStream::OpenOrCreate);

        sEngineRef().console().print(Console::Info, "Got config\nParsing...");
        parse();
    }
    catch(zelda::error::Exception e)
    {
        sEngineRef().console().print(Console::Error, e.message());
    }
}

void Config::shutdown()
{
    // Clear the file
    m_configFile->truncate();

    m_configFile->writeLine("// Generated by Sakura DO NOT MODIFY");
    m_configFile->writeLine("// Any personal notes *WILL BE LOST*\n");

    m_configFile->writeLine("unbind all\n");

    // write settings first
    m_configFile->writeLine("// Settings");
    for (std::pair<std::string, std::string> setting : m_settings)
    {
        std::string val = settingLiteral(setting.first);
        if (val != std::string())
        {
            if (val.find_first_of(' ') != std::string::npos)
            {
                val.insert(val.begin(), '\"');
                val += "\"";
            }

            m_configFile->writeLine("set " + setting.first + " " + val);
        }
    }

    m_configFile->writeLine("\n// Keys");
    // Write the key bindings
    for (std::pair<std::string, sf::Keyboard::Key> key : m_keyBinding)
    {
        std::string keyName ;
        for (int i = 0; i < 100; i++)
        {
            if (key.second == KeyInfo[i].code)
            {
                keyName = KeyInfo[i].name;
                break;
            }
        }

        if (keyName != std::string())
            m_configFile->writeLine("bind " + keyName + " " + key.first);
    }

    m_configFile->writeLine("\n// Mouse");

    for (std::pair<std::string, sf::Mouse::Button> button: m_mouseButtonBinding)
    {
        std::string buttonName ;
        for (int i = 0; i < 5; i++)
        {
            if (button.second == (int)MouseButtonInfo[i].button)
            {
                buttonName = MouseButtonInfo[i].name;
                break;
            }
        }

        if (buttonName != std::string())
            m_configFile->writeLine("bind " + MouseButtonInfo[button.second].name + " " + button.first);
    }

    m_configFile->writeLine("\n// Joystick Axis");

    for (std::pair<std::string, Joy> axisBinding : m_axisBinding)
    {
        Joy joy = axisBinding.second;
        std::string action = axisBinding.first;
        action.erase(action.size() - 1);
        m_configFile->writeLine(zelda::utility::sprintf("bind %sjoy%i.%s %s", (joy.neg ? "-" : ""), joy.id, AxisInfo[joy.axis].name.c_str(), action.c_str()));
    }

    m_configFile->writeLine("\n// Joystick Button");

    for (std::pair<std::string, Joy> buttonBinding : m_joyButtonBinding)
    {
        Joy joy = buttonBinding.second;
        std::string action = buttonBinding.first;
        action.erase(action.size() - 1);
        m_configFile->writeLine(zelda::utility::sprintf("bind joy%i.button%i %s", joy.id, joy.button, action.c_str()));
    }

    m_configFile->save();
    delete m_configFile;
    m_configFile = NULL;
}

bool Config::bind(const std::string& action, const std::string& binding)
{
    std::string bindingName = binding;
    zelda::utility::tolower(bindingName);
    if (binding.compare("mouse") != -1)
    {
        for (int i = 0; i < sf::Mouse::ButtonCount; i++)
        {
            if(!MouseButtonInfo[i].name.compare(bindingName))
            {
                bindMouseButton(action, MouseButtonInfo[i].button);
                return true;
            }
        }
    }

    if (binding.compare("joy") != -1)
    {
        for (int i = 0; i < sf::Joystick::Count; i++)
        {
            // first axis
            for (int j = 0; j < sf::Joystick::AxisCount; j++)
            {
                // First try axis
                std::string axisName = zelda::utility::sprintf("joy%i.%s", i, AxisInfo[j].name.c_str());
                std::string negAxisName = zelda::utility::sprintf("-joy%i.%s", i, AxisInfo[j].name.c_str());

                if (!axisName.compare(bindingName))
                {
                    bindJoyAxis(action, i, false,  AxisInfo[j].axis);
                    return true;
                }
                if (!negAxisName.compare(bindingName))
                {
                    bindJoyAxis(action, i, true,  AxisInfo[j].axis);
                    return true;
                }
            }

            // Now buttons
            for (int j = 0; j < sf::Joystick::ButtonCount; j++)
            {
                // First try axis
                std::string axisName = zelda::utility::sprintf("joy%i.button%i", i, j);

                if (!axisName.compare(bindingName))
                {
                    bindJoyButton(action, i, j);
                    return true;
                }
            }
        }
    }

    // Now for keyboard
    for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
    {
        if (!bindingName.compare(KeyInfo[i].name))
        {
            bindKey(action, KeyInfo[i].code);
            return true;
        }
    }

    // Oops user specified an invalid key!;
    return false;
}

void Config::bindKey(const std::string& action, sf::Keyboard::Key key)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);
    m_keyBinding[actionName] = key;
}

void Config::bindJoyAxis(const std::string& action, int joy, bool neg, sf::Joystick::Axis axis)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);

    Joy binding;
    binding.id = joy;
    binding.neg = neg;
    binding.axis = axis;
    actionName = zelda::utility::sprintf("%s%i", actionName.c_str(), joy);
    m_axisBinding[actionName] = binding;
}

void Config::bindJoyButton(const std::string& action, int joy, int button)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);

    Joy binding;
    binding.id = joy;
    binding.button = button;
    actionName = zelda::utility::sprintf("%s%i", actionName.c_str(), joy);
    m_joyButtonBinding[actionName] = binding;
}

void Config::bindMouseButton(const std::string& action, sf::Mouse::Button button)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);

    m_mouseButtonBinding[actionName] = button;
}

sf::Keyboard::Key Config::keyForAction(const std::string& action, sf::Keyboard::Key def)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);
    if (m_keyBinding.find(actionName) == m_keyBinding.end())
        m_keyBinding[actionName] = def;

    return m_keyBinding[actionName];
}

Int32 Config::joyAxisForAction(const std::string& action, int joy, sf::Joystick::Axis defAxis, bool defNeg, bool &neg)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);
    actionName = zelda::utility::sprintf("%s%i", actionName.c_str(), joy);

    if (m_axisBinding.find(actionName) == m_axisBinding.end())
    {
        Joy _default;
        _default.id = joy;
        _default.axis = defAxis;
        _default.neg = defNeg;
        m_axisBinding[actionName] = _default;
    }


    neg = ((Joy)m_axisBinding[actionName]).neg;
    return ((Joy)m_axisBinding[actionName]).axis;
}

Int32 Config::joyButtonForAction(const std::string& action, int joy, int def)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);
    actionName = zelda::utility::sprintf("%s%i", actionName.c_str(), joy);
    if (m_joyButtonBinding.find(actionName) == m_joyButtonBinding.end())
    {
        Joy _default;
        _default.id = joy;
        _default.button = def;
        m_axisBinding[actionName] = _default;
    }

    return ((Joy)m_joyButtonBinding[actionName]).button;
}

Int32 Config::mouseButtonForAction(const std::string& action, sf::Mouse::Button def)
{
    std::string actionName = action;
    zelda::utility::tolower(actionName);
    if (m_mouseButtonBinding.find(actionName) == m_mouseButtonBinding.end())
        m_mouseButtonBinding[actionName] = def;

    return m_mouseButtonBinding[actionName];
}

void Config::unbind(const std::string& binding)
{
    std::string bindingName = binding;
    zelda::utility::tolower(bindingName);

    if (!bindingName.compare("all"))
    {
        m_keyBinding.clear();
        m_joyButtonBinding.clear();
        m_axisBinding.clear();
        m_mouseButtonBinding.clear();
        sEngineRef().console().print(Console::Info, "Cleared all active bindings");
        return;
    }
    std::string action;
    for (std::pair<std::string, sf::Keyboard::Key> key: m_keyBinding)
    {
        for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
        {
            if (!KeyInfo[i].name.compare(bindingName) && key.second == KeyInfo[i].code)
            {
                action = key.first;
                break;
            }
        }
    }
    if (action != std::string())
    {
        m_keyBinding.erase(action);
        return;
    }

    for (std::pair<std::string, sf::Mouse::Button> key: m_mouseButtonBinding)
    {
        for (int i = 0; i < sf::Mouse::ButtonCount - 1; i++)
        {
            if (!MouseButtonInfo[i].name.compare(bindingName) && key.second == i)
            {
                action = (key.first);
                break;
            }
        }
    }

    if (action != std::string())
    {
        m_mouseButtonBinding.erase(action);
        return;
    }

    for (std::pair<std::string, Joy> key: m_joyButtonBinding)
    {
        for (int i = 0; i < sf::Joystick::Count; i++)
        {
            for (int j = 0; j< sf::Joystick::ButtonCount; j++)
            {
                std::string button = zelda::utility::sprintf("joy%i.button%i", i, j);
                if (!button.compare(bindingName) && (key.second.id == i && key.second.button == j))
                {
                    action = key.first;
                    break;
                }
            }
        }
    }
    if (action != std::string())
    {
        m_joyButtonBinding.erase(action);
        return;
    }

    for (std::pair<std::string, Joy> key : m_axisBinding)
    {
        for (int i = 0; i < sf::Joystick::Count; i++)
        {
            for (int j = 0; j< sf::Joystick::AxisCount; j++)
            {
                std::string axis = zelda::utility::sprintf("joy%i.%s", i, AxisInfo[j].name.c_str());
                std::string negAxis = zelda::utility::sprintf("-joy%i.%s", i, AxisInfo[j].name.c_str());

                if ((!axis.compare(bindingName) || !negAxis.compare(bindingName)) && (key.second.id == i
                                                                                      && (sf::Joystick::Axis) j == key.second.axis))
                {
                    action = key.first;
                    break;
                }
            }
        }
    }


}

void Config::setSettingBoolean(const std::string& setting, bool val)
{
    std::stringstream ss;
    ss << val;

    setSettingLiteral(setting, ss.str());
}

bool Config::settingBoolean(const std::string& setting, bool def)
{
    bool valid;
    bool ret = zelda::utility::parseBool(settingLiteral(setting), valid);

    if (!valid)
    {
        setSettingBoolean(setting, def);
    }

    return ret;
}

void Config::setSettingInt(const std::string& setting, int val)
{
    std::stringstream ss;
    ss << val;
    setSettingLiteral(setting, ss.str());
}

int Config::settingInt(const std::string& setting, int def)
{
    if (settingLiteral(setting) == std::string())
        setSettingInt(setting, def);

    std::stringstream ss;
    ss << settingLiteral(setting);
    int ret;
    ss >> ret;

    return ret;
}

void Config::setSettingFloat(const std::string& setting, float val)
{
    std::stringstream ss;
    ss << val;
    setSettingLiteral(setting, ss.str());
}

float Config::settingFloat(const std::string& setting, float def)
{
    if (settingLiteral(setting) == std::string())
        setSettingFloat(setting, def);

    std::stringstream ss;
    ss << settingLiteral(setting);
    float ret;
    ss >> ret;

    return ret;
}

void Config::setSettingColor(const std::string& setting, const sf::Color& color)
{
    std::stringstream ss;
    ss << (int)color.r << " " <<
          (int)color.g << " " <<
          (int)color.b << " " <<
          (int)color.a;

    setSettingLiteral(setting, ss.str());
}

sf::Color Config::settingColor(const std::string& setting, const sf::Color& def)
{
    if (settingLiteral(setting) == std::string())
        setSettingColor(setting, def);

    int r, g, b, a;
    std::stringstream ss;
    std::string tmp = settingLiteral(setting);
    ss << tmp;
    ss >> r;
    ss >> g;
    ss >> b;
    ss >> a;

    return sf::Color(r, g, b, a);
}

void Config::setSettingLiteral(const std::string& setting, const std::string& val)
{
    if (setting == std::string() || val == std::string())
        return;

    if (val.find_first_of(' ') != std::string::npos && val.find_first_of('"') == std::string::npos)
        m_settings[setting] = "\"" + val + "\"";
    else
        m_settings[setting] = val;
}

void Config::cleanString(std::string& ret)
{
    if (ret == std::string())
        return;

    std::replace(ret.begin(), ret.end(), '"', ' ');
    while (ret.size() > 0 && ret.at(0) == ' ')
        ret.erase(0, 1);
    while (ret.size() > 0 && ret.at(ret.size() - 1) == ' ')
        ret.erase(ret.size() - 1, 1);
}

std::string Config::settingLiteral(const std::string& setting, const std::string& def)
{
    std::string ret = std::string(m_settings[setting]);

    if (ret != "")
    {
        std::replace(ret.begin(), ret.end(), '"', ' ');
        cleanString(ret);
    }
    else
    {
        setSettingLiteral(setting, def);
        ret = def;
        cleanString(ret);
    }

    return ret;
}

void Config::parse()
{
    std::vector<std::string> config = m_configFile->readAllLines();
    int lineNum = 0;
    for (std::string line : config)
    {
        lineNum++;
        // If the line starts with a comment don't try to parse it
        if (line.find("//") == 0)
            continue;

        if (line.find("/") == 0)
        {
            sEngineRef().console().print(Console::Warning, "Malformed comment at line %i", lineNum);
            continue;
        }

        bool done = false;
        // Remove comments
        if (line.find("//") != std::string::npos)
            line.erase(line.find("//"), line.find("//") - line.size());

        std::replace(line.begin(), line.end(), '\n', ' ');
        zelda::utility::tolower(line);
        line.erase(line.size());
        std::vector<std::string> tokens = zelda::utility::split(line, ' ');

        if (!tokens[0].compare("unbind"))
        {
            unbind(tokens[1]);
            continue;
        }
        if (!tokens[0].compare("bind"))
        {
            if (tokens.size() < 3)
            {
                sEngineRef().console().print(Console::Warning, "Malformed key binding at line %i", lineNum);
                continue;
            }

            for (int i = 0; i < sf::Keyboard::KeyCount - 1; i++)
            {
                if (!tokens[1].compare(KeyInfo[i].name))
                {
                    m_keyBinding[tokens[2]] = KeyInfo[i].code;
                    done = true;
                    break;
                }
            }

            if (done)
                continue;

            for (int i = 0; i < sf::Mouse::ButtonCount - 1; i++)
            {
                if (!tokens[1].compare(MouseButtonInfo[i].name))
                {
                    m_mouseButtonBinding[tokens[2]] = MouseButtonInfo[i].button;
                    done = true;
                    break;
                }
            }

            if (done)
                continue;
            for (int i = 0; i < sf::Joystick::Count - 1; i++)
            {
                if (done)
                    break;

                for (int j = 0; j < sf::Joystick::AxisCount; j++)
                {
                    if (done)
                        break;
                    bool neg = (tokens[1][0] == '-');

                    std::string axis = zelda::utility::sprintf("%sjoy%i.%s", (neg ? "-" : ""), i, AxisInfo[j].name.c_str());
                    if (!tokens[1].compare(axis))
                    {
                        Joy joy;
                        joy.id = i;
                        joy.button = -1;
                        joy.neg = neg;
                        joy.axis = (sf::Joystick::Axis)j;
                        std::string action = zelda::utility::sprintf("%s%i", tokens[2].c_str(), i);
                        m_axisBinding[action] = joy;
                        done = true;
                        break;
                    }
                }

                for (int j = 0; j < sf::Joystick::ButtonCount; j++)
                {
                    if (done)
                        break;

                    std::string button = zelda::utility::sprintf("joy%i.button%i", i, j);
                    if (!tokens[1].compare(button))
                    {
                        Joy joy;
                        joy.id = i;
                        joy.button = j;
                        std::string action = zelda::utility::sprintf("%s%i", tokens[2].c_str(), i);
                        m_joyButtonBinding[action] = joy;
                        done = true;
                        break;
                    }
                }
            }
        }
        else if (!tokens[0].compare("set"))
        {
            if (tokens.size() < 3)
            {
                sEngineRef().console().print(Console::Warning, "Malformed set variable at line %i", lineNum);
                continue;
            }


            std::string setting = tokens[1];
            std::string val;
            if (tokens.size() > 3)
            {
                std::string l2 = m_configFile->readLineAt(lineNum);
                std::replace(l2.begin(), l2.end(), '\n', ' ');

                std::vector<std::string> tokens2 = zelda::utility::split(l2, ' ');
                tokens2.erase(tokens2.begin());
                tokens2.erase(tokens2.begin());

                for (std::string s : tokens2)
                    val += s + " ";

                int lastIndex;

                if (val.find_last_of('"') == std::string::npos || (zelda::utility::countChar(val, '"', lastIndex) % 2))
                    sEngineRef().console().print(Console::Warning, "Malformed literal at line %i", lineNum);
            }
            else
            {
                val = tokens[2];

            }


            m_settings[setting] = val;
        }
    }

    sEngineRef().console().print(Console::Info, "Config loaded");
}
} // Core
} // Sakura
