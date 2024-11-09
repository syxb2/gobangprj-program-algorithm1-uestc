#include<tchar.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<graphics.h>
#include<string.h>
#include<limits.h>
#include<easyx.h>
#include<Windows.h>
#include<conio.h>

#include"game.hpp"

#pragma comment(lib,"winmm.lib")

int map[border][border] = { 0 };
int score = 0;
int MODE = 0;
int m = 1;

/*-------------------------------------------------------------------------------------电脑下棋------------------------------------------------------------------------------------------*/

//评分以成五，活四，冲四，活三，冲三，活二，冲二，活一，冲一从大到小依次打分
int score_table(int num, int empty) {	//评分表，依据连子数和左右空格数评分
    if (num >= 5)return 100000;
    else if (num == 4) {
        if (empty == 2)return 10000;
        else if (empty == 1)return 1000;
    }
    else if (num == 3) {
        if (empty == 2)return 1000;
        else if (empty == 1)return 100;
    }
    else if (num == 2) {
        if (empty == 2)return 100;
        else if (empty == 1)return 10;
    }
    else if (num == 1) {
        if (empty == 2)return 10;
        else if (empty == 1)return 1;
    }
    return 0;
}

//逐行得到连子数和空格数，进而计算总分数（行列，左斜行，右斜行）
int count_score(int arr[ ], int color) {
    int num = 0, empty = 0, i, len = arr[0];
    score = 0;
    if (arr[1] == 0) empty++;
    else if (arr[1] == color) num++;

    for (i = 2; i <= len; i++) {
        if (arr[i] == color)num++;

        else if (arr[i] == 0) {
            if (num == 0)empty = 1;
            else {
                score += score_table(num, empty + 1);
                empty = 1;
                num = 0;
            }
        }
        else {
            score += score_table(num, empty);
            num = 0;
            empty = 0;
        }
    }
    score += score_table(num, empty);
    return score;
}

int evaluate() {
    int score_ai = 0, score_person = 0, i, j, m, n;
    int arr[30] = { 0 };

    //读取行
    for (i = 0; i < border; i++) {
        for (j = 0; j < border; j++) {
            arr[++arr[0]] = map[i][j];
        }
        score_ai += count_score(arr, _WHITE);
        score_person += count_score(arr, _BLACK);
        arr[0] = 0;
    }

    //读取列
    for (j = 0; j < border; j++) {
        for (i = 0; i < border; i++) {
            arr[++arr[0]] = map[i][j];
        }
        score_ai += count_score(arr, _WHITE);
        score_person += count_score(arr, _BLACK);
        arr[0] = 0;
    }

    //读取右上斜行
    for (i = 0; i < border; i++) {
        for (j = 0, m = i; j < border; j++, m++) {
            arr[++arr[0]] = map[j][m];
        }
        score_ai += count_score(arr, _WHITE);
        score_person += count_score(arr, _BLACK);
        arr[0] = 0;
    }

    //读取右下斜行
    for (j = 1; j < border; j++) {
        for (i = 0, m = j; m < border; i++, m++) {
            arr[++arr[0]] = map[m][i];
        }
        score_ai += count_score(arr, _WHITE);
        score_person += count_score(arr, _BLACK);
        arr[0] = 0;
    }

    //读取左下斜行
    for (i = 0; i < border; i++) {
        for (j = border - 1, m = i, n = 0; m < border; j--, m++, n++) {
            arr[++arr[0]] = map[m][j];
        }
        score_ai += count_score(arr, _WHITE);
        score_person += count_score(arr, _BLACK);
        arr[0] = 0;
    }

    //读取左上斜行
    for (j = border - 2; j >= 0; j--) {
        for (i = 0, m = j; m >= 0; i++, m--) {
            arr[++arr[0]] = map[i][m];
        }
        score_ai += count_score(arr, _WHITE);
        score_person += count_score(arr, _BLACK);
        arr[0] = 0;
    }
    return score_ai - score_person;
}

int around_empty(int x, int y) {
    int i = 0, j = 0, m = 0;
    for (i = (x - 1 > 0 ? x - 1 : 0); i <= x + 1 && i < border; i++) {
        for (j = (y - 1 > 0 ? y - 1 : 0); j <= y + 1 && j < border; j++) {
            if (map[i][j] != 0)m++;
        }
    }
    if (m != 0)return 1;
    return 0;
}

