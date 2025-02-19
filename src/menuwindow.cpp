#include "../include/menuwindow.h"
#include "../include/JoinDialog.h"
#include "../include/CodeDialog.h"
#include "../include/utils.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHostAddress>
#include <QMessageBox>

MenuWindow::MenuWindow(QWidget *parent)
        : QMainWindow(parent), game(nullptr), server(nullptr), client(nullptr), ui(new Ui::MenuWindow) {
    ui->setupUi(this);
    connect(ui->startButton, &QPushButton::clicked, this, &MenuWindow::onStart);
    connect(ui->quitButton, &QPushButton::clicked, this, &MenuWindow::onQuit);
    connect(ui->joinButton, &QPushButton::clicked, this, &MenuWindow::onJoin);

}

MenuWindow::~MenuWindow() {
    delete game;
    delete server;
    delete client;
    delete ui;
}

void MenuWindow::onStart() {
    if (!server) {
        server = new GameServer(this);
        int port = 12345;
        server->startServer(port, 1, false);

        QString ip = getLocalIPAddress();
        QString joinCode = generateJoinCode(ip, port);

        CodeDialog codeDialog(joinCode, this);
        codeDialog.exec();
    }

    if (!client) {
        client = new GameClient(this);
        client->connectToServer(QHostAddress::LocalHost, 12345);
    }

    if (!game) {
        game = new Game();
        connect(game, &Game::gameClosed, this, &MenuWindow::onGameClosed);
    }

    hide();
    game->show();
}


void MenuWindow::onQuit() {
    close();
}

void MenuWindow::onGameClosed() {
    show();
    game->deleteLater();
    game = nullptr;
}

void MenuWindow::onJoin() {
    JoinDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString code = dialog.getCode();

        if (code.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un code !");
            return;
        }

        QPair<QString, int> decoded = decodeIPPort(code);
        QString ip = decoded.first;
        int port = decoded.second;

        if (ip.isEmpty() || port <= 0) {
            QMessageBox::warning(this, "Erreur", "Le code est invalide !");
            return;
        }

        if (!client) {
            client = new GameClient(this);
            client->connectToServer(QHostAddress(ip), port);
        }
    }
}



QString MenuWindow::generateJoinCode(const QString &ip, int port) {
    int a, b, c, d;
    sscanf(ip.toUtf8().constData(), "%d.%d.%d.%d", &a, &b, &c, &d);

    quint64 num = ((quint64)a << 24) |
                  ((quint64)b << 16) |
                  ((quint64)c << 8)  |
                  d;
    num = (num << 16) | port;

    const QString base = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString code;
    if (num == 0) return "0";
    while (num) {
        code.prepend(base[num % 36]);
        num /= 36;
    }
    return code;
}

QPair<QString, int> MenuWindow::decodeIPPort(const QString& code) {
    if (code.isEmpty()) return qMakePair(QString(), -1);

    quint64 num = 0;
    const QString base = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (QChar c : code) {
        int value = base.indexOf(c);
        if (value == -1) return qMakePair(QString(), -1);
        num = num * 36 + value;
    }

    int port = num & 0xFFFF;
    num >>= 16;
    int d = num & 0xFF;
    int c = (num >> 8) & 0xFF;
    int b = (num >> 16) & 0xFF;
    int a = (num >> 24) & 0xFF;

    if (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255 || port < 0 || port > 65535)
        return qMakePair(QString(), -1);

    QString ip = QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
    return qMakePair(ip, port);
}