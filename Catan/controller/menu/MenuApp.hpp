//
// Created by matija on 2/1/26.
//

#ifndef CATAN_MENUAPP_HPP
#define CATAN_MENUAPP_HPP
#include <HostJoinNetworkAdapter.hpp>
#include <common/AbstractApplet.hpp>
#include <common/ApplicationRoot.hpp>
#include <screens/connect/HostJoinView.h>
#include <screens/mainmenu/MainMenu.h>

class MenuApp : public AbstractApplet{

public:
    explicit MenuApp(ApplicationRoot *root);
    ~MenuApp() override{
        m_mainMenuView->deleteLater();
    }
    void show();
    void hide();
    QWidget* getView() {return m_mainMenuView;}
private:
    MainMenu* m_mainMenuView;
};

#endif //CATAN_MENUAPP_HPP