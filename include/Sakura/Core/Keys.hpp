#ifndef KEYS_HPP
#define KEYS_HPP
#ifndef DOXYGEN

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Joystick.hpp>
#include <string>

struct KeyCode
{
    std::string name;
    sf::Keyboard::Key code;
};

static const KeyCode KeyInfo[101] =
{
    {"a",        sf::Keyboard::A},
    {"b",        sf::Keyboard::B},
    {"c",        sf::Keyboard::C},
    {"d",        sf::Keyboard::D},
    {"e",        sf::Keyboard::E},
    {"f",        sf::Keyboard::F},
    {"g",        sf::Keyboard::G},
    {"h",        sf::Keyboard::H},
    {"i",        sf::Keyboard::I},
    {"j",        sf::Keyboard::J},
    {"k",        sf::Keyboard::K},
    {"l",        sf::Keyboard::L},
    {"m",        sf::Keyboard::M},
    {"n",        sf::Keyboard::N},
    {"o",        sf::Keyboard::O},
    {"p",        sf::Keyboard::P},
    {"q",        sf::Keyboard::Q},
    {"r",        sf::Keyboard::R},
    {"s",        sf::Keyboard::S},
    {"t",        sf::Keyboard::T},
    {"u",        sf::Keyboard::U},
    {"v",        sf::Keyboard::V},
    {"w",        sf::Keyboard::W},
    {"x",        sf::Keyboard::X},
    {"y",        sf::Keyboard::Y},
    {"z",        sf::Keyboard::Z},
    {"0",        sf::Keyboard::Num0},
    {"1",        sf::Keyboard::Num1},
    {"2",        sf::Keyboard::Num2},
    {"3",        sf::Keyboard::Num3},
    {"4",        sf::Keyboard::Num4},
    {"5",        sf::Keyboard::Num5},
    {"6",        sf::Keyboard::Num6},
    {"7",        sf::Keyboard::Num7},
    {"8",        sf::Keyboard::Num8},
    {"9",        sf::Keyboard::Num9},
    {"esc",      sf::Keyboard::Escape},
    {"lcontrol", sf::Keyboard::LControl},
    {"lshift",   sf::Keyboard::LShift},
    {"lalt",     sf::Keyboard::LAlt},
    {"lsys",     sf::Keyboard::LSystem},
    {"rcontrol", sf::Keyboard::RControl},
    {"rshift",   sf::Keyboard::RShift},
    {"ralt",     sf::Keyboard::RAlt},
    {"rsys",     sf::Keyboard::LSystem},
    {"menu",     sf::Keyboard::Menu},
    {"[",        sf::Keyboard::LBracket},
    {"]",        sf::Keyboard::RBracket},
    {";",        sf::Keyboard::SemiColon},
    {",",        sf::Keyboard::Comma},
    {".",        sf::Keyboard::Period},
    {"'",        sf::Keyboard::Quote},
    {"/",        sf::Keyboard::Slash},
    {"\\",       sf::Keyboard::BackSlash},
    {"`",        sf::Keyboard::Tilde},
    {"=",        sf::Keyboard::Equal},
    {"dash",     sf::Keyboard::Dash},
    {"space",    sf::Keyboard::Space},
    {"return",   sf::Keyboard::Return},
    {"backspace",sf::Keyboard::BackSpace},
    {"tab",      sf::Keyboard::Tab},
    {"pageup",   sf::Keyboard::PageUp},
    {"pagedown", sf::Keyboard::PageDown},
    {"end",      sf::Keyboard::End},
    {"home",     sf::Keyboard::Home},
    {"insert",   sf::Keyboard::Insert},
    {"delete",   sf::Keyboard::Delete},
    {"+",        sf::Keyboard::Add},
    {"-",        sf::Keyboard::Subtract},
    {"*",        sf::Keyboard::Multiply},
    {"num/",     sf::Keyboard::Divide},
    {"left",     sf::Keyboard::Left},
    {"right",    sf::Keyboard::Right},
    {"up",       sf::Keyboard::Up},
    {"down",     sf::Keyboard::Down},
    {"num0",     sf::Keyboard::Numpad0},
    {"num1",     sf::Keyboard::Numpad1},
    {"num2",     sf::Keyboard::Numpad2},
    {"num3",     sf::Keyboard::Numpad3},
    {"num4",     sf::Keyboard::Numpad4},
    {"num5",     sf::Keyboard::Numpad5},
    {"num6",     sf::Keyboard::Numpad6},
    {"num7",     sf::Keyboard::Numpad7},
    {"num8",     sf::Keyboard::Numpad8},
    {"num9",     sf::Keyboard::Numpad9},
    {"f1",       sf::Keyboard::F1},
    {"f2",       sf::Keyboard::F2},
    {"f3",       sf::Keyboard::F3},
    {"f4",       sf::Keyboard::F4},
    {"f5",       sf::Keyboard::F5},
    {"f6",       sf::Keyboard::F6},
    {"f7",       sf::Keyboard::F7},
    {"f8",       sf::Keyboard::F8},
    {"f9",       sf::Keyboard::F9},
    {"f10",      sf::Keyboard::F10},
    {"f11",      sf::Keyboard::F11},
    {"f12",      sf::Keyboard::F12},
    {"f13",      sf::Keyboard::F13},
    {"f14",      sf::Keyboard::F14},
    {"f15",      sf::Keyboard::F15},
    {"pause",    sf::Keyboard::Pause}
};

struct MouseButton
{
    std::string name;
    sf::Mouse::Button button;
};

static const MouseButton MouseButtonInfo[5] =
{
    {"mouse.left", sf::Mouse::Left},
    {"mouse.right", sf::Mouse::Right},
    {"mouse.middle", sf::Mouse::Middle},
    {"mouse.x1", sf::Mouse::XButton1},
    {"mouse.x2", sf::Mouse::XButton2}
};

struct JoyAxisCode
{
    std::string name;
    sf::Joystick::Axis axis;
};

static const JoyAxisCode AxisInfo[8] =
{
    {"x", sf::Joystick::X},
    {"y", sf::Joystick::Y},
    {"z", sf::Joystick::Z},
    {"r", sf::Joystick::R},
    {"u", sf::Joystick::U},
    {"v", sf::Joystick::V},
    {"povx", sf::Joystick::PovX},
    {"povy", sf::Joystick::PovY},
};

#endif // DOXYGEN
#endif // KEYS_HPP
