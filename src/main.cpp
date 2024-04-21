#include <iostream>
#include "UI/ConsoleUI.h"
#include "Game/Game.h"

int main() {
    auto *game = new Game::Game(new UI::ConsoleUI());
    std::optional<FileManagement::DeserializedGame> deserializedGame = game->initializeTeams();
    if (deserializedGame.has_value())
        game->startGame(deserializedGame->teams, deserializedGame->side, deserializedGame->board);
    else game->startGame();

    return 0;
}
