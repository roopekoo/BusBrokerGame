#include "core/logic.hh"
#include "player.h"
#include <math.h>
#include <QDebug>

namespace Game
{

Player::Player() : passengersOnBoard_({})
{
    eastCoord_ = 3327703;
    northCoord_ = 6825409;
    location_.setNorthEast(northCoord_,eastCoord_);
    type_ = 0;
    rotation_ = 0;
}
Player::~Player()
{

}

Interface::Location Player::giveLocation() const
{
    return location_;
}

void Player::move(Interface::Location loc)
{
    location_.setNorthEast(loc.giveNorthernCoord(),loc.giveEasternCoord());
}

bool Player::isRemoved() const
{
    // Fix this if needed
    return false;
}

void Player::remove()
{

}

std::string Player::getName() const
{
    if(type_ == 2)
    {
        return "Tram";
    }
    return "N/A";
}

std::vector<std::shared_ptr<Interface::IPassenger>> Player::getPassengers() const
{
    return passengersOnBoard_;
}

void Player::addPassenger(std::shared_ptr<Interface::IPassenger> passenger)
{
    passengersOnBoard_.push_back(passenger);
}

void Player::removePassenger(std::shared_ptr<Interface::IPassenger> passenger)
{
    passengersOnBoard_.erase(std::remove(passengersOnBoard_.begin(),
                                         passengersOnBoard_.end(), passenger),
                             passengersOnBoard_.end());
}
void Player::movePlayer(int dir)
{
    Coordinates c;
    double newEast = eastCoord_+dir*SPEED.at(type_)*cos(rotation_*M_PI/180);
    double newNorth = northCoord_+dir*SPEED.at(type_)*sin(rotation_*M_PI/180);
    int newX = c.xFromEast(newEast);
    int newY = c.yFromNorth(newNorth);

    int BL = c.BORDER_LEFT;
    int BR = c.BORDER_RIGHT;
    int BU = c.BORDER_UP;
    int BD = c.BORDER_DOWN;

    if((BL<=newX and newX<=BR) and (BU<=newY and newY<=BD))
    {
        eastCoord_ = newEast;
        northCoord_ = newNorth;
        targetLocation_.setNorthEast(northCoord_,eastCoord_);
        move(targetLocation_);
    }
    if( type_ == 2 )
    {
       // emit tramMoved();
    }
}
void Player::setType(int type)
{
    type_ = type;
}

int Player::getType()
{
    return type_;
}

QPixmap Player::getPix()
{
    return QPixmap(PLAYERICONS.at(type_));
}

int Player::getX()
{
    Coordinates c;
    return c.xFromEast(location_.giveEasternCoord());
}

int Player::getY()
{
    Coordinates c;
    return c.yFromNorth(location_.giveNorthernCoord());
}
void Player::setXY(int x,int y)
{
    Coordinates c;
    northCoord_ = c.NorthFromY(y);
    eastCoord_ = c.EastFromX(x);
    location_.setNorthEast(northCoord_, eastCoord_);
}

void Player::changeRotation(int deg, bool isIncremental)
{
    if(!isIncremental)
    {
        rotation_=0;
    }
    if(rotation_+deg >= 360)
    {
        rotation_ += deg-360;
    }
    else if(rotation_+deg < 0)
    {
        rotation_ += deg+360;
    }
    else
    {
        rotation_ += deg;
    }
}

int Player::getRotation()
{
    return rotation_;
}

int Player::destroyBus( std::shared_ptr< Game::City > gameCity )
{
    int destroyedBuses = 0;

    std::vector<std::shared_ptr< Interface::IActor >> nearbyActors =
            gameCity->getNearbyActors( location_ );

    for( auto actor : nearbyActors )
    {
        if( typeid ( *actor) == typeid ( CourseSide::Nysse) )
        {
            Interface::Location loc = actor->giveLocation();
            std::shared_ptr<Interface::IStop> nearestStop =
                 gameCity->getNearestStop( loc );
            std::shared_ptr< CourseSide::Nysse > busPtr =
                    std::dynamic_pointer_cast< CourseSide::Nysse >(actor);

            for( auto IPassenger : busPtr->getPassengers() )
            {
                busPtr->removePassenger(IPassenger);
                std::shared_ptr< CourseSide::Passenger > passenger =
                 std::dynamic_pointer_cast< CourseSide::Passenger >(IPassenger);
                passenger->enterStop( nearestStop );
                gameCity->actorMoved( passenger );
                qDebug() << "Moved passenger from bus to stop " << nearestStop->getName();
            }
            gameCity->removeActor( actor );
            ++destroyedBuses;
        }
    }
    return destroyedBuses;
}

void Player::persuadePassengers(std::shared_ptr<City> gameCity)
{
    std::vector<std::shared_ptr< Interface::IActor >> nearbyActors =
            gameCity->getNearbyActors( location_ );

    for( auto actor : nearbyActors )
    {
        if( typeid( *actor ) == typeid( CourseSide::Passenger ) )
        {
            std::shared_ptr< CourseSide::Passenger > passenger =
                 std::dynamic_pointer_cast< CourseSide::Passenger >( actor );
            if( !passenger->isInVehicle() )
            {
                passenger->enterStop( gameCity->giveTramStop1() );
                gameCity->giveTramStop1()->addPassenger( passenger );
                gameCity->actorMoved( actor );
            }
        }
    }
}

void Player::enterOrExitPassengers(std::shared_ptr<City> gameCity)
{
    /*
    std::shared_ptr< CourseSide::Passenger > passenger = nullptr;

    if( passengersOnBoard_.empty() )
    {
        std::vector<std::shared_ptr< Interface::IPassenger> >
               IPassengersOnStop1 = gameCity->giveTramStop1()->getPassengers();

        for( auto IPassenger : IPassengersOnStop1 )
        {
            passenger = std::dynamic_pointer_cast< CourseSide::Passenger >(
                        IPassenger );
            std::shared_ptr< Interface::IVehicle > playerSharedPtr( this );

            passenger->enterVehicle( playerSharedPtr );
            gameCity->giveTramStop1()->removePassenger( IPassenger );
            addPassenger( IPassenger );
        }
    }
    else
    {
        for( auto IPassenger: passengersOnBoard_ )
        {
            passenger = std::dynamic_pointer_cast< CourseSide::Passenger >(
                        IPassenger );

            std::weak_ptr< Interface::IStop > tramStop2 =
                    std::dynamic_pointer_cast< Interface::IStop >(
                        gameCity->giveTramStop2() );
            passenger->enterStop( tramStop2 );
            // remove passengers from tram
            // increase captured passenger amount
            // increase score
            // remove passengers from tram stop 2
        }
    }
    */
}
}


