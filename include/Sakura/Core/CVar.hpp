#ifndef CVAR_HPP
#define CVAR_HPP

#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Graphics/Color.hpp>

class TiXmlNode;

namespace Sakura
{
namespace Core
{
struct Joystick
{
    enum JoyAxis
    {
        None = -1,
        X    = sf::Joystick::X,
        Y    = sf::Joystick::Y,
        R    = sf::Joystick::R,
        U    = sf::Joystick::U,
        V    = sf::Joystick::V,
        PovX = sf::Joystick::PovX,
        PovY = sf::Joystick::PovY
    };

    int     Button;
    JoyAxis Axis;
    bool    NegativeAxis;
};

enum MouseButton
{
    UnknownButton = -1,
    MouseLeft,
    MouseRight,
    MouseXButton1,
    MouseXButton2
};

class CVar
{
public:

    class Binding
    {
    public:
        sf::Keyboard::Key Key;
        MouseButton       Button;
        Joystick          Joysticks[sf::Joystick::Count];

        Binding();
        void clear();
    };

    enum Type
    {
        Boolean,
        Integer,
        Float,
        Literal,
        Color,
        Bind
    };

    enum Flags
    {
        All      = -1,
        System   = (1 << 0),
        Game     = (1 << 1),
        Gui      = (1 << 2),
        Cheat    = (1 << 3),
        ReadOnly = (1 << 4),
        Archive  = (1 << 5),
        Modified = (1 << 6)
    };

    CVar(const std::string& name, const std::string& value, const std::string& help, Type type, int flags);
    CVar(const std::string& name, Binding value, const std::string& help, int flags);
    CVar(const std::string& name, const sf::Color& value, const std::string& help, int flags);



    std::string name()                         const;
    std::string help()                         const;
    float       toFloat(bool* isValid   =NULL) const;
    bool        toBoolean(bool* isValid =NULL) const;
    int         toInteger(bool* isValid =NULL) const;
    std::string toLiteral(bool* isValid =NULL) const;
    sf::Color   toColor  (bool* isValid =NULL) const;
    Binding     toBinding(bool* isValid =NULL) const;

    bool fromFloat  (const float val);
    bool fromBoolean(const bool val);
    bool fromInteger(const int val);
    bool fromLiteral(const std::string& val);
    bool fromColor  (const sf::Color& val);
    bool fromBinding(Binding binding);

    bool isFloat()       const;
    bool isBoolean()     const;
    bool isInteger()     const;
    bool isLiteral()     const;
    bool isBinding()     const;
    bool isModified()    const;
    bool isReadOnly()    const;
    bool isCheat()       const;
    bool isArchive()     const;
    void clearModified();
    void setModified();
    void clearBindings();

    Type type() const;
    int flags() const;

    void deserialize(TiXmlNode* rootNode);
    void serializeCVar(TiXmlNode* rootNode, bool oldDeveloper);
    void serializeBinding(TiXmlNode* rootNode);

    /*!
     * \brief Unlocks the CVar for writing if it is ReadOnly.
     * <b>Handle with care!!!</b> if you use unlock(), make sure
     * you lock the cvar using lock()
     * \see lock
     */
    void unlock();

    /*!
     * \brief Locks the CVar to prevent writing if it is ReadOnly.
     * Unlike it's partner function unlock, lock is harmless
     * \see unlock
     */
    void lock();
protected:
    std::string m_name;
    std::string m_value;
    std::string m_defaultValue;
    std::string m_help;
    Binding     m_binding;
    Type        m_type;
    int         m_flags;
    bool        m_allowedWrite;
};

} // Core
} // Sakura


#endif // CVAR_HPP

