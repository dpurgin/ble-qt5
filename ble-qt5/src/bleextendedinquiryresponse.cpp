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

#include "bleextendedinquiryresponse.h"

#include <cstring>

BLEExtendedInquiryResponse::BLEExtendedInquiryResponse(const void* data)
    : m_type(Invalid),
      m_size(0),
      m_data(Q_NULLPTR)
{
    const char* bytes = reinterpret_cast< const char* >(data);

    m_size = static_cast< int >(bytes[0]);

    if (m_size > 0)
    {
        m_size -= 1; // subtract type field

        m_type = static_cast< BLEExtendedInquiryResponse::Type >(bytes[1]);
        m_data = new char[m_size];

        std::memcpy(m_data, bytes + 2, m_size);
    }
}

BLEExtendedInquiryResponse::~BLEExtendedInquiryResponse()
{
    if (m_data)
        delete[] m_data;
}
