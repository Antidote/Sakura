#ifndef RUPEE_HPP
#define RUPEE_HPP

#include "Collideable.hpp"
#include "Entity.hpp"
class Rupee : public Entity, public Collideable
{
public:
    enum Type
    {
        Green,
        Blue,
        Red,
        Green50,
        Green100,
        Green300
    };
};

#endif // RUPEE_HPP
