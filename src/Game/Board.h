#ifndef PJC_HEXAGON_BOARD_H
#define PJC_HEXAGON_BOARD_H

#include <algorithm>
#include <optional>
#include "Field.h"
#include "Team.h"
#include "../Consts.h"
#include "Points.h"
#include "Move.h"

namespace Game {
    // blocked fields always need to be the same
    const std::vector<Field *> REQUIRED_INITIAL_FIELDS = {
            new Game::Field(Blocked, 6, 2),
            new Game::Field(Blocked, 9, 1),
            new Game::Field(Blocked, 9, 2),
    };

    // pawns positions at the start of the game
    const std::vector<Field *> INITIAL_FIELDS = {
            new Game::Field(Red, 4, 0),
            new Game::Field(Red, 4, 4),
            new Game::Field(Red, 16, 0),
            new Game::Field(Blue, 0, 0),
            new Game::Field(Blue, 12, 0),
            new Game::Field(Blue, 12, 4),
    };

    class Board {
        std::array<std::vector<Field *>, BOARD_ROWS_COUNT> fields;
    private:
    public:
        /**
         * Creates a board initialized with \p initialFields, and then with \p REQUIRED_INITIAL_FIELDS.
         * All the other fields are filled with empty fields.
         * @param initialFields If not provided, defaults to \p INITIAL_FIELDS
         */
        explicit Board(const std::vector<Field *> &initialFields = INITIAL_FIELDS);

        std::array<std::vector<Field *>, BOARD_ROWS_COUNT> getFields() const;

        bool isGameFinished() const;

        /**
         * @return True if a \p move can be made by the provided \p side
         */
        bool isMoveLegal(Side side, Move move) const;

        /**
         * Handles a move and all other events that should take place when the move is made
         * @param side Side making a move
         * @param move Passing an illegal move will result in an exception being thrown
         */
        void makeMove(Side side, Move move) const;

        /**
         * Handles changing the states of fields around the provided \p field
         * @param side Side making a move
         * @param field Field to which the pawn will be moved
         */
        void runMoveSideEffects(Side side, Field *field) const;

        /**
         * @param field Field to be checked
         * @param isBordering Whether the fields should be search for next to the provided \p field,
         * or one field away from it
         * @return Vector of pointers to fields which can be found around the provided \p field
         */
        std::vector<Field *> findFieldsAround(Field *field, bool isBordering) const;

        /**
         * @return Returns a field pointed to by the \p moveUnit, if one can be found
         */
        std::optional<Field *> getFieldByMoveUnit(MoveUnit moveUnit) const;

        Points getPoints() const;

        /**
         * @param side Side for which legal moves should be found
         * @param field If provided, legal moves returned will be these that can be done from that \p field
         * @return Vector of moves that can be made
         */
        std::vector<MoveWithBorderingStatus> findLegalMoves(Side side, std::optional<Field *> field) const;

        /**
         * @param side Side making a move
         * @return Move which will result in the highest amount of points gained, can return a null option if no
         * move can be made. Used by the computer team
         */
        std::optional<Move> findBestMove(Side side) const;

        /**
         * Won't affect Blocked fields
         */
        void fillBoardWithState(FieldState state) const;
    };
}


#endif //PJC_HEXAGON_BOARD_H
