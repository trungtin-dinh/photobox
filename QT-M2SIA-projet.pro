#-------------------------------------------------
#
# Project created by QtCreator 2022-01-09T13:24:45
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 warn_on link_pkgconfig
PKGCONFIG += opencv4 jsoncpp

TARGET = Photobox-beta
TEMPLATE = app

# Emit warnings for deprecated Qt APIs used by the project code.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PHOTOBOX_SOURCE_DIR=\\\"$$PWD\\\"
# Qt 5.9 headers produce many deprecated-copy warnings with recent GCC versions.
# They are external-library warnings and do not indicate a problem in the app code.
QMAKE_CXXFLAGS += -Wno-deprecated-copy

SOURCES += \
    ./sources/main.cpp \
    ./sources/mainwindow.cpp \
    ./sources/bibliotheque.cpp \
    ./sources/ImageToolBox/ITB_utils.cpp \
    ./sources/ImageToolBox/ITB_couleur.cpp \
    ./sources/ImageToolBox/ITB_filtres_couleur.cpp \
    ./sources/ImageToolBox/ITB_luminosite.cpp \
    ./sources/ImageToolBox/ITB_histogramme.cpp \
    ./sources/ImageToolBox/ITB_debruitage.cpp \
    ./sources/ImageToolBox/ITB_geometrie.cpp \
    ./sources/ImageToolBox/ITB_contours.cpp \
    ./sources/ImageToolBox/ITB_seuillage.cpp \
    ./sources/ImageToolBox/ITB_resolution.cpp \
    ./sources/ImageToolBox/ITB_fourier.cpp \
    ./sources/ImageToolBox/ITB_kmeans.cpp \
    ./sources/ImageToolBox/ITB_hough.cpp

HEADERS += \
    ./headers/mainwindow.h \
    ./headers/bibliotheque.h \
    ./headers/ImageToolBox.h

FORMS += \
    ./ui/mainwindow.ui

RESOURCES += \
    ./DATA/Ressources/icons.qrc \
    ./DATA/Ressources/files.qrc
