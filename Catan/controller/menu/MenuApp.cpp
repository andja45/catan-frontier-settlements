//
// Created by matija on 2/2/26.
//

#include "MenuApp.hpp"


MenuApp::MenuApp(ApplicationRoot *root): AbstractApplet(root) {
    m_mainMenuView=new MainMenu();
    m_mainMenuView->connect(m_mainMenuView,&MainMenu::hostGame,[this]() {
        m_root->showHostPopup();
    });
    m_mainMenuView->connect(m_mainMenuView,&MainMenu::joinGame,[this]() {
        m_root->showJoinPopup();
    });
    m_mainMenuView->connect(m_mainMenuView, &MainMenu::quit, [this]() {
        m_root->closeApp();
    });
}

void MenuApp::show() {
    m_mainMenuView->show();
}

void MenuApp::hide() {
    m_mainMenuView->hide();
}
