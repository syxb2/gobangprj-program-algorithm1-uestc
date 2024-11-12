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
