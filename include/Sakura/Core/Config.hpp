// This file is part of Sakura.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura.  If not, see <http://www.gnu.org/licenses/>

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

namespace Sakura
{
namespace Core
{
class Config
{
public:
    /*!
     * \brief Config
     */
    Config();
    ~Config();


    /*!
     * \brief Initializes the Config subsystem with the specified configuration file
     * \param configFile The configuration file to load
     */
    void initialize     (const std::string& configFile);

    /*!
     * \brief shutdown
     */
    void shutdown();

    /*!
     * \brief Associates a command with an action
     * \param action The action to bind e.g "player1.walkleft"
     * \param commandString The command to bind eg "-joy7.x"
     * \return
     */
    bool bind           (const std::string& action, const std::string& commandString);

    /*!
     * \brief Associates a Key with an action
     * \param action The action to bind e.g "player1.walkleft"
     * \param key The key to bind eg "a"
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
     * \brief Returns the joysticks axis for a given action e.g "player1.walkleft"
     * \param action The requested action
     * \param joy The requested joystick
     * \param defAxis The default axis, used if one isn't specified.
     * \param defNeg The default axis direction, true if negative, false if positive
     * \param neg The value stored
     * \return Int32 The axis for the requested action
     */
    Int32 joyAxisForAction(const std::string& action, int joy, sf::Joystick::Axis defAxis, bool defNeg, bool& neg);

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
     * \brief <i>unbind</i> Removes the given action from the binding map
     * \param binding The binding to remove
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
} // Core
} // Sakura

#endif // CONFIG_HPP
