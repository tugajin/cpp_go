#ifndef __NN_HPP__
#define __NN_HPP__
#include "game.hpp"
#include "common.hpp"
#include <vector>
namespace nn {
constexpr inline int FEAT_SIZE = 2;
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
    Feature feat(FEAT_SIZE, std::vector<int>(SQUARE_SIZE, 0));
    REP_POS(i) {
        feat[0][i] = pos.self(i);
        feat[1][i] = pos.enemy(i);
    }
    return feat;
}
void test_nn() {
}

}
#endif