#include "display.hpp"

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#include "window.hpp"

const int WindowWidth = 500;
const int WindowHeight = 500;
const int SquareSize = WindowWidth / 8;


TextDisplay::TextDisplay() {
    this->theBoard.resize(8, std::vector<char>(8, ' '));
}

void TextDisplay::display() {
    for (int y = 7; y >= 0; --y) {
        std::cout << (y + 1) << ' ';
        for (int x = 0; x < 8; ++x) {
            std::cout << this->theBoard[x][y] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h" << std::endl;
}

void TextDisplay::update_display(BoardCoor coor, OptPiece piece) {
    char displayChar;
    if (!piece)
        displayChar = '_';
    else
        displayChar = piece->to_char();

    this->theBoard[coor.x - 1][coor.y - 1] = displayChar;
}

/*
GraphicDisplay::GraphicDisplay() : w(WindowWidth, WindowHeight), gridDrawn(false) {
    this->theBoard.resize(8, std::vector<char>(8, ' '));
    drawGrid();
}

GraphicDisplay::~GraphicDisplay() {}

void GraphicDisplay::display() {
    updateBoard();
}

void GraphicDisplay::update_display(BoardCoor coor, OptPiece piece) {
    char pieceChar = ' ';

    if (piece) {
        pieceChar = piece->to_char();
    }

    int x = coor.x - 1;
    int y = coor.y - 1;
    this->theBoard[x][y] = pieceChar;
    drawPiece(x, y, pieceChar);
}

void GraphicDisplay::drawGrid() {
    w.fillRectangle(0, 0, 500, 500, Xwindow::Brown);

    const int xOffset = 25;
    const int yOffset = 25;
    const int squareHalfSize = 50 / 2;

    for (int i = 0; i < 2; i++) {
        for (char c = 'A'; c <= 'H'; ++c) {
            std::string letter(1, c);
            w.drawBigString(50 * (c - 'A' + 1) + squareHalfSize, yOffset + (450 * i), letter, Xwindow::White);        }

        for (int j = 1; j <= 8; ++j) {
            std::string num = std::to_string(j);
            w.drawBigString(xOffset, 50 * (j - 1) + squareHalfSize + 50, num, Xwindow::White);
            w.drawBigString(475 + xOffset, 50 * (j - 1) + squareHalfSize + 50, num, Xwindow::White);
        }
    }
    gridDrawn = true;
}

void GraphicDisplay::drawPiece(int x, int y, char pieceChar) {
    bool isTopHalf = y >= 4;

    if ((x + y) % 2 == 0) {
        w.fillRectangle(50 * (x + 1), 50 * (8 - y), 50, 50, Xwindow::Black);
    } else {
        w.fillRectangle(50 * (x + 1), 50 * (8 - y), 50, 50, Xwindow::White);
    }

    if (pieceChar != ' ' && pieceChar != '_') {
        int color;
        if (isTopHalf && pieceChar >= 'A' && pieceChar <= 'Z') {
            pieceChar = pieceChar - 'A' + 'a';
            color = Xwindow::Blue;
        } else {
            color = (pieceChar >= 'a' && pieceChar <= 'z') ? Xwindow::Blue : Xwindow::Red;
        }
        w.drawBigString(50 * (x + 1) + 25, 50 * (9 - y) - 25, std::string(1, pieceChar), color);
    }
}


void GraphicDisplay::updateBoard() {
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            drawPiece(j, i, this->theBoard[j][i]);
        }
    }
}
*/