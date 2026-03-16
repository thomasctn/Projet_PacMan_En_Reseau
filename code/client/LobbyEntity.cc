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
    : m_font("../common/fonts/arial.ttf"), m_leaveBtn("Quitter", m_font), m_minusBtn("-", m_font), m_plusBtn("+", m_font), m_minusBotBtn("-", m_font), m_plusBotBtn("+", m_font), m_minusDurBtn("-", m_font), m_plusDurBtn("+", m_font), m_minusPHPBtn("-", m_font), m_plusPHPBtn("+", m_font), m_minusGHPBtn("-", m_font), m_plusGHPBtn("+", m_font), m_readyBtn("PRÊT", m_font), m_changeRolePreBtn("<", m_font), m_changeRoleNextBtn(">", m_font), m_lastAction(LobbyAction::None)
{
    m_leaveBtn.setCallback([this]()
                           { m_lastAction = LobbyAction::Leave; });

    m_minusBtn.setCallback([this]()
                           { m_lastAction = LobbyAction::RoomDec; });

    m_plusBtn.setCallback([this]()
                          { m_lastAction = LobbyAction::RoomInc; });

    m_minusBotBtn.setCallback([this]()
                              { m_lastAction = LobbyAction::BotDec; });

    m_plusBotBtn.setCallback([this]()
                             { m_lastAction = LobbyAction::BotInc; });

    m_minusDurBtn.setCallback([this]()
                              { m_lastAction = LobbyAction::DurDec; });

    m_plusDurBtn.setCallback([this]()
                             { m_lastAction = LobbyAction::DurInc; });

    m_minusPHPBtn.setCallback([this]()
                              { m_lastAction = LobbyAction::PacmanHPDec; });

    m_plusPHPBtn.setCallback([this]()
                             { m_lastAction = LobbyAction::PacmanHPInc; });

    m_minusGHPBtn.setCallback([this]()
                              { m_lastAction = LobbyAction::GhostHPDec; });

    m_plusGHPBtn.setCallback([this]()
                             { m_lastAction = LobbyAction::GhostHPInc; });

    m_readyBtn.setCallback([this]()
                           { m_lastAction = LobbyAction::ToggleReady; });

    m_changeRolePreBtn.setCallback([this]()
                                   { m_lastAction = LobbyAction::ChangeRole; });
    m_changeRoleNextBtn.setCallback([this]()
                                    { m_lastAction = LobbyAction::ChangeRole; });

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

    m_settingsLabel.setFont(m_font);
    m_settingsLabel.setCharacterSize(SETTINGS_CHAR_SIZE);
    m_settingsLabel.setColor(DEFAULT_TEXT_COLOR);
    m_settingsLabel.setString("Paramètre du jeu");

    m_roomName.setFont(m_font);
    m_roomName.setCharacterSize(ROOM_NAME_CHAR_SIZE);
    m_roomName.setColor(DEFAULT_TEXT_COLOR);

    m_playerListLabel.setFont(m_font);
    m_playerListLabel.setCharacterSize(PLAYER_LIST_CHAR_SIZE);
    m_playerListLabel.setColor(DEFAULT_TEXT_COLOR);

    m_pseudoLabel.setFont(m_font);
    m_pseudoLabel.setCharacterSize(PSEUDO_CHAR_SIZE);
    m_pseudoLabel.setAnchor(gf::Anchor::TopCenter);
    m_pseudoLabel.setColor(DEFAULT_TEXT_COLOR);
    m_pseudoLabel.setAlignment(gf::Alignment::Center);

    m_roleLabel.setFont(m_font);
    m_roleLabel.setAnchor(gf::Anchor::TopCenter);
    m_roleLabel.setAlignment(gf::Alignment::Center);
    m_roleLabel.setCharacterSize(ROLE_CHAR_SIZE);
    m_roleLabel.setColor(DEFAULT_TEXT_COLOR);

    m_roleDesc.setFont(m_font);
    m_roleDesc.setAnchor(gf::Anchor::TopCenter);
    m_roleDesc.setAlignment(gf::Alignment::Justify);
    m_roleDesc.setCharacterSize(ROLE_DESC_CHAR_SIZE);
    m_roleDesc.setColor(DEFAULT_TEXT_COLOR);

    m_changeRolePreBtn.setCharacterSize(CHANGE_ROLE_CHAR_SIZE);
    m_changeRolePreBtn.setAnchor(gf::Anchor::TopCenter);
    m_changeRolePreBtn.setAlignment(gf::Alignment::Center);
    defaultButtonColor(m_changeRolePreBtn);
    m_changeRolePreBtn.setPadding(CHANGE_ROLE_CHAR_SIZE * 0.65f);

    m_changeRoleNextBtn.setCharacterSize(CHANGE_ROLE_CHAR_SIZE);
    m_changeRoleNextBtn.setAnchor(gf::Anchor::TopCenter);
    m_changeRoleNextBtn.setAlignment(gf::Alignment::Center);
    defaultButtonColor(m_changeRoleNextBtn);
    m_changeRoleNextBtn.setPadding(CHANGE_ROLE_CHAR_SIZE * 0.65f);

    m_readyBtn.setCharacterSize(READY_CHAR_SIZE);
    defaultButtonColor(m_readyBtn);
    m_readyBtn.setDefaultBackgroundColor(gf::Color::Green);
    m_readyBtn.setDefaultBackgroundOutlineColor(gf::Color::Transparent);
    m_readyBtn.setSelectedBackgroundOutlineColor(gf::Color::Transparent);
    m_readyBtn.setPadding(READY_CHAR_SIZE * .5f);

    m_leaveBtn.setCharacterSize(READY_CHAR_SIZE);
    m_leaveBtn.setAnchor(gf::Anchor::TopCenter);
    defaultButtonColor(m_leaveBtn);
    m_leaveBtn.setDefaultBackgroundColor(gf::Color::Red);
    m_leaveBtn.setDefaultBackgroundOutlineColor(gf::Color::Transparent);
    m_leaveBtn.setSelectedBackgroundOutlineColor(gf::Color::Transparent);
    m_leaveBtn.setPadding(READY_CHAR_SIZE * .5f);
}

