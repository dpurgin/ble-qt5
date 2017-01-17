# Qt5 wrapper for Bluetooth LE using HCI and GATT
# Copyright (c) 2016 Dmitriy Purgin <dpurgin@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute,
# sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

TEMPLATE = lib

TARGET = ble-qt5
VERSION = 0.1

QT = core

QMAKE_CXXFLAGS *= -Wall -Werror

CONFIG += qt link_pkgconfig plugin

DEFINES += BLE_QT5_LIB \
    # BLEDEVICEDISCOVERYWORKER_DEBUG

PKGCONFIG += bluez

INCLUDEPATH += include

HEADERS = \
    include/ble-qt5/bledevicediscovery.h \
    src/bledevicediscovery_p.h \
    src/bledevicediscoveryexception.h \
    src/bledevicediscoveryworker.h \
    src/bleextendedinquiryresponse.h

SOURCES = \
    src/bledevicediscovery.cpp \
    src/bledevicediscovery_p.cpp \
    src/bledevicediscoveryexception.cpp \
    src/bledevicediscoveryworker.cpp \
    src/bleextendedinquiryresponse.cpp
