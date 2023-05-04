#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>

const double radius = 0.35;
const double interdis = 1.3;
const double center = 0.5;
const double block = 1;

extern Player Players[2 + 5];
extern struct Ingredient Ingredient[20 + 5];
extern int IngredientCount;
extern char Map[20 + 5][20 + 5];
extern int entityCount; // 实体
extern struct Entity Entity[20 + 5];

int worktype[2 + 5] = {0}; // 0 for dish 1 for wash
// My move plan remain to solve crash
bool topick = false;
int crashdir[2 + 5] = {0};

std::string Solvecrash()
{
    return "Move";
}
std::string Moveplan(double x, double y, int ptype, int op)
{
    double x0, x1, y0, y1, v0x, v0y; // p0 and p1 location
    x0 = Players[ptype].x, x1 = Players[(ptype + 1) % 2].x, y0 = Players[ptype].y, y1 = Players[(ptype + 1) % 2].y;
    v0x = Players[ptype].X_Velocity, v0y = Players[ptype].Y_Velocity;
    bool crash = ((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1)) <= 4 * radius;
    bool pickobj_x1 = (x0 - x <= block + center + 0.3); // left
    bool pickobj_x2 = (x - x0 <= center + 0.3);         // right
    bool pickobj_xy1 = (y0 - y <= center + 0.3);        // x+up
    bool pickobj_xy2 = (y - y0 <= center + 0.3);        // x + down
    bool pickobj_y1 = (y0 - y <= block + center + 0.3);
    bool pickobj_y2 = (y - y0 <= center + 0.3);
    bool pickobj_yx1 = (y0 - y <= center + 0.2); // y+left
    bool pickobj_yx2 = (x0 - x <= center + 0.2); // y+right
    bool gettrap = ((8 - x0) <= center + 0.3) && ((y0 > 8 && (y0 - 8 <= block + center)) || (y0 <= 8 && 8 - y0 <= center));
    if (x == 0)
    { // right line
        if (v0x == 0 && v0y == 0)
        { // step3 has stop
            if (crash)
            {
                // todo remain unsolve
                return "Move";
            }
            if (pickobj_x1)
            {
                if (y < y0 && pickobj_xy1)
                {
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
        }
        if (v0x == 0)
        { // step2 stop
            if (pickobj_x1)
            { // because of dis
                if (y > y0)
                {
                    if (crash)
                        return "Move";
                    return "Move D";
                }
                else
                {
                    if (y0 - y <= center + 0.3 || crash)
                    {
                        return "Move";
                    }
                    else
                        return "Move U";
                }
            }
        }
        // step1 left first
        if (crash || pickobj_x1)
            return "Move";
        else
            return "Move L";
    }
    else if (x == 9)
    { // left line
        // 先向右
        if (crash || pickobj_x2 || gettrap)
        {
            if (v0x != 0)
            {
                return "Move";
            }
        }
        else
        {
            return "Move R";
        }
        // 向上或向下
        if (y > y0)
        {
            if (crash)
                return "Move";
            return "Move D";
        }
        else
        {
            if (y0 - y <= center + 0.3 || crash)
            {
                if (v0y != 0)
                {
                    return "Move";
                }
            }
            else
                return "Move U";
        }
        if (v0x == 0 && v0y == 0)
        { // step3 has stop
            if (crash)
            {
                // todo remain unsolve
                return "Move";
            }
            if (pickobj_x2)
                if (y < y0 && pickobj_xy1)
                {
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
    }
    // 6.5 8.58714 0 0 0
    // 7.10355 4.5 5.25984 0 0
    else if (y == 9)
    {
        if (v0x == 0 && v0y == 0)
        { // step3 has stop
            if (crash)
            {
                // todo remain unsolve
                return "Move";
            }
            if (pickobj_y2)
                if (x0 > x && pickobj_yx2)
                {
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
        if (v0y == 0)
        { // step2 stop
            if (pickobj_y2)
            { // because of dis
                if (x > x0)
                { // left
                    if (crash)
                        return "Move";
                    return "Move R";
                }
                else
                { // right
                    if (x0 - x <= center + 0.3 || crash)
                    {
                        return "Move";
                    }
                    else
                        return "Move L";
                }
            }
        }
        if (crash || pickobj_y2)
            return "Move";
        else
            return "Move D";
    }
    else if (y == 0)
    { // up
        if (v0x == 0 && v0y == 0)
        { // step3 has stop
            if (crash)
            {
                // todo remain unsolve
                return "Move";
            }
            if (pickobj_y1)
                if (x0 > x && pickobj_yx2)
                {
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
        if (v0y == 0)
        { // step2 stop
            if (pickobj_y1)
            { // because of dis
                if (x > x0)
                { // left
                    if (crash)
                        return "Move";
                    return "Move R";
                }
                else
                { // right
                    if (x0 - x <= center + 0.3 || crash)
                    {
                        return "Move";
                    }
                    else
                        return "Move L";
                }
            }
        }
        if (crash || pickobj_y1)
            return "Move";
        else
            return "Move U";
    }
    return "Move";
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cerr.tie(nullptr);
    std::cerr << std::nounitbuf;
    std::string s;
    std::stringstream ss;
    int frame;

    init_read();

    /*
        你可以在读入后进行一些相关预处理，时间限制：5秒钟
        init();
    */

    int totalFrame = 14400;
    for (int i = 0; i < totalFrame; i++)
    {
        bool skip = frame_read(i);
        if (skip)
            continue;

        /* 输出当前帧的操作，此处仅作示例 */
        std::cout << "Frame " << i << "\n";

        std::string player0_Action = "Move";
        std::string player1_Action = "Move";
        if (Players[0].entity.empty())
        {
            for (int i = 0; i < IngredientCount; i++)
            {
                if (Ingredient[i].name.compare("kelp") == 0)
                {
                    std::cerr << "step1 " << std::endl;
                    player0_Action = Moveplan(Ingredient[i].x, Ingredient[i].y, 0, 0);
                    break;
                }
            }
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty())
                {
                    std::cerr << "step2 " << std::endl;
                    player0_Action = Moveplan(Entity[i].x, Entity[i].y, 0, 0);
                    break;
                }
            }
        }
        else
        {
            if (Players[0].containerKind == ContainerKind::None)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Plate)
                    {
                        std::cerr << "step3 " << std::endl;
                        player0_Action = Moveplan(Entity[i].x, Entity[i].y, 0, 0);

                        break;
                    }
                }
            }
            else if (Players[0].containerKind == ContainerKind::Plate)
            {
                std::cerr << "step4" << std::endl;
                player0_Action = Moveplan(4, 0, 0, 0);
            }
        }
        bool haveplate = false;
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate)
            {
                haveplate == true;
            }
            if (Entity[i].containerKind == ContainerKind::DirtyPlates && Entity[i].x == 9 && Entity[i].y == 3)
            {
                player1_Action = Moveplan(9, 3, 1, 1);
            }
        }
        if (!haveplate)
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                {
                    player1_Action = Moveplan(Entity[i].x, Entity[i].y, 1, 0);
                }
            }
        }
        if (Players[1].containerKind == ContainerKind::DirtyPlates)
        {
            player1_Action = Moveplan(9, 3, 1, 0);
        }

        /* 合成一个字符串再输出，否则输出有可能会被打断 */
        std::string action = player0_Action + "\n" + player1_Action + "\n";
        std::cout << action;

        /* 不要忘记刷新输出流，否则游戏将无法及时收到响应 */
        std::cout.flush();
    }
}
