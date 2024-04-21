#include "Team.h"

using namespace Game;

FieldState Team::sideToFieldStatus(Side side) {
    switch (side) {
        case RedSide:
            return FieldState::Red;
        case BlueSide:
            return FieldState::Blue;
        default:
            throw std::logic_error("Invalid side");
    }
}

Team::Team(Side side, TeamType type) {
    this->side = side;
    this->type = type;
}

Side Team::getSide() const {
    return side;
}

TeamType Team::getType() const {
    return type;
}
