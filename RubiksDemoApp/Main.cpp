#include "LoginDialog.h"
#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.showMaximized();
        return app.exec();
    }

    return 0;
}

