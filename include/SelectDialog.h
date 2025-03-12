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

/**
 * @brief Classe représentant la boîte de dialogue de sélection.
 */
class SelectDialog : public QDialog
{
Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe SelectDialog.
     * @param parent Le parent QWidget.
     * @param availableSlots La liste des emplacements disponibles.
     * @param client Pointeur vers l'objet GameClient.
     */
    explicit SelectDialog(QWidget *parent, const QStringList &availableSlots, GameClient *client);

    /**
     * @brief Destructeur de la classe SelectDialog.
     */
    ~SelectDialog();

    /**
     * @brief Vérifie si un joueur est prêt.
     * @param player Le nom du joueur.
     * @return true si le joueur est prêt, false sinon.
     */
    bool isPlayerReady(const QString &player) const;

    /**
     * @brief Vérifie si le joueur 1 est prêt.
     * @return true si le joueur 1 est prêt, false sinon.
     */
    bool isPlayer1Ready() const;

    /**
     * @brief Vérifie si le joueur 2 est prêt.
     * @return true si le joueur 2 est prêt, false sinon.
     */
    bool isPlayer2Ready() const;

    /**
     * @brief Obtient le rôle sélectionné.
     * @return Le rôle sélectionné sous forme de QString.
     */
    QString getSelectedRole() const;

    /**
     * @brief Méthode appelée lorsque le rôle est confirmé.
     * @param role Le rôle du joueur.
     * @param playerId L'identifiant du joueur.
     * @param join Indique si le joueur rejoint une partie.
     */
    void onRoleConfirmed(const QString &role, int playerId, bool join);

signals:
    /**
     * @brief Signal émis lorsque le rôle est sélectionné.
     * @param player Le nom du joueur.
     * @param self Indique si c'est le joueur lui-même.
     * @param join Indique si le joueur rejoint une partie.
     */
    void roleSelected(const QString &player, bool self, bool join);

    /**
     * @brief Signal émis lorsque le jeu commence.
     */
    void gameStarted();

private slots:
    /**
     * @brief Slot appelé pour démarrer le jeu.
     */
    void onStartGame();

    /**
     * @brief Met à jour l'état du bouton de démarrage.
     */
    void updateStartButton();

    /**
     * @brief Met à jour l'état d'un emplacement.
     * @param player Le nom du joueur.
     * @param self Indique si c'est le joueur lui-même.
     * @param join Indique si le joueur rejoint une partie.
     */
    void updateSlot(const QString &player, bool self, bool join);

    /**
     * @brief Slot appelé lorsque les informations du jeu sont reçues.
     * @param gameMode Le mode de jeu.
     */
    void onGameInfoReceived(const QString &gameMode);

private:
    Game *game{}; ///< Pointeur vers l'objet Game.
    Ui::SelectDialog *ui; ///< Interface utilisateur de la boîte de dialogue de sélection.
    QPushButton *startButton{}; ///< Bouton de démarrage.
    QCheckBox *player1CheckBox{}; ///< Case à cocher pour le joueur 1.
    QCheckBox *player2CheckBox{}; ///< Case à cocher pour le joueur 2.
    QLabel *statusLabel{}; ///< Étiquette de statut.
    QStringList slotsDisponibles; ///< Liste des emplacements disponibles.
    GameClient *m_client; ///< Pointeur vers l'objet GameClient.
};

#endif // SELECTDIALOG_H