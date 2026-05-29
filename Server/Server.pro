QT -= gui
QT += network sql
CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    myserver.cpp \
    servercore.cpp \
    dbmanager.cpp

HEADERS += \
    myserver.h \
    servercore.h \
    dbmanager.h
