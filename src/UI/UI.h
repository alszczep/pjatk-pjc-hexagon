#ifndef PJC_HEXAGON_UI_H
#define PJC_HEXAGON_UI_H

#include "../Game/Teams.h"
#include "../Game/Board.h"
#include "../FileManagement/GameSerializer.h"

namespace UI {
    class MoveOrLoad {
    public:
        std::optional<Game::Move> move;
        std::optional<FileManagement::DeserializedGame> loadedGame;

        MoveOrLoad() = default;

        MoveOrLoad(std::optional<Game::Move> move, std::optional<FileManagement::DeserializedGame> loadedGame) {
            this->move = move;
            this->loadedGame = loadedGame;
        }
    };

    /**
     * This class has been created in order to simplify creating of new UI implementations, as its methods
     * reflect three possible states of the app.
     */
    class UI {
    public:
        /**
         * Displays initial screen.
         * @return Teams picked by the user, null option means that a game save should be loaded
         */
        virtual std::optional<Game::Teams *> getTeams() = 0;

        /**
         * Asks user for game save filename and loads the save, displays errors if there are any
         * @return Deserialized game data when loaded successfully, null option otherwise
         */
        virtual std::optional<FileManagement::DeserializedGame> loadGame() const = 0;

        /**
         * Saves the passed game state, and displays errors if there are any
         */
        virtual void saveGame(const Game::Teams &teams, const Game::Side &side, const Game::Board &board) const = 0;

        /**
         * Handles all of display inside the actual game: display of the board, score and getting the move from the user.
         * @param board Current board
         * @param side Side making a move
         * @param teams Needed for saving the game
         * @return If user can make a move then returned object will contain a move field,
         * if there is a loadedGame field, move should be made, but the game should be reloaded with returned data.
         * If both options are null, then no move can be made
         */
        virtual MoveOrLoad getMove(
                const Game::Board &board,
                const Game::Side &side,
                const Game::Teams &teams) const = 0;

        /**
         * Displays the results after the game finishes.
         * @param board Current board
         * @param side Side making a move, might be needed for board display,
         * but it's not an actual winning side, winning side should be calculated out of the board.
         */
        virtual void displayEndScreen(const Game::Board &board, const Game::Side &side) const = 0;
    };
}

#endif //PJC_HEXAGON_UI_H
