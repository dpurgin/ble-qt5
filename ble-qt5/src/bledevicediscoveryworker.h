/*
 * Qt5 wrapper for Bluetooth LE using HCI and GATT
 * Copyright (c) 2016 Dmitriy Purgin <dpurgin@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef BLEDEVICEDISCOVERYWORKER_H
#define BLEDEVICEDISCOVERYWORKER_H

#include <QThread>
#include <QMutex>

#include <ble-qt5/bledevicediscovery.h>

class BLEDeviceDiscoveryWorker : public QThread
{
    Q_OBJECT

public:
    BLEDeviceDiscoveryWorker(int socket);
    ~BLEDeviceDiscoveryWorker();

    void run() Q_DECL_OVERRIDE;

    void setActive(bool active);
    bool active();

signals:
    void error(BLEDeviceDiscovery::Error error, QString errorString);

    void discovered(QString address, QString name);

private:
    int m_socket;

    bool m_active;
    QMutex m_activeMutex;
};

#endif // BLEDEVICEDISCOVERYWORKER_H
