#ifndef PJC_HEXAGON_GAME_H
#define PJC_HEXAGON_GAME_H

#include <iostream>
#include "../UI/UI.h"
#include "Board.h"
#include "Move.h"
#include "../FileManagement/FileManager.h"

namespace Game {
    class Game {
    private:
        UI::UI *ui;
        Teams *teams;
        Board *board;
        Side currentSide;

        void startGameLoop(Side startingSide);

    public:
        Game() = default;

        /**
         * @param ui UI implementation to be used throughout the game
         */
        explicit Game(UI::UI *ui);

        /**
         * Initialization of teams using the provided UI implementation. At this stage the can be loaded from
         * a save instead of selecting teams.
         * @return If game was loaded from a save, then the loaded game data will be returned
         */
        std::optional<FileManagement::DeserializedGame> initializeTeams();

        /**
         * Starts the actual game, handles players moves and displays and end screen using UI after the game ends.
         */
        void startGame();

        /**
         * Starts a game using existing data, can be used for starting a game loaded from a save.
         */
        void startGame(Teams *_teams, Side startingSide, Board *_board);

        /**
         * Should be called after the game is finished, updates the ranking file
         */
        void updateRanking();
    };
}


#endif //PJC_HEXAGON_GAME_H
