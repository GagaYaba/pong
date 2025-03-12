#ifndef CODEDIALOG_H
#define CODEDIALOG_H

#include <QDialog>
#include "../src/ui_codedialog.h"

/**
 * @brief Classe représentant une boîte de dialogue pour afficher du code.
 */
class CodeDialog : public QDialog
{
Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe CodeDialog.
     * @param code Le code à afficher dans la boîte de dialogue.
     * @param parent Le parent QWidget.
     */
    explicit CodeDialog(const QString& code, QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe CodeDialog.
     */
    ~CodeDialog();

private slots:
    /**
     * @brief Slot appelé lorsque le bouton de copie est cliqué.
     */
    void onCopyClicked();

    /**
     * @brief Slot appelé lorsque le bouton OK est cliqué.
     */
    void onOkClicked();

private:
    Ui::CodeDialog *ui; ///< Interface utilisateur de la boîte de dialogue.
};

#endif // CODEDIALOG_H