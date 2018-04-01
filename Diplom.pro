#-------------------------------------------------
#
# Project created by QtCreator 2018-03-04T09:09:04
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT		 += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diplom
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

PRECOMPILED_HEADER = Headers/ph/ph.hpp

INCLUDEPATH += $$PWD 'include/boost_1_65_1/'

SOURCES += \
main.cpp \
MainWindow.cpp \
Sources/Model/Gauss.cpp \
Sources/Model/Matrix.cpp \
Sources/Model/PhisicalModel.cpp \
Sources/Tests/testMain.cpp \
Sources/ph/ph.cpp \
Sources/Utils/Log.cpp \
Sources/Views/ChartBuilder.cpp


HEADERS += \
MainWindow.hpp \
Headers/Model/Gauss.hpp \
Headers/Model/Matrix.hpp \
Headers/Model/PhisicalModel.hpp \
Headers/ph/ph.hpp \
Headers/Tests/testslib.hpp \
Headers/Utils/Log.hpp \
Headers/Views/ChartBuilder.hpp \
    Headers/Views/Builder.hpp


FORMS += \
mainwindow.ui

DISTFILES += \
Input_value_viol.inf
