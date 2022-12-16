#include "coordinates.h"
namespace Game
{
Coordinates::Coordinates()
{

}

int Coordinates::xFromEast(double eastcoord)
{
    return (eastcoord - EASTERN_COORD) * COORD_MULT;
}

int Coordinates::yFromNorth(double northcoord)
{
    return (northcoord - NORTHERN_COORD) * COORD_MULT;
}

double Coordinates::EastFromX(int x)
{
    return x/COORD_MULT + EASTERN_COORD;
}

double Coordinates::NorthFromY(int y)
{
    return y/COORD_MULT + NORTHERN_COORD;
}
}
