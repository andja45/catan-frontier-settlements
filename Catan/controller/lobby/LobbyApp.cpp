//
// Created by matija on 1/27/26.
//

#include "LobbyApp.hpp"

#include "../ApplicationRoot.hpp"

LobbyApp::LobbyApp(ApplicationRoot *root, Role role)
    : AbstractApplet(root){

    switch (role) {
        case Role::Host:
            // make appropriate window

            break;
        case Role::Player:



            break;
    }

    // create everything and connect signals!

}

void LobbyApp::start() {
}

void LobbyApp::exit() {
}

void LobbyApp::switchToNext() {
    //get data
    //m_root->startGame()
}

void LobbyApp::switchToPrevious() {
    //destroy resources?
    m_root->showMainMenu();
}
