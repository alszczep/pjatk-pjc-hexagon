#include "GameSerializer.h"

using namespace FileManagement;

DeserializedGame::DeserializedGame(Game::Teams *teams, Game::Side side, Game::Board *board) {
    this->teams = teams;
    this->side = side;
    this->board = board;
}

std::string GameSerializer::serializeGame(
        const Game::Teams &teams,
        const Game::Side &side,
        const Game::Board &board) {
    return serializeTeams(teams) + "\n"
           + serializeSide(side) + "\n"
           + serializeBoard(board);
}

std::string GameSerializer::serializeTeams(const Game::Teams &teams) {
    return std::to_string(teams.getRed()->getSide()) + '\n'
           + std::to_string(teams.getRed()->getType()) + '\n'
           + std::to_string(teams.getBlue()->getSide()) + '\n'
           + std::to_string(teams.getBlue()->getType());
}

std::string GameSerializer::serializeSide(const Game::Side &side) {
    return std::to_string(side);
}

// board field scheme: {state},{row},{column}
std::string GameSerializer::serializeBoard(const Game::Board &board) {
    std::string serializedBoard;

    auto boardFields = board.getFields();
    std::for_each(boardFields.begin(), boardFields.end(),
                  [&serializedBoard](const std::vector<Game::Field *> &fields) {
                      std::for_each(fields.begin(), fields.end(), [&serializedBoard](Game::Field *field) {
                          // no need to save empty and blocked fields as these will be autofilled
                          if (field->getState() == Game::FieldState::Red ||
                              field->getState() == Game::FieldState::Blue) {
                              // newline is added before every line except first
                              if (!serializedBoard.empty()) serializedBoard += '\n';
                              serializedBoard +=
                                      std::to_string(field->getState()) + ','
                                      + std::to_string(field->getRow()) + ','
                                      + std::to_string(field->getColumn());
                          }
                      });
                  });

    return serializedBoard;
}

std::optional<DeserializedGame> GameSerializer::deserializeGame(const std::string &game) {
    std::vector<std::string> lines = splitString(game, '\n');

    std::optional<Game::Teams *> teams = deserializeTeams({lines.begin(), lines.begin() + 4});
    std::optional<Game::Side> side = deserializeSide(lines[4]);
    std::optional<Game::Board *> board = deserializeBoard({lines.begin() + 5, lines.end()});

    if (!teams.has_value() || !side.has_value() || !board.has_value()) return std::nullopt;

    return DeserializedGame(teams.value(), side.value(), board.value());
}

std::optional<Game::Teams *> GameSerializer::deserializeTeams(std::vector<std::string> teamsLines) {
    try {
        return new Game::Teams(
                new Game::Team(static_cast<Game::Side>(stoi(teamsLines[0])),
                               static_cast<Game::TeamType>(stoi(teamsLines[1]))),
                new Game::Team(static_cast<Game::Side>(stoi(teamsLines[2])),
                               static_cast<Game::TeamType>(stoi(teamsLines[3]))));
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

std::optional<Game::Side> GameSerializer::deserializeSide(const std::string &side) {
    try {
        return static_cast<Game::Side>(std::stoi(side));
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

std::optional<Game::Board *> GameSerializer::deserializeBoard(std::vector<std::string> boardLines) {
    try {
        std::vector<Game::Field *> fields;

        std::for_each(boardLines.begin(), boardLines.end(), [&fields](const std::string &line) {
            std::vector<std::string> lineParts = splitString(line, ',');
            auto state = static_cast<Game::FieldState>(std::stoi(lineParts[0]));
            short row = std::stoi(lineParts[1]);
            short column = std::stoi(lineParts[2]);

            if (!Game::Field::isRowValid(row) || !Game::Field::isColumnInRowValid(column, row)) throw;

            fields.emplace_back(new Game::Field(state, row, column));
        });

        return new Game::Board(fields);
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

DeserializedRankingRecord::DeserializedRankingRecord(unsigned short redPoints, unsigned short bluePoints) {
    this->redPoints = redPoints;
    this->bluePoints = bluePoints;
}

// ranking record scheme: {index}. Red: {redPoints} - Blue: {bluePoints}
std::string GameSerializer::serializeRanking(const std::vector<DeserializedRankingRecord> &points) {
    std::string ranking;

    for (short i = 0; i < points.size(); i++) {
        if (i != 0) ranking += '\n';
        ranking += std::to_string(i + 1)
                   + ". Red: " + std::to_string(points[i].redPoints)
                   + " - Blue: " + std::to_string(points[i].bluePoints);
    }

    return ranking;
}

std::optional<std::vector<DeserializedRankingRecord>> GameSerializer::deserializeRanking(const std::string &ranking) {
    try {
        std::vector<DeserializedRankingRecord> deserializedRanking;
        std::vector<std::string> rankingLines = splitString(ranking, '\n');

        std::for_each(rankingLines.begin(), rankingLines.end(), [&deserializedRanking](const std::string &line) {
            std::vector<std::string> lineParts = splitString(line, ':');
            std::string red = splitString(lineParts[1], '-')[0];
            // remove spaces
            red.erase(std::remove(red.begin(), red.end(), ' '), red.end());
            std::string blue = lineParts[2];
            blue.erase(std::remove(blue.begin(), blue.end(), ' '), blue.end());

            deserializedRanking.emplace_back(DeserializedRankingRecord(std::stoi(red), std::stoi(blue)));
        });

        return deserializedRanking;
    } catch (const std::exception &) {
        return std::nullopt;
    }
}

std::vector<std::string> GameSerializer::splitString(const std::string &stringToSplit, char delimiter) {
    std::stringstream stream(stringToSplit);
    std::string currentString;
    std::vector<std::string> strings;

    while (std::getline(stream, currentString, delimiter)) {
        strings.push_back(currentString);
    }

    return strings;
}
