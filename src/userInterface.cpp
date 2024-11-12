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

// //绘制棋盘的函数
// void board_print() {
//   //initgraph(800, 600,EX_SHOWCONSOLE);//创建窗口
//   //setbkcolor(RGB(232,234,237));//设置窗口背景颜色为浅白色 使用RGB宏定义   这三行写为了另一个函数get_graph
//   //cleardevice();//使白色覆盖窗口

//   setlinestyle(PS_SOLID, 2);//设置线条风格：实线 宽度为两个像素
//   setlinecolor(BLACK);//设置线条颜色

//   //绘制线条
//   int width = 35;//定义棋盘格子的边长,方便修改
//   int i;
//   //line函数中四个参数分别是起点/终点的横纵坐标
//   for (i = 1; i <= 16; i++) {
//     line(i * width, 1 * width, i * width, 16 * width);
//     line(1 * width, i * width, 16 * width, i * width);
//   }
//   // getchar();
//   // closegraph();
// }
