#include "../include/codedialog.h"
#include <QClipboard>
#include <QApplication>

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

CodeDialog::~CodeDialog()
{
    delete ui;
}

void CodeDialog::onCopyClicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->codeLineEdit->text());
}

void CodeDialog::onOkClicked()
{
    accept();
}
