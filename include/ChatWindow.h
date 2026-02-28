#ifndef CHATWINDOW_H
#define CHATWINDOW_H
#include <QNetworkDatagram>

#include <QMainWindow>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "NetworkManager.h"

class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    ChatWindow(QString username, QWidget *parent = nullptr);

private slots:
    void onSendClicked();
    void addDiscoveredUser(QString name, QString ip);
    void displayMessage(QString fromName, QString message);
    void removeUser(QString name, QString ip);

private:
    NetworkManager *netManager;
    QListWidget *userList;
    QTextEdit *chatDisplay;
    QLineEdit *inputField;
    QString m_username;
};



#endif