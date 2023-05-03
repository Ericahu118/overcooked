#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <math.h>
#include <framework.h>

const double radius = 0.35;
const double interdis = 1.3;
const double center = 0.5;

extern Player Players[2 + 5];
extern struct Ingredient Ingredient[20 + 5];
extern char Map[20 + 5][20 + 5];
int worktype[2 + 5]; // 0 for dish 1 for wash
int flag = 0;
char Moveplan(double x, double y, int ptype)
{
    return 'N';
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

    int totalFrame = 14400;
    for (int i = 0; i < totalFrame; i++)
    {
        bool skip = frame_read(i);
        if (skip)
            continue;

        /* 输出当前帧的操作，此处仅作示例 */

        std::string player0_Action = "Move L";
        std::string player1_Action = "Move";

        if (Players[0].x <= 2 && Players[0].entity.empty())
        {
            // flag = 1;
            player0_Action = "PutOrPick L";
        }
        if (!Players[0].entity.empty())
        {
            player0_Action = "Move";
        }
        if (Players[0].X_Velocity == 0)
            Players[0].x = 1.5;

        /* 合成一个字符串再输出，否则输出有可能会被打断 */
        std::string action = player0_Action + "\n" + player1_Action + "\n";
        std::cout << action;

        /* 不要忘记刷新输出流，否则游戏将无法及时收到响应 */
        std::cout.flush();
    }
}
