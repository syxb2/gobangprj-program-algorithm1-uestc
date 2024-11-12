#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <graphics.h>
#include <string.h>
#include <limits.h>
#include <easyx.h>
#include <Windows.h>
#include <conio.h>

#include "game.hpp"

/**
 * @brief 判断输赢('A'是继续'B'是平局'C'是黑子胜利'D'是白子胜利)（1表示白子，2表示黑子）
 *
 * @return char
 */
char is_win(int(*arr)[BORDER]) {
    // 判断行
    for (int i = 0; i < BORDER; ++i) {
        for (int j = 0; j < BORDER - 4; ++j) {
            if (arr[i][j] == 1 && arr[i][j + 1] == 1 && arr[i][j + 2] == 1 && arr[i][j + 3] == 1 && arr[i][j + 4] == 1) {
                return 'C';
            }
            else if (arr[i][j] == 2 && arr[i][j + 1] == 2 && arr[i][j + 2] == 2 && arr[i][j + 3] == 2 && arr[i][j + 4] == 2) {
                return 'D';
            }
        }
    }
    // 判断列
    for (int i = 0; i < BORDER - 4; ++i) {
        for (int j = 0; j < BORDER; ++j) {
            if (arr[i][j] == 1 && arr[i + 1][j] == 1 && arr[i + 2][j] == 1 && arr[i + 3][j] == 1 && arr[i + 4][j] == 1) {
                return 'C';
            }
            else if (arr[i][j] == 2 && arr[i + 1][j] == 2 && arr[i + 2][j] == 2 && arr[i + 3][j] == 2 && arr[i + 4][j] == 2) {
                return 'D';
            }
        }
    }
    // 判断对角
    for (int i = 0; i < BORDER - 4; ++i) {
        for (int j = 0; j < BORDER - 4; ++j) {
            if (arr[i][j] == 1 && arr[i + 1][j + 1] == 1 && arr[i + 2][j + 2] == 1 && arr[i + 3][j + 3] == 1 && arr[i + 4][j + 4] == 1) {
                return 'C';
            }
            else if (arr[i][j] == 2 && arr[i + 1][j + 1] == 2 && arr[i + 2][j + 2] == 2 && arr[i + 3][j + 3] == 2 && arr[i + 4][j + 4] == 2) {
                return 'D';
            }
        }
    }
    // 判断斜对角
    for (int i = 4; i < BORDER; ++i) {
        for (int j = 0; j < BORDER - 4; ++j) {
            if (arr[i][j] == 1 && arr[i - 1][j + 1] == 1 && arr[i - 2][j + 2] == 1 && arr[i - 3][j + 3] == 1 && arr[i - 4][j + 4] == 1) {
                return 'C';
            }
            else if (arr[i][j] == 2 && arr[i - 1][j + 1] == 2 && arr[i - 2][j + 2] == 2 && arr[i - 3][j + 3] == 2 && arr[i - 4][j + 4] == 2) {
                return 'D';
            }
        }
    }
    // if (is_full()) {
    //   return 'B';
    // }
    return 'A';
    return 'C';
}


/**
 * @brief 选择'是'-返回6；选择'否'-返回7（当选择是时重新开始，当选择否是退回菜单）
 *
 * @return int
 */
int game_over_white() {
    board_print();
    int msgboxID = MessageBox(
        GetHWnd(),
        _T("///////////////////////////////////////////////////////////////\n\n你想再来一局吗？\n\n选择“是”以重新开始\n选择“否”以返回菜单\n\n///////////////////////////////////////////////////////////////"),
        _T("游戏结束，白棋胜利"),
        MB_YESNO
    );
    m = 1;

    return msgboxID;

    //    MessageBox(GetHWnd(), _T("白棋获胜"), _T("游戏结束"), MB_OK);
}

int game_over_black() {
    board_print();
    int msgboxID = MessageBox(
        GetHWnd(),
        _T("///////////////////////////////////////////////////////////////\n\n你想再来一局吗？\n\n选择“是”以重新开始\n选择“否”以返回菜单\n\n///////////////////////////////////////////////////////////////"),
        _T("游戏结束，黑棋胜利"),
        MB_YESNO
    );
    m = 1;

    return msgboxID;

    //    MessageBox(GetHWnd(), _T("白棋获胜"), _T("游戏结束"), MB_OK);
}


//判断棋盘是否满(满返回1，不满返回0)
bool is_full(int(*map)[border]) {
    int a = 0;
    for (int i = 0; i < border; ++i) {
        for (int j = 0; j < border; ++j) {
            if (map[i][j] == 0) {
                a = 1;
            }
        }
    }
    if (a == 0) {
        return 1;
    }
    else
        return 0;
}


// 清除棋盘
void clear_map(int(*map)[border]) {
    for (int i = 0; i < border; ++i) {
        for (int j = 0; j < border; ++j) {
            map[i][j] = 0;
        }
    }
}


// 打印菜单
void menu() {
    cleardevice();
    menu_print();
    switch (mouse_click()) {
        case 1:
            MODE = 1;
            cleardevice();
            board_print();
            break;
        case 2:
            MODE = 2;
            cleardevice();
            board_print();
            break;
        case 3:
            closegraph();
            exit(1);
            break;
    }
}


/**
 *
 * @brief 游戏主循环
 *   n 为模式表示变量：
 *        如果玩家选择“玩家vs玩家”就把n赋值为0
 *        如果玩家选择“玩家vs电脑”就把n赋值为1
 */
void loop() {
    if (is_win(map) == 'A') {
        board_print();
        if (MODE == 1) {
            if (is_win(map) == 'A')
                MouseChess_black(map);
            if (is_win(map) == 'A')
                MouseChess_white(map);
        }
        else if (MODE == 2) {
            if (m == 1) {
                MouseChess_black(map);
                m = 0;
            }
            else if (m == 0) {
                struct position xy = minmax(2);
                map[xy.x][xy.y] = 2;
                m = 1;
            }
        }

    }
    // 接下来的任务就是实现重新开始和返回菜单：
    else if (is_win(map) == 'C') {
        if (game_over_black() == 7) {
            clear_map(map);
            menu();
            while (1) {
                loop();
            }
        }
        else {
            clear_map(map);
            cleardevice();
            while (1) {
                loop();
            }
        }
    }
    else if (is_win(map) == 'D') {
        if (game_over_white() == 7) {
            clear_map(map);
            menu();
            while (1) {
                loop();
            }
        }
        else {
            clear_map(map);
            cleardevice();
            while (1) {
                loop();
            }
        }
    }
}

// 调试代码
// void print_arr(int(*arr)[BORDER]) {
//     // arr[1][1] = 1;
//     int j = 0, i = 0;
//     for (i = 0; i < BORDER; i++) {
//         for (j = 0; j < BORDER; j++)
//             printf("%3d", arr[i][j]);
//         printf("\n");
//     }
// }
