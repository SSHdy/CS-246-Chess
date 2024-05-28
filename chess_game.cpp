#include "chess_game.hpp"
#include "code_utils.inc"

#define DBG(x) x

ChessGame::ChessGame() : 
_up_operator(nullptr), _up_factory(nullptr) {}


static E_Result wins(E_Color curr_turn) {
    return to_underlying(curr_turn) ? E_Result::WhiteWin : E_Result::BlackWin;
}


bool ChessGame::execute_move(
    BoardCoor from_coor, 
    BoardCoor target_coor, 
    std::optional<E_PieceType> promotion_type
) {
    assert(on_board(target_coor) && on_board(from_coor));
    
    DBG(std::cout << __FUNCTION__ << " called" << std::endl);

    if (!this->_board.get(from_coor)) {
        DBG(std::cout << "selected empty" << std::endl);
        return false;
    }

    E_Color curr_turn = this->_status.get_turn();
    E_Result& res = this->_status.res;

    // resigns
    if (!from_coor && !target_coor) {
        res = wins(!curr_turn);
        return true;
    }

    std::unique_ptr<PossibleMovement> up_moves = this->_up_factory->get_move(from_coor);
    auto has_move = [target_coor](PieceMove pm) { return pm.coor == target_coor; };
    auto moves = { &up_moves->moves, &up_moves->captures };
    auto all_legal_move_rng = moves
        | stdvws::transform([](auto* p) -> const auto& { return *p; })
        | stdvws::join;
    auto it_move = stdrng::find_if(all_legal_move_rng, has_move);

    // if illegal move
    if (it_move == all_legal_move_rng.end()) {
        DBG(std::cout << target_coor.x << target_coor.y << "is not a legal selection" << std::endl);
        return false;
    }

    this->_up_operator->execute_move(from_coor, *it_move, promotion_type);

    Piece moving_piece = *this->get_piece(target_coor);
    switch (moving_piece.type) {
    case E_PieceType::King:
        this->_status.can_castle_short[curr_turn] = false;
        this->_status.can_castle_long[curr_turn] = false;
        break;
    case E_PieceType::Rook:
        if (target_coor == BoardCoor(1, nth_from_last_rank<1>[curr_turn]))
            this->_status.can_castle_long[curr_turn] = false;
        else if (target_coor == BoardCoor(8, nth_from_last_rank<1>[curr_turn]))
            this->_status.can_castle_short[curr_turn] = false;
        break;
    default:
        break;
    }

    if (it_move->unique_action == E_UniqueAction::DoublePawnPush)
        this->_status.last_double_pawn_move = it_move->coor;

    if (this->_up_factory->is_mated())
        res = wins(curr_turn);
    else if (this->_up_factory->is_drawn())
        res = E_Result::Draw;

    this->_status.next_turn({ from_coor, target_coor, promotion_type });

    return true;
}

void ChessGame::attach(BoardObserver* observer) {
    this->_board.attach(observer);
}

/*
bool ChessGame::execute_move_promote(BoardCoor from_coor, BoardCoor target_coor, E_PieceType promotion_type) {
    assert(on_board(target_coor));

    if (!this->_board.get(from_coor))
        return false;

    if (this->_board.get(from_coor)->type != E_PieceType::Pawn)
        return false;

    E_Color curr_turn = this->_status.turn;
    E_Result* p_res = &this->_status.res;

    std::unique_ptr<PossibleMovement> up_moves = this->_up_factory->get_move(from_coor);
    auto has_move = [target_coor](PieceMove pm) { return pm.coor == target_coor; };
    auto moves = { &up_moves->moves, &up_moves->captures };
    auto filtered_move_rng = moves 
        | stdvws::transform([](auto* p) -> const auto& { return *p; })
        | stdvws::join
        | stdvws::filter([](PieceMove pm) { return pm.unique_action == E_UniqueAction::Promote; });
    auto it_move = stdrng::find_if(filtered_move_rng, has_move);

    if (it_move != filtered_move_rng.end())
        return false;

    this->_up_operator->execute_move_promote(from_coor, *it_move, promotion_type);

    if (this->_up_factory->is_mated())
        *p_res = wins(curr_turn);
    else if (this->_up_factory->is_drawn())
        *p_res = E_Result::Draw;

    this->_status.next_turn();

    return true;
}
*/
NAMESPACE_DDDELTA_END