int min_alphabeta(int dep, int alpha, int beta) {
    if (dep == 0)return evaluate();
    struct position v[300];
    int i, j;
    v[0].x = 0;
    for (i = 0; i < border; i++) {
        for (j = 0; j < border; j++) {
            if (map[i][j] == EMPTY && around_empty(i, j)) {
                v[++v[0].x].x = i;
                v[v[0].x].y = j;
            }
        }
    }

    int len = v[0].x;
    int best = INT_MAX;
    for (i = 1; i <= len; i++) {
        map[v[i].x][v[i].y] = _BLACK;
        int t = max_alphabeta(dep - 1, best < alpha ? best : alpha, beta);
        map[v[i].x][v[i].y] = EMPTY;
        if (t < best)best = t;
        if (t < beta)break;
    }
    return best;
}

int max_alphabeta(int dep, int alpha, int beta) {
    if (dep == 0)return evaluate();
    struct position v[300];
    int i, j;
    v[0].x = 0;
    for (i = 0; i < border; i++) {
        for (j = 0; j < border; j++) {
            if (map[i][j] == EMPTY && around_empty(i, j)) {
                v[++v[0].x].x = i;
                v[v[0].x].y = j;
            }
        }
    }

    int len = v[0].x;
    int best = INT_MIN;
    for (i = 1; i <= len; i++) {
        map[v[i].x][v[i].y] = _WHITE;
        int t = min_alphabeta(dep - 1, alpha, best > beta ? best : beta);
        map[v[i].x][v[i].y] = EMPTY;
        if (t > best)best = t;
        if (t > alpha)break;
    }
    return best;
}

struct position minmax(int dep) {
    struct position v[300], v_best[300];
    int i, j;
    v_best[0].x = 0;
    v_best[0].x = 0;
    v[0].x = 0;
    for (i = 0; i < border; i++) {
        for (j = 0; j < border; j++) {
            if (map[i][j] == EMPTY && around_empty(i, j)) {
                v[++v[0].x].x = i;
                v[v[0].x].y = j;
            }
        }
    }

    int best = INT_MIN;
    int len = v[0].x;
    for (i = 1; i <= len; i++) {
        map[v[i].x][v[i].y] = _WHITE;
        int t = min_alphabeta(dep, INT_MAX, INT_MIN);
        if (t == best)v_best[++v_best[0].x] = v[i];
        if (t > best) {
            best = t;
            v_best[0].x = 0;
            v_best[++v_best[0].x] = v[i];
        }
        map[v[i].x][v[i].y] = EMPTY;
    }
    len = v_best[0].x;
    int k = rand() % len + 1;
    Sleep(300);
    return v_best[k];
}

//贪心算法，简单模式
struct position greed() {
    struct position v[300], v_best[300];
    int i, j;
    v[0].x = 0;
    for (i = 0; i < border; i++) {
        for (j = 0; j < border; j++) {
            if (map[i][j] == EMPTY && around_empty(i, j)) {
                v[++v[0].x].x = i;
                v[v[0].x].y = j;
            }
        }
    }
    int t = 0;
    int best = INT_MIN;
    v_best[0].x = 0;
    int len = v[0].x;
    for (i = 1; i <= len; i++) {
        map[v[i].x][v[i].y] = _WHITE;
        t = evaluate();
        if (t == best)v_best[++v_best[0].x] = v[i];
        else if (t > best) {
            best = t;
            v_best[0].x = 0;
            v_best[++v_best[0].x] = v[i];
        }
        map[v[i].x][v[i].y] = EMPTY;
    }
    len = v_best[0].x;
    int k = rand() % len + 1;

    Sleep(400);
    return v_best[k];
}

/*-------------------------------------------------------------------------------电脑下棋结束------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------创建窗口和菜单--------------------------------------------------------------------------------------*/
//创建窗口的函数
void get_graph() {
    initgraph(800, 600);//创建窗口
    setbkcolor(RGB(232, 234, 237));//设置窗口背景颜色为浅白色 使用RGB宏定义
    cleardevice();//使白色覆盖窗口
}


//绘制矩形按钮的函数(前四个参数分别为矩形左上角/右下角的横纵坐标 第五个为文本 第六个为文本的高度 用于调整文本大小
void button_print(int sx, int sy, int ex, int ey, const char* str, int height) {
    setfillcolor(RED);//填充矩形的颜色为红色
    setlinecolor(BLACK);//矩形边框颜色为黑色
    fillrectangle(sx, sy, ex, ey);//绘制矩形
    //将文本居中打印在按钮中
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    settextstyle(height, 0, "微软雅黑");//三个参数分别为文本的高度 宽度 字体 宽度为0表示自适应即根据高度自动调整宽度
    int length = textwidth(str);
    int tx = sx + (ex - sx - length) / 2;
    int ty = sy + (ey - sy - height) / 2;
    outtextxy(tx, ty, str);
    settextcolor(GREEN);//再次将文本颜色转为绿色 方便绘制菜单
}


