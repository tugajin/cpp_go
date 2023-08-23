#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include "util.hpp"


typedef std::vector<std::vector<int>> Feature;
typedef double NNScore;
typedef uint64 Key;

static constexpr int MAX_PLY = 128;

enum Move : int {
    MOVE_PASS = -1,
};
enum Color : int {
    EMPTY = 0, 
    BLACK = 1, 
    WHITE = 2, 
    WALL = 3,
    COLOR_SIZE = 3,
    EM = EMPTY,
    BL = BLACK,
    WH = WHITE,
};
enum File : int {
    FILE_A = 1,
    FILE_B = 2,
    FILE_C = 3,
    FILE_D = 4,
    FILE_E = 5,
    FILE_F = 6,
    FILE_G = 7,
    FILE_H = 8,
    FILE_I = 9,
    FILE_J = 10,
    FILE_K = 11,
    FILE_L = 12,
    FILE_M = 13,
    FILE_N = 14,
    FILE_O = 15,
    FILE_P = 16,
    FILE_Q = 17,
    FILE_R = 18,
    FILE_S = 19,
    FILE_SIZE = FILE_D + 2,
};
enum Rank : int {
    RANK_1 = 1,
    RANK_2 = 2,
    RANK_3 = 3,
    RANK_4 = 4,
    RANK_5 = 5,
    RANK_6 = 6,
    RANK_7 = 7,
    RANK_8 = 8,
    RANK_9 = 9,
    RANK_10 = 10,
    RANK_11 = 11,
    RANK_12 = 12,
    RANK_13 = 13,
    RANK_14 = 14,
    RANK_15 = 15,
    RANK_16 = 16,
    RANK_17 = 17,
    RANK_18 = 18,
    RANK_19 = 19,
    RANK_SIZE = RANK_4 + 2,
};

enum Square : int {
    SQUARE_SIZE = FILE_SIZE * RANK_SIZE,
    DIR_UP = -FILE_SIZE,
    DIR_LEFT  = -1,
    DIR_RIGHT = +1,
    DIR_DOWN = FILE_SIZE, 
    SQUARE_NONE = 0,
};

#define OverloadEnumOperators(T)                                        \
    inline T& operator += (T& lhs, const int rhs) { return lhs  = static_cast<T>(static_cast<int>(lhs) + rhs); } \
    inline T& operator += (T& lhs, const T   rhs) { return lhs += static_cast<int>(rhs); } \
    inline T& operator -= (T& lhs, const int rhs) { return lhs  = static_cast<T>(static_cast<int>(lhs) - rhs); } \
    inline T& operator -= (T& lhs, const T   rhs) { return lhs -= static_cast<int>(rhs); } \
    inline T& operator *= (T& lhs, const int rhs) { return lhs  = static_cast<T>(static_cast<int>(lhs) * rhs); } \
    inline T& operator /= (T& lhs, const int rhs) { return lhs  = static_cast<T>(static_cast<int>(lhs) / rhs); } \
    inline T& operator |= (T& lhs, const int rhs) { return lhs  = static_cast<T>(static_cast<int>(lhs) | rhs); } \
    inline constexpr T operator + (const T   lhs, const int rhs) { return static_cast<T>(static_cast<int>(lhs) + rhs); } \
    inline constexpr T operator + (const T   lhs, const T   rhs) { return lhs + static_cast<int>(rhs); } \
    inline constexpr T operator - (const T   lhs, const int rhs) { return static_cast<T>(static_cast<int>(lhs) - rhs); } \
    inline constexpr T operator - (const T   lhs, const T   rhs) { return lhs - static_cast<int>(rhs); } \
    inline constexpr T operator * (const T   lhs, const int rhs) { return static_cast<T>(static_cast<int>(lhs) * rhs); } \
    inline constexpr T operator * (const int lhs, const T   rhs) { return rhs * lhs; } \
    inline constexpr T operator * (const T   lhs, const T   rhs) { return lhs * static_cast<int>(rhs); } \
    inline constexpr T operator / (const T   lhs, const int rhs) { return static_cast<T>(static_cast<int>(lhs) / rhs); } \
    inline constexpr int operator / (const T   lhs, const T rhs) { return static_cast<int>(lhs) / static_cast<int>(rhs); } \
    inline constexpr T operator | (const T   lhs, const int rhs) { return static_cast<T>(static_cast<int>(lhs) | rhs); } \
    inline constexpr T operator - (const T   rhs) { return static_cast<T>(-static_cast<int>(rhs)); } \
    inline T operator ++ (T& lhs) { lhs += 1; return lhs; } /* 前置 */  \
    inline T operator -- (T& lhs) { lhs -= 1; return lhs; } /* 前置 */  \
    inline T operator ++ (T& lhs, int) { const T temp = lhs; lhs += 1; return temp; } /* 後置 */ \
    /* inline T operator -- (T& lhs, int) { const T temp = lhs; lhs -= 1; return temp; } */ /* 後置 */

