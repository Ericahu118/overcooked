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
extern Ingredient Ingredient[20 + 5];
extern char Map[20 + 5][20 + 5];
int worktype[2 + 5]; // 0 for dish 1 for wash
char Moveplan(double x, double y, int ptype)
{
    // 遵循先左右后上下 不能交互就要移动的策略
    int line = ceil(Players[ptype].x + center) - 1;
    int row = ceil(Players[ptype].y + center) - 1;
    int line1 = ceil(Players[(ptype + 1) % 2].x + center) - 1;
    int row1 = ceil(Players[(ptype + 1) % 2].y + center) - 1;
    double dis = (Players[(ptype + 1) % 2].y-Players[ptype].y)*(Players[(ptype + 1) % 2].y-Players[ptype].y)+(Players[(ptype + 1) % 2].x-Players[ptype].x)*(Players[(ptype + 1) % 2].x-Players[ptype].x);
    std::cerr << "player row: " << row << " line:" <<line << std::endl;
    std::cerr << "des row: " << x << " line:" <<y<< std::endl;

    if (x < Players[ptype].x)
    {
        // 左
        if (line > x + 1)
        { // 不能交互
            if (Map[row][line - 1] == '.' && (row != row1 || (line - 1) != line1))
            { // 可走
                return 'L';
            }
        }
        else
        {
            return 'U';
            if (y < Players[ptype].y)
            {
                // 上
                if (row > y + 1)
                { // 不能交互
                    //std::cerr << "map:" << Map[row - 1][line] << std::endl;
                    //std::cerr << "dis:" << dis<< std::endl;
                    if (Map[row - 1][line] == '.' && dis > 0.5)
                    {
                        // 可走
                        return 'U';
                    }

                }
            }
            else if (y > Players[ptype].y)
            {
                if (row < y - 1)
                {
                    // 不能交互
                    if (Map[row + 1][line] == '.' && ((row + 1) != row1 || line != line1))
                    {
                        // 可走
                        return 'D';
                    }
                }
            }
        }
    }
    else if (x > Players[ptype].x)
    {
        // 右
        if (line < x - 1)
        { // 不能交互
            if (Map[row][line + 1] == '.' && (row != row1 || (line + 1) != line1))
            { // 可走
                return 'R';
            }
            else
            {
                if (y < Players[ptype].y)
                {
                    // 上
                    if (row > y + 1)
                    { // 不能交互
                        if (Map[row - 1][line] == '.' && ((row - 1) != row1 || line != line1))
                        {
                            // 可走
                            return 'U';
                        }
                    }
                }
                else if (y > Players[ptype].y)
                {
                    if (row < y - 1)
                    {
                        // 不能交互
                        if (Map[row + 1][line] == '.' && ((row + 1) != row1 || line != line1))
                        {
                            // 可走
                            return 'D';
                        }
                    }
                }
            }
        }
    }
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
        std::cout << "Frame " << i << "\n";
        char move0 = 'N';
        char move1 = Moveplan(4,0,1);
        std::string player0_Action;
        std::string player1_Action;
        switch(move0){
        case 'U':
            player0_Action = "Move U";break;
        case 'R':
            player0_Action = "Move R";break;
        case 'D':
            player0_Action = "Move D";break;
        case 'L':
            player0_Action = "Move L";break;
        default:
            player0_Action = "Move";break;
        }
        switch(move1){
        case 'U':
            player1_Action = "Move U";break;
        case 'R':
            player1_Action = "Move R";break;
        case 'D':
            player1_Action = "Move D";break;
        case 'L':
            player1_Action = "Move L";break;
        default:
            player1_Action = "Move";break;
        }

        /* test
        std::cerr << "player x: " << Players[0].x << "x" <<Ingredient[0].x  << std::endl;//可以输出到log.txt文件中
        if(Players[0].x - Ingredient[0].x <= 2.3 && Players[0].entity.empty()){
            player0_Action ="PutOrPick L";
        }
        if(!Players[0].entity.empty()){
            player0_Action ="Move R";
        }
        test*/

        /* 合成一个字符串再输出，否则输出有可能会被打断 */
        std::string action = player0_Action + "\n" + player1_Action + "\n";
        std::cout << action;

        /* 不要忘记刷新输出流，否则游戏将无法及时收到响应 */
        std::cout.flush();
    }
}
