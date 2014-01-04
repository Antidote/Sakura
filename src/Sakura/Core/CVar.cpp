#include "Sakura/Core/Engine.hpp"
#include "Sakura/Core/CVar.hpp"
#include "Sakura/Core/CVarManager.hpp"
#include "Sakura/Core/Keys.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <tinyxml.h>

namespace Sakura
{
namespace Core
{
CVar::CVar(const std::string &cvar, const std::string &value, const std::string &help, Type type, int flags)
    : m_name(cvar),
      m_value(value),
      m_defaultValue(value),
      m_help(help),
      m_type(type),
      m_flags(flags)
{
    clearBindings();
}

std::string CVar::name() const
{
    return m_name;
}

std::string CVar::help() const
{
    return std::string(m_help + " default: " + m_defaultValue);
}

float CVar::toFloat(bool* isValid) const
{
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Float)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Cvar " << m_name << " is not a float..." << std::endl;
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
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Boolean)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Cvar " << m_name << " is not a boolean..." << std::endl;
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
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Integer)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Cvar " << m_name << " is not an integer..." << std::endl;
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

std::string CVar::toLiteral() const
{
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Literal  && (com_developer && com_developer->toBoolean()))
        std::cout << "Cvar " << m_name << "is not a literal..." << std::endl;

    // Even if it's not a literal, it's still safe to return
    return m_value;
}

CVar::Binding CVar::toBinding() const
{
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Bind)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "CVar " << m_name << " is not a binding" << std::endl;

        return Binding();
    }

    return m_binding;
}

bool CVar::fromFloat(const float val)
{
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Float)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Invalid assignment, CVar " << m_name << " is not a float"<< std::endl;

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
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Boolean)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Invalid assignment, CVar " << m_name << " is not a boolean"<< std::endl;
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
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Integer)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Invalid assignment, CVar " << m_name << " is not an integer"<< std::endl;
        return false;
    }

    std::stringstream ss;
    ss << val;
    m_value = ss.str();
    m_flags |= Modified;
    return true;
}

bool CVar::fromLiteral(const std::string &val)
{
    m_value = val;
    m_flags |= Modified;
    return true;
}

bool CVar::fromBinding(CVar::Binding binding)
{
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    if (m_type != Bind)
    {
        if (com_developer && com_developer->toBoolean())
            std::cout << "Invalid assignment, CVar " << m_name << " is not an binding"<< std::endl;
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

void CVar::clearModified()
{
    m_flags &= ~Modified;
}

void CVar::setModified()
{
    m_flags |= Modified;
}

void CVar::clearBindings()
{
    m_binding.Key    = sf::Keyboard::Unknown;
    m_binding.Button = UnknownButton;

    for (int i = 0; i < sf::Joystick::Count; i++)
    {
        m_binding.Joysticks[i].Button       = -1;
        m_binding.Joysticks[i].Axis         = Joystick::None;
        m_binding.Joysticks[i].NegativeAxis = false;
    }
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
    UNUSED(rootNode);
}

void CVar::serializeCVar(TiXmlNode* rootNode, bool oldDeveloper)
{
    CVar* com_developer = sEngineRef().cvarManager().findCVar("developer");
    TiXmlElement* cvarNode = rootNode->FirstChildElement("CVar");

    while (cvarNode != NULL)
    {
        if (!std::string(cvarNode->Attribute("name")).compare(name()))
            break;
        cvarNode = cvarNode->NextSiblingElement("CVar");
    }
    if (isModified() || !cvarNode)
    {
        TiXmlText* text = NULL;
        TiXmlText* oldText = NULL;


        if (!cvarNode)
        {
            cvarNode = new TiXmlElement("CVar");
            cvarNode->SetAttribute("name", name());
            rootNode->LinkEndChild(cvarNode);
        }
        else
            oldText = cvarNode->FirstChild()->ToText();

        switch(type())
        {
            case CVar::Boolean:
            {
                cvarNode->SetAttribute("type", "boolean");
                std::string val;
                if (name() == com_developer->name())
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
            default: break;
        }

        if (!text)
            text = new TiXmlText(toLiteral());

        if (oldText)
        {
            cvarNode->RemoveChild(oldText);
        }

        cvarNode->LinkEndChild(text);
    }
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

    if (isModified() || !bindNode)
    {
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
}

} // Core
} // Sakura
