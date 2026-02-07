//
// Created by matija on 2/1/26.
//

#include "HostJoinController.hpp"

#include <QTimer>

HostJoinController::HostJoinController(RoleType role, HostJoinView *view, NetworkTransport *transport) : QObject(nullptr), m_view(view), m_role(role) {
    m_transport = new HostJoinNetworkAdapter(this);
    m_transport->setTransport(transport);

    // if we dont make connection we show error
    QTimer* m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->setInterval(5000);
    m_timer->start();


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

    connect(m_timer, &QTimer::timeout, this, [this]() {
        if (!m_transport->isConnected()) {
            m_view->onRejected("Server not responding...");
        }
    });
}

