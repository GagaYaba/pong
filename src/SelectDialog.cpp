#include "../include/SelectDialog.h"
#include <QMessageBox>
#include <QDebug>
#include "../include/globals.h"

SelectDialog::SelectDialog(QWidget *parent, const QStringList &availableSlots, GameClient *client) : QDialog(parent),
                                                                                                     ui(new Ui::SelectDialog),
                                                                                                     m_client(client)
{
    ui->setupUi(this);

    // Mise à jour des slots disponibles selon les données reçues
    if (!availableSlots.contains("p1"))
    {
        ui->player1CheckBox->setChecked(false);
        ui->player1CheckBox->setEnabled(false);
        ui->player1CheckBox->setText("Player 1 (Occupé)");
    }
    if (!availableSlots.contains("p2"))
    {
        ui->player2CheckBox->setChecked(false);
        ui->player2CheckBox->setEnabled(false);
        ui->player2CheckBox->setText("Player 2 (Occupé)");
    }

    // Connexion pour la gestion du toggling par le joueur local avec exclusion mutuelle.
    connect(ui->player1CheckBox, &QCheckBox::toggled, this, [this](bool checked)
            {
        emit roleSelected("p1", true, checked); });
    connect(ui->player2CheckBox, &QCheckBox::toggled, this, [this](bool checked)
            {
        emit roleSelected("p2", true, checked); });

    // Seul l'hôte peut lancer la partie
    connect(ui->startButton, &QPushButton::clicked, this, &SelectDialog::onStartGame);

    // Connexion du signal du client pour confirmer ou libérer un rôle
    if (m_client)
    {
        qDebug() << "Connexion du signal roleEmit à onRoleConfirmed";
        // ATTENTION : Assurez-vous que le signal roleEmit émet bien trois paramètres : role, playerId et join.
        connect(m_client, &GameClient::roleEmit, this, &SelectDialog::onRoleConfirmed);
    }

    updateStartButton();
    // Mise à jour initiale du bouton de démarrage et du label de statut
    ui->statusLabel->setText(g_isHost ? "En attente que tous les joueurs soient prêts..." : "En attente que l'hôte lance la partie.");
}

SelectDialog::~SelectDialog()
{
    delete ui;
}

// Vérifie si le slot pour le joueur donné est activé et prêt
bool SelectDialog::isPlayerReady(const QString &player) const
{
    if (player == "p1")
        return ui->player1CheckBox->isChecked();
    else if (player == "p2")
        return ui->player2CheckBox->isChecked();
    return false;
}

// Renvoie le rôle sélectionné (devrait être unique grâce à l'exclusion mutuelle)
QString SelectDialog::getSelectedRole() const
{
    if (isPlayerReady("p1"))
        return "p1";
    else if (isPlayerReady("p2"))
        return "p2";
    return "";
}

// Mise à jour générique d'un slot.
// 'player' : le nom du slot ("p1", "p2", etc.).
// 'self'   : true si le changement vient du joueur local (donc contrôlable par l'utilisateur).
// 'join'   : true si le joueur prend le slot, false s'il le libère.
void SelectDialog::updateSlot(const QString &player, bool self, bool join)
{
    if (player == "p1")
    {
        {
            QSignalBlocker blocker(ui->player1CheckBox);
            ui->player1CheckBox->setChecked(join);
            if (join)
            {
                ui->player1CheckBox->setText(self ? "Player 1 (Vous)" : "Player 1 (Occupé)");
                if (self)
                {
                    ui->player1CheckBox->setEnabled(false);
                    ui->player2CheckBox->setEnabled(false);
                }
            }
            else
            {
                ui->player1CheckBox->setText("Player 1");
            }
            if (!self)
            {
                ui->player1CheckBox->setEnabled(false);
            }
        }
    }
    else if (player == "p2")
    {
        {
            QSignalBlocker blocker(ui->player2CheckBox);
            ui->player2CheckBox->setChecked(join);
            if (join)
            {
                ui->player2CheckBox->setText(self ? "Player 2 (Vous)" : "Player 2 (Occupé)");
                if (self)
                {
                    ui->player1CheckBox->setEnabled(false);
                    ui->player2CheckBox->setEnabled(false);
                }
            }
            else
            {
                ui->player2CheckBox->setText("Player 2");
            }
            if (!self)
            {
                ui->player2CheckBox->setEnabled(false);
            }
        }
    }
    updateStartButton();
}

// Mise à jour du bouton de démarrage.
// Pour l'hôte, le bouton s'active si tous les slots disponibles (activables) sont pris.
void SelectDialog::updateStartButton()
{
    if (g_isHost)
    {
        bool ready = false;
        if (ui->player1CheckBox->isChecked() && ui->player2CheckBox->isChecked())
        {
            ready = true;
        }

        ui->startButton->setEnabled(ready);
        ui->statusLabel->setText(ready ? "Tous les joueurs sont prêts ! Cliquez pour démarrer." : "En attente que tous les joueurs soient prêts...");
    }
    else
    {
        ui->startButton->setEnabled(false);
    }
}

// Seul l'hôte peut lancer la partie, et uniquement si tous les joueurs disponibles sont prêts.
void SelectDialog::onStartGame()
{
    if (g_isHost)
    {
        bool ready = true;
        if (ui->player1CheckBox->isEnabled() && !ui->player1CheckBox->isChecked())
            ready = false;
        if (ui->player2CheckBox->isEnabled() && !ui->player2CheckBox->isChecked())
            ready = false;

        if (ready)
        {
            emit gameStarted();
            accept();
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Tous les joueurs doivent être prêts pour démarrer le jeu !");
        }
    }
}

// Mise à jour lorsqu'un rôle est confirmé par le serveur.
// Le signal doit transmettre le rôle, l'identifiant du joueur (playerId) et join (prendre ou libérer le slot).
void SelectDialog::onRoleConfirmed(const QString &role, int playerId, bool join)
{
    qDebug() << "Rôle confirmé pour le joueur:" << role << ", playerId:" << playerId << ", join:" << join;
    if (g_playerId == playerId)
    {
        updateSlot(role, true, join);
    }
    else
    {
        updateSlot(role, false, join);
    }
}
