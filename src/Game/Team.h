#ifndef PJC_HEXAGON_TEAM_H
#define PJC_HEXAGON_TEAM_H

#include <functional>
#include <stdexcept>
#include "Field.h"

namespace Game {
    enum Side {
        RedSide = 0,
        BlueSide = 1,
    };

    enum TeamType {
        Player = 0,
        Computer = 1,
    };

    class Team {
    protected:
        Side side;
        TeamType type;
    public:
        Team() = default;

        Team(Side side, TeamType type);

        static Game::FieldState sideToFieldStatus(Side side);

        Side getSide() const;

        TeamType getType() const;
    };
}

#endif //PJC_HEXAGON_TEAM_H
