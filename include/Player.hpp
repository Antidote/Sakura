#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Audio.hpp>
#include "Entity.hpp"
#include "Killable.hpp"
#include "AnimatedSprite.hpp"
#include "Collideable.hpp"

class Cell;

class Player : public Entity, public Killable, public Collideable
{
public:
    /*!
     * \brief Player
     */
    Player();

    /*!
     * \brief playerId
     * \return
     */
    int playerId() const;

protected:
    static int PlayerCount;
    int m_playerId;
};

#endif // LINK_HPP
