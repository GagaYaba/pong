#include "../include/menuwindow.h"
#include "../include/JoinDialog.h"
#include "../include/CodeDialog.h"
#include "../include/utils.h"
#include "../include/CodeDialog.h"
#include "../include/SelectDialog.h"
#include "../include/globals.h"
#include <QMenu>
#include <QAction>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>

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
void MenuWindow::onAvailableSlotsReceived(const QStringList &availableSlots) {
    qDebug() << "Slots reçus:" << availableSlots;

    if (selectDialog) {
        delete selectDialog;
    }

    selectDialog = new SelectDialog(this, availableSlots, client);
    connect(selectDialog, &SelectDialog::roleSelected, this, &MenuWindow::onRoleSelected);
    connect(selectDialog, &SelectDialog::gameStarted, this, &MenuWindow::onGameStarted);

    selectDialog->show();
}


void MenuWindow::onStart() {
    if (!server) {
        server = new GameServer(this);
        server->startServer(1, false);

        QString ip = getLocalIPAddress();
        QString joinCode = generateJoinCode(ip);

        g_isHost = true;

        CodeDialog *codeDialog = new CodeDialog(joinCode, this);
        if (codeDialog->exec() == QDialog::Accepted) {
            delete codeDialog;
        }
    }

    if (!g_client) {
        g_client = new GameClient(this);
        g_client ->connectToServer(QHostAddress::LocalHost);
    }

    if (g_client) {
        qDebug() << "Connexion du signal availableSlotsReceived à onAvailableSlotsReceived";
        connect(g_client, &GameClient::availableSlotsReceived, this, &MenuWindow::onAvailableSlotsReceived);
    } else {
        qDebug() << "g_client est NULL, impossible de connecter le signal.";
    }
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

        QString ip = decodeJoinCode(code);
        g_isHost = false;

        if (ip.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le code est invalide !");
            return;
        }

        if (!g_client) {
            g_client = new GameClient(this);
            g_client->connectToServer(QHostAddress(ip));
        }

        if (g_client) {
            qDebug() << "Connexion du signal availableSlotsReceived à onAvailableSlotsReceived";
            connect(g_client, &GameClient::availableSlotsReceived, this, &MenuWindow::onAvailableSlotsReceived);
            } else {
            qDebug() << "g_client est NULL, impossible de connecter le signal.";
        }

    }
}
QString MenuWindow::generateJoinCode(const QString &ip) {
    int a, b, c, d;
    sscanf(ip.toUtf8().constData(), "%d.%d.%d.%d", &a, &b, &c, &d);

    quint32 num = ((quint32)a << 24) | ((quint32)b << 16) | ((quint32)c << 8) | d;
    
    const QString base = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString code;
    do {
        code.prepend(base[num % 36]);
        num /= 36;
    } while (num);

    return code;
}

QString MenuWindow::decodeJoinCode(const QString &code) {
    quint32 num = 0;
    const QString base = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (QChar c : code) {
        int value = base.indexOf(c);
        if (value == -1) return QString();
        num = num * 36 + value;
    }

    int d = num & 0xFF;
    int c = (num >> 8) & 0xFF;
    int b = (num >> 16) & 0xFF;
    int a = (num >> 24) & 0xFF;

    return QString("%1.%2.%3.%4").arg(a).arg(b).arg(c).arg(d);
}

void MenuWindow::onRoleSelected(const QString &player, bool selected) {
    if (g_client) {
        g_client->selectRole(player);
        qDebug() << "Rôle sélectionné:" << player;
    }
}

void MenuWindow::onGameStarted() {
    if (g_client) {
        g_client->startGame();
        qDebug() << "Jeu démarré!";
    }
}
