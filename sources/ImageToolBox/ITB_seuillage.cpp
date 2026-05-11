#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// ---- Seuillage ----

// Seuillage simple d'une composante (binaire : 0 ou 255)
Mat ImageSeuillage(const Mat image, const int seuil){
    int ligne, colonne ;
    Mat imageSeuil(image.size(), CV_8U) ;
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageSeuil.at<unsigned char>(ligne, colonne) = ((int)image.at<unsigned char>(ligne, colonne) < seuil) ? 0 : 255 ;
        }
    }
    return imageSeuil ;
}

// Seuillage simple d'une image (un seuil par canal)
Mat ImageSeuillage(const Mat image, vector<int> seuil){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageSeuillage ;
    vector<Mat> imageSeuillageComposante ;

    imageSeuillageComposante.clear() ;
    split(image, imageComposante) ;
    for(c = 0 ; c < nbComposante ; c++) imageSeuillageComposante.push_back(ImageSeuillage(imageComposante[c], seuil[c])) ;
    merge(imageSeuillageComposante, imageSeuillage) ;
    return imageSeuillage ;
}

// Seuillage par hysteresis d'une composante (bande [seuilBas, seuilHaut])
Mat ImageSeuillage(const Mat image, const int seuilBas, const int seuilHaut){
    return (ImageSeuillage(image, seuilBas) - ImageSeuillage(image, seuilHaut)) ;
}

// Seuillage par hysteresis d'une image (un intervalle par canal)
Mat ImageSeuillage(const Mat image, vector<int> seuilBas, vector<int> seuilHaut){
    return (ImageSeuillage(image, seuilBas) - ImageSeuillage(image, seuilHaut)) ;
}

// ---- Segmentation ----

// Segmentation d'une composante (masquage par seuillage simple)
Mat ImageSegmentation(const Mat image, const int seuil){
    int ligne, colonne ;
    Mat imageSeuillee = ImageSeuillage(image, seuil) ;
    Mat imageSegmentee(image.size(), CV_8U) ;
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageSegmentee.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne, colonne) * (imageSeuillee.at<unsigned char>(ligne, colonne)/255) ;
        }
    }
    return imageSegmentee ;
}

// Segmentation d'une image (un seuil par canal)
Mat ImageSegmentation(const Mat image, vector<int> seuil){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageSegmentee ;
    vector<Mat> imageSegmenteeComposante ;

    imageSegmenteeComposante.clear() ;
    split(image, imageComposante) ;
    for(c = 0 ; c < nbComposante ; c++) imageSegmenteeComposante.push_back(ImageSegmentation(imageComposante[c], seuil[c])) ;
    merge(imageSegmenteeComposante, imageSegmentee) ;
    return imageSegmentee ;
}

// Segmentation par hysteresis d'une composante
Mat ImageSegmentation(const Mat image, const int seuilBas, const int seuilHaut){
    return (ImageSegmentation(image, seuilBas) - ImageSegmentation(image, seuilHaut)) ;
}

// Segmentation par hysteresis d'une image
Mat ImageSegmentation(const Mat image, vector<int> seuilBas, vector<int> seuilHaut){
    return (ImageSegmentation(image, seuilBas) - ImageSegmentation(image, seuilHaut)) ;
}

} // namespace ImageToolBox
