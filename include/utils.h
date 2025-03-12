#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QNetworkInterface>
#include <QHostAddress>

/**
 * @brief Obtient l'adresse IP locale de la machine.
 * @return L'adresse IP locale sous forme de QString.
 */
QString getLocalIPAddress();

#endif // UTILS_H