#include "board_operator.hpp"

#define DBG(x) x

bool StandardBoardOperator::execute_move(
	BoardCoor selection, 
	PieceMove piece_move, 
	std::optional<E_PieceType> pro_type) {

    DBG(std::cout << __FUNCTION__ << " called" << std::endl);

    Piece moving_piece = *this->p_board->get(selection);
    E_Color color = moving_piece.color;
    E_UniqueAction action = piece_move.unique_action;
    BoardCoor target = piece_move.coor;

    this->p_board->set(target, moving_piece);
    switch (action) {
    case E_UniqueAction::ShortCastle:
        this->p_board->set(selection + Vec2(1, 0), Piece(color, E_PieceType::Rook));
        this->p_board->reset(selection + Vec2(3, 0));
        break;
    case E_UniqueAction::LongCastle:
        this->p_board->set(selection - Vec2(1, 0), Piece(color, E_PieceType::Rook));
        this->p_board->reset(selection - Vec2(4, 0));
        break;
    case E_UniqueAction::EnPassant:
        this->p_board->reset(target.x, selection.y);
        break;
    case E_UniqueAction::Promote:
        this->p_board->set(target, Piece(color, *pro_type));
        break;
    case E_UniqueAction::None:
    case E_UniqueAction::DoublePawnPush:
    default:
        break;
    }

    this->p_board->reset(selection);

    DBG(std::cout << "success" << std::endl);

    return true;
}