#ifndef RUNSTATE_HPP
#define RUNSTATE_HPP


#include <SFML/Graphics.hpp>

namespace Sakura
{
namespace Core
{
/*!
 * \class RunState
 * \brief The RunState baseclass, used to implement the various game states in a game
 * such as Splashes, Menus, Game, and Credits.
 */
class RunState
{
public:
    /*!
     * \brief The Type enum specifies what kind of runstate the inheriting class is.
     */
    enum Type
    {
        Splash, //!< Used for splash screens, displaying information about the game and engine
        Menu,   //!< Used for the various menus in a game, <b>Not for inventories</b>
        Game,   //!< Used for the main game state, used to display the HUD, inventory, and various other assets
        End     //!< Used for the end credits when the player completes the game
    };

    virtual ~RunState() {}

    /*!
     * \brief Initializes the RunState, loading assets, and managing internal substates
     */
    virtual void initialize() =0;

    /*!
     * \brief Updates the RunState based on the delta time given to it by the main loop
     * \param dt The delta given by the main loop
     */
    virtual void update(sf::Time dt)=0;

    /*!
     * \brief Draws the RunState's various drawable assets, such as a Hud.
     * \param rt The window to draw to.
     */
    virtual void draw(sf::RenderTarget& rt)=0;

    /*!
     * \brief Returns whether the RunState has been initialized
     * \return True if intialized, False otherwise
     */
    virtual bool isInitialized() const=0;

    /*!
     * \brief type
     * \return
     */
    virtual Type type() const=0;

    /*!
     * \brief Returns if the RunState has finished it's execution.
     * \return True is finished, false otherwise.
     */
    virtual bool isDone() const=0;

    /*!
     * \brief Returns the name of the RunState
     * \return The name of the RunState
     */
    virtual std::string name() const=0;

    /*!
     * \brief Returns the RunState to execute after this one
     * \return The next RunState, NULL if no RunState specified
     */
    virtual RunState* nextState() const=0;
};
} // Core
} // Sakura

#endif // RUNSTATE_HPP
