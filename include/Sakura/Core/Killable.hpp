#ifndef KILLABLE_HPP
#define KILLABLE_HPP

namespace Sakura
{
namespace Core
{
class Entity;

class Killable
{
public:
    /*!
     * \brief canTakeDamage
     * \return
     */
    bool canTakeDamage() const;

    /*!
     * \brief takeHp
     * \param amount
     * \param e
     */
    void takeHp(int amount, Entity* e);

    /*!
     * \brief giveHp
     * \param amount
     */
    void giveHp(int amount);

    /*!
     * \brief currentHp
     * \return
     */
    int currentHp() const;

    /*!
     * \brief onDamage
     * \param e
     */
    virtual void onDamage(Entity* e)=0;

    /*!
     * \brief onDeath
     */
    virtual void onDeath()=0;
protected:
    int  m_hp;
    int  m_maxHp;
    int  m_hpCounter;
    bool m_takeDamage;
};
} // Core
} // Sakura


#endif // KILLABLE_HPP
