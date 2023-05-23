#ifndef MOVEMENT
#define MOVEMENT

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>
#include <destination.h>

// int changedir(double des_x, double des_y, int ptype);
int bfs(int ptype, int des_x, int des_y);
std::string frame_move(double des_x, double des_y, int ptype, int op);
#endif
