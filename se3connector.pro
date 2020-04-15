TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += `pkg-config libusb-1.0 libudev --libs`
LIBS += -pthread

INCLUDEPATH += ../hidapi `pkg-config libusb-1.0 --cflags`
INCLUDEPATH += /usr/include/libusb-1.0/ ./Database

DEFINES += HOST_DEV
SOURCES += main.cpp \
    se3device.cpp \
    Sockets/filesocket.cpp \
    Sockets/metersocket.cpp \
    hidapi/hid-libusb.c \
    Helpers/helpers.cpp \
    Helpers/leq.cpp \
    common.cpp \
    Helpers/leds.cpp \

HEADERS += \
    DeviceBase.hpp \
    se3device.hpp \
    ise3commands.hpp \
    Sockets/filesocket.hpp \
    Sockets/protocoldefs.hpp \
    Sockets/metersocket.hpp \
    Sockets/iprotocolinterface.hpp \
    hidapi/hidapi.h \
    Sockets/isocketbase.hpp \
    Observers/observers.hpp \
    Helpers/helpers.hpp \
    Helpers/leq.hpp \
    common.hpp \
    Helpers/leds.hpp \
    version.hpp
