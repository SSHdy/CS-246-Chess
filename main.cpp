#include "main_game.hpp"

int main() {
    MainGame game;
    game.set_display(true);
    game.set_player();
    return game.run();
}
