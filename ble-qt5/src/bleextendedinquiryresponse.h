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

#ifndef BLEEXTENDEDINQUIRYRESPONSE_H
#define BLEEXTENDEDINQUIRYRESPONSE_H

#include <QString>

class BLEExtendedInquiryResponse
{
public:
    // https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile
    enum Type
    {
        Invalid = 0x00,

        Flags = 0x01,
        IncompleteUuid16List = 0x02,
        CompleteUuid16List = 0x03,
        ShortenedLocalName = 0x08,
        CompleteLocalName = 0x09,
        DeviceId = 0x10
    };

public:
    explicit BLEExtendedInquiryResponse(const void* data);
    ~BLEExtendedInquiryResponse();

    Type type() const { return m_type; }
    int size() const { return m_size; }
    const char* data() const { return m_data; }

    bool isValid() const { return m_size > 0; }

    QString toString() const { return QString::fromUtf8(m_data, m_size); }

private:
    Type m_type;

    int m_size;
    char* m_data;
};

#endif
