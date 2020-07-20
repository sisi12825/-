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

//dot���¼���ӵ�״̬
class dot {
public:

    //���ӵ���Ϣ:�������ɫ
    int x = 0, y = 0, color = 0;

    //���캯��
    dot(int x, int y, int color) :x(x), y(y), color(color) {}

    //���ظ�ֵ�����
    dot& operator = (const dot& other) {
        this->x = other.x;
        this->y = other.y;
        this->color = other.color;
        return *this;
    }

    //��������ת
    void flip() {
        color *= -1;
    }

   
    //��Ԫ���� ʵ�ַ�ת dot1 �� dot2 ������֮������ӵķ�ת����Ҫ��������
    friend void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board);

    //��Ԫ���� ʵ���ҵ� �µ����� �İ˸���������Ҫ��ת�Ĳ�ͬ��ɫ������յ����ӵ�λ�ã�
    //�����ݸ�flip_between_two_dot���� ʵ�ַ�ת
    friend void game(int x, int y, int color, vector<vector<dot>>& chess_board);

    //�ı�����������ɫΪ�ض���ɫ�������ڿհ�λ������
    void set_color(int new_color) {
        color = new_color;
    }
 
    //���رȽ��������Ϊ��֮���ȥ��
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
    //Ѱ����һ������
    friend void findway(vector<vector<dot> > const chess, vector<dot>& store);

    friend void drawChess(const dot& chessdot);
    
    friend void highlight(dot& chessdot);

    friend void reminderclear(dot& chessdot);


    friend void sumup(vector<vector<dot> > const chess);

};

MOUSEMSG m;//���Ĳ�������
int Mouse_x, Mouse_y;//����λ��
int steps = 80;
int reset = 0;
int win = 0;//����ʤ��Ϊ0������ʤ��Ϊ1
static vector<vector<dot>> chess_board(8, vector<dot>(8, dot(0, 0, 0)));
static vector<vector<dot>> last_chess_board(8, vector<dot>(8, dot(0, 0, 0)));
static vector<dot> available;//����洢���µ����������أ���һά��̬����洢
int num_black, num_white;//�����������
static int flag1 = 0, flag2 = 0;//��־�Ƿ��������
int colorset = -1;//��������
int state = 1;

int count_dot = 0;//��¼ÿ�ֵ���

int Set_state() {
    return (-1) * state;
}

//ȫ�ֺ���

//�ж�ʤ��
void judgeGame();
//�ж��Ƿ����
bool is_End();
//˫��ģʽ
void double_Mode();
//ģʽѡ�����
void draw_choose_Mode();
//ģʽѡ����
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
    setlinestyle(PS_SOLID, 3);//ѡ����
    setcolor(RGB(0, 0, 0));
    for (int i = 0; i <= 8; i++) {
        line(i * steps, 0, i * steps, 8 * steps);//��
        line(0, i * steps, 8 * steps, i * steps);//��
    }
    setfillcolor(RGB(200,200,150));
    solidrectangle(790,245,900,305);
    solidrectangle(790,325,900,385);
    solidrectangle(790,405,900,465);
    outtextxy(814, 267, _T("����һ��"));
    outtextxy(814, 347, _T("��ͣ��Ϸ"));
    outtextxy(830, 427, _T("����"));

}

void drawChess(dot& chessdot) {
    int RealChess_x = (chessdot.x + 0.5) * steps;//�������ӵ���Ӧ��ʵλ��
    int RealChess_y = (chessdot.y + 0.5) * steps;//ͬ��
    if (chessdot.color == -1) {
        setfillcolor(BLACK);
        solidcircle(RealChess_x, RealChess_y, 35);//Բ�����ꡢ�뾶
    }//��Ϊ����
    else if (chessdot.color == 1) {
        setfillcolor(WHITE);
        solidcircle(RealChess_x, RealChess_y, 35);
    }//��Ϊ����
}

