#ifndef FRAMEWORK
#define FRAMEWORK

#include <string>
#include <vector>
#include <enum.h>

const double radius = 0.35;
const double interdis = 1.3;
const double center = 0.5;
const double block = 1;

struct Ingredient
{
    int x, y, price;  // 原料箱位置，原料价格
    std::string name; // 原料名称
};

struct Recipe
{
    int time;                          // 加工时间
    std::string nameBefore, nameAfter; // 加工前name,加工后name
    std::string kind;                  // 加工类型
};

struct Order
{
    int validFrame;                  // 有效时间
    int price;                       // 赚钱
    int frequency;                   // 权重
    std::vector<std::string> recipe; // 食材
};

struct Player
{
    double x, y; // 位置
    double X_Velocity;
    double Y_Velocity;
    int live;
    ContainerKind containerKind;
    std::vector<std::string> entity;
};

struct Entity
{
    double x, y;                     // 坐标
    ContainerKind containerKind;     // 实体容器
    std::vector<std::string> entity; // ？
    int currentFrame, totalFrame;
    int sum;
};

/* 初始化时的读入。 */
void init_read();

/* 每一帧的读入；返回：是否跳帧。 */
bool frame_read(int nowFrame);

#endif
