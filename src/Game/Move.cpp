#include "Move.h"

Game::Move::Move(MoveUnit from, MoveUnit to) {
    this->from = from;
    this->to = to;
}

Game::MoveUnit::MoveUnit(unsigned short row, unsigned short uiColumn) {
    this->row = row;
    this->uiColumn = uiColumn;
}

Game::MoveWithBorderingStatus::MoveWithBorderingStatus(Game::MoveUnit from, Game::MoveUnit to, bool isBordering)
        : Move(from, to) {
    this->isBordering = isBordering;
}
