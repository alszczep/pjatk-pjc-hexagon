#include "ConsoleUI.h"

using namespace UI;

std::optional<Game::Teams *> ConsoleUI::getTeams() {
    std::string input = ConsoleUI::askForInput(
            "Choose game mode:\n"
            "[1] Player vs Player\n"
            "[2] Player vs Computer\n"
            "[3] Load the game",
            {"1", "2", "3"});

    if (input == "3") return std::nullopt;

    if (input == "1")
        return new Game::Teams(new Game::Team(Game::Side::RedSide, Game::TeamType::Player),
                               new Game::Team(Game::Side::BlueSide, Game::TeamType::Player));
    return new Game::Teams(new Game::Team(Game::Side::RedSide, Game::TeamType::Player),
                           new Game::Team(Game::Side::BlueSide, Game::TeamType::Computer));
}

MoveOrLoad ConsoleUI::getMove(
        const Game::Board &board,
        const Game::Side &side,
        const Game::Teams &teams) const {
    std::optional<Game::Move> move;

    do {
        if (move.has_value()) std::cout << "Move is illegal" << std::endl;

        displayBoard(board, side, std::nullopt);
        displayPoints(board.getPoints());
        displayMoveTeam(side);

        std::optional<FileManagement::DeserializedGame> loadedGame = askForBeforeMoveActions(board, side, teams);
        if (loadedGame.has_value()) return {std::nullopt, loadedGame.value()};

        // selecting a pawn to be moved, looped until the user selects correct field with a pawn that can be moved
        std::optional<Game::MoveUnit> from;
        std::optional<Game::Field *> fromField;
        bool shouldMoveBeContinued = false;
        bool hasLegalMoves;

        do {
            // errors should be displayed only when user has provided some input
            if (from.has_value()) {
                if (fromField.has_value() && fromField.value()->getState() != Game::FieldState::Empty &&
                    fromField.value()->getState() != Game::FieldState::Blocked)
                    std::cout << "No moves can be made with picked pawn" << std::endl;
                else
                    std::cout << "Field is invalid" << std::endl;
            }

            std::cout << "Which pawn do you want to move?" << std::endl;
            from = getMoveUnit();
            fromField = board.getFieldByMoveUnit(from.value());
            hasLegalMoves = fromField.has_value() && !board.findLegalMoves(side, fromField.value()).empty();

            // user should be asked if move should be continued only when the pawn is a valid selection
            if (hasLegalMoves) {
                shouldMoveBeContinued = askIfMoveShouldBeContinued();
                // if move shouldn't be continued, then errors won't be displayed
                if (!shouldMoveBeContinued) from = std::nullopt;
            }
        } while (!hasLegalMoves || !shouldMoveBeContinued);

        displayBoard(board, side, fromField.value());

        std::cout << "To which field do you the pawn to be moved?" << std::endl;
        Game::MoveUnit to = getMoveUnit();

        move = Game::Move(from.value(), to);
    } while (!board.isMoveLegal(side, move.value()));

    return {move, std::nullopt};
}

std::optional<FileManagement::DeserializedGame> ConsoleUI::askForBeforeMoveActions(
        const Game::Board &board,
        const Game::Side &side,
        const Game::Teams &teams) const {
    std::string input;

    do {
        input = ConsoleUI::askForInput(
                "Choose an action:\n"
                "[1] Make a move\n"
                "[2] Save the game\n"
                "[3] Load saved game",
                {"1", "2", "3"});

        if (input == "1") return std::nullopt;
        else if (input == "2") saveGame(teams, side, board);
        else if (input == "3") {
            std::optional<FileManagement::DeserializedGame> loadedGame = loadGame();
            if (loadedGame.has_value()) return loadedGame.value();
        }
    } while (true);
}

bool ConsoleUI::askIfMoveShouldBeContinued() {
    std::string input = ConsoleUI::askForInput(
            "Choose an action:\n"
            "[1] Continue the move\n"
            "[2] Unselect current pawn",
            {"1", "2"});
    return input == "1";
}

