#ifndef PJC_HEXAGON_GAMESERIALIZER_H
#define PJC_HEXAGON_GAMESERIALIZER_H

#include <sstream>
#include "../Game/Board.h"
#include "../Game/Teams.h"

namespace FileManagement {
    class DeserializedGame {
    public:
        Game::Teams *teams;
        Game::Side side;
        Game::Board *board;

        DeserializedGame() = default;

        DeserializedGame(Game::Teams *teams, Game::Side side, Game::Board *board);
    };

    class DeserializedRankingRecord {
    public:
        unsigned short redPoints;
        unsigned short bluePoints;

        DeserializedRankingRecord() = default;

        DeserializedRankingRecord(unsigned short redPoints, unsigned short bluePoints);
    };

    /**
     * Deserialize methods can return a null option if any exception occurs, every deviation in the passed data
     * will be treated as an exception.
     */
    class GameSerializer {
    private:
        static std::string serializeTeams(const Game::Teams &teams);

        static std::string serializeSide(const Game::Side &side);

        static std::string serializeBoard(const Game::Board &board);

        static std::optional<Game::Teams *> deserializeTeams(std::vector<std::string> teamsLines);

        static std::optional<Game::Side> deserializeSide(const std::string &side);

        static std::optional<Game::Board *> deserializeBoard(std::vector<std::string> boardLines);

    public:
        static std::string serializeGame(const Game::Teams &teams, const Game::Side &side, const Game::Board &board);

        static std::optional<DeserializedGame> deserializeGame(const std::string &game);

        static std::string serializeRanking(const std::vector<DeserializedRankingRecord> &points);

        static std::optional<std::vector<DeserializedRankingRecord>> deserializeRanking(const std::string &ranking);

        static std::vector<std::string> splitString(const std::string &stringToSplit, char delimiter);
    };
}

#endif //PJC_HEXAGON_GAMESERIALIZER_H
