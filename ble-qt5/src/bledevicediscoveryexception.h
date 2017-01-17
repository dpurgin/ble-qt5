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

#ifndef BLEDEVICEDISCOVERYEXCEPTION_H
#define BLEDEVICEDISCOVERYEXCEPTION_H

#include <QString>

#include <cerrno>
#include <cstring>
#include <exception>

#include <ble-qt5/bledevicediscovery.h>

class BLEDeviceDiscoveryException : public std::exception
{
public:
    explicit BLEDeviceDiscoveryException(BLEDeviceDiscovery::Error error)
        : m_error(error)
    {
        m_errorString = QString::fromUtf8(std::strerror(errno));
    }

    BLEDeviceDiscoveryException(BLEDeviceDiscovery::Error error, QString errorString)
        : m_error(error),
          m_errorString(errorString)
    {
    }

    virtual ~BLEDeviceDiscoveryException() throw()
    {
    }

    BLEDeviceDiscovery::Error error() const { return m_error; }
    QString errorString() const { return m_errorString; }

private:
    BLEDeviceDiscovery::Error m_error;
    QString m_errorString;
};

#endif
