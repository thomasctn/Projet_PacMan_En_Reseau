#include "RulesEntity.h"
#include <gf/RenderTarget.h>
#include <gf/RenderStates.h>
#include <gf/Coordinates.h>
#include <gf/Text.h>
#include <gf/Color.h>


RulesEntity::RulesEntity(gf::Font& font)
: m_returnWidget("RETOUR", font)
, m_font(font)
, m_font_emulogic("../common/fonts/Emulogic-zrEw.ttf")
{
    m_returnWidget.setCallback([this]() { m_clicked = true; });
    m_container.addWidget(m_returnWidget);
}

void RulesEntity::pointTo(gf::Vector2f coords) {
    m_container.pointTo(coords);   
  
}

void RulesEntity::triggerAction() {
    m_container.triggerAction();
}

bool RulesEntity::wasClicked() const {
    return m_clicked;
}

void RulesEntity::resetClick() {
    m_clicked = false;
}

void RulesEntity::render(gf::RenderTarget& target, const gf::RenderStates& states) {
    const float LOGICAL_W = 1280.f;
    const float LOGICAL_H = 720.f;
    const float padding = 40.f;

    //    float q1 = LOGICAL_W * 0.25f; // 320
    float q1 = 280.f;
//    float q3 = LOGICAL_W * 0.75f; // 960
    float q3 = 970.f; // 960

    float lineSpacing = 30.f;


    //boutont retour en haut a gauche
    const unsigned btnChar = 26u;
    m_returnWidget.setCharacterSize(btnChar);
    m_returnWidget.setAnchor(gf::Anchor::TopLeft);
    m_returnWidget.setPosition({ padding, padding });
    m_returnWidget.setDefaultTextColor(gf::Color::White);
    m_returnWidget.setSelectedTextColor(gf::Color::Black);
    m_returnWidget.setDefaultBackgroundColor(gf::Color::Black);
    m_returnWidget.setSelectedBackgroundColor(gf::Color::White);
    m_returnWidget.setBackgroundOutlineThickness(btnChar * .05f);
    m_returnWidget.setDefaultBackgroundOutlineColor(gf::Color::White);
    m_returnWidget.setSelectedBackgroundOutlineColor(gf::Color::White);
    m_returnWidget.setPadding(btnChar * .4f);
    target.draw(m_returnWidget, states);

    //gros titre
    const float headerTop = LOGICAL_H * 0.12f;

    gf::Text title;
    title.setFont(m_font_emulogic);
    title.setCharacterSize(92u);
    title.setString("PACMAN");
    title.setAnchor(gf::Anchor::TopLeft);
    title.setColor(gf::Color::White);
    title.setPosition({ q1, headerTop });
    target.draw(title, states);

    gf::Text subtitle;
    subtitle.setFont(m_font_emulogic);
    subtitle.setCharacterSize(40u);
    subtitle.setString("EN RESEAU");
    subtitle.setAnchor(gf::Anchor::TopLeft);
    subtitle.setColor(gf::Color::White);
    subtitle.setPosition({ q1, headerTop + 92.f + 8.f });
    target.draw(subtitle, states);


    const float rightZoneTop = LOGICAL_H * 0.12f;
    float sectionTop = LOGICAL_H * 0.38f;   //hauteur de départ !!

    gf::Text playHeading;
    playHeading.setFont(m_font);
    playHeading.setCharacterSize(28u);
    playHeading.setString("Comment jouer?");
    playHeading.setAnchor(gf::Anchor::TopRight);
    playHeading.setColor(gf::Color::White);
    playHeading.setPosition({ q3, sectionTop });
    target.draw(playHeading, states);


    const unsigned bulletSize = 18u;
    float bulletY = rightZoneTop + 34.f;
    const float bulletSpacing = 28.f;

    std::vector<std::string> playLines ={
        "- Utilisez les flèches du clavier pour vous déplacer",
        "- Les portails sur les bords de maps permettent de se téléporter rapidement",
        "- Les pacgommes jaunes font augmenter le score de Pacman",
        "- Les pacgommes vertes permettent à Pacman de passer en mode Chasseur et \nde manger les fantômes"
    };

    float playY = sectionTop + 46.f; //les play lignes hauteur

    //rectangle
    float boxPadding = 20.f;

    float playBoxLeft = q1 - boxPadding;
    float playBoxRight = q3 + boxPadding;   // élargit à droite aussi
    float playBoxWidth = playBoxRight - playBoxLeft;

    float playBoxTop = sectionTop + 46.f - 10.f;  
    float playBoxHeight = playLines.size() * lineSpacing +30.f;

    gf::RoundedRectangleShape playBox({ playBoxWidth, playBoxHeight }, 14.f);
    playBox.setPosition({ playBoxLeft, playBoxTop });
    playBox.setColor(gf::Color::Transparent);
    playBox.setOutlineThickness(2.f);
    playBox.setOutlineColor(gf::Color::Blue);

    target.draw(playBox, states);


        

    for (const auto& line : playLines) {
        gf::Text t;
        t.setFont(m_font);
        t.setCharacterSize(18u);
        t.setString(line);
        t.setAnchor(gf::Anchor::TopLeft);
        t.setColor(gf::Color::White);
        t.setPosition({ q1, playY });
        target.draw(t, states);
        playY += lineSpacing;
    }


    //comment gaggner
    float winTop = playY + 50.f;

    gf::Text winHeading;
    winHeading.setFont(m_font);
    winHeading.setCharacterSize(28u);
    winHeading.setString("Comment gagner?");
    winHeading.setAnchor(gf::Anchor::TopLeft);
    winHeading.setColor(gf::Color::White);
    winHeading.setPosition({ q1, winTop });
    target.draw(winHeading, states);


    float winBulletY = winTop + 46.f;
    std::vector<std::string> winLines = {
        "- Pacman gagne si il a mangé toutes les pacgommes ou si il a mangé tous les fantômes.",
        "- Les Fantômes gagnent si Pacman n'a plus de vie ou si le temps est écoulé."
    };

    float winY = winTop + 40.f;

    float winBoxLeft = q1 - boxPadding;
    float winBoxRight = q3 + boxPadding;
    float winBoxWidth = winBoxRight - winBoxLeft;

    int winTotalLines = winLines.size();

    float winBoxHeight = winTotalLines * lineSpacing +10.f;

    gf::RoundedRectangleShape winBox({ winBoxWidth, winBoxHeight }, 14.f);
    winBox.setPosition({ winBoxLeft, winY - 10.f }); 
    winBox.setColor(gf::Color::Transparent);
    winBox.setOutlineThickness(2.f);
    winBox.setOutlineColor(gf::Color::Blue);

    target.draw(winBox, states);

    for (const auto& line : winLines) {
        gf::Text t;
        t.setFont(m_font);
        t.setCharacterSize(18u);
        t.setString(line);
        t.setAnchor(gf::Anchor::TopLeft);
        t.setColor(gf::Color::White);
        t.setPosition({ q1, winY });
        target.draw(t, states);
        winY += lineSpacing;
    }

}
