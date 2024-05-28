#include "player.hpp"
#include <string>
#include <type_traits>



bool PlayerBase::play_move() {
    auto [from, to, opt_promo] = this->get_move();
   
    if (!on_board(from) || !on_board(to))
        return false;

    return this->p_game->execute_move(from, to, opt_promo);
}

RawMove LocalPlayer::get_move() {
    std::string str;
    std::getline(std::cin, str);
    return parse_move(str);
}

const MoveFactoryBase& Computer::get_factory() {
    return *p_game->_up_factory;
}

const GameStatus& Computer::get_status() {
    return p_game->_status;
}

const Board& Computer::get_board() {
    return p_game->_board;
}

/*
E_PieceType getRandomPieceType() {
    std::random_device rd; 
    std::mt19937 eng(rd()); 
    std::uniform_int_distribution<> distr(0, static_cast<int>(E_PieceType::Queen)); 

    
    return static_cast<E_PieceType>(distr(eng));
}
*/

RawMove ComputerLv1::get_move() {
    std::mt19937 mt1(time(nullptr));
    std::unique_ptr<PossibleMovement> up_moves = nullptr;
    i32 x1 = mt1() % 8 + 1;
    i32 y1 = mt1() % 8 + 1;
    E_Color temp = get_status().get_turn();
start:
    x1 = mt1() % 8 + 1;
    y1 = mt1() % 8 + 1;
    while (!get_board().get(x1, y1) || get_board().get(x1, y1)->color != temp) {
        x1 = mt1() % 8 + 1;
        y1 = mt1() % 8 + 1;
    }
    up_moves = get_factory().get_move(BoardCoor(x1, y1));
    if (up_moves->unmoveable()) {
        goto start;
    }
    BoardCoor coor1 = BoardCoor(x1, y1);
    auto moves = { up_moves->captures, up_moves->moves };
    auto joined = moves | stdvws::join;
    auto joined_it = joined.begin();
    std::advance(joined_it, mt1() % (up_moves->captures.size() + up_moves->moves.size()));
    BoardCoor coor2 = joined_it->coor;
    std::optional<E_PieceType> promo = nullopt;
    if (joined_it->unique_action == E_UniqueAction::Promote) {
        promo = E_PieceType::Queen;
        // or promo = getRandomPieceType();
    }
    return RawMove(coor1, coor2, promo);
}

