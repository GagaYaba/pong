#include "../include/SelectDialog.h"
#include <QMessageBox>

SelectDialog::SelectDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SelectDialog),
        game(nullptr)
{
    ui->setupUi(this);

    // Connexion des signaux aux slots
    connect(ui->player1CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer1ReadyChanged);
    connect(ui->player2CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer2ReadyChanged);
    connect(ui->startButton, &QPushButton::clicked, this, &SelectDialog::onStartGame);

    // Assurer que le bouton "Start" est désactivé au début
    ui->startButton->setEnabled(false);
}

SelectDialog::~SelectDialog()
{
    delete ui;
}

bool SelectDialog::isPlayer1Ready() const
{
    return ui->player1CheckBox->isChecked();
}

bool SelectDialog::isPlayer2Ready() const
{
    return ui->player2CheckBox->isChecked();
}

QString SelectDialog::getSelectedRole() const
{
    if (isPlayer1Ready()) {
        return "p1";
    } else if (isPlayer2Ready()) {
        return "p2";
    } else {
        return "";
    }
}

void SelectDialog::onPlayer1ReadyChanged(bool checked)
{
    updateStartButton();
}

void SelectDialog::onPlayer2ReadyChanged(bool checked)
{
    updateStartButton();
}

void SelectDialog::updateStartButton()
{
    if (isPlayer1Ready() && isPlayer2Ready()) {
        ui->startButton->setEnabled(true);
        ui->statusLabel->setText("Both players are ready! Click to start the game.");
    } else {
        ui->startButton->setEnabled(false);
        ui->statusLabel->setText("Waiting for both players to be ready...");
    }
}

void SelectDialog::onStartGame()
{
    if (isPlayer1Ready() && isPlayer2Ready()) {
        QString role = getSelectedRole();

        emit gameStarted(role);

        accept();
    } else {
        QMessageBox::warning(this, "Erreur", "Both players must be ready before starting the game!");
    }
}
