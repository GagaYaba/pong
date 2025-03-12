#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHostAddress>
#include <QMessageBox>
#include <QPair>
#include "../server/GameServer/gameserver.h"
#include "../client/GameClient/gameclient.h"
#include "SelectDialog.h"
#include "../src/ui_menuwindow.h"

namespace Ui {
    class MenuWindow;
}
class SelectDialog;

/**
 * @brief Classe représentant la fenêtre de menu de l'application.
 */
class MenuWindow : public QMainWindow
{
Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe MenuWindow.
     * @param parent Le parent QWidget.
     */
    explicit MenuWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe MenuWindow.
     */
    ~MenuWindow();

    /**
     * @brief Méthode appelée lorsque le rôle est confirmé.
     * @param role Le rôle du joueur.
     * @param playerId L'identifiant du joueur.
     * @param join Indique si le joueur rejoint une partie.
     */
    void onRoleConfirmed(const QString &role, int playerId, bool join);

private slots:
    /**
     * @brief Slot appelé pour démarrer le jeu.
     */
    void onStart();

    /**
     * @brief Slot appelé pour quitter le jeu.
     */
    void onQuit();

    /**
     * @brief Slot appelé pour rejoindre une partie.
     */
    void onJoin();

    /**
     * @brief Slot appelé lorsque le jeu est fermé.
     */
    void onGameClosed();

    /**
     * @brief Slot appelé lorsque le rôle est sélectionné.
     * @param role Le rôle sélectionné.
     * @param selected Indique si le rôle est sélectionné.
     */
    void onRoleSelected(const QString &role, bool selected);

    /**
     * @brief Slot appelé lorsque le jeu est démarré.
     */
    void onGameStarted();

    /**
     * @brief Slot appelé lorsque les emplacements disponibles sont reçus.
     * @param availableSlots La liste des emplacements disponibles.
     */
    void onAvailableSlotsReceived(const QStringList &availableSlots);

private:
    /**
     * @brief Génère un code de jonction à partir d'une adresse IP.
     * @param ip L'adresse IP.
     * @return Le code de jonction sous forme de QString.
     */
    QString generateJoinCode(const QString &ip);

    /**
     * @brief Décode un code de jonction.
     * @param code Le code de jonction.
     * @return L'adresse IP sous forme de QString.
     */
    QString decodeJoinCode(const QString& code);

    SelectDialog *selectDialog = nullptr; ///< Boîte de dialogue de sélection.
    Game *game; ///< Pointeur vers l'objet Game.
    GameServer *server; ///< Pointeur vers l'objet GameServer.
    GameClient *client; ///< Pointeur vers l'objet GameClient.
    Ui::MenuWindow *ui; ///< Interface utilisateur de la fenêtre de menu.
};

#endif // MENUWINDOW_H