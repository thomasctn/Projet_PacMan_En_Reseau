#include "Player.h"
#include <gf/Log.h>
#include "../common/Constants.h"

// ------------------
// Constructeur
// ------------------
Player::Player(uint32_t id_, PlayerRole role_, const std::string& name_)
: id(id_)
, role(role_)
, pos(0.f)
, color(0xFFFFFFFF)
, score(0)
, ready(false)
, name(name_)
, moveAccumulator(0.0)
, bufferedDir(Direction::None)
, powerTimeRemaining(0.0)
{
    switch (role) {
        case PlayerRole::PacMan:
            moveRate = PACMAN_SPEED_MOVERATE;
            vulnerable = true;
            hp = 3;
            break;
        case PlayerRole::Ghost:
            moveRate = GHOST_SPEED_MOVERATE;
            vulnerable = false;
            hp = 2;
            break;
        default:
            moveRate = DEFAULT_SPEED_MOVERATE;
            vulnerable = false;
            hp = 0;
            break;
    }
}


// ------------------
// Accesseurs rôle
// ------------------
PlayerRole Player::getRole() const {
    return role;
}

void Player::setRole(PlayerRole r) {
    role = r;
}

// ------------------
// État réseau
// ------------------
PlayerData Player::getState() const {

    if (!isAlive()) {
        return { id, 0.0f, 0.0f, color, name, role, score, true, 0 };
    }

    return { id, pos.x,pos.y, color, name, role, score, true, hp };
}


void Player::setState(const PlayerData& state) {
    pos = {state.x,state.y};
    color = state.color;
}

// ------------------
// Gameplay : manger
// ------------------
bool Player::eat(std::optional<PacGommeType> pacGommeType, Player* otherPlayer)
{
    switch (role)
    {
        case PlayerRole::PacMan:
        {
            // --- Mange une pac-gomme ---
            if (pacGommeType.has_value())
            {
                if (pacGommeType.value() == PacGommeType::Basic)
                {
                    score += 10;
                    gf::Log::info("PacMan %d a mangé une pac-gomme normale. Score=%d\n", id, score);
                }
                else if (pacGommeType.value() == PacGommeType::Power)
                    {
                        score += 50;
                        vulnerable = false; // devient chasseur
                        powerTimeRemaining = CHASSEUR_MODE_TIME;

                        gf::Log::info("PacMan %d a mangé une PAC-GOMME POWER !\n", id);

                        // --- Callback ---
                        if (onPowerModeEvent) {
                            onPowerModeEvent(*this, "start", static_cast<int>(powerTimeRemaining));
                        }
                    }

                return true;
            }

            // --- Mange un fantôme ---
            if (otherPlayer && otherPlayer->getRole() == PlayerRole::Ghost)
            {
                if (!vulnerable) // mode POWER
                {
                    score += 100;

                    otherPlayer->setHP(otherPlayer->getHP()-1);
                    // Respawn fantôme
                    otherPlayer->setPos({50.f,50.f});

                    gf::Log::info("PacMan %d a mangé Fantôme %d !\n", id, otherPlayer->id);
                    return true;
                }
            }

            break;
        }

        case PlayerRole::Ghost:
        {
            if (otherPlayer && otherPlayer->getRole() == PlayerRole::PacMan)
            {
                if (otherPlayer->isVulnerable()) // PacMan normal
                {
                    otherPlayer->setHP(otherPlayer->getHP()-1);
                    otherPlayer->setJustDied(true);
                    return true;
                }
            }

            break;
        }

        default:
            break;
    }

    return false;
}


void Player::update(double dt) {
    if (!isVulnerable()) {
        double oldTime = powerTimeRemaining;
        powerTimeRemaining -= dt;

        // --- Tick chaque seconde ---
        int oldSec = static_cast<int>(oldTime);
        int newSec = static_cast<int>(powerTimeRemaining);
        if (oldSec != newSec && onPowerModeEvent) {
            onPowerModeEvent(*this, "tick", newSec);
        }

        // --- Fin du mode chasseur ---
        if (powerTimeRemaining <= 0.0) {
            vulnerable = true;
            powerTimeRemaining = 0.0;
            gf::Log::info("PacMan %d n'est plus en mode chasseur !\n", id);

            if (onPowerModeEvent) {
                onPowerModeEvent(*this, "end", 0);
            }
        }
    }
}


