#ifndef LINK_HPP
#define LINK_HPP

#include <SFML/Audio.hpp>
#include "Entity.hpp"
#include "Killable.hpp"
#include "AnimatedSprite.hpp"
#include "Collideable.hpp"

class Cell;

class Link : public Entity, public Killable, public Collideable
{
public:
    enum State
    {
        Idle,
        Moving,
        Lifting,
        Pushing,
        Pulling
    };

    Link();

    void update(sf::Time delta);
    void draw(sf::RenderTarget &rt);

    void onDamage(Entity *e);
    void onDeath();

    void giveRupees(int num);
    void takeRupees(int num);
    void checkCell(Cell* cell, sf::IntRect cellBounds);
private:
    void doCollision();

    sf::Keyboard::Key      m_walkup;
    sf::Keyboard::Key      m_walkdown;
    sf::Keyboard::Key      m_walkleft;
    sf::Keyboard::Key      m_walkright;
    sf::Vector2f           m_velocity;
    sf::Texture*           m_linkAnimations;
    std::vector<Animation> m_animations;
    AnimatedSprite         m_sprite;
    State                  m_state;
    sf::Clock              m_invincibilityTimer;
    sf::Clock              m_blinkTimer;
    sf::Time               m_blinkTime;
    sf::Color              m_blinkColor;
    sf::Time               m_invincibilityTime;
    sf::Time               m_currentTime;

    int                    m_rupees;
    int                    m_maxRupees;

    sf::RenderTarget* target;

    sf::RectangleShape m_colShape;
};

#endif // LINK_HPP
