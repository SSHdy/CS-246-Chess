#include "move_factory.hpp"
#include "code_utils.inc"

#define DBG(x) x

MoveFactoryBase::MoveFactoryBase(const Board* p_bd, const GameStatus* p_status) : p_board(p_bd), p_status(p_status) {}


static std::array<BoardCoor, 8> all_king_movement(BoardCoor co) {
    return {
        co + Vec2(1, 0),  co + Vec2(1, 1),  co + Vec2(1, -1),
        co + Vec2(0, 1),                    co + Vec2(0, -1),
        co + Vec2(-1, 0), co + Vec2(-1, 1), co + Vec2(-1, -1)
    };
}


static std::array<BoardCoor, 8> all_knight_movement(BoardCoor co) {
    return {
        co + Vec2(-1, 2),  co + Vec2(1, 2),
        co + Vec2(-2, 1),  co + Vec2(2, 1),
        co + Vec2(-2, -1), co + Vec2(2, -1),
        co + Vec2(-1, -2), co + Vec2(1, -2)
    };
}

static constexpr auto adap_remove_nullopt = stdvws::filter([](OptPiece pc) {
    return pc != nullopt;
});

static constexpr auto adap_to_piecemove = stdvws::transform([](BoardCoor coor) -> PieceMove {
    return { coor, E_UniqueAction::None };
});


static constexpr auto adap_remove_off_board = stdvws::filter([](BoardCoor coor) {
    return on_board(coor);
});


static auto adap_remove_empty(const Board* self) {
    return stdvws::filter([self](BoardCoor coor) {
        return static_cast<bool>(self->get(coor));
    });
}


static auto adap_remove_pieces(const Board* self) {
    return stdvws::filter([self](BoardCoor coor) {
        return !self->get(coor);
    });
}


