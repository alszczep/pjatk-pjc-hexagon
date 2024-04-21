#ifndef PJC_HEXAGON_MOVE_H
#define PJC_HEXAGON_MOVE_H

#include "Field.h"

namespace Game {
    // pointer to some space on the board, may not point to the actual field
    // needed for storing data like user input
    class MoveUnit {
    public:
        unsigned short row;
        unsigned short uiColumn;

        MoveUnit() = default;

        MoveUnit(unsigned short row, unsigned short uiColumn);
    };

    class Move {
    public:
        MoveUnit from;
        MoveUnit to;

        Move() = default;

        Move(MoveUnit from, MoveUnit to);
    };

    class MoveWithBorderingStatus : public Move {
    public:
        // moves which take place between the fields that are next to each other are considered to be bordering,
        // move between those fields will trigger a duplication of a pawn
        bool isBordering;

        MoveWithBorderingStatus() = default;

        MoveWithBorderingStatus(MoveUnit from, MoveUnit to, bool isBordering);
    };
}


#endif //PJC_HEXAGON_MOVE_H
