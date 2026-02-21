#pragma once

#include <gf/Entity.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Text.h>
#include <gf/Font.h>
#include <vector>
#include <memory>

#include "../common/Protocol.h"

enum class LobbyAction
{
  None,

  Leave,

  RoomDec,
  RoomInc,

  BotDec,
  BotInc,

  DurDec,
  DurInc,

  PacmanHPDec,
  PacmanHPInc,

  GhostHPDec,
  GhostHPInc,

  ToggleReady,
  ChangeRole
};

class LobbyEntity : public gf::Entity
{
public:
  LobbyEntity();

  void pointTo(gf::Vector2f coords);
  void triggerAction();
  void setRoomID(uint32_t roomID);
  void setPlayers(const std::vector<PlayerData> &players);
  void setRoomSettings(const RoomSettings &settings);
  void setClientId(uint32_t id);

  LobbyAction getAndResetLastAction();

  void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

private:
  void renderPlayerRow(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position, const PlayerData &data);
  void renderPlayerList(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position);
  void renderRoleSelectionAndReady(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position);
  void renderSettingsRow(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position, std::string stgName, gf::TextButtonWidget &minusBtn, gf::TextButtonWidget &plusBtn, int currentValue);
  void renderSettings(gf::RenderTarget &target, const gf::RenderStates &states, gf::Vector2f position);
  void defaultButtonColor(gf::TextButtonWidget &w);

public:
  const float LOGICAL_W = 1280.f;
  const float LOGICAL_H = 720.f;
  const unsigned int ROOM_NAME_CHAR_SIZE = 28u;
  const unsigned int SETTINGS_CHAR_SIZE = 24u;
  const unsigned int SETTINGS_NAME_CHAR_SIZE = 18u;
  const unsigned int PLAYER_LIST_CHAR_SIZE = 24u;
  const unsigned int PLAYER_ROW_CHAR_SIZE = 20u;
  const unsigned int PSEUDO_CHAR_SIZE = 18u;
  const unsigned int ROLE_CHAR_SIZE = 20u;
  const unsigned int ROLE_DESC_CHAR_SIZE = 20u;
  const unsigned int CHANGE_ROLE_CHAR_SIZE = 18u;
  const unsigned int READY_CHAR_SIZE = 18u;

  const gf::Color4f DEFAULT_TEXT_COLOR = gf::Color::White;
  const gf::Color4f DEFAULT_OUTLINE_COLOR = gf::Color::Blue;

private:
  gf::Font m_font;
  gf::Text m_settingsLabel;
  gf::Text m_roomName;
  gf::Text m_playerListLabel;
  gf::Text m_pseudoLabel;
  gf::Text m_roleLabel;
  gf::Text m_roleDesc;

  gf::WidgetContainer m_container;
  gf::TextButtonWidget m_leaveBtn;
  gf::TextButtonWidget m_minusBtn;
  gf::TextButtonWidget m_plusBtn;
  gf::TextButtonWidget m_minusBotBtn;
  gf::TextButtonWidget m_plusBotBtn;
  gf::TextButtonWidget m_minusDurBtn;

  // hp
  gf::TextButtonWidget m_plusPHPBtn;
  gf::TextButtonWidget m_minusPHPBtn;
  gf::TextButtonWidget m_plusGHPBtn;
  gf::TextButtonWidget m_minusGHPBtn;

  gf::TextButtonWidget m_plusDurBtn;
  gf::TextButtonWidget m_readyBtn;
  gf::TextButtonWidget m_changeRolePreBtn;
  gf::TextButtonWidget m_changeRoleNextBtn;

  LobbyAction m_lastAction = LobbyAction::None;

  // état affiché
  uint32_t m_roomID;
  std::vector<PlayerData> m_players;
  PlayerData m_client;
  RoomSettings m_roomSettings{};
  uint32_t m_clientId = 0;
};
