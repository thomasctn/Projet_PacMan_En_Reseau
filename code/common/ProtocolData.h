#pragma once
#include "Types.h"
#include "Constants.h"
namespace std
{
    template <>
    struct hash<gf::Vector2i>
    {
        std::size_t operator()(const gf::Vector2i &p) const noexcept
        {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };

    template <>
    struct equal_to<gf::Vector2i>
    {
        constexpr bool operator()(const gf::Vector2i &lhs, const gf::Vector2i &rhs) const
        {
            return lhs.x == rhs.x && lhs.y == rhs.y;
        }
    };
}
//Incroyable !
namespace gf
{
    template <typename Archive, typename T>
    Archive &operator|(Archive &ar, gf::v1::Vector<T, 2> &data)
    {
        return ar | data.x | data.y;
    }
}

struct CaseCommon
{
    CellType celltype;
    bool pacGomme = false;
    CaseCommon() : celltype(CellType::Floor) {}
    CaseCommon(CellType t)
    {
        celltype = t;
    }
};
template <typename Archive>
Archive &operator|(Archive &ar, CaseCommon &data)
{
    return ar | data.celltype;
}

struct BoardCommon
{
    BoardCommon() {};
    BoardCommon(unsigned int w, unsigned int h) : grid({w, h}), width(w), height(h) {}
    unsigned int width;
    unsigned int height;
    gf::Array2D<CaseCommon> grid;
};
template <typename Archive>
Archive &operator|(Archive &ar, BoardCommon &data)
{
    return ar | data.width | data.height | data.grid;
}

struct PlayerData
{
    uint32_t id;
    float x, y;
    uint32_t color; // RGBA
    std::string name;
    PlayerRole role;
    int score;
    bool ready = false;
    unsigned int hp = 0;
};
template <typename Archive>
Archive &operator|(Archive &ar, PlayerData &data)
{
    return ar | data.id | data.x | data.y | data.color | data.name | data.role | data.score | data.ready | data.hp;
}

struct RoomSettings
{
    unsigned int roomSize = MIN_NB_PLAYERS;
    unsigned int nbBot = MIN_NB_BOTS;
    unsigned int gameDuration = MIN_DURATION;
    unsigned int nbLifePacman = MIN_HP_PACMAN;
    unsigned int nbLifeGhost = MIN_HP_GHOST;
};
template <typename Archive>
Archive &operator|(Archive &ar, RoomSettings &data)
{
    return ar | data.roomSize | data.nbBot | data.gameDuration | data.nbLifePacman | data.nbLifeGhost;
}

struct RoomData
{
    unsigned int roomID;
    unsigned int roomSize;
    std::string hostName;
    unsigned int nbPlayer;
};
template <typename Archive>
Archive &operator|(Archive &ar, RoomData &data)
{
    return ar | data.roomID | data.roomSize | data.hostName | data.nbPlayer;
}