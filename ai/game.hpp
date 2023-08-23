#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <array>
#include <bitset>
#include <functional>
#include "common.hpp"
#include "util.hpp"
#include "movelist.hpp"
namespace game {
class Position;
}
namespace hash {
Key hash_key(const game::Position &pos);
}
namespace game {

class Position;

class NeighborStone {
public:
    NeighborStone() {
        this->color_ = WALL;
        this->liberty_num_ = this->stone_num_ = 0;
        REP(i, SQUARE_SIZE) { this->checked_[i] = false; }
    };
    void search(const game::Position &pos, const Square sq);
    int liberty_num() const { 
        return this->liberty_num_;
    }
    int stone_num() const {
        return this->stone_num_;
    }
    void set_color(const Color c) {
        this->color_ = c;
    }
    Color color() const {
        return this->color_;
    }
private:
    bool checked_[SQUARE_SIZE];
    int liberty_num_;//呼吸点の数
    int stone_num_;//石の数
    Color color_;//石の色
};

class NeighborStoneInfo {
public:
    enum MoveType {
        LEGAL,
        SUICIDE,
        KO,
        EYE,
        FILLED,
    };

    NeighborStoneInfo(const Square sq) : 
        neighbor_stone_{ NeighborStone(), NeighborStone(), NeighborStone(), NeighborStone() },
        sq_(sq),
        maybe_ko_(MOVE_PASS),
        captured_stone_num_(0),
        space_num_(0),
        safe_stone_num_(0),
        wall_num_(0)
    {}
    void search(const game::Position &pos);
    MoveType is_legal(const game::Position &pos) const;
    RemoveStoneFlag get_flag(const game::Position &pos);
private:
    NeighborStone neighbor_stone_[4];
    Square sq_;
    Move maybe_ko_;
    int captured_stone_num_;
    int space_num_;
    int safe_stone_num_;
    int wall_num_;
};
class Position {
public:
    Position() : turn_(BLACK), ko_(SQUARE_NONE), ply_(0)  {
        this->captured_[BLACK] = this->captured_[WHITE] = 0;
        this->record_[0] = MOVE_PASS;
        REP_FILE(f) {
            REP_RANK(r) {
                const auto sq = to_sq(f, r);
                if (f == 0 || f == (FILE_SIZE-1) || r == 0 || r == (RANK_SIZE-1)) {
                    this->stones_[sq] = WALL;
                } else {
                    this->stones_[sq] = EMPTY;
                }
            }
        }
    }
    Position(const Color stones[], const int captured[], const Color turn, const Square ko = SQUARE_NONE, const int ply = 0) :
        turn_(turn),
        ko_(ko),
        ply_(ply) {
        this->captured_[BLACK] = captured[0];
        this->captured_[WHITE] = captured[1];
        this->record_[0] = MOVE_PASS;
        auto sp = 0;
        REP_FILE(f) {
            { const auto sq = to_sq(f, Rank(RANK_1-1)); this->stones_[sq] = WALL; }
            { const auto sq = to_sq(f, Rank(RANK_SIZE-1)); this->stones_[sq] = WALL; }
        }
        REP_RANK(r) {
            { const auto sq = to_sq(File(FILE_A-1), r); this->stones_[sq] = WALL; }
            { const auto sq = to_sq(File(FILE_SIZE-1), r); this->stones_[sq] = WALL; }
        }
        REP_RANK2(r) {
            REP_FILE2(f) {
                const auto sq = to_sq(f, r);
                this->stones_[sq] = static_cast<Color>(stones[sp++]);
            }
        }
    }
    Position(const uint32 h) {
    }
    Color square(const Square sq) const {
        ASSERT2(sq_is_ok(sq),{Tee<<sq<<std::endl;});
        return this->stones_[sq];
    }
    bool is_done() const {
        if (this->ply() < 2) {
            return false;
        }
        const auto prev_move = this->record_[this->ply()-1];
        const auto prev2_move = this->record_[this->ply()-2];
        return (prev_move == MOVE_PASS && prev2_move == MOVE_PASS);
    }
    bool is_lose() const {
        if (!this->is_done()) {
            return false;
        }
        return this->count_score(this->turn()) < 0;
    }
    bool is_win() const {
        if (!this->is_done()) {
            return false;
        }
        return this->count_score(this->turn()) > 0;
    }
    bool is_draw() const {
        if (!this->is_done()) {
            return false;
        }
        return this->count_score(this->turn()) == 0;
    }
    int ply() const {
        return this->ply_;
    }
    int count_score(const Color c) const {
        auto black_score = 0;
        auto white_score = 0;
        REP_FILE(f) {
            REP_RANK(r) {
                const auto sq = to_sq(f,r);
                switch (this->square(sq)) {
                    case BLACK:
                        ++black_score;
                        break;
                    case WHITE:
                        ++white_score;
                        break;
                    case EMPTY: {
                        int nei_count[COLOR_SIZE + 1] = {0,0,0,0};
                        for (const auto dir : DIR4) {
                            ++nei_count[this->square(static_cast<Square>(sq + dir))];
                        }
                        if (nei_count[BLACK] && !nei_count[WHITE]) {
                            ++black_score;
                        } else if (nei_count[WHITE] && !nei_count[BLACK]) {
                            ++white_score;
                        }
                    }
                    default:
                        break;
                }
            }
        }
        const auto score = black_score - white_score;
        return c == BLACK ? score : -score;
    }
    Position next(const Move action) const {
        auto new_pos = *this;
        const auto opp_color = change_turn(this->turn());
        new_pos.turn_ = opp_color;
        new_pos.ko_ = SQUARE_NONE;
        new_pos.record_[this->ply()] = action;
        ++new_pos.ply_;
        if (action == MOVE_PASS) {
            return new_pos;
        } 
        const auto flag = move_to_flag(action);
        const auto sq = move_to_sq(action);
        auto clear_stone_num = 0;
        Square clear_stone_list[SQUARE_SIZE] = { SQUARE_SIZE };
        REP(i,4) {
            if (static_cast<int>(flag) & (1 << i)) {
                const auto next_sq = static_cast<Square>(sq + DIR4[i]);
                if (new_pos.square(next_sq) != EMPTY) {
                    new_pos.clear_stone(next_sq, opp_color, clear_stone_list, &clear_stone_num);
                }
            }
        }
        new_pos.stones_[sq] = this->turn();
        NeighborStone ns;
        ns.set_color(this->turn());
        ns.search(new_pos, sq);
        if (clear_stone_num == 1 && ns.stone_num() == 1 && ns.liberty_num() == 1) {
            ASSERT(sq_is_ok(clear_stone_list[0]));
            new_pos.ko_ = clear_stone_list[0];
        }
        new_pos.captured_[this->turn()] += clear_stone_num;
        return new_pos;
    }
    Color turn() const {
        return this->turn_;
    }
    Square ko() const {
        return this->ko_;
    }
    bool is_ok() const {
        REP_RANK(r) {
            REP_FILE(f) {
                const auto sq = to_sq(f,r);
                if (f == 0 || f == (FILE_SIZE-1) || r == 0 || r == (RANK_SIZE-1)) {
                    if (this->square(sq) != WALL) {
                        Tee<<"wall error\n";
                        Tee<<sq<<std::endl;
                        return false;
                    }
                } else {
                    if (this->square(sq) != EMPTY
                    && this->square(sq) != BLACK
                    && this->square(sq) != WHITE) {
                        Tee<<"square error\n";
                        Tee<<sq<<std::endl;
                        Tee<<this->square(sq)<<std::endl;
                        return false;
                    }
                }
            }
        }
        return true;
    }
    std::string str() const {
        std::string str = "";
        str += "turn:" + color_str(this->turn()) + "\n";
        str += "ply:" + to_string(this->ply()) + "\n";
        if (this->ko_ == SQUARE_NONE) {
            str += "ko:NONE\n";
        } else {
            str += "ko:"+move_str(make_move(REMOVE_NONE, this->ko_))+"\n";
        }
        str += "white captured:" + to_string(this->captured_[WHITE]) + "\n";
        REP_RANK2(r) {
            if (r == RANK_1) { 
                str += " ";
                REP_FILE2(f) {
                    str += " " + file_str(f);
                }
                str += "\n";
            }
            REP_FILE2(f) {
                if (f == FILE_A) {
                    str += rank_str(r);
                }
                const auto sq = to_sq(f, r);
                if (this->stones_[sq] == EMPTY) {
                    str += " .";
                } else if (this->stones_[sq] == BLACK) {
                    str += " o";
                } else if (this->stones_[sq] == WHITE) {
                    str += " x";
                } else if (this->stones_[sq] == WALL) {
                    str += " #";
                }
            }
            str += "\n";
        }
        str += "black captured:" + to_string(this->captured_[BLACK]) + "\n";
        return str;
    }
    Key history() const {
        return Key(0);
    }
	friend std::ostream& operator<<(std::ostream& os, const Position& pos) {
        os << pos.str();
		return os;
	}
private:
    void clear_stone(const Square sq, const Color color, Square clear_stone_list[], int* clear_stone_num) {
        ASSERT2(this->square(sq) != EMPTY,{
            Tee<<"sq:"<<sq<<std::endl;
        });
        clear_stone_list[*clear_stone_num] = sq;
        (*clear_stone_num) += 1;
        this->stones_[sq] = EMPTY;
        for(const auto dir : DIR4) {
            const Square next_sq = static_cast<Square>(sq + dir);
            const auto stone = this->square(next_sq);
            if (stone == color) {
                ASSERT(this->square(next_sq) != EMPTY);
                this->clear_stone(next_sq, color, clear_stone_list, clear_stone_num);
            }
        }
    }