void highlight(dot& chessdot) {
    int RealChess_x = (chessdot.x + 0.5) * steps;//�������ӵ���Ӧ��ʵλ��
    int RealChess_y = (chessdot.y + 0.5) * steps;//ͬ��
    setfillcolor(RGB(220, 220, 190));
    solidcircle(RealChess_x, RealChess_y, 35);
    
}
//������Ѹ߹�
void reminderclear(dot& chessdot) {
    int RealChess_x = (chessdot.x + 0.5) * steps;//�������ӵ���Ӧ��ʵλ��
    int RealChess_y = (chessdot.y + 0.5) * steps;//ͬ��
    clearrectangle(RealChess_y - 36, RealChess_x - 36, RealChess_y + 36, RealChess_x + 36);//����ÿ����λ
}
//��ʼ�����̣��м��ĸ�����Ϊ����ĺڰ�
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
//����˸��ˢ��
void gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(handle, pos);
}
//���ع��
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//��ʼ�˵�
void startmenu() {
    PlaySound(L"music.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);//�ű�������
  

    initgraph(800, 1000);
    IMAGE img;
    loadimage(&img, L"123.jpg", 0, 0);
    putimage(0, 0, &img);

    while (1) {
        m = GetMouseMsg();//��ȡ�����Ϣ
        if (m.uMsg == WM_LBUTTONDOWN) {//����Ƿ�����Ϣ
            break;
        }
    }
    closegraph();

    PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
//��ʼ��
void startup() {
    colorset = -1;//��������
    drawChessboard();//������
    initialset();
}

//��ӡ���Ӻ͸߹�����
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
        outtextxy(800,200, _T("WHITE��Goes"));
    }
    else {
        outtextxy(800,200, _T("BLACK��Goes"));
    }

    TCHAR s[50];
    _stprintf_s(s, _T("black: %d    white: %d   empty:%d  "), num_black, num_white, 64 - num_white - num_black);
    outtextxy(750, 100, s);

    EndBatchDraw();
}
//ͳ�ƺ���Ͱ��������
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
//�жϸ�λ���Ƿ��������
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
//������������һ��
void resetf() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            chess_board[i][j] = last_chess_board[i][j];
        }
    }
}
//�������̵�״̬
void setf() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            last_chess_board[i][j] = chess_board[i][j];
        }
    }
}
//���������
void updateWithoutInput() {
    num_black = 0;
    num_white = 0;//����������
    available.clear();//��¼���µ�λ�õĶ�̬�������
    sumup(chess_board);
    findway(chess_board, available);
    if (colorset == -1 && available.empty() == 1) {//�Ƿ������ӿ���
        flag1 == 1;
    }
    else if (colorset == 1 && available.empty() == 1) {
        flag2 == 1;
    }

    if (is_End()) {//�ж��Ƿ����
        judgeGame();
    }
    else {
        flag1 = flag2 = 0;
    }
}
//���������
void updateWithInput() {
    //ʵ��������Է�ת
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

            //����
            if (Mouse_y >= 405 && Mouse_y <= 465 && Mouse_x >= 790 && Mouse_x <= 900) {
                resetf();
                colorset = -colorset;
                cout << "\a";
            }
            setf();//��������

            if (ok_to_draw(Mouse_x, Mouse_y)) {
                chess_board[a][b].set_color(colorset);//�ڸõ�����
                PlaySound(L"chess.wav", NULL, SND_FILENAME | SND_ASYNC);
              
                game(Mouse_x, Mouse_y, colorset, chess_board);
                colorset = -colorset;//����
            }
            //����һ��
            if (Mouse_y >= 245 && Mouse_y <= 305 && Mouse_x >= 790 && Mouse_x <= 900) {
                startup();
            }
            //��ͣ
            if (Mouse_y >= 325 && Mouse_y <= 385 && Mouse_x >= 790 && Mouse_x <= 900) {
                outtextxy(780, 390, _T("���س���������Ϸ"));
                system("pause");
                outtextxy(780, 390, _T("                                 "));
            }
            for (int i = 0; i < 8; i++) {//������ͼЧ��
                for (int j = 0; j < 8; j++) {
                    reminderclear(chess_board[i][j]);
                }
            }
        }
    }
}
//��ʾѡ��ģʽ����
void draw_choose_Mode()
{
    initgraph(480, 480);
    setbkcolor(RGB(200, 200, 169));//������ɫ
    cleardevice();
    setfillcolor(RGB(200, 200, 150));//������
    solidrectangle(160, 120, 320, 200);
    solidrectangle(160, 280, 320, 360);
    setcolor(RGB(0, 0, 0));//����ɫ
    outtextxy(207, 150, _T("����ģʽ"));
    outtextxy(207, 310, _T("˫��ģʽ"));
}
//ѡ��ģʽ����
void choose_Mode() {
    draw_choose_Mode();//��ʾ����
    while (1) {
        if (MouseHit()) {//�ж��Ƿ���
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {//��ȡ������µ�λ��
                Mouse_x = m.x;
                Mouse_y = m.y;
            }//ѡ����ģʽ
            if(Mouse_y >= 120 && Mouse_y <= 200 && Mouse_x >= 160 && Mouse_x <= 320){
                single_mode();
                closegraph();
            }
        
            //ѡ��˫��ģʽ
            if (Mouse_y >= 280 && Mouse_y <= 360 && Mouse_x >= 160 && Mouse_x <= 320) {
                closegraph();
                double_Mode();
            }
        }
    }
}
//˫��ģʽ
void double_Mode() {
    startup();//���̵ĳ�ʼ������ʼ�Ľ��棩
    while (1) {
        show();//ÿһ��չʾ�����ӵľ���״�������ӵıߣ����ӵ�����
        updateWithInput();//ֻ�����������λ�õķ�ת
        updateWithoutInput();//��ת���ж�������ʣ������Ӽ��㣬ʤ����������exit��0�����������Ľ���
    }
}


