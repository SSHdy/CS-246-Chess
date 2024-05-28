#pragma once
#define CS246CHESS_MOVES_HPP

#include <vector>
#include <cstdint>
#include <tuple>
#include <string_view>
#include <optional>
#include <string>

#include "containers.hpp"
#include "pieces.hpp"
#include "code_utils.inc"

struct RawMove {
    BoardCoor from;
    BoardCoor to;
    std::optional<E_PieceType> promotion;

    bool operator ==(const RawMove& rhs) const = default;
    explicit operator bool() const { return static_cast<bool>(promotion); }
};


namespace constant {
inline constexpr RawMove INVALID_MOVE { constant::INVALID_COOR, constant::INVALID_COOR, nullopt };
}


RawMove parse_move(const std::string& raw_command);


class MoveRecord {
public:
    MoveRecord() = default;

    RawMove operator [](std::size_t i) const { return this->_moves[i - 1]; }
    void clear() { _moves.clear(); _curr_move = E_Color::White; }
    std::size_t size() const { return this->_moves.size(); }
    E_Color curr_move() const { return this->_curr_move; }
    E_Color add_move(RawMove rm);
    void set_curr(E_Color cur) { this->_curr_move = cur; }

private:
    E_Color _curr_move = E_Color::White;
    std::vector<RawMove> _moves;
};


enum class E_UniqueAction : u8 {
    None,
    ShortCastle,
    LongCastle,
    EnPassant,
    DoublePawnPush,
    Promote
};


struct PieceMove {
    BoardCoor coor;
    E_UniqueAction unique_action;
};


struct PossibleMovement {
    std::vector<PieceMove> moves;
    std::vector<PieceMove> captures;
    std::vector<PieceMove> protects;

    NODISCARD bool unmoveable() const { return this->moves.empty() && this->captures.empty(); }
};
