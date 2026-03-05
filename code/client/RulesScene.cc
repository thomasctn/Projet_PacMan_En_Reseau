
#include "RulesScene.h"
#include "ClientGame.h"

bool firstFrame_RulesScene = true;


RulesScene::RulesScene(ClientGame& game)
: gf::Scene(gf::vec(1280,720))  
, m_game(game)
, m_font("../common/fonts/arial.ttf")
, m_entity(m_font)
{
    setClearColor(gf::Color::Black);
    addWorldEntity(m_entity);
    
}

void RulesScene::doProcessEvent(gf::Event& event) {
    switch (event.type) {
        case gf::EventType::MouseMoved:
            m_entity.pointTo(
                m_game.computeWindowToGameCoordinates(event.mouseCursor.coords, getWorldView())
            );
            break;

        case gf::EventType::MouseButtonPressed:
            m_entity.pointTo(
                m_game.computeWindowToGameCoordinates(event.mouseButton.coords, getWorldView())
            );
            m_entity.triggerAction();
            break;

        case gf::EventType::Closed: 
            gf::Log::info("Fenêtre fermée (LobbyListScene)\n");
            m_game.shutdown();
            break;
        case gf::EventType::Resized: {
            resizeYourself();
            break;
        }

        default:
        break;
    }
}

void RulesScene::doUpdate(gf::Time time) {
    if (firstFrame_RulesScene) {
        resizeYourself();
        firstFrame_RulesScene = false;
    }

    //logique du bouton
    if (m_entity.wasClicked()) {
        m_entity.resetClick();
        m_game.requestScene(SceneRequest::GoToWelcome);
    }
}

void RulesScene::resizeYourself(){
    auto size = m_game.getWindow().getSize(); 
    m_game.handleResize(size.x, size.y);
    getWorldView() = m_game.getMainView();
}

void RulesScene::doShow()
{
    std::thread([&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            resizeYourself();
        }).detach();
}
