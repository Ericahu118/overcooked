#ifndef MOVEMENT
#define MOVEMENT

#include <string>
#include <vector>
#include <enum.h>
#include <framework.h>
using namespace std;
extern Player Players[2 + 5];
extern struct Ingredient Ingredient[20 + 5];
extern int IngredientCount;
extern char Map[20 + 5][20 + 5];
extern int entityCount; // 实体
extern struct Entity Entity[20 + 5];
extern struct Order totalOrder[20 + 5]; // 总订单数组，
extern int orderCount;                  // 订单数
extern struct Order Order[20 + 5];

int dir[2 + 5];

int changedir(double des_x, double des_y, int ptype);

string frame_move(double des_x, double des_y, int ptype, int op);

#endif