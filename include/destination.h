#ifndef DESTINATION
#define DESTINATION

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>
#include <movement.h>
#include <map>

using namespace std;

struct Task
{
    int id;
    int x, y, op; // 位置，操作
    int flag = 0; // 附加
};
map<string, string> Origin;
map<string, int> Cookstep;

Task arrangetask(int ptype);

#endif