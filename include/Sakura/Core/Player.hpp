#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Audio.hpp>
#include "Sakura/Core/Entity.hpp"
#include "Sakura/Core/Killable.hpp"
#include "Sakura/Core/Collideable.hpp"

namespace Sakura
{
namespace Core
{
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
} // Core
} // Sakura

#endif // PLAYER_HPP
