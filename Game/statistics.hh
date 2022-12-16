#ifndef STATISTICS_HH
#define STATISTICS_HH

#include "interfaces/istatistics.hh"

/**
  * @file
  * @brief Defines scoring statistics.
  */

namespace Game
{
/**
 * @brief The Statistics class
 *
 * Manages scoring, bus and passenger statistics
 */
class Statistics : public Interface::IStatistics
{
public:
    /**
     * @brief Statistics constructor
     *
     * sets score_ and passengersCaptured_ to 0
     */
    Statistics();

    /**
     * @brief ~Statistics
     *
     * Virtual destructor for Statistics
     */
    virtual ~Statistics();

    /**
     * @brief morePassengers notifies, that more passangers are added to the game.
     * @param num how many new passangers are added.
     * @pre num > 0
     * @post Exception guarantee: strong
     */
    void morePassengers(int num);

    /**
     * @brief nysseRemoved
     *
     * updates statistic data for amount of buses removed
     */
    void nysseRemoved();

    /**
     * @brief newNysse notifies, that a new nysse is added to the game.
     * @post Exception guarantee: strong
     */
    void newNysse();

    /**
     * @brief nysseLeft notifies, that a nysse has left the game.
     * @post Exception guarantee: strong
     */
    void nysseLeft();

    /**
     * @brief getScore
     * @return current score
     */
    int getScore();

    /**
     * @brief getDestroyedBuses
     * @return amount of buses that have been destroyed
     */
    int getDestroyedBuses();

    /**
     * @brief PassengerCaptured
     * @return
     */
    void PassengerCaptured();

    /**
     * @brief getPassengersCaptured
     * @return amount of passengers that have been captured
     */
    int getPassengersCaptured();

private:
    int score_;
    int busesDestroyed_;
    const int BUS_DESTROYED_POINTS = 10;

    int passengersCaptured_;
    const int PASSENGER_CAPTURED_POINTS = 20;
    const int PASSENGER_GOAL = 40;

};
}

#endif // STATISTICS_HH
