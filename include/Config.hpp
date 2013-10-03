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
    /*!
     * \brief Config
     */
    Config();
    ~Config();


    /*!
     * \brief initialize
     * \param configFile
     */
    void initialize     (const std::string& configFile);

    /*!
     * \brief shutdown
     */
    void shutdown();

    /*!
     * \brief bind
     * \param action
     * \param commandString
     * \return
     */
    bool bind           (const std::string& action, const std::string& commandString);

    /*!
     * \brief bindKey
     * \param action
     * \param key
     */
    void bindKey        (const std::string& action, sf::Keyboard::Key key);

    /*!
     * \brief bindJoyAxis
     * \param action
     * \param joy
     * \param neg
     * \param axis
     */
    void bindJoyAxis    (const std::string& action, int joy, bool neg, sf::Joystick::Axis axis);

    /*!
     * \brief bindJoyButton
     * \param action
     * \param joy
     * \param button
     */
    void bindJoyButton  (const std::string& action, int joy, int button);

    /*!
     * \brief bindMouseButton
     * \param action
     * \param button
     */
    void bindMouseButton(const std::string& action, sf::Mouse::Button button);

    /*!
     * \brief keyForAction
     * \param action
     * \param def
     * \return
     */
    sf::Keyboard::Key keyForAction(const std::string& action, sf::Keyboard::Key def);

    /*!
     * \brief joyAxisForAction
     * \param action
     * \param joy
     * \param def
     * \param defNeg
     * \param neg
     * \return
     */
    Int32 joyAxisForAction(const std::string& action, int joy, sf::Joystick::Axis def, bool defNeg, bool& neg);

    /*!
     * \brief joyButtonForAction
     * \param action
     * \param joy
     * \param def
     * \return
     */
    Int32 joyButtonForAction(const std::string& action, int joy, int def);

    /*!
     * \brief mouseButtonForAction
     * \param action
     * \param def
     * \return
     */
    Int32 mouseButtonForAction(const std::string& action, sf::Mouse::Button def);

    /*!
     * \brief unbind
     * \param binding
     */
    void unbind(const std::string& binding);

    /*!
     * \brief setSettingBoolean
     * \param setting
     * \param val
     */
    void setSettingBoolean(const std::string& setting, bool val);

    /*!
     * \brief settingBoolean
     * \param setting
     * \param def
     * \return
     */
    bool settingBoolean(const std::string& setting, bool def);

    /*!
     * \brief setSettingInt
     * \param setting
     * \param val
     */
    void setSettingInt(const std::string& setting, int val);

    /*!
     * \brief settingInt
     * \param setting
     * \param def
     * \return
     */
    int settingInt(const std::string& setting, int def);

    /*!
     * \brief setSettingFloat
     * \param setting
     * \param val
     */
    void setSettingFloat(const std::string& setting, float val);

    /*!
     * \brief settingFloat
     * \param setting
     * \param def
     * \return
     */
    float settingFloat(const std::string& setting, float def);

    /*!
     * \brief setSettingColor
     * \param setting
     * \param color
     */
    void setSettingColor(const std::string& setting, const sf::Color& color);

    /*!
     * \brief settingColor
     * \param setting
     * \param def
     * \return
     */
    sf::Color settingColor(const std::string& setting, const sf::Color& def);

    /*!
     * \brief setSettingLiteral
     * \param setting
     * \param val
     */
    void setSettingLiteral(const std::string& setting, const std::string& val);

    /*!
     * \brief settingLiteral
     * \param setting
     * \param def
     * \return
     */
    std::string settingLiteral(const std::string& setting, const std::string& def = "");
private:
    void cleanString(std::string& ret);
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
