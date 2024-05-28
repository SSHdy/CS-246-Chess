#pragma once
#define CS246CHESS_MAIN_GAME_HPP

#include <memory>
#include <sstream>
#include <cctype>



#include "player.hpp"
#include "display.hpp"
#include "board_operator.hpp"
#include "move_factory.hpp"
#include "chess_game.hpp"

#include "type_traits"
#include "code_utils.inc"


class MainGame {
public:
    MainGame();

    void set_display(bool graphic = true);
    void set_player(std::string& P1, std::string& P2);
    int run();

private:
    std::unique_ptr<ChessDisplay> _up_display;
    std::unique_ptr<PlayerBase> _p1, _p2;
    ChessGame _game;
    double whiteScore;
    double blackScore;
    void updateScores(const std::string& winner);
};
