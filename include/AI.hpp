#ifndef AI_HPP
#define AI_HPP

#include <SFML/System/Time.hpp>

class AI
{
public:
    virtual void think(sf::Time dt)=0;
};

#endif // AI_HPP
