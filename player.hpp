#pragma once
#define CS246CHESS_PLAYER_HPP

#include <tuple>
#include <optional>
#include <iostream>
#include <ctime>
#include <random>
#include <ranges>

#include "pieces.hpp"
#include "chess_game.hpp"
#include "containers.hpp"
#include "code_utils.inc"

NAMESPACE_DDDELTA_START
// NVI
class PlayerBase {
public:
    PlayerBase(ChessGame* p_game) : p_game(p_game) {}
    virtual ~PlayerBase() = default;

    bool play_move();

protected:
    virtual RawMove get_move() = 0;

protected:
    ChessGame* p_game;
};


class LocalPlayer : public PlayerBase {
public:
    using PlayerBase::PlayerBase;

protected:
    RawMove get_move() override;
};


class Computer : public PlayerBase {
public:
    using PlayerBase::PlayerBase;
    ~Computer() = 0;

protected:
    const MoveFactoryBase& get_factory();
    const GameStatus& get_status();
    const Board& get_board();
};


inline Computer::~Computer() = default;


class ComputerLv1 : public Computer {
public:
    using Computer::Computer;
    ~ComputerLv1() override = default;

protected:
    RawMove get_move() override;
};


class ComputerLv2 : public Computer {
public:
    using Computer::Computer;
    ~ComputerLv2() override = default;

protected:
    RawMove get_move() override;
};


class ComputerLv3 : public Computer {
public:
    using Computer::Computer;
    ~ComputerLv3() override = default;

protected:
    RawMove get_move() override;
};


class ComputerLv4 : public Computer {
public:
    using Computer::Computer;
    ~ComputerLv4() override = default;

protected:
    RawMove get_move() override;
};
NAMESPACE_DDDELTA_END
