#include <iostream>
#include "Board.h"

using namespace Game;

Board::Board(const std::vector<Field *> &initialFields) {
    std::vector<Field *> allInitialFields = REQUIRED_INITIAL_FIELDS;
    allInitialFields.insert(allInitialFields.end(), initialFields.begin(), initialFields.end());

    for (short row = 0; row < BOARD_ROWS_COUNT; row++) {

        std::vector<Field *> fieldsRow;

        short columns = Field::columnsInRowByRowIndex(row);

        // fills the board with "FieldState::Empty" fields, but if there is a field with different state
        // specified in the "initialFields", it's being used instead
        for (short column = 0; column < columns; column++) {
            auto initialField = std::find_if(
                    allInitialFields.begin(),
                    allInitialFields.end(),
                    [row, column](Field *f) -> bool {
                        return f->getRow() == row && f->getColumn() == column;
                    });

            if (initialField != std::end(allInitialFields)) {
                fieldsRow.emplace_back(allInitialFields[std::distance(allInitialFields.begin(), initialField)]);
                continue;
            }

            fieldsRow.emplace_back(new Field(Empty, row, column));
        }

        fields[row] = fieldsRow;
    }
}

std::array<std::vector<Field *>, BOARD_ROWS_COUNT> Board::getFields() const {
    return this->fields;
}

bool Board::isGameFinished() const {
    bool redFound = false;
    bool blueFound = false;
    bool emptyFound = false;

    for (int i = 0; i < this->fields.size(); i++) {
        for (auto it = fields[i].begin(); it < fields[i].end(); it++) {
            if ((*it)->getState() == Red) {
                redFound = true;
                continue;
            }
            if ((*it)->getState() == Blue) {
                blueFound = true;
                continue;
            }
            if ((*it)->getState() == Empty) {
                emptyFound = true;
                continue;
            }
        }

        if (redFound && blueFound && emptyFound) return false;
    }

    return true;
}

bool Board::isMoveLegal(Side side, Move move) const {
    std::optional<Field *> fieldFromOption = getFieldByMoveUnit(move.from);
    std::optional<Field *> fieldToOption = getFieldByMoveUnit(move.to);

    if (!fieldFromOption.has_value() || !fieldToOption.has_value()) return false;

    Field *fieldFrom = fieldFromOption.value();
    Field *fieldTo = fieldToOption.value();

    if (fieldFrom->getState() != Team::sideToFieldStatus(side)) return false;

    if (fieldTo->getState() != Empty) return false;

    // checks if move complies with the rules:
    // checks horizontal distance
    if (fieldFrom->getRow() == fieldTo->getRow() && abs(fieldTo->getUiColumn() - fieldFrom->getUiColumn()) != 2)
        return false;
    // checks vertical distance, 4 stands for 2 rows
    if (fieldFrom->getUiColumn() == fieldTo->getUiColumn() && abs(fieldTo->getRow() - fieldFrom->getRow()) > 4)
        return false;
    // checks diagonals
    if ((abs(fieldFrom->getRow() - fieldTo->getRow()) == 1 || abs(fieldFrom->getRow() - fieldTo->getRow()) == 3) &&
        abs(fieldFrom->getUiColumn() - fieldTo->getUiColumn()) > 1)
        return false;
    if (abs(fieldFrom->getRow() - fieldTo->getRow()) == 2 &&
        abs(fieldFrom->getUiColumn() - fieldTo->getUiColumn()) > 2)
        return false;

    return true;
}

void Board::makeMove(Side side, Move move) const {
    Field *fieldFrom = getFieldByMoveUnit(move.from).value();
    Field *fieldTo = getFieldByMoveUnit(move.to).value();

    if (!isMoveLegal(side, move)) {
        throw std::logic_error("Cannot make an illegal move");
    }

    fieldTo->setState(fieldFrom->getState());

    // checks if pawns should be duplicated
    // checks horizontal
    if (fieldFrom->getRow() == fieldTo->getRow())
        fieldFrom->setState(Empty);
    // checks vertical
    if (fieldFrom->getUiColumn() == fieldTo->getUiColumn() && abs(fieldFrom->getRow() - fieldTo->getRow()) == 4)
        fieldFrom->setState(Empty);
    // checks diagonals
    if (abs(fieldFrom->getRow() - fieldTo->getRow()) == 3)
        fieldFrom->setState(Empty);
    if (abs(fieldFrom->getRow() - fieldTo->getRow()) == 2 &&
        abs(fieldFrom->getUiColumn() - fieldTo->getUiColumn()) == 2)
        fieldFrom->setState(Empty);

    runMoveSideEffects(side, fieldTo);
}

void Board::runMoveSideEffects(Side side, Field *field) const {
    FieldState enemyFieldState = side == Side::RedSide ? FieldState::Blue : FieldState::Red;
    FieldState desiredFieldState = field->getState();

    std::vector<Field *> fieldsAround = findFieldsAround(field, true);

    std::for_each(fieldsAround.begin(), fieldsAround.end(), [enemyFieldState, desiredFieldState](Field *field) {
        if (field->getState() == enemyFieldState) field->setState(desiredFieldState);
    });
}

