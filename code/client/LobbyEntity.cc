#include "LobbyEntity.h"

#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>
#include <gf/Anchor.h>
#include <gf/Text.h>
#include <gf/Shapes.h>
#include <gf/Sprite.h>
#include <gf/Texture.h>
#include <gf/Log.h>

LobbyEntity::LobbyEntity()
: m_font("../common/fonts/arial.ttf")
, m_leaveBtn("Quitter", m_font)
, m_minusBtn("-", m_font)
, m_plusBtn("+", m_font)
, m_minusBotBtn("-", m_font)
, m_plusBotBtn("+", m_font)
, m_minusDurBtn("-", m_font)
, m_plusDurBtn("+", m_font)
, m_minusPHPBtn("-", m_font)
, m_plusPHPBtn("+", m_font)
, m_minusGHPBtn("-", m_font)
, m_plusGHPBtn("+", m_font)
, m_readyBtn("PRÊT", m_font)
, m_changeRolePreBtn("<", m_font)
, m_changeRoleNextBtn(">", m_font)
, m_lastAction(LobbyAction::None)
{
    m_leaveBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::Leave; 
    });
    
    m_minusBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::RoomDec; 
    });
    
    m_plusBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::RoomInc; 
    });
    
    m_minusBotBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::BotDec; 
    });
    
    m_plusBotBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::BotInc; 
    });
    
    m_minusDurBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::DurDec; 
    });
    
    m_plusDurBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::DurInc; 
    });

    m_minusPHPBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::PacmanHPDec; 
    });
    
    m_plusPHPBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::PacmanHPInc; 
    });

    m_minusGHPBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::GhostHPDec; 
    });
    
    m_plusGHPBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::GhostHPInc; 
    });
    
    m_readyBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::ToggleReady; 
    });
    
    m_changeRolePreBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::ChangeRole; 
    });
    m_changeRoleNextBtn.setCallback([this]() { 
        m_lastAction = LobbyAction::ChangeRole; 
    });

    m_container.addWidget(m_leaveBtn);
    m_container.addWidget(m_minusBtn);
    m_container.addWidget(m_plusBtn);
    m_container.addWidget(m_minusBotBtn);
    m_container.addWidget(m_plusBotBtn);
    m_container.addWidget(m_minusDurBtn);
    m_container.addWidget(m_plusDurBtn);
    m_container.addWidget(m_minusPHPBtn);
    m_container.addWidget(m_plusPHPBtn);
    m_container.addWidget(m_minusGHPBtn);
    m_container.addWidget(m_plusGHPBtn);
    m_container.addWidget(m_readyBtn);
    m_container.addWidget(m_changeRolePreBtn);
    m_container.addWidget(m_changeRoleNextBtn);
}

void LobbyEntity::pointTo(gf::Vector2f coords) {
    m_container.pointTo(coords);
}

void LobbyEntity::triggerAction() {
    m_container.triggerAction();
}

void LobbyEntity::setPlayers(const std::vector<PlayerData>& players) {
    m_players = players;
    for(auto& p : players) {
        if(p.id == m_clientId) {
            m_client = p;
        }
    }
}

void LobbyEntity::setRoomSettings(const RoomSettings& settings) {
    m_roomSettings = settings;
}

void LobbyEntity::setClientId(uint32_t id) {
    m_clientId = id;
}

LobbyAction LobbyEntity::getAndResetLastAction(){
    LobbyAction a = m_lastAction;
    m_lastAction = LobbyAction::None;
    return a;
}

void LobbyEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    // ✅ Zone logique fixe
    const float margin = 16.f;

    // Calculs logiques pour UI
    float uiOffsetX = LOGICAL_W * 0.05f;
    float uiOffsetY = LOGICAL_H * 0.05f;

    float textPosX = margin + uiOffsetX;
    float textStartPosY = margin + uiOffsetY;

    gf::Vector2f settingsPos{textPosX, LOGICAL_H*0.125f};
    gf::Vector2f playerListPos{textPosX, LOGICAL_H * 0.6f};
    gf::Vector2f changeRolePos{LOGICAL_W *3/4.f - margin - uiOffsetX, LOGICAL_H*0.125f};

    //Nom de la room
    gf::Text roomName;
    roomName.setFont(m_font);
    roomName.setCharacterSize(28U);
    roomName.setColor(gf::Color::White);
    roomName.setString("Nom de la room ?");
    roomName.setPosition({textPosX, LOGICAL_W* .04f});
    target.draw(roomName, states);


    //draws
    renderSettings(target, states, settingsPos);
    renderPlayerList(target, states, playerListPos);
    renderRoleSelectionAndReady(target, states, changeRolePos);
}

