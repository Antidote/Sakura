#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "RunState.hpp"

namespace Sakura
{
namespace Core
{
class GameState : public RunState
{
public:
    /*!
     * \brief GameState
     */
    GameState();

    /*!
     * \brief initialize
     */
    virtual void initialize();

    /*!
     * \brief update
     * \param dt
     */
    virtual void update(sf::Time dt);

    /*!
     * \brief draw
     * \param rt
     */
    virtual void draw(sf::RenderTarget &rt);

    /*!
     * \brief isInitialized
     * \return
     */
    virtual bool isInitialized() const;

    /*!
     * \brief type
     * \return
     */
    Type type() const;

    /*!
     * \brief isDone
     * \return
     */
    virtual bool isDone() const;

    /*!
     * \brief name
     * \return
     */
    virtual std::string name() const;

    /*!
     * \brief nextState
     * \return
     */
    virtual RunState* nextState() const;
private:
    bool m_initialized;
};
} // Core
} // Sakura

#endif // GAMESTATE_HPP
