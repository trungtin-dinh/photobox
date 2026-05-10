#-------------------------------------------------
#
# Project created by QtCreator 2022-01-09T13:24:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
        /home/vm/M2SIA-projet-QT/sources/main.cpp \
        /home/vm/M2SIA-projet-QT/sources/mainwindow.cpp \
        /home/vm/M2SIA-projet-QT/sources/bibliotheque.cpp \
        /home/vm/M2SIA-projet-QT/sources/ImageToolBox.cpp

HEADERS += \
        /home/vm/M2SIA-projet-QT/headers/mainwindow.h \
        /home/vm/M2SIA-projet-QT/headers/bibliotheque.h \
        /home/vm/M2SIA-projet-QT/headers/ImageToolBox.h \

FORMS += \
        /home/vm/M2SIA-projet-QT/ui/mainwindow.ui \


LIBS += \
        -ljsoncpp -lstdc++fs \
        -I/opencv2/include/ -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio

RESOURCES += \
    /home/vm/DATA/Ressources/icons.qrc \
    /home/vm/DATA/Ressources/files.qrc



