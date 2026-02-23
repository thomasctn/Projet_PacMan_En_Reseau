#include <gf/TcpSocket.h>
#include <gf/Packet.h>
#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Shapes.h>

#include <gf/Action.h>
#include <gf/Color.h>
#include <gf/Log.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <gf/Keyboard.h>
#include <gf/Event.h>
#include <mutex>
#include <queue>
#include "../common/Types.h"
#include "../common/Constants.h"
#include "Renderer.h"
#include "WelcomeScene.h"
#include "WelcomeEntity.h"
#include "LobbyScene.h"
#include "GameScene.h"
#include "GameEntity.h"
#include "EndScene.h"
#include "LobbyListScene.h"
#include "ClientNetworkHandler.h"


#include "ClientGame.h"

int main(int argc, const char * argv[]) {
    if(argc > 3) {
        fprintf(stderr,"Invalid number of argument\n");
        printf("Usage : %s serv_ip serv_port\n",argv[0]);
        return -1;
    }
    ClientGame game;
    std::string ip = "127.0.0.1";
    std::string port = "5000";
    if(argc == 2) {
        ip = argv[1];
    }
    if(argc == 3) {
        port = argv[2];
    }
    game.run(ip, port);
    return 0;
}