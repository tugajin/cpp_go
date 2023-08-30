#ifndef __NN_HPP__
#define __NN_HPP__
#include "game.hpp"
#include "common.hpp"
#include <vector>
namespace nn {
constexpr inline int FEAT_SIZE = 3;
typedef std::vector<std::vector<int>> Feature;
typedef double NNScore;

inline NNScore to_nnscore(const float sc) {
    auto score = static_cast<int>(sc * 10000);
    if (score >= 10000) {
        score = 9999;
    } else if (score <= -10000) {
        score = -9999;
    }
    return static_cast<NNScore>(static_cast<double>(score) / 10000.0);
}

Feature feature(const game::Position &pos) {
    Feature feat(FEAT_SIZE, std::vector<int>(POS_SIZE, 0));
    auto i = 0;
    REP_RANK2(r) {
        REP_FILE2(f) {
            const auto sq = to_sq(f, r);
            if (pos.ko() == sq) {
                feat[i][2] = 1;
            }
            switch(pos.square(sq)) {
                case BLACK:
                    if (pos.turn() == BLACK) {
                        feat[i][0] = 1;
                    } else {
                        feat[i][1] = 1;
                    }
                    break;
                case WHITE:
                    if (pos.turn() == BLACK) {
                        feat[i][1] = 1;
                    } else {
                        feat[i][0] = 1;
                    }
                    break;
                default:
                    break;
            }
            ++i;
        }
    }
    return feat;
}
void test_nn() {
}

}
#endif