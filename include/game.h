#ifndef GAME_H
#define GAME_H

// 宏定义
#define BORDER 15
#define border 15	//棋盘边界大小
#define MAX 256
#define _BLACK 1
#define _WHITE 2
#define EMPTY 0

// 结构体定义
struct position {
    int x, y;
};

// 函数声明
void menu();
void loop();
void clear_map(int map[BORDER][BORDER]);
void cleardevice();
void print_arr(int arr[BORDER][BORDER]);
void get_graph();
void button_print(int sx, int sy, int ex, int ey, const char* str, int height);
void menu_print();
int mouse_click();
void board_print();
void MouseChess_black(int board[BORDER][BORDER]);
void MouseChess_white(int board[BORDER][BORDER]);
char is_win(int arr[BORDER][BORDER]);
int game_over_white();
int game_over_black();
bool is_full(int map[BORDER][BORDER]);
int max_alphabeta(int dep, int alpha, int beta);
int min_alphabeta(int dep, int alpha, int beta);
int score_table(int num, int empty);
int count_score(int arr[], int color);
int evaluate();
int around_empty(int x, int y);
struct position minmax(int dep);
struct position greed();

#endif // GAME_H
