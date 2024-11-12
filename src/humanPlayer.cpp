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

//绘制棋盘
void board_print() {
    //initgraph(800, 600,EX_SHOWCONSOLE);//创建窗口
    //setbkcolor(RGB(232,234,237));//设置窗口背景颜色为浅白色 使用RGB宏定义   这三行写为了另一个函数get_graph
    //cleardevice();//使白色覆盖窗口

    setlinestyle(PS_SOLID, 2);//设置线条风格：实线 宽度为两个像素
    setlinecolor(BLACK);//设置线条颜色

    //绘制线条
    int width = 35;//定义棋盘格子的边长,方便修改
    int i;
    //line函数中四个参数分别是起点/终点的横纵坐标
    for (i = 1; i <= border; i++) {
        line(i * width, 1 * width, i * width, border * width);
        line(1 * width, i * width, border * width, i * width);
    }

    // 绘制棋盘上的九个定位点
    for (i = 4; i <= border - 3; i += 4) {
        for (int j = 4; j <= border - 3; j += 4) {
            setfillcolor(BLACK);
            fillcircle(i * width, j * width, 5);
        }
    }

    // 根据棋盘状态绘制棋子
    for (int i = 0; i < border; ++i) {
        for (int j = 0; j < border; ++j) {
            if (map[i][j] == 1) {
                setfillcolor(BLACK);
                fillcircle((j + 1) * width, (i + 1) * width, width / 2 - 5); // 绘制黑子
            }
            else if (map[i][j] == 2) {
                setfillcolor(WHITE);
                fillcircle((j + 1) * width, (i + 1) * width, width / 2 - 5); // 绘制白子
            }
        }
    }

    // Sleep(50);
    // getchar();
    // closegraph();
}


// 鼠标落子
void MouseChess_black(int(*board)[border]) {
    // get_graph();

    int currentPlayer = 1;
    int a = 0;

    while (a != 1) {
        MOUSEMSG m;
        while (MouseHit()) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                int mouseX = m.x / 36;
                int mouseY = m.y / 36;
                if (mouseX >= 0 && mouseX < border && mouseY >= 0 && mouseY < border && board[mouseY][mouseX] == 0) {
                    board[mouseY][mouseX] = currentPlayer;
                    board_print();
                    ++a;
                }
                /*
                if (m.uMsg == WM_LBUTTONDOWN) {
                  int mouseX = m.x / (getwidth() / border);
                  int mouseY = m.y / (getheight() / border);
                  if (board[mouseY][mouseX] == 0) {
                    board[mouseY][mouseX] = currentPlayer;
                    board_print();
                  }
                  ++a;*/

            }
        }
    }

    // closegraph();
}

void MouseChess_white(int(*board)[border]) {
    // get_graph();

    int currentPlayer = 2;
    int a = 0;

    while (a != 1) {
        MOUSEMSG m;
        while (MouseHit()) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                int mouseX = m.x / 36;
                int mouseY = m.y / 36;
                if (mouseX >= 0 && mouseX < border && mouseY >= 0 && mouseY < border && board[mouseY][mouseX] == 0) {
                    board[mouseY][mouseX] = currentPlayer;
                    board_print();
                    ++a;
                }
                /*
              if (m.uMsg == WM_LBUTTONDOWN) {
                int mouseX = m.x / (getwidth() / border);
                int mouseY = m.y / (getheight() / border);
                if (board[mouseY][mouseX] == 0) {
                  board[mouseY][mouseX] = currentPlayer;
                  board_print();
                }
                ++a;*/
            }
        }
    }

    // closegraph();
}
