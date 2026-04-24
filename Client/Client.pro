QT += core gui network printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    loginform.cpp \
    regform.cpp \
    dashboardform.cpp \
    socketmanager.cpp \
    qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    loginform.h \
    regform.h \
    dashboardform.h \
    socketmanager.h \
    qcustomplot.h

FORMS += \
    loginform.ui \
    regform.ui \
    dashboardform.ui

qnx: target.path = /tmp/C:\MyProject\Client{TARGET}/bin
else: unix:!android: target.path = /opt/C:\MyProject\Client{TARGET}/bin
!isEmpty(target.path): INSTALLS += target
