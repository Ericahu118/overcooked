#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>
#include <movement.h>
#include <destination.h>
#include <queue>
#include <stack>

using namespace std;

extern Player Players[2 + 5];
extern struct Ingredient Ingredient[20 + 5];
extern int IngredientCount;
extern char Map[20 + 5][20 + 5];
extern int width, height;
extern int entityCount; // 实体
extern struct Entity Entity[20 + 5];
extern struct Order totalOrder[20 + 5]; // 总订单数组，
extern int orderCount;                  // 订单数
extern struct Order Order[20 + 5];      // 订单
extern pair<double, double> window;
extern pair<double, double> sink;

char Player_Map[20 + 5][20 + 5];
// 八个方向， 上，下，左，右，左上，右上，左下，右下
int dx[8] = {0, 0, -1, 1, -1, 1, -1, 1};
int dy[8] = {-1, 1, 0, 0, -1, -1, 1, 1};

int bfs(int ptype, int des_x, int des_y)
{
    // 初始化
    cerr << ptype << ": bfs:des_x:" << des_x << " des_y:" << des_y << endl;
    int x = Players[ptype].x / block, y = Players[ptype].y / block;
    cerr << ptype << ": " << Players[ptype].x << " " << Players[ptype].y << endl;
    cerr << ptype << ": "
         << "bfs:x:" << x << " y:" << y << endl;
    Players[ptype].des_x = x;
    Players[ptype].des_y = y;
    // 清空
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (Player_Map[i][j] == (ptype + '0'))
                Player_Map[i][j] = '.';
        }
    }
    // 当前位置标记
    Player_Map[y][x] = ptype + '0';
    int father[20 + 5][20 + 5] = {0}, vis[20 + 5][20 + 5] = {0};
    queue<pair<int, int>> q;
    q.push(make_pair(x, y));
    int flag = 0;
    while (!q.empty())
    {
        pair<int, int> cur = q.front();
        q.pop();
        vis[cur.second][cur.first] = 1;
        for (int d = 0; d < 8; d++)
        {
            int tmpx = cur.first + dx[d];
            int tmpy = cur.second + dy[d];
            if (tmpx == des_x && tmpy == des_y)
            { // 找到了
                if (d < 4 || (d >= 4 && Player_Map[cur.second][tmpx] == '.' && Player_Map[tmpy][cur.first] == '.'))
                {
                    father[tmpy][tmpx] = d;
                    flag = 1;
                    break;
                }
            }
            if (tmpx > 0 && tmpx < 9 && tmpy > 0 && tmpy < 9)
            { // 范围内
                if (!vis[tmpy][tmpx])
                {
                    if (d < 4)
                    { // 上下左右
                        if (Player_Map[tmpy][tmpx] == '.')
                        {
                            father[tmpy][tmpx] = d;
                            vis[tmpy][tmpx] = 1;
                            q.push(make_pair(tmpx, tmpy));
                        }
                    }
                    else
                    {
                        if (Player_Map[tmpy][tmpx] == '.' && Player_Map[cur.second][tmpx] == '.' && Player_Map[tmpy][cur.first] == '.')
                        {
                            father[tmpy][tmpx] = d;
                            vis[tmpy][tmpx] = 1;
                            q.push(make_pair(tmpx, tmpy));
                        }
                    }
                }
            }
        }
        if (flag)
            break;
    }
    if (flag)
    {
        int tmpx = des_x - dx[father[des_y][des_x]];
        int tmpy = des_y - dy[father[des_y][des_x]];
        stack<int> st;
        while (tmpx != x || tmpy != y)
        {
            st.push(father[tmpy][tmpx]);
            int d = father[tmpy][tmpx];
            tmpx -= dx[d], tmpy -= dy[d];
        }
        int stay = st.top();
        while (!st.empty() && st.top() == stay)
        {
            Players[ptype].des_x += dx[stay];
            Players[ptype].des_y += dy[stay];
            Player_Map[(int)Players[ptype].des_y][(int)Players[ptype].des_x] = ptype + '0'; // 打上标记
            st.pop();
        }
        cerr << ptype << ": "
             << "bfs:pdesx:" << Players[ptype].des_x << " pdesy:" << Players[ptype].des_y << endl;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                cerr << Player_Map[i][j];
            }
            cerr << endl;
        }
    }
    return flag;
}

std::string frame_move(double des_x, double des_y, int ptype, int op)
{
    cerr << "frame_move:des_x:" << des_x << " des_y:" << des_y << endl;
    if (Players[ptype].X_Velocity == 0 && Players[ptype].Y_Velocity == 0)
    { // 当速度为0时
        // 首先判断东西是否可拿到
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (Player_Map[i][j] == (ptype + '0'))
                    Player_Map[i][j] = '.';
            }
        }
        int x = Players[ptype].x / block, y = Players[ptype].y / block;
        cerr << "Now v is 0:" << ptype << ": " << x << " " << y << endl;
        Player_Map[y][x] = ptype + '0';

        if (des_y == 0 && (Players[ptype].y - des_y <= block + center + 0.3) && (Players[ptype].x >= des_x && (Players[ptype].x - des_x) <= center + radius))
        { // 上
            switch (op)
            {
            case 0:
                return "PutOrPick U";
            case 1:
                return "Interact U";
            case 2:
                return "Move";
            default:
                assert(op == 0 || op == 1 || op == 2);
                break;
            }
        }
        else if (des_y == 9 && (des_y - Players[ptype].y <= center + 0.3) && (Players[ptype].x >= des_x && (Players[ptype].x - des_x) <= center + radius))
        { // 下
            switch (op)
            {
            case 0:
                return "PutOrPick D";
            case 1:
                return "Interact D";
            case 2:
                return "Move";
            default:
                assert(op == 0 || op == 1 || op == 2);
                break;
            }
        }
        else if (des_x == 0 && (Players[ptype].x - des_x <= block + center + 0.3) && (Players[ptype].y >= des_y && (Players[ptype].y - des_y) <= center + radius))
        { // 左
            switch (op)
            {
            case 0:
                return "PutOrPick L";
            case 1:
                return "Interact L";
            case 2:
                return "Move";
            default:
                assert(op == 0 || op == 1 || op == 2);
                break;
            }
        }
        else if (des_x == 9 && (des_x - Players[ptype].x <= center + 0.3) && (Players[ptype].y >= des_y && (Players[ptype].y - des_y) <= center + radius))
        { // 右
            switch (op)
            {
            case 0:
                return "PutOrPick R";
            case 1:
                return "Interact R";
            case 2:
                return "Move";
            default:
                assert(op == 0 || op == 1 || op == 2);
                break;
            }
        }
        // 当都不可拿到时 bfs更新路径
        if (bfs(ptype, des_x, des_y) == 0)
            return "Move";
    }
    string ret = "Move ";
    // fix
    if (Players[ptype].x - center > Players[ptype].des_x + 0.3)
        ret += "L";
    if (Players[ptype].x < Players[ptype].des_x)
        ret += "R";
    if (Players[ptype].y - center > Players[ptype].des_y + 0.3)
        ret += "U";
    if (Players[ptype].y < Players[ptype].des_y)
        ret += "D";
    return ret;
}
