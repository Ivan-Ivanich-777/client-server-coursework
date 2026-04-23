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

qnx: target.path = /tmp/C:\MyProject\Server{TARGET}/bin
else: unix:!android: target.path = /opt/C:\MyProject\Server{TARGET}/bin
!isEmpty(target.path): INSTALLS += target
