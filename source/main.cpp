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
extern int width, height;
extern char Map[20 + 5][20 + 5];

extern int curplates;
extern Task taketask[2 + 5];
extern bool changephase[2 + 5];

void init()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (Map[i][j] == '$')
            {
                window.first = j, window.second = i;
            }
            else if (Map[i][j] == 'k')
            {
                sink.first = j, sink.second = i;
            }
        }
    }
    curplates = 0;
    for (int i = 0; i < entityCount; i++)
    {
        if (Entity[i].containerKind == ContainerKind::Plate)
        {
            cerr << "Why1" << endl;
            curplates++;
        }
    }
    Task task;
    task.id = -1;
    task.op = 0, task.x = 0, task.y = 0, task.flag = 0;
    taketask[0] = taketask[1] = task;
    changephase[0] = changephase[1] = 0;
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
    init();
    int totalFrame = 14400;
    cerr << "window: " << window.first << " " << window.second << endl;
    cerr << "sink: " << sink.first << " " << sink.second << endl;
    cerr << "curplates" << curplates << endl;
    for (int i = 0; i < totalFrame; i++)
    {
        bool skip = frame_read(i);
        if (skip)
            continue;

        /* 输出当前帧的操作，此处仅作示例 */
        std::cout << "Frame " << i << "\n";

        // std::string player0_Action = "Move";
        // std::string player1_Action = "Move";
        string playerAction[2 + 5];
        playerAction[0] = "Move";
        playerAction[1] = "Move";
        for (int ptype = 0; ptype <= 1; ptype++)
        {
            if (taketask[ptype].id == -1 || changephase[ptype] == 1)
            {
                taketask[ptype] = arrangetask(ptype);
            }
            if (taketask[ptype].id != -1)
            {
                playerAction[ptype] = frame_move(taketask[ptype].x, taketask[ptype].y, ptype, taketask[ptype].op);
                if (playerAction[ptype][0] == 'P' || playerAction[ptype][0] == 'I')
                {
                    int flag = 0;
                    switch (taketask[ptype].op)
                    {
                    case 0:

                        if (taketask[ptype].flag = 1)
                        {
                            taketask[ptype].id = -1;
                        }
                        else
                            changephase[ptype] = 1;
                        break;
                    case 1:
                        if (taketask[ptype].id == 0)
                        {
                            for (int i = 0; i < entityCount; i++)
                            {
                                if (Entity[i].containerKind == ContainerKind::DirtyPlates && Entity[i].x == sink.first && Entity[i].y == sink.second)
                                {
                                    if (Entity[i].currentFrame == Entity[i].currentFrame - 2)
                                    {
                                        curplates++;
                                    }
                                    flag = 1;
                                    break;
                                }
                            }
                            if (flag == 0)
                            {
                                if (taketask[ptype].flag = 1)
                                {
                                    taketask[ptype].id = -1;
                                }
                                else
                                    changephase[ptype] = 1;
                            }
                        }
                        break;
                    default:
                        cerr << "main:case op wrong" << endl;
                        assert(0);
                        break;
                    }
                }
            }
        }
        /*if (Players[0].entity.empty())
        {
            for (int i = 0; i < IngredientCount; i++)
            {
                std::string need = Order[0].recipe[0];
                if (Ingredient[i].name.compare(need) == 0)
                {
                    cerr << "1: step1" << endl;
                    player0_Action = frame_move(Ingredient[i].x, Ingredient[i].y, 0, 0);

                    break;
                }
            }
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty())
                {
                    cerr << "1: step2" << endl;
                    player0_Action = frame_move(Entity[i].x, Entity[i].y, 0, 0);
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
                        cerr << "1: step3" << endl;
                        player0_Action = frame_move(Entity[i].x, Entity[i].y, 0, 0);

                        break;
                    }
                }
            }
            else if (Players[0].containerKind == ContainerKind::Plate)
            {
                cerr << "1: step4" << endl;
                player0_Action = frame_move(window.first, window.second, 0, 0);
            }
        }

        if (Players[1].containerKind == ContainerKind::DirtyPlates)
        {
            cerr << "2: step1" << endl;
            player1_Action = frame_move(sink.first, sink.second, 1, 0);
        }
        else
        {
            bool flag = 0;
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::DirtyPlates && Entity[i].x == sink.first && Entity[i].y == sink.second)
                {
                    cerr << "2: step2" << endl;
                    flag = 1;
                    break;
                }
            }
            if (flag)
                player1_Action = frame_move(sink.first, sink.second, 1, 1);
            else
            {
                bool haveplate = false;
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Plate)
                    {
                        haveplate = true;
                    }
                }
                if (!haveplate)
                {

                    for (int i = 0; i < entityCount; i++)
                    {
                        if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                        {
                            cerr << "2: step3" << endl;
                            player1_Action = frame_move(Entity[i].x, Entity[i].y, 1, 0);
                        }
                    }
                }
            }
        }*/
        /* 合成一个字符串再输出，否则输出有可能会被打断 */
        std::string action = playerAction[0] + "\n" + playerAction[1] + "\n";
        std::cout << action;

        /* 不要忘记刷新输出流，否则游戏将无法及时收到响应 */
        std::cout.flush();
    }
}
