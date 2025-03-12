#include "../include/JoinDialog.h"

/**
 * @brief Constructeur de la classe JoinDialog.
 * @param parent Le parent QWidget.
 */
JoinDialog::JoinDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::JoinDialog) {
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::clicked, this, &JoinDialog::reject);
    connect(ui->joinButton, &QPushButton::clicked, this, &JoinDialog::accept);
}

/**
 * @brief Destructeur de la classe JoinDialog.
 */
JoinDialog::~JoinDialog() {
    delete ui;
}

/**
 * @brief Obtient le code saisi dans le champ de texte.
 * @return Le code saisi sous forme de QString.
 */
QString JoinDialog::getCode() const {
    return ui->codeLineEdit->text();
}