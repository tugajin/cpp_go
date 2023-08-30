#include "game.hpp"
#include "movelist.hpp"
#include "util.hpp"
#include "movelegal.hpp"
#include "search.hpp"
#include "hash.hpp"
#include "nn.hpp"
#include "model.hpp"
#include "ubfm.hpp"
#include "selfplay.hpp"
#include "io.hpp"

TeeStream Tee;
namespace hash {
Key g_hash_pos[COLOR_SIZE][SQUARE_SIZE];
Key g_hash_turn;
}
namespace ubfm {
UBFMSearcherGlobal g_searcher_global;
}
namespace selfplay {
SelfPlayWorker g_selfplay_worker[SelfPlayWorker::NUM];
int g_thread_counter;
SelfPlayInfo g_selfplay_info;
}
namespace model {
GPUModel g_gpu_model[GPUModel::GPU_NUM];
}
int main(int /*argc*/, char **/*argv*/){
    /*auto num = 999999999;
    if (argc > 1) {
        num = std::stoi(std::string(argv[1]));
    }*/
    check_mode();
    hash::init();
    gen::test_legal();
    //gen::test_gen();
    //search::test_search();
    //io::test_io();
    return 0;
}
