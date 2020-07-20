#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <graphics.h>
#include <vector>
#include <algorithm>
#include <string>
#include <windows.h>
#include <tchar.h>
#include <easyx.h>
#pragma comment(lib, "Winmm.lib")  

using namespace std;

//dot类记录棋子的状态
class dot {
public:

    //棋子的信息:坐标和颜色
    int x = 0, y = 0, color = 0;

    //构造函数
    dot(int x, int y, int color) :x(x), y(y), color(color) {}

    //重载赋值运算符
    dot& operator = (const dot& other) {
        this->x = other.x;
        this->y = other.y;
        this->color = other.color;
        return *this;
    }

    //棋子自身翻转
    void flip() {
        color *= -1;
    }

   
    //友元函数 实现翻转 dot1 和 dot2 两棋子之间的棋子的翻转，需要传递棋盘
    friend void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board);

    //友元函数 实现找到 新的棋子 的八个方向上需要翻转的不同颜色的棋的终点棋子的位置，
    //并传递给flip_between_two_dot函数 实现翻转
    friend void game(int x, int y, int color, vector<vector<dot>>& chess_board);

    //改变棋子自身颜色为特定颜色，用于在空白位置下棋
    void set_color(int new_color) {
        color = new_color;
    }
 
    //重载比较运算符，为了之后的去重
    bool operator == (const dot& other)const {
        if (this->x == other.x && this->y == other.y) return true;
        else return false;
    }
    bool operator < (const dot& other)const {
        if (this->x < other.x || (this->x == other.x && this->y < other.y)) return true;
        else return false;
    }
    bool operator > (const dot& other)const {
        if (this->x > other.x || (this->x == other.x && this->y > other.y)) return true;
        else return false;
    }
    //寻找哪一步能下
    friend void findway(vector<vector<dot> > const chess, vector<dot>& store);

    friend void drawChess(const dot& chessdot);
    
    friend void highlight(dot& chessdot);

    friend void reminderclear(dot& chessdot);


    friend void sumup(vector<vector<dot> > const chess);

};

MOUSEMSG m;//鼠标的参数变量
int Mouse_x, Mouse_y;//鼠标的位置
int steps = 80;
int reset = 0;
int win = 0;//黑棋胜利为0，白棋胜利为1
static vector<vector<dot>> chess_board(8, vector<dot>(8, dot(0, 0, 0)));
static vector<vector<dot>> last_chess_board(8, vector<dot>(8, dot(0, 0, 0)));
static vector<dot> available;//定义存储能下的棋子所处地，用一维动态数组存储
int num_black, num_white;//黑棋白棋数量
static int flag1 = 0, flag2 = 0;//标志是否无棋可下
int colorset = -1;//黑棋先行
int state = 1;

int count_dot = 0;//记录每轮的数

int Set_state() {
    return (-1) * state;
}

//全局函数

//判断胜负
void judgeGame();
//判断是否结束
bool is_End();
//双人模式
void double_Mode();
//模式选择界面
void draw_choose_Mode();
//模式选择函数
void choose_Mode();

//single_mode
int game_count(int x, int y, int color, vector<vector<dot>>& chess_board);
void computer_think();
void count_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board);
void updateWithInput_single();
void updateWithoutInput_single();
void show_single();
void single_mode();

void drawChessboard() {
    initgraph(1000, 1000);
    setbkcolor(RGB(200, 200, 169));
    cleardevice();
    setlinestyle(PS_SOLID, 3);//选择线
    setcolor(RGB(0, 0, 0));
    for (int i = 0; i <= 8; i++) {
        line(i * steps, 0, i * steps, 8 * steps);//行
        line(0, i * steps, 8 * steps, i * steps);//列
    }
    setfillcolor(RGB(200,200,150));
    solidrectangle(790,245,900,305);
    solidrectangle(790,325,900,385);
    solidrectangle(790,405,900,465);
    outtextxy(814, 267, _T("重来一局"));
    outtextxy(814, 347, _T("暂停游戏"));
    outtextxy(830, 427, _T("悔棋"));

}

void drawChess(dot& chessdot) {
    int RealChess_x = (chessdot.x + 0.5) * steps;//计算棋子的相应真实位置
    int RealChess_y = (chessdot.y + 0.5) * steps;//同上
    if (chessdot.color == -1) {
        setfillcolor(BLACK);
        solidcircle(RealChess_x, RealChess_y, 35);//圆心坐标、半径
    }//若为黑棋
    else if (chessdot.color == 1) {
        setfillcolor(WHITE);
        solidcircle(RealChess_x, RealChess_y, 35);
    }//若为白棋
}

