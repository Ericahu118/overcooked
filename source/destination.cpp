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

using namespace std;

int curplates;
Task taketask[2 + 5];
int curorder = -1;

map<string, string> Origin;
map<string, int> Cookkind;

Dishes currentdish;
queue<pair<double, double>> topick;
int testcount = 0;

pair<int, vector<int>> whosent;
int cursent;

// 0 for wash

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
extern pair<double, double> cutplate;

bool checkdish(int i, int n)
{
    if (Entity[i].containerKind == ContainerKind::Plate)
    {
        int total = 0;
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < Entity[i].entity.size(); k++)
            {
                if (Entity[i].entity[k] == currentdish.dish[j])
                    total++;
            }
        }
        if (total == n)
        {
            return true;
        }
    }
    return false;
}

Task arrangetask(int ptype)
{
    Task task;
    task.id = -1;
    task.op = 0, task.x = 0, task.y = 0, task.flag = 0;
    cerr << "player[" << ptype << "]"
         << " take task:" << taketask[ptype].id << endl;
    if (taketask[ptype].id == -1)
    {
        if (ptype == 0)
        {

            if (currentdish.cur >= currentdish.dish.size())
            {
                currentdish.cur = 0;
                curorder++;
                testcount++;
                cerr << "testcount: " << testcount << endl;
                // pre work know just copy
                assert(curorder >= 0);
                currentdish.dish.clear();
                cursent = -1;
                // pre work
                for (int i = 0; i < Order[curorder].recipe.size(); i++)
                {
                    if (Order[curorder].recipe[i][1] != '_')
                        currentdish.dish.push_back(Order[curorder].recipe[i]);
                }
                for (int i = 0; i < Order[curorder].recipe.size(); i++)
                {
                    if (Order[curorder].recipe[i][0] == 'c')
                        currentdish.dish.push_back(Order[curorder].recipe[i]);
                }
                for (int i = 0; i < Order[curorder].recipe.size(); i++)
                {
                    if (Order[curorder].recipe[i][0] == 's')
                        currentdish.dish.push_back(Order[curorder].recipe[i]);
                }
            }
            for (int i = 0; i < IngredientCount; i++)
            {
                string need = currentdish.dish[currentdish.cur];
                cerr << "need:" << need << endl;
                if (Ingredient[i].name == Origin.find(need)->second)
                {
                    task.id = 1;
                    task.op = 0, task.x = Ingredient[i].x, task.y = Ingredient[i].y, task.flag = 0, task.sum = currentdish.dish.size();
                    return task;
                }
            }

            cerr << "Must have ingredient" << endl;
            assert(0);
        }
        else
        {
            // fix
            /*if (taketask[(ptype + 1) % 2].id != 3)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (checkdish(i))
                    {
                        task.id = 3;
                        task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                        return task;
                    }
                }
            }*/
            // fix!!
            /*if (taketask[(ptype + 1) % 2].id != 7 && Players[ptype].containerKind == ContainerKind::Plate)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if ((Entity[i].containerKind == ContainerKind::Pot && !Entity->entity.empty()) || (Entity[i].containerKind == ContainerKind::Pan && !Entity->entity.empty()))
                    {
                        task.id = 7;
                        task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y;
                        if (Entity[i].containerKind == ContainerKind::Pot)
                            task.flag = 1;
                        else
                            task.flag = 2;
                        return task;
                    }
                }
            }*/
            if (Players[ptype].containerKind == ContainerKind::Plate)
            {
                assert(whosent.first == 1);
                int tmp = 0;
                for (int k = 0; k < Players[ptype].entity.size(); k++)
                {
                    if (Players[ptype].entity[k][0] != 's')
                        tmp++;
                }
                assert(!whosent.second.empty());
                /* task.id = 7;
                 task.op = 0;
                 task.sum = taketask[ptype].sum;*/
                cerr << "whosent" << whosent.second[Players[0].entity.size() - tmp] << endl;
                for (int i = 0; i < entityCount; i++)
                {
                    if (whosent.second[Players[0].entity.size() - tmp] == 1 && Entity[i].containerKind == ContainerKind::Pot)
                    {
                        task.id = 7;
                        task.op = 0;
                        task.sum = taketask[ptype].sum;
                        task.x = Entity[i].x, task.y = Entity[i].y;
                        task.flag = 1;
                        return task;
                    }
                    else if (whosent.second[Players[0].entity.size() - tmp] == 2 && Entity[i].containerKind == ContainerKind::Pan)
                    {
                        task.id = 7;
                        task.op = 0;
                        task.sum = taketask[ptype].sum;
                        task.x = Entity[i].x, task.y = Entity[i].y;
                        task.flag = 2;
                        return task;
                    }
                }
                cerr << "Not here" << endl;
            }
            // 是否需要洗盘子
            else
            {
                if (curplates == 0 && Players[ptype].containerKind == ContainerKind::None)
                {
                    for (int i = 0; i < entityCount; i++)
                    {
                        if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                        {
                            task.id = 0;
                            task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                            return task;
                        }
                    }
                }
            }
        }
    }
    else if (taketask[ptype].id == 1)
    {
        return taketask[ptype];
    }
    else if (taketask[ptype].id == 2)
    {
        assert(currentdish.cur >= 0 && currentdish.cur < currentdish.dish.size());
        if (currentdish.cur == 0)
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
                {
                    task.id = 2;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0, task.sum = taketask[ptype].sum;
                    return task;
                }
            }
        }
        else if (currentdish.cur > 0)
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (checkdish(i, currentdish.cur))
                {

                    task.id = 2;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0, task.sum = taketask[ptype].sum;
                    return task;
                }
            }
        }

        cerr << "have no plates" << endl;
        taketask[ptype].op = 2;
        return taketask[ptype];
    }
    else if (taketask[ptype].id == 3)
    { // 3 一定由2 转移
        task.flag = 0;
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].x == taketask[ptype].x && Entity[i].y == taketask[ptype].y && Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty())
            {
                cerr << "here 33" << endl;
                task.flag = Entity[i].entity.size();
            }
        }
        task.id = 3;
        task.op = 0, task.x = taketask[ptype].x, task.y = taketask[ptype].y, task.sum = taketask[ptype].sum;
        return task;
    }
    else if (taketask[ptype].id == 4)
    {
        task.id = 4;
        task.op = 0, task.x = window.first, task.y = window.second, task.flag = 0;
        return task;
    }
    else if (taketask[ptype].id == 0)
    {

        if (Players[ptype].containerKind == ContainerKind::DirtyPlates)
        {
            task.id = 0;
            task.op = 0, task.x = sink.first, task.y = sink.second, task.flag = 0;
            return task;
        }
        else
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::DirtyPlates && Entity[i].x == sink.first && Entity[i].y == sink.second)
                {
                    task.id = 0;
                    task.op = 1, task.x = sink.first, task.y = sink.second, task.flag = 0;
                    return task;
                }
            }
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                {
                    task.id = 0;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                    return task;
                }
            }
        }
        taketask[ptype].id = -1; // 洗完了
    }
    else if (taketask[ptype].id == 5)
    {
        if (!Players[ptype].entity.empty())
        {
            task.id = 5;
            task.op = 0, task.x = cutplate.first, task.y = cutplate.second, task.flag = 0, task.sum = taketask[ptype].sum;
            return task;
        }
        else
        {
            for (int i = 0; i < entityCount; i++)
            {
                // fix
                if (!Entity[i].entity.empty() && Entity[i].entity[0][0] != 'c' && Entity[i].x == cutplate.first && Entity[i].y == cutplate.second)
                {
                    task.id = 5;
                    task.op = 1, task.x = cutplate.first, task.y = cutplate.second, task.flag = 0, task.sum = taketask[ptype].sum;
                    return task;
                }
            }
            for (int i = 0; i < entityCount; i++)
            {
                if (!Entity[i].entity.empty() && Entity[i].entity[0][0] == 'c' && Entity[i].x == cutplate.first && Entity[i].y == cutplate.second)
                {
                    task.id = 5;
                    task.op = 0, task.x = cutplate.first, task.y = cutplate.second, task.flag = 1, task.sum = taketask[ptype].sum;
                    return task;
                }
            }
            cerr << "Not reach here 5" << endl;
            assert(0);
        }
    }
    else if (taketask[ptype].id == 6)
    {
        assert(taketask[ptype].flag == 1 || taketask[ptype].flag == 2);
        if (taketask[ptype].flag == 1)
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Pot && Entity[i].entity.empty())
                {
                    task.id = 6;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.sum = taketask[ptype].sum;
                    task.flag = 1;
                    return task;
                }
            }
            // no pot
            taketask[ptype].op = 2;
            return taketask[ptype];
        }
        else if (taketask[ptype].flag == 2)
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Pan && Entity[i].entity.empty())
                {
                    task.id = 6;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 2, task.sum = taketask[ptype].sum;
                    return task;
                }
            }
            taketask[ptype].op = 2;
            return taketask[ptype];
        }
        return task;
    }
    else if (taketask[ptype].id == 7)
    {
        if (Players[ptype].containerKind == ContainerKind::None)
        { // 去拿盘子 fix
            for (int i = 0; i < entityCount; i++)
            {
                if (ptype == 0)
                {
                    if (checkdish(i, currentdish.cur - 1))
                    {
                        cerr << "test 7" << endl;
                        task.id = 7, task.x = Entity[i].x, task.y = Entity[i].y,
                        task.op = 0, task.flag = taketask[ptype].flag, task.sum = taketask[ptype].sum;
                        return task;
                    }
                }
                else
                {
                    // 只拿空盘子
                    if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
                    {
                        cerr << "test 7" << endl;
                        task.id = 7, task.x = Entity[i].x, task.y = Entity[i].y,
                        task.op = 0, task.flag = taketask[ptype].flag, task.sum = taketask[ptype].sum;
                        return task;
                    }
                }
            }

            ////
            /*for (int i = 0; i < entityCount; i++)
             {
                 if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
                 {
                     task.id = 7;
                     task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = taketask[ptype].flag;
                     return task;
                 }
             }*/
            cerr << "have no plates" << endl;
            taketask[ptype].op = 2;
            return taketask[ptype];
        }
        else
        {
            assert(Players[ptype].containerKind == ContainerKind::Plate);
            assert(taketask[ptype].flag == 1 || taketask[ptype].flag == 2);
            if (taketask[ptype].flag == 1)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Pot && !Entity[i].entity.empty())
                    {
                        // assert(!Entity[i].entity.empty());
                        cerr << "have pot" << endl;
                        task.id = 7;
                        task.op = 2;
                        task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 1, task.sum = taketask[ptype].sum;
                        if (Entity[i].currentFrame >= Entity[i].totalFrame)
                        {
                            task.op = 0;
                        }
                        return task;
                    }
                }
                // no pot
                // assert(0);
                taketask[ptype].op = 2;
                return taketask[ptype];
            }
            else if (taketask[ptype].flag == 2)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Pan && !Entity[i].entity.empty())
                    {
                        // assert(!Entity[i].entity.empty());
                        task.id = 7;
                        task.op = 2;
                        task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 2, task.sum = taketask[ptype].sum;
                        if (Entity[i].currentFrame >= Entity[i].totalFrame)
                        {
                            task.op = 0;
                        }
                        return task;
                    }
                }
                // no pot
                // assert(0);
                taketask[ptype].op = 2;
                return taketask[ptype];
            }
            return task;
        }
    }
    //////////
    cerr << "player[" << ptype << "]"
         << " takes no task:" << endl;
    assert(task.id == -1);
    return task;
}
