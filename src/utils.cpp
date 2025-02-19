#include "../include/utils.h"
#include <QNetworkInterface>
#include <QHostAddress>

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
