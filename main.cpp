#include <QApplication>
#include "include/menuwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MenuWindow window;
    window.setWindowTitle("Menu Principal");
    window.resize(400, 300);
    window.show();

    return app.exec();
}
