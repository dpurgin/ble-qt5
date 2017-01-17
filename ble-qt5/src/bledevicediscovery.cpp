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

#include <ble-qt5/bledevicediscovery.h>

#include <QDebug>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "bledevicediscovery_p.h"
#include "bledevicediscoveryexception.h"
#include "bledevicediscoveryworker.h"


BLEDeviceDiscovery::BLEDeviceDiscovery(QObject* parent)
    : QObject(parent),
      d_ptr(new BLEDeviceDiscoveryPrivate(this))
{
    qDebug();
}

BLEDeviceDiscovery::BLEDeviceDiscovery(BLEDeviceDiscoveryPrivate* d, QObject* parent)
    : QObject(parent),
      d_ptr(d)
{
}

BLEDeviceDiscovery::~BLEDeviceDiscovery()
{
    qDebug();

    Q_D(BLEDeviceDiscovery);

    if (d)
    {
        if (busy())
            stop();

        delete d;
    }
}

bool BLEDeviceDiscovery::busy() const
{
    Q_D(const BLEDeviceDiscovery);

    return d->busy;
}

BLEDeviceDiscovery::Error BLEDeviceDiscovery::error() const
{
    Q_D(const BLEDeviceDiscovery);

    return d->error;
}

QString BLEDeviceDiscovery::errorString() const
{
    return QString();
}


BLEDeviceDiscovery::ScanType BLEDeviceDiscovery::scanType() const
{
    Q_D(const BLEDeviceDiscovery);

    return d->scanType;
}

void BLEDeviceDiscovery::setBusy(bool busy)
{
    Q_D(BLEDeviceDiscovery);

    if (d->busy != busy)
        emit busyChanged(d->busy = busy);
}

void BLEDeviceDiscovery::setError(BLEDeviceDiscovery::Error error, QString errorString)
{
    Q_D(BLEDeviceDiscovery);

    if (d->error != error)
    {
        emit errorChanged(d->error = error,
                          d->errorString = errorString);

        if (busy())
            stop();
    }
}

void BLEDeviceDiscovery::setScanType(ScanType scanType)
{
    Q_D(BLEDeviceDiscovery);

    if (d->scanType != scanType)
    {        
        emit scanTypeChanged(d->scanType = scanType);

        if (d->socket >= 0)
            d->tryUpdateScanParameters();
    }
}

void BLEDeviceDiscovery::start()
{
    if (error() != NoError || busy())
        return;

    Q_D(BLEDeviceDiscovery);

    Q_ASSERT(d->worker == Q_NULLPTR);
    Q_ASSERT(d->socket < 0);

    try
    {
        int deviceId = hci_get_route(Q_NULLPTR);

        if (deviceId < 0)
            throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::HciGetRouteError);

        d->socket = hci_open_dev(deviceId);

        if (d->socket < 0)
            throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::HciOpenDevError);

        d->updateScanParameters();

        d->worker = new BLEDeviceDiscoveryWorker(d->socket);

        connect(d->worker, SIGNAL(discovered(QString, QString)),
                this, SIGNAL(deviceDiscovered(QString, QString)));
        connect(d->worker, SIGNAL(error(BLEDeviceDiscovery::Error, QString)),
                this, SLOT(setError(BLEDeviceDiscovery::Error, QString)));

        d->worker->setActive(true);
        d->worker->start();

        setBusy(true);
    }
    catch(const BLEDeviceDiscoveryException& e)
    {
        setError(e.error(), e.errorString());
    }
}

void BLEDeviceDiscovery::stop()
{
    if (!busy())
        return;

    Q_D(BLEDeviceDiscovery);

    Q_ASSERT(d->worker != Q_NULLPTR);
    Q_ASSERT(d->socket >= 0);

    d->worker->setActive(false);
    d->worker->wait(5000);

    delete d->worker;
    d->worker = Q_NULLPTR;

    if (d->socket >= 0)
    {
        hci_close_dev(d->socket);
        d->socket = -1;
    }

    setBusy(false);
}

QDebug operator<<(QDebug dbg, BLEDeviceDiscovery::Error error)
{
    switch (error)
    {
        case BLEDeviceDiscovery::NoError: dbg << "NoError"; break;
        case BLEDeviceDiscovery::HciGetRouteError: dbg << "HciGetRouteError"; break;
        case BLEDeviceDiscovery::HciOpenDevError: dbg << "HciOpenDevError"; break;
        case BLEDeviceDiscovery::HciLeSetScanParametersError: dbg << "HciLeSetScanParametersError"; break;
        case BLEDeviceDiscovery::HciLeSetScanEnableError: dbg << "HciLeSetScanEnableError"; break;
        case BLEDeviceDiscovery::HciLeSetScanDisableError: dbg << "HciLeSetScanDisableError"; break;
        case BLEDeviceDiscovery::GetSockOptError: dbg << "GetSockOptError"; break;
        case BLEDeviceDiscovery::SetSockOptError: dbg << "SetSockOptError"; break;
        default: dbg << static_cast< int >(error);
    }

    return dbg;
}

