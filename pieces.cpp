#include "pieces.hpp"

E_Color operator !(E_Color color) {
    return static_cast<E_Color>(!to_underlying(color));
}


bool on_board(BoardCoor v) {
    return v.x >= 1 && v.x <= 8 && v.y >= 1 && v.y <= 8;
}

E_PieceType StringToPieceType(const std::string& piece) {
    if (piece == "P") return E_PieceType::Pawn;
    if (piece == "K") return E_PieceType::Knight;
    if (piece == "B") return E_PieceType::Bishop;
    if (piece == "K") return E_PieceType::King;
    if (piece == "R") return E_PieceType::Rook;
    if (piece == "Q") return E_PieceType::Queen;
}


char Piece::to_char() const {
    if (to_underlying(this->color))
        return std::toupper(to_underlying(this->type));
    else
        return std::tolower(to_underlying(this->type));
}


std::strong_ordering operator <=>(const Piece& lhs, const Piece& rhs) {
    if (lhs.color != rhs.color)
        return lhs.color <=> rhs.color;
    else
        return lhs.type <=> rhs.type;
}