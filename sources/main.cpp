#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{   // Vérifier les dossiers DATA, Images et Bibliothèques, les créer si nécessaire
    if (!(QDir("DATA").exists())){
        QDir().mkdir("DATA");
        QDir().mkdir("DATA/Images");
        QDir().mkdir("DATA/Bibliotheques");
    }
    if (!(QDir("DATA/Images").exists())){
            QDir().mkdir("DATA/Images");
        }
    if (!(QDir("DATA/Bibliotheques").exists())){
            QDir().mkdir("DATA/Bibliotheques");
        }
    // Lancement de AppMainWindow
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

