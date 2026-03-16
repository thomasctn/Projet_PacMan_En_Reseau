#pragma once

#include <gf/Entity.h>
#include <gf/WidgetContainer.h>
#include <gf/Widgets.h>
#include <gf/Font.h>

class WelcomeEntity : public gf::Entity {
public:
    explicit WelcomeEntity(gf::Font& font);

    void pointTo(gf::Vector2f coords);
    void triggerAction();

    bool wasClicked() const;
    void resetClick();

    bool wasRulesClicked() const;   
    void resetRulesClick(); 

    

    void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

private:
    gf::TextButtonWidget m_enterWidget;
    gf::TextButtonWidget m_rulesWidget;
    gf::WidgetContainer m_container;
    bool m_clicked = false;
    bool m_rulesClicked = false; 
    gf::Font& m_font;
    gf::Font m_font_emulogic;
};
