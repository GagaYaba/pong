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
        ui->player1CheckBox->setChecked(false); // Player 1 n'est pas disponible, donc ne pas cocher
        ui->player1CheckBox->setEnabled(false);
        ui->player1CheckBox->setText("Player 1 (Non disponible)");
    }
    if (!availableSlots.contains("p2")) {
        ui->player2CheckBox->setChecked(false); // Player 2 n'est pas disponible, donc ne pas cocher
        ui->player2CheckBox->setEnabled(false);
        ui->player2CheckBox->setText("Player 2 (Non disponible)");
    }

    // Connecter les signaux de changement de disponibilité des joueurs
    connect(ui->player1CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer1ReadyChanged);
    connect(ui->player2CheckBox, &QCheckBox::toggled, this, &SelectDialog::onPlayer2ReadyChanged);
    connect(ui->startButton, &QPushButton::clicked, this, &SelectDialog::onStartGame);

    // Vérifier la connexion au client
    if (m_client) {
        qDebug() << "Connexion du signal roleEmit à onRoleConfirmed";
        connect(m_client, &GameClient::roleEmit, this, &SelectDialog::onRoleConfirmed);
        connect(m_client, &GameClient::gameStartedEmit, this, &SelectDialog::onGameStarted);
    }
    ui->startButton->setEnabled(false); // Le bouton démarrer est désactivé tant que les deux joueurs ne sont pas prêts
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
    emit roleSelected("p1", checked);
    updateStartButton();
}

void SelectDialog::onPlayer2ReadyChanged(bool checked)
{
    emit roleSelected("p2", checked);
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

        // Signaler au serveur que le jeu peut commencer avec le rôle sélectionné
        emit gameStarted(role);

        accept(); // Fermer la fenêtre
    } else {
        QMessageBox::warning(this, "Erreur", "Both players must be ready before starting the game!");
    }
}

void SelectDialog::onRoleConfirmed(const QString &role)
{
    qDebug() << "Rôle confirmé pour le joueur:" << role;

    // Mettre à jour l'état des cases à cocher selon le rôle confirmé
    if (role == "p1") {
        ui->player1CheckBox->setChecked(true);
    } else if (role == "p2") {
        ui->player2CheckBox->setChecked(true);
    }
}

void SelectDialog::onGameStarted()
{
    // Action à faire quand le serveur confirme que le jeu a démarré
    qDebug() << "Le jeu a commencé!";
    accept();  // Fermer la fenêtre si le jeu commence
}
