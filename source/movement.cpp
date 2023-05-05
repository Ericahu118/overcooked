#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>
#include <movement.h>
extern Player Players[2 + 5];
extern struct Ingredient Ingredient[20 + 5];
extern int IngredientCount;
extern char Map[20 + 5][20 + 5];
extern int entityCount; // 实体
extern struct Entity Entity[20 + 5];
extern struct Order totalOrder[20 + 5]; // 总订单数组，
extern int orderCount;                  // 订单数
extern struct Order Order[20 + 5];

const double radius = 0.35;
const double interdis = 1.3;
const double center = 0.5;
const double block = 1;

int solvecrash = 0;
pair<double, double> crashloc;
int solvetrap[2 + 5] = {0};
pair<double, double> traploc[2 + 5];

int changedir(double des_x, double des_y, int ptype)
{
    double x0, x1, y0, y1, v0x, v0y; // p0 and p1 location
    x0 = Players[ptype].x, x1 = Players[(ptype + 1) % 2].x, y0 = Players[ptype].y, y1 = Players[(ptype + 1) % 2].y;
    v0x = Players[ptype].X_Velocity, v0y = Players[ptype].Y_Velocity;
    bool trapy = (y0 <= 8 && (8 - y0 <= center + 0.3)) && ((x0 < 8 && 8 - x0 <= center) || (x0 >= 8) && (x0 - 8 <= block + center));
    bool trapx = (x0 <= 8 && (8 - x0 <= center + 0.3) && ((y0 >= 8 && y0 - 8 <= block + center) || (y0 < 8 && 8 - y0 <= center)));
    bool crashy1 = (y0 >= y1 && (y0 - y1 <= 2 * radius + 0.3)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashy2 = (y0 <= y1 && (y1 - y0 <= 2 * radius + 0.3)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashx1 = (x0 >= x1 && (x0 - x1 <= 2 * radius + 0.3)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool crashx2 = (x0 < x1 && (x1 - x0 <= 2 * radius + 0.3)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool getup = (y0 - des_y) <= block + center + 0.3;
    bool getdown = (des_y - y0) <= center + 0.3;
    bool getleft = (x0 - des_x) <= center + block + 0.3;
    bool getright = (des_x - x0) <= center + 0.3;
    bool pickLR = ((y0 > des_y) && (y0 - des_y <= center + 0.3)) || (y0 <= des_y);
    bool pickUD = ((x0 > des_x) && (x0 - des_x <= center + 0.3));

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
            if (ptype == 0)
            {
                // giveway
                crashloc.first = x0, crashloc.second = y0;
                solvecrash = 1;
                if (x0 >= 3 * block)
                {
                    return 3;
                }
                else
                    return 4;
            }
            else
                return dir[ptype];
        }
        if (trapy)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 1;
            if (x0 >= 3 * block)
            {
                return 3;
            }
            else
                return 4;
        }
        if (des_x < x0)
        {
            return 3;
        }
        if (des_x >= x0)
        {
            return 4;
        }
    }
    else if (dir[ptype] == 2)
    {
        if (crashy2)
        {
            if (ptype == 0)
            {
                // giveway
                crashloc.first = x0, crashloc.second = y0;
                solvecrash = 1;
                if (x0 >= 3 * block)
                {
                    return 3;
                }
                else
                    return 4;
            }
            else
                return dir[ptype];
        }
        if (trapy)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 1;
            if (x0 >= 3 * block)
            {
                return 3;
            }
            else
                return 4;
        }
        if (des_x < x0)
        {
            return 3;
        }
        if (des_x >= x0)
        {
            return 4;
        }
    }
    else if (dir[ptype] == 3)
    {
        if (crashx1)
        {
            if (ptype == 0)
            {
                // giveway
                crashloc.first = x0, crashloc.second = y0;
                solvecrash = 1;
                if (y0 >= 3 * block)
                {
                    return 1;
                }
                else
                    return 2;
            }
            else
                return dir[ptype];
        }
        if (trapx)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 1;
            if (y0 >= 3 * block)
            {
                return 1;
            }
            else
                return 2;
        }
        if (des_y < y0)
        {
            return 1;
        }
        if (des_x >= y0)
        {
            return 2;
        }
    }
    else if (dir[ptype] == 4)
    {
        if (crashx2)
        {
            if (ptype == 0)
            {
                // giveway
                crashloc.first = x0, crashloc.second = y0;
                solvecrash = 1;
                if (y0 >= 3 * block)
                {
                    return 1;
                }
                else
                    return 2;
            }
            else
                return dir[ptype];
        }
        if (trapx)
        {
            traploc[ptype].first = x0, traploc[ptype].second = y0;
            solvetrap[ptype] = 1;
            if (y0 >= 3 * block)
            {
                return 1;
            }
            else
                return 2;
        }
        if (des_y < y0)
        {
            return 1;
        }
        if (des_x >= y0)
        {
            return 2;
        }
    }
    std::cerr << "Not reach changedir end" << std::endl;
    assert(0);
    return 0;
}