void LobbyEntity::pointTo(gf::Vector2f coords)
{
    m_container.pointTo(coords);
}

void LobbyEntity::triggerAction()
{
    m_container.triggerAction();
}

void LobbyEntity::setRoomID(uint32_t roomID)
{
    m_roomID = roomID;
}

void LobbyEntity::setPlayers(const std::vector<PlayerData> &players)
{
    m_players = players;
    for (auto &p : players)
    {
        if (p.id == m_clientId)
        {
            m_client = p;
            break;
        }
    }
}

void LobbyEntity::setRoomSettings(const RoomSettings &settings)
{
    m_roomSettings = settings;
}

void LobbyEntity::setClientId(uint32_t id)
{
    m_clientId = id;
}

LobbyAction LobbyEntity::getAndResetLastAction()
{
    LobbyAction a = m_lastAction;
    m_lastAction = LobbyAction::None;
    return a;
}

void LobbyEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
{
    const float margin = 16.f;

    float uiOffsetX = LOGICAL_W * 0.05f;
    float uiOffsetY = LOGICAL_H * 0.05f;

    float textPosX = margin + uiOffsetX;
    float textStartPosY = margin + uiOffsetY;

    gf::Vector2f settingsPos{textPosX, LOGICAL_H * 0.125f};
    gf::Vector2f playerListPos{textPosX, LOGICAL_H * 0.6f};
    gf::Vector2f changeRolePos{LOGICAL_W * 3 / 4.f - margin - uiOffsetX, LOGICAL_H * 0.125f};

    // Nom de la room
    m_roomName.setString("Room n° " + std::to_string(m_roomID));
    m_roomName.setPosition({textPosX, LOGICAL_W * .04f});
    target.draw(m_roomName, states);

    // draws
    renderSettings(target, states, settingsPos);
    renderPlayerList(target, states, playerListPos);
    renderRoleSelectionAndReady(target, states, changeRolePos);

    if(startIsRefused)
        renderStartRefused(target, states, changeRolePos);
}