void LobbyEntity::renderPlayerRow(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f position, const PlayerData& data) {
    const float ICON_SIZE = 24.f; //taille logique avant scale

    //Icon
    gf::Texture iconTexture;
    gf::RectF rectf;
    if (data.role == PlayerRole::PacMan) {
        iconTexture = gf::Texture("../client/assets/pacman/right_pacman_sheet.png");
        rectf = gf::RectF::fromPositionSize({0.f,0.f},{0.25f,1.f});
    } else {
        iconTexture = gf::Texture("../client/assets/ghosts/inky.png");
        rectf = gf::RectF::fromSize({1.f,1.f});
    }
    gf::RectangleShape roleSprite({ICON_SIZE,ICON_SIZE});
    roleSprite.setAnchor(gf::Anchor::TopLeft);
    roleSprite.setPosition({position.x,position.y-ICON_SIZE*0.8f});
    roleSprite.setTexture(iconTexture,rectf);
    target.draw(roleSprite, states);

    gf::Text playerText;
    playerText.setFont(m_font);
    playerText.setCharacterSize(20u);
    playerText.setAnchor(gf::Anchor::TopLeft);
    playerText.setColor(gf::Color::White);

    std::string playerStr = data.name;
    if (data.id == m_clientId) {
        playerStr += " (vous)";
    }
    playerStr += data.ready ? " (prêt)" : " (pas prêt)";

    playerText.setString(playerStr);
    playerText.setPosition({position.x + roleSprite.getSize().x + 5.f, position.y});
    target.draw(playerText, states);
}

void LobbyEntity::renderPlayerList(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f position) {
    //Rectangle
    gf::RoundedRectangleShape playBox({ LOGICAL_W/3.f, LOGICAL_H*0.33f}, 14.f);
    playBox.setPosition(position);
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(gf::Color::White);

    target.draw(playBox, states);


    gf::Text playerListLabel;
    playerListLabel.setFont(m_font);
    playerListLabel.setCharacterSize(24);
    playerListLabel.setColor(gf::Color::White);
    playerListLabel.setString(
        "Liste des joueurs (" +
        std::to_string(m_players.size()) + " / " +
        std::to_string(m_roomSettings.roomSize) + ") :"
    );
    playerListLabel.setPosition({position.x + playBox.getSize().x*.05f, position.y + playBox.getSize().y*.15f});
    target.draw(playerListLabel, states);

    float rowSpacing = 32.f; //hauteur fixe par joueur
    for (unsigned int i = 0; i < m_players.size(); i++) {
        renderPlayerRow(target, states, {playerListLabel.getPosition().x, playerListLabel.getPosition().y + rowSpacing * (i + 1)}, m_players[i]);
    }
}


