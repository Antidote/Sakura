#include "Sakura/Core/Killable.hpp"
#include "Sakura/Core/Entity.hpp"

namespace Sakura
{
namespace Core
{
bool Killable::canTakeDamage() const
{
    return m_takeDamage;
}

void Killable::takeHp(int amount, Entity* e)
{
    if (!m_takeDamage)
        return;

    m_hp -= amount;

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
} // Core
} // Sakura
