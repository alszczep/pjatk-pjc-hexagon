#include "Points.h"

using namespace Game;

Points::Points(unsigned short red, unsigned short blue) {
    this->red = red;
    this->blue = blue;
}

unsigned short Points::getTeamPoints(Side side) const {
    switch (side) {
        case Side::RedSide:
            return this->red;
        case Side::BlueSide:
            return this->blue;
        default:
            throw std::runtime_error("Invalid side");
    }
}
