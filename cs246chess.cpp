// cs246chess.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "main_game.hpp"

int main(int argc, char* argv[])
{
    MainGame game;
    game.set_display(false);
    return game.run();
}