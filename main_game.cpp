#include "main_game.hpp"


MainGame::MainGame() : whiteScore(0), blackScore(0) {
    _game.init<StandardBoardOperator, StandardMoveFactory>();
}

void MainGame::updateScores(const std::string& winner) {
    if (winner == "white") {
        whiteScore += 1;
    } else if (winner == "black") {
        blackScore += 1;
    } else if (winner == "draw") {
        whiteScore += 0.5;
        blackScore += 0.5;
    }
}

void MainGame::set_display(bool graphic) {
     if (graphic) {
         // _up_display = std::make_unique<GraphicDisplay>();
     } else {
         _up_display = std::make_unique<TextDisplay>();
     }

    // _up_display = std::make_unique<GraphicDisplay>();
    _game.attach(_up_display.get());
}

void MainGame::set_player(std::string& P1, std::string& P2) {
    if (P1 == "human") {
        _p1 = std::make_unique<LocalPlayer>(&this->_game);
    }
    else {
        if (P1 == "computer[1]") _p1 = std::make_unique<ComputerLv1>(&this->_game);
        if (P1 == "computer[2]") _p1 = std::make_unique<ComputerLv2>(&this->_game);
    }
    
    if (P2 == "human") {
        _p2 = std::make_unique<LocalPlayer>(&this->_game);
    }
    else {
        if (P2 == "computer[1]") _p2 = std::make_unique<ComputerLv1>(&this->_game);
        if (P2 == "computer[2]") _p2 = std::make_unique<ComputerLv2>(&this->_game);
    }
}

int MainGame::run() {
    bool gameActive = true;

    while (gameActive) {
        std::string cm;
        std::cin >> cm;

        if (std::cin.eof()) {
            gameActive = false;
            break;
        }
        if (cm == "game") {
            std::string player1, player2;
            std::cin >> player1;
            std::cin >> player2;
            set_player(player1, player2);
            _up_display->display();
            bool END = false;
            while (!END) {
                std::string cm2;
                std::cin >> cm2;
                if (cm2 == "move") {
                    E_Color turn = _game.get_turn();
                    if (turn == E_Color::White) {
                        _p1->play_move();
                    }
                    else {
                        _p2->play_move();
                    }
                    _up_display->display();
                    if (_game.get_result() == E_Result::WhiteWin) {
                        updateScores("white");
                        END = true;
                    }
                    if (_game.get_result() == E_Result::BlackWin) {
                        updateScores("black");
                        END = true;
                    }
                    if (_game.get_result() == E_Result::Draw) {
                        updateScores("draw");
                        END = true;
                    }
                    if (_game.get_result() == E_Result::Unfinished) {
                        continue;
                    }
                }
                if (cm2 == "resign") {
                    E_Color turn = _game.get_turn();
                    if (turn == E_Color::White) {
                        updateScores("black");
                        END = true;
                    }
                    if (turn == E_Color::Black) {
                        updateScores("white");
                        END = true;
                    }
                }
            }
            _game.reset();
        }
        if (cm == "setup") {
            bool END = false;
            while (!END) {
                std::string cm3;
                std::cin >> cm3;
                if (cm3 == "+") {
                    std::string temp1, temp2;
                    std::cin >> temp1;
                    std::cin >> temp2;
                    BoardCoor coor = BoardCoor(temp2[0] - 'a' + 1, temp2[1] - '0');
                    E_Color the_color = E_Color::White;
                    if (std::isupper(temp1[0])) {
                        the_color = E_Color::White;
                    }
                    else {
                        the_color = E_Color::Black;
                    }

                    OptPiece piece1 = Piece(the_color, StringToPieceType(temp1));
                    _game.set_piece(coor, piece1);
                    _up_display->display();
                }
                if (cm3 == "-") {
                    std::string temp;
                    std::cin >> temp;
                    BoardCoor coor = BoardCoor(temp[0] - 'a' + 1, temp[1] - '0');
                    _game.reset_piece(coor);
                    _up_display->display();
                }
                if (cm3 == "=") {
                    std::string temp;
                    std::cin >> temp;
                    if (temp == "black") {
                        _game.set_turn(E_Color::Black);
                    }
                    if (temp == "white") {
                        _game.set_turn(E_Color::White);
                    }
                }
                if (cm3 == "done") {
                    END = true;
                    break;
                }
            }    
        }
    }
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: " << whiteScore << std::endl;
    std::cout << "Black: " << blackScore << std::endl;

    return 0;
}


/*
        PlayerBase* currentPlayer = _game.getCurrentPlayer();
        bool moveResult = currentPlayer->playMove();

        if (moveResult) {
            _up_display->updateDisplay();
        }

        if (_game->isCheckmate()) {
            std::string winnerColor = _game->getCurrentColor() == WHITE ? "white" : "black";
            updateScores(winnerColor);
            std::cout << "Checkmate! " << winnerColor << " wins!" << std::endl;
            gameActive = false;
        } else if (_game->isStalemate()) {
            updateScores("draw");
            std::cout << "Stalemate! It's a draw!" << std::endl;
            gameActive = false;
        }

        if (gameActive) {
            _game->switchTurns();
        }
        */
