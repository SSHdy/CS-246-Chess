#pragma once
#define CS246CHESS_BOARD_OPERATOR_H

#include <optional>

#include "containers.hpp"
#include "board.hpp"
#include "moves.hpp"

#include "code_utils.inc"

class BoardOperatorBase {
public:
    explicit BoardOperatorBase(Board* p_board) : p_board(p_board) {}
    virtual ~BoardOperatorBase() = default;

    virtual bool execute_move(
        BoardCoor selection, 
        PieceMove piece_move, 
        std::optional<E_PieceType> pro_type = nullopt
    ) = 0;

protected:
    Board* p_board;
};


class StandardBoardOperator : public BoardOperatorBase {
public:
    explicit StandardBoardOperator(Board* p_board) : BoardOperatorBase(p_board) {}
    ~StandardBoardOperator() override = default;

    bool execute_move(
        BoardCoor selection, 
        PieceMove piece_move, 
        std::optional<E_PieceType> pro_type = nullopt
    ) override;
};