void LobbyEntity::renderPlayerRow(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position, const PlayerData &data)
{
    const float ICON_SIZE = 24.f; // taille logique avant scale

    // Icon
    gf::Texture iconTexture;
    gf::RectF rectf;
    if (data.role == PlayerRole::PacMan)
    {
        iconTexture = gf::Texture("../client/assets/pacman/right_pacman_sheet.png");
        rectf = gf::RectF::fromPositionSize({0.f, 0.f}, {0.25f, 1.f});
    }
    else
    {
        iconTexture = gf::Texture("../client/assets/ghosts/inky.png");
        rectf = gf::RectF::fromSize({1.f, 1.f});
    }
    gf::RectangleShape roleSprite({ICON_SIZE, ICON_SIZE});
    roleSprite.setAnchor(gf::Anchor::TopLeft);
    roleSprite.setPosition({position.x, position.y - ICON_SIZE * 0.8f});
    roleSprite.setTexture(iconTexture, rectf);
    target.draw(roleSprite, states);

    gf::Text playerText;
    playerText.setFont(m_font);
    playerText.setCharacterSize(PLAYER_ROW_CHAR_SIZE);
    playerText.setAnchor(gf::Anchor::TopLeft);
    playerText.setColor(DEFAULT_TEXT_COLOR);
    std::string playerStr = data.name;
    if (data.id == m_clientId)
    {
        playerStr += " (vous)";
    }
    playerStr += data.ready ? " (prêt)" : " (pas prêt)";

    playerText.setString(playerStr);
    playerText.setPosition({position.x + roleSprite.getSize().x + 5.f, position.y});
    target.draw(playerText, states);
}

void LobbyEntity::renderPlayerList(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position)
{
    // Rectangle
    gf::RoundedRectangleShape playBox({LOGICAL_W / 3.f, LOGICAL_H * 0.33f}, 14.f);
    playBox.setPosition(position);
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(DEFAULT_OUTLINE_COLOR);

    target.draw(playBox, states);

    m_playerListLabel.setString(
        "Liste des joueurs (" +
        std::to_string(m_players.size()) + " / " +
        std::to_string(m_roomSettings.roomSize) + ") :");
    m_playerListLabel.setPosition({position.x + playBox.getSize().x * .05f, position.y + playBox.getSize().y * .15f});
    target.draw(m_playerListLabel, states);

    const float ROW_SPACING = 32.f; // hauteur fixe par joueur
    for (unsigned int i = 0; i < m_players.size(); i++)
    {
        renderPlayerRow(target, states, {m_playerListLabel.getPosition().x, m_playerListLabel.getPosition().y + ROW_SPACING * (i + 1)}, m_players[i]);
    }
}

