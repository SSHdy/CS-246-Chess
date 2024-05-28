#pragma once
#define CS246CHESS_PIECES_HPP

#include <cstdint>
#include <string>
#include <compare>
#include <optional>
#include <array>

#include "code_utils.inc"


NAMESPACE_DDDELTA_START
struct Vec2 {
    i32 x;
    i32 y;

    bool operator ==(const Vec2& rhs) const = default;
    Vec2 operator +(Vec2 rhs) const { return { x + rhs.x, y + rhs.y }; };
    Vec2 operator -() const { return { -x, -y }; }
    Vec2 operator -(Vec2 rhs) const { return *this + (-rhs); }
    Vec2& operator +=(Vec2 rhs) { this->x += rhs.x, this->y += rhs.y; return *this; }
    Vec2& operator -=(Vec2 rhs) { rhs = -rhs; return *this += rhs; }
    explicit operator bool() const { return this->x || this->y; }
};


using BoardCoor = Vec2;


bool on_board(BoardCoor v);


enum class E_Color : bool {
    White = true,
    Black = false
};


E_Color operator !(E_Color color);


enum class E_PieceType : u8 {
    Pawn = 'P',
    Knight = 'N',
    Bishop = 'B',
    King = 'K',
    Rook = 'R',
    Queen = 'Q'
};


E_PieceType StringToPieceType(const std::string& piece);


struct Piece {
    E_Color color;
    E_PieceType type;

    bool operator ==(const Piece&) const = default;

    NODISCARD char to_char() const;
};


using OptPiece = std::optional<Piece>;
using RawBoard = std::array<std::array<OptPiece, 8>, 8>;


// used for std::map
std::strong_ordering operator <=>(const Piece& lhs, const Piece& rhs);


namespace constant {
inline constexpr BoardCoor INVALID_COOR { 0, 0 };
inline constexpr Piece WhiteKing   { E_Color::White, E_PieceType::King };
inline constexpr Piece WhiteQueen  { E_Color::White, E_PieceType::Queen };
inline constexpr Piece WhiteRook   { E_Color::White, E_PieceType::Rook };
inline constexpr Piece WhiteBishop { E_Color::White, E_PieceType::Bishop };
inline constexpr Piece WhiteKnight { E_Color::White, E_PieceType::Knight };
inline constexpr Piece WhitePawn   { E_Color::White, E_PieceType::Pawn };
inline constexpr Piece BlackKing   { E_Color::Black, E_PieceType::King };
inline constexpr Piece BlackQueen  { E_Color::Black, E_PieceType::Queen };
inline constexpr Piece BlackRook   { E_Color::Black, E_PieceType::Rook };
inline constexpr Piece BlackBishop { E_Color::Black, E_PieceType::Bishop };
inline constexpr Piece BlackKnight { E_Color::Black, E_PieceType::Knight };
inline constexpr Piece BlackPawn   { E_Color::Black, E_PieceType::Pawn };
}
NAMESPACE_DDDELTA_END
