#include "game.hpp"

//主函数部分
int main() {
    get_graph();
    menu();

    while (1) {
        loop();
    }
    return 0;
}