void LobbyEntity::renderRoleSelectionAndReady(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position)
{
    const float ROLE_ICON_TEXTURE_SIZE = 16.f;
    const float ROLE_ICON_SCALE = 4.f;
    const float ROLE_ICON_SIZE = ROLE_ICON_TEXTURE_SIZE * ROLE_ICON_SCALE;

    // Rectangle
    gf::RoundedRectangleShape playBox({LOGICAL_W / 4.f, LOGICAL_H * 0.825f}, 14.f);
    playBox.setPosition(position);
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(DEFAULT_OUTLINE_COLOR);

    target.draw(playBox, states);

    // Nom
    m_pseudoLabel.setString(m_client.name);
    m_pseudoLabel.setPosition({position.x + playBox.getSize().x / 2.f, position.y + playBox.getSize().y * .075f});
    target.draw(m_pseudoLabel, states);

    // Icon
    gf::Texture iconTexture;
    gf::RectF rectf;
    if (m_client.role == PlayerRole::PacMan)
    {
        iconTexture = gf::Texture("../client/assets/pacman/right_pacman_sheet.png");
        rectf = gf::RectF::fromPositionSize({0.f, 0.f}, {0.25f, 1.f});
    }
    else
    {
        iconTexture = gf::Texture("../client/assets/ghosts/inky.png");
        rectf = gf::RectF::fromSize({1.f, 1.f});
    }
    gf::RectangleShape roleSprite({ROLE_ICON_SIZE, ROLE_ICON_SIZE});
    roleSprite.setPosition({position.x + playBox.getSize().x / 2.f - (ROLE_ICON_SIZE / 2), position.y + playBox.getSize().y * .125f});
    roleSprite.setTexture(iconTexture, rectf);
    target.draw(roleSprite, states);

    // Rôle texte
    m_roleLabel.setParagraphWidth(playBox.getSize().x * 0.9f);
    m_roleLabel.setString(m_client.role == PlayerRole::PacMan ? "Rôle : Pac-Man" : "Rôle : Fantôme");
    m_roleLabel.setPosition({position.x + playBox.getSize().x / 2.f - m_roleLabel.getParagraphWidth() / 2.f, position.y + playBox.getSize().y * .3f});
    target.draw(m_roleLabel, states);

    // Desciption du rôle
    m_roleDesc.setParagraphWidth(playBox.getSize().x * 0.9f);
    m_roleDesc.setPosition({position.x + playBox.getSize().x * .05f, position.y + playBox.getSize().y * .375f});
    std::string txt = "Pacman doit manger toutes les pacgommes ou tous les fantômes";
    if(m_client.role == PlayerRole::Ghost) {
        txt = "Les fantômes doivent manger Pacman.";
    }
    m_roleDesc.setString(txt);
    target.draw(m_roleDesc, states);

    // Bouton changement rôle
    m_changeRolePreBtn.setPosition({position.x + playBox.getSize().x / 4.f, roleSprite.getPosition().y + (roleSprite.getSize().y / 2)});
    target.draw(m_changeRolePreBtn, states);
    m_changeRoleNextBtn.setPosition({position.x + playBox.getSize().x * 3.f / 4.f, roleSprite.getPosition().y + (roleSprite.getSize().y / 2)});
    target.draw(m_changeRoleNextBtn, states);

    // Bouton Prêt
    m_readyBtn.setDefaultBackgroundColor(m_client.ready ? gf::Color::Red : gf::Color::Green);
    m_readyBtn.setString(m_client.ready ? "PLUS PRÊT?" : "PRÊT");
    m_readyBtn.setAnchor(gf::Anchor::TopCenter);
    m_readyBtn.setPosition({position.x + playBox.getSize().x / 2.f, position.y + playBox.getSize().y * .825f});
    target.draw(m_readyBtn, states);

    // Bouton quitter
    m_leaveBtn.setPosition({position.x + playBox.getSize().x / 2.f, position.y + playBox.getSize().y * .9f});
    target.draw(m_leaveBtn, states);
}

void LobbyEntity::renderStartRefused(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position){
    const float LOGICAL_W = 1280.f;
    const float LOGICAL_H = 720.f;

    gf::Vector2f playBoxSize = { LOGICAL_W / 4.f, LOGICAL_H * 0.825f };

    float textY = position.y + playBoxSize.y * 0.62f;

    gf::Text refusedText;
    refusedText.setFont(m_font);
    refusedText.setCharacterSize(17);
    refusedText.setColor(gf::Color::White);
    refusedText.setAnchor(gf::Anchor::TopLeft);

    refusedText.setString("Une partie ne peut pas démarrer\nsans qu'un joueur ait pour rôle\nPacman.");

    refusedText.setPosition({position.x + playBoxSize.x * 0.08f,textY + 0.08f});

    target.draw(refusedText, states);

    //rectangle rouge autour du texte
    gf::Vector2f rectSize = {playBoxSize.x * 0.9f, LOGICAL_H * 0.1f};

    gf::RoundedRectangleShape rect(rectSize, 8.f);
    rect.setPosition({position.x + playBoxSize.x * 0.05f,textY - 20.f});

    rect.setColor(gf::Color::Transparent);
    rect.setOutlineThickness(2.f);
    rect.setOutlineColor(gf::Color::Red);

    target.draw(rect, states);
}

