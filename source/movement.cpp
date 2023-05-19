#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>
#include <movement.h>
#include <destination.h>

using namespace std;

extern Player Players[2 + 5];
extern struct Ingredient Ingredient[20 + 5];
extern int IngredientCount;
extern char Map[20 + 5][20 + 5];
extern int entityCount; // 实体
extern struct Entity Entity[20 + 5];
extern struct Order totalOrder[20 + 5]; // 总订单数组，
extern int orderCount;                  // 订单数
extern struct Order Order[20 + 5];      // 订单
extern pair<double, double> window;
extern pair<double, double> sink;

int dir[2 + 5];
int solvecrash[2 + 5];
int whosolve = 0;
int hascrash = 0;
pair<double, double> crashloc[2 + 5];
int solvetrap[2 + 5] = {0};
pair<double, double> traploc[2 + 5];

int changedir(double des_x, double des_y, int ptype)
{
    double x0, x1, y0, y1, v0x, v0y; // p0 and p1 location
    x0 = Players[ptype].x, x1 = Players[(ptype + 1) % 2].x, y0 = Players[ptype].y, y1 = Players[(ptype + 1) % 2].y;
    v0x = Players[ptype].X_Velocity, v0y = Players[ptype].Y_Velocity;
    bool trapy = (y0 <= 8 && (8 - y0 <= center + 0.3)) && ((x0 < 8 && 8 - x0 <= center) || (x0 >= 8) && (x0 - 8 <= block + center));
    bool trapx = (x0 <= 8 && (8 - x0 <= center + 0.3) && ((y0 >= 8 && y0 - 8 <= block + center) || (y0 < 8 && 8 - y0 <= center)));
    bool crashy1 = (y0 >= y1 && (y0 - y1 <= 2 * radius + 0.5)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashy2 = (y0 <= y1 && (y1 - y0 <= 2 * radius + 0.5)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashx1 = (x0 >= x1 && (x0 - x1 <= 2 * radius + 0.5)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool crashx2 = (x0 < x1 && (x1 - x0 <= 2 * radius + 0.5)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool getup = (y0 - des_y) <= block + center + 0.3;
    bool getdown = (des_y - y0) <= center + 0.3;
    bool getleft = (x0 - des_x) <= center + block + 0.3;
    bool getright = (des_x - x0) <= center + 0.3;
    bool pickLR = ((y0 > des_y) && (y0 - des_y <= center + 0.3)) || (y0 <= des_y);
    bool pickUD = ((x0 > des_x) && (x0 - des_x <= center + 0.3));
    std::cerr << ptype << "changedir: " << des_x << " " << des_y << dir[ptype] << endl;
    if (dir[ptype] == 0)
    {
        if (des_x == 0)
            return 3;
        if (des_x == 9)
            return 4;
        if (des_y == 0)
            return 1;
        if (des_y == 9)
            return 2;
    }
    else if (dir[ptype] == 1)
    {
        if (crashy1)
        {
            cerr << ptype << " dir 1: crashy1" << endl;
            crashloc[ptype].first = x0, crashloc[ptype].second = y0;
            if (x0 >= 3 * block)
            {
                solvecrash[ptype] = 3;
                // return dir[ptype];
            }
            else
            {
                solvecrash[ptype] = 4;
                // return dir[ptype];
            }
            if (ptype == 1)
            {
                hascrash = 1;
            }
            return dir[ptype];
        }
        if (trapy)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 3;
            if (x0 >= 3 * block)
            {
                solvetrap[ptype] = 3;
                return dir[ptype];
            }
            else
            {
                solvetrap[ptype] = 4;
                return dir[ptype];
            }
        }
        if (des_x < x0)
        {
            return 3;
        }
        if (des_x >= x0)
        {
            return 4;
        }
        std::cerr << ptype << ": dir " << dir[ptype] << endl;
    }
    else if (dir[ptype] == 2)
    {
        if (crashy2)
        {
            cerr << ptype << " dir 2: crashy2" << endl;
            crashloc[ptype].first = x0, crashloc[ptype].second = y0;
            if (x0 >= 3 * block)
            {
                solvecrash[ptype] = 3;
                // return dir[ptype];
            }
            else
            {
                solvecrash[ptype] = 4;
            }
            if (ptype == 1)
            {
                hascrash = 1;
            }
            return dir[ptype];
        }
        if (trapy)
        {
            std::cerr << "solvetrap" << endl;
            traploc[ptype].first = x0, traploc[ptype].second = y0;

            if (x0 >= 3 * block)
            {
                solvetrap[ptype] = 3;
                return dir[ptype];
            }
            else
            {
                solvetrap[ptype] = 4;
                return dir[ptype];
            }
        }
        if (des_x < x0)
        {
            return 3;
        }
        if (des_x >= x0)
        {
            return 4;
        }
        std::cerr << ptype << ": dir " << dir[ptype] << endl;
    }
    else if (dir[ptype] == 3)
    {
        if (crashx1)
        {
            cerr << ptype << " dir 3: crashx1" << endl;
            crashloc[ptype].first = x0, crashloc[ptype].second = y0;

            // giveway
            if (y0 >= 3 * block)
            {
                solvecrash[ptype] = 1;
            }
            else
            {
                solvecrash[ptype] = 2;
            }

            if (ptype == 1)
            {
                hascrash = 1;
            }
            return dir[ptype];
        }
        if (trapx)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 1;
            if (y0 >= 3 * block)
            {
                solvetrap[ptype] = 1;
                return dir[ptype];
            }
            else
            {
                solvetrap[ptype] = 2;
                return dir[ptype];
            }
        }
        if (des_y < y0)
        {
            return 1;
        }
        if (des_y >= y0)
        {
            return 2;
        }
        std::cerr << ptype << ": dir " << dir[ptype] << endl;
    }
    else if (dir[ptype] == 4)
    {
        if (crashx2)
        {
            cerr << ptype << " dir 4: crashx2" << endl;
            crashloc[ptype].first = x0, crashloc[ptype].second = y0;
            // giveway
            if (y0 >= 3 * block)
            {
                solvecrash[ptype] = 1;
            }
            else
            {
                solvecrash[ptype] = 2;
            }

            if (ptype == 1)
            {
                hascrash = 1;
            }
            return dir[ptype];
        }
        if (trapx)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 1;
            if (y0 >= 3 * block)
            {
                solvetrap[ptype] = 1;
                return dir[ptype];
            }
            else
            {
                solvetrap[ptype] = 2;
                return dir[ptype];
            }
        }
        if (des_y < y0)
        {
            return 1;
        }
        if (des_y >= y0)
        {
            return 2;
        }
        std::cerr << ptype << ": dir " << dir[ptype] << endl;
    }

    std::cerr << "Not reach changedir end" << std::endl;
    assert(0);
    return dir[ptype];
}

string frame_move(double des_x, double des_y, int ptype, int op)
{
    std::cerr << ptype << ": framemove: " << des_x << " " << des_y << dir[ptype] << endl;
    double x0, x1, y0, y1, v0x, v0y; // p0 and p1 location
    x0 = Players[ptype].x, x1 = Players[(ptype + 1) % 2].x, y0 = Players[ptype].y, y1 = Players[(ptype + 1) % 2].y;
    v0x = Players[ptype].X_Velocity, v0y = Players[ptype].Y_Velocity;
    bool trapy = (y0 <= 8 && (8 - y0 <= center + 0.4)) && ((x0 < 8 && 8 - x0 <= center) || (x0 >= 8) && (x0 - 8 <= block + center));
    bool trapx = (x0 <= 8 && (8 - x0 <= center + 0.4) && ((y0 >= 8 && y0 - 8 <= block + center) || (y0 < 8 && 8 - y0 <= center)));
    bool crashy1 = (y0 >= y1 && (y0 - y1 <= 2 * radius + 0.4)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashy2 = (y0 <= y1 && (y1 - y0 <= 2 * radius + 0.4)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashx1 = (x0 >= x1 && (x0 - x1 <= 2 * radius + 0.4)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool crashx2 = (x0 < x1 && (x1 - x0 <= 2 * radius + 0.4)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool getup = (y0 - des_y) <= block + center + 0.4;
    bool getdown = (des_y - y0) <= center + 0.4;
    bool getleft = (x0 - des_x) <= center + block + 0.4;
    bool getright = (des_x - x0) <= center + 0.4;
    bool pickLR = ((y0 > des_y) && (y0 - des_y <= center + 0.3));
    bool pickUD = ((x0 > des_x) && (x0 - des_x <= center + 0.3));
    if (hascrash && ptype == 1)
    {
        if (v0x != 0 || v0y != 0)
            hascrash = 0;
        if (dir[ptype] == 1)
            return "Move U";
        if (dir[ptype] == 2)
            return "Move D";
        if (dir[ptype] == 3)
            return "Move L";
        if (dir[ptype] == 1)
            return "Move R";
    }
    if (solvecrash[ptype] != 0)
    {
        if (whosolve != ptype && solvecrash[(ptype + 1) % 2] == 0)
            whosolve = ptype;
        if (whosolve == ptype)
        {
            if (solvecrash[ptype] == 1)
            {
                if (crashloc[ptype].second - y0 >= block + 0.5)
                {
                    if (v0y == 0)
                    {
                        solvecrash[ptype] = 0;
                        solvecrash[(ptype + 1) % 2] = 0;
                    }
                    return "Move";
                }
                else
                    return "Move U";
            }
            else if (solvecrash[ptype] == 2)
            {
                if (y0 - crashloc[ptype].second >= block + 0.5)
                {
                    if (v0y == 0)
                        solvecrash[ptype] = 0;
                    return "Move";
                }
                else
                    return "Move D";
            }
            else if (solvecrash[ptype] == 3)
            {
                if (crashloc[ptype].first - x0 >= block + 0.5)
                {
                    if (v0x == 0)
                        solvecrash[ptype] = 0;
                    return "Move";
                }
                else
                    return "Move L";
            }
            else if (solvecrash[ptype] == 4)
            {
                if (x0 - crashloc[ptype].first >= block + 0.5)
                {
                    if (v0x == 0)
                        solvecrash[ptype] = 0;
                    return "Move";
                }
                else
                    return "Move R";
            }
        }
    }

    if (solvetrap[ptype] != 0)
    {
        if (solvetrap[ptype] == 1)
        {
            if (traploc[ptype].second - y0 >= block - 0.3)
            {
                if (v0y == 0)
                    solvetrap[ptype] = 0;
                return "Move";
            }
            else
                return "Move U";
        }
        else if (solvetrap[ptype] == 2)
        {
            if (y0 - traploc[ptype].second >= block - 0.3)
            {
                if (v0y == 0)
                    solvetrap[ptype] = 0;
                return "Move";
            }
            else
                return "Move D";
        }
        else if (solvetrap[ptype] == 3)
        {
            if (traploc[ptype].first - x0 >= block - 0.3)
            {
                if (v0x == 0)
                    solvetrap[ptype] = 0;
                return "Move";
            }
            else
                return "Move L";
        }
        else if (solvetrap[ptype] == 4)
        {
            if (x0 - traploc[ptype].first >= block - 0.3)
            {
                if (v0x == 0)
                    solvetrap[ptype] = 0;
                return "Move";
            }
            else
                return "Move R";
        }
    }
    if (v0x == 0 && v0y == 0)
    {
        if (des_x == 0)
        {
            if (getleft && ((y0 > des_y) && (y0 - des_y <= center + radius)))
            {
                dir[ptype] = 0;
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
        }
        else if (des_x == 9)
        {
            if (getright && ((y0 > des_y) && (y0 - des_y <= center + radius)))
            {
                dir[ptype] = 0;
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
        }
        else if (des_y == 0)
        {
            if (getup && ((x0 > des_x) && (x0 - des_x <= center + radius)))
            {
                dir[ptype] = 0;
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
        }
        else if (des_y == 9)
        {
            if (getdown && ((x0 > des_x) && (x0 - des_x <= center + radius)))
            {
                dir[ptype] = 0;
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
        }
        // add to solve stay and wait
        dir[ptype] = changedir(des_x, des_y, ptype);
    }
    if (dir[ptype] == 1)
    { // UP
        if (des_y == 0)
        {
            if (getup || crashy1)
            {
                return "Move";
            }
            else
            {
                return "Move U";
            }
        }
        else
        {
            if (pickLR || crashy1)
            {
                return "Move";
            }
            else
            {
                return "Move U";
            }
        }
    }
    else if (dir[ptype] == 2)
    { // DOWN 8.58714 7.82534 0 5.25984 0
        if (des_y == 9)
        {
            if (getdown || crashy2 || trapy)
            {
                return "Move";
            }
            else
            {
                return "Move D";
            }
        }
        else
        {
            if (pickLR || crashy2 || trapy)
            {
                if (pickLR)
                    cerr << ptype << ": here1" << endl;
                return "Move";
            }
            else
            {
                cerr << ptype << ": here2" << endl;
                return "Move D";
            }
        }
    }
    else if (dir[ptype] == 3)
    { // left
        if (des_x == 0)
        {
            if (getleft || crashx1)
            {
                return "Move";
            }
            else
            {
                return "Move L";
            }
        }
        else
        {
            if (pickUD || crashx1)
            {
                return "Move";
            }
            else
            {
                return "Move L";
            }
        }
    }
    else if (dir[ptype] == 4)
    {
        if (des_x == 9)
        {
            if (getright || crashx2 || trapx)
            {
                return "Move";
            }
            else
            {
                return "Move R";
            }
        }
        else
        {
            if (pickUD || crashx2 || trapx)
            {
                return "Move";
            }
            else
            {
                return "Move R";
            }
        }
    }
    std::cerr << "NOt reach frameend" << std::endl;
    return "Move";
}
