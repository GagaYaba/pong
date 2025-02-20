#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>

#include "config.h"
#if defined(USE_QMAKE)
    #include "./ui_JoinDialog.h"
#else
    #include "../src/ui_JoinDialog.h"
#endif

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
