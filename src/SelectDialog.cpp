#include "../include/SelectDialog.h"
#include <QMessageBox>

SelectDialog::SelectDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SelectDialog),
        game(nullptr)
{
    ui->setupUi(this);

    // Création des éléments de l'interface
    player1CheckBox = new QCheckBox("Player 1 Ready", this);
    player2CheckBox = new QCheckBox("Player 2 Ready", this);
    startButton = new QPushButton("Start Game", this);
    statusLabel = new QLabel("Waiting for both players to be ready...", this);

    // Positionnement des éléments
    player1CheckBox->setGeometry(30, 40, 200, 30);
    player2CheckBox->setGeometry(30, 80, 200, 30);
    startButton->setGeometry(30, 120, 200, 30);
    statusLabel->setGeometry(30, 160, 300, 30);

    startButton->setEnabled(false);  // Désactive le bouton au départ

    // Connexion des signaux aux slots
    connect(player1CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer1ReadyChanged);
    connect(player2CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer2ReadyChanged);
    connect(startButton, &QPushButton::clicked, this, &SelectDialog::onStartGame);
}

SelectDialog::~SelectDialog()
{
    delete ui;
}

bool SelectDialog::isPlayer1Ready() const
{
    return player1CheckBox->isChecked();
}

bool SelectDialog::isPlayer2Ready() const
{
    return player2CheckBox->isChecked();
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
    // Si les deux joueurs sont prêts, active le bouton
    if (isPlayer1Ready() && isPlayer2Ready()) {
        startButton->setEnabled(true);
        statusLabel->setText("Both players are ready! Click to start the game.");
    } else {
        startButton->setEnabled(false);
        statusLabel->setText("Waiting for both players to be ready...");
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
