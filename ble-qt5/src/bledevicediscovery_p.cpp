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

#include "bledevicediscovery_p.h"

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "bledevicediscoveryexception.h"

BLEDeviceDiscoveryPrivate::BLEDeviceDiscoveryPrivate(BLEDeviceDiscovery* q)
    : q_ptr(q),
      busy(false),
      error(BLEDeviceDiscovery::NoError),
      scanType(BLEDeviceDiscovery::Active),
      socket(-1),
      worker(Q_NULLPTR)
{
}

BLEDeviceDiscoveryPrivate::~BLEDeviceDiscoveryPrivate()
{
}

void BLEDeviceDiscoveryPrivate::tryUpdateScanParameters()
{
    Q_Q(BLEDeviceDiscovery);

    try
    {
        updateScanParameters();
    }
    catch (const BLEDeviceDiscoveryException& e)
    {
        q->setError(e.error(), e.errorString());
    }
}

void BLEDeviceDiscoveryPrivate::updateScanParameters()
{
    int own_type = 0x00;
    int interval = htobs(0x0010);
    int window = htobs(0x0010);

    int err = hci_le_set_scan_parameters(
                socket,
                static_cast< uint8_t >(scanType),
                interval, //        ?
                window,   //        ?
                own_type, //        ?
                0,        // filter ?
                1000);    // to     ?

    if (err < 0)
        throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::HciLeSetScanParametersError);
}
