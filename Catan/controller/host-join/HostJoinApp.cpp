//
// Created by matija on 2/2/26.
//

#include "HostJoinApp.hpp"

HostJoinApp::HostJoinApp(ApplicationRoot *root,QWidget*parent, RoleType role): AbstractApplet(root),m_role(role){
    m_view=new HostJoinView(role,parent);
    m_controller=new HostJoinController(m_role,m_view,m_root->getNetworkService()->getTransport());

    m_controller->connect(m_controller,&HostJoinController::accepted,[this]() {
        if (m_role==RoleType::Host){
            m_root->showLobbyHost();
        }else {
            m_root->showLobbyPlayer();
        }
    });
    m_controller->connect(m_controller,&HostJoinController::closed,[this]() {
        m_root->showMainMenu();
    });
}
