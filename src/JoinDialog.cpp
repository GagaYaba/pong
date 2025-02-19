#include "../include/JoinDialog.h"

JoinDialog::JoinDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::JoinDialog) {
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::clicked, this, &JoinDialog::reject);
    connect(ui->joinButton, &QPushButton::clicked, this, &JoinDialog::accept);
}

JoinDialog::~JoinDialog() {
    delete ui;
}

QString JoinDialog::getCode() const {
    return ui->codeLineEdit->text();
}
