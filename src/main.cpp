#include <QApplication>
#include <QInputDialog>
#include "ChatWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    bool ok;
    QString name = QInputDialog::getText(nullptr, "Login", "Enter your Username:", 
                                         QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        ChatWindow window(name);
        window.show();
        return app.exec();
    }

    return 0;
}