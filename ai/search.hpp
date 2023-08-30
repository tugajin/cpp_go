#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include <climits>
#include "game.hpp"
#include "common.hpp"
#include "movelegal.hpp"

namespace search {

constexpr int SEARCH_MATE = 10000;
constexpr int SEARCH_MAX  = 20000;
constexpr int SEARCH_MIN  = -SEARCH_MAX;

int search(game::Position &pos, int alpha, int beta, int depth, uint64& node_num);

Move search_root(game::Position &pos, int depth, int &best_sc) {
    ASSERT(pos.is_ok());
    auto best_move = MOVE_PASS;
    auto best_score = -SEARCH_MATE;
    auto alpha = SEARCH_MIN;
    auto beta = SEARCH_MAX;
    movelist::MoveList ml;
    uint64 node_num = 0ull;
    gen::legal_moves(pos,ml);
    auto i = 0;
    for (const auto m : ml) {
        auto next_pos = pos.next(m);
        Tee<<i++<<"("<<ml.len()<<"):"<<move_str(m)<<":"<<node_num<<std::endl;
        const auto score = -search(next_pos,-beta, -alpha, depth-1, node_num);
        if (score > best_score) {
            best_score = score;
            best_move = m;
            if (score > alpha) {
                alpha = score;
            }
        }
    }
    best_sc = best_score;
    Tee<<"node:"<<node_num<<std::endl;
    return best_move;
}

int search(game::Position &pos, int alpha, int beta, int depth, uint64 &node_num) {
    ASSERT2(pos.is_ok(),{
        Tee<<pos<<std::endl;
    });
    ASSERT(alpha < beta);
    node_num++;
    if (pos.is_done()) {
        // Tee<<"done";
        // Tee<<pos<<std::endl;
        return pos.count_score(pos.turn());
    }
    if (pos.is_rep()) {
        //Tee<<"loop\n";
        //Tee<<pos<<std::endl;
        return 0;
    }
    /*if (depth < 0) {
        Tee<<pos<<std::endl;
        return pos.count_score(pos.turn());
    }*/
    if (pos.ply() >= MAX_PLY-1) {
        // Tee<<pos<<std::endl;
        // auto test = game::Position();
        // for(auto i = 1; i < MAX_PLY; i++) {
        //     test = test.next(pos.record_[i].prev_move());
        //     Tee<<test<<std::endl;
        // }
        //return pos.count_score(pos.turn());
        //Tee<<"loop";
        return 0;
    }
    movelist::MoveList ml;
    gen::legal_moves<false>(pos, ml);
    auto best_score = SEARCH_MIN;
    for (const auto m : ml) {
        auto next_pos = pos.next(m);
        const auto score = -search(next_pos, -beta, -alpha, depth-1, node_num);
        if (score > best_score) {
            best_score = score;
            alpha = score;
            if (score >= beta) {
                return best_score;
            }
        }
    }
    if (best_score == SEARCH_MIN) {
        Tee<<"??"<<std::endl;
        Tee<<pos<<std::endl;
        return -SEARCH_MATE + 7;
    }
    return best_score;
}
void test_search() {
    {
        auto pos = game::Position();
        Tee<<pos<<std::endl;
        int score = 0;
        Tee<<move_str(search_root(pos,50,score))<<std::endl;
        Tee<<score<<std::endl;
    }
}

}
#endif