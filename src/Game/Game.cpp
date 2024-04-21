#include "Game.h"

Game::Game::Game(UI::UI *ui) : Game() {
    this->ui = ui;
}

std::optional<FileManagement::DeserializedGame> Game::Game::initializeTeams() {
    do {
        std::optional<Teams *> uiTeams = this->ui->getTeams();

        if (uiTeams.has_value()) {
            this->teams = uiTeams.value();
            return std::nullopt;
        }

        std::optional<FileManagement::DeserializedGame> deserializedGame = this->ui->loadGame();

        if (deserializedGame.has_value()) return deserializedGame;
    } while (true);
}

void Game::Game::startGame() {
    this->board = new Board();
    this->startGameLoop(Side::RedSide);
}

void Game::Game::startGame(Teams *_teams, Side startingSide, Board *_board) {
    this->teams = _teams;
    this->board = _board;
    this->startGameLoop(startingSide);
}

void Game::Game::startGameLoop(Side startingSide) {
    this->currentSide = startingSide;

    while (!this->board->isGameFinished()) {
        UI::MoveOrLoad moveOrLoad;
        Team *currentTeam;

        if (this->currentSide == Side::RedSide) currentTeam = this->teams->getRed();
        else currentTeam = this->teams->getBlue();

        if (currentTeam->getType() == TeamType::Player) {
            std::vector<MoveWithBorderingStatus> legalMoves =
                    this->board->findLegalMoves(this->currentSide, std::nullopt);

            // move should be skipped if there are no legal moves
            if (!legalMoves.empty())
                moveOrLoad = this->ui->getMove(*(this->board), this->currentSide, *(this->teams));
            else moveOrLoad = UI::MoveOrLoad(std::nullopt, std::nullopt);
        } else if (currentTeam->getType() == TeamType::Computer)
            moveOrLoad = UI::MoveOrLoad(this->board->findBestMove(this->currentSide), std::nullopt);

        if (moveOrLoad.loadedGame.has_value()) {
            this->startGame(moveOrLoad.loadedGame->teams, moveOrLoad.loadedGame->side, moveOrLoad.loadedGame->board);
            return;
        }
        if (moveOrLoad.move.has_value()) this->board->makeMove(this->currentSide, moveOrLoad.move.value());

        // switching sides
        if (this->currentSide == Side::RedSide) this->currentSide = Side::BlueSide;
        else if (this->currentSide == Side::BlueSide) this->currentSide = Side::RedSide;

        // it is possible that one of the teams loses all the pawns when there are still empty fields,
        // there is no sense in continuing a game like that
        Points points = board->getPoints();
        if (points.getTeamPoints(RedSide) == 0) board->fillBoardWithState(Blue);
        if (points.getTeamPoints(BlueSide) == 0) board->fillBoardWithState(Red);
    }

    this->ui->displayEndScreen(*(this->board), this->currentSide);
    updateRanking();
}

void Game::Game::updateRanking() {
    Points points = this->board->getPoints();

    std::optional<std::string> rankingFile = FileManagement::FileManager::loadRankingFile();
    if (!rankingFile.has_value()) return;

    auto rankingRecords = FileManagement::GameSerializer::deserializeRanking(rankingFile.value());
    if (!rankingRecords.has_value()) return;

    std::vector<FileManagement::DeserializedRankingRecord> allRecords = rankingRecords.value();
    allRecords.emplace_back(FileManagement::DeserializedRankingRecord(
            points.getTeamPoints(RedSide),
            points.getTeamPoints(BlueSide)));

    std::sort(allRecords.begin(), allRecords.end(),
              [](FileManagement::DeserializedRankingRecord left, FileManagement::DeserializedRankingRecord right) {
                  return abs(left.redPoints - left.bluePoints) > abs(right.redPoints - right.bluePoints);
              });

    // no more than 5 records should be saved
    if (allRecords.size() > 5)
        allRecords = {allRecords.begin(), allRecords.begin() + 5};

    std::string serializedRanking = FileManagement::GameSerializer::serializeRanking(allRecords);
    FileManagement::FileManager::updateRankingFile(serializedRanking);
}
