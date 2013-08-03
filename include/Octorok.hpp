#ifndef OCTOROCK_HPP
#define OCTOROCK_HPP

#include "Entity.hpp"
#include "Collideable.hpp"
#include "AI.hpp"

class Octorok : public Entity, public Collideable, public AI
{
public:
    enum AiState
    {
        Run,
        Shoot,
        AiStateSize
    };

    Octorok();

    void collide(Entity *entity);

    void update(sf::Time delta);
    void draw(sf::RenderTarget &rt);

    void think(sf::Time dt);
public:
    void doRun(sf::Time dt);
    void doShoot(sf::Time dt);

    float         m_scale;
    sf::Time      m_currentAiTime;
    sf::Time      m_thinkTime;
    sf::Time      m_runThinkTime;
    sf::Time      m_runTime;
    sf::Sprite    m_sprite;
    AiState       m_aiState;

    sf::RectangleShape m_debugShape;

    sf::FloatRect m_patrolZone;

};

#endif // OCTOROCK_HPP
