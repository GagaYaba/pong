#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>
#include "config.h"
#include "./ui_JoinDialog.h"

namespace Ui {
    class JoinDialog;
}

class JoinDialog : public QDialog {
Q_OBJECT

public:
    explicit JoinDialog(QWidget *parent = nullptr);
    ~JoinDialog();

    QString getCode() const;  // Récupérer le code entré

private:
    Ui::JoinDialog *ui;
};

#endif // JOINDIALOG_H
