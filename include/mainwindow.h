#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief Classe représentant la fenêtre principale de l'application.
 */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe MainWindow.
     * @param parent Le parent QWidget.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructeur de la classe MainWindow.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui; ///< Interface utilisateur de la fenêtre principale.
};

#endif // MAINWINDOW_H