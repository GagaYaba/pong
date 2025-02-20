#ifndef CODEDIALOG_H
#define CODEDIALOG_H

#include <QDialog>

#include "config.h"
#if defined(USE_CMAKE)
    #include "../src/ui_codedialog.h"
#elif defined(USE_QMAKE)
    #include "./ui_codedialog.h"
#else
    #error "CodeDialog.h: Aucun outil de build n'a été défini"
#endif

class CodeDialog : public QDialog
{
Q_OBJECT

public:
    explicit CodeDialog(const QString& code, QWidget *parent = nullptr);
    ~CodeDialog();

private slots:
    void onCopyClicked();
    void onOkClicked();

private:
    Ui::CodeDialog *ui;
};

#endif // CODEDIALOG_H
