#include "board.hpp"

Board::Board() : _board(constant::standard_starting_position) {}


void Board::attach(BoardObserver* p_o) {
    BoardSubject::attach(p_o);

    for (i32 i : stdvws::iota(1, 9))
        for (i32 j : stdvws::iota(1, 9))
            this->get_observer().get_notified({ i, j }, this->get(i, j));
}


Board& Board::set(BoardCoor coor, OptPiece opt_piece) {
    this->_get(coor) = opt_piece;
    this->get_observer().get_notified(coor, this->get(coor));
    return *this;
}


Board& Board::set(i32 x, i32 y, OptPiece opt_piece) {
    return this->set({ x, y }, opt_piece);
}


Board& Board::reset(BoardCoor coor) {
    this->_get(coor).reset();
    this->get_observer().get_notified(coor, this->get(coor));
    return *this;
}


Board& Board::reset(i32 x, i32 y) {
    return this->reset({ x, y });
}


void Board::clear() {
    this->_board = constant::standard_starting_position;

    for (i32 i : stdvws::iota(1, 9))
        for (i32 j : stdvws::iota(1, 9))
            this->get_observer().get_notified({ i, j }, this->get(i, j));
}
