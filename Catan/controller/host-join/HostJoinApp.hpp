//
// Created by matija on 2/1/26.
//

#ifndef CATAN_HOSTJOINAPP_HPP
#define CATAN_HOSTJOINAPP_HPP
#include <QObject>
#include <qobjectdefs.h>
#include <QString>
#include <qtmetamacros.h>
#include <common/AbstractApplet.hpp>
#include <common/ApplicationRoot.hpp>
#include <host-join/HostJoinController.hpp>
#include <model/GameConfig.h>
#include <screens/connect/HostJoinView.h>

class HostJoinApp :  public AbstractApplet {

public:
    explicit HostJoinApp(ApplicationRoot *root,QWidget*parent, RoleType role);

    void show() override {m_view->show();}
    void hide() override {m_view->hide();}

    ~HostJoinApp() override{
        m_view->deleteLater();
        m_controller->deleteLater();
    }

private:

    HostJoinView* m_view;
    HostJoinController* m_controller;

    RoleType m_role;

};

#endif //CATAN_HOSTJOINAPP_HPP