#ifndef JOYSTICKMANAGER_HPP
#define JOYSTICKMANAGER_HPP

#include "InputBase.hpp"
#include <SFML/Window/Joystick.hpp>
#include <map>
#include <vector>
class JoystickManager : public InputBase
{
public:
    JoystickManager();
    void update();

    bool isButtonPressed(int joystick, int button) const;
    bool wasButtonReleased(int joystick, int button) const;
private:

    class Joystick
    {
    public:
        Joystick(int id);
        ~Joystick();
        void update();

        bool hasAxis(sf::Joystick::Axis axis) const;
        float axisPosition(sf::Joystick::Axis axis) const;
        bool isButtonPressed(int button) const;
        bool wasButtonReleased(int button) const;
    private:
        int                m_id;
        std::vector<bool>  m_hasAxis;
        std::vector<float> m_axisPos;
        std::vector<bool>  m_buttons;
        std::vector<bool>  m_releasedButtons;
    };

    std::vector<Joystick*> m_joysticks;
};

#endif // JOYSTICKMANAGER_HPP