void LobbyEntity::renderRoleSelectionAndReady(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f position) {
    const float ICON_SIZE = 16.f;
    const float SPRITE_SCALE = 4.f;
    const unsigned int ROLE_TEXT_SIZE = 20u;
    const unsigned int CHANGE_ROLE_TEXT_SIZE = 18u;

    //Rectangle
    gf::RoundedRectangleShape playBox({ LOGICAL_W/4.f, LOGICAL_H*0.825f }, 14.f);
    playBox.setPosition(position);
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(gf::Color::White);

    target.draw(playBox, states);

    //Nom
    gf::Text pseudoLabel;
    pseudoLabel.setFont(m_font);
    pseudoLabel.setCharacterSize(ROLE_TEXT_SIZE);
    pseudoLabel.setAnchor(gf::Anchor::TopCenter);
    pseudoLabel.setColor(gf::Color::White);
    pseudoLabel.setString(m_client.name);
    pseudoLabel.setAlignment(gf::Alignment::Center);
    pseudoLabel.setPosition({position.x + playBox.getSize().x/2.f,position.y+playBox.getSize().y*.075f});
    target.draw(pseudoLabel, states);

    //Icon
    gf::Texture iconTexture;
    gf::RectF rectf;
    if (m_client.role == PlayerRole::PacMan) {
        iconTexture = gf::Texture("../client/assets/pacman/right_pacman_sheet.png");
        rectf = gf::RectF::fromPositionSize({0.f,0.f},{0.25f,1.f});
    } else {
        iconTexture = gf::Texture("../client/assets/ghosts/inky.png");
        rectf = gf::RectF::fromSize({1.f,1.f});
    }
    gf::RectangleShape roleSprite({64.f,64.f});
    roleSprite.setPosition({position.x + playBox.getSize().x/2.f - (ICON_SIZE*SPRITE_SCALE/2),position.y+playBox.getSize().y*.125f});
    roleSprite.setTexture(iconTexture,rectf);
    target.draw(roleSprite, states);

    //Rôle texte
    gf::Text roleLabel;
    roleLabel.setFont(m_font);
    roleLabel.setAnchor(gf::Anchor::TopCenter);
    roleLabel.setAlignment(gf::Alignment::Center);
    roleLabel.setParagraphWidth(playBox.getSize().x*0.9f);
    roleLabel.setCharacterSize(ROLE_TEXT_SIZE);
    roleLabel.setColor(gf::Color::White);
    roleLabel.setString(m_client.role == PlayerRole::PacMan ? "Rôle : Pac-Man" : "Rôle : Fantôme");
    roleLabel.setPosition({position.x + playBox.getSize().x/2.f - roleLabel.getParagraphWidth()/2.f, position.y + playBox.getSize().y*.3f});
    target.draw(roleLabel, states);


    gf::Text roleDesc;
    roleDesc.setFont(m_font);
    roleDesc.setAnchor(gf::Anchor::TopCenter);
    roleDesc.setAlignment(gf::Alignment::Justify);
    roleDesc.setParagraphWidth(playBox.getSize().x*0.9f);
    roleDesc.setCharacterSize(ROLE_TEXT_SIZE);
    roleDesc.setColor(gf::Color::White);
    roleDesc.setPosition({position.x + playBox.getSize().x*.05f, position.y + playBox.getSize().y*.375f});
    roleDesc.setString("Une description de l\'objectif du rôle et de ses pouvoirs.\n voilà...");
    target.draw(roleDesc, states);




    //Bouton changement rôle
    m_changeRolePreBtn.setCharacterSize(CHANGE_ROLE_TEXT_SIZE);
    m_changeRolePreBtn.setAnchor(gf::Anchor::TopCenter);
    m_changeRolePreBtn.setAlignment(gf::Alignment::Center);
    defaultButtonColor(m_changeRolePreBtn);
    m_changeRolePreBtn.setPadding(CHANGE_ROLE_TEXT_SIZE * 0.65f);
    m_changeRolePreBtn.setPosition({position.x + playBox.getSize().x/4.f,roleSprite.getPosition().y+(roleSprite.getSize().y/2)});
    target.draw(m_changeRolePreBtn, states);

    m_changeRoleNextBtn.setCharacterSize(CHANGE_ROLE_TEXT_SIZE);
    m_changeRoleNextBtn.setAnchor(gf::Anchor::TopCenter);
    m_changeRoleNextBtn.setAlignment(gf::Alignment::Center);
    defaultButtonColor(m_changeRoleNextBtn);
    m_changeRoleNextBtn.setPadding(CHANGE_ROLE_TEXT_SIZE * 0.65f);
    m_changeRoleNextBtn.setPosition({position.x + playBox.getSize().x*3.f/4.f,roleSprite.getPosition().y+(roleSprite.getSize().y/2)});
    target.draw(m_changeRoleNextBtn, states);

    
    const unsigned int READY_TEXT_SIZE = 18u;
    //Bouton Prêt
    m_readyBtn.setCharacterSize(READY_TEXT_SIZE);
    m_readyBtn.setAnchor(gf::Anchor::TopCenter);
    m_readyBtn.setString(m_client.ready ? "PLUS PRÊT?" : "PRÊT");
    m_readyBtn.setPosition({position.x + playBox.getSize().x/2.f,position.y+playBox.getSize().y*.825f});
    defaultButtonColor(m_readyBtn);
    m_readyBtn.setDefaultBackgroundColor(m_client.ready ? gf::Color::Red : gf::Color::Green);
    m_readyBtn.setPadding(READY_TEXT_SIZE * .5f);
    target.draw(m_readyBtn, states);

    //Bouton quitter
    m_leaveBtn.setCharacterSize(READY_TEXT_SIZE);
    m_leaveBtn.setAnchor(gf::Anchor::TopCenter);
    m_leaveBtn.setPosition({position.x + playBox.getSize().x/2.f,position.y+playBox.getSize().y*.9f});
    defaultButtonColor(m_leaveBtn);
    m_leaveBtn.setDefaultBackgroundColor(gf::Color::Red);
    m_leaveBtn.setPadding(READY_TEXT_SIZE * .5f);
    target.draw(m_leaveBtn, states);
}