//������
int main() {
    startmenu();
    choose_Mode();
    return 0;
}


//
void findway(vector<vector<dot> > const chess, vector<dot>& store)
{
    int m, n;
    int find_color = colorset;//����ʲô��ɫ����
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (chess[i][j].color == find_color) {
                //�����ң�
                m = i;
                n = j;
                if (m > 0 && chess[--m][n].color == (-1) * find_color) {//���ڲ�ͬ��ɫ����
                    while (m >= 0 && chess[m][n].color == (-1) * find_color) {
                        m--;
                    }
                    if (m >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);//�ҵ��������ӵ㲢���
                }
                //������
                m = i;
                n = j;
                if (m < 7 && chess[++m][n].color == (-1) * find_color) {
                    while (m <= 7 && chess[m][n].color == (-1) * find_color) {
                        m++;
                    }
                    if (m <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //������
                m = i;
                n = j;
                if (n > 0 && chess[m][--n].color == (-1) * find_color) {
                    while (n >= 0 && chess[m][n].color == (-1) * find_color) {
                        n--;
                    }
                    if (n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //������
                m = i;
                n = j;
                if (n < 7 && chess[m][++n].color == (-1) * find_color) {
                    while (n <= 7 && chess[m][n].color == (-1) * find_color) {
                        n++;
                    }
                    if (n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //��������
                m = i;
                n = j;
                if (m > 0 && n > 0 && chess[--m][--n].color == (-1) * find_color) {
                    while (m >= 0 && n >= 0 && chess[m][n].color == (-1) * find_color) {
                        m--;
                        n--;
                    }
                    if (m >= 0 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //��������
                m = i;
                n = j;
                if (m < 7 && n>0 && chess[++m][--n].color == (-1) * find_color) {
                    while (m <= 7 && n >= 0 && chess[m][n].color == (-1) * find_color) {
                        m++;
                        n--;
                    }
                    if (m <= 7 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //��������
                m = i;
                n = j;
                if (m > 0 && n < 7 && chess[--m][++n].color == (-1) * find_color) {
                    while (m >= 0 && n <= 7 && chess[m][n].color == (-1) * find_color) {
                        m--;
                        n++;
                    }
                    if (m >= 0 && n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
                }
                //��������
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
//��ת����
void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board) {
    try {
        if (d1.color != d2.color || d1.color * d2.color == 0) {//����������Ӳ�һ�����׳�
            throw(1);
        }
        else {
            if (d1.x == d2.x && abs(d1.y - d2.y)>1) {//����
                int step = d1.y < d2.y ? 1 : -1;
                for (int i = d1.y + step; i != d2.y; i += step) {//��ת
                    chess_board[d1.x][i].set_color(d1.color);
                }
            }
            else if (d1.y == d2.y && abs(d1.x - d2.x)>1) {//����
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step; i != d2.x; i += step) {
                    chess_board[i][d1.y].set_color(d1.color);
                }
            }
            else if (abs(d1.x - d2.x)>1 && d1.x - d2.x == d1.y - d2.y) {
                //��������
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step, j = d1.y + step; i != d2.x; i += step, j += step) {
                    chess_board[i][j].set_color(d1.color);
                }
            }
            else if (abs(d1.x - d2.x)>1 && d2.x - d1.x == d1.y - d2.y) {
                //��������
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
//����
void game(int x, int y, int color, vector<vector<dot>>& chess_board) {//��������λ�ú�����
  
    //��
    int i = 1;
    while (x - i >= 0 && chess_board[x - i][y].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && chess_board[x - i][y].color == color) {//�ҵ�ͬɫ����
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);//��ת
    }
    //��
    i = -1;
    while (x - i < 8 && chess_board[x - i][y].color == color * -1) {
        i--;
    }
    if (x - i < 8 && chess_board[x - i][y].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
    }
    //��
    i = 1;
    while (y - i >= 0 && chess_board[x][y - i].color == color * -1) {
        i++;
    }
    if (y - i >= 0 && chess_board[x][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //��
    i = -1;
    while (y - i < 8 && chess_board[x][y - i].color == color * -1) {
        i--;
    }
    if (y - i < 8 && chess_board[x][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //����
    i = 1;
    while (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //����
    i = -1;
    while (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color * -1) {
        i--;
    }
    if (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //����
    i = 1;
    while (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color * -1) {
        i++;
    }
    if (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
    //����
    i = -1;
    while (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color * -1) {
        i--;
    }
    if (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color) {
        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
}
//��Ϸ����
void judgeGame() {
    cleardevice();
    int k;
    if (num_black > num_white) {
        outtextxy(0, 0, _T("BLACK��WIN!")); 
        cout << '\a';
        //k=_getch();
        //exit(0);
    }
    if (num_white > num_black) {
        outtextxy(0, 0, _T("WHITE��WIN!"));
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
//�ж���Ϸ�Ƿ����
bool is_End() {
    //���1:��������ʱ
    if (num_black + num_white == 64) return true;
    //���2:˫�������ӿ���ʱ
    else if (flag1 == 1 && flag2 == 1) return true; 
    else return false;

}
//����
int game_count(int x, int y, int color, vector<vector<dot>>& chess_board) {//�������Ӻ�����
    //chess_board[x][y].set_color(color);
    count_dot = 0;
    //��
    int i = 1;
    while (x - i >= 0 && chess_board[x - i][y].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && chess_board[x - i][y].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
    }
    //��
    i = -1;
    while (x - i < 8 && chess_board[x - i][y].color == color * -1) {
        i--;
    }
    if (x - i < 8 && chess_board[x - i][y].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
    }
    //��
    i = 1;
    while (y - i >= 0 && chess_board[x][y - i].color == color * -1) {
        i++;
    }
    if (y - i >= 0 && chess_board[x][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //��
    i = -1;
    while (y - i < 8 && chess_board[x][y - i].color == color * -1) {
        i--;
    }
    if (y - i < 8 && chess_board[x][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
    }
    //����
    i = 1;
    while (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color * -1) {
        i++;
    }
    if (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //����
    i = -1;
    while (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color * -1) {
        i--;
    }
    if (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
    }
    //����
    i = 1;
    while (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color * -1) {
        i++;
    }
    if (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
    //����
    i = -1;
    while (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color * -1) {
        i--;
    }
    if (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color) {
        count_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
    }
    return count_dot;
}
//AI����
void computer_think() {
    int max_count = 0;
    int max_count_index = 0;//��available�е����±�
    for (int i = 0; i < available.size();i++) {
        int present_count = game_count(available[i].x, available[i].y, 1, chess_board);
            if (present_count > max_count) {
                max_count = present_count;
                max_count_index = i;
        }
    }
    //�ҵ�����֮�󣬵�������
    chess_board[available[max_count_index].x][available[max_count_index].y].set_color(1);//�ڸõ�����
    game(available[max_count_index].x, available[max_count_index].y, 1, chess_board);//��ת
    colorset = -1;//����
}
//��¼һ�������ܷ�ת������
void count_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board) {
    try {
        if (0) {//��ִ��
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
                //��������
                int step = d1.x < d2.x ? 1 : -1;
                for (int i = d1.x + step, j = d1.y + step; i != d2.x; i += step, j += step) {
                    count_dot++;
                }
            }
            else if (abs(d1.x - d2.x) > 1 && d2.x - d1.x == d1.y - d2.y) {
                //��������
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
    if (colorset == -1) {//��������
        //ʵ��������Է�ת
        if (MouseHit()) {
            m = GetMouseMsg();
            if (m.uMsg == WM_LBUTTONDOWN) {
                Mouse_x = m.x;//ȡ�������λ��
                Mouse_y = m.y;
                int a, b, flag = 0;

                for (a = 0; a < 8; a++) {
                    for (b = 0; b < 8; b++) {
                        if (a >= (Mouse_x / steps) && b >= (Mouse_y / steps)) {//ȷ����������λ��
                            Mouse_x = a;
                            Mouse_y = b;
                            flag = 1;
                            break;
                        }
                    }
                    if (flag == 1)//�Ƿ��ҵ�
                        break;
                }//locate

                //�����˫��ģʽ��ͬ��ֻ���¼��ҵ�����
                //����
                if (colorset == -1){//����ˡ����塱
                    if (Mouse_y >= 405 && Mouse_y <= 465 && Mouse_x >= 790 && Mouse_x <= 900) {
                        resetf();//��������
                        //colorset *= -1;
                        cout << "\a";
                    }
                
                setf();//��������
            }


                if (ok_to_draw(Mouse_x, Mouse_y)) {
                    chess_board[a][b].set_color(colorset);//�ڸõ�����
                    PlaySound(L"chess.wav", NULL, SND_FILENAME | SND_ASYNC);

                    game(Mouse_x, Mouse_y, colorset, chess_board);//���ӷ�ת
                    colorset = -colorset;//����
                }
                //����һ��
                if (Mouse_y >= 245 && Mouse_y <= 305 && Mouse_x >= 790 && Mouse_x <= 900) {
                    startup();
                }
                //��ͣ
                if (Mouse_y >= 325 && Mouse_y <= 385 && Mouse_x >= 790 && Mouse_x <= 900) {
                    outtextxy(780, 390, _T("���س���������Ϸ"));
                    system("pause");
                    outtextxy(780, 390, _T("                                 "));
                }
                for (int i = 0; i < 8; i++) {//���ÿ����λ
                    for (int j = 0; j < 8; j++) {
                        reminderclear(chess_board[i][j]);
                    }
                }
            }
        }
    }
    else {
        computer_think();//��������
    }

}
void updateWithoutInput_single() {
    num_black = 0;
    num_white = 0;//����������
    available.clear();//��¼���µ�λ�õĶ�̬�������
    sumup(chess_board);//����
    findway(chess_board, available);//����������ĵ��Ž�available��

    if (colorset == -1 && available.empty() == 1) {//�Ƿ������ӿ���
        flag1 == 1;
    }
    else if (colorset == 1 && available.empty() == 1) {
        flag2 == 1;
    }

    if (is_End()) {//�ж��Ƿ����
        judgeGame();
    }
    else {
        flag1 = flag2 = 0;
    }

}
void show_single() {

    HideCursor();//���ع��
    gotoxy(0, 0);
    BeginBatchDraw();//ʵ��������ͼ

    if (colorset == -1) {//��������
        for (int i = 0; i < available.size(); i++) {
            highlight(available[i]);//�ÿ�������λ����ʾ�߹�
        }
    }

    for (int i = 0; i < 8; i++) {//��ʾ����
        for (int j = 0; j < 8; j++) {
            drawChess(chess_board[i][j]);
        }
    }

    if (colorset == 1) {
        outtextxy(800, 200, _T("WHITE��Goes"));
    }
    else {
        outtextxy(800, 200, _T("BLACK��Goes"));
    }

    TCHAR s[50];//�洢����
    _stprintf_s(s, _T("black: %d    white: %d   empty:%d  "), num_black, num_white, 64 - num_white - num_black);
    outtextxy(750, 100, s);

    EndBatchDraw();//��֮ǰ�Ļ�ͼ����һ����ʾ
}
void single_mode() {
    startup();
    while (1) {
        show_single();//����ģʽչʾ
        
        if (colorset == 1) { //����ǵ����£��ͼ�װ˼��һ��
            Sleep(300);
        }
        updateWithInput_single();
        updateWithoutInput_single();

    }
}

