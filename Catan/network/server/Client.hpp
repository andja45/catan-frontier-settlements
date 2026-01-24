//
// Created by matija on 1/24/26.
//

#ifndef CATAN_CLIENTCONNECTION_HPP
#define CATAN_CLIENTCONNECTION_HPP


class Client : public QObject {
    Q_OBJECT
public:
    ClientId id() const;
    void send(const Envelope&);

    signals:
        void envelopeReceived(Client*, const Envelope&);
    void disconnected(Client*);
};

#endif //CATAN_CLIENTCONNECTION_HPP