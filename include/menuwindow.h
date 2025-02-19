#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>

class MenuWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void onStart();
    void onQuit();

private:
    void createMenu();
};

#endif // MENUWINDOW_H
