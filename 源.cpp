//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//int state = 1;
//int Set_state() {
//    return (-1) * state;
//}
//class dot {
//public:
//
//    //棋子的信息
//    int x = 0, y = 0, color = 0;
//
//    //构造函数
//    dot(int x, int y, int color) :x(x), y(y), color(color) {}
//
//    //重载赋值运算符
//    dot& operator = (const dot& other) {
//        this->x = other.x;
//        this->y = other.y;
//        this->color = other.color;
//        return *this;
//    }
//
//    //棋子自身翻转
//    void flip() {
//        color *= -1;
//    }
//
//    //打印棋子本身的坐标和颜色
//    void print() {
//        //        string color_string = color==1?"white)":(color?"black)":"empty)");
//        string color_string = color == 1 ? "■)" : (color ? "□)" : " )");
//
//        cout << "(" << x << "," << y << "," << color_string;
//    }
//
//    //友元函数 实现翻转 dot1 和 dot2 两棋子之间的棋子的翻转，需要传递棋盘
//    friend void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board);
//
//    //友元函数 实现找到 新的棋子 的八个方向上需要翻转的不同颜色的棋的终点棋子的位置，
//    //并传递给flip_between_two_dot函数 实现翻转
//    friend void game(int x, int y, int color, vector<vector<dot>>& chess_board);
//
//    //改变棋子自身颜色为特定颜色，用于在空白位置下棋
//    void set_color(int new_color) {
//        color = new_color;
//    }
//    //
//
//
//    //重载比较运算符，为了之后的去重
//    bool operator == (const dot& other)const {
//        if (this->x == other.x && this->y == other.y) return true;
//        else return false;
//    }
//    bool operator < (const dot& other)const {
//        if (this->x < other.x || (this->x == other.x && this->y < other.y)) return true;
//        else return false;
//    }
//    bool operator > (const dot& other)const {
//        if (this->x > other.x || (this->x == other.x && this->y > other.y)) return true;
//        else return false;
//    }
//    //
//    friend bool findway(vector<vector<dot> > const chess, vector<dot>& store);
//
//
//};
////全局函数
//void Show_present_chess_and_way(vector<vector<dot>>& chess);//vector<vector<dot>>
////    friend void game(int x, int y, int color, vector<vector<dot>>& chess_board);
//
//int main() {
//    // initialization
//    // declare chess_board to store the information of chess's location and color
//    static vector<vector<dot>> chess_board(8, vector<dot>(8, dot(0, 0, 0)));
//    // initialize the chess_board
//    //中间四个棋子 交叉的黑白
//    for (int i = 0;i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            if ((i == 3 && j == 3) || (i == 4 && j == 4)) {
//                chess_board[i][j] = dot(i, j, 1);
//            }
//            else if ((i == 3 && j == 4) || (i == 4 && j == 3)) {
//                chess_board[i][j] = dot(i, j, -1);
//            }
//            else {
//                chess_board[i][j] = dot(i, j, 0);
//            }
//            chess_board[i][j].print();
//        }
//        cout << endl;
//    }
//
//    //new chess's location and color
//    int x = 3, y = 5, color = 1;
//    chess_board[x][y].set_color(color);
//
//    //test of game
//    game(x, y, color, chess_board);
//
//    cout << endl;
//    for (int i = 0;i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            chess_board[i][j].print();
//        }
//        cout << endl;
//    }
//    //
//    chess_board[3][5] = dot(3, 5, 0);
//    chess_board[4][3] = dot(4, 3, 0);
//    chess_board[3][4] = dot(3, 4, 0);
//    chess_board[4][4] = dot(4, 4, 0);
//    chess_board[3][3] = dot(3, 3, 0);
//    //
//    chess_board[1][1] = dot(1, 1, -1);
//    chess_board[1][2] = dot(1, 2, -1);
//    chess_board[1][3] = dot(1, 3, -1);
//    chess_board[2][1] = dot(2, 1, 1);
//    chess_board[2][2] = dot(2, 2, 1);
//    chess_board[2][3] = dot(2, 3, -1);
//    //
//    Show_present_chess_and_way(chess_board);
//}
////
//void Show_present_chess_and_way(vector<vector<dot>>& chess) {
//    vector<dot> available;//定义存储能下的棋子所处地
//    int i, j;
//    cout << "此时棋盘为：" << endl;
//    for (i = 0; i < 8; i++) {
//        for (j = 0; j < 8; j++) {
//            chess[i][j].print();
//        }
//        cout << endl;
//    }
//    if (state == -1) {
//        cout << "此时白棋能下的个数和地方分别为：" << endl;
//        if (findway(chess, available)) {
//            cout << available.size() << endl;
//            for (int num = 0; num < available.size(); num++) available[num].print();
//            cout << endl;
//        }
//        else cout << "没有能下棋的地方" << endl;
//    }
//    else {
//        cout << "此时黑棋能下的个数和地方分别为：" << endl;
//        if (findway(chess, available)) {
//            cout << available.size() << endl;
//            for (int num = 0; num < available.size(); num++) available[num].print();
//            cout << endl;
//        }
//        else cout << "没有能下棋的地方" << endl;
//    }
//}
//
//
//void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board) {
//    try {
//        if (d1.color != d2.color || d1.color * d2.color == 0) {
//            throw;
//        }
//        else {
//            if (d1.x == d2.x && d1.y != d2.y) {
//                int step = d1.y < d2.y ? 1 : -1;
//                for (int i = d1.y + step;i != d2.y;i += step) {
//                    chess_board[d1.x][i].flip();
//                }
//            }
//            else if (d1.y == d2.y && d1.x != d2.x) {
//                int step = d1.x < d2.x ? 1 : -1;
//                for (int i = d1.x + step; i != d2.x;i += step) {
//                    chess_board[i][d1.y].flip();
//                }
//            }
//            else if (d1.x != d2.x && d1.x - d2.x == d1.y - d2.y) {
//                //左上右下
//                int step = d1.x < d2.x ? 1 : -1;
//                for (int i = d1.x + step, j = d1.y + step;i != d2.x;i += step, j += step) {
//                    chess_board[i][j].flip();
//                }
//            }
//            else if (d1.x != d2.x && d2.x - d1.x == d1.y - d2.y) {
//                //左下右上
//                int step = d1.x < d2.x ? 1 : -1;
//                for (int i = d1.x + step, j = d1.y - step; i != d2.x; i += step, j += step) {
//                    chess_board[i][j].flip();
//                }
//            }
//            else {
//                throw;
//            }
//        }
//    }
//    catch (...) {
//        cout << "Error in flip_between_two_dot." << endl;
//    }
//}
//void game(int x, int y, int color, vector<vector<dot>>& chess_board) {
//    //左
//    int i = 1;
//    while (x - i >= 0 && chess_board[x - i][y].color == color * -1) {
//        i++;
//    }
//    if (x - i >= 0 && chess_board[x - i][y].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
//    }
//    //右
//    i = -1;
//    while (x - i < 8 && chess_board[x - i][y].color == color * -1) {
//        i++;
//    }
//    if (x - i < 8 && chess_board[x - i][y].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
//    }
//    //上
//    i = 1;
//    while (y - i >= 0 && chess_board[x][y - i].color == color * -1) {
//        i++;
//    }
//    if (y - i >= 0 && chess_board[x][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
//    }
//    //下
//    i = -1;
//    while (y - i < 8 && chess_board[x][y - i].color == color * -1) {
//        i++;
//    }
//    if (y - i < 8 && chess_board[x][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
//    }
//    //左上
//    i = 1;
//    while (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
//    }
//    //右下
//    i = -1;
//    while (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
//    }
//    //左下
//    i = 1;
//    while (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
//    }
//    //右上
//    i = -1;
//    while (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x + i >= 0 && y - i < 8 && chess_board[x + i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
//    }
//}
//
//bool findway(vector<vector<dot> > const chess, vector<dot>& store)
//{
//    int m, n;
//    int const find_color = -1 * state;
//    for (int i = 0; i < chess.size(); i++) {
//        for (int j = 0; j < chess.size(); j++) {
//            if (chess[i][j].color == find_color) {
//                //向上找；
//                m = i;
//                n = j;
//                if (m > 0 && chess[--m][n].color == (-1) * find_color) {
//                    while (m >= 0 && chess[m][n].color == (-1) * find_color) {
//                        m--;
//                    }
//                    if (m >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向下找
//                m = i;
//                n = j;
//                if (m < 7 && chess[++m][n].color == (-1) * find_color) {
//                    while (m <= 7 && chess[m][n].color == (-1) * find_color) {
//                        m++;
//                    }
//                    if (m <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向左找
//                m = i;
//                n = j;
//                if (n > 0 && chess[m][--n].color == (-1) * find_color) {
//                    while (n >= 0 && chess[m][n].color == (-1) * find_color) {
//                        n--;
//                    }
//                    if (n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向右找
//                m = i;
//                n = j;
//                if (n < 7 && chess[m][++n].color == (-1) * find_color) {
//                    while (n <= 7 && chess[m][n].color == (-1) * find_color) {
//                        n++;
//                    }
//                    if (n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向左上找
//                m = i;
//                n = j;
//                if (m > 0 && n > 0 && chess[--m][--n].color == (-1) * find_color) {
//                    while (m >= 0 && n >= 0 && chess[m][n].color == (-1) * find_color) {
//                        m--;
//                        n--;
//                    }
//                    if (m >= 0 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向左下找
//                m = i;
//                n = j;
//                if (m < 7 && n>0 && chess[++m][--n].color == (-1) * find_color) {
//                    while (m <= 7 && n >= 0 && chess[m][n].color == (-1) * find_color) {
//                        m++;
//                        n--;
//                    }
//                    if (m <= 7 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向右上找
//                m = i;
//                n = j;
//                if (m > 0 && n < 7 && chess[--m][++n].color == (-1) * find_color) {
//                    while (m >= 0 && n <= 7 && chess[m][n].color == (-1) * find_color) {
//                        m--;
//                        n++;
//                    }
//                    if (m >= 0 && n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //向右下找
//                m = i;
//                n = j;
//                if (m < 7 && n < 7 && chess[++m][++n].color == (-1) * find_color) {
//                    while (m <= 7 && n <= 7 && chess[m][n].color == (-1) * find_color) {
//                        m++;
//                        n++;
//                    }
//                    if (m <= 7 && n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//            }
//        }
//    }
//    if (store.size() > 0) {
//        sort(store.begin(), store.end());
//        store.erase(unique(store.begin(), store.end()), store.end());
//        return true;
//    }
//    else return false;
//}
//
