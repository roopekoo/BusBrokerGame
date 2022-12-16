#include "police.h"
#include "QDebug"
#include <math.h>

namespace Game
{
Police::Police()
{
    speed_ = 2;
    eastCoord_ = 3326946;
    northCoord_ = 6824987;
}

Police::~Police()
{

}

Interface::Location Police::giveLocation() const
{
    return location_;
}

void Police::move(Interface::Location loc)
{
    location_.setNorthEast(loc.giveNorthernCoord(),loc.giveEasternCoord());
}

bool Police::isRemoved() const
{
    return false; // Change this to something better if needed later
}

void Police::remove()
{

}

int Police::getX()
{
    Coordinates c;
    return c.xFromEast(location_.giveEasternCoord());
}

int Police::getY()
{
    Coordinates c;
    return c.yFromNorth(location_.giveNorthernCoord());
}
double Police::getRotation()
{
    return rotation_;
}
void Police::updateRotation(int x,int y)
{
    double dy=y-getY();
    double dx=x-getX();
    if(int(dx)==0)
    {
        if(int(dy) > 0)
        {
            rotation_ =90;
        }
        else if(int(dy) < 0)
        {
            rotation_ = 270;
        }
        else
        {
            qDebug() << "CAUGHT!";
            emit playerCaught();
        }
    }
    else if(int(dx) < 0)
    {
        rotation_ = atan(dy/dx)*180/M_PI + 180;
    }
    else
    {
        rotation_ = atan(dy/dx)*180/M_PI;
    }
}
void Police::approachPlayer()
{
    Coordinates c;
    double newEastCoord = eastCoord_+speed_*cos(rotation_*M_PI/180);
    double newNorthCoord = northCoord_+speed_*sin(rotation_*M_PI/180);
    int newX = c.xFromEast(newEastCoord);
    int newY = c.yFromNorth(newNorthCoord);

    int BL = c.BORDER_LEFT;
    int BR = c.BORDER_RIGHT;
    int BU = c.BORDER_UP;
    int BD = c.BORDER_DOWN;

    if((BL<=newX and newX<=BR) and (BU<=newY and newY<=BD))
    {
        eastCoord_ = newEastCoord;
        northCoord_ = newNorthCoord;
        targetLocation_.setNorthEast(northCoord_, eastCoord_);
        move(targetLocation_);
    }
}
}
