#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHostAddress>
#include <QMessageBox>
#include <QPair>
#include "../server/GameServer/gameserver.h"
#include "../client/GameClient/gameclient.h"
#include "SelectDialog.h"
#include "./ui_menuwindow.h"

namespace Ui {
    class MenuWindow;
}
class SelectDialog;
class MenuWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();
    void onRoleConfirmed(const QString &role, int playerId, bool join);

private slots:
    void onStart();
    void onQuit();
    void onJoin();
    void onGameClosed();
    void onRoleSelected(const QString &role, bool selected);
    void onGameStarted();
    void onAvailableSlotsReceived(const QStringList &availableSlots);


private:
    QString generateJoinCode(const QString &ip);
    QString decodeJoinCode(const QString& code);
    SelectDialog *selectDialog = nullptr;
    Game *game;
    GameServer *server;
    GameClient *client;
    Ui::MenuWindow *ui;

};

#endif // MENUWINDOW_H
