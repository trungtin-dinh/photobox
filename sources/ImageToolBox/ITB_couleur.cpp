#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Modifier la temperature de l'image
Mat ImageTemperature(const Mat image, const int valeur){
    int c, ligne, colonne ;
    int val ;
    Mat imageResultante ;
    Mat imageComposante[3] ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    split(image, imageComposante) ;

    if(valeur > 0){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = VerifierSaturation(imageComposante[2].at<unsigned char>(ligne, colonne) + valeur) ;
                imageComposante[2].at<unsigned char>(ligne, colonne) = val ;
                val = VerifierSaturation(imageComposante[1].at<unsigned char>(ligne, colonne) + valeur) ;
                imageComposante[1].at<unsigned char>(ligne, colonne) = val ;
            }
        }
    }else{
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = VerifierSaturation(imageComposante[0].at<unsigned char>(ligne, colonne) + abs(valeur)) ;
                imageComposante[0].at<unsigned char>(ligne, colonne) = val ;
                val = VerifierSaturation(imageComposante[1].at<unsigned char>(ligne, colonne) + abs(valeur)) ;
                imageComposante[1].at<unsigned char>(ligne, colonne) = val ;
            }
        }
    }

    for(c = 0 ; c < 3 ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Modifier la saturation
Mat ImageSaturation(const Mat image, const int valeur){
    Mat imageHSV = image ;
    int c ;
    Mat imageResultante ;
    Mat imageComposante[3] ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    cvtColor(imageHSV, imageHSV, COLOR_BGR2HSV) ;
    split(imageHSV, imageComposante) ;

    if(VerifierImage(image, imageResultante) == false){
        imageComposante[1] = ImageSaturationElement(imageComposante[1], valeur) ;
        for(c = 0 ; c < 3 ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
        merge(imageResultanteComposante, imageHSV) ;
        cvtColor(imageHSV, imageResultante, COLOR_HSV2BGR) ;
    }
    return imageResultante ;
}

// Calcul elementaire pour la modification de saturation
Mat ImageSaturationElement(const Mat image, const int valeur){
    int ligne, colonne ;
    int val ;
    Mat imageResultante = image ;

    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            val = VerifierSaturation(imageResultante.at<unsigned char>(ligne, colonne) + valeur) ;
            imageResultante.at<unsigned char>(ligne, colonne) = val ;
        }
    }
    return imageResultante ;
}

// Modifier la teinte
Mat ImageTeinte(const Mat image, const int valeur){
    Mat imageHSV ;
    int c, ligne, colonne ;
    int val ;
    Mat imageResultante ;
    Mat imageComposante[3] ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    cvtColor(image, imageHSV, COLOR_BGR2HSV) ;
    split(imageHSV, imageComposante) ;

    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            val = (double)imageComposante[0].at<unsigned char>(ligne, colonne) + valeur ;
            imageComposante[0].at<unsigned char>(ligne, colonne) = (int)val%255 ;
        }
    }

    for(c = 0 ; c < 3 ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
    merge(imageResultanteComposante, imageHSV) ;
    cvtColor(imageHSV, imageResultante, COLOR_HSV2BGR) ;
    return imageResultante ;
}

// Modifier la vividite
Mat ImageVividite(const Mat image, const int valeur){
    int c, ligne, colonne ;
    int maxR, maxV, maxB ;
    int minR, minV, minB ;
    int val ;
    Mat imageResultante ;
    Mat imageComposante[3] ;
    vector<int> vectVal(3, 0) ;
    vector<int> intervalle ;
    vector<Mat> imageResultanteComposante ;

    intervalle.reserve(3) ;
    imageResultanteComposante.reserve(3) ;
    split(image, imageComposante) ;

    maxB = MaxImage(imageComposante[0]) ; minB = MinImage(imageComposante[0]) ;
    intervalle.push_back(maxB - minB) ;
    maxV = MaxImage(imageComposante[1]) ; minV = MinImage(imageComposante[1]) ;
    intervalle.push_back(maxV - minV) ;
    maxR = MaxImage(imageComposante[2]) ; minR = MinImage(imageComposante[2]) ;
    intervalle.push_back(maxR - minR) ;

    if((intervalle[0] != intervalle[1]) || (intervalle[0] != intervalle[2]) || (intervalle[2] != intervalle[1])){
        val = MinVecteur(intervalle) ;
        for(c = 0 ; c < (int)intervalle.size() ; c++){
            vectVal[c] = (intervalle[c] == val) ? valeur : valeur/3 ;
        }
    }

    if(VerifierImage(image, imageResultante) == false){
        for(c = 0 ; c < 3 ; c++){
            for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
                for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                    val = VerifierSaturation((int)imageComposante[c].at<unsigned char>(ligne, colonne) + vectVal[c]) ;
                    imageComposante[c].at<unsigned char>(ligne, colonne) = val ;
                }
            }
        }
        for(c = 0 ; c < 3 ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = image ;
    }
    return imageResultante ;
}

} // namespace ImageToolBox
