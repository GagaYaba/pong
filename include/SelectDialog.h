#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>
#include "Game.h"
#include "menuwindow.h"
#include "../src/ui_SelectDialog.h"

namespace Ui {
    class SelectDialog;
}

class SelectDialog : public QDialog
{
Q_OBJECT

public:
    explicit SelectDialog(QWidget *parent, const QStringList &availableSlots);
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
    void updateStartButton();

private:
    Game *game{};  // Variable pour gérer l'instance du jeu
    Ui::SelectDialog *ui;
    QPushButton *startButton{};
    QCheckBox *player1CheckBox{};
    QCheckBox *player2CheckBox{};
    QLabel *statusLabel{};
    QStringList slotsDisponibles;
};

#endif // SELECTDIALOG_H
