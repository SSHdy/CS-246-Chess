#ifndef CS246CHESS_CODE_UTILS_INC
#define CS246CHESS_CODE_UTILS_INC
#endif


// symbols
#ifndef CODE_UTILS_SYMBOLS_NAMESPACES
#define CODE_UTILS_SYMBOLS_NAMESPACES
#define NAMESPACE_DDDELTA_START
#define NAMESPACE_DDDELTA_END
#endif // CODE_UTILS_SYMBOLS_NAMESPACES


#ifndef CODE_UTILS_SYMBOLS_NODISCARD
#define CODE_UTILS_SYMBOLS_NODISCARD
#define NODISCARD [[nodiscard]]
#endif // CODE_UTILS_SYMBOLS_NODISCARD


#ifndef CODE_UTILS_SYMBOLS_FORCEINLINE
#define CODE_UTILS_SYMBOLS_FORCEINLINE
#if defined __GNUC__ || defined __clang__
#define FORCEINLINE inline __attribute__((always_inline))
#elif defined _MSC_VER
#define FORCEINLINE __forceinline
#endif
#endif // CODE_UTILS_SYMBOLS_FORCEINLINE


#if (defined _OPTIONAL_ || defined _GLIBCXX_OPTIONAL) && !defined CODE_UTILS_SYMBOLS_NULLOPT
#define CODE_UTILS_SYMBOLS_NULLOPT
using std::nullopt;
#endif // defined _GLIBCXX_OPTIONAL && defined CODE_UTILS_SYMBOLS_NULLOPT


// macro functions
#ifndef CODE_UTILS_FUNCTIONS_UNREACHABLE
#define CODE_UTILS_FUNCTIONS_UNREACHABLE
#if defined __GNUC__
#define UNREACHABLE() __builtin_unreachable()
#elif defined _MSC_VER
#define UNREACHABLE() __assume(false);
#endif
#endif // CODE_UTILS_FUNCTIONS_UNREACHABLE


#if !defined CODE_UTILS_FUNCTIONS_ASSERT_ON_BOARD && defined assert
#define CODE_UTILS_FUNCTIONS_ASSERT_ON_BOARD
#define assert_on_board_xy(x, y) (assert(on_board(BoardCoor(x, y))))
#define assert_on_board_coor(coor) (assert(on_board(coor)))
#endif // !defined CODE_UTILS_FUNCTIONS_ASSERT_ON_BOARD && defined assert


#if !defined CODE_UTILS_FUNCTIONS_LOG_TO_STDOUT && (defined _IOSTREAM_ || defined _GLIBCXX_IOSTREAM)
#define CODE_UTILS_FUNCTIONS_LOG_TO_STDOUT
#define LOG_TO_STDOUT(message) std::cout << message << std::endl
#endif // !defined CODE_UTILS_FUNCTIONS_LOG_TO_STDOUT && defined _GLIBCXX_IOSTREAM


#if (defined _TYPE_TRAITS_ || defined _GLIBCXX_TYPE_TRAITS) && __cplusplus <= 202002L && !defined CODE_UTILS_FUNCTIONS_TO_UNDERLYING
#define CODE_UTILS_FUNCTIONS_TO_UNDERLYING
template <class E>
inline constexpr std::underlying_type_t<E> to_underlying(E e) {
    return static_cast<std::underlying_type_t<E>>(e);
}
#endif // defined __cplusplus <= 202002L && !defined CODE_UTILS_FUNCTIONS_TO_UNDERLYING


// types
NAMESPACE_DDDELTA_START
#ifndef CODE_UTILS_TYPES_INTDEF
#define CODE_UTILS_TYPES_INTDEF
NAMESPACE_DDDELTA_END
#include <cstdint>
NAMESPACE_DDDELTA_START
using u8 = std::uint8_t;
using i8 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
#define USING_DDDELTA_INTDEF \
using DDDelta::u8, DDDelta::i8, DDDelta::u16, DDDelta::i16, DDDelta::u32, DDDelta::i32, DDDelta::u64, DDDelta::i64;
#endif // defined _GLIBCXX_CSTDINT && !defined CODE_UTILS_TYPES_INTDEF


#ifndef CODE_UTILS_TYPES_FLOATDEF
#define CODE_UTILS_TYPES_FLOATDEF
NAMESPACE_DDDELTA_END
#include <cfloat>
NAMESPACE_DDDELTA_START
using f32 = float;
using f64 = double;
using f128 = long double;
#define USING_DDDELTA_FLOATDEF \
using DDDelta::f32, DDDelta::f64, DDDelta::f128;
#endif // defined _GLIBCXX_CFLOAT && !defined CODE_UTILS_TYPES_FLOATDEF
NAMESPACE_DDDELTA_END


#if (defined _CHRONO_ || defined _GLIBCXX_CHRONO) && !defined CODE_UTILS_TYPES_STDCHR
#define CODE_UTILS_TYPES_STDCHR
namespace stdchr = std::chrono;
#endif // defined _GLIBCXX_CHRONO && !defined CODE_UTILS_TYPES_STDCHR


#if (defined _ALGORITHM_ || defined _RANGES_ || defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES) && !defined CODE_UTILS_TYPES_STDRNG
#define CODE_UTILS_TYPES_STDRNG
namespace stdrng = std::ranges;
#endif // (defined _RANGES_ALGO_H || defined _GLIBCXX_RANGES) && !defined CODE_UTILS_TYPES_STDRNG


#if (defined _RANGES_ || defined _GLIBCXX_RANGES) && !defined CODE_UTILS_TYPES_STDVWS
#define CODE_UTILS_TYPES_STDVWS
namespace stdvws = std::views;
#endif // defined _GLIBCXX_RANGES && !defined CODE_UTILS_TYPES_STDVWS
