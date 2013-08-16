#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <TextStream.hpp>

class Config
{
public:
    Config();
    ~Config();


    void initialize     (const std::string& configFile);
    void shutdown();
    bool bind           (const std::string& action, const std::string& commandString);
    void bindKey        (const std::string& action, sf::Keyboard::Key key);
    void bindJoyAxis    (const std::string& action, int joy, bool neg, sf::Joystick::Axis axis);
    void bindJoyButton  (const std::string& action, int joy, int button);
    void bindMouseButton(const std::string& action, sf::Mouse::Button button);

    sf::Keyboard::Key keyForAction(const std::string& action, sf::Keyboard::Key def);

    Int32 joyAxisForAction(const std::string& action, int joy, sf::Joystick::Axis def, bool defNeg, bool& neg);
    Int32 joyButtonForAction(const std::string& action, int joy, int def);

    Int32 mouseButtonForAction(const std::string& action, sf::Mouse::Button def);
    void unbind(const std::string& binding);


    void setSettingBoolean(const std::string& setting, bool val);
    bool settingBoolean(const std::string& setting, bool def);
    void setSettingInt(const std::string& setting, int val);
    int settingInt(const std::string& setting, int def);
    void setSettingFloat(const std::string& setting, float val);
    float settingFloat(const std::string& setting, float def);
    void setSettingColor(const std::string& setting, const sf::Color& color);
    sf::Color settingColor(const std::string& setting, const sf::Color& def);
    void setSettingLiteral(const std::string& setting, const std::string& val);
    std::string settingLiteral(const std::string& setting, const std::string& def = "");
    void cleanString(std::string& ret);
private:
    void parse();
    struct Joy
    {
        int id;
        int button;
        bool neg;
        sf::Joystick::Axis axis;
    };

    std::unordered_map<std::string, sf::Keyboard::Key>  m_keyBinding;
    std::unordered_map<std::string, Joy>                m_axisBinding;
    std::unordered_map<std::string, Joy>                m_joyButtonBinding;
    std::unordered_map<std::string, sf::Mouse::Button>  m_mouseButtonBinding;
    std::unordered_map<std::string, std::string>        m_settings;

    zelda::io::TextStream* m_configFile;

    //std::fstream m_configFile;
};

#endif // CONFIG_HPP
