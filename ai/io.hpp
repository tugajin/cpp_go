#ifndef __IO_HPP__
#define __IO_HPP__
#include "game.hpp"
namespace io {
std::string io_key(const game::Position &pos) {
    std::string ret = pos.turn() == BLACK ? "b_" : "w_";
    REP_FILE2(f) {
        REP_RANK2(r) {
            const auto sq = to_sq(f, r);
            const auto stone = pos.square(sq);
            ret += to_string(static_cast<int>(stone));        
        }
    }
    ret += "_" + to_string(pos.captured(BLACK))
        + "_" + to_string(pos.captured(WHITE)) 
        + "_" + to_string(pos.ko());
    return ret;
}
game::Position from_io(const std::string io_str) {
    std::vector<std::string> io_split_list;
    my_split(io_str,"_",io_split_list);
    auto color = io_split_list[0] == "b" ? BLACK : WHITE;
    auto pos = 0;
    Color stones[POS_SIZE] = {};
    for(const auto c : io_split_list[1]) {
        if (pos < POS_SIZE) {
            switch(c) {
                case '0':
                    stones[pos++] = EMPTY; break;
                case '1':
                    stones[pos++] = BLACK; break;
                case '2':
                    stones[pos++] = WHITE; break;
                default: break;
            }
        }
    }
    int captured[COLOR_SIZE] = {0,0,0};
    captured[BLACK] = to_int(io_split_list[2]);
    captured[WHITE] = to_int(io_split_list[3]);
    Square ko = static_cast<Square>(to_int(io_split_list[4]));
    return game::Position(stones,captured,color,ko);
}
void test_io() {
    {
        auto pos = game::Position();
        Tee<<pos<<std::endl;
        
    }
}
}
#endif