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
//    //���ӵ���Ϣ
//    int x = 0, y = 0, color = 0;
//
//    //���캯��
//    dot(int x, int y, int color) :x(x), y(y), color(color) {}
//
//    //���ظ�ֵ�����
//    dot& operator = (const dot& other) {
//        this->x = other.x;
//        this->y = other.y;
//        this->color = other.color;
//        return *this;
//    }
//
//    //��������ת
//    void flip() {
//        color *= -1;
//    }
//
//    //��ӡ���ӱ�����������ɫ
//    void print() {
//        //        string color_string = color==1?"white)":(color?"black)":"empty)");
//        string color_string = color == 1 ? "��)" : (color ? "��)" : " )");
//
//        cout << "(" << x << "," << y << "," << color_string;
//    }
//
//    //��Ԫ���� ʵ�ַ�ת dot1 �� dot2 ������֮������ӵķ�ת����Ҫ��������
//    friend void flip_between_two_dot(dot d1, dot d2, vector<vector<dot>>& chess_board);
//
//    //��Ԫ���� ʵ���ҵ� �µ����� �İ˸���������Ҫ��ת�Ĳ�ͬ��ɫ������յ����ӵ�λ�ã�
//    //�����ݸ�flip_between_two_dot���� ʵ�ַ�ת
//    friend void game(int x, int y, int color, vector<vector<dot>>& chess_board);
//
//    //�ı�����������ɫΪ�ض���ɫ�������ڿհ�λ������
//    void set_color(int new_color) {
//        color = new_color;
//    }
//    //
//
//
//    //���رȽ��������Ϊ��֮���ȥ��
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
////ȫ�ֺ���
//void Show_present_chess_and_way(vector<vector<dot>>& chess);//vector<vector<dot>>
////    friend void game(int x, int y, int color, vector<vector<dot>>& chess_board);
//
//int main() {
//    // initialization
//    // declare chess_board to store the information of chess's location and color
//    static vector<vector<dot>> chess_board(8, vector<dot>(8, dot(0, 0, 0)));
//    // initialize the chess_board
//    //�м��ĸ����� ����ĺڰ�
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
//    vector<dot> available;//����洢���µ�����������
//    int i, j;
//    cout << "��ʱ����Ϊ��" << endl;
//    for (i = 0; i < 8; i++) {
//        for (j = 0; j < 8; j++) {
//            chess[i][j].print();
//        }
//        cout << endl;
//    }
//    if (state == -1) {
//        cout << "��ʱ�������µĸ����͵ط��ֱ�Ϊ��" << endl;
//        if (findway(chess, available)) {
//            cout << available.size() << endl;
//            for (int num = 0; num < available.size(); num++) available[num].print();
//            cout << endl;
//        }
//        else cout << "û��������ĵط�" << endl;
//    }
//    else {
//        cout << "��ʱ�������µĸ����͵ط��ֱ�Ϊ��" << endl;
//        if (findway(chess, available)) {
//            cout << available.size() << endl;
//            for (int num = 0; num < available.size(); num++) available[num].print();
//            cout << endl;
//        }
//        else cout << "û��������ĵط�" << endl;
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
//                //��������
//                int step = d1.x < d2.x ? 1 : -1;
//                for (int i = d1.x + step, j = d1.y + step;i != d2.x;i += step, j += step) {
//                    chess_board[i][j].flip();
//                }
//            }
//            else if (d1.x != d2.x && d2.x - d1.x == d1.y - d2.y) {
//                //��������
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
//    //��
//    int i = 1;
//    while (x - i >= 0 && chess_board[x - i][y].color == color * -1) {
//        i++;
//    }
//    if (x - i >= 0 && chess_board[x - i][y].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
//    }
//    //��
//    i = -1;
//    while (x - i < 8 && chess_board[x - i][y].color == color * -1) {
//        i++;
//    }
//    if (x - i < 8 && chess_board[x - i][y].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y], chess_board);
//    }
//    //��
//    i = 1;
//    while (y - i >= 0 && chess_board[x][y - i].color == color * -1) {
//        i++;
//    }
//    if (y - i >= 0 && chess_board[x][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
//    }
//    //��
//    i = -1;
//    while (y - i < 8 && chess_board[x][y - i].color == color * -1) {
//        i++;
//    }
//    if (y - i < 8 && chess_board[x][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x][y - i], chess_board);
//    }
//    //����
//    i = 1;
//    while (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x - i >= 0 && y - i >= 0 && chess_board[x - i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
//    }
//    //����
//    i = -1;
//    while (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x - i < 8 && y - i < 8 && chess_board[x - i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x - i][y - i], chess_board);
//    }
//    //����
//    i = 1;
//    while (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color * -1) {
//        i++;
//    }
//    if (x + i < 8 && y - i >= 0 && chess_board[x + i][y - i].color == color) {
//        flip_between_two_dot(chess_board[x][y], chess_board[x + i][y - i], chess_board);
//    }
//    //����
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
//                //�����ң�
//                m = i;
//                n = j;
//                if (m > 0 && chess[--m][n].color == (-1) * find_color) {
//                    while (m >= 0 && chess[m][n].color == (-1) * find_color) {
//                        m--;
//                    }
//                    if (m >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //������
//                m = i;
//                n = j;
//                if (m < 7 && chess[++m][n].color == (-1) * find_color) {
//                    while (m <= 7 && chess[m][n].color == (-1) * find_color) {
//                        m++;
//                    }
//                    if (m <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //������
//                m = i;
//                n = j;
//                if (n > 0 && chess[m][--n].color == (-1) * find_color) {
//                    while (n >= 0 && chess[m][n].color == (-1) * find_color) {
//                        n--;
//                    }
//                    if (n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //������
//                m = i;
//                n = j;
//                if (n < 7 && chess[m][++n].color == (-1) * find_color) {
//                    while (n <= 7 && chess[m][n].color == (-1) * find_color) {
//                        n++;
//                    }
//                    if (n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //��������
//                m = i;
//                n = j;
//                if (m > 0 && n > 0 && chess[--m][--n].color == (-1) * find_color) {
//                    while (m >= 0 && n >= 0 && chess[m][n].color == (-1) * find_color) {
//                        m--;
//                        n--;
//                    }
//                    if (m >= 0 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //��������
//                m = i;
//                n = j;
//                if (m < 7 && n>0 && chess[++m][--n].color == (-1) * find_color) {
//                    while (m <= 7 && n >= 0 && chess[m][n].color == (-1) * find_color) {
//                        m++;
//                        n--;
//                    }
//                    if (m <= 7 && n >= 0 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //��������
//                m = i;
//                n = j;
//                if (m > 0 && n < 7 && chess[--m][++n].color == (-1) * find_color) {
//                    while (m >= 0 && n <= 7 && chess[m][n].color == (-1) * find_color) {
//                        m--;
//                        n++;
//                    }
//                    if (m >= 0 && n <= 7 && chess[m][n].color == 0) store.push_back(chess[m][n]);
//                }
//                //��������
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
