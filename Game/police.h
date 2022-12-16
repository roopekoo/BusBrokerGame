#ifndef POLICE_H
#define POLICE_H
#include "interfaces/iactor.hh"
#include "core/location.hh"
#include "coordinates.h"

#include <QObject>


/**
  * @file
  * @brief Defines police actor.
  */

namespace Game{

/**
 * @brief The Police class
 *
 * Handles police movement calculations. Approaches always the player.
 */
class Police : public QObject, public Interface::IActor
{
    Q_OBJECT
public:
    /**
     * @brief Police constructor
     *
     * Sets up default starting coordinates and moving speed
     */
    Police();

    /**
     * @brief ~Police destructor
     *
     * Virtual destructor for Police
     */
    virtual ~Police();

    /**
     * @brief giveLocation
     * @return current stored location data
     * @post Exception guarantee: nothrow.
     *
     * Police location is always valid. Set up on constructor and approachPlayer
     * follows the player that cannot go outside the borders.
     * Just in case, police also respects game border.
     */
    Interface::Location giveLocation() const;

    /**
     * @brief move
     * @param loc target location
     * @post Police location has changed. Excaption guarantee: nothrow.
     * This function is used only when approachPlayer()
     * has valid new coordinates
     *
     * Sets location_ values to loc values
     */
    void move(Interface::Location loc);

    /**
     * @brief isRemoved
     * @return true (not used)
     *
     * NOTE! This function does not have purpose in this class and is not used!
     */
    bool isRemoved() const;

    /**
     * @brief remove
     * Does nothing because police does not exist in city class where it should
     * be removed in the first place
     */
    void remove();

    /**
     * @brief getX
     * @return police x coordinate value
     */
    int getX();

    /**
     * @brief getY
     * @return police y coordinate value
     */
    int getY();

    /**
     * @brief updateRotation
     * @param x player coordnate
     * @param y player coordnate
     *
     * Updates police direction to look at the player
     * Also check if the coordinates are same as the player's so the game
     * can end with playerCaught signal
     */
    void updateRotation(int x, int y);

    /**
     * @brief approachPlayer
     *
     * Updates northCoord_, eastCoord_ and location_
     * Moving at speed (2) to rotation direction
     */
    void approachPlayer();

    /**
     * @brief getRotation
     * @return rotation_ as double
     *
     * Used only for debugging purposes
     */
    double getRotation();
signals:
    void playerCaught();
private:
    double northCoord_;
    double eastCoord_;
    Interface::Location targetLocation_;
    Interface::Location location_;
    int speed_;
    double rotation_;
};
}

#endif // POLICE_H
