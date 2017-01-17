TEMPLATE = app
TARGET = bttest

CONFIG *= c++11

QMAKE_CXXFLAGS *= -Wall -Werror

INCLUDEPATH *= ../ble-qt5/include

LIBS *= -L../ble-qt5 -lble-qt5 -lbluetooth

QMAKE_RPATHDIR = /home/nemo/bttest

QT = core

SOURCES = \
    main.cpp
