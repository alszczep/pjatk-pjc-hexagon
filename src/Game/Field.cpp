#include <iostream>
#include "Field.h"

using namespace Game;

Field::Field(FieldState state, unsigned short row, unsigned short column) {
    this->state = state;
    this->row = row;
    this->column = column;

    short columns = columnsInRowByRowIndex(row);
    short sideEmptyColumns = (BOARD_COLUMNS_COUNT - (columns * 2 - 1)) / 2;
    this->uiColumn = (sideEmptyColumns + column * 2);
}

void Field::setState(FieldState _state) {
    if (_state == Blocked || this->state == Blocked) {
        // state cannot be change to or from blocked during the game,
        // so setting it shouldn't be possible except initialization
        throw std::logic_error("Field's state cannot be changed to or from blocked");
    }

    this->state = _state;
}

Game::FieldState Field::getState() const {
    return this->state;
}

unsigned short Field::getRow() const {
    return this->row;
}

unsigned short Field::getColumn() const {
    return this->column;
}

unsigned short Field::getUiColumn() const {
    return this->uiColumn;
}

bool Field::isRowValid(short row) {
    return row >= 0 && row < BOARD_ROWS_COUNT;
}

bool Field::isColumnInRowValid(short column, short row) {
    short columns = columnsInRowByRowIndex(row);
    return column >= 0 && column < columns;
}

bool Field::isUiColumnValid(short uiColumn) {
    return uiColumn >= 0 && uiColumn < BOARD_COLUMNS_COUNT;
}

short Field::columnsInRowByRowIndex(short row) {
    short columns;

    // may not work correctly if BOARD_ROWS_COUNT is changed,
    // but for the purpose of this game there is no need to handle that
    if (row == 0 || row == BOARD_ROWS_COUNT - 1) columns = 1;
    else if (row == 1 || row == BOARD_ROWS_COUNT - 2) columns = 2;
    else if (row == 2 || row == BOARD_ROWS_COUNT - 3) columns = 3;
        // all the other columns have alternating values of 4 and 5
    else columns = 5 - (row % 2);

    return columns;
}
