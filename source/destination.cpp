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

int curplates;
Task taketask[2 + 5];
int curorder = 0;

map<string, string> Origin;
map<string, int> Cookkind;

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
            if (taketask[1].id == 0)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty() && *Entity[i].entity.begin() == *Order[0].recipe.begin())
                    {
                        task.id = 3;
                        task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                        return task;
                    }
                }
            }
            for (int i = 0; i < IngredientCount; i++)
            {
                string need = Order[curorder].recipe[0];
                cerr << "need:" << need << endl;
                if (Ingredient[i].name == Origin.find(need)->second)
                {
                    task.id = 1;
                    task.op = 0, task.x = Ingredient[i].x, task.y = Ingredient[i].y, task.flag = 0;
                    curorder++;
                    return task;
                }
            }
            cerr << "Must have ingredient" << endl;
            assert(0);
        }
        else
        {
            if (taketask[(ptype + 1) % 2].id != 3)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty() && *Entity[i].entity.begin() == *Order[0].recipe.begin())
                    {
                        task.id = 3;
                        task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                        return task;
                    }
                }
            }
            // 是否需要洗盘子
            if (curplates == 0)
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
    else if (taketask[ptype].id == 1)
    {
        /*for (int i = 0; i < IngredientCount; i++)
        {
            string need = Order[0].recipe[0];
            if (Ingredient[i].name.compare(need) == 0)
            {
                task.id = 1;
                task.op = 0, task.x = Ingredient[i].x, task.y = Ingredient[i].y, task.flag = 0;
                return task;
            }
        }*/
        return taketask[ptype];
    }
    else if (taketask[ptype].id == 2)
    {
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
            {
                task.id = 2;
                task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                return task;
            }
        }

        cerr << "have no plates" << endl;
        taketask[ptype].op = 2;
        return taketask[ptype];
    }
    else if (taketask[ptype].id == 3)
    {
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty() && *Entity[i].entity.begin() == *Order[0].recipe.begin())
            {
                task.id = 3;
                task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                return task;
            }
        }
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
            task.op = 0, task.x = cutplate.first, task.y = cutplate.second, task.flag = 0;
            return task;
        }
        else
        {
            for (int i = 0; i < entityCount; i++)
            {
                if (!Entity[i].entity.empty() && Entity[i].entity[0][0] != 'c' && Entity[i].x == cutplate.first && Entity[i].y == cutplate.second)
                {
                    task.id = 5;
                    task.op = 1, task.x = cutplate.first, task.y = cutplate.second, task.flag = 0;
                    return task;
                }
            }
            for (int i = 0; i < entityCount; i++)
            {
                if (!Entity[i].entity.empty() && Entity[i].entity[0][0] == 'c' && Entity[i].x == cutplate.first && Entity[i].y == cutplate.second)
                {
                    task.id = 5;
                    task.op = 0, task.x = cutplate.first, task.y = cutplate.second, task.flag = 1;
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
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 1;
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
                if (Entity[i].containerKind == ContainerKind::Pot && Entity[i].entity.empty())
                {
                    task.id = 6;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 2;
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
        { // 去拿盘子
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
                {
                    task.id = 7;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = taketask[ptype].flag;
                    return task;
                }
            }
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
                    if (Entity[i].containerKind == ContainerKind::Pot)
                    {
                        assert(!Entity[i].entity.empty());
                        cerr << "have pot" << endl;
                        task.id = 7;
                        task.op = 2;
                        task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 1;
                        if (Entity[i].currentFrame >= Entity[i].totalFrame)
                        {
                            task.op = 0;
                        }
                        return task;
                    }
                }
                // no pot
                assert(0);
                taketask[ptype].op = 2;
                return taketask[ptype];
            }
            else if (taketask[ptype].flag == 2)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Pan)
                    {
                        assert(!Entity[i].entity.empty());
                        task.id = 7;
                        task.op = 2;
                        task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 2;
                        if (Entity[i].currentFrame >= Entity[i].totalFrame)
                        {
                            task.op = 0;
                        }
                        return task;
                    }
                }
                // no pot
                assert(0);
                taketask[ptype].op = 2;
                return taketask[ptype];
            }
            return task;
        }
    }
    //////////
    cerr << "player[" << ptype << "]"
         << " takes no task" << endl;
    assert(task.id == -1);
    return task;
}
