#include "Teams.h"

using namespace Game;

Teams::Teams(Team *red, Team *blue) {
    if (red->getSide() != Side::RedSide || blue->getSide() != Side::BlueSide)
        throw std::logic_error("Teams have invalid sides");

    this->red = red;
    this->blue = blue;
}

Team *Teams::Teams::getRed() const {
    return this->red;
}

Team *Teams::Teams::getBlue() const {
    return this->blue;
}

