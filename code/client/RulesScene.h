#pragma once

#include <gf/Scene.h>
#include <gf/Font.h>
#include <gf/Log.h>

#include "RulesEntity.h"
#include <gf/Scene.h>

class ClientGame;

class RulesScene : public gf::Scene {
public:
    explicit RulesScene(ClientGame& game);
    void resizeYourself() ;
    virtual void doShow() override;


private:
    void doProcessEvent(gf::Event& event) override;
    void doUpdate(gf::Time time) override;

private:
    ClientGame& m_game;
    gf::Font m_font;
    RulesEntity m_entity;
};
