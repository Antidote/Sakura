#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/CVar.hpp"
#include "Sakura/Core/CVarManager.hpp"
#include "Sakura/Core/Keys.hpp"
#include <sstream>
#include <algorithm>
#include <tinyxml.h>
#include <utility.hpp>

namespace Sakura
{
namespace Core
{
extern CVar* com_developer;

CVar::CVar(const std::string& name, const std::string &value, const std::string &help, Type type, int flags)
    : m_name(name),
      m_value(value),
      m_defaultValue(value),
      m_help(help),
      m_type(type),
      m_flags(flags),
      m_allowedWrite(false)
{
    sEngineRef().cvarManager().registerCVar(this);
}

CVar::CVar(const std::string& name, CVar::Binding value, const std::string& help, int flags)
    : m_name(name),
      m_help(help),
      m_binding(value),
      m_type(Bind),
      m_flags(flags),
      m_allowedWrite(false)
{
    sEngineRef().cvarManager().registerCVar(this);
}

CVar::CVar(const std::string& name, const sf::Color& value, const std::string& help, int flags)
    : m_name(name),
      m_help(help),
      m_type(Color),
      m_flags(flags),
      m_allowedWrite(false)
{
    // Unlock the cvar for writing if readonly
    unlock();

    fromColor(value);

    // Lock the cvar
    lock();
    // Clear the modified flag, just incase lock didn't do it.
    m_flags = flags;

    sEngineRef().cvarManager().registerCVar(this);
}

std::string CVar::name() const
{
    return m_name;
}

std::string CVar::help() const
{
    return std::string(m_help + (m_defaultValue != std::string() ? "\ndefault: " + m_defaultValue : "") +
                       (isReadOnly() ? "[ReadOnly]" : ""));
}

float CVar::toFloat(bool* isValid) const
{
    if (m_type != Float)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "CVar %s is not a float", name().c_str());
        if (isValid)
            *isValid = false;
        return 0.0f;
    }

    try
    {
        float ret;
        std::stringstream ss;
        ss << m_value;
        ss >> ret;

        *isValid = true;
        return ret;
    }
    catch (...)
    {
        if (isValid)
            *isValid = false;
        return 0.0f;
    }
}

bool CVar::toBoolean(bool* isValid) const
{
    if (m_type != Boolean)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "CVar %s is not a boolean", name().c_str());
        if (isValid)
            *isValid = false;

        return false;
    }

    // We don't want to modify the original value;
    std::string tmp = m_value;
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);

    if (!tmp.compare("yes") || !tmp.compare("true") || !tmp.compare("1"))
    {
        if (isValid)
            *isValid = true;
        return true;
    }
    else if (!tmp.compare("no") || !tmp.compare("false") || !tmp.compare("0"))
    {
        if (isValid)
            *isValid = true;
        return false;
    }

    if (isValid)
        *isValid = false;

    return false;
}

int CVar::toInteger(bool* isValid) const
{
    if (m_type != Integer)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "CVar %s is not an integer", name().c_str());
        if (isValid)
            *isValid = false;
        return -1;
    }

    try
    {
        int ret;
        std::stringstream ss;
        ss << m_value;
        ss >> ret;
        if (isValid)
            *isValid = true;
        return ret;
    }
    catch(...)
    {
        if (isValid)
            *isValid = false;
        return -1;
    }
}

std::string CVar::toLiteral(bool* isValid) const
{
    if (m_type != Literal  && (com_developer && com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is not a literal", name().c_str());
        if (isValid != NULL)
            *isValid = false;
    }
    else if (isValid != NULL)
        *isValid = true;

    // Even if it's not a literal, it's still safe to return
    return m_value;
}

sf::Color CVar::toColor(bool* isValid) const
{
    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "CVar %s is not a binding", name().c_str());

        if (isValid != NULL)
            *isValid = false;

        return sf::Color();
    }

    if (isValid != NULL)
        *isValid = true;

    int r, g, b, a;
    std::stringstream ss;

    ss << m_value;
    ss >> r;
    ss >> g;
    ss >> b;
    ss >> a;

    return sf::Color(r, g, b, a);
}

CVar::Binding CVar::toBinding(bool* isValid) const
{
    if (m_type != Bind)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "CVar %s is not a binding", name().c_str());

        if (isValid != NULL)
            *isValid = false;

        return Binding();
    }

    if (isValid != NULL)
        *isValid = true;

    return m_binding;
}

