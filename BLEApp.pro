TEMPLATE = app

QT += qml quick widgets bluetooth

SOURCES += main.cpp \
    xble/characteristicinfo.cpp \
    xble/deviceinfo.cpp \
    xble/serviceinfo.cpp \
    xble/xblecommunication.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    xble/characteristicinfo.h \
    xble/deviceinfo.h \
    xble/serviceinfo.h \
    xble/xblecommunication.h
