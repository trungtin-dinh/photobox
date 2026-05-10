#include "headers/mainwindow.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

#ifndef PHOTOBOX_SOURCE_DIR
#define PHOTOBOX_SOURCE_DIR "."
#endif

namespace {
QString cleanPath(const QString& path){
    return QDir::cleanPath(path) ;
}

QString sourceDataRoot(){
    const QString sourceDir = QString::fromLocal8Bit(PHOTOBOX_SOURCE_DIR) ;
    if(sourceDir.isEmpty()){
        return QString() ;
    }
    return cleanPath(QDir(sourceDir).filePath("DATA")) ;
}

QString findExistingDataRoot(const QString& appDir){
    // Keep the source/release DATA directory stable. Do not use the current
    // directory as a first candidate: QFileDialog may change it to
    // DATA/Bibliotheques, which previously created paths such as
    // DATA/Bibliotheques/DATA/Bibliotheques.
    const QStringList candidates = {
        sourceDataRoot(),
        appDir + "/DATA",
        appDir + "/../DATA",
        appDir + "/../photobox/DATA",
        appDir + "/../../photobox/DATA"
    } ;

    for(const QString& candidate : candidates){
        if(candidate.isEmpty()){
            continue ;
        }
        const QDir dir(cleanPath(candidate)) ;
        if(dir.exists()){
            return dir.absolutePath() ;
        }
    }

    return cleanPath(appDir + "/DATA") ;
}

void prepareDataDirectories(const QString& appDir){
    const QString dataRoot = findExistingDataRoot(appDir) ;
    QDir().mkpath(dataRoot + "/Images") ;
    QDir().mkpath(dataRoot + "/Bibliotheques") ;

    // Make relative paths such as ./DATA/Images/... stable when the program
    // is launched from Qt Creator or from a release folder.
    const QFileInfo dataInfo(dataRoot) ;
    if(dataInfo.exists()){
        QDir::setCurrent(dataInfo.absoluteDir().absolutePath()) ;
    }
}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    prepareDataDirectories(QCoreApplication::applicationDirPath()) ;

    MainWindow w;
    w.show();

    return a.exec();
}