Game::MoveUnit ConsoleUI::getMoveUnit() {
    std::string row = askForInput(
            "Enter the row:",
            [](const std::string &input) {
                short inputNumber;

                try {
                    inputNumber = std::stoi(input);
                } catch (const std::exception &) {
                    return false;
                }

                return Game::Field::isRowValid(inputNumber - 1);
            });
    std::string column = askForInput(
            "Enter the column:",
            [](const std::string &input) {
                short inputNumber;

                try {
                    inputNumber = std::stoi(input);
                } catch (const std::exception &) {
                    return false;
                }

                return Game::Field::isUiColumnValid(inputNumber - 1);
            });

    return {static_cast<unsigned short>(std::stoi(row) - 1), static_cast<unsigned short>(std::stoi(column) - 1)};
}

void ConsoleUI::displayEndScreen(const Game::Board &board, const Game::Side &side) const {
    displayBoard(board, side, std::nullopt);

    Game::Points points = board.getPoints();
    displayPoints(points);

    std::string team =
            points.getTeamPoints(Game::Side::RedSide) > points.getTeamPoints(Game::Side::BlueSide) ? "Red" : "Blue";
    std::cout << team << " team won";
}

std::string ConsoleUI::askForInput(const std::string &question, std::vector<std::string> validAnswers) {
    std::string answer;

    do {
        if (!answer.empty()) std::cout << "Invalid input" << std::endl;
        std::cout << question << std::endl;
        std::cin >> answer;
    } while (std::find(validAnswers.begin(), validAnswers.end(), answer) == validAnswers.end());

    std::cout << std::endl;

    return answer;
}

std::string ConsoleUI::askForInput(const std::string &question, const std::function<bool(std::string)> &predicate) {
    std::string answer;

    do {
        if (!answer.empty()) std::cout << "Invalid input" << std::endl;
        std::cout << question << std::endl;
        std::cin >> answer;
    } while (!predicate(answer));

    std::cout << std::endl;

    return answer;
}

void
ConsoleUI::displayBoard(const Game::Board &board, const Game::Side &side, std::optional<Game::Field *> selectedField) {
    // displays column labels
    std::cout << "    ";
    for (short i = 0; i < BOARD_COLUMNS_COUNT; i++) {
        // 49 is an ascii for '1'
        displayBoardCell(i + 49, None, "");
    }
    std::cout << std::endl << std::endl;

    // finds all the fields to which a move can be made from the selected field (if provided)
    std::vector<Game::Field *> fieldsToHighlight;
    if (selectedField.has_value()) {
        std::vector<Game::MoveWithBorderingStatus> moves = board.findLegalMoves(side, selectedField);
        std::for_each(moves.begin(), moves.end(), [&board, &fieldsToHighlight](Game::MoveWithBorderingStatus move) {
            std::optional<Game::Field *> field = board.getFieldByMoveUnit(move.to);

            if (field.has_value()) fieldsToHighlight.emplace_back(field.value());
        });
    }

    for (short i = 0; i < BOARD_ROWS_COUNT; i++) {
        displayBoardRow(board.getFields()[i], i, selectedField, fieldsToHighlight);
    }
}

