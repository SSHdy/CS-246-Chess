#pragma once
#define DELTAS_CHESS_GAME_STATUS_H

#include <optional>
#include <cassert>
#include <string>
#include <string_view>

#include "moves.hpp"
#include "containers.hpp"
#include "pieces.hpp"
#include "code_utils.inc"

using BoolPair = colored_pair<bool>;
using CoorPair = colored_pair<BoardCoor>;


enum class E_Result : u8 {
    Unfinished,
    WhiteWin,
    BlackWin,
    Draw
};


struct GameStatus {
    E_Color next_turn(RawMove rm) { this->move_record.add_move(rm); return this->get_turn(); }
    E_Color get_turn() const { return this->move_record.curr_move(); }
    void set_turn(E_Color cur) { this->move_record.set_curr(cur); }
    void clear() { 
        this->res = E_Result::Unfinished;
        can_castle_short = { true, true };
        can_castle_long = { true, true };
        last_double_pawn_move = constant::INVALID_COOR;
        move_record.clear();
    }

    E_Result res = E_Result::Unfinished;
    BoolPair can_castle_short = { true, true };
    BoolPair can_castle_long = { true, true };
    BoardCoor last_double_pawn_move = constant::INVALID_COOR;
    MoveRecord move_record;
};
