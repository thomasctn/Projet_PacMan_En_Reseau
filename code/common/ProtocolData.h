#pragma once
#include "Types.h"
#include "Constants.h"

struct Position
{
    unsigned int x;
    unsigned int y;

    Position() : x(0), y(0) {}
    Position(unsigned int xp, unsigned int yp) : x(xp), y(yp) {}

    bool operator==(const Position &a) const
    {
        return (x == a.x && y == a.y);
    }

    bool operator<(const Position &a) const
    {
        return (x < a.x || (x == a.x && y < a.y));
    }
};

namespace std
{
    template <>
    struct hash<Position>
    {
        std::size_t operator()(const Position &p) const noexcept
        {
            return std::hash<unsigned int>()(p.x) ^ (std::hash<unsigned int>()(p.y) << 1);
        }
    };
}

template <typename Archive>
Archive &operator|(Archive &ar, Position &data)
{
    return ar | data.x | data.y;
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