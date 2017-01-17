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

#include "bledevicediscoveryworker.h"

#include <QDebug>
#include <QSet>
#include <QStringBuilder>

#include <cctype>

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <ble-qt5/bledevicediscovery.h>

#include "bledevicediscoveryexception.h"
#include "bleextendedinquiryresponse.h"

QString _write_bytes(void* ptr, int length)
{
    QStringList repr;

    QStringList binaryRepr;
    QString printRepr;

    for (int i = 0; i < length; ++i)
    {
        char ch = reinterpret_cast< char* >(ptr)[i];

        binaryRepr.push_back(QString("%1").arg(ch, 2, 16, QLatin1Char('0')));
        printRepr.push_back(QChar::isPrint(ch)? QChar(ch): '.');

        if ((i + 1) % 8 == 0)
        {
            repr.push_back(binaryRepr.join(QLatin1Char(' ')) % QString("    ") % printRepr);

            binaryRepr.clear();
            printRepr.clear();
        }
    }

    if (binaryRepr.size() > 0)
    {
        while (binaryRepr.size() < 8)
            binaryRepr.push_back("  ");

        repr.push_back(binaryRepr.join(QLatin1Char(' ')) % QString("    ") % printRepr);
    }

    QString data = repr.join(QChar('\n'));

    if (repr.size() > 1)
        data.prepend(QLatin1Char('\n'));

    return data;
}

BLEDeviceDiscoveryWorker::BLEDeviceDiscoveryWorker(int socket)
    : m_socket(socket),
      m_active(false)
{
#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
    qDebug();
#endif
}

BLEDeviceDiscoveryWorker::~BLEDeviceDiscoveryWorker()
{
#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
    qDebug();
#endif
}

bool BLEDeviceDiscoveryWorker::active()
{
    QMutexLocker locker(&m_activeMutex);
    Q_UNUSED(locker);

    return m_active;
}

void BLEDeviceDiscoveryWorker::run()
{
#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
    qDebug();
#endif

    struct HciFilterContainer
    {
        explicit HciFilterContainer(int socket)
            : sockfd(socket)
        {
            socklen_t filterLen = sizeof(filter);

            if (getsockopt(sockfd, SOL_HCI, HCI_FILTER, &filter, &filterLen) < 0)
                throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::GetSockOptError);
        }

        ~HciFilterContainer()
        {
            setsockopt(sockfd, SOL_HCI, HCI_FILTER, &filter, sizeof(filter));
        }

        int sockfd;
        hci_filter filter;
    };

    try
    {
        if (hci_le_set_scan_enable(m_socket, true, true, 1000) < 0)
            throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::HciLeSetScanEnableError);

        HciFilterContainer f(m_socket);
        Q_UNUSED(f);

        hci_filter filter;

        hci_filter_clear(&filter);
        hci_filter_set_ptype(HCI_EVENT_PKT, &filter);
        hci_filter_set_event(EVT_LE_META_EVENT, &filter);

        if (setsockopt(m_socket, SOL_HCI, HCI_FILTER, &filter, sizeof(filter)) < 0)
            throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::SetSockOptError);

        timeval readTimeout;
        memset(&readTimeout, 0, sizeof(readTimeout));
        readTimeout.tv_sec = 1;

        if (setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &readTimeout, sizeof(readTimeout)) < 0)
            throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::SetSockOptError);

#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
        qDebug() << "entering discovery loop";
#endif

        QSet< QString > discoveredAddresses;

        while (active())
        {
            unsigned char buf[HCI_MAX_EVENT_SIZE];

            if (read(m_socket, buf, sizeof(buf)) > 0)
            {
                hci_event_hdr* eventHeader = reinterpret_cast< hci_event_hdr* >(buf);

#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
                qDebug() << "eventHeader.evt:" << eventHeader->evt;
                qDebug() << "eventHeader.plen:" << eventHeader->plen;
#else
                Q_UNUSED(eventHeader);
#endif

                Q_ASSERT(eventHeader->evt == HCI_EVENT_PKT);

                evt_le_meta_event* metaEvent =
                        reinterpret_cast< evt_le_meta_event* >(buf + HCI_EVENT_HDR_SIZE + 1);

#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
                qDebug() << "metaEvent.subevent:" << metaEvent->subevent;
                qDebug() << "metaEvent.data" << metaEvent->data;
#endif

                if (metaEvent->subevent == EVT_LE_ADVERTISING_REPORT)
                {
                    le_advertising_info* info =
                            reinterpret_cast< le_advertising_info* >(metaEvent->data + 1);

#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
                    qDebug() << "info.evt_type" << info->evt_type;
                    qDebug() << "info.bdaddr_type" << info->bdaddr_type;
                    qDebug() << "info.bdaddr" << _write_bytes(&info->bdaddr, sizeof(info->bdaddr));
                    qDebug() << "info.length" << info->length;
                    qDebug() << "info.data" << _write_bytes(info->data, info->length);
#endif

                    char bdaddr[18];
                    memset(bdaddr, 0, sizeof(bdaddr));
                    ba2str(&info->bdaddr, bdaddr);
                    QString address = QString::fromUtf8(bdaddr, sizeof(bdaddr) - 1);

                    if (!discoveredAddresses.contains(address))
                    {
                        discoveredAddresses.insert(address);

                        QString name;

                        int offset = 0;

                        while (offset < info->length && name.isEmpty())
                        {
                            BLEExtendedInquiryResponse response(info->data + offset);

                            if (response.type() == BLEExtendedInquiryResponse::CompleteLocalName ||
                                response.type() == BLEExtendedInquiryResponse::ShortenedLocalName)
                            {
                                name = response.toString();
                            }

                            offset += response.size() + 2;
                        }

                        if (name.isEmpty())
                            name = "(unnamed device)";

                        emit discovered(address, name);
                    }
                }
            }
        }

#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
        qDebug() << "out of discovery loop";
#endif

        if (hci_le_set_scan_enable(m_socket, false, true, 1000) < 0)
            throw BLEDeviceDiscoveryException(BLEDeviceDiscovery::HciLeSetScanDisableError);
    }
    catch (const BLEDeviceDiscoveryException& e)
    {
#ifdef BLEDEVICEDISCOVERYWORKER_DEBUG
        qDebug() << e.error() << e.errorString();
#endif

        emit error(e.error(), e.errorString());
    }
}

void BLEDeviceDiscoveryWorker::setActive(bool active)
{
    QMutexLocker locker(&m_activeMutex);
    Q_UNUSED(locker);

    m_active = active;
}

