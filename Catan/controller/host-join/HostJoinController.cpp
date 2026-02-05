//
// Created by matija on 2/1/26.
//

#include "HostJoinController.hpp"

HostJoinController::HostJoinController(RoleType role, HostJoinView *view, NetworkTransport *transport) : QObject(nullptr), m_view(view), m_role(role) {
    m_transport = new HostJoinNetworkAdapter(this);
    m_transport->setTransport(transport);

    connect(m_view, &HostJoinView::requestSent,this, [this](const QString &gameName, const QString &playerName) {
        if (m_role==RoleType::Host) {
            m_transport->sendHost(gameName.toStdString(), playerName.toStdString());
        } else {
            m_transport->sendJoin(gameName.toStdString(), playerName.toStdString());
        }
    });

    connect(m_transport, &HostJoinNetworkAdapter::rejectReceived,m_view,&HostJoinView::onRejected);

    connect(m_view,&HostJoinView::closed,[this]() {
        emit closed();
    });

    connect(m_transport, &HostJoinNetworkAdapter::acceptReceived, this, [this]() {
        emit accepted();
    });


}

