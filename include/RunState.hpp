#ifndef RUNSTATE_HPP
#define RUNSTATE_HPP


#include <SFML/Graphics.hpp>

class RunState
{
public:
    /*!
     * \brief The Type enum
     */
    enum Type
    {
        Splash,
        Menu,
        Game,
        End
    };

    virtual ~RunState() {}

    /*!
     * \brief initialize
     */
    virtual void initialize() =0;

    /*!
     * \brief update
     * \param dt
     */
    virtual void update(sf::Time dt)=0;

    /*!
     * \brief draw
     * \param rt
     */
    virtual void draw(sf::RenderTarget& rt)=0;
    /*!
     * \brief isInitialized
     * \return
     */
    virtual bool isInitialized() const=0;
    /*!
     * \brief type
     * \return
     */
    virtual Type type() const=0;
    /*!
     * \brief isDone
     * \return
     */
    virtual bool isDone() const=0;
    /*!
     * \brief name
     * \return
     */
    virtual std::string name() const=0;

    /*!
     * \brief nextState
     * \return
     */
    virtual RunState* nextState() const=0;
};

#endif // RUNSTATE_HPP
