#pragma once
#define CS246CHESS_BOARD_HPP

#include <array>
#include <cstdint>
#include <cassert>
#include <optional>
#include <utility>
#include <memory>
#include <ranges>
#include <iostream>

#include "containers.hpp"
#include "observer.hpp"
#include "pieces.hpp"
#include "code_utils.inc"

namespace _impl {
template <E_PieceType Type>
inline constexpr std::array<std::optional<Piece>, 8> init_board_file {
    Piece(E_Color::White, Type), constant::WhitePawn, nullopt, nullopt,
    nullopt, nullopt, constant::BlackPawn, Piece(E_Color::Black, Type)
};
}


namespace constant {
inline constexpr RawBoard standard_starting_position {
    _impl::init_board_file<E_PieceType::Rook>,
    _impl::init_board_file<E_PieceType::Knight>,
    _impl::init_board_file<E_PieceType::Bishop>,
    _impl::init_board_file<E_PieceType::Queen>,
    _impl::init_board_file<E_PieceType::King>,
    _impl::init_board_file<E_PieceType::Bishop>,
    _impl::init_board_file<E_PieceType::Knight>,
    _impl::init_board_file<E_PieceType::Rook>
};
}


template <i32 Distance> requires (Distance <= 8 && Distance >= 1)
inline constexpr colored_pair<i32> nth_from_last_rank { 0 + Distance, 9 - Distance };


class Board : public BoardSubject {
public:
    using joined_range = decltype(static_cast<const RawBoard&>(RawBoard()) | stdvws::join);
    using column_citer = RawBoard::const_iterator;
    using piece_citer = std::array<Piece, 8>::const_iterator;

    static constexpr i32 size = 8;

public:
    Board();

    void clear();
    void attach(BoardObserver* p_o) override;
    NODISCARD OptPiece get(BoardCoor coor) const { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    NODISCARD OptPiece get(i32 x, i32 y) const { assert_on_board_xy(x, y); return this->_board[x - 1][y - 1]; }
    Board& set(BoardCoor coor, OptPiece opt_piece); // notifies display
    Board& set(i32 x, i32 y, OptPiece opt_piece); // notifies display
    Board& reset(BoardCoor coor); // notifies display
    Board& reset(i32 x, i32 y); // notifies display

    column_citer cbegin() const { return this->_board.cbegin(); }
    column_citer cend() const { return this->_board.cend(); }
    NODISCARD joined_range board_range() const { return this->_board | stdvws::join; }

private:
    OptPiece& _get(BoardCoor coor) { assert_on_board_coor(coor); return this->_board[coor.x - 1][coor.y - 1]; }
    OptPiece& _get(i32 x, i32 y) { return this->_get({ x, y }); }

private:
    RawBoard _board;
};
