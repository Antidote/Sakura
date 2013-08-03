#include "Killable.hpp"
#include "Entity.hpp"
#include <iostream>

bool Killable::canTakeDamage() const
{
    return m_takeDamage;
}

void Killable::takeHp(int amount, Entity* e)
{
    if (!m_takeDamage)
        return;

    m_hp -= amount;
    std::cout << "Health: " << m_hp << std::endl;
    onDamage(e);

    if (m_hp <= 0)
    {
        m_hp = 0;
        onDeath();
    }
}

void Killable::giveHp(int amount)
{
    m_hp += amount;
}

int Killable::currentHp() const
{
    return m_hp;
}
