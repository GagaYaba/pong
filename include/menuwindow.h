#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include "../src/ui_menuwindow.h"

#include "Game.h"
#include "../server/GameServer/gameserver.h"
#include "../client/GameClient/gameclient.h"

class MenuWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void onStart();
    void onQuit();
    void onGameClosed();  // Slot pour afficher le menu après la fermeture du jeu

private:
    void createMenu();
    Ui::MenuWindow *ui;  // Utilisation de la classe générée par Qt

    Game *game;
    GameServer *server;
    GameClient *client;
};

#endif // MENUWINDOW_H
