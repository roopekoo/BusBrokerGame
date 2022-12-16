#include "city.hh"
#include "errors/gameerror.hh"
#include "errors/initerror.hh"
#include "coordinates.h"
#include <QDebug>
namespace Game
{

City::City() : state_( INIT_STATE ), gameOver_(false), actorsInCity_({}),
  stopsInCity_({})
{

}

City::~City()
{
    for( auto actor : actorsInCity_ )
    {
        delete actor.second;
        actor.second = nullptr;
    }
    for( auto stop : stopsInCity_ )
    {
        delete stop.second;
        stop.second = nullptr;
    }
}

void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{
    if( basicbackground.isNull() || bigbackground.isNull() )
    {
        throw Interface::InitError( "Setting the picture was"
                            " unsuccesful or the picture was invalid." );
    }
    emit givenMapsAreValid( basicbackground, bigbackground );
}

void City::setClock(QTime clock)
{
    std::swap( gameClock_, clock );
}

void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
    try
    {
        QGraphicsRectItem* stopGraphics = nullptr;

        emit newStopNeededInScene( stop, stopGraphics );

        stopsInCity_.insert( { stop, stopGraphics } );
    }
    catch (...)
    {
        throw Interface::InitError( "Stops position is not valid." );
    }
}

void City::addTramStops()
{
    Game::Coordinates c;
    Interface::Location loc = Interface::Location();

    int east1 = c.EastFromX(tramStop1XCoord);
    int north1 = c.NorthFromY(tramStop1YCoord);
    loc.setNorthEast(north1, east1);
    tramStop1_ = std::make_shared< CourseSide::Stop >(
                CourseSide::Stop( loc, "Ratikka 1", 10001) );
    emit tramStopNeededInScene( tramStop1_ );

    int east2 = c.EastFromX(tramStop2XCoord);
    int north2 = c.NorthFromY(tramStop2YCoord);
    loc.setNorthEast(north2, east2);
    tramStop2_ = std::make_shared< CourseSide::Stop >(
                CourseSide::Stop( loc, "Ratikka 2", 10002 ) );
    emit tramStopNeededInScene( tramStop2_ );
}

void City::startGame()
{    
    state_ = GAME_STATE;
}

void City::addActor(std::shared_ptr<Interface::IActor> newactor)
{
    if( findActor( newactor ) == true )
    {
        throw Interface::GameError( "Actor is already in the city.");
    }

    QGraphicsPixmapItem* actorGraphics = nullptr;

    emit newActorNeededInScene( newactor, actorGraphics );

    actorsInCity_.insert( { newactor, actorGraphics } );
}

void City::removeActor(std::shared_ptr<Interface::IActor> actor)
{
    std::map< std::shared_ptr< Interface::IActor >,
              QGraphicsPixmapItem* >::iterator actorPos =
            actorsInCity_.find( actor );

    if( actorPos != actorsInCity_.end() )
    {
        actor->remove();

        delete actorPos->second;
        actorPos->second = nullptr;

        actorsInCity_.erase( actorPos );
    }
    else
    {
        throw Interface::GameError( "Actor not found in the city");
    }
}

void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
    removeActor( actor );
}

bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
    std::map< std::shared_ptr< Interface::IActor >,
              QGraphicsPixmapItem* >::const_iterator actorIter =
            actorsInCity_.find( actor );

    if( actorIter == actorsInCity_.end() )
    {
        return false;
    }
    return true;
}

void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
    emit actorMovedInCity( actor, actorsInCity_.at( actor ) );
}

std::vector<std::shared_ptr<Interface::IActor> > City::getNearbyActors(
        Interface::Location loc) const
{
   std::vector<std::shared_ptr<Interface::IActor> > nearbyActors = {};

   for( auto actor : actorsInCity_ )
   {
       if( actor.first ->giveLocation().isClose( loc ) )
       {
           nearbyActors.push_back( actor.first );
       }
   }
   return nearbyActors;
}

bool City::isGameOver() const
{
    return gameOver_;
}

void City::gameIsOver()
{
    gameOver_ = true;
}

std::shared_ptr<Interface::IStop> City::getNearestStop(Interface::Location loc) const
{
    Coordinates c;
    std::map< std::shared_ptr< Interface::IStop >,
              QGraphicsRectItem* >::const_iterator stopIter = stopsInCity_.begin();
    std::shared_ptr<Interface::IStop> nearestStop = stopIter->first;

    double currentShortestDistance = Interface::Location::calcDistance( loc,
                                        stopIter->first->getLocation());
    ++stopIter;
    double distance = 0.0;
    for( stopIter; stopIter != stopsInCity_.end(); ++stopIter )
    {
        if( stopIter->first->getLocation().giveX() < c.BORDER_LEFT ||
            stopIter->first->getLocation().giveX() > c.BORDER_RIGHT ||
            stopIter->first->getLocation().giveY() < c.BORDER_UP ||
            stopIter->first->getLocation().giveY() > c.BORDER_DOWN )
        {
            continue;
        }

        distance = Interface::Location::calcDistance(
                    loc, stopIter->first->getLocation() );
        if( distance < currentShortestDistance )
        {
            nearestStop = stopIter->first;
            currentShortestDistance = distance;
        }
    }
    return nearestStop;
}

std::shared_ptr<CourseSide::Stop> City::giveTramStop1()
{
    return tramStop1_;
}

std::shared_ptr<CourseSide::Stop> City::giveTramStop2()
{
    return tramStop2_;
}


}