void highlight(dot& chessdot) {
    int RealChess_x = (chessdot.x + 0.5) * steps;//计算棋子的相应真实位置
    int RealChess_y = (chessdot.y + 0.5) * steps;//同上
    setfillcolor(RGB(220, 220, 190));
    solidcircle(RealChess_x, RealChess_y, 35);
    
}
//清楚提醒高光
void reminderclear(dot& chessdot) {
    int RealChess_x = (chessdot.x + 0.5) * steps;//计算棋子的相应真实位置
    int RealChess_y = (chessdot.y + 0.5) * steps;//同上
    clearrectangle(RealChess_y - 36, RealChess_x - 36, RealChess_y + 36, RealChess_x + 36);//消除每个棋位
}
//初始化棋盘，中间四个棋子为交叉的黑白
void initialset() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((i == 3 && j == 3) || (i == 4 && j == 4)) {
                chess_board[i][j] = dot(i, j, 1);
                drawChess(chess_board[i][j]);
            }
            else if ((i == 3 && j == 4) || (i == 4 && j == 3)) {
                chess_board[i][j] = dot(i, j, -1);
                drawChess(chess_board[i][j]);
            }
            else {
                chess_board[i][j] = dot(i, j, 0);
            }
        }
    }
}
//不闪烁的刷新
void gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
//隐藏光标
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//开始菜单
void startmenu() {
    PlaySound(L"music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);//放背景音乐
  

    initgraph(800, 1000);
    IMAGE img;
    loadimage(&img, L"123.jpg", 0, 0);
    putimage(0, 0, &img);

    while (1) {
        m = GetMouseMsg();//获取鼠标信息
        if (m.uMsg == WM_LBUTTONDOWN) {//左键是否按下消息
            break;
        }
    }
    closegraph();

    PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
//初始化
void startup() {
    colorset = -1;//黑棋先行
    drawChessboard();//画棋盘
    initialset();
}

//打印棋子和高光区域
void show() {
    HideCursor();
    gotoxy(0, 0);
    BeginBatchDraw();
    for (int i = 0; i < available.size(); i++) {
        highlight(available[i]);
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            drawChess(chess_board[i][j]);
        }
    }
    if (colorset == 1) {
        outtextxy(800,200, _T("WHITE　Goes"));
    }
    else {
        outtextxy(800,200, _T("BLACK　Goes"));
    }

    TCHAR s[50];
    _stprintf_s(s, _T("black: %d    white: %d   empty:%d  "), num_black, num_white, 64 - num_white - num_black);
    outtextxy(750, 100, s);

    EndBatchDraw();
}
//统计黑棋和白棋的数量
void sumup(vector<vector<dot> > const chess) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) { 
            if (chess[i][j].color == 1) {
                num_white++;
            }
            else if (chess[i][j].color == -1) {
                num_black++;
            }
        }
   }
}
//判断该位置是否可以下棋
bool ok_to_draw(int x,int y) {
    int n = 0;
    for (int i = 0; i < available.size(); i++) {
        if (available[i].x == x && available[i].y == y) {
            n = 1;
            break;
        }
    }
    if (n == 0) {
        return false;
    }
    else return true;
}
//重置棋盘至上一步
void resetf() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chess_board[i][j] = last_chess_board[i][j];
        }
    }
}
//保存棋盘的状态
void setf() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            last_chess_board[i][j] = chess_board[i][j];
        }
    }
}
//无输入更新
void updateWithoutInput() {
    num_black = 0;
    num_white = 0;//计数器清零
    available.clear();//记录能下的位置的动态数组清空
    sumup(chess_board);
    findway(chess_board, available);
    if (colorset == -1 && available.empty() == 1) {//是否无棋子可下
        flag1 == 1;
    }
    else if (colorset == 1 && available.empty() == 1) {
        flag2 == 1;
    }

    if (is_End()) {//判断是否结束
        judgeGame();
    }
    else {
        flag1 = flag2 = 0;
    }
}
//有输入更新
void updateWithInput() {
    //实现鼠标点击以翻转
    if (MouseHit()) {
        m = GetMouseMsg();
        if (m.uMsg == WM_LBUTTONDOWN) {
            Mouse_x = m.x;
            Mouse_y = m.y;
            int a, b, flag = 0;

            for (a = 0; a < 8; a++) {
                for (b = 0; b < 8; b++) {
                    if (a >= (Mouse_x / steps) && b >= (Mouse_y / steps)) {
                        Mouse_x = a;
                        Mouse_y = b;
                        flag = 1;
                        break;
                    }
                }
                if (flag == 1)
                    break;
            }//locate

            //悔棋
            if (Mouse_y >= 405 && Mouse_y <= 465 && Mouse_x >= 790 && Mouse_x <= 900) {
                resetf();
                colorset = -colorset;
                cout << "\a";
            }
            setf();//保存棋盘

            if (ok_to_draw(Mouse_x, Mouse_y)) {
                chess_board[a][b].set_color(colorset);//在该点下棋
                PlaySound(L"chess.wav", NULL, SND_FILENAME | SND_ASYNC);
              
                game(Mouse_x, Mouse_y, colorset, chess_board);
                colorset = -colorset;//换方
            }
            //重来一局
            if (Mouse_y >= 245 && Mouse_y <= 305 && Mouse_x >= 790 && Mouse_x <= 900) {
                startup();
            }
            //暂停
            if (Mouse_y >= 325 && Mouse_y <= 385 && Mouse_x >= 790 && Mouse_x <= 900) {
                outtextxy(780, 390, _T("按回车键返回游戏"));
                system("pause");
                outtextxy(780, 390, _T("                                 "));
            }
            for (int i = 0; i < 8; i++) {//消除绘图效果
                for (int j = 0; j < 8; j++) {
                    reminderclear(chess_board[i][j]);
                }
            }
        }
    }
}
//显示选择模式界面
void draw_choose_Mode()
{
    initgraph(480, 480);
    setbkcolor(RGB(200, 200, 169));//背景颜色
    cleardevice();
    setfillcolor(RGB(200, 200, 150));//画方框
    solidrectangle(160, 120, 320, 200);
    solidrectangle(160, 280, 320, 360);
    setcolor(RGB(0, 0, 0));//字颜色
    outtextxy(207, 150, _T("单人模式"));
    outtextxy(207, 310, _T("双人模式"));
}
//选择模式函数
void choose_Mode() {
    draw_choose_Mode();//显示画面
    while (1) {
        if (MouseHit()) {//判断是否点击
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {//获取左键点下的位置
                Mouse_x = m.x;
                Mouse_y = m.y;
            }//选择单人模式
            if(Mouse_y >= 120 && Mouse_y <= 200 && Mouse_x >= 160 && Mouse_x <= 320){
                single_mode();
                closegraph();
            }
        
            //选择双人模式
            if (Mouse_y >= 280 && Mouse_y <= 360 && Mouse_x >= 160 && Mouse_x <= 320) {
                closegraph();
                double_Mode();
            }
        }
    }
}
//双人模式
void double_Mode() {
    startup();//棋盘的初始化（开始的界面）
    while (1) {
        show();//每一步展示，棋子的具体状况，棋子的边，棋子的数量
        updateWithInput();//只考虑鼠标所在位置的翻转
        updateWithoutInput();//翻转，判断条件，剩余的棋子计算，胜负条件——exit（0）——结束的界面
    }
}


