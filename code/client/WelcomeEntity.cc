#include "WelcomeEntity.h"
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>
#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/Color.h>


WelcomeEntity::WelcomeEntity(gf::Font& font)
: m_enterWidget("ENTRER", font)
, m_rulesWidget("REGLES", font)
, m_font(font)
, m_font_emulogic("../common/fonts/Emulogic-zrEw.ttf")
{
    m_enterWidget.setCallback([this]() { m_clicked = true; });
    m_container.addWidget(m_enterWidget);

    m_rulesWidget.setCallback([this]() { m_rulesClicked = true; });
    m_container.addWidget(m_rulesWidget);
}

void WelcomeEntity::pointTo(gf::Vector2f coords) {
    m_container.pointTo(coords);   
  
}

void WelcomeEntity::triggerAction() {
    m_container.triggerAction();
}

bool WelcomeEntity::wasClicked() const {
    return m_clicked;
}

void WelcomeEntity::resetClick() {
    m_clicked = false;
}

bool WelcomeEntity::wasRulesClicked() const {
    return m_rulesClicked;
}

void WelcomeEntity::resetRulesClick() {
    m_rulesClicked = false;
}


void WelcomeEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    const float LOGICAL_W = 1280.f;
    const float LOGICAL_H = 720.f;

    unsigned titleSize = 70u; 
    gf::Text title;
    title.setFont(m_font_emulogic);
    title.setCharacterSize(titleSize);
    title.setString("PACMAN");
    title.setAnchor(gf::Anchor::Center);
    title.setColor(gf::Color::White);
    title.setPosition({ LOGICAL_W * 0.5f, LOGICAL_H * 0.2f });
    target.draw(title, states);

    unsigned subTitleSize = 22u; 
    gf::Text subtitle;
    subtitle.setFont(m_font_emulogic);
    subtitle.setCharacterSize(subTitleSize);
    subtitle.setString("EN RESEAU");
    subtitle.setAnchor(gf::Anchor::Center);
    subtitle.setColor(gf::Color::White);
    subtitle.setPosition({ LOGICAL_W * 0.5f, LOGICAL_H * 0.3f });
    target.draw(subtitle, states);

    unsigned charSize = 26u;

    float bw = LOGICAL_W * 0.3f;
    float bh = LOGICAL_H * 0.08f;
    float bx = (LOGICAL_W - bw) * 0.5f;
    float by = LOGICAL_H * 0.45f;

    m_enterWidget.setCharacterSize(charSize);
    m_enterWidget.setAnchor(gf::Anchor::Center);
    m_enterWidget.setPosition({ bx + bw * 0.5f, by + bh * 0.5f });

    m_enterWidget.setDefaultTextColor(gf::Color::White);
    m_enterWidget.setSelectedTextColor(gf::Color::Black);
    m_enterWidget.setDefaultBackgroundColor(gf::Color::Black);
    m_enterWidget.setSelectedBackgroundColor(gf::Color::White);

    m_enterWidget.setBackgroundOutlineThickness(charSize * .05f);
    m_enterWidget.setDefaultBackgroundOutlineColor(gf::Color::White);
    m_enterWidget.setSelectedBackgroundOutlineColor(gf::Color::White);
    m_enterWidget.setPadding(charSize * .4f);

    float centerX = LOGICAL_W * 0.5f;

    float rulesY = by + 80.f; 

    m_rulesWidget.setCharacterSize(charSize);
    m_rulesWidget.setAnchor(gf::Anchor::Center);
    m_rulesWidget.setPosition({ centerX, rulesY });

    m_rulesWidget.setDefaultTextColor(gf::Color::White);
    m_rulesWidget.setSelectedTextColor(gf::Color::Black);
    m_rulesWidget.setDefaultBackgroundColor(gf::Color::Black);
    m_rulesWidget.setSelectedBackgroundColor(gf::Color::White);
    m_rulesWidget.setBackgroundOutlineThickness(charSize * .05f);
    m_rulesWidget.setDefaultBackgroundOutlineColor(gf::Color::White);
    m_rulesWidget.setSelectedBackgroundOutlineColor(gf::Color::White);
    m_rulesWidget.setPadding(charSize * .4f);

    target.draw(m_rulesWidget, states);

    target.draw(m_enterWidget, states);

}

