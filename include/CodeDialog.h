#ifndef CODEDIALOG_H
#define CODEDIALOG_H

#include <QDialog>
#include "./ui_codedialog.h"

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
