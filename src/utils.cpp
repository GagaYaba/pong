#include "../include/utils.h"
#include <QNetworkInterface>
#include <QHostAddress>

/**
 * @brief Obtient l'adresse IP locale de l'ordinateur.
 *
 * Cette fonction parcourt toutes les interfaces réseau disponibles et retourne l'adresse IP
 * de la première interface Wi-Fi ou sans fil trouvée. Si aucune interface Wi-Fi ou sans fil
 * n'est trouvée, elle retourne l'adresse IP de la première interface réseau disponible.
 * Si aucune adresse IP n'est trouvée, elle retourne "127.0.0.1".
 *
 * @return QString L'adresse IP locale sous forme de chaîne de caractères.
 */
QString getLocalIPAddress() {
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    QString fallbackIP;

    for (const QNetworkInterface &interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol && !entry.ip().isNull()) {
                    QString ifaceName = interface.humanReadableName();

                    if (ifaceName.contains("Wi-Fi", Qt::CaseInsensitive) ||
                        ifaceName.contains("Wireless", Qt::CaseInsensitive))
                    {
                        return entry.ip().toString();
                    } else {
                        if (fallbackIP.isEmpty())
                            fallbackIP = entry.ip().toString();
                    }
                }
            }
        }
    }

    return (!fallbackIP.isEmpty() ? fallbackIP : QString("127.0.0.1"));
}