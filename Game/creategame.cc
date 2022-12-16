#include "city.hh"
#include "creategame.hh"


std::shared_ptr< Interface::ICity > Interface::createGame()
{
    std::shared_ptr< Interface::ICity > cityPtr( new Game::City );

    return cityPtr;
}
