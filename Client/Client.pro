QT += core gui network printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
SOURCES += main.cpp mainwindow.cpp authform.cpp titleform.cpp taskform.cpp dashboardform.cpp socketmanager.cpp
HEADERS += mainwindow.h authform.h titleform.h taskform.h dashboardform.h socketmanager.h
FORMS += titleform.ui taskform.ui dashboardform.ui
