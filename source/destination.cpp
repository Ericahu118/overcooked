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
bool changephase[2 + 5];
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

Task arrangetask(int ptype)
{
    changephase[ptype] = 0;
    Task task;
    task.id = -1;
    task.op = 0, task.x = 0, task.y = 0, task.flag = 0;
    cerr << "player[" << ptype << "]"
         << " take task:" << taketask[ptype].id << endl;
    if (taketask[ptype].id == -1)
    {
        if (ptype == 0)
        {
            int takenow = 0;
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::Plate && *Entity[i].entity.begin() == *Order[0].recipe.begin())
                {
                    if (Order[0].recipe[0] != Order[1].recipe[0])
                    {
                        takenow = 1;
                    }
                }
            }
            if ((!Players[1].entity.empty() && Players[1].containerKind == ContainerKind::Plate) && (Order[0].recipe[0] != Order[1].recipe[0]))
            {
                takenow = 1;
            }

            for (int i = 0; i < IngredientCount; i++)
            {
                string need = Order[takenow].recipe[0];
                if (Ingredient[i].name.compare(need) == 0)
                {
                    task.id = 1;
                    task.op = 0, task.x = Ingredient[i].x, task.y = Ingredient[i].y, task.flag = 0;
                    return task;
                }
            }
        }
        else
        {
            if (taketask[(ptype + 1) % 2].id != 3)
            {
                for (int i = 0; i < entityCount; i++)
                {
                    if (Entity[i].containerKind == ContainerKind::Plate && *Entity[i].entity.begin() == *Order[0].recipe.begin())
                    {
                        task.id = 3;
                        task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                        return task;
                    }
                }
            }
            // 是否需要洗盘子
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
        cerr << "always have plates" << endl;
        assert(0);
    }
    else if (taketask[ptype].id == 3)
    {
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate && *Entity[i].entity.begin() == *Order[0].recipe.begin())
            {
                task.id = 3;
                task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                return task;
            }
        }
    }
    else if (taketask[ptype].id == 4)
    {
        // tofix
        // assert(Players[ptype].containerKind == ContainerKind::Plate && *Players[ptype].entity.begin() == *Order[0].recipe.begin());
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

    //////////

    // 正在洗碗中
    /*if (taketask[ptype].id == 0)
    {
        if (Players[ptype].containerKind == ContainerKind::DirtyPlates)
        { // 送碗
            task.id = 0;
            task.op = 0, task.x = sink.first, task.y = sink.second, task.flag = 0;
            return task;
        }
        else
        { // 洗碗
            for (int i = 0; i < entityCount; i++)
            {
                if (Entity[i].containerKind == ContainerKind::DirtyPlates)
                {
                    if (Entity[i].x == sink.first && Entity[i].y == sink.second)
                    {
                        task.id = 0;
                        task.op = 1, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 1;
                        return task;
                    }
                }
            }
        }
        cerr << "Not reach here1 in arrangetask" << endl;
        assert(0);
    }
    else if (taketask[ptype].id == 1)
    {
        cerr << "step 1:2" << endl;
        assert(!Players[ptype].entity.empty());
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate && Entity[i].entity.empty())
            {
                task.id = 1;
                task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 1;
                return task;
            }
        }
        cerr << "Not reach here2 in arrangetask" << endl;
        assert(0);
    }
    else if (taketask[ptype].id == 2)
    {
        assert(Players[ptype].containerKind == ContainerKind::Plate);
        task.id = 2;
        task.op = 0, task.x = window.first, task.y = window.second, task.flag = 1;
        return task;
    }
    // 以下是当前无任务时
    // 是否要洗盘子
    if (curplates == 0)
    { // 当前没有盘子
        if (taketask[(ptype + 1) % 2].id != 0)
        { // 无人洗盘子
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
    // 是否有人送盘子
    if (taketask[(ptype + 1) % 2].id != 2)
    {
        for (int i = 0; i < entityCount; i++)
        {
            if (Entity[i].containerKind == ContainerKind::Plate && !Entity[i].entity.empty())
            {
                if (Entity[i].entity[0] == Order[0].recipe[0]) // 做好了
                {
                    task.id = 2;
                    task.op = 0, task.x = Entity[i].x, task.y = Entity[i].y, task.flag = 0;
                    return task;
                }
            }
        }
    }
    // 是否有人点单拿菜
    if (taketask[(ptype + 1) % 2].id != 1)
    {
        if (curplates > 0)
        {
            cerr << "Why" << endl;
            curplates--;
            for (int i = 0; i < IngredientCount; i++)
            {
                string need = Order[0].recipe[0];
                if (Ingredient[i].name.compare(need) == 0)
                {
                    task.id = 1;
                    task.op = 0, task.x = Ingredient[i].x, task.y = Ingredient[i].y, task.flag = 0;
                    return task;
                }
            }
        }
    }*/
    cerr << "player[" << ptype << "]"
         << " takes no task" << endl;
    assert(task.id == -1);
    return task;
}