bool if_capture(const Board& board, i32 x, i32 y, E_Color color, E_PieceType type) {
    if (type == E_PieceType::Pawn) {
        if (color == E_Color::White) {
            if (y == 8) return false;
            if (x == 8) {
                auto temp = board.get(x - 1, y + 1);
                if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
            }
            else if (x == 0) {
                auto temp = board.get(x + 1, y + 1);
                if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
            }
            else {
                auto temp = board.get(x - 1, y + 1);
                if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
                auto temp2 = board.get(x + 1, y + 1);
                if (temp2 != nullopt && temp2->color != color && temp2->type == E_PieceType::King) return true;
            }
            return false;
        }
        else {
            if (y == 0) return false;
            if (x == 8) {
                auto temp = board.get(x - 1, y - 1);
                if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
            }
            else if (x == 0) {
                auto temp = board.get(x + 1, y - 1);
                if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
            }
            else {
                auto temp = board.get(x - 1, y - 1);
                if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
                auto temp2 = board.get(x + 1, y - 1);
                if (temp2 != nullopt && temp2->color != color && temp2->type == E_PieceType::King) return true;
            }
            return false;
        }
    }
    if (type == E_PieceType::Knight) {
        if (x - 1 >= 1 && y + 2 <= 8) {
            auto temp = board.get(x - 1, y + 2);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x + 1 <= 8 && y + 2 <= 8) {
            auto temp = board.get(x + 1, y + 2);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x + 2 <= 8 && y + 1 <= 8) {
            auto temp = board.get(x + 2, y + 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x + 2 <= 8 && y - 1 >= 1) {
            auto temp = board.get(x + 2, y - 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x + 1 <= 8 && y - 2 >= 1) {
            auto temp = board.get(x + 1, y - 2);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x - 1 >= 1 && y - 2 >= 1) {
            auto temp = board.get(x - 1, y - 2);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x - 2 >= 1 && y - 1 >= 1) {
            auto temp = board.get(x - 2, y - 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x - 2 >= 1 && y + 1 <= 8) {
            auto temp = board.get(x - 2, y + 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
    }
    if (type == E_PieceType::Bishop || type == E_PieceType::Queen) {
        for (i32 i = x + 1; i <= 8; i++) {
            for (i32 j = y + 1; j <= 8; j++) {
                auto temp = board.get(x, y);
                if (temp != nullopt && temp->color != color) {
                    if (temp->type == E_PieceType::King) {
                        return true;
                    } else {
                        goto the_A;
                    }
                }
            }
        }
    the_A:
        for (i32 i = x - 1; i >= 1; i--) {
            for (i32 j = y - 1; j >= 1; j--) {
                auto temp = board.get(x, y);
                if (temp != nullopt && temp->color != color) {
                    if (temp->type == E_PieceType::King) {
                        return true;
                    } else {
                        goto the_B;
                    }
                }
            }
        }
    the_B:
        for (i32 i = x + 1; i <= 8; i++) {
            for (i32 j = y - 1; j >= 1; j--) {
                auto temp = board.get(x, y);
                if (temp != nullopt && temp->color != color) {
                    if (temp->type == E_PieceType::King) {
                        return true;
                    } else {
                        goto the_C;
                    }
                }
            }
        }
    the_C:
        for (i32 i = x - 1; i >= 1; i--) {
            for (i32 j = y + 1; j <= 8; j++) {
                auto temp = board.get(x, y);
                if (temp != nullopt && temp->color != color) {
                    if (temp->type == E_PieceType::King) {
                        return true;
                    } else {
                        goto end;
                    }
                }
            }
        }
    end:;
    }
    if (type == E_PieceType::King) {
        if (x + 1 <= 8) {
            auto temp = board.get(x + 1, y);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (y + 1 <= 8) {
            auto temp = board.get(x, y + 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x + 1 <= 8 && y + 1 <= 8) {
            auto temp = board.get(x + 1, y + 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x + 1 <= 8 && y - 1 >= 1) {
            auto temp = board.get(x + 1, y - 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x - 1 >= 1) {
            auto temp = board.get(x - 1, y);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (y - 1 >= 1) {
            auto temp = board.get(x, y - 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x - 1 >= 1 && y - 1 >= 1) {
            auto temp = board.get(x - 1, y - 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        if (x - 1 >= 1 && y + 1 <= 8) {
            auto temp = board.get(x - 1, y + 1);
            if (temp != nullopt && temp->color != color && temp->type == E_PieceType::King) return true;
        }
        return false;
    }
    if (type == E_PieceType::Rook || type == E_PieceType::Queen) {
        for (i32 i = x + 1; i <= 8; i++) {
            auto temp = board.get(i, y);
            if (temp != nullopt && temp->color != color) {
                if (temp->type == E_PieceType::King) {
                    return true;
                }
                else {
                    break;
                }
            }
        }

        for (i32 i = x - 1; i >= 1; i--) {
            auto temp = board.get(i, y);
            if (temp != nullopt && temp->color != color) {
                if (temp->type == E_PieceType::King) {
                    return true;
                }
                else {
                    break;
                }
            }
        }

        for (i32 j = y + 1; y <= 8; y++) {
            auto temp = board.get(x, j);
            if (temp != nullopt && temp->color != color) {
                if (temp->type == E_PieceType::King) {
                    return true;
                }
                else {
                    break;
                }
            }
        }

        for (i32 j = y - 1; y >= 1; y--) {
            auto temp = board.get(x, j);
            if (temp != nullopt && temp->color != color) {
                if (temp->type == E_PieceType::King) {
                    return true;
                }
                else {
                    break;
                }
            }
        }
    }
    return false;
}


RawMove ComputerLv2::get_move() {
    i32 x1 = 0;
    i32 y1 = 0;
    i32 check_index = 0;
    std::mt19937 mt1(time(nullptr));
    E_Color temp = get_status().get_turn();
    bool have_captures = false;
    bool have_check = false;
    std::unique_ptr<PossibleMovement> up_moves = nullptr;
    for (i32 i = 1; i <= 8; i++) {
        for (i32 j = 1; j <= 8; j++) {
            if (get_board().get(i, j) && get_board().get(i, j)->color == temp) {
                up_moves = get_factory().get_move(BoardCoor(i, j));
                if (up_moves->unmoveable()) continue;
                if (up_moves->captures.size() != 0) {
                    have_captures = true;
                    for (i32 s = 0; s < up_moves->captures.size(); s++) {
                        if (if_capture(get_board(), up_moves->captures[s].coor.x, up_moves->captures[s].coor.y, temp, get_board().get(i, j)->type)) {
                            have_check = true;
                            check_index = s;
                        }
                    }
                    if (have_check) {
                        x1 = i;
                        y1 = j;
                        goto end;
                    }
                }
            }
        }
    }
end:
    if (!have_captures) {
        bool found = false;
        while (!found) {
            for (i32 i = mt1() % 8 + 1; i <= 8; i++) {
                for (i32 j = mt1() % 8 + 1; j <= 8; j++) {
                    if (get_board().get(i, j) && get_board().get(i, j)->color == temp) {
                        up_moves = get_factory().get_move(BoardCoor(i, j));
                        if (up_moves->unmoveable()) continue;
                        x1 = i;
                        y1 = j;
                        found = true;
                        goto end1;
                    }
                }
            }
        }
    }

end1:
    if (!have_check) {
        for (i32 i = 1; i <= 8; i++) {
            for (i32 j = 1; j <= 8; j++) {
                if (get_board().get(i, j) && get_board().get(i, j)->color == temp) {
                    up_moves = get_factory().get_move(BoardCoor(i, j));
                    if (up_moves->unmoveable()) continue;
                    if (up_moves->captures.size() != 0) {
                        x1 = i;
                        y1 = j;
                        goto end2;
                    }
                }
            }
        }
    }
end2:
    BoardCoor coor1 = BoardCoor(x1, y1);
    BoardCoor coor2 = BoardCoor(0, 0);
    i32 x2 = 0;
    i32 y2 = 0;
    up_moves = get_factory().get_move(coor1);
    std::optional<E_PieceType> promo = nullopt;
    if (have_check) {
        x2 = up_moves->captures[check_index].coor.x;
        y2 = up_moves->captures[check_index].coor.y;
        if (up_moves->captures[check_index].unique_action == E_UniqueAction::Promote) promo = E_PieceType::Queen;
        coor2.x = x2;
        coor2.y = y2;
    }
    else if (have_captures) {
        x2 = up_moves->captures[0].coor.x;
        y2 = up_moves->captures[0].coor.y;
        if (up_moves->captures[0].unique_action == E_UniqueAction::Promote) promo = E_PieceType::Queen;
        coor2.x = x2;
        coor2.y = y2;
    }
    else {
        x2 = up_moves->moves[0].coor.x;
        y2 = up_moves->moves[0].coor.y;
        if (up_moves->moves[0].unique_action == E_UniqueAction::Promote) promo = E_PieceType::Queen;
        coor2.x = x2;
        coor2.y = y2;
    }
    return RawMove(coor1, coor2, promo);
}