void LobbyEntity::renderSettingsRow(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position, std::string stgName, gf::TextButtonWidget &minusBtn, gf::TextButtonWidget &plusBtn, int currentValue)
{
    const unsigned int MINUS_SIZE = 16u;
    const unsigned int PLUS_SIZE = 14u;
    const float MINUS_BTN_POS_X = 260.f;
    const float PLUS_BTN_POS_X = 360.f;

    gf::Text stgNameLabel;
    stgNameLabel.setFont(m_font);
    stgNameLabel.setCharacterSize(SETTINGS_NAME_CHAR_SIZE);
    stgNameLabel.setColor(DEFAULT_TEXT_COLOR);
    stgNameLabel.setString(stgName);
    stgNameLabel.setPosition(position);
    target.draw(stgNameLabel, states);

    minusBtn.setCharacterSize(MINUS_SIZE);
    minusBtn.setPosition({position.x + MINUS_BTN_POS_X, position.y});
    defaultButtonColor(minusBtn);
    minusBtn.setPadding(MINUS_SIZE * .65f);
    target.draw(minusBtn, states);

    plusBtn.setCharacterSize(PLUS_SIZE);
    plusBtn.setPosition({position.x + PLUS_BTN_POS_X, position.y});
    defaultButtonColor(plusBtn);
    plusBtn.setPadding(PLUS_SIZE * .65f);
    target.draw(plusBtn, states);

    gf::Text valueText;
    valueText.setFont(m_font);
    valueText.setCharacterSize(20u);
    valueText.setColor(DEFAULT_TEXT_COLOR);
    valueText.setAlignment(gf::Alignment::Center);
    valueText.setString(std::to_string(currentValue));
    valueText.setPosition({position.x + (PLUS_BTN_POS_X + MINUS_BTN_POS_X) / 2, position.y});
    target.draw(valueText, states);
}

void LobbyEntity::renderSettings(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position)
{
    // Rectangle
    gf::RoundedRectangleShape playBox({LOGICAL_W / 3.f, LOGICAL_H * 0.4f}, 14.f);
    playBox.setPosition(position);
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(DEFAULT_OUTLINE_COLOR);

    target.draw(playBox, states);

    // Labels
    m_settingsLabel.setPosition({position.x + playBox.getSize().x * .05f, position.y + playBox.getSize().y * .1f});
    target.draw(m_settingsLabel, states);

    float settingsPosX = position.x + playBox.getSize().x * .05f;

    gf::Vector2f maxPlayerTextPos{settingsPosX, m_settingsLabel.getPosition().y + SETTINGS_NAME_CHAR_SIZE * 2.5f};
    gf::Vector2f nbBotsTextPos{settingsPosX, maxPlayerTextPos.y + SETTINGS_NAME_CHAR_SIZE * 2.5f};
    gf::Vector2f durationTextPos{settingsPosX, nbBotsTextPos.y + SETTINGS_NAME_CHAR_SIZE * 2.5f};
    gf::Vector2f pacmanHpPos{settingsPosX, durationTextPos.y + SETTINGS_NAME_CHAR_SIZE * 2.5f};
    gf::Vector2f ghostHpPos{settingsPosX, pacmanHpPos.y + SETTINGS_NAME_CHAR_SIZE * 2.5f};

    // Joueurs max
    renderSettingsRow(target, states, maxPlayerTextPos, "Joueurs max :", m_minusBtn, m_plusBtn, static_cast<int>(m_roomSettings.roomSize));

    // Nb bots
    renderSettingsRow(target, states, nbBotsTextPos, "Nb de bots :", m_minusBotBtn, m_plusBotBtn, static_cast<int>(m_roomSettings.nbBot));

    // Durée
    renderSettingsRow(target, states, durationTextPos, "Temps de jeu (secondes) :", m_minusDurBtn, m_plusDurBtn, static_cast<int>(m_roomSettings.gameDuration));

    // Vie
    renderSettingsRow(target, states, pacmanHpPos, "Nb. PV Pacman :", m_minusPHPBtn, m_plusPHPBtn, static_cast<int>(m_roomSettings.nbLifePacman));
    renderSettingsRow(target, states, ghostHpPos, "Nb. PV Fantômes :", m_minusGHPBtn, m_plusGHPBtn, static_cast<int>(m_roomSettings.nbLifeGhost));
}

void LobbyEntity::defaultButtonColor(gf::TextButtonWidget &w)
{
    w.setDefaultTextColor(DEFAULT_TEXT_COLOR);
    w.setSelectedTextColor(gf::Color::Black);
    w.setDefaultBackgroundColor(gf::Color::Black);
    w.setSelectedBackgroundColor(gf::Color::White);
    w.setBackgroundOutlineThickness(1.f);
    w.setDefaultBackgroundOutlineColor(DEFAULT_OUTLINE_COLOR);
    w.setSelectedBackgroundOutlineColor(DEFAULT_OUTLINE_COLOR);
}
