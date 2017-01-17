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

#ifndef BLEDEVICEDISCOVERY_P_H
#define BLEDEVICEDISCOVERY_P_H

#include <ble-qt5/bledevicediscovery.h>

#include "bledevicediscoveryworker.h"

class BLEDeviceDiscoveryPrivate
{
    Q_DECLARE_PUBLIC(BLEDeviceDiscovery)
    BLEDeviceDiscovery* const q_ptr;

public:
    explicit BLEDeviceDiscoveryPrivate(BLEDeviceDiscovery* q);
    ~BLEDeviceDiscoveryPrivate();

    void updateScanParameters();
    void tryUpdateScanParameters();

    void tryStartWorker();

public:
    bool busy;
    BLEDeviceDiscovery::Error error;
    QString errorString;
    BLEDeviceDiscovery::ScanType scanType;

    int socket;
    BLEDeviceDiscoveryWorker* worker;
};

#endif
