//
// Created by jovan on 24/01/2026.
//

#include "NetworkTransport.h"
#include <QHostAddress>

NetworkTransport::NetworkTransport(QObject* parent) : QObject(parent)
{
    m_framer.setMessageCallback([this](const QByteArray& msg) {
        net::Envelope env;
        if (!env.ParseFromArray(msg.data(), msg.size())) {
            emit errored("Invalid envelope received");
            return;
        }
        if (env.seq()!= m_nextSeqToReceive) {
            emit errored("Invalid sequence number received:");
            return;
        }
        if (env.msg_type()==net::MSG_ERROR) {
            emit errored(env.error().message());
        }
        m_nextSeqToReceive++;
        emit envelopeReceived(env);
    });
}

void NetworkTransport::sendEnvelope(const net::Envelope& env) {
    net::Envelope envCopy = env;
    std::string bytes;
    envCopy.set_seq(m_nextSeqToSend);
    envCopy.SerializeToString(&bytes);
    QByteArray payload(bytes.data(), bytes.size());
    QByteArray framed = MessageFramer::frameMessage(payload);

    m_socket->write(framed);
    m_nextSeqToSend++;
}

void NetworkTransport::setSocket(QTcpSocket *socket) {
    this->m_socket = socket;
    connect(m_socket, &QTcpSocket::readyRead, this, [this]() {
        m_framer.onBytes(m_socket->readAll());
    });
    connect(socket, &QTcpSocket::disconnected,this, [this] {
        emit disconnected();
    });
    connect(socket,&QTcpSocket::errorOccurred, this, [socket, this](QAbstractSocket::SocketError error){
        qDebug() << "Socket error:" << socket->errorString();
        emit errored(socket->errorString().toStdString());
    });

}

void NetworkTransport::sendError(const std::string &error) {
    net::Envelope env;
    env.set_msg_type(net::MSG_ERROR);
    env.mutable_error()->set_message(error);
    sendEnvelope(env);
}

void NetworkTransport::sendAck() {
    net::Envelope env;
    env.set_msg_type(net::MSG_ACK);
    sendEnvelope(env);
}
