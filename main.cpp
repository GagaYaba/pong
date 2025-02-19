#include <QApplication>
#include "Game.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Game game;
    game.show();
    return a.exec();
}
