#ifndef CITY_HH
#define CITY_HH

#include "actors/nysse.hh"
#include "interfaces/icity.hh"
#include <map>
#include <QGraphicsRectItem>
#include <QTime>


/**
  * @file
  * @brief Defines a class that does city's operations.
  */

namespace Game
{

enum State { INIT_STATE, GAME_STATE };

/**
 * @brief City class inherited from ICity
 *
 * This class handles actors, background, gamestate and ingame timer
 *  * If class method doesn't have exception guarantee of nothrow,
 * method can leak out exception `std::bad_alloc` (out of memory)
 */
class City : public QObject, public Interface::ICity
{
    Q_OBJECT
public:
    /**
     * @brief City class constructor
     *
     * Initalizes city with no buses or passenegers
     */
    City();

    /**
     * @brief ~City, City destructor
     *
     * Deletes actors from actorsInCity_ and stops from stopsInCity_
     */
    virtual ~City();

    /**
     * @brief setBackground funtion
     * @param basicbackground default image used in-game
     * @param bigbackground bigger image of the map (here only a placeholder)
     * @pre City is in init state.
     * @post Picture for the game area is set. Exception guarantee: basic.
     * @exception InitError Setting the picture was unsuccesful or
     * the picture was invalid.
     *
     * Only send signal to gamewindow that backgrounds exists and can be shown
     * so this virtual function has some use
     */
    void setBackground( QImage& basicbackground, QImage& bigbackground );

    /**
     * @brief setClock function
     * @param clock Game clock time at the function call.
     * @pre `kello.isValid() == true`.
     * @post Time is set. Exception guarantee: nothrow.
     *
     * Sets gameClock_ to clock value
     */
    void setClock( QTime clock );

    /**
     * @brief addStop function
     * @param stop pointer to a stop object.
     * @pre City is in init state.
     * @post Stop is added to the city. Exception guarantee: basic
     * @exception InitError Stops position is not valid.
     *
     * adds stops to stopsInCity_ with graphics
     * also sends signal to gamewindow.
     */
    void addStop( std::shared_ptr< Interface::IStop > stop );

    /**
     * @brief addTramStops
     *
     * adds two tram stops to City
     * also sends signal to gamewindow for adding stop graphics to screen
     */
    void addTramStops();

    /**
     * @brief startGame function
     * @pre City is init state. setBackground() and setClock() have been called.
     * @post City is in gamestate. Exception guarantee: nothrow.
     *
     * updates state_ to current "true" state
     */
    void startGame();

    /**
     * @brief addActor function
     * @param newactor actor to be added to the city that fulfills ActorIF.
     * @post Actor is added to the city. Exception guarantee: basic.
     *
     * Adds actor to actorsInCity_ if it doesn't exist there yet
     * sends signal to gamewindow
     */
    void addActor( std::shared_ptr< Interface::IActor > newactor );

    /**
     * @brief removeActor function
     * @param actor Actor to be removed.
     * @pre City is in gamestate.
     * @post Actor is removed from the city. Exception guarantee: strong.
     * @exception GameError Actor not found in the city
     *
     * Removes actor from the city if it exists.
     */
    void removeActor( std::shared_ptr< Interface::IActor > actor );

    /**
     * @brief actorRemoved function
     * @param actor Actor that is set removed ingame.
     * @pre City is in gamestate. Given actor is found in the city. Actor has `actor.isRemoved() == true`.
     * @post Exception guarantee: strong.
     */
    void actorRemoved( std::shared_ptr< Interface::IActor > actor );

    /**
     * @brief findActor function
     * @param actor Actor that that is looked for in the city.
     * @return true if the actor is in city, false if not
     * @post Exception guarantee: nothrow.
     */
    bool findActor( std::shared_ptr< Interface::IActor > actor ) const;

    /**
     * @brief actorMoved function
     * @param actor Actor that has moved.
     * @pre City is in gamestate. Given actor is found in the city.
     * @post Exception guarantee: basic.
     *
     * Sends signal to the gamewindow that icon needs a new location on screen
     */
    void actorMoved( std::shared_ptr< Interface::IActor > actor );

    /**
     * @brief getNearbyActors function
     * @param loc Location for getting the actors close to it.
     * @pre City is in gamestate.
     * @return Vector containing actors close to the location, that pass `getLocation().isClose(loc) == true`.
     */
    std::vector< std::shared_ptr< Interface::IActor > > getNearbyActors
        ( Interface::Location loc ) const;

    /**
     * @brief isGameOver function
     * @pre City is in gamestate.
     * @post Exception guarantee: nothrow.
     * @return true if game have ended, false otherwise
     */
    bool isGameOver() const;

    /**
     * @brief gameIsOver
     * Updates gameOver_ data to true when the game ends
     */
    void gameIsOver();

    std::shared_ptr< Interface::IStop > getNearestStop(
            Interface::Location loc ) const;

    /**
     * @brief giveTramStop1
     * @return pointer to tram stop 1
     */
    std::shared_ptr< CourseSide::Stop > giveTramStop1();

    /**
     * @brief giveTramStop2
     * @return pointer to tram stop 2
     */
    std::shared_ptr< CourseSide::Stop > giveTramStop2();

signals:
    /**
     * @brief givenMapsAreValid signal
     * @param basicbackground image
     * @param bigbackground image
     *
     * emitted if both params exists
     */
    void givenMapsAreValid( QImage &basicbackground, QImage &bigbackground );

    /**
     * @brief newStopNeededInScene signal
     * @param stop sata pointer
     * @param stopGraphics stored graphics for a stop
     *
     * emitted when trying to add stops to the map
     */
    void newStopNeededInScene( std::shared_ptr< Interface::IStop > stop,
                               QGraphicsRectItem*& stopGraphics );

    void tramStopNeededInScene( std::shared_ptr< Interface::IStop >  );

    /**
     * @brief newActorNeededInScene signal
     * @param newactor datapointer
     * @param newactorGraphics graphics data for actor
     *
     * emitted when adding actor to city
     */
    void newActorNeededInScene( const std::shared_ptr< Interface::IActor >& newactor,
                                QGraphicsPixmapItem*& newactorGraphics);

    /**
     * @brief actorMovedInCity signal
     * @param actor data pointer
     * @param actorGraphics graphics for the actor
     *
     * emitted when moving actor to new location
     */
    void actorMovedInCity( const std::shared_ptr< Interface::IActor >& actor,
                            QGraphicsPixmapItem*& actorGraphics );

private:
    QTime gameClock_;
    State state_;
    bool gameOver_;

    // Below are buses and passangers that are currently in game
    std::map< std::shared_ptr< Interface::IActor >,
              QGraphicsPixmapItem* > actorsInCity_;
    std::map< std::shared_ptr< Interface::IStop >,
              QGraphicsRectItem* > stopsInCity_;
    std::shared_ptr< CourseSide::Stop > tramStop1_;
    std::shared_ptr< CourseSide::Stop > tramStop2_;

    const int tramStop1XCoord = 44;
    const int tramStop1YCoord = 467;
    const int tramStop2XCoord = 811;
    const int tramStop2YCoord = 549;

};
}
#endif // CITY_HH
