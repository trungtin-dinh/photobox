#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Modifier la luminosite globale de l'image
Mat ImageLuminosite(const Mat image, const int valeur){
    int c, ligne, colonne ;
    int intensite ;
    Mat imageModif ;
    Mat imageComposante[image.channels()] ;
    vector<Mat> imageModifComposante ;

    imageModifComposante.clear() ;
    split(image, imageComposante) ;

    for(c = 0 ; c < image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                intensite = (int)imageComposante[c].at<unsigned char>(ligne, colonne) + valeur ;
                imageComposante[c].at<unsigned char>(ligne, colonne) = VerifierSaturation(intensite) ;
            }
        }
        imageModifComposante.push_back(imageComposante[c]) ;
    }

    merge(imageModifComposante, imageModif) ;
    return imageModif ;
}

// Modifier les ombres (basses intensites, < 128)
Mat ImageOmbre(const Mat image, int valeur){
    int c, ligne, colonne ;
    int intensite ;
    Mat imageModif ;
    Mat imageComposante[image.channels()] ;
    vector<Mat> imageModifComposante ;

    imageModifComposante.clear() ;
    split(image, imageComposante) ;

    for(c = 0 ; c < image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                if(imageComposante[c].at<unsigned char>(ligne, colonne) < 128){
                    intensite = (int)imageComposante[c].at<unsigned char>(ligne, colonne) + valeur ;
                    imageComposante[c].at<unsigned char>(ligne, colonne) = VerifierSaturation(intensite) ;
                }
            }
        }
        imageModifComposante.push_back(imageComposante[c]) ;
    }

    merge(imageModifComposante, imageModif) ;
    return imageModif ;
}

// Modifier les hautes lumieres (hautes intensites, >= 128)
Mat ImageHauteLumiere(const Mat image, int valeur){
    int c, ligne, colonne ;
    int intensite ;
    Mat imageModif ;
    Mat imageComposante[image.channels()] ;
    vector<Mat> imageModifComposante ;

    imageModifComposante.clear() ;
    split(image, imageComposante) ;

    for(c = 0 ; c < image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                if(imageComposante[c].at<unsigned char>(ligne, colonne) >= 128){
                    intensite = (int)imageComposante[c].at<unsigned char>(ligne, colonne) + valeur ;
                    imageComposante[c].at<unsigned char>(ligne, colonne) = VerifierSaturation(intensite) ;
                }
            }
        }
        imageModifComposante.push_back(imageComposante[c]) ;
    }

    merge(imageModifComposante, imageModif) ;
    return imageModif ;
}

} // namespace ImageToolBox