//绘制菜单的函数
void menu_print() {
    // initgraph(800, 600);//创建窗口
    setbkcolor(RGB(232, 234, 237));//设置窗口背景颜色为浅白色 使用RGB宏定义
    cleardevice();//使白色覆盖窗口

    //加入贴图
    IMAGE img[5];
    loadimage(img, "pictures/原图n1.jpg");
    putimage(20, 80, img);

    loadimage(img + 1, "pictures/原图n2.jpg");
    putimage(20, 280, img + 1);

    loadimage(img + 2, "pictures/原图n3.jpg");
    putimage(520, 80, img + 2);

    loadimage(img + 3, "pictures/原图n4.jpg");
    putimage(520, 330, img + 3);

    //绘制上下两条虚线作为边界
    setlinestyle(PS_DASH, 3);
    setlinecolor(GREEN);
    line(50, 50, 750, 50);
    line(50, 550, 750, 550);

    //在菜单窗口上打印需要的文本
    settextstyle(30, 0, ("微软雅黑"));
    settextcolor(GREEN);

    outtextxy((getwidth() - textwidth("请选择对局模式")) / 2, 75, ("请选择对局模式"));

    setlinestyle(PS_SOLID, 3);
    settextstyle(20, 0, ("微软雅黑"));

    outtextxy((getwidth() - textwidth("Player versus Player")) / 2, 125, ("Player versus Player"));
    button_print((getwidth() - textwidth("Player versus Player")) / 2 + 20, 155, (getwidth() + textwidth("Player versus Player")) / 2 - 20, 185, "对局开始", 20);

    outtextxy((getwidth() - textwidth("Player versus Environment")) / 2, 195, ("Player versus Environment"));
    button_print((getwidth() - textwidth("Player versus Player")) / 2 + 20, 225, (getwidth() + textwidth("Player versus Player")) / 2 - 20, 255, "对局开始", 20);

    outtextxy((getwidth() - textwidth("Quit")) / 2, 265, ("Quit"));
    button_print((getwidth() - textwidth("Player versus Player")) / 2 + 20, 295, (getwidth() + textwidth("Player versus Player")) / 2 - 20, 325, "退出游戏", 20);

    //getchar();
    //closegraph();

}


//判断鼠标是否点击按钮（左键点击）并根据点击区域返回不同的整数 配合switch case语句实现不同模式的选择
int mouse_click() {
    ExMessage msg;
    while (true)//使用循环确保获取的是鼠标左键单击的消息
    {
        if (peekmessage(&msg, EX_MOUSE) && msg.message == WM_LBUTTONDOWN) {

            if (msg.x > (getwidth() - textwidth("Player versus Player")) / 2 + 20 && msg.x < (getwidth() + textwidth("Player versus Player")) / 2 - 20 && msg.y > 155 && msg.y < 185)
                return 1;
            if (msg.x > (getwidth() - textwidth("Player versus Player")) / 2 + 20 && msg.x < (getwidth() + textwidth("Player versus Player")) / 2 - 20 && msg.y > 225 && msg.y < 255)
                return 2;
            if (msg.x > (getwidth() - textwidth("Player versus Player")) / 2 + 20 && msg.x < (getwidth() + textwidth("Player versus Player")) / 2 - 20 && msg.y > 295 && msg.y < 325)
                return 3;
        }
    }
}


/*
//绘制棋盘的函数
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
  for (i = 1; i <= 16; i++) {
    line(i * width, 1 * width, i * width, 16 * width);
    line(1 * width, i * width, 16 * width, i * width);
  }
  // getchar();
  // closegraph();
}
*/

/*-----------------------------------------------------------------创建窗口和菜单结束----------------------------------------------------------------------------------------*/

/*
// 玩家下棋（鼠标落子）
void MouseChess(int (*board)[border]) {
  initgraph(800, 600);

  int currentPlayer = 1;

  while (!_kbhit()) {
    MOUSEMSG m;
    while (MouseHit()) {
      m = GetMouseMsg();
      if (m.uMsg == WM_LBUTTONDOWN) {
        int mouseX = m.x / (getwidth() / border);
        int mouseY = m.y / (getheight() / border);
        if (board[mouseY][mouseX] == 0) {
          board[mouseY][mouseX] = currentPlayer;
          board_print();
          currentPlayer = (currentPlayer == 1) ? 2 : 1;
        }
      }
    }
  }

  closegraph();
}
*/

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




/*------------------------------------------------------------------------------------调试代码------------------------------------------------------------------*/
/*
void print_arr(int(*arr)[BORDER]) {
    // arr[1][1] = 1;
    int j = 0, i = 0;
    for (i = 0; i < BORDER; i++) {
        for (j = 0; j < BORDER; j++)
            printf("%3d", arr[i][j]);
        printf("\n");
    }
}
*/
/*----------------------------------------------------------------------------------调试代码结束------------------------------------------------------------------*/
