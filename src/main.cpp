#include <QCoreApplication>
#include <QDebug>
#include <QTimer>

#include <ble-qt5/bledevicediscovery.h>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    BLEDeviceDiscovery deviceDiscovery;

    QObject::connect(&deviceDiscovery, &BLEDeviceDiscovery::deviceDiscovered,
        [](QString address, QString name)
        {
            qDebug() << "device discovered:" << address << name;
        }
    );

    QObject::connect(&deviceDiscovery, &BLEDeviceDiscovery::errorChanged,
        [](BLEDeviceDiscovery::Error error, QString errorString)
        {
            qDebug() << "discovery error:" << error << errorString;
        }
    );

    QObject::connect(&deviceDiscovery, &BLEDeviceDiscovery::busyChanged,
        [](bool busy)
        {
            qDebug() << "busy: " << busy;
        }
    );

    deviceDiscovery.start();

    QTimer timer;
    timer.setInterval(10000);

    QObject::connect(&timer, &QTimer::timeout,
                     &deviceDiscovery, &BLEDeviceDiscovery::stop);

    timer.start();

    return app.exec();
}