OverloadEnumOperators(File);
OverloadEnumOperators(Rank);

enum RemoveStoneFlag : int {
    REMOVE_NONE = 0,
    REMOVE_UP = 1 << 0,
    REMOVE_LEFT = 1 << 1,
    REMOVE_DOWN = 1 << 2,
    REMOVE_RIGHT = 1 << 3,
};
OverloadEnumOperators(RemoveStoneFlag);

static constexpr inline Square DIR4[4] = { DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT };
static constexpr inline RemoveStoneFlag REMOVE_FLAG4[4] = { REMOVE_UP, REMOVE_LEFT, REMOVE_DOWN, REMOVE_RIGHT };

#define REP(i,e) for (auto i = 0; i < (e); ++i)
#define REP_POS(i) for (auto i = 0; i < SQUARE_SIZE; ++i)
#define REP_FILE(f) for (File f = File(0); f < FILE_SIZE; ++f)
#define REP_RANK(r) for (Rank r = Rank(0); r < RANK_SIZE; ++r)

#define REP_FILE2(f) for (File f = FILE_A; f < static_cast<int>(FILE_SIZE)-1; ++f)
#define REP_RANK2(r) for (Rank r = RANK_1; r < static_cast<int>(RANK_SIZE)-1; ++r)

std::string color_str(const Color c) {
    if (c == BLACK) {
        return "BLACK";
    } else if (c == WHITE) {
        return "WHITE";
    } else if (c == EMPTY) {
        return "EMPTY";
    } else if (c == WALL) {
        return "WALL";
    } else {
        return "ERROR:" + to_string(static_cast<int>(c));
    }
}
std::ostream& operator<<(std::ostream& os, const Color c) {
    os << color_str(c);
    return os;
}
inline Color change_turn(const Color turn) {
    return static_cast<Color>(3 - static_cast<int>(turn));
}
inline Rank sq_to_rank(const Square sq) {
    return static_cast<Rank>(static_cast<int>(sq) / static_cast<int>(RANK_SIZE));
}
inline File sq_to_file(const Square sq) {
    return static_cast<File>(static_cast<int>(sq) % static_cast<int>(FILE_SIZE));
}
inline Square to_sq(const File f, const Rank r) {
    return static_cast<Square>(static_cast<int>(f) + static_cast<int>(r) * static_cast<int>(RANK_SIZE));
}
inline Square move_to_sq(const Move mv) {
    return static_cast<Square>( static_cast<int>(mv) & 0xfff );
}
inline RemoveStoneFlag move_to_flag(const Move mv) {
    return static_cast<RemoveStoneFlag>(static_cast<int>(mv) >> 12);
}
inline Rank move_to_rank(const Move m) {
    return sq_to_rank(move_to_sq(m));
}
inline File move_to_file(const Move m) {
    return sq_to_file(move_to_sq(m));
}
inline Move make_move(const int flag, const Square sq) {
    return static_cast<Move>((flag << 12) | static_cast<int>(sq) );
}
inline Move make_move(const int flag, const File f, const Rank r) {
    return make_move(flag,to_sq(f,r));
}

// 壁は含まずOKかどうか？
inline bool move_is_ok(const Move m) {
    const auto f = move_to_file(m);
    const auto r = move_to_rank(m);
    if (f < FILE_A || f >= FILE_SIZE) {
        return false;
    }
    if (r < RANK_1 || r >= RANK_SIZE) {
        return false;
    }
    return true;
}
// 壁も含めOKかどうか？
inline bool sq_is_ok(const Square sq) {
    const auto v = static_cast<int>(sq);
    return (v >= 0 && v < SQUARE_SIZE);
}

NNScore to_nnscore(const float sc) {
    auto score = static_cast<int>(sc * 10000);
    if (score >= 10000) {
        score = 9999;
    } else if (score <= -10000) {
        score = -9999;
    }
    return static_cast<NNScore>(static_cast<double>(score) / 10000.0);
}

inline std::string file_str(const File f) {
    std::string str[] = { "WALL_F", "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S" };
    return str[f];
}
inline std::string rank_str(const Rank r) {
    std::string str[] = { "WALL_R", "1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20" };
    return str[r];
}
inline std::string move_str(const Move m) {
    if (m == MOVE_PASS) {
        return "PASS";
    }
    const auto sq = move_to_sq(m);
    const auto flag = move_to_flag(m);
    return file_str(sq_to_file(sq)) + rank_str(sq_to_rank(sq))+ ":" + to_string(static_cast<int>(flag));
}
void check_mode() {
#if DEBUG
    Tee<<"debug mode\n";
#elif NDEBUG
    Tee<<"release mode\n";
#else
    Tee<<"unknown mode\n";
#endif
}

#endif