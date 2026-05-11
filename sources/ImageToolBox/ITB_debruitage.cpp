#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Filtre median sur une composante (voisinage 3×3)
Mat ImageMedianMono(const Mat image){
    int ligne, colonne, lig, col ;
    int taille = 3 ;
    Mat matrice = ImageZero(taille, taille) ;
    Mat imageMiroir = ImageMiroir(image, taille) ;
    Mat imageMedian(image.size(), CV_8U) ;

    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            for(lig = 0 ; lig < taille ; lig++){
                for(col = 0 ; col < 3 ; col++){
                    matrice.at<unsigned char>(lig, col) = imageMiroir.at<unsigned char>(ligne + lig, colonne + col) ;
                }
            }
            imageMedian.at<unsigned char>(ligne, colonne) = MatriceMedian(matrice) ;
        }
    }
    return imageMedian ;
}

// Filtre median sur tous les canaux
Mat ImageMedian(const Mat image){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageMedian ;
    vector<Mat> imageMedianComposante ;

    split(image, imageComposante) ;
    for(c = 0 ; c < nbComposante ; c++) imageMedianComposante.push_back(ImageMedianMono(imageComposante[c])) ;
    merge(imageMedianComposante, imageMedian) ;
    return imageMedian ;
}

// Filtrage par convolution (1=moyenneur, 2=gaussien, 3=median)
Mat ImageFiltrage(const Mat image, const int choix){
    switch(choix){
        case 1 : return ImageConvolution(image, GenererFiltre(1)) ;
        case 2 : return ImageConvolution(image, GenererFiltre(3)) ;
        case 3 : return ImageMedian(image) ;
        default: return ImageConvolution(image, GenererFiltre(3)) ;
    }
}

// Lissage iteratif par filtre gaussien
Mat ImageLissage(const Mat image, const int valeur){
    int c ;
    Mat imageResultante = image ;
    for(c = 0 ; c < valeur ; c++) imageResultante = ImageConvolution(imageResultante, GenererFiltre(3)) ;
    return imageResultante ;
}

// Filtre Kuwahara-Nagao sur une composante (9 domaines de 3×3)
Mat ImageKuwaharaMono(const Mat image){
    int ligne, colonne ;
    int indiceMin ;
    int val ;
    Mat imageMiroir  = ImageMiroir(image, 5) ;
    Mat imageResultante = image ;
    vector<double> moyenne(9) ;
    vector<double> variance(9) ;
    vector<double> domaine(9) ;

    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            moyenne.clear() ; variance.clear() ;

            // Domaine 1
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+4)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 2
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 3
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+3)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 4
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 5
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 6
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 7
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+4)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 8
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+1)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 9
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne,   colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back(0) ; domaine.push_back(0) ;
            moyenne.push_back(MoyenneVecteur(domaine)) ; variance.push_back(VarianceVecteur(domaine)) ;

            indiceMin = MinVecteurIndice(variance) ;
            val = (int)moyenne[indiceMin] ;
            imageResultante.at<unsigned char>(ligne, colonne) = VerifierSaturation(val) ;
        }
    }
    return imageResultante ;
}

// Filtre Kuwahara-Nagao sur tous les canaux
Mat ImageKuwahara(const Mat image){
    int c ;
    Mat imageComposante[(int)image.channels()] ;
    Mat imageResultante ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    split(image, imageComposante) ;
    for(c = 0 ; c < (int)image.channels() ; c++) imageResultanteComposante.push_back(ImageKuwaharaMono(imageComposante[c])) ;
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Bruit gaussien additif
Mat ImageBruitGaussien(const Mat image, const double moyenne, const double sigma){
    int c, ligne, colonne ;
    int val ;
    Mat imageComposante[(int)image.channels()] ;
    Mat imageBruit(image.size(), CV_8U) ;
    Mat imageResultante ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    randn(imageBruit, moyenne, sigma) ;
    split(image, imageComposante) ;

    for(c = 0 ; c < (int)image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = VerifierSaturation((int)imageComposante[c].at<unsigned char>(ligne, colonne) + (int)imageBruit.at<unsigned char>(ligne, colonne)) ;
                imageComposante[c].at<unsigned char>(ligne, colonne) = val ;
            }
        }
        imageResultanteComposante.push_back(imageComposante[c]) ;
    }
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Bruit poivre et sel
Mat ImageBruitPoivreSel(const Mat image){
    int c, ligne, colonne ;
    int val ;
    Mat imageComposante[(int)image.channels()] ;
    Mat imageBruitSel(image.size(), CV_8U) ;
    Mat imageBruitPoivre(image.size(), CV_8U) ;
    Mat imageResultante ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    randn(imageBruitSel, 75, 20) ;
    imageBruitSel = ImageSeuillage(imageBruitSel, 127) ;
    randn(imageBruitPoivre, 75, 20) ;
    imageBruitPoivre = ImageSeuillage(imageBruitPoivre, 127) ;

    split(image, imageComposante) ;
    for(c = 0 ; c < (int)image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = VerifierSaturation((int)imageComposante[c].at<unsigned char>(ligne, colonne) + (int)imageBruitSel.at<unsigned char>(ligne, colonne)) ;
                imageComposante[c].at<unsigned char>(ligne, colonne) = val ;
                val = VerifierSaturation((int)imageComposante[c].at<unsigned char>(ligne, colonne) - (int)imageBruitPoivre.at<unsigned char>(ligne, colonne)) ;
                imageComposante[c].at<unsigned char>(ligne, colonne) = val ;
            }
        }
        imageResultanteComposante.push_back(imageComposante[c]) ;
    }
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

} // namespace ImageToolBox