    Color stones_[SQUARE_SIZE];
    int captured_[COLOR_SIZE];
    Color turn_;
    Square ko_;
    Move record_[MAX_PLY];
    int ply_;
};


void NeighborStone::search(const game::Position &pos, const Square sq) {
    ASSERT(pos.square(sq) != EMPTY);
    this->checked_[sq] = true;
    ++this->stone_num_;
    for(auto dir : DIR4) {
        const auto next_sq = static_cast<Square>(sq + dir);
        if (this->checked_[next_sq]) { continue; }
        if (pos.square(next_sq) == EMPTY) {
            this->checked_[next_sq] = true;
            ++this->liberty_num_;
        } else if (pos.square(next_sq) == this->color_){
            this->search(pos,next_sq);
        }
    }
}

void NeighborStoneInfo::search(const game::Position &pos) {
    auto opp_color = change_turn(pos.turn());
    REP(i,4) {
        const auto dir = DIR4[i];
        const auto next_sq = Square(this->sq_ + dir);
        const auto color = pos.square(next_sq);
        neighbor_stone_[i].set_color(color);
        if (color == EMPTY) { ++this->space_num_; continue; }
        if (color == WALL) { ++this->wall_num_; continue; }
        neighbor_stone_[i].search(pos,next_sq);
        if (color == opp_color && neighbor_stone_[i].liberty_num() == 1) {
            this->captured_stone_num_ += neighbor_stone_[i].stone_num();
            this->maybe_ko_ = static_cast<Move>(next_sq);
        } else if (color == pos.turn() && neighbor_stone_[i].liberty_num() >= 2) {
            ++this->safe_stone_num_;
        }
    }
}
NeighborStoneInfo::MoveType NeighborStoneInfo::is_legal(const game::Position &pos) const {
    if (pos.ko() == this->sq_) {
        return KO;
    }
    if (pos.square(this->sq_) != EMPTY) {
        return FILLED;
    }
    if (this->captured_stone_num_ == 0 
    && this->space_num_ == 0 
    && this->safe_stone_num_ == 0) { 
        return SUICIDE; 
    }
    if ((this->wall_num_ + this->safe_stone_num_) == 4) { 
        return EYE; 
    }
    return LEGAL;
}
RemoveStoneFlag NeighborStoneInfo::get_flag(const game::Position &pos) {
    const auto opp_color = change_turn(pos.turn());
    auto ret = REMOVE_NONE;
    REP(i, 4) {
        const auto liberty = this->neighbor_stone_[i].liberty_num();
        const auto col = this->neighbor_stone_[i].color();
        if (col == opp_color && liberty == 1 ) {
            ret |= REMOVE_FLAG4[i];
        }
    }
    return ret;
}

void test_pos() {

}    

}
#endif