string frame_move(double des_x, double des_y, int ptype, int op)
{
    double x0, x1, y0, y1, v0x, v0y; // p0 and p1 location
    x0 = Players[ptype].x, x1 = Players[(ptype + 1) % 2].x, y0 = Players[ptype].y, y1 = Players[(ptype + 1) % 2].y;
    v0x = Players[ptype].X_Velocity, v0y = Players[ptype].Y_Velocity;
    bool trapy = (y0 <= 8 && (8 - y0 <= center + 0.3)) && ((x0 < 8 && 8 - x0 <= center) || (x0 >= 8) && (x0 - 8 <= block + center));
    bool trapx = (x0 <= 8 && (8 - x0 <= center + 0.3) && ((y0 >= 8 && y0 - 8 <= block + center) || (y0 < 8 && 8 - y0 <= center)));
    bool crashy1 = (y0 >= y1 && (y0 - y1 <= 2 * radius + 0.3)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashy2 = (y0 <= y1 && (y1 - y0 <= 2 * radius + 0.3)) && ((x0 < x1 && (x1 - x0) <= 2 * radius) || (x0 > x1) && (x0 - x1 <= 2 * radius));
    bool crashx1 = (x0 >= x1 && (x0 - x1 <= 2 * radius + 0.3)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool crashx2 = (x0 < x1 && (x1 - x0 <= 2 * radius + 0.3)) && ((y0 < y1 && (y1 - y0) <= 2 * radius) || (y0 > y1) && (y0 - y1 <= 2 * radius));
    bool getup = (y0 - des_y) <= block + center + 0.3;
    bool getdown = (des_y - y0) <= center + 0.3;
    bool getleft = (x0 - des_x) <= center + block + 0.3;
    bool getright = (des_x - x0) <= center + 0.3;
    bool pickLR = ((y0 > des_y) && (y0 - des_y <= center + 0.3)) || (y0 <= des_y);
    bool pickUD = ((x0 > des_x) && (x0 - des_x <= center + 0.3));

    if (solvecrash && ptype == 0)
    {
        if (dir[ptype] == 1)
        {
            if (crashloc.second - y0 >= block - 0.3)
            {
                if (v0y == 0)
                    solvecrash = 0;
                return "Move";
            }
            else
                return "Move U";
        }
        else if (dir[ptype] == 2)
        {
            if (y0 - crashloc.second >= block - 0.3)
            {
                if (v0y == 0)
                    solvecrash = 0;
                return "Move";
            }
            else
                return "Move D";
        }
        else if (dir[ptype] == 3)
        {
            if (crashloc.first - x0 >= block - 0.3)
            {
                if (v0x == 0)
                    solvecrash = 0;
                return "Move";
            }
            else
                return "Move L";
        }
        else if (dir[ptype] == 4)
        {
            if (x0 - crashloc.first >= block - 0.3)
            {
                if (v0x == 0)
                    solvecrash = 0;
                return "Move";
            }
            else
                return "Move R";
        }
    }

    if (solvetrap[ptype])
    {
        if (dir[ptype] == 1)
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
        else if (dir[ptype] == 2)
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
        else if (dir[ptype] == 3)
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
        else if (dir[ptype] == 4)
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
            if (getleft && pickLR)
            {
                dir[ptype] = 0;
                switch (op)
                {
                case 0:
                    return "PutOrPick L";
                case 1:
                    return "Interact L";
                default:
                    assert(op == 0 || op == 1);
                    break;
                }
            }
        }
        else if (des_x == 9)
        {
            if (getright && pickLR)
            {
                dir[ptype] = 0;
                switch (op)
                {
                case 0:
                    return "PutOrPick R";
                case 1:
                    return "Interact R";
                default:
                    assert(op == 0 || op == 1);
                    break;
                }
            }
        }
        else if (des_y == 0)
        {
            if (getup && pickUD)
            {
                dir[ptype] = 0;
                switch (op)
                {
                case 0:
                    return "PutOrPick U";
                case 1:
                    return "Interact U";
                default:
                    assert(op == 0 || op == 1);
                    break;
                }
            }
        }
        else if (des_y == 9)
        {
            if (getdown && pickUD)
            {
                dir[ptype] = 0;
                switch (op)
                {
                case 0:
                    return "PutOrPick D";
                case 1:
                    return "Interact D";
                default:
                    assert(op == 0 || op == 1);
                    break;
                }
            }
        }
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
    { // DOWN
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
                return "Move";
            }
            else
            {
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
            if (pickUD || crashy2)
            {
                return "Move";
            }
            else
            {
                return "Move D";
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
}