std::vector<Field *> Board::findFieldsAround(Field *field, bool isBordering) const {
    std::vector<Field *> fieldsAround;

    if (isBordering) {
        short borderingRowModifiers[6] = {-1, -1, -2, 1, 1, 2};
        short borderingUiColumnModifiers[6] = {-1, 1, 0, -1, 1, 0};

        for (unsigned short i = 0; i < 6; i++) {
            std::optional<Field *> optionalField = getFieldByMoveUnit(
                    MoveUnit(
                            field->getRow() + borderingRowModifiers[i],
                            field->getUiColumn() + borderingUiColumnModifiers[i]));

            if (optionalField.has_value()) {
                fieldsAround.emplace_back(optionalField.value());
            }
        }

        return fieldsAround;
    }

    short nonBorderingRowModifiers[12] = {-4, -3, -2, 0, 2, 3, 4, 3, 2, 0, -2, -3};
    short nonBorderingUiColumnModifiers[12] = {0, 1, 2, 2, 2, 1, 0, -1, -2, -2, -2, -1};

    for (unsigned short i = 0; i < 12; i++) {
        std::optional<Field *> optionalField = getFieldByMoveUnit(
                MoveUnit(
                        field->getRow() + nonBorderingRowModifiers[i],
                        field->getUiColumn() + nonBorderingUiColumnModifiers[i]));

        if (optionalField.has_value()) {
            fieldsAround.emplace_back(optionalField.value());
        }
    }

    return fieldsAround;
}

std::optional<Field *> Board::getFieldByMoveUnit(MoveUnit moveUnit) const {
    if (moveUnit.row < 0 || moveUnit.row >= BOARD_ROWS_COUNT || moveUnit.uiColumn < 0 ||
        moveUnit.uiColumn >= BOARD_COLUMNS_COUNT)
        return std::nullopt;

    auto it = std::find_if(
            this->fields[moveUnit.row].begin(),
            this->fields[moveUnit.row].end(),
            [moveUnit](Field *field) -> bool {
                return field->getUiColumn() == moveUnit.uiColumn;
            });

    if (it == fields[moveUnit.row].end()) return std::nullopt;
    else return this->fields[moveUnit.row][std::distance(this->fields[moveUnit.row].begin(), it)];
}

Points Board::getPoints() const {
    unsigned short red = 0;
    unsigned short blue = 0;

    for (short row = 0; row < BOARD_ROWS_COUNT; row++) {
        std::for_each(this->fields[row].begin(), this->fields[row].end(), [&red, &blue](Field *field) {
            if (field->getState() == Red) red++;
            if (field->getState() == Blue) blue++;
        });
    }

    return {red, blue};
}

std::vector<MoveWithBorderingStatus> Board::findLegalMoves(Side side, std::optional<Field *> field) const {
    // find all side fields
    std::vector<Field *> allSideFields;
    FieldState desiredFieldState = Team::sideToFieldStatus(side);

    if (field.has_value())
        allSideFields.emplace_back(field.value());
    else
        for (short row = 0; row < BOARD_ROWS_COUNT; row++) {
            std::for_each(this->fields[row].begin(), this->fields[row].end(),
                          [desiredFieldState, &allSideFields](Field *field) {
                              if (field->getState() == desiredFieldState) allSideFields.emplace_back(field);
                          });
        }

    // find all possible moves, categorize which are bordering the original field
    std::vector<MoveWithBorderingStatus> legalMoves;

    std::for_each(allSideFields.begin(), allSideFields.end(),
                  [this, &legalMoves, side](Field *field) {
                      MoveUnit fromMoveUnit(field->getRow(), field->getUiColumn());

                      // runs once for fields bordering the original field, and once for those which do not border
                      for (short i = 0; i < 2; i++) {
                          bool isBordering = i == 0;

                          std::vector<Field *> aroundFields = findFieldsAround(field, isBordering);

                          // of all the fields around the original field find those that are Empty
                          // and can be used to construct a legal move
                          std::for_each(aroundFields.begin(), aroundFields.end(),
                                        [this, &legalMoves, fromMoveUnit, side, isBordering](Field *aroundField) {
                                            MoveUnit toMoveUnit = MoveUnit(aroundField->getRow(),
                                                                           aroundField->getUiColumn());
                                            if (aroundField->getState() == Empty &&
                                                isMoveLegal(side, Move(fromMoveUnit, toMoveUnit)))
                                                legalMoves.emplace_back(MoveWithBorderingStatus(
                                                        fromMoveUnit,
                                                        toMoveUnit,
                                                        isBordering
                                                ));
                                        });
                      }
                  });

    return legalMoves;
}


std::optional<Move> Board::findBestMove(Side side) const {
    std::vector<MoveWithBorderingStatus> legalMoves = findLegalMoves(side, std::nullopt);

    FieldState enemyFieldState = side == RedSide ? Blue : Red;

    if (legalMoves.empty()) return std::nullopt;

    // calculate points for each move and save the best one
    short bestMovePoints = 0;
    Move bestMove = legalMoves[0];

    std::for_each(legalMoves.begin(), legalMoves.end(),
                  [this, enemyFieldState, &bestMovePoints, &bestMove](MoveWithBorderingStatus move) {
                      short points = 0;

                      if (move.isBordering) points++;

                      Field *to = this->getFieldByMoveUnit(move.to).value();
                      std::vector<Field *> fieldsAround = this->findFieldsAround(to, true);

                      std::for_each(fieldsAround.begin(), fieldsAround.end(),
                                    [enemyFieldState, &points](Field *field) {
                                        if (field->getState() == enemyFieldState) {
                                            // double points because enemy loses one point
                                            points += 2;
                                        }
                                    });

                      if (points > bestMovePoints) {
                          bestMovePoints = points;
                          bestMove = move;
                      }
                  });

    return bestMove;
}

void Board::fillBoardWithState(FieldState state) const {
    for (int i = 0; i < this->fields.size(); i++) {
        for (auto it = fields[i].begin(); it < fields[i].end(); it++) {
            if ((*it)->getState() != state && (*it)->getState() != Blocked) {
                (*it)->setState(state);
            }
        }
    }
}
