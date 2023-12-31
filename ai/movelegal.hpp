#ifndef __MOVELEGAL_HPP__
#define __MOVELEGAL_HPP__

#include "game.hpp"
#include "movelist.hpp"
#include "io.hpp"

namespace gen {

template<bool contain_eye = true>void legal_moves(const game::Position &pos, movelist::MoveList &ml) {
    ASSERT2(pos.is_ok(),{
        Tee<<pos<<std::endl;
    })
    REP_RANK2(r) {
        REP_FILE2(f) {
            const auto sq = to_sq(f,r);
            game::NeighborStoneInfo ns(sq);
            ns.search(pos);
            const auto result = ns.is_legal(pos);
            if (result == game::NeighborStoneInfo::LEGAL 
            || (contain_eye && result == game::NeighborStoneInfo::EYE)) {
                const auto flag = ns.get_flag(pos);
                const auto m = make_move(flag,sq);
                ml.add(m);
            }
        }
    }
    //if (ml.len() == 0) {
        ml.add(MOVE_PASS);
    //}
}
void test_gen() {
    if(1){
        auto pos = game::Position();
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
    }
    {
        Color stone[] = {
            WH,WH,WH,WH,
            WH,EM,WH,WH,
            WH,WH,WH,WH,
            WH,WH,WH,WH
        };
        int prisoner[] = {0,0};
        auto pos = game::Position(stone, prisoner, BLACK);
        Tee<<pos<<std::endl;
    }
    {
        Color stone[] = {
            WH,WH,WH,WH,
            WH,EM,WH,WH,
            WH,WH,WH,WH,
            WH,WH,WH,WH
        };
        int prisoner[] = {0,0};
        auto pos = game::Position(stone, prisoner, BLACK);
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
        legal_moves(pos,ml);
        pos = pos.next(ml[0]);
        Tee<<pos<<std::endl;
    }
    {
        Color stone[] = {
            EM,BL,EM,EM,
            BL,EM,BL,EM,
            EM,BL,EM,EM,
            EM,EM,EM,EM
        };
        int prisoner[] = {0,0};
        auto pos = game::Position(stone, prisoner, BLACK);
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
        legal_moves(pos,ml);
        Tee<<ml<<std::endl;
        game::NeighborStoneInfo ni(Square(7));
        ni.search(pos);
        Tee<<ni.is_legal(pos)<<std::endl;
        
    }
    {
        Color stone[] = {
            WH,WH,WH,WH,
            WH,EM,WH,WH,
            WH,WH,EM,WH,
            WH,WH,WH,WH
        };
        int prisoner[] = {0,0};
        auto pos = game::Position(stone, prisoner, BLACK);
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
        legal_moves(pos,ml);
        Tee<<ml<<std::endl;
    }
    {
        Color stone[] = {
            EM,BL,BL,BL,
            BL,BL,WH,WH,
            WH,BL,EM,BL,
            WH,BL,WH,BL
        };
        int prisoner[] = {3,0};
        auto pos = game::Position(stone, prisoner, WHITE);
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
        legal_moves(pos,ml);
        Tee<<ml<<std::endl;
        pos = pos.next(ml[0]);
        Tee<<pos<<std::endl;
    }
    {
        Color stone[] = {
            EM,EM,EM,EM,
            EM,BL,WH,EM,
            BL,WH,EM,WH,
            EM,BL,WH,EM
        };
        int prisoner[] = {0,0};
        auto pos = game::Position(stone, prisoner, BLACK);
        Tee<<pos<<std::endl;
        movelist::MoveList ml;
        legal_moves(pos,ml);
        Tee<<ml<<std::endl;
        pos = pos.next(ml[6]);
        Tee<<pos<<std::endl;
    }
    {
        auto pos = game::Position();
        int list[] = {7,8,9,10,13,14,15,16,19,20,21,22,25,8218,7,4109,19,28,32795,16,10,22,4124,16,4118,28688,9,10,15,4117,9,4121,22,27,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,16,16394,12304,9,10,32783,10,4112,28,-1,24586,9,4111,16,9};
        for(auto v : list) {
            Tee<<pos<<std::endl;
            const auto mv = static_cast<Move>(v);
            pos = pos.next(mv);
        }
    }
}
void test_legal() {
    auto black_win = 0;
    auto white_win = 0;
    auto draw = 0;
    REP(i,100000000) {
        auto pos = game::Position();
        const auto io_str = io::io_key(pos);
        auto pos2 = io::from_io(io_str);
        const auto io_str2 = io::io_key(pos2);
        ASSERT(io_str == io_str2);
        Tee<<i<<":"<<black_win<<":"<<white_win<<":"<<draw<<"\r";
        REP(j,5000000) {
            //Tee<<i<<std::endl;
            Tee<<pos<<std::endl;
            ASSERT(pos.is_ok());
            movelist::MoveList ml;
            gen::legal_moves<false>(pos,ml);
            const auto index = my_rand(ml.len());
            const auto move = ml[index];
      
            if (pos.is_done()) {
                //Tee<<pos<<std::endl;
                if (pos.is_draw()) {
                    draw++;
                }
                if (pos.is_win()) {
                    if (pos.turn() == BLACK) { black_win++; }
                    if (pos.turn() == WHITE) { white_win++; }
                }
                if (pos.is_lose()) {
                    if (pos.turn() == BLACK) { white_win++; }
                    if (pos.turn() == WHITE) { black_win++; }
                }
                break;
            }
            if (pos.is_rep()) {
                break;
            }
            if (pos.ply() >= 100) {
                //Tee<<"loop\n";
                draw++;
                break;
            }
            if (pos.ko() != SQUARE_NONE) {
                Tee<<"ko\n";
            }
            Tee<<to_string(index)<<":"<<move_str(move)<<std::endl;
            pos = pos.next(move);
        }
    }
}
}

#endif