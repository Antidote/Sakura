#ifndef RUNSTATE_HPP
#define RUNSTATE_HPP


#include <SFML/Graphics.hpp>

class RunState
{
public:
    virtual void initialize() =0;
    virtual void update(sf::Time dt)=0;
    virtual void draw(sf::RenderTarget& rt)=0;
    virtual bool isInitialized() const=0;
};

#endif // RUNSTATE_HPP
