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

#ifndef BLEDEVICEDISCOVERY_H
#define BLEDEVICEDISCOVERY_H

#include <QObject>

#include "config.h"

class BLEDeviceDiscoveryPrivate;

class BLE_QT5_SOSPEC BLEDeviceDiscovery : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(BLEDeviceDiscovery)
    Q_DECLARE_PRIVATE(BLEDeviceDiscovery)

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(Error error READ error NOTIFY errorChanged)
    Q_PROPERTY(ScanType scanType READ scanType WRITE setScanType NOTIFY scanTypeChanged)

public:
    enum Error
    {
        NoError,
        HciGetRouteError,
        HciOpenDevError,
        HciLeSetScanParametersError,
        HciLeSetScanEnableError,
        HciLeSetScanDisableError,
        GetSockOptError,
        SetSockOptError
    };

    enum ScanType
    {
        Passive,
        Active
    };

public:
    BLEDeviceDiscovery(QObject* parent = Q_NULLPTR);
    virtual ~BLEDeviceDiscovery();

    bool busy() const;

    Error error() const;
    QString errorString() const;

    ScanType scanType() const;
    void setScanType(ScanType scanType);

public slots:
    virtual void start();
    virtual void stop();

protected:
    BLEDeviceDiscovery(BLEDeviceDiscoveryPrivate* d, QObject* parent = Q_NULLPTR);

    void setBusy(bool busy);

protected slots:
    void setError(BLEDeviceDiscovery::Error error, QString errorString = QString());

signals:
    void busyChanged(bool);
    void deviceDiscovered(QString address, QString name);
    void errorChanged(BLEDeviceDiscovery::Error, QString);
    void scanTypeChanged(ScanType);

private:
    BLEDeviceDiscoveryPrivate* const d_ptr;
};

QDebug operator<<(QDebug dbg, BLEDeviceDiscovery::Error error);

#endif
