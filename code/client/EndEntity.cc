#include "EndEntity.h"

#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>
#include <gf/Anchor.h>
#include <gf/Text.h>
#include <gf/Shapes.h>
#include <gf/Log.h>

EndEntity::EndEntity()
: m_font("../common/fonts/arial.ttf")
, m_font_motion_control("../common/fonts/motion-control.bold.otf")
, m_font_mc_italic("../common/fonts/motion-control.bolditalic.otf")
, m_enterWidget("Retour au Lobby", m_font)
{
    m_enterWidget.setCallback([this](){
        m_lastAction = EndAction::BackToLobby;
    });
    m_container.addWidget(m_enterWidget);
    m_enterWidget.setCharacterSize(24);
    m_enterWidget.setAnchor(gf::Anchor::Center);
}

void EndEntity::pointTo(gf::Vector2f position) {
    m_container.pointTo(position);
}

void EndEntity::triggerAction() {
    m_container.triggerAction();
}

void EndEntity::setEndData(GameEndReason reason, int lastScore) {
    m_endReason = reason;
    m_lastScore = lastScore;
}

EndAction EndEntity::getAndResetLastAction() {
    EndAction a = m_lastAction;
    m_lastAction = EndAction::None;
    return a;
}

void EndEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    const float LOGICAL_W = 1280.f;
    const float LOGICAL_H = 720.f;

    float leftX  = LOGICAL_W * 0.2f;
    float rightX = LOGICAL_W * 0.55f;
    //titre a gauche
    unsigned int titleSize = 120u;

    gf::Text title;
    title.setFont(m_font_mc_italic);
    title.setCharacterSize(titleSize);
    title.setColor(gf::Color::White);
    title.setAnchor(gf::Anchor::TopLeft);

    title.setString("FIN\nDE JEU");
    title.setPosition({ leftX, LOGICAL_H * 0.3f });

    target.draw(title, states);

    //rectangle autour
    float playBoxWidth = 300.f;
    float playBoxHeight = 250.f;
    float playBoxLeft = leftX -20.f;
    float playBoxTop = LOGICAL_H * 0.3f -90.f;
    gf::RoundedRectangleShape playBox({ playBoxWidth, playBoxHeight }, 14.f);
    playBox.setPosition({ playBoxLeft, playBoxTop });
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(gf::Color::Blue);

    target.draw(playBox, states);


    //la raison
    unsigned int reasonSize = 36u;

    gf::Text reasonText;
    reasonText.setFont(m_font_motion_control);
    reasonText.setCharacterSize(reasonSize);
    reasonText.setColor(gf::Color::White);
    reasonText.setAnchor(gf::Anchor::TopLeft);


    std::string reasonStr;
    switch (m_endReason) {
        case GameEndReason::ALL_DOT_EATEN: 
            reasonStr = "Pacman a mangé toutes les pacgommes.\nPacman gagne !"; break;
        case GameEndReason::TIME_OUT:       
            reasonStr = "Le temps est écoulé sans \n que pacman ai mangé toutes \nles pacgommes.\nLes fantômes gagnent !"; break;
        case GameEndReason::PACMAN_DEATH:   
            reasonStr = "Pacman est mort trop de fois.\nLes fantômes gagnent!"; break;
        case GameEndReason::ALL_GHOST_DEATH:
            reasonStr = "Les fantômes sont tous mort\nPacman gagne !"; break;
        default:                            
            reasonStr = "Fin de la partie."; break;
    }

    reasonText.setString(reasonStr);
    reasonText.setPosition({ rightX, LOGICAL_H * 0.25f });
    target.draw(reasonText, states);

    //le score
    unsigned int scoreSize = 32u;

    gf::Text scoreText;
    scoreText.setFont(m_font_motion_control);
    scoreText.setCharacterSize(scoreSize);
    scoreText.setColor(gf::Color::White);
    scoreText.setAnchor(gf::Anchor::TopLeft);

    scoreText.setString("Score final : " + std::to_string(m_lastScore));
    scoreText.setPosition({ rightX , LOGICAL_H * 0.50f });

    target.draw(scoreText, states);

    float bw = 400.f; // largeur du bouton
    float bh = 80.f;  // hauteur du bouton
    float bx = (LOGICAL_W - bw) * 0.5f;
    float by = LOGICAL_H * 0.60f;

    unsigned int btnChar = 32u;

    m_enterWidget.setCharacterSize(btnChar);
    m_enterWidget.setAnchor(gf::Anchor::Center);
    m_enterWidget.setPosition({ bx + bw * 0.5f, by + bh * 0.5f });

    m_enterWidget.setDefaultTextColor(gf::Color::White);
    m_enterWidget.setSelectedTextColor(gf::Color::Black);
    m_enterWidget.setDefaultBackgroundColor(gf::Color::Black);
    m_enterWidget.setSelectedBackgroundColor(gf::Color::White);
    m_enterWidget.setBackgroundOutlineThickness(btnChar * 0.05f);
    m_enterWidget.setDefaultBackgroundOutlineColor(gf::Color::White);
    m_enterWidget.setSelectedBackgroundOutlineColor(gf::Color::White);
    m_enterWidget.setPadding(btnChar * 0.4f);

    target.draw(m_enterWidget, states);
}
