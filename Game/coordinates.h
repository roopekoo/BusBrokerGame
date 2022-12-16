#ifndef COORDINATES_H
#define COORDINATES_H

namespace Game
{
class Coordinates
{
public:
    Coordinates();

    /**
     * @brief xFromEast function
     * @param eastcoord eastern coordinate
     * @return integer that represents map x-coordinates
     */
    int xFromEast(double eastcoord);

    /**
     * @brief yFromNorth function
     * @param northcoord eastern coordinate
     * @return integer that represents map y-coordinate
     */
    int yFromNorth(double northcoord);

    /**
     * @brief EastFromX function
     * @param x integer coordinate
     * @return double value of the east coordinate
     */
    double EastFromX(int x);

    /**
     * @brief NorthFromY function
     * @param y integer coordinate
     * @return double value of the north coordinate
     */
    double NorthFromY(int y);

    const int BORDER_UP = 0;
    const int BORDER_DOWN = 570;
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 1013;
private:
    const int EASTERN_COORD = 3326932;
    const int NORTHERN_COORD = 6824973;
    const double COORD_MULT = 0.711805;
};
}
#endif // COORDINATES_H