void ConsoleUI::displayBoardRow(
        const std::vector<Game::Field *> &row,
        short rowIndex,
        std::optional<Game::Field *> selectedField,
        std::vector<Game::Field *> fieldsToHighlight) {
    std::string rowNumberLabel = rowIndex + 1 > 9 ? "  " : "   ";
    std::cout << rowIndex + 1 << rowNumberLabel;

    short sideEmptyColumns = (BOARD_COLUMNS_COUNT - (row.size() * 2 - 1)) / 2;

    for (short uiColumn = 0; uiColumn < BOARD_COLUMNS_COUNT; uiColumn++) {
        // display empty cells on the sides
        if (uiColumn < sideEmptyColumns || uiColumn >= BOARD_COLUMNS_COUNT - sideEmptyColumns) {
            displayBoardCell(' ', None, "");
            continue;
        }

        // display empty cells between the fields
        if (uiColumn % 2 == row.size() % 2) displayBoardCell(' ', None, "");
            // display fields
        else {
            // calculates a cell modifier based on provided fields
            CellModifier modifier = None;
            if (selectedField.has_value() && selectedField.value()->getRow() == rowIndex &&
                selectedField.value()->getUiColumn() == uiColumn)
                modifier = Selected;
            else {
                auto it = std::find_if(fieldsToHighlight.begin(), fieldsToHighlight.end(),
                                       [rowIndex, uiColumn](Game::Field *field) {
                                           return field->getRow() == rowIndex && field->getUiColumn() == uiColumn;
                                       });

                if (it != fieldsToHighlight.end()) modifier = MovePossible;
            }

            Game::FieldState fieldState = row[(uiColumn - sideEmptyColumns) / 2]->getState();
            displayBoardCell(fieldStateToChar(fieldState), modifier, fieldStateToColor(fieldState));
        }
    }

    std::cout << std::endl;
}

void ConsoleUI::displayBoardCell(char middleChar, CellModifier modifier, const std::string &charColor) {
    switch (modifier) {
        case None:
            std::cout << "  " << charColor << middleChar << "\033[0m" << "  ";
            return;
        case Selected:
            std::cout << " [" << charColor << middleChar << "\033[0m" << "] ";
            return;
        case MovePossible:
            std::cout << " (" << charColor << middleChar << "\033[0m" << ") ";
            return;
    }
}

void ConsoleUI::displayMoveTeam(Game::Side side) {
    switch (side) {
        case Game::Side::RedSide:
            std::cout << "Red team's move" << std::endl;
            return;
        case Game::Side::BlueSide:
            std::cout << "Blue team's move" << std::endl;
    }
}

void ConsoleUI::displayPoints(Game::Points points) {
    std::cout << "Points - Red: " << points.getTeamPoints(Game::Side::RedSide)
              << " - Blue: " << points.getTeamPoints(Game::Side::BlueSide) << std::endl;
}

char ConsoleUI::fieldStateToChar(Game::FieldState state) {
    switch (state) {
        case Game::FieldState::Blocked:
            return 'X';
        case Game::FieldState::Red:
            return 'R';
        case Game::FieldState::Blue:
            return 'B';
        case Game::FieldState::Empty:
        default:
            return '0';
    }
}

std::string ConsoleUI::fieldStateToColor(Game::FieldState state) {
    switch (state) {
        case Game::FieldState::Blocked:
            return "";
        case Game::FieldState::Red:
            return "\033[31m";
        case Game::FieldState::Blue:
            return "\033[34m";
        case Game::FieldState::Empty:
        default:
            return "";
    }
}

std::optional<FileManagement::DeserializedGame> ConsoleUI::loadGame() const {
    std::string filename = askForInput("Enter the save name:", [](const std::string &answer) { return true; });

    std::optional<std::string> saveFile = FileManagement::FileManager::loadSaveFile(filename);
    if (!saveFile.has_value()) {
        std::cout << "Failed to load the file" << std::endl;
        return std::nullopt;
    }

    std::optional<FileManagement::DeserializedGame> game =
            FileManagement::GameSerializer::deserializeGame(saveFile.value());
    if (!game.has_value()) {
        std::cout << "File is corrupted" << std::endl;
        return std::nullopt;
    }

    return game.value();
}

void ConsoleUI::saveGame(const Game::Teams &teams, const Game::Side &side, const Game::Board &board) const {
    std::string game = FileManagement::GameSerializer::serializeGame(teams, side, board);

    std::string filename = askForInput("Enter the save name:", [](const std::string &answer) { return true; });

    bool saveSuccessful = FileManagement::FileManager::createSaveFile(filename, game);

    if (saveSuccessful) std::cout << "Game successfully saved" << std::endl;
    else std::cout << "Failed to save the game" << std::endl;
}
