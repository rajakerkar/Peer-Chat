#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QHostAddress>

class NetworkManager : public QObject {
    Q_OBJECT
public:
    explicit NetworkManager(QString username, QObject *parent = nullptr);
    void sendBroadcast(); 
    void sendGroupMessage(QString message); 
    void sendMessage(QString targetIp, QString message); 

signals:
    void userDiscovered(QString name, QString ip);
    void userLeft(QString name, QString ip);
    void messageReceived(QString fromName, QString message);

private slots:
    void processBroadcasts();
    void handleNewConnection();
    void readIncomingMessage();
    void checkOnlineStatus();

private:
    QString m_username;
    QUdpSocket *udpSocket;
    QTcpServer *tcpServer;
    QMap<QString, QString> onlineUsers;
    QMap<QString, qint64> lastSeen;
};

#endif