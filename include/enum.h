#ifndef ENUM
#define ENUM
#include <cassert>
#include <string>

enum class Worktype
{
    None = -1,
    Washdish,   // 0
    Takeorder,  // 1 flag=4 for cut
    PutinPlate, // 2
    TakeFood,   // 3
    SendFood,   // 4
    CutFood,    // 5->2 or 5->6
    FireFood,   // 6->2
};

enum class ContainerKind
{
    None,
    Pan,
    Pot,
    Plate,
    DirtyPlates,
};

enum class TileKind
{
    None,
    Void,
    Floor,
    Wall,
    Table,
    IngredientBox,
    Trashbin,
    ChoppingStation,
    ServiceWindow,
    Stove,
    PlateReturn,
    Sink,
    PlateRack,
};

inline TileKind getTileKind(char kindChar)
{
    switch (kindChar)
    {
    case '_':
        return TileKind::Void;
    case '.':
        return TileKind::Floor;
    case '*':
        return TileKind::Table;
    case 't':
        return TileKind::Trashbin;
    case 'c':
        return TileKind::ChoppingStation;
    case '$':
        return TileKind::ServiceWindow;
    case 's':
        return TileKind::Stove;
    case 'p':
        return TileKind::PlateReturn;
    case 'k':
        return TileKind::Sink;
    case 'r':
        return TileKind::PlateRack;
    default:
        assert(0);
    }
}

inline char getAbbrev(TileKind kind)
{
    switch (kind)
    {
    case TileKind::IngredientBox:
        return 'i';
    case TileKind::Trashbin:
        return 't';
    case TileKind::ChoppingStation:
        return 'c';
    case TileKind::ServiceWindow:
        return '$';
    case TileKind::Stove:
        return 's';
    case TileKind::PlateReturn:
        return 'p';
    case TileKind::Sink:
        return 'k';
    case TileKind::PlateRack:
        return 'r';
    default:
        assert(0);
    }
}

#endif