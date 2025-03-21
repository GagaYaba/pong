#include "../include/SelectDialog.h"
#include <QMessageBox>
#include <QDebug>
#include "../include/globals.h"
#include "../include/game.h"

/**
 * @brief Constructeur de la classe SelectDialog.
 * @param parent Le parent QWidget.
 * @param availableSlots Liste des emplacements disponibles.
 * @param client Pointeur vers l'objet GameClient.
 */
SelectDialog::SelectDialog(QWidget *parent, const QStringList &availableSlots, GameClient *client) : QDialog(parent),
                                                                                                     ui(new Ui::SelectDialog),
                                                                                                     m_client(client)
{
    ui->setupUi(this);

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

    connect(ui->player1CheckBox, &QCheckBox::toggled, this, [this](bool checked)
    {
        emit roleSelected("p1", true, checked); });
    connect(ui->player2CheckBox, &QCheckBox::toggled, this, [this](bool checked)
    {
        emit roleSelected("p2", true, checked); });

    connect(ui->startButton, &QPushButton::clicked, this, &SelectDialog::onStartGame);
    connect(g_client, &GameClient::gameInfoReceived, this, &SelectDialog::onGameInfoReceived);
    if (g_client)
    {
        qDebug() << "Connexion du signal roleEmit à onRoleConfirmed";
        connect(g_client, &GameClient::roleEmit, this, &SelectDialog::onRoleConfirmed);
    }

    updateStartButton();
    ui->statusLabel->setText(g_isHost ? "En attente que tous les joueurs soient prêts..." : "En attente que l'hôte lance la partie.");
}

/**
 * @brief Destructeur de la classe SelectDialog.
 */
SelectDialog::~SelectDialog()
{
    delete ui;
}

/**
 * @brief Vérifie si un joueur est prêt.
 * @param player Le nom du joueur.
 * @return true si le joueur est prêt, false sinon.
 */
bool SelectDialog::isPlayerReady(const QString &player) const
{
    if (player == "p1")
        return ui->player1CheckBox->isChecked();
    else if (player == "p2")
        return ui->player2CheckBox->isChecked();
    return false;
}

/**
 * @brief Obtient le rôle sélectionné.
 * @return Le rôle sélectionné.
 */
QString SelectDialog::getSelectedRole() const
{
    if (isPlayerReady("p1"))
        return "p1";
    else if (isPlayerReady("p2"))
        return "p2";
    return "";
}

/**
 * @brief Met à jour l'état d'un emplacement.
 * @param player Le nom du joueur.
 * @param self Indique si c'est le joueur lui-même.
 * @param join Indique si le joueur a rejoint.
 */
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

/**
 * @brief Met à jour l'état du bouton de démarrage.
 */
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

/**
 * @brief Slot appelé lorsque le bouton de démarrage est cliqué.
 */
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

/**
 * @brief Slot appelé lorsque le rôle d'un joueur est confirmé.
 * @param role Le rôle du joueur.
 * @param playerId L'identifiant du joueur.
 * @param join Indique si le joueur a rejoint.
 */
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

/**
 * @brief Slot appelé lorsque les informations de jeu sont reçues.
 * @param gameMode Le mode de jeu.
 */
void SelectDialog::onGameInfoReceived(const QString &gameMode) {
    g_gameMode = gameMode;
    qDebug() << "Mode de jeu reçu:" << gameMode;
    accept();
}