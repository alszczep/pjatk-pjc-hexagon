#ifndef PJC_HEXAGON_TEAMS_H
#define PJC_HEXAGON_TEAMS_H

#include "Team.h"
#include <stdexcept>

namespace Game {
    class Teams {
        Team *red;
        Team *blue;
    public:
        Teams() = default;

        Teams(Team *red, Team *blue);

        Team *getRed() const;

        Team *getBlue() const;
    };
}


#endif //PJC_HEXAGON_TEAMS_H