bool CVar::fromFloat(const float val)
{
    if (isCheat() && (com_developer && !com_developer->toBoolean()))
        return false;

    if (m_type != Float)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "Invalid assignment, CVar %s is not a float", name().c_str());

        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is readonly.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromBoolean(const bool val)
{
    if (isCheat() && (com_developer && !com_developer->toBoolean()))
        return false;

    if (m_type != Boolean)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "Invalid assignment, CVar %s is not a boolean", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is readonly.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromInteger(const int val)
{
    if (isCheat() && (com_developer && !com_developer->toBoolean()))
        return false;

    if (m_type != Integer)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "Invalid assignment, CVar %s is not an integer", name().c_str());

        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is readonly.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromLiteral(const std::string& val)
{
    if (isCheat() && (com_developer && !com_developer->toBoolean()))
        return false;

    if (m_type != Literal)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "Invalid assignment, CVar %s is not a literal", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    m_value = val;
    m_flags |= Modified;
    return true;
}

bool CVar::fromColor(const sf::Color& val)
{
    if (isCheat() && (com_developer && !com_developer->toBoolean()))
        return false;

    if (m_type != Color)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "Invalid assignment, CVar %s is not a color", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    std::stringstream ss;
    ss << (int)val.r << " " <<
          (int)val.g << " " <<
          (int)val.b << " " <<
          (int)val.a;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromBinding(CVar::Binding binding)
{
    if (isCheat() && (com_developer && !com_developer->toBoolean()))
        return false;

    if (m_type != Bind)
    {
        if (com_developer && com_developer->toBoolean())
            sEngineRef().console().print(Console::Warning, "Invalid assignment, CVar %s is not a binding", name().c_str());
        return false;
    }

    if (isReadOnly() && (com_developer && !com_developer->toBoolean()))
    {
        sEngineRef().console().print(Console::Warning, "CVar %s is readonly, restart in developer mode.", name().c_str());
        return false;
    }

    m_binding = binding;
    return true;
}

bool CVar::isFloat() const
{
    return m_type == Float;
}

bool CVar::isBoolean() const
{
    return m_type == Boolean;
}

bool CVar::isInteger() const
{
    return m_type == Integer;
}

bool CVar::isLiteral() const
{
    return m_type == Literal;
}

bool CVar::isBinding() const
{
    return m_type == Bind;
}

bool CVar::isModified() const
{
    return (m_flags & Modified) == Modified;
}

bool CVar::isReadOnly() const
{
    return (m_flags & ReadOnly) == ReadOnly;
}

bool CVar::isCheat() const
{
    return (m_flags & Cheat) == Cheat;
}

bool CVar::isArchive() const
{
    return (m_flags & Archive) == Archive;
}

void CVar::clearModified()
{
    if (isArchive())
        sEngineRef().cvarManager().requestSerialization(this);
    m_flags &= ~Modified;
}

void CVar::setModified()
{
    m_flags |= Modified;
}

void CVar::clearBindings()
{
    m_binding.clear();
}

CVar::Type CVar::type() const
{
    return m_type;
}

int CVar::flags() const
{
    return m_flags;
}

void CVar::deserialize(TiXmlNode* rootNode)
{
    int oldFlags = m_flags;
    unlock();
    if (rootNode == NULL)
        return;
    TiXmlElement* cvarNode = NULL;
    if (type() != Bind)
        cvarNode = rootNode->FirstChildElement("CVar");
    else
        cvarNode = rootNode->FirstChildElement("Bind");

    while (cvarNode != NULL)
    {
        if (type() != Bind)
        {
            if (!std::string(cvarNode->Attribute("name")).compare(name()))
                break;

            cvarNode = cvarNode->NextSiblingElement("CVar");
        }
        else
        {
            if (!std::string(cvarNode->Attribute("action")).compare(name()))
                break;

            cvarNode = cvarNode->NextSiblingElement("Bind");
        }
    }

    if (!cvarNode)
        return;

    switch(type())
    {
        case Bind:
        {
            std::string tmp = cvarNode->Attribute("key");
            zelda::utility::tolower(tmp);
            for (int k = 0; k < sf::Keyboard::KeyCount; k++)
            {
                if (!KeyInfo[k].name.compare(tmp))
                    m_binding.Key = KeyInfo[k].code;
            }

            tmp = cvarNode->Attribute("mouseButton");
            for (int m = 0; m < sf::Mouse::ButtonCount; m++)
            {
                if (!MouseButtonInfo[m].name.compare(tmp))
                    m_binding.Button = (MouseButton)MouseButtonInfo[m].button;
            }

            for (int j = 0; j < sf::Joystick::Count; j++)
            {
                std::stringstream ss;
                ss << j;
                TiXmlElement* joyNode = cvarNode->FirstChildElement("Joy");
                while (joyNode != NULL)
                {
                    if (!std::string(joyNode->Attribute("id")).compare(ss.str()))
                        break;
                    joyNode = joyNode->NextSiblingElement("Joy");
                }

                if (!joyNode)
                    continue;

                int button;
                joyNode->Attribute("button", &button);
                m_binding.Joysticks[j].Button = button;
                tmp = joyNode->Attribute("axis");
                zelda::utility::tolower(tmp);

                for (int a = 0; a < sf::Joystick::AxisCount; a++)
                {
                    if (!AxisInfo[a].name.compare(tmp))
                        m_binding.Joysticks[j].Axis = (Joystick::JoyAxis)(AxisInfo[a].axis);
                }
                tmp = joyNode->Attribute("isAxisNegative");
                m_binding.Joysticks[j].NegativeAxis = zelda::utility::parseBool(tmp);

            }
        }
        break;
        case Color:
        {
            int r, g, b, a;
            cvarNode->Attribute("r", &r);
            cvarNode->Attribute("g", &g);
            cvarNode->Attribute("b", &b);
            cvarNode->Attribute("a", &a);
            fromColor(sf::Color(r, g, b, a));
        }
        break;
        default:
            m_value = cvarNode->FirstChild()->ToText()->ValueStr();
        break;
    }
    lock();
    m_flags = oldFlags;
}

void CVar::serializeCVar(TiXmlNode* rootNode, bool oldDeveloper)
{
    if (rootNode == NULL)
        return;

    TiXmlElement* cvarNode = rootNode->FirstChildElement("CVar");

    while (cvarNode != NULL)
    {
        if (!std::string(cvarNode->Attribute("name")).compare(name()))
            break;
        cvarNode = cvarNode->NextSiblingElement("CVar");
    }

    TiXmlText* text = NULL;
    TiXmlText* oldText = NULL;


    if (!cvarNode)
    {
        cvarNode = new TiXmlElement("CVar");
        cvarNode->SetAttribute("name", name());
        rootNode->LinkEndChild(cvarNode);
    }
    else
    {
        if (cvarNode->FirstChild())
        {
            oldText = cvarNode->FirstChild()->ToText();
        }
    }

    switch(type())
    {
        case CVar::Boolean:
        {
            cvarNode->SetAttribute("type", "boolean");
            std::string val;
            if (com_developer == this)
                val = (oldDeveloper ? "true" : "false");
            else
                val = (toBoolean() ? "true" : "false");

            text = new TiXmlText(val);
            break;
        }
        case CVar::Integer:
            cvarNode->SetAttribute("type", "integer");
        break;
        case CVar::Float:
            cvarNode->SetAttribute("type", "float");
        break;
        case CVar::Literal:
        {
            cvarNode->SetAttribute("type", "literal");
            text = new TiXmlText(toLiteral());
            text->SetCDATA(true);
            break;
        }
        case CVar::Color:
        {
            sf::Color col = toColor();
            cvarNode->SetAttribute("type", "color");
            cvarNode->SetAttribute("r", (int)col.r);
            cvarNode->SetAttribute("g", (int)col.g);
            cvarNode->SetAttribute("b", (int)col.b);
            cvarNode->SetAttribute("a", (int)col.a);
        }
            break;
        default: break;
    }

    if (!text && type() != Color)
        text = new TiXmlText(toLiteral());

    if (oldText && type() != Color)
    {
        cvarNode->RemoveChild(oldText);
    }

    if (text && type() != Color)
        cvarNode->LinkEndChild(text);
}

void CVar::serializeBinding(TiXmlNode* rootNode)
{
    TiXmlElement* bindNode = rootNode->FirstChildElement("Bind");

    while (bindNode != NULL)
    {
        if (!std::string(bindNode->Attribute("action")).compare(name()))
            break;

        bindNode = bindNode->NextSiblingElement("Bind");
    }

    if (!bindNode)
    {
        bindNode = new TiXmlElement("Bind");
        bindNode->SetAttribute("action", name());
        rootNode->LinkEndChild(bindNode);
    }

    bindNode->SetAttribute("key", (m_binding.Key == sf::Keyboard::Unknown ? "unknown" : KeyInfo[m_binding.Key].name));
    bindNode->SetAttribute("mouseButton", (m_binding.Button == UnknownButton ? "unknown" : MouseButtonInfo[m_binding.Button].name));

    for (int i = 0; i < sf::Joystick::Count; i++)
    {
        std::stringstream ss;
        ss << i;
        TiXmlElement* joyNode = bindNode->FirstChildElement("Joy");
        while (joyNode != NULL)
        {
            if (!std::string(joyNode->Attribute("id")).compare(ss.str()))
                break;

            joyNode = joyNode->NextSiblingElement("Joy");
        }

        if (!joyNode)
        {
            joyNode = new TiXmlElement("Joy");
            joyNode->SetAttribute("id", i);
            bindNode->LinkEndChild(joyNode);
        }

        joyNode->SetAttribute("button", m_binding.Joysticks[i].Button);
        joyNode->SetAttribute("axis", (m_binding.Joysticks[i].Axis == Joystick::None ? "unknown" : AxisInfo[m_binding.Joysticks[i].Axis].name));
        joyNode->SetAttribute("isAxisNegative", (m_binding.Joysticks[i].NegativeAxis ? "true" : "false"));
    }
}

void CVar::unlock()
{
    if (!isReadOnly())
        return;

    if (!m_allowedWrite)
    {
        m_allowedWrite = true;
        m_flags &= ~ReadOnly;
    }
}

void CVar::lock()
{
    if (!isReadOnly())
        return;

    if (m_allowedWrite)
    {
        m_flags |= ReadOnly;
        m_allowedWrite = false;
        clearModified();
    }
}

CVar::Binding::Binding()
{
    clear();
}

void CVar::Binding::clear()
{
    Key    = sf::Keyboard::Unknown;
    Button = UnknownButton;

    for (int i = 0; i < sf::Joystick::Count; i++)
    {
        Joysticks[i].Button       = -1;
        Joysticks[i].Axis         = Joystick::None;
        Joysticks[i].NegativeAxis = false;
    }
}

} // Core
} // Sakura