static auto adap_remove_color_of(const Board* const self, E_Color color) {
    return stdvws::filter([self, color](BoardCoor coor) {
        return !(self->get(coor) && self->get(coor)->color == color);
    });
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::_knight_move(BoardCoor co) const {
    assert(!this->_empty(co));

    auto all_move = all_knight_movement(co);
    auto valid_move = all_move | adap_remove_off_board;

    auto move_range = valid_move
        | adap_remove_pieces(this->p_board)
        | adap_to_piecemove;
    auto capture_range = valid_move
        | adap_remove_empty(this->p_board)
        | adap_remove_color_of(this->p_board, this->_get(co)->color)
        | adap_to_piecemove;
    auto protect_range = valid_move
        | adap_remove_empty(this->p_board)
        | adap_remove_color_of(this->p_board, !this->_get(co)->color)
        | adap_to_piecemove;

    return std::make_unique<PossibleMovement>(
        std::vector<PieceMove>(move_range.begin(), move_range.end()),
        std::vector<PieceMove>(capture_range.begin(), capture_range.end()),
        std::vector<PieceMove>(protect_range.begin(), protect_range.end())
    );
}


void StandardMoveFactory::_diagonal_move(PossibleMovement* p_movement, BoardCoor co) const {
    assert(!this->_empty(co));

    E_Color curr_color = this->_get(co)->color;

    auto insert_move = [this, p_movement, curr_color](BoardCoor co) mutable {
        if (!on_board(co)) {
            return false;
        } else if (this->_empty(co)) {
            p_movement->moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else if (this->_get(co)->color == curr_color) {
            p_movement->protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else {
            p_movement->captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
    curr = co;    while (insert_move(curr += Vec2(1, 1)));
    curr = co;    while (insert_move(curr += Vec2(1, -1)));
    curr = co;    while (insert_move(curr += Vec2(-1, -1)));
    curr = co;    while (insert_move(curr += Vec2(-1, 1)));
}


void StandardMoveFactory::_linear_move(PossibleMovement* p_movement, BoardCoor co) const {
    assert(!this->_empty(co));

    E_Color curr_color = this->_get(co)->color;

    auto insert_move = [this, p_movement, curr_color](BoardCoor co) mutable {
        if (!on_board(co)) {
            return false;
        } else if (this->_empty(co)) {
            p_movement->moves.emplace_back(co, E_UniqueAction::None);
            return true;
        } else if (this->_get(co)->color == curr_color) {
            p_movement->protects.emplace_back(co, E_UniqueAction::None);
            return false;
        } else {
            p_movement->captures.emplace_back(co, E_UniqueAction::None);
            return false;
        }
    };

    BoardCoor
    curr = co;    while (insert_move(curr += Vec2(1, 0)));
    curr = co;    while (insert_move(curr += Vec2(-1, 0)));
    curr = co;    while (insert_move(curr += Vec2(0, 1)));
    curr = co;    while (insert_move(curr += Vec2(0, -1)));
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::_bishop_move(BoardCoor co) const {
    assert(!this->_empty(co));

    auto p_ret = std::make_unique<PossibleMovement>();
    this->_diagonal_move(p_ret.get(), co);
    
    return p_ret;
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::_rook_move(BoardCoor co) const {
    assert(!this->_empty(co));

    auto p_ret = std::make_unique<PossibleMovement>();
    this->_linear_move(p_ret.get(), co);

    return p_ret;
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::_queen_move(BoardCoor co) const {
    assert(!this->_empty(co));

    auto p_ret = std::make_unique<PossibleMovement>();
    this->_linear_move(p_ret.get(), co);
    this->_diagonal_move(p_ret.get(), co);

    return p_ret;
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::_pawn_move(BoardCoor co) const {
    assert(!this->_empty(co));
    assert(co.y != 1 && co.y != 8);

    E_Color selected_color = this->_get(co)->color;
    i32 direction = to_underlying(selected_color) ? 1 : -1;
    i32 promotion_rank = nth_from_last_rank<8>[selected_color];
    i32 initial_rank = nth_from_last_rank<2>[selected_color];
    i32 en_passant_rank = nth_from_last_rank<5>[selected_color];

    std::array<BoardCoor, 2> diagonal {
        BoardCoor(co.x + 1, co.y + direction),
        BoardCoor(co.x - 1, co.y + direction)
    };

    auto p_ret = std::make_unique<PossibleMovement>();

    for (BoardCoor coor : diagonal | adap_remove_off_board) {
        if (this->_empty(coor)) {
            if (co.y == en_passant_rank && this->p_status->last_double_pawn_move.x == coor.x)
                p_ret->captures.emplace_back(coor, E_UniqueAction::EnPassant);

            continue;
        }
        // get rid of case of empty / En Passant

        if (this->_get(coor)->color != selected_color)
            if (coor.y == promotion_rank) // capturing and promoting
                p_ret->captures.emplace_back(coor, E_UniqueAction::Promote);
            else // capturing and !promoting
                p_ret->captures.emplace_back(coor, E_UniqueAction::None);
        else // protecting
            p_ret->protects.emplace_back(coor, E_UniqueAction::None);
    }

    BoardCoor single_push = co + Vec2(0, direction);
    if (on_board(single_push) && this->_empty(single_push))
        p_ret->moves.emplace_back(single_push, E_UniqueAction::None);
    else
        return p_ret;

    BoardCoor double_push = co + Vec2(0, 2 * direction);
    if (co.y == initial_rank && this->_empty(double_push)) // on initial rank and no blocking
        p_ret->moves.emplace_back(double_push, E_UniqueAction::DoublePawnPush);

    return p_ret;
}


// TODO: finish this
std::unique_ptr<PossibleMovement> StandardMoveFactory::_king_move(BoardCoor co) const {
    assert(!this->_empty(co));

    E_Color color = this->_get(co)->color;
    auto all_move = all_king_movement(co);
    auto valid_move = all_move | adap_remove_off_board;

    auto move_range = valid_move
        | adap_remove_pieces(this->p_board)
        | adap_to_piecemove;
    auto capture_range = valid_move
        | adap_remove_empty(this->p_board)
        | adap_remove_color_of(this->p_board, this->_get(co)->color)
        | adap_to_piecemove;
    auto protect_range = valid_move
        | adap_remove_empty(this->p_board)
        | adap_remove_color_of(this->p_board, !this->_get(co)->color)
        | adap_to_piecemove;

    auto ret = std::make_unique<PossibleMovement>(
        std::vector<PieceMove>(move_range.begin(), move_range.end()),
        std::vector<PieceMove>(capture_range.begin(), capture_range.end()),
        std::vector<PieceMove>(protect_range.begin(), protect_range.end())
    );

    BoardCoor short_castle_target = co + Vec2(2, 0);
    if (this->p_status->can_castle_short[color] &&
        this->_empty(short_castle_target) &&
        this->_empty(co + Vec2(1, 0)))
        ret->moves.emplace_back(short_castle_target, E_UniqueAction::ShortCastle);

    BoardCoor long_castle_target = co - Vec2(2, 0);
    if (this->p_status->can_castle_long[color] &&
        this->_empty(co - Vec2(3, 0)) &&
        this->_empty(short_castle_target) &&
        this->_empty(co - Vec2(1, 0)))
        ret->moves.emplace_back(long_castle_target, E_UniqueAction::LongCastle);

    return ret;
}


std::pair<BoardCoor, BoardCoor> StandardMoveFactory::_in_check() const {
    E_Color color = this->_get_turn();
    BoardCoor king_pos = this->_find_king(this->_get_turn());
    auto has_move = [king_pos](PieceMove pm) { return pm.coor == king_pos; };
    BoardCoor attacker1 = constant::INVALID_COOR, attacker2 = constant::INVALID_COOR;

    /*
    for (i32 i : stdvws::iota(1, 9)) {
        for (i32 j : stdvws::iota(1, 9)) {
            auto opt = this->p_board->get(i, j);
            if (opt != nullopt && opt->color != color) {
                std::unique_ptr<PossibleMovement> up_moves = this->get_move({ i, j });
                auto it = stdrng::find_if(up_moves->captures, has_move);
                if (it != up_moves->captures.end())
                    if (attacker1)
                        attacker2 = it->coor;
                    else
                        attacker1 = it->coor;
            }
        }
    }
    */

    return { attacker1, attacker2 };
}


BoardCoor StandardMoveFactory::_find_king(E_Color color) const {
    for (i32 i : stdvws::iota(1, 9))
    for (i32 j : stdvws::iota(1, 9))
        if (auto opt = this->p_board->get(i, j); 
            opt != nullopt &&
            opt == Piece(color, E_PieceType::King))
            return { i, j };
}


void StandardMoveFactory::_filter_check_defence(PossibleMovement* p_pm) const {
    auto [attacker1, attacker2] = this->_in_check();
    BoardCoor king_pos = this->_find_king(this->_get_turn());

    auto has_move = [attacker1](PieceMove pm) { return pm.coor == attacker1; };

    if (!attacker1 && !attacker2)
        return;

    if (attacker1 && attacker2) {
        p_pm->captures.clear();
        p_pm->moves.clear();
        return;
    }

    switch (this->_get(attacker1)->type) {
        using enum E_PieceType;

    case King:
        UNREACHABLE();
        [[fallthrough]];
    case Knight:
    case Pawn:
        p_pm->moves.clear();
        break;

    {
    case Bishop:
    case Queen:
    case Rook:
        i32 diffx = attacker1.x - king_pos.x;
        i32 diffy = attacker1.y - king_pos.y;
        i32 dirx = 0;
        i32 diry = 0;

        if (diffx != 0)
            dirx = diffx / std::abs(diffx);
        if (diffy != 0)
            diry = diffy / std::abs(diffy);

        auto valid_blocking = stdvws::iota(1, std::abs(diffx))
            | stdvws::transform([dirx, diry, king_pos](i32 i) {
                return Vec2(dirx * i, diry * i) + king_pos;
            });
        std::vector<PieceMove> vec_move = p_pm->moves;
        auto blocking_moves = vec_move
            | stdvws::filter([&valid_blocking](PieceMove pm) {
                BoardCoor co = pm.coor;
                auto it = stdrng::find(valid_blocking, co);
                return it != valid_blocking.end();
            });

        p_pm->moves = std::vector<PieceMove>(blocking_moves.begin(), blocking_moves.end());

        break;
    }
    }

    auto it = stdrng::find_if(p_pm->captures, has_move);
    if (it != p_pm->captures.end()) {
        PieceMove move = *it;
        p_pm->captures.clear();
        p_pm->captures.push_back(move);
    }
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::get_move(BoardCoor coor) const {
    assert_on_board_coor(coor);

    DBG(std::cout << __FUNCTION__ << "selected " << coor.x << coor.y << std::endl);

    if (!this->_get(coor)) {
        DBG(std::cout << "selected empty" << std::endl);
        return nullptr;
    }

    std::unique_ptr<PossibleMovement> ret = nullptr;

    E_PieceType selected_type = this->_get(coor)->type;
    if (selected_type == E_PieceType::King) {
        return this->_king_move(coor);
    }

    ret = this->_get_move(coor);

    this->_filter_check_defence(ret.get());

    DBG(std::cout << "returned a move" << std::endl);
    DBG(std::cout << "underlying ptr " << ret.get() << std::endl);
    DBG(std::cout << "is empty?" << ret->unmoveable() << std::endl);

    return ret;
}


std::unique_ptr<PossibleMovement> StandardMoveFactory::_get_move(BoardCoor coor) const {
    E_PieceType selected_type = this->_get(coor)->type;
    switch (selected_type) {
    case E_PieceType::Pawn:
        return this->_pawn_move(coor);
    case E_PieceType::Knight:
        return this->_knight_move(coor);
    case E_PieceType::Bishop:
        return this->_bishop_move(coor);
    case E_PieceType::Rook:
        return this->_rook_move(coor);
    case E_PieceType::Queen:
        return this->_queen_move(coor);

    default:
        UNREACHABLE();
    }
}


bool StandardMoveFactory::is_mated() const {
    BoardCoor king_pos = this->_find_king(this->_get_turn());
    auto [a1, a2] = this->_in_check();
    std::unique_ptr<PossibleMovement> up_pm = this->get_move(king_pos);

    if (a1 && up_pm->unmoveable())
        return true;
    else
        return false;
}


bool StandardMoveFactory::is_drawn() const {
    using enum E_Color;

    E_Color curr_turn = this->_get_turn();

    std::vector<std::pair<RawMove, RawMove>> white_moves;
    std::vector<std::pair<RawMove, RawMove>> black_moves;
    std::size_t game_len = this->p_status->move_record.size();
    
    // threefold
    auto& rcd = this->p_status->move_record;
    if (this->p_status->move_record.size() >= 12) {
        white_moves.emplace_back(rcd[game_len - 0], rcd[game_len - 2]);
        white_moves.emplace_back(rcd[game_len - 4], rcd[game_len - 6]);
        white_moves.emplace_back(rcd[game_len - 8], rcd[game_len - 10]);
        black_moves.emplace_back(rcd[game_len - 1], rcd[game_len - 3]);
        black_moves.emplace_back(rcd[game_len - 5], rcd[game_len - 7]);
        black_moves.emplace_back(rcd[game_len - 9], rcd[game_len - 11]);

        bool w_same = stdrng::adjacent_find(white_moves, std::not_equal_to<std::pair<RawMove, RawMove>>()) == white_moves.end();
        bool b_same = stdrng::adjacent_find(black_moves, std::not_equal_to<std::pair<RawMove, RawMove>>()) == black_moves.end();

        if (w_same && b_same)
            return true;
    }

    // stalemate
    for (i32 i : stdvws::iota(1, 9)) {
        for (i32 j : stdvws::iota(1, 9)) {
            if (!this->_get({ i, j }))
                continue;
            if (this->_get({ i, j })->color != curr_turn)
                continue;
            if (!this->get_move({ i, j })->unmoveable())
                goto EndOfStalemateCheck;
        }
    }
    return true;

EndOfStalemateCheck:

    // insufficient material
    auto joined_board = this->p_board->board_range() | adap_remove_nullopt;
    i32 piece_count = stdrng::count_if(joined_board, [](OptPiece pc) { return true; });
    i32 minor_count = stdrng::count_if(joined_board, [](OptPiece pc) { 
        return pc->type == E_PieceType::Knight || pc->type == E_PieceType::Bishop;
    });

    if (piece_count == 3 && minor_count == 1)
        return true;

    return false;
}
