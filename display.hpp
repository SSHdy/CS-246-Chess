#pragma once
#define CS246CHESS_DISPLAY_HPP

#include <iostream>
#include <vector>
#include "window.hpp"
#include "observer.hpp"
#include "pieces.hpp"
#include "code_utils.inc"


class ChessDisplay : public BoardObserver {
public:
    void get_notified(BoardCoor bc, OptPiece p) final {
        update_display(bc, p);
    }

    virtual void update_display(BoardCoor coor, OptPiece piece) = 0;
    virtual void display() = 0;
};

class TextDisplay : public ChessDisplay {
    std::vector<std::vector<char>> theBoard;

public:
    TextDisplay();
    void display() override;
    void update_display(BoardCoor coor, OptPiece piece) override;
};

/*
class GraphicDisplay : public ChessDisplay {
    Xwindow w;
    std::vector<std::vector<char>> theBoard;
    bool gridDrawn;

public:
    GraphicDisplay();
    ~GraphicDisplay() override;
    void display() override;
    void update_display(BoardCoor coor, OptPiece piece) override;

private:
    void drawGrid();
    void drawPiece(int x, int y, char pieceChar);
    void updateBoard();
};

*/