#ifndef PLAYER_H
#define PLAYER_H
#include "city.hh"
#include <vector>
#include <QString>
#include <QPixmap>
#include "interfaces/ivehicle.hh"
#include "core/location.hh"
#include "coordinates.h"

/**
  * @file
  * @brief Defines player actor.
  */

namespace Game{

/**
 * @brief Player class
 *
 * Handles player movement calculations and playertype changes.
 * Inheritated from IVehicle, (also from IActor via IVechicle)
 */
class Player : public QObject, public Interface::IVehicle

{
    Q_OBJECT
public:
    /**
     * @brief Player constructor
     *
     * Setting up default coordinates, type and rotation.
     */
    Player();

    /**
     * @brief ~Player
     * Virtual destructor for the Player
     */
    virtual ~Player();

    /**
     * @brief giveLocation function
     * @return current stored location data
     * @post Exception guarantee: nothrow.
     *
     * Player location is always valid. Set up on constructor and movePlayer
     * sets new coordinates if they are withing the game borders
     */
    Interface::Location giveLocation() const;

    /**
     * @brief move function
     * @param loc Actors new location.
     * @post Actors location has changed. Excaption guarantee: nothrow.
     * This function is used only when movePlayer() has valid new coordinates
     *
     * Updates location_ data to given loc data
     */
    void move(Interface::Location loc);

    /**
     * @brief isRemoved
     * @return false
     *
     * NOTE! This function does not have purpose in this class and is not used!
     * Only a placeholder function for the inheritation to work
     * Currently does nothing, this function is not used anywhere
     */
    bool isRemoved() const;

    /**
     * @brief remove function
     *
     *NOTE! This function does not have purpose in this class and is not used!
     */
    void remove();

    /**
     * @brief getName returns the name of the vehicle(might not be unique).
     * @pre -
     * @return name of the vehicle
     * @post Exception guarantee: strong
     *
     * Currently player can be only Tram-vehicle so other types return "N/A"
     */
    std::string getName() const;

    /**
     * @brief getPassengers returns all passengers in the vehicle.
     * @pre -
     * @return Vector containing all passengers in the vehicle.
     * @post Exception guarantee: strong.
     */
    std::vector<std::shared_ptr<Interface::IPassenger>> getPassengers() const;

    /**
     * @brief addPassenger adds a new passenger to the vehicle.
     * @param passenger an passenger object to be added to the Vehicle.
     * @pre `matkustaja.onkoKulkuneuvossa() == false`.
     * @post Passenger is added into the vehicle. Exception guarantee: basic.
     */
    void addPassenger(std::shared_ptr<Interface::IPassenger> passenger);

    /**
     * @brief removePassenger removes the passenger from the vehicle.
     * @param passenger Passenger to be removed from the vehicle.
     * @pre -
     * @post Passenger is removed from the vehicle. Exception guarantee: basic.
     * @exception GameError Passenger is not in the vehicle.
     */
    void removePassenger(std::shared_ptr<Interface::IPassenger> passenger);

    /**
     * @brief setType function
     * @param type int value from 0 to 2
     *
     * Changes type_ to new value
     */
    void setType(int type);

    /**
     * @brief getType function
     * @return current playertype from type_
     */
    int getType();

    /**
     * @brief getPix function
     * @return current player image depending on what is the current playertype.
     */
    QPixmap getPix();

    /**
     * @brief getX function
     * @return player location x value
     */
    int getX();

    /**
     * @brief getY
     * @return player location y value
     */
    int getY();

    /**
     * @brief setXY function
     * @param x integer coordinate value
     * @param y integer coordinate value
     *
     * Sets player north and east coordinates to playerdata and updates
     * location_
     */
    void setXY(int x, int y);

    /**
     * @brief changeRotation function
     * @param deg integer value in degrees
     * @param isIncremental if true the deg will be added to rotation_.
     * if false rotation_ will be set to value of deg.
     *
     * This function will keep the rotatyion values between 0 and 360 degrees.
     */
    void changeRotation(int deg, bool isIncremental);

    /**
     * @brief getRotation function
     * @return integer value of the direction the player is facing
     */
    int getRotation();

    /**
     * @brief destroyBus function
     * @param gameCity data pointer
     * @return amount of buses destroyed (integer)
     */
    int destroyBus( std::shared_ptr< Game::City > gameCity );

    /**
     * @brief persuadePassengers
     * @param gameCity data pointer
     *
     * Nearby passengers are moved from bus stops to tram stop.
     */
    void persuadePassengers( std::shared_ptr< Game::City > gameCity );

    /**
     * @brief enterOrExitPassengers
     * @param gameCity data pointer
     *
     * If tram is empty, move passengers from stop 1 to tram.
     * If there are passengers inside tram, move them out and add score.
     */
    void enterOrExitPassengers( std::shared_ptr< Game::City > gameCity );

    /**
     * @brief movePlayer function
     * @param dir direction forwards or backwards (int -1 or 1)
     *
     * calculates new location coordinates (x,y) and updates them to north- and
     * eastcoords and to location_ if the new location is withing the game area
     */
    void movePlayer(int dir);
signals:
    void tramMoved();

private:
    double northCoord_;
    double eastCoord_;
    Interface::Location targetLocation_;
    Interface::Location location_;
    int rotation_;
    int type_;
    std::vector<std::shared_ptr<Interface::IPassenger>> passengersOnBoard_;


    const std::vector<double> SPEED={1.420,5.195,9.933};
    const std::vector<QString> PLAYERICONS=
    {
        "images/walker.png",
        "images/biker.png",
        "images/tram.png",
    };
};
}
#endif // PLAYER_H
