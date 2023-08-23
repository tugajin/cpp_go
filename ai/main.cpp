#include "game.hpp"
#include "movelist.hpp"
#include "util.hpp"
#include "movelegal.hpp"
TeeStream Tee;

int main(int /*argc*/, char **/*argv*/){
    /*auto num = 999999999;
    if (argc > 1) {
        num = std::stoi(std::string(argv[1]));
    }*/
    check_mode();
    gen::test_legal();
    //gen::test_gen();
    return 0;
}
