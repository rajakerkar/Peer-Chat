#include "NetworkManager.h"
#include <QTimer>
#include <QNetworkDatagram>
#include <QDateTime>

NetworkManager::NetworkManager(QString username, QObject *parent) 
    : QObject(parent), m_username(username) {
    
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket, &QUdpSocket::readyRead, this, &NetworkManager::processBroadcasts);

    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 12345);
    connect(tcpServer, &QTcpServer::newConnection, this, &NetworkManager::handleNewConnection);

    QTimer *bTimer = new QTimer(this);
    connect(bTimer, &QTimer::timeout, this, &NetworkManager::sendBroadcast);
    bTimer->start(3000);

    QTimer *rTimer = new QTimer(this);
    connect(rTimer, &QTimer::timeout, this, &NetworkManager::checkOnlineStatus);
    rTimer->start(5000);
}

void NetworkManager::sendBroadcast() {
    udpSocket->writeDatagram(m_username.toUtf8(), QHostAddress("255.255.255.255"), 45454);
}

void NetworkManager::sendGroupMessage(QString message) {
    QByteArray datagram = ("GRP:" + m_username + ":" + message).toUtf8();
    udpSocket->writeDatagram(datagram, QHostAddress("255.255.255.255"), 45454);
}

void NetworkManager::processBroadcasts() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        QString rawData = QString::fromUtf8(datagram.data());
        QString ip = datagram.senderAddress().toString();
        if (ip.startsWith("::ffff:")) ip = ip.mid(7);

        if (rawData.startsWith("GRP:")) {
            QStringList parts = rawData.split(":");
            if (parts.size() >= 3) {
                QString senderName = parts[1];
                QString message = parts[2];
                if (senderName != m_username) {
                    emit messageReceived("[Global] " + senderName, message);
                }
            }
        } else {
            if (rawData != m_username) {
                lastSeen[ip] = QDateTime::currentSecsSinceEpoch();
                if (!onlineUsers.contains(ip)) {
                    onlineUsers[ip] = rawData;
                    emit userDiscovered(rawData, ip);
                }
            }
        }
    }
}

void NetworkManager::checkOnlineStatus() {
    qint64 currentTime = QDateTime::currentSecsSinceEpoch();
    QMutableMapIterator<QString, qint64> i(lastSeen);
    while (i.hasNext()) {
        i.next();
        if (currentTime - i.value() > 10) {
            QString ipToRemove = i.key();
            QString nameToRemove = onlineUsers[ipToRemove];
            onlineUsers.remove(ipToRemove);
            i.remove();
            emit userLeft(nameToRemove, ipToRemove);
        }
    }
}

void NetworkManager::sendMessage(QString targetIp, QString message) {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(targetIp), 12345);
    if (socket->waitForConnected(3000)) {
        QString data = m_username + "|" + message;
        socket->write(data.toUtf8());
        socket->waitForBytesWritten();
        socket->disconnectFromHost();
    }
    socket->deleteLater();
}

void NetworkManager::handleNewConnection() {
    QTcpSocket *socket = tcpServer->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &NetworkManager::readIncomingMessage);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void NetworkManager::readIncomingMessage() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();
    QString strData = QString::fromUtf8(data);
    QStringList parts = strData.split("|");
    if (parts.size() >= 2) {
        emit messageReceived("[Private] " + parts[0], parts[1]);
    }
}