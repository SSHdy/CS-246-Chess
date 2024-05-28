#pragma once
#define DELTAS_CHESS_CHESS_GAME_H

#include <vector>
#include <cstdint>
#include <utility>
#include <tuple>
#include <memory>
#include <optional>
#include <cassert>
#include <algorithm>
#include <ranges>
#include <type_traits>

#include "pieces.hpp"
#include "game_status.hpp"
#include "board.hpp"
#include "moves.hpp"
#include "board_operator.hpp"
#include "move_factory.hpp"
#include "code_utils.inc"

class ChessGame {
public:
    ChessGame();

    // init must be called before any other funtion is called
    template <typename OperatorVariant, typename FactoryVariant> 
        requires (
            std::is_base_of_v<BoardOperatorBase, OperatorVariant> && 
            std::is_base_of_v<MoveFactoryBase, FactoryVariant>
        )
    void init() {
        this->_up_operator = std::make_unique<OperatorVariant>(&this->_board);
        this->_up_factory = std::make_unique<FactoryVariant>(&this->_board, &this->_status);
    }
    
    // getters
    NODISCARD E_Result get_result() const { return this->_status.res; }
    NODISCARD E_Color get_turn() const { return this->_status.get_turn(); }
    NODISCARD OptPiece get_piece(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board.get(x, y); }
    NODISCARD OptPiece get_piece(BoardCoor co) const { assert_on_board_coor(co); return this->_board.get(co); }
    ChessGame& set_piece(BoardCoor co, OptPiece piece) { assert_on_board_coor(co); this->_board.set(co, piece); return *this; }
    ChessGame& set_piece(i32 x, i32 y, OptPiece piece) { assert_on_board_xy(x, y); this->_board.set(x, y, piece); return *this; }
    void reset_piece(BoardCoor co) { assert_on_board_coor(co); this->_board.reset(co); }
    void reset_piece(i32 x, i32 y) { assert_on_board_xy(x, y); this->_board.reset(x, y); }
    void set_turn(E_Color cur) { this->_status.set_turn(cur); }

    // operators
    /*
     * return true if move is legal
     * modifies _status.res when game finished
     * executing INVALID_COOR, INVALID_COOR stands for resignation
     */
    bool execute_move(
        BoardCoor from_coor, 
        BoardCoor target_coor, 
        std::optional<E_PieceType> opt_promotion = nullopt
    );
    void attach(BoardObserver* observer);
    void reset() {
        this->_board.clear();
        this->_status.clear();
    }

private:
    GameStatus _status;
    Board _board;
    std::unique_ptr<BoardOperatorBase> _up_operator;
    std::unique_ptr<MoveFactoryBase> _up_factory;

    friend class Computer;
};
NAMESPACE_DDDELTA_END
