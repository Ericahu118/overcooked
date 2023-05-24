#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>
#include <movement.h>
#include <destination.h>
#include <map>
#include <queue>

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
extern int width, height;
extern int recipeCount; // 菜谱种类,y
extern struct Recipe Recipe[20 + 5];

extern int curplates;
extern Task taketask[2 + 5];
extern int curorder;

extern Dishes currentdish;
extern queue<pair<double, double>> topick;

extern int cursent;

extern map<string, string> Origin;
extern map<string, int> Cookkind;

extern pair<double, double> window;
extern pair<double, double> sink;
extern pair<double, double> cutplate;

extern char Player_Map[20 + 5][20 + 5];

extern vector<string> dishinplate;

void init()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Player_Map[i][j] = Map[i][j];
        }
    }

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
            else if (Map[i][j] == 'c')
            {
                cutplate.first = j, cutplate.second = i;
            }
        }
    }

    Task task;
    task.id = -1;
    task.op = 0, task.x = 0, task.y = 0, task.flag = 0;
    taketask[0] = taketask[1] = task;

    // init recipe
    for (int i = 0; i < IngredientCount; i++)
    {
        Origin.emplace(Ingredient[i].name, Ingredient[i].name);
        Cookkind.emplace(Ingredient[i].name, 0);
    }
    for (int i = 0; i < recipeCount; i++)
    {
        assert(Recipe[i].nameAfter[1] == '_');
        switch (Recipe[i].nameAfter[2])
        {
        case 'f':
            Origin.emplace(Recipe[i].nameAfter, "fish");
            break;
        case 'r':
            Origin.emplace(Recipe[i].nameAfter, "rice");
            break;
        case 'k':
            Origin.emplace(Recipe[i].nameAfter, "kelp");
            break;
        default:
            assert(0);
            break;
        }
        switch (Recipe[i].nameAfter[0])
        {
        case 'c':
            Cookkind.emplace(Recipe[i].nameAfter, 1);
            break;
        case 's':
            if (Origin.find(Recipe[i].nameAfter)->second == "rice")
            {
                Cookkind.emplace(Recipe[i].nameAfter, 2);
            }
            else if (Origin.find(Recipe[i].nameAfter)->second == "fish")
            {
                Cookkind.emplace(Recipe[i].nameAfter, 3);
            }
            assert(Origin.find(Recipe[i].nameAfter)->second == "fish" || Origin.find(Recipe[i].nameAfter)->second == "rice");
            break;
        case 'p':
            if (Origin.find(Recipe[i].nameAfter)->second == "fish")
            {
                Cookkind.emplace(Recipe[i].nameAfter, 3);
            }
            break;
        default:
            assert(0);
            break;
        }
    }
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
        curplates = 0;
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
            {
                curplates++;
            }
        }
        for (int ptype = 0; ptype <= 1; ptype++)
        {
            // if (!(taketask[ptype].id == 2 && curplates == 0))
            //{
            taketask[ptype] = arrangetask(ptype);
            if (taketask[ptype].id != -1)
            {

                playerAction[ptype] = frame_move(taketask[ptype].x, taketask[ptype].y, ptype, taketask[ptype].op);
                if (playerAction[ptype][0] == 'P')
                {
                    switch (taketask[ptype].id)
                    {
                    case 1:
                        assert(Cookkind.find(currentdish.dish[currentdish.cur]) != Cookkind.end());
                        switch (Cookkind.find(currentdish.dish[currentdish.cur])->second)
                        {
                        case 0:
                            taketask[ptype].id = 2;
                            break;
                        case 3: // sfish
                        case 1: // cfish
                            taketask[ptype].id = 5;
                            break;
                        case 2:
                            taketask[ptype].id = 6;
                            taketask[ptype].flag = 1; // 1 for pot
                            break;
                        default:
                            cerr << "fill me 1" << endl;
                            assert(0);
                            break;
                        }
                        break;
                    case 2:
                        assert(ptype == 0);
                        currentdish.cur++;
                        // hjx change 0 to -1
                        if (currentdish.cur == currentdish.dish.size())
                        { // 最后一个
                            if (taketask[(ptype + 1) % 2].id == -1)
                            {
                                taketask[ptype].id = -1;
                                taketask[(ptype + 1) % 2].id = 3;
                                taketask[(ptype + 1) % 2].x = taketask[ptype].x;
                                taketask[(ptype + 1) % 2].y = taketask[ptype].y;
                            }
                            else
                            {
                                taketask[ptype].id = 3;
                            }
                        }
                        else
                        {
                            taketask[ptype].id = -1;
                        }
                        break;
                    case 3:
                        // 拿盘子
                        /*cerr << ptype << endl;
                        assert(taketask[ptype].flag != 0);
                        if (taketask[ptype].flag < taketask[ptype].sum)
                        {
                            if (ptype == 0)
                            {
                                // 放盘子 task 9?
                                int todo = 1;
                            }
                            else
                            {
                                taketask[1].id = -1;
                            }
                        }
                        else
                        {*/
                        taketask[ptype].id = 4;
                        //}
                        break;
                    case 4:
                        // assert(whosent == ptype);
                        dishinplate.clear();
                        curorder--;
                        cerr << "curorder" << curorder << endl;
                        assert(curorder >= -1);
                        taketask[ptype].id = -1;
                        break;
                    case 5:
                        if (taketask[ptype].flag == 1)
                        { // last step
                            assert(Cookkind.find(currentdish.dish[currentdish.cur])->second == 1 || Cookkind.find(currentdish.dish[currentdish.cur])->second == 3);
                            if (Cookkind.find(currentdish.dish[currentdish.cur])->second == 1)
                            {
                                taketask[ptype].id = 2;
                                taketask[ptype].flag = 1;
                            }
                            else if (Cookkind.find(currentdish.dish[currentdish.cur])->second == 3)
                            {
                                cerr << "here 5" << endl;
                                taketask[ptype].id = 6;
                                taketask[ptype].flag = 2;
                            }
                        }
                        break;
                    case 6:
                        assert(ptype == 0);
                        currentdish.cur++;
                        // if (currentdish.cur == 1 && currentdish.dish.size() == 1)
                        if (currentdish.cur == currentdish.dish.size())
                        { // 最后一个
                            if (taketask[(ptype + 1) % 2].id == -1)
                            {
                                taketask[ptype].id = -1;
                                taketask[(ptype + 1) % 2].id = 7;
                                taketask[(ptype + 1) % 2].flag = taketask[ptype].flag;
                                taketask[(ptype + 1) % 2].sum = taketask[ptype].sum;
                                dishinplate.clear();
                                for (int k = 0; k < currentdish.cur - 1; k++)
                                {
                                    dishinplate.push_back(currentdish.dish[k]);
                                }
                            }
                            else
                            {
                                taketask[ptype].id = 7;
                            }
                        }

                        else
                        {
                            taketask[ptype].id = 7;
                        }

                    case 7:
                        if (Players[ptype].containerKind == ContainerKind::Plate)
                        {
                            cerr << "case 7" << endl;
                            if (ptype == 0)
                            {
                                if (Players[ptype].entity.size() == taketask[ptype].sum - 1)
                                {
                                    cerr << "case 7 1" << endl;
                                    taketask[ptype].id = 4;
                                }
                                else
                                { // always because of s
                                    cerr << "case 7 2" << endl;
                                    int todo = 1;
                                    taketask[ptype].id = 8;
                                }
                            }
                            else
                            {
                                taketask[ptype].id = 4; // fix
                            }
                        }
                        break;
                    case 8:
                        assert(ptype == 0);
                        assert(Players[ptype].containerKind == ContainerKind::Plate);
                        taketask[ptype].id = -1;
                        break;
                    }
                }
            }
            else
            {
                // 清空
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        if (Player_Map[i][j] == (ptype + '0'))
                            Player_Map[i][j] = '.';
                    }
                }
                int x = Players[ptype].x / block, y = Players[ptype].y / block;
                cerr << "Now -1:" << ptype << ": " << x << " " << y << endl;
                Player_Map[y][x] = ptype + '0';
            }
            //}
        }
        /* 合成一个字符串再输出，否则输出有可能会被打断 */
        std::string action = playerAction[0] + "\n" + playerAction[1] + "\n";
        // std::string action = player0_Action + "\n" + player1_Action + "\n";
        std::cout << action;

        /* 不要忘记刷新输出流，否则游戏将无法及时收到响应 */
        std::cout.flush();
    }
}
