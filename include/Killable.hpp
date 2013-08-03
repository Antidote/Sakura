#ifndef KILLABLE_HPP
#define KILLABLE_HPP

class Entity;

class Killable
{
public:
    bool canTakeDamage() const;
    void takeHp(int amount, Entity* e);
    void giveHp(int amount);
    int currentHp() const;

    virtual void onDamage(Entity* e)=0;
    virtual void onDeath()=0;
protected:
    int  m_hp;
    bool m_takeDamage;
};

#endif // KILLABLE_HPP
