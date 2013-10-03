#ifndef AI_HPP
#define AI_HPP

#include <SFML/System/Time.hpp>


/*!
 *  \class AI
 *  \brief BaseClass inherited by AI objects (usually entities)
 *
 *
 */
class AI
{
public:

    /*!
     * \brief think Performs AI related routines baseed on the delta time.
     * \param dt Delta Time
     */
    virtual void think(sf::Time dt)=0;
};

#endif // AI_HPP
