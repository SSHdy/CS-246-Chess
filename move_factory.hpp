#pragma once
#define CS246CHESS_MOVE_FACTORY_HPP

#include <array>
#include <vector>
#include <cstdint>
#include <optional>
#include <utility>
#include <memory>
#include <algorithm>
#include <ranges>
#include <cassert>
#include <tuple>
#include <type_traits>
#include <iostream>
#include <cmath>

#include "containers.hpp"
#include "board.hpp"
#include "moves.hpp"
#include "game_status.hpp"
#include "code_utils.inc"

class MoveFactoryBase {
public:
    MoveFactoryBase(const Board* p_bd, const GameStatus* p_status);
    virtual ~MoveFactoryBase() = default;

    virtual std::unique_ptr<PossibleMovement> get_move(BoardCoor selection) const = 0;
    virtual bool is_mated() const = 0;
    virtual bool is_drawn() const = 0;

protected:
    const Board* p_board;
    const GameStatus* p_status;
};


class StandardMoveFactory : public MoveFactoryBase {
public:
    using MoveFactoryBase::MoveFactoryBase;
    ~StandardMoveFactory() override = default;

    std::unique_ptr<PossibleMovement> get_move(BoardCoor selection) const override;
    bool is_mated() const override;
    bool is_drawn() const override;

private:
    std::unique_ptr<PossibleMovement> _get_move(BoardCoor selection) const;
    NODISCARD std::unique_ptr<PossibleMovement> _knight_move(BoardCoor co) const;
    void _diagonal_move(PossibleMovement* p_movement, BoardCoor co) const;
    void _linear_move(PossibleMovement* p_movement, BoardCoor co) const;
    NODISCARD std::unique_ptr<PossibleMovement> _bishop_move(BoardCoor co) const;
    NODISCARD std::unique_ptr<PossibleMovement> _rook_move(BoardCoor co) const;
    NODISCARD std::unique_ptr<PossibleMovement> _queen_move(BoardCoor co) const;
    NODISCARD std::unique_ptr<PossibleMovement> _pawn_move(BoardCoor co) const;
    NODISCARD std::unique_ptr<PossibleMovement> _king_move(BoardCoor co) const;
    void _filter_check_defence(PossibleMovement* p_pm) const;
    NODISCARD E_Color _get_turn() const { return this->p_status->get_turn(); }

    OptPiece _get(BoardCoor coor) const { return this->p_board->get(coor); }
    bool _empty(BoardCoor coor) const { return !static_cast<bool>(this->_get(coor)); }
    std::pair<BoardCoor, BoardCoor> _in_check() const;
    BoardCoor _find_king(E_Color color) const;
};
