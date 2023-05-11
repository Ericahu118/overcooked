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

using namespace std;

struct Task
{
    int id;
    int x, y, op; // 位置，操作
    bool flag = 0;
};

Task arrangetask(int ptype);

#endif