//主函数
int main() {
    startmenu();
    choose_Mode();
    return 0;
}


//
void findway(vector<vector<dot> > const chess, vector<dot>& store)
{
    int m, n;
    int find_color = colorset;//该下什么颜色的棋
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chess[i][j].color == find_color) {
                //向上找；
                m = i;
                n = j;
                if (m > 0 && chess[--m][n].color == (-1) * find_color) {//紧邻不同颜色棋子
                    while (m >= 0 && chess[m][n].color == (-1) * find_color) {
                        m--;
                    }
                    if (m >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);//找到可以落子点并存放
                }
                //向下找
                m = i;
                n = j;
                if (m < 7 && chess[++m][n].color == (-1) * find_color) {
                    while (m <= 7 && chess[m][n].color == (-1) * find_color) {
                        m++;
                    }
                    if (m <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //向左找
                m = i;
                n = j;
                if (n > 0 && chess[m][--n].color == (-1) * find_color) {
                    while (n >= 0 && chess[m][n].color == (-1) * find_color) {
                        n--;
                    }
                    if (n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //向右找
                m = i;
                n = j;
                if (n < 7 && chess[m][++n].color == (-1) * find_color) {
                    while (n <= 7 && chess[m][n].color == (-1) * find_color) {
                        n++;
                    }
                    if (n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //向左上找
                m = i;
                n = j;
                if (m > 0 && n > 0 && chess[--m][--n].color == (-1) * find_color) {
                    while (m >= 0 && n >= 0 && chess[m][n].color == (-1) * find_color) {
                        m--;
                        n--;
                    }
                    if (m >= 0 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //向左下找
                m = i;
                n = j;
                if (m < 7 && n>0 && chess[++m][--n].color == (-1) * find_color) {
                    while (m <= 7 && n >= 0 && chess[m][n].color == (-1) * find_color) {
                        m++;
                        n--;
                    }
                    if (m <= 7 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //向右上找
                m = i;
                n = j;
                if (m > 0 && n < 7 && chess[--m][++n].color == (-1) * find_color) {
                    while (m >= 0 && n <= 7 && chess[m][n].color == (-1) * find_color) {
                        m--;
                        n++;
                    }
                    if (m >= 0 && n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //向右下找
                m = i;
                n = j;
                if (m < 7 && n < 7 && chess[++m][++n].color == (-1) * find_color) {
                    while (m <= 7 && n <= 7 && chess[m][n].color == (-1) * find_color) {
                        m++;
                        n++;
                    }
                    if (m <= 7 && n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
            }
        }
    }

    if (store.size() > 0) {
        flag1 = 0;
        flag2 = 0;
        sort(store.begin(), store.end());
        store.erase(unique(store.begin(), store.end()), store.end());
    }
    else {
        if (colorset == 1) flag1 = 1;
        else if (colorset == -1) flag2 = 1;
        if (flag1 * flag2 != 1) {
            colorset = -colorset;
            findway(chess,store);
        }
        
    }
}
//翻转棋子
void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board) {
    try {
        if (d1.color != d2.color || d1.color * d2.color == 0) {//如果两边棋子不一样，抛出
            throw(1);
        }
        else {
            if (d1.x == d2.x && abs(d1.y - d2.y)>1) {//上下
                int step = d1.y < d2.y ? 1 : -1;
                for (int i = d1.y + step; i != d2.y; i += step) {//翻转
                    chess_board[d1.x][i].set_color(d1.color);
                }
            }
            else if (d1.y == d2.y && abs(d1.x - d2.x)>1) {//左右
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step; i != d2.x; i += step) {
                    chess_board[i][d1.y].set_color(d1.color);
                }
            }
            else if (abs(d1.x - d2.x)>1 && d1.x - d2.x == d1.y - d2.y) {
                //左上右下
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step, j = d1.y + step; i != d2.x; i += step, j += step) {
                    chess_board[i][j].set_color(d1.color);
                }
            }
            else if (abs(d1.x - d2.x)>1 && d2.x - d1.x == d1.y - d2.y) {
                //左下右上
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step, j = d1.y - step; i != d2.x; i += step, j -= step) {
                    chess_board[i][j].set_color(d1.color);
                }
            }
            else {
            }
        }

    }

    catch (...) {
        cout << "Error in flip_between_two_dot." << endl;
    }
}
//落子
void game(int x, int y, int color, vector<vector<dot>>& chess_board) {//传递下棋位置和棋盘
  
    //左
    int i = 1;
    while (x - i >= 0 && chess_board[x - i][y].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && chess_board[x - i][y].color == color) {//找到同色棋子
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);//翻转
    }
    //右
    i = -1;
    while (x - i < 8 && chess_board[x - i][y].color == color * -1) {
        i--;
    }
    if (x - i < 8 && chess_board[x - i][y].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
    }
    //上
    i = 1;
    while (y - i >= 0 && chess_board[x][y - i].color == color * -1) {
        i++;
    }
    if (y - i >= 0 && chess_board[x][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //下
    i = -1;
    while (y - i < 8 && chess_board[x][y - i].color == color * -1) {
        i--;
    }
    if (y - i < 8 && chess_board[x][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //左上
    i = 1;
    while (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //右下
    i = -1;
    while (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color * -1) {
        i--;
    }
    if (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //左下
    i = 1;
    while (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color * -1) {
        i++;
    }
    if (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
    //右上
    i = -1;
    while (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color * -1) {
        i--;
    }
    if (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
}
//游戏结算
void judgeGame() {
    cleardevice();
    int k;
    if (num_black > num_white) {
        outtextxy(0, 0, _T("BLACK　WIN!")); 
        cout << '\a';
        //k=_getch();
        //exit(0);
    }
    if (num_white > num_black) {
        outtextxy(0, 0, _T("WHITE　WIN!"));
        cout << '\a';
        //k=_getch();
        //exit(0);
    }
    if (num_white == num_black) {
        outtextxy(0, 0, _T("TIE"));
        cout << '\a';
        //k=_getch();
        //exit(0);
    }
    PlaySound(L"win.wav", NULL, SND_FILENAME | SND_ASYNC);
    setfillcolor(RGB(200, 200, 150));
    solidrectangle(80, 80, 240, 160);
    solidrectangle(320,80,480,160);
    outtextxy(145, 113, _T("Quit"));
    outtextxy(366, 113, _T("Play again"));
    while(1) {
        if(MouseHit()){
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                Mouse_x = m.x;
                Mouse_y = m.y;
                if (Mouse_x >= 80 && Mouse_x <= 240 && Mouse_y >= 80 && Mouse_y <= 160) {
                    exit(0);
                }
                else if (Mouse_x >= 320 && Mouse_x <= 480 && Mouse_y >= 80 && Mouse_y <= 160) {
                    choose_Mode();
                    break;
                }
            }
        }
    }
}
//判断游戏是否结束
bool is_End() {
    //情况1:下满棋子时
    if (num_black + num_white == 64) return true;
    //情况2:双方无棋子可下时
    else if (flag1 == 1 && flag2 == 1) return true; 
    else return false;

}
//数数
int game_count(int x, int y, int color, vector<vector<dot>>& chess_board) {//传递棋子和棋盘
    //chess_board[x][y].set_color(color);
    count_dot = 0;
    //左
    int i = 1;
    while (x - i >= 0 && chess_board[x - i][y].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && chess_board[x - i][y].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
    }
    //右
    i = -1;
    while (x - i < 8 && chess_board[x - i][y].color == color * -1) {
        i--;
    }
    if (x - i < 8 && chess_board[x - i][y].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
    }
    //上
    i = 1;
    while (y - i >= 0 && chess_board[x][y - i].color == color * -1) {
        i++;
    }
    if (y - i >= 0 && chess_board[x][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //下
    i = -1;
    while (y - i < 8 && chess_board[x][y - i].color == color * -1) {
        i--;
    }
    if (y - i < 8 && chess_board[x][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //左上
    i = 1;
    while (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //右下
    i = -1;
    while (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color * -1) {
        i--;
    }
    if (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //左下
    i = 1;
    while (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color * -1) {
        i++;
    }
    if (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
    //右上
    i = -1;
    while (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color * -1) {
        i--;
    }
    if (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
    return count_dot;
}
//AI落子
void computer_think() {
    int max_count = 0;
    int max_count_index = 0;//在available中的坐下标
    for (int i = 0; i < available.size();i++) {
        int present_count = game_count(available[i].x, available[i].y, 1, chess_board);
            if (present_count > max_count) {
                max_count = present_count;
                max_count_index = i;
        }
    }
    //找到最大的之后，电脑下棋
    chess_board[available[max_count_index].x][available[max_count_index].y].set_color(1);//在该点下棋
    game(available[max_count_index].x, available[max_count_index].y, 1, chess_board);//翻转
    colorset = -1;//换边
}
//计录一个棋子能翻转的总数
void count_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board) {
    try {
        if (0) {//不执行
            throw(1);
        }
        else {
            if (d1.x == d2.x && abs(d1.y - d2.y) > 1) {
                int step = d1.y < d2.y ? 1 : -1;
                for (int i = d1.y + step; i != d2.y; i += step) {
                    count_dot++;
                }
            }
            else if (d1.y == d2.y && abs(d1.x - d2.x) > 1) {
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step; i != d2.x; i += step) {
                    count_dot++;
                }
            }
            else if (abs(d1.x - d2.x) > 1 && d1.x - d2.x == d1.y - d2.y) {
                //左上右下
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step, j = d1.y + step; i != d2.x; i += step, j += step) {
                    count_dot++;
                }
            }
            else if (abs(d1.x - d2.x) > 1 && d2.x - d1.x == d1.y - d2.y) {
                //左下右上
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step, j = d1.y - step; i != d2.x; i += step, j -= step) {
                    count_dot++;
                }
            }
            else {
            }
        }

    }

    catch (...) {
        cout << "Error in flip_between_two_dot." << endl;
    }
}
void updateWithInput_single() {
    if (colorset == -1) {//如果玩家下
        //实现鼠标点击以翻转
        if (MouseHit()) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                Mouse_x = m.x;//取得鼠标点击位置
                Mouse_y = m.y;
                int a, b, flag = 0;

                for (a = 0; a < 8; a++) {
                    for (b = 0; b < 8; b++) {
                        if (a >= (Mouse_x / steps) && b >= (Mouse_y / steps)) {//确定在数组中位置
                            Mouse_x = a;
                            Mouse_y = b;
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1)//是否找到
                        break;
                }//locate

                //这里和双人模式不同，只会记录玩家的棋盘
                //悔棋
                if (colorset == -1){//点击了“悔棋”
                    if (Mouse_y >= 405 && Mouse_y <= 465 && Mouse_x >= 790 && Mouse_x <= 900) {
                        resetf();//重置棋盘
                        //colorset *= -1;
                        cout << "\a";
                    }
                
                setf();//保存棋盘
            }


                if (ok_to_draw(Mouse_x, Mouse_y)) {
                    chess_board[a][b].set_color(colorset);//在该点下棋
                    PlaySound(L"chess.wav", NULL, SND_FILENAME | SND_ASYNC);

                    game(Mouse_x, Mouse_y, colorset, chess_board);//落子翻转
                    colorset = -colorset;//换方
                }
                //重来一局
                if (Mouse_y >= 245 && Mouse_y <= 305 && Mouse_x >= 790 && Mouse_x <= 900) {
                    startup();
                }
                //暂停
                if (Mouse_y >= 325 && Mouse_y <= 385 && Mouse_x >= 790 && Mouse_x <= 900) {
                    outtextxy(780, 390, _T("按回车键返回游戏"));
                    system("pause");
                    outtextxy(780, 390, _T("                                 "));
                }
                for (int i = 0; i < 8; i++) {//清空每个棋位
                    for (int j = 0; j < 8; j++) {
                        reminderclear(chess_board[i][j]);
                    }
                }
            }
        }
    }
    else {
        computer_think();//电脑下棋
    }

}
void updateWithoutInput_single() {
    num_black = 0;
    num_white = 0;//计数器清零
    available.clear();//记录能下的位置的动态数组清空
    sumup(chess_board);//计数
    findway(chess_board, available);//将可以下棋的点存放进available中

    if (colorset == -1 && available.empty() == 1) {//是否无棋子可下
        flag1 == 1;
    }
    else if (colorset == 1 && available.empty() == 1) {
        flag2 == 1;
    }

    if (is_End()) {//判断是否结束
        judgeGame();
    }
    else {
        flag1 = flag2 = 0;
    }

}
void show_single() {

    HideCursor();//隐藏光标
    gotoxy(0, 0);
    BeginBatchDraw();//实现批量绘图

    if (colorset == -1) {//如果玩家下
        for (int i = 0; i < available.size(); i++) {
            highlight(available[i]);//让可以下棋位置显示高光
        }
    }

    for (int i = 0; i < 8; i++) {//显示棋盘
        for (int j = 0; j < 8; j++) {
            drawChess(chess_board[i][j]);
        }
    }

    if (colorset == 1) {
        outtextxy(800, 200, _T("WHITE　Goes"));
    }
    else {
        outtextxy(800, 200, _T("BLACK　Goes"));
    }

    TCHAR s[50];//存储文字
    _stprintf_s(s, _T("black: %d    white: %d   empty:%d  "), num_black, num_white, 64 - num_white - num_black);
    outtextxy(750, 100, s);

    EndBatchDraw();//将之前的绘图操作一并显示
}
void single_mode() {
    startup();
    while (1) {
        show_single();//单人模式展示
        
        if (colorset == 1) { //如果是电脑下，就假装思考一下
            Sleep(300);
        }
        updateWithInput_single();
        updateWithoutInput_single();

    }
}