void LobbyEntity::renderSettingsRow(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position, std::string stgName, gf::TextButtonWidget& minusBtn, gf::TextButtonWidget& plusBtn, int currentValue)
{
    const unsigned int MINUS_SIZE = 16u;
    const unsigned int PLUS_SIZE = 14u;
    const float MINUS_BTN_POS_X = 260.f;
    const float PLUS_BTN_POS_X = 360.f;


    gf::Text stgNameLabel;
    stgNameLabel.setFont(m_font);
    stgNameLabel.setCharacterSize(SETTINGS_CHARACTER_SIZE);
    stgNameLabel.setColor(gf::Color::White);
    stgNameLabel.setString(stgName);
    stgNameLabel.setPosition(position);
    target.draw(stgNameLabel, states);

    minusBtn.setCharacterSize(MINUS_SIZE);
    minusBtn.setPosition({position.x + MINUS_BTN_POS_X,position.y});
    defaultButtonColor(minusBtn);
    minusBtn.setPadding(MINUS_SIZE * .65f);
    target.draw(minusBtn, states);

    plusBtn.setCharacterSize(PLUS_SIZE);
    plusBtn.setPosition({position.x + PLUS_BTN_POS_X,position.y});
    defaultButtonColor(plusBtn);
    plusBtn.setPadding(PLUS_SIZE * .65f);
    target.draw(plusBtn, states);

    gf::Text valueText;
    valueText.setFont(m_font);
    valueText.setCharacterSize(20u);
    valueText.setColor(gf::Color::White);
    valueText.setAlignment(gf::Alignment::Center);
    valueText.setString(std::to_string(currentValue));
    valueText.setPosition({position.x + (PLUS_BTN_POS_X + MINUS_BTN_POS_X)/2,position.y});
    target.draw(valueText, states);
}

void LobbyEntity::renderSettings(gf::RenderTarget& target, const gf::RenderStates& states, gf::Vector2f position) {
    const float margin = 16.f;

    //Rectangle
    gf::RoundedRectangleShape playBox({ LOGICAL_W/3.f, LOGICAL_H*0.4f }, 14.f);
    playBox.setPosition(position);
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(gf::Color::White);

    target.draw(playBox, states);

    //Labels
    gf::Text settingsLabel;
    settingsLabel.setFont(m_font);
    settingsLabel.setCharacterSize(24u);
    settingsLabel.setColor(gf::Color::White);
    settingsLabel.setString("Paramètre du jeu");
    settingsLabel.setPosition({position.x + playBox.getSize().x*.05f, position.y + playBox.getSize().y*.1f});
    target.draw(settingsLabel, states);

    float settingsPosX = position.x + playBox.getSize().x*.05f;

    gf::Vector2f maxPlayerTextPos{settingsPosX, settingsLabel.getPosition().y + SETTINGS_CHARACTER_SIZE * 2.5f};
    gf::Vector2f nbBotsTextPos{settingsPosX, maxPlayerTextPos.y + SETTINGS_CHARACTER_SIZE * 2.5f};
    gf::Vector2f durationTextPos{settingsPosX, nbBotsTextPos.y + SETTINGS_CHARACTER_SIZE * 2.5f};
    gf::Vector2f pacmanHpPos{settingsPosX, durationTextPos.y + SETTINGS_CHARACTER_SIZE * 2.5f};
    gf::Vector2f ghostHpPos{settingsPosX, pacmanHpPos.y + SETTINGS_CHARACTER_SIZE * 2.5f};


    //Joueurs max
    renderSettingsRow(target,states,maxPlayerTextPos,"Joueurs max :",m_minusBtn,m_plusBtn,static_cast<int>(m_roomSettings.roomSize));

    //Nb bots
    renderSettingsRow(target,states,nbBotsTextPos,"Nb de bots :",m_minusBotBtn,m_plusBotBtn,static_cast<int>(m_roomSettings.nbBot));

    //Durée
    renderSettingsRow(target,states,durationTextPos,"Temps de jeu (secondes) :",m_minusDurBtn,m_plusDurBtn,static_cast<int>(m_roomSettings.gameDuration));

    //Vie
    renderSettingsRow(target,states,pacmanHpPos,"Nb. PV Pacman :",m_minusPHPBtn,m_plusPHPBtn,static_cast<int>(m_roomSettings.nbLifePacman));
    renderSettingsRow(target,states,ghostHpPos,"Nb. PV Fantômes :",m_minusGHPBtn,m_plusGHPBtn,static_cast<int>(m_roomSettings.nbLifeGhost));
}


void LobbyEntity::defaultButtonColor(gf::TextButtonWidget& w)
{
    w.setDefaultTextColor(gf::Color::White);
    w.setSelectedTextColor(gf::Color::Black);
    w.setDefaultBackgroundColor(gf::Color::Black);
    w.setSelectedBackgroundColor(gf::Color::White);
    w.setBackgroundOutlineThickness(1.f);
    w.setDefaultBackgroundOutlineColor(gf::Color::White);
    w.setSelectedBackgroundOutlineColor(gf::Color::White);
}
