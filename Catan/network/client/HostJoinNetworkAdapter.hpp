//
// Created by matija on 1/25/26.
//

#ifndef CATAN_HOSTJOINNETWORKADAPTER_HPP
#define CATAN_HOSTJOINNETWORKADAPTER_HPP
#include <cstdint>
#include <memory>
#include <NetworkTransport.h>
#include <string>

class HostJoinNetworkAdapter : public QObject {
    Q_OBJECT
private:
    NetworkTransport* m_transport;
    net::Envelope wrapHost(std::string gameName, std::string hostName);
    net::Envelope wrapJoin(std::string gameName, std::string playerName);
public:
    explicit HostJoinNetworkAdapter(QObject* parent = nullptr) {
        m_transport=nullptr;
    }

    void setTransport(NetworkTransport* );

    void onEnvelope(const net::Envelope& env);
    void handleAcceptResponse(const net::Envelope& env);
    void handleRejectResponse(const net::Envelope& env);


    void sendHost(std::string gameName, std::string hostName);
    void sendJoin(std::string gameName, std::string playerName);

signals:
    void rejectReceived(std::string reason);
    void acceptReceived();
    //disconnect?

};

#endif //CATAN_HOSTJOINNETWORKADAPTER_HPP