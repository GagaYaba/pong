#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>
#include "../src/ui_JoinDialog.h"

namespace Ui {
    class JoinDialog;
}

/**
 * @brief Classe représentant une boîte de dialogue pour rejoindre un jeu.
 */
class JoinDialog : public QDialog {
Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe JoinDialog.
     * @param parent Le parent QWidget.
     */
    explicit JoinDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe JoinDialog.
     */
    ~JoinDialog();

    /**
     * @brief Obtient le code de la boîte de dialogue.
     * @return Le code sous forme de QString.
     */
    QString getCode() const;

private:
    Ui::JoinDialog *ui; ///< Interface utilisateur de la boîte de dialogue.
};

#endif