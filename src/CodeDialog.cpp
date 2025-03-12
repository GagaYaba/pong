#include "../include/codedialog.h"
#include <QClipboard>
#include <QApplication>

/**
 * @brief Constructeur de la classe CodeDialog.
 * @param code Le code à afficher dans la boîte de dialogue.
 * @param parent Le parent QWidget.
 */
CodeDialog::CodeDialog(const QString& code, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::CodeDialog)
{
    ui->setupUi(this);

    ui->codeLineEdit->setText(code);
    ui->codeLineEdit->setReadOnly(true);

    connect(ui->copyButton, &QPushButton::clicked, this, &CodeDialog::onCopyClicked);

    connect(ui->okButton, &QPushButton::clicked, this, &CodeDialog::onOkClicked);
}

/**
 * @brief Destructeur de la classe CodeDialog.
 */
CodeDialog::~CodeDialog()
{
    delete ui;
}

/**
 * @brief Slot appelé lorsque le bouton de copie est cliqué.
 * Copie le texte du champ de code dans le presse-papiers.
 */
void CodeDialog::onCopyClicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->codeLineEdit->text());
}

/**
 * @brief Slot appelé lorsque le bouton OK est cliqué.
 * Ferme la boîte de dialogue.
 */
void CodeDialog::onOkClicked()
{
    accept();
}