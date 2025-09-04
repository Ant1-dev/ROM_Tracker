QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# Uncomment the following line if needed.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/fileManager.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/database.cpp

HEADERS += \
    include/fileManager.h \
    include/mainwindow.h \
    include/database.h \
    include/measurement.h

FORMS += \
    ui/mainwindow.ui

# style.qss is not a Qt resource, just a plain file for runtime
DISTFILES += \
    resources/style.qss

RESOURCES += \
    resources.qrc
