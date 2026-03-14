#include "GameEntity.h"
#include "../common/Constants.h"
#include <gf/Text.h>
#include <gf/Font.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>
#include <gf/Shapes.h>
#include <algorithm>
#include <cmath>

GameEntity::GameEntity()
    : m_font("../common/fonts/Emulogic-zrEw.ttf")
{
    ghostTextures.push_back(gf::Texture("../client/assets/ghosts/inky.png"));
    ghostTextures.push_back(gf::Texture("../client/assets/ghosts/clyde.png"));
    ghostTextures.push_back(gf::Texture("../client/assets/ghosts/pinky.png"));
    ghostTextures.push_back(gf::Texture("../client/assets/ghosts/blinky.png"));

    ghostSprite.setTexture(ghostTextures[0]);
    ghostSprite.setOrigin({0.f, 0.f});

    m_ghostVulnerableTexture = gf::Texture("../client/assets/ghosts/blue_ghost.png");

    m_pacmanRightTexture = gf::Texture("../client/assets/pacman/right_pacman_sheet.png");
    m_pacmanLeftTexture = gf::Texture("../client/assets/pacman/left_pacman_sheet.png");
    m_pacmanUpTexture = gf::Texture("../client/assets/pacman/up_pacman_sheet.png");
    m_pacmanDownTexture = gf::Texture("../client/assets/pacman/down_pacman_sheet.png");

    m_pacmanRightAnim.addTileset(m_pacmanRightTexture, {4, 1}, gf::seconds(0.1f), 4);
    m_pacmanLeftAnim.addTileset(m_pacmanLeftTexture, {4, 1}, gf::seconds(0.1f), 4);
    m_pacmanUpAnim.addTileset(m_pacmanUpTexture, {4, 1}, gf::seconds(0.1f), 4);
    m_pacmanDownAnim.addTileset(m_pacmanDownTexture, {4, 1}, gf::seconds(0.1f), 4);

    m_pacmanRightAnim.setLoop(true);
    m_pacmanLeftAnim.setLoop(true);
    m_pacmanUpAnim.setLoop(true);
    m_pacmanDownAnim.setLoop(true);

    m_pacmanSprite.setAnimation(m_pacmanRightAnim);
    m_pacmanSprite.setOrigin({0.f, 0.f});
    m_pacmanDir = 'R';

    wall_texture_rectF.insert({0, gf::RectF::fromPositionSize({0.8f, 0.f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({1, gf::RectF::fromPositionSize({0.6f, 0.f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({2, gf::RectF::fromPositionSize({0.2f, 0.f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({3, gf::RectF::fromPositionSize({0.4f, 0.f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({4, gf::RectF::fromPositionSize({0.f, 0.50f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({5, gf::RectF::fromPositionSize({0.4f, 0.50f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({6, gf::RectF::fromPositionSize({0.2f, 0.5f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({7, gf::RectF::fromPositionSize({0.8f, 0.25f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({8, gf::RectF::fromPositionSize({0.f, 0.f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({9, gf::RectF::fromPositionSize({0.4f, 0.25f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({10, gf::RectF::fromPositionSize({0.2f, 0.25f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({11, gf::RectF::fromPositionSize({0.6f, 0.25f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({12, gf::RectF::fromPositionSize({0.f, 0.25f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({13, gf::RectF::fromPositionSize({0.8f, 0.50f}, {0.2f, 0.25f})});
    wall_texture_rectF.insert({14, gf::RectF::fromPositionSize({0.6f, 0.50f}, {0.2f, 0.25f})});
}

void GameEntity::setGameState(const std::vector<PlayerData> &states)
{
    m_myRole = PlayerRole::Spectator; // par defaut
    auto now = std::chrono::steady_clock::now();

    for (auto &s : states)
    {
        auto it = pos.find(s.id);

        if (it == pos.end())
        {
            MovementState m;
            m.startPos = {s.x, s.y};
            m.destPos = {s.x, s.y};
            m.startTime = now;
            m.lastServerUpdate = now;
            m.expectedDuration = 0.2f;
            pos[s.id] = m;
            continue;
        }

        MovementState &m = it->second;

        m.destPos = {s.x, s.y};

        float timeSinceLast = std::chrono::duration<float>(now - m.lastServerUpdate).count();
        m.expectedDuration = 0.8f * m.expectedDuration + 0.2f * timeSinceLast;

        m.lastServerUpdate = now;

        if (s.role == PlayerRole::PacMan)
        {
            m_pacmanHp = s.hp;
        }
        if (s.id == m_clientId)
        {
            m_clientData = s;
            m_myHp = s.hp;
            m_myRole = s.role;
        }
    }

    m_states = states;
}

void GameEntity::setBoard(const BoardCommon &board)
{
    m_board = board;
}

void GameEntity::setPacGommes(const std::vector<std::pair<gf::Vector2i, PacGommeType>> &pacgommes)
{
    m_pacgommes = pacgommes;
}

void GameEntity::setTimeLeft(int timeLeftPre, unsigned int timeLeft)
{
    m_timeLeftPre = timeLeftPre;
    m_timeLeft = timeLeft;
}

void GameEntity::setGameTimeLeft(unsigned int timeLeft)
{
    m_timeLeft = timeLeft;
}

void GameEntity::setHoleLinks(const std::unordered_map<gf::Vector2i, gf::Vector2i> &holeLinks)
{
    m_holeLinks = holeLinks;
}

void GameEntity::setClientId(uint32_t id)
{
    m_clientId = id;
}

void GameEntity::update(gf::Time time)
{ // update anim pacman
    m_pacmanSprite.update(time);
}

void GameEntity::startPacmanPower()
{
    m_pacmanPowerActive = true;
}

void GameEntity::updatePacmanPower(int timeLeft)
{
    m_pacmanPowerActive = true;
    m_pacmanPowerTimeLeft = timeLeft;
}

void GameEntity::endPacmanPower()
{
    m_pacmanPowerActive = false;
    m_pacmanPowerTimeLeft = 0;
}

void GameEntity::calculateMovement(gf::RenderTarget &target, const BoardCommon &map, float &tileSize, float &offsetX, float &offsetY)
{
    gf::View view = target.getView();
    gf::Vector2f viewSize = view.getSize();

    float topMargin = std::min(viewSize.y * 0.16f, 120.0f); // espace ou on met le texte en haut
    float availW = viewSize.x;
    float availH = viewSize.y - topMargin;

    tileSize = std::min(availW / float(map.width), availH / float(map.height));
    offsetX = (viewSize.x - tileSize * float(map.width)) / 2.f + (view.getCenter().x - viewSize.x * 0.5f);
    offsetY = topMargin + (view.getCenter().y - viewSize.y * 0.5f);
}

void GameEntity::renderPacManPower(gf::RenderTarget &target, const gf::RenderStates &states, float mapOriginX, float logicalTileSize, float topMargin)
{
    if (!m_pacmanPowerActive)
        return;

    float mapRight = mapOriginX + m_board.width * logicalTileSize;

    float boxPadding = 12.f;

    float startX = mapRight + 20.f;
    float startY = topMargin - 10.f;

    unsigned charSize = 12u;

    gf::Text text;
    text.setFont(m_font);
    text.setCharacterSize(charSize);
    text.setColor(gf::Color::Yellow);
    text.setString("Pacman mode chasseur\nactive : " + std::to_string(m_pacmanPowerTimeLeft) + "s restantes");

    gf::RectF bounds = text.getLocalBounds();
    float textWidth = bounds.max.x - bounds.min.x;
    float textHeight = bounds.max.y - bounds.min.y;

    gf::RoundedRectangleShape box({textWidth + boxPadding * 2.f, textHeight + boxPadding * 2.f}, 8.f);

    box.setPosition({startX, startY});
    box.setColor(gf::Color::Black);
    box.setOutlineThickness(2.f);
    box.setOutlineColor(gf::Color::Yellow);

    target.draw(box, states);

    text.setPosition({startX + boxPadding, startY + boxPadding + 10.f});

    target.draw(text, states);
}

void GameEntity::renderMap(gf::RenderTarget &target, const gf::RenderStates &states, const BoardCommon &map, float logicalTileSize, float mapOriginX, float mapOriginY)
{
    int posX = abs(floor(m_clientData.x)) / 50;
    int posY = abs(floor(m_clientData.y)) / 50;
    for (unsigned int y = 0; y < map.height; ++y)
    {
        for (unsigned int x = 0; x < map.width; ++x)
        {
            const CaseCommon &cell = map.grid({x, y});
            gf::RectangleShape tile({logicalTileSize, logicalTileSize});
            tile.setPosition({mapOriginX + x * logicalTileSize, mapOriginY + y * logicalTileSize});
            if (!posIsInRange(posX, posY, x, y, VISION_RANGE_PLAYER) && m_clientData.role == PlayerRole::Ghost)
            {
                tile.setColor(gf::Color::Black);
            }
            else if (cell.celltype == CellType::Wall)
            {
                gf::Texture tex = gf::Texture("../client/assets/board/tiles_set_simplified.png");
                unsigned int res = 0;
                if ((x != 0) && map.grid({x - 1, y}).celltype == CellType::Wall)
                    res += 1;
                if ((x != map.width - 1) && map.grid({x + 1, y}).celltype == CellType::Wall)
                    res += 2;
                if ((y != 0) && map.grid({x, y - 1}).celltype == CellType::Wall)
                    res += 4;
                if ((y != map.height - 1) && map.grid({x, y + 1}).celltype == CellType::Wall)
                    res += 8;
                if (res < 15)
                {
                    tile.setTexture(tex, wall_texture_rectF.at(res));
                    target.draw(tile, states);
                }
                else
                {
                    tile.setColor(gf::Color::Black);
                    target.draw(tile, states);
                }
            }
            else if (cell.celltype == CellType::Floor)
            {
                tile.setColor(gf::Color::Black);
                target.draw(tile, states);
            }
            else
            {
                tile.setColor(gf::Color::Red);
                target.draw(tile, states);
            }
        }
    }
    const std::vector<gf::Color4f> portalColors = {gf::Color::Magenta, gf::Color::Blue, gf::Color::Red, gf::Color::Yellow};
    unsigned int colorID = 0;
    for (auto &p : m_holeLinks)
    {
        gf::RectangleShape portalTile({logicalTileSize, logicalTileSize});
        portalTile.setColor(portalColors[colorID]);
        if (posIsInRange(posX, posY, p.first.x, p.first.y, VISION_RANGE_PLAYER) || m_clientData.role != PlayerRole::Ghost)
        {
            portalTile.setPosition({mapOriginX + p.first.x * logicalTileSize, mapOriginY + p.first.y * logicalTileSize});
            target.draw(portalTile, states);
        }
        if (posIsInRange(posX, posY, p.second.x, p.second.y, VISION_RANGE_PLAYER) || m_clientData.role != PlayerRole::Ghost)
        {
            portalTile.setPosition({mapOriginX + p.second.x * logicalTileSize, mapOriginY + p.second.y * logicalTileSize});
            target.draw(portalTile, states);
        }

        colorID = (colorID + 1) % portalColors.size();
    }
}

void GameEntity::renderPacGommes(gf::RenderTarget &target, const gf::RenderStates &states, const std::vector<std::pair<gf::Vector2i, PacGommeType>> &pacgommes, float logicalTileSize, float mapOriginX, float mapOriginY)
{
    float radius = logicalTileSize / 6.0f;

    int posX = abs(floor(m_clientData.x)) / 50;
    int posY = abs(floor(m_clientData.y)) / 50;
    for (auto &[pos, type] : pacgommes)
    {
        if (!posIsInRange(posX, posY, pos.x, pos.y, VISION_RANGE_PLAYER) && m_clientData.role == PlayerRole::Ghost)
        {
            continue;
        }
        gf::CircleShape pacGomme(radius);
        pacGomme.setOrigin({radius, radius});
        pacGomme.setPosition({mapOriginX + pos.x * logicalTileSize + logicalTileSize / 2.f,
                              mapOriginY + pos.y * logicalTileSize + logicalTileSize / 2.f});

        switch (type)
        {
        case PacGommeType::Basic:
            pacGomme.setColor(gf::Color::Yellow);
            break;
        case PacGommeType::Power:
            pacGomme.setColor(gf::Color::Green);
            break;
        default:
            pacGomme.setColor(gf::Color::White);
            break;
        }
        target.draw(pacGomme, states);
    }
}

void GameEntity::renderSprites(gf::RenderTarget &target, const gf::RenderStates &states,float logicalTileSize, float mapOriginX, float mapOriginY)
{
    float tilePx = logicalTileSize; // logique == pixels fixes
    auto pacTexSize = m_pacmanRightTexture.getSize();
    if (pacTexSize.x > 0)
    {
        float fw = float(pacTexSize.x) / 4.f;
        float fh = float(pacTexSize.y);
        m_pacmanSprite.setScale({tilePx / fw, tilePx / fh});
    }

    int ghostIndex = 0;
    int posX = abs(floor(m_clientData.x)) / 50;
    int posY = abs(floor(m_clientData.y)) / 50;

    for (const auto &s : m_states)
    {
        if (s.x == 0.f && s.y == 0.f)
            continue;

        auto &m = pos.at(s.id);

        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration<float>(now - m.startTime).count();

        float speedFactor = SPEEDFACTOR; // 0.5 = moitié de vitesse
        float t = std::clamp(elapsed / m.expectedDuration * speedFactor, 0.f, 1.f);
        gf::Vector2f currentPos = m.startPos + (m.destPos - m.startPos) * t;

        float px = mapOriginX + (currentPos.x / 50.f) * logicalTileSize;
        float py = mapOriginY + (currentPos.y / 50.f) * logicalTileSize;

        int s_posX = static_cast<int>(currentPos.x / 50.f);
        int s_posY = static_cast<int>(currentPos.y / 50.f);
        if (!posIsInRange(posX, posY, s_posX, s_posY, VISION_RANGE_PLAYER) && m_clientData.role == PlayerRole::Ghost)
        {
            if (s.role == PlayerRole::Ghost)
            {
                ghostIndex = (ghostIndex + 1) % ghostTextures.size();
            }
            continue;
        }

        // --- rendu Pac-Man / fantômes ---
        if (s.role == PlayerRole::PacMan)
        {
            gf::Vector2f moveVec = m.destPos - m.startPos;

            if (std::abs(moveVec.x) > std::abs(moveVec.y))
            {
                if (moveVec.x > 0 && m_pacmanDir != 'R')
                {
                    m_pacmanDir = 'R';
                    m_pacmanSprite.setAnimation(m_pacmanRightAnim);
                }
                else if (moveVec.x < 0 && m_pacmanDir != 'L')
                {
                    m_pacmanDir = 'L';
                    m_pacmanSprite.setAnimation(m_pacmanLeftAnim);
                }
            }
            else if (std::abs(moveVec.y) > 0.001f)
            {
                if (moveVec.y > 0 && m_pacmanDir != 'D')
                {
                    m_pacmanDir = 'D';
                    m_pacmanSprite.setAnimation(m_pacmanDownAnim);
                }
                else if (moveVec.y < 0 && m_pacmanDir != 'U')
                {
                    m_pacmanDir = 'U';
                    m_pacmanSprite.setAnimation(m_pacmanUpAnim);
                }
            }

            m_pacmanSprite.setPosition({px, py});
            target.draw(m_pacmanSprite, states);

            gf::Text scoreText;
            scoreText.setFont(m_font);
            scoreText.setCharacterSize(16u);
            scoreText.setColor(gf::Color::White);
            scoreText.setString(std::to_string(s.score));
            lastScore=s.score;
            scoreText.setPosition({px + 5.f, py - 18.f});
            target.draw(scoreText, states);

            m_lastPacmanX = px;
            m_lastPacmanY = py;
            m_hasLastPacmanPos = true;
        }
        else
        {
            if (ghostTextures[ghostIndex].getSize().x > 0)
            {
                float s = tilePx / float(ghostTextures[ghostIndex].getSize().x);
                ghostSprite.setScale({s, s});
            }
            ghostSprite.setTexture(m_pacmanPowerActive ? m_ghostVulnerableTexture : ghostTextures[ghostIndex]);
            ghostSprite.setPosition({px, py});
            target.draw(ghostSprite, states);
            ghostIndex = (ghostIndex + 1) % ghostTextures.size();
        }

        m.startPos = currentPos;
        m.startTime = now;
    }
}

void GameEntity::renderMyLife(gf::RenderTarget &target, const gf::RenderStates &states, float logicalTileSize, float mapOriginX, float mapOriginY, float topMargin)
{
    unsigned labelSize = 14u;
    unsigned numberSize = 18u;

    float mapLeft = mapOriginX;
    float mapRight = mapOriginX + m_board.width * logicalTileSize;
    float mapCenter = (mapLeft + mapRight) * 0.5f;

    float labelY = topMargin * 0.35f;
    float numberY = labelY + 20.f;

    gf::Text label;
    label.setFont(m_font);
    label.setCharacterSize(labelSize);
    label.setColor(gf::Color::White);
    label.setString("PV PERSO");

    gf::RectF bounds = label.getLocalBounds();
    label.setPosition({mapCenter - (bounds.max.x - bounds.min.x) * 0.5f, labelY});
    target.draw(label, states);

    gf::Text value;
    value.setFont(m_font);
    value.setCharacterSize(numberSize);
    value.setColor(gf::Color::White);
    value.setString(std::to_string(m_myHp));

    gf::RectF valBounds = value.getLocalBounds();
    value.setPosition({mapCenter - (valBounds.max.x - valBounds.min.x) * 0.5f, numberY});
    target.draw(value, states);
}

void GameEntity::renderPacmanLife(gf::RenderTarget &target, const gf::RenderStates &states, float logicalTileSize, float mapOriginX, float mapOriginY, float topMargin)
{

    unsigned labelSize = 14u;
    unsigned numberSize = 18u;

    float mapRight = mapOriginX + m_board.width * logicalTileSize;

    float labelY = topMargin * 0.35f;
    float numberY = labelY + 20.f;

    gf::Text label;
    label.setFont(m_font);
    label.setCharacterSize(labelSize);
    label.setColor(gf::Color::White);
    label.setString("PV PACMAN");

    gf::RectF bounds = label.getLocalBounds();
    label.setPosition({mapRight - (bounds.max.x - bounds.min.x), labelY});
    target.draw(label, states);

    gf::Text value;
    value.setFont(m_font);
    value.setCharacterSize(numberSize);
    value.setColor(gf::Color::White);
    value.setString(std::to_string(m_pacmanHp));

    gf::RectF valBounds = value.getLocalBounds();
    value.setPosition({mapRight - (valBounds.max.x - valBounds.min.x), numberY});
    target.draw(value, states);
}

void GameEntity::render(gf::RenderTarget &target, const gf::RenderStates &states)
{

    const float LOGICAL_W = 1280.f;
    const float LOGICAL_H = 720.f;

    if (m_board.width == 0 || m_board.height == 0)
        return;

    float topMargin = 100.f; // espace réservé au timer en haut
    float padding = 20.f;    // marge autour de la map

    float logicalAvailableW = LOGICAL_W - 2 * padding;
    float logicalAvailableH = LOGICAL_H - topMargin - padding; // hauteur dispo pour la map

    float logicalTileX = logicalAvailableW / float(m_board.width);
    float logicalTileY = logicalAvailableH / float(m_board.height);
    float logicalTileSize = std::min(logicalTileX, logicalTileY);

    float mapOriginX = padding + (logicalAvailableW - m_board.width * logicalTileSize) / 2.f;
    float mapOriginY = topMargin + (logicalAvailableH - m_board.height * logicalTileSize) / 2.f;

    float timerX = 20.f; // marge gauche
    float timerY = 50.f; // toujours visible en haut

    renderMap(target, states, m_board, logicalTileSize, mapOriginX, mapOriginY);
    renderPacGommes(target, states, m_pacgommes, logicalTileSize, mapOriginX, mapOriginY);
    renderSprites(target, states, logicalTileSize, mapOriginX, mapOriginY);

    unsigned labelSize = 14u;
    unsigned numberSize = 18u;

    float mapLeft = mapOriginX;

    float labelY = topMargin * 0.35f;
    float numberY = labelY + 20.f;

    gf::Text label;
    label.setFont(m_font);
    label.setCharacterSize(labelSize);
    label.setColor(gf::Color::White);
    if (m_timeLeftPre != 0)
        label.setString("TIMER PRE-START : ");
    else
        label.setString("TIMER : ");

    label.setPosition({mapLeft, labelY});
    target.draw(label, states);

    unsigned timeValue = (m_timeLeftPre != 0) ? m_timeLeftPre : m_timeLeft;

    gf::Text value;
    value.setFont(m_font);
    value.setCharacterSize(numberSize);
    value.setColor(gf::Color::White);
    value.setString(std::to_string(timeValue));
    value.setPosition({mapLeft, numberY});
    target.draw(value, states);

    // les vies

    if (m_myRole == PlayerRole::Ghost)
    {
        renderMyLife(target, states, logicalTileSize, mapOriginX, mapOriginY, topMargin);
        renderPacmanLife(target, states, logicalTileSize, mapOriginX, mapOriginY, topMargin);
    }
    else if (m_myRole == PlayerRole::PacMan)
    {
        renderPacmanLife(target, states, logicalTileSize, mapOriginX, mapOriginY, topMargin);
    }
    else
    {
        renderPacmanLife(target, states, logicalTileSize, mapOriginX, mapOriginY, topMargin);
    }

    // mode chasseur
    renderPacManPower(target, states, mapOriginX, logicalTileSize, topMargin);
}
