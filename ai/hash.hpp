#ifndef __HASH_HPP__
#define __HASH_HPP__
#include "game.hpp"

namespace hash {
extern Key g_hash_pos[COLOR_SIZE][SQUARE_SIZE];
extern Key g_hash_turn;
//https://ja.wikipedia.org/wiki/Xorshift
// static関数とかstatic変数を使ってやばそうだが、cppファイルは1コなので多分大丈夫。
static uint32 xor128() {
    static uint32_t x = 123456789;
	static uint32_t y = 362436069;
	static uint32_t z = 521288629;
	static uint32_t w = 88675123;
	uint32_t t;

	t = x ^ (x << 11);
	x = y; y = z; z = w;
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}
void init() {
    REP(col,COLOR_SIZE) {
        REP(sq, SQUARE_SIZE) {
            g_hash_pos[col][sq] = xor128();
        }
    }
    g_hash_turn = xor128();
}
Key hash_key(const game::Position &pos) {
    auto k = Key(0);
    if (pos.turn() == WHITE) { k^= g_hash_turn; }
    REP_FILE2(file) {
        REP_RANK2(rank) {
            const auto sq = to_sq(file, rank);
            if (pos.square(sq) != EMPTY) {
                k ^= g_hash_pos[pos.square(sq)][sq];
            }
        }
    }
    return k;
}

void test_hash() {
}
}
#endif