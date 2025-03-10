#include <QApplication>
#include <QInputDialog>
#include "./include/game.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    bool ok;
    QStringList modes;
    modes << "1v1" << "2v2";
    QString selectedMode = QInputDialog::getItem(nullptr, "Choisir le mode de jeu",
                                                 "Mode de jeu", modes, 0, false, &ok);

    Game::GameMode mode = Game::OneVOne;
    if (ok) {
        if (selectedMode == "1v1") {
            mode = Game::OneVOne;
        } else if (selectedMode == "2v2") {
            mode = Game::TwoVTwo;
        }
    }

    Game game(nullptr, mode);
    game.show();

    return a.exec();
}

