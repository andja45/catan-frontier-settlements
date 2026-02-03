//
// Created by matija on 2/1/26.
//

#ifndef CATAN_HOSTJOINCONTROLLER_HPP
#define CATAN_HOSTJOINCONTROLLER_HPP

#include <HostJoinNetworkAdapter.hpp>
#include <NetworkTransport.h>
#include <QObject>
#include <screens/connect/HostJoinView.h>

class HostJoinController : public QObject {
    Q_OBJECT
public:
    HostJoinController(RoleType role,HostJoinView* view, NetworkTransport* transport);

signals:
    void rejected(std::string); // notify view display hint

    void accepted(); // host app, view will close
    void closed(); // notify host app go back to menu

private:
    RoleType m_role;
    HostJoinView* m_view;
    HostJoinNetworkAdapter* m_transport;
};

#endif //CATAN_HOSTJOINCONTROLLER_HPP