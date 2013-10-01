#ifndef RUNSTATE_HPP
#define RUNSTATE_HPP


#include <SFML/Graphics.hpp>

class RunState
{
public:
    enum Type
    {
        Logo,
        Menu,
        Game,
        End
    };

    virtual ~RunState() {}
    virtual void initialize() =0;
    virtual void update(sf::Time dt)=0;
    virtual void draw(sf::RenderTarget& rt)=0;
    virtual bool isInitialized() const=0;
    virtual Type type() const=0;
    virtual bool isDone() const=0;
    virtual std::string name() const=0;
    virtual RunState* nextState() const=0;
};

#endif // RUNSTATE_HPP
