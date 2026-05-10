#-------------------------------------------------
#
# Project created by QtCreator 2022-01-09T13:24:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += opencv4
CONFIG += link_pkgconfig
PKGCONFIG += jsoncpp

TARGET = Photobox-beta
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        ./sources/main.cpp \
        ./sources/mainwindow.cpp \
        ./sources/bibliotheque.cpp \
        ./sources/ImageToolBox.cpp

HEADERS += \
        ./headers/mainwindow.h \
        ./headers/bibliotheque.h \
        ./headers/ImageToolBox.h \

FORMS += \
        ./ui/mainwindow.ui \


LIBS += \
        -ljsoncpp -lstdc++fs \
        -I/opencv2/include/ -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio

RESOURCES += \
    ./DATA/Ressources/icons.qrc \
    ./DATA/Ressources/files.qrc



