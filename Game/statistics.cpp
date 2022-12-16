#include "statistics.hh"
#include <QDebug>

namespace Game
{
Statistics::Statistics(): score_(0), busesDestroyed_(0), passengersCaptured_(0)
{

}

Statistics::~Statistics()
{

}

void Statistics::morePassengers(int num)
{
    qDebug() << "Added " << num << " passengers";
}

void Statistics::nysseRemoved()
{
    qDebug() << "Nysse removed ingame";
    score_ += BUS_DESTROYED_POINTS;
    busesDestroyed_ += 1;
}

void Statistics::newNysse()
{
    qDebug() << "New nysse added to the game";
}

void Statistics::nysseLeft()
{
    qDebug() << "Nysse has left the game";
}

int Statistics::getScore()
{
    return score_;
}

int Statistics::getDestroyedBuses()
{
    return busesDestroyed_;
}

void Statistics::PassengerCaptured()
{
    score_ += PASSENGER_CAPTURED_POINTS;
    passengersCaptured_ += 1;
}

int Statistics::getPassengersCaptured()
{
    return passengersCaptured_;
}
}
