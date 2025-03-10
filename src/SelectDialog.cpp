#include "../include/SelectDialog.h"
#include <QMessageBox>

SelectDialog::SelectDialog(QWidget *parent, const QStringList &availableSlots, GameClient *client) :
        QDialog(parent),
        ui(new Ui::SelectDialog),
        m_client(client)
{
    ui->setupUi(this);

    // Désactiver les checkboxes non disponibles
    if (!availableSlots.contains("p1")) {
        ui->player1CheckBox->setDisabled(true);
        ui->player1CheckBox->setText("Player 1 (Non disponible)");
    }
    if (!availableSlots.contains("p2")) {
        ui->player2CheckBox->setDisabled(true);
        ui->player2CheckBox->setText("Player 2 (Non disponible)");
    }

    connect(ui->player1CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer1ReadyChanged);
    connect(ui->player2CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer2ReadyChanged);
    connect(ui->startButton, &QPushButton::clicked, this, &SelectDialog::onStartGame);
    if (m_client) {
        qDebug() << "Connexion du signal roleEmit à onRoleConfirmed";
        connect(m_client, &GameClient::roleEmit, this, &SelectDialog::onRoleConfirmed);
    }
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
    emit roleSelected("P1", checked);
}

void SelectDialog::onPlayer2ReadyChanged(bool checked)
{
    updateStartButton();
    emit roleSelected("P2", checked);
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

void SelectDialog::onRoleConfirmed(const QString &role) {
    qDebug() << "Rôle confirmé pour le joueur:" << role;
    if(role == "p1") {
        ui->player1CheckBox->setChecked(true);
    } else if (role == "p2"){
        ui->player2CheckBox->setChecked(true);
    }
}