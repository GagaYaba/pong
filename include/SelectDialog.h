#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include "Game.h"
#include "menuwindow.h"

#include "config.h"
#if defined(USE_CMAKE)
    #include "../src/ui_SelectDialog.h"
#elif defined(USE_QMAKE)
    #include "./ui_SelectDialog.h"
#else
    #error "SelectDialog.h: Aucun outil de build n'a été défini"
#endif

namespace Ui {
    class SelectDialog;
}

class SelectDialog : public QDialog
{
Q_OBJECT

public:
    explicit SelectDialog(QWidget *parent = nullptr);
    ~SelectDialog();

    bool isPlayer1Ready() const;
    bool isPlayer2Ready() const;
    QString getSelectedRole() const;

signals:
    void gameStarted(QString role);

private slots:
    void onPlayer1ReadyChanged(bool checked);
    void onPlayer2ReadyChanged(bool checked);
    void onStartGame();

private:
    void updateStartButton();

    Game *game;  // Variable pour gérer l'instance du jeu
    Ui::SelectDialog *ui;
    QPushButton *startButton;
    QCheckBox *player1CheckBox;
    QCheckBox *player2CheckBox;
    QLabel *statusLabel;
};

#endif // SELECTDIALOG_H
