#include "ChatWindow.h"

ChatWindow::ChatWindow(QString username, QWidget *parent) 
    : QMainWindow(parent), m_username(username) {
    
    setWindowTitle("root@p2p-mesh:~# " + username);
    resize(800, 500);

    // --- HACKER THEME STYLE SHEET ---
    this->setStyleSheet(
        "QMainWindow { background-color: #0a0a0a; }"
        "QListWidget { "
        "   background-color: #0f0f0f; "
        "   color: #00ff00; "
        "   border: 1px solid #004400; "
        "   font-family: 'Monospace'; "
        "   font-size: 12px; "
        "}"
        "QTextEdit { "
        "   background-color: #050505; "
        "   color: #00ff00; "
        "   border: 1px solid #004400; "
        "   font-family: 'Monospace'; "
        "   selection-background-color: #004400; "
        "}"
        "QLineEdit { "
        "   background-color: #0f0f0f; "
        "   color: #00ff00; "
        "   border: 1px solid #00ff00; "
        "   padding: 5px; "
        "   font-family: 'Monospace'; "
        "}"
        "QPushButton { "
        "   background-color: #002200; "
        "   color: #00ff00; "
        "   border: 1px solid #00ff00; "
        "   padding: 5px 15px; "
        "   font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #004400; }"
    );

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    userList = new QListWidget();
    userList->setFixedWidth(220);
    
    // Global Group with a distinct neon look
    QListWidgetItem *group = new QListWidgetItem(" [!] GLOBAL_NET ");
    group->setForeground(QBrush(Qt::black));
    group->setBackground(QBrush(QColor(0, 255, 0))); 
    userList->addItem(group);
    
    mainLayout->addWidget(userList);

    QVBoxLayout *chatLayout = new QVBoxLayout();
    chatDisplay = new QTextEdit();
    chatDisplay->setReadOnly(true);
    
    // System boot messages
    chatDisplay->append("<span style='color:#00ff00;'>[SYSTEM] P2P Mesh Network Initialized...</span>");
    chatDisplay->append("<span style='color:#00ff00;'>[SYSTEM] Identity Verified: " + m_username + "</span>");
    
    inputField = new QLineEdit();
    inputField->setPlaceholderText("type command or message...");
    QPushButton *sendBtn = new QPushButton("RUN >");

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(inputField);
    inputLayout->addWidget(sendBtn);

    chatLayout->addWidget(chatDisplay);
    chatLayout->addLayout(inputLayout);
    mainLayout->addLayout(chatLayout);

    setCentralWidget(central);

    netManager = new NetworkManager(username, this);

    connect(netManager, &NetworkManager::userDiscovered, this, &ChatWindow::addDiscoveredUser);
    connect(netManager, &NetworkManager::messageReceived, this, &ChatWindow::displayMessage);
    connect(netManager, &NetworkManager::userLeft, this, &ChatWindow::removeUser);
    connect(sendBtn, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(inputField, &QLineEdit::returnPressed, this, &ChatWindow::onSendClicked);
}

void ChatWindow::addDiscoveredUser(QString name, QString ip) {
    QString entry = name + " @" + ip;
    if (userList->findItems(entry, Qt::MatchExactly).isEmpty()) {
        userList->addItem(entry);
        chatDisplay->append("<span style='color:#00ff00;'>[+] New peer identified: " + name + "</span>");
    }
}

void ChatWindow::removeUser(QString name, QString ip) {
    QString entry = name + " @" + ip;
    QList<QListWidgetItem*> items = userList->findItems(entry, Qt::MatchExactly);
    for (QListWidgetItem* item : items) {
        delete userList->takeItem(userList->row(item));
    }
    chatDisplay->append("<span style='color:#ff0000;'>[-] Peer disconnected: " + name + "</span>");
}

void ChatWindow::displayMessage(QString fromName, QString message) {
    // This displays messages sent by OTHER people
    // Format: [Source] Username: Message
    chatDisplay->append("<span style='color:#00ff00;'><b>" + fromName + ":</b> " + message + "</span>");
}

void ChatWindow::onSendClicked() {
    QString msg = inputField->text();
    if (msg.isEmpty()) return;

    QListWidgetItem *current = userList->currentItem();
    if (!current) {
        chatDisplay->append("<span style='color:#ff0000;'>[ERROR] No target selected. Use GLOBAL_NET or select a peer.</span>");
        return;
    }

    if (current->text().contains("GLOBAL_NET")) {
        netManager->sendGroupMessage(msg);
        // Show YOUR message with YOUR username in the terminal
        chatDisplay->append("<span style='color:#008800;'><b>[GLOBAL] " + m_username + ":</b> " + msg + "</span>");
    } else {
        QString itemText = current->text();
        QString targetIp = itemText.section('@', 1);
        netManager->sendMessage(targetIp, msg);
        // Show YOUR private message with YOUR username
        chatDisplay->append("<span style='color:#00cc00;'><b>[PRIV] " + m_username + ":</b> " + msg + "</span>");
    }
    inputField->clear();
}