//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CLIENTCONNECTION_HPP
#define CATAN_CLIENTCONNECTION_HPP


class ClientConnection : public QObject {
    Q_OBJECT
public:
    ClientId id() const;
    void send(const Envelope&);

    signals:
        void envelopeReceived(ClientConnection*, const Envelope&);
    void disconnected(ClientConnection*);
};

#endif //CATAN_CLIENTCONNECTION_HPP