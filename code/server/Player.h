#pragma once

#include <cstdint>
#include <gf/TcpSocket.h>
#include <optional>

#include "../common/Protocol.h"
#include "../common/Types.h"


class Controller;

class Player {
public:
    Player(uint32_t id = 0, PlayerRole role = PlayerRole::Spectator, const std::string& name = "moi");

    //Getter
    uint32_t getId() const { return id; }
    PlayerRole getRole() const;
    PlayerData getState() const;
    int getScore() const { return score;}
    int getHP() const {return hp;}
    bool hasJustDied() const { return justDied;}
    bool isBot() const {return bot;}
    bool isReady() const { return ready; }
    bool isAlive() const {return hp > 0;}
    bool isVulnerable() const {return vulnerable;}
    gf::Vector2f getPos() const {return pos;}
    Direction getBufferedDir() const {return bufferedDir;}
    bool hasMoveRequest() const {return moveRequest;}
    double getMoveAccumulator() const {return moveAccumulator;}
    double getMoveRate() const {return moveRate;}
    std::function<void(Player&, const std::string&, int)> getOnPowerModeEvent() const {return onPowerModeEvent;};


    //Setter
    void setRole(PlayerRole r);
    void setState(const PlayerData& state);
    void setScore(int s) { score = s; }
    void setHP(unsigned int newHP) { hp = newHP;}
    void setBot(bool b) {bot = b;}
    void setReady(bool r) { ready = r; }
    void setPos(gf::Vector2f newPos) {pos = newPos;}
    void setBufferedDir(Direction bdir) {bufferedDir = bdir;}
    void setMoveRequest(bool r) {moveRequest = r;}
    void setMoveAccumulator(double newVal) {moveAccumulator = newVal;}
    void setMoveRate(double newVal) {moveRate = newVal;}
    void setJustDied(bool d) {justDied = d;}
    void setOnPowerModeEvent(std::function<void(Player&, const std::string&, int)> event) {onPowerModeEvent = event;}
    Player(const Player&) = delete;
    Player(Player&&) = default;

    Player& operator=(const Player&) = delete;
    Player& operator=(Player&&) = default;

    void update(double dt);
    bool eat(std::optional<PacGommeType> pacGommeType, Player* otherPlayer = nullptr);
    void clearDeathFlag() { justDied = false; }
    
private:
    uint32_t id;
    PlayerRole role;
    gf::Vector2f pos;
    uint32_t color = 0xFFFFFFFF;
    int score = 0;
    std::string name;
    Controller* controller = nullptr;
    bool bot = false;
    unsigned int hp;
    bool ready = false;
    Direction bufferedDir = Direction::None;
    bool moveRequest = false;
    double moveAccumulator = 0.;
    double moveRate = 0.;
    bool vulnerable = false;
    double powerTimeRemaining = 0.;
    bool justDied = false;
    std::function<void(Player&, const std::string&, int)> onPowerModeEvent;
};

