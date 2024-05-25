#ifndef PJC_HEXAGON_CONSOLEUI_H
#define PJC_HEXAGON_CONSOLEUI_H

#include <algorithm>
#include <iostream>
#include "UI.h"
#include "../Game/Field.h"
#include "../Game/Teams.h"
#include "../Game/Move.h"
#include "../FileManagement/FileManager.h"

namespace UI {
    enum CellModifier {
        None = 0,
        Selected = 1,
        MovePossible = 2,
    };

    class ConsoleUI : public UI {
    private:
        /**
         * Displays whole board with labels by displaying every row in it with \p displayBoardRow.
         * @param board Board to be displayed
         * @param side Side currently making a move
         * @param selectedField If provided, the field will be highlighted, as well as all the field
         * to which a move can be made from that field
         */
        static void displayBoard(
                const Game::Board &board,
                const Game::Side &side,
                std::optional<Game::Field *> selectedField);

        /**
         * Displays board's row by displaying every cell in it with \p displayBoardCell.
         * @param row Row of fields to be displayed
         * @param rowIndex Index of a row being displayed
         * @param selectedField If found in the row, the cell will be displayed with \p Selected modifier
         * @param fieldsToHighlight If found in the row, the cell will be displayed with \p MovePossible modifier
         */
        static void displayBoardRow(
                const std::vector<Game::Field *> &row,
                short rowIndex,
                std::optional<Game::Field *> selectedField,
                std::vector<Game::Field *> fieldsToHighlight);

        /**
         * @param middleChar Indicates a status of the field
         * @param modifier Displayed for cells which represent selected field or one to which move can be made
         * @param charColor ANSI code for changing the color of the passed char, can be an empty string to skip the color change
         */
        static void displayBoardCell(char middleChar, CellModifier modifier, const std::string &charColor);

        /**
         * Displays name of the team currently making a move.
         * @param side Side which is currently making a move
         */
        static void displayMoveTeam(Game::Side side);

        /**
         * Displays current score for each team.
         */
        static void displayPoints(Game::Points points);

        static char fieldStateToChar(Game::FieldState state);

        static std::string fieldStateToColor(Game::FieldState state);

        /**
         * Asked before user makes a move, can allows actions like saving and loading a game.
         * @return If the game is loaded correctly, its data will be returned
         */
        std::optional<FileManagement::DeserializedGame> askForBeforeMoveActions(
                const Game::Board &board,
                const Game::Side &side,
                const Game::Teams &teams) const;

        /**
         * Used after the user selects a pawn which is going to be moved.
         * @return If false then currently selected field should get unselected
         */
        static bool askIfMoveShouldBeContinued();

    public:
        ConsoleUI() = default;

        std::optional<FileManagement::DeserializedGame> loadGame() const override;

        void saveGame(const Game::Teams &teams, const Game::Side &side, const Game::Board &board) const override;

        std::optional<Game::Teams *> getTeams() override;

        MoveOrLoad getMove(const Game::Board &board, const Game::Side &side, const Game::Teams &teams) const override;

        /**
         * @return Pawn position picked by the user
         */
        static Game::MoveUnit getMoveUnit();

        void displayEndScreen(const Game::Board &board, const Game::Side &side) const override;

        /**
         * Asks user for console input until valid input is entered.
         * @param question Is displayed to the user before asking for input
         * @param validAnswers Vector of user inputs which are considered valid.
         * If user's input is not valid, the error will be displayed and the question will be
         * shown once again
         * @return String entered by user, which is one of \p validAnswers
         */
        static std::string askForInput(const std::string &question, std::vector<std::string> validAnswers);

        /**
         * Works the same as the other \p askForInput overload, but uses a \p predicate function
         * for input validation.
         */
        static std::string askForInput(const std::string &question, const std::function<bool(std::string)> &predicate);
    };
}

#endif //PJC_HEXAGON_CONSOLEUI_H
