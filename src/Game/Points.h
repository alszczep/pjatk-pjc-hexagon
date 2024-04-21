#ifndef PJC_HEXAGON_POINTS_H
#define PJC_HEXAGON_POINTS_H

#include <stdexcept>
#include "Team.h"

namespace Game {
    class Points {
        unsigned short red;
        unsigned short blue;
    public:
        Points() = default;

        Points(unsigned short red, unsigned short blue);

        unsigned short getTeamPoints(Game::Side side) const;
    };
}


#endif //PJC_HEXAGON_POINTS_H
