#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Verifier la saturation d'une valeur de pixel
int VerifierSaturation(const int valeur){
    if(valeur > 255) return 255 ;
    if(valeur < 0)   return 0 ;
    return valeur ;
}

// Verifier si l'image est en couleurs ou en niveau de gris
bool VerifierImage(const Mat image, Mat& imageVerifiee){
    bool imageNiveauGris ;
    vector<Mat> composante ;

    if(image.channels() == 1){
        imageVerifiee = image ;
        imageNiveauGris = true ;
    }else{
        split(image, composante) ;
        if(MatriceEgale(composante[0], composante[1]) && MatriceEgale(composante[1], composante[2])){
            imageVerifiee = composante[0] ;
            imageNiveauGris = true ;
        }else{
            imageVerifiee = image ;
            imageNiveauGris = false ;
        }
    }
    return imageNiveauGris ;
}

// Echanger les composantes R et B de l'image (BGR → RGB)
Mat ImageBGRRGB(const Mat image){
    if(image.channels() == 1) return image.clone() ;
    Mat imageResultante ;
    cvtColor(image, imageResultante, COLOR_BGR2RGB) ;
    return imageResultante ;
}

// Convertir une image monochrome en image 3 canaux identiques
Mat MonoCouleur(const Mat image){
    int c ;
    Mat imageCouleur(image.size(), CV_8UC3) ;
    vector<Mat> imageCouleurComposante ;
    for(c = 0 ; c < 3 ; c++){
        imageCouleurComposante.push_back(image) ;
    }
    merge(imageCouleurComposante, imageCouleur) ;
    return imageCouleur ;
}

// Generer une matrice zero
Mat ImageZero(const int nbLigne, const int nbColonne){
    return Mat::zeros(nbLigne, nbColonne, CV_8U) ;
}

// Valeur minimum dans une image
int MinImage(const Mat image){
    double minVal ;
    cv::minMaxLoc(image, &minVal, nullptr) ;
    return (int)minVal ;
}

// Valeur maximum dans une image
int MaxImage(const Mat image){
    double maxVal ;
    cv::minMaxLoc(image, nullptr, &maxVal) ;
    return (int)maxVal ;
}

// Valeur minimum dans un vecteur reel
double MinVecteur(const vector<double> vecteur){
    int c ;
    double valeurMin = vecteur[0] ;
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMin >= vecteur[c]) valeurMin = vecteur[c] ;
    }
    return valeurMin ;
}

// Valeur maximum dans un vecteur reel
double MaxVecteur(const vector<double> vecteur){
    int c ;
    double valeurMax = vecteur[0] ;
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMax <= vecteur[c]) valeurMax = vecteur[c] ;
    }
    return valeurMax ;
}

// Valeur minimum dans un vecteur entier
int MinVecteur(const vector<int> vecteur){
    int c ;
    int valeurMin = vecteur[0] ;
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMin >= vecteur[c]) valeurMin = vecteur[c] ;
    }
    return valeurMin ;
}

// Valeur maximum dans un vecteur entier
int MaxVecteur(const vector<int> vecteur){
    int c ;
    int valeurMax = vecteur[0] ;
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMax < vecteur[c]) valeurMax = vecteur[c] ;
    }
    return valeurMax ;
}

// Indice du minimum dans un vecteur entier
int MinVecteurIndice(const vector<int> vecteur){
    int c, valeurMin = vecteur[0], indiceMin = 0 ;
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(vecteur[c] < valeurMin){ valeurMin = vecteur[c] ; indiceMin = c ; }
    }
    return indiceMin ;
}

// Indice du minimum dans un vecteur reel
int MinVecteurIndice(const vector<double> vecteur){
    int c, indiceMin = 0 ;
    double valeurMin = vecteur[0] ;
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(vecteur[c] < valeurMin){ valeurMin = vecteur[c] ; indiceMin = c ; }
    }
    return indiceMin ;
}

// Valeur mediane d'une matrice
int MatriceMedian(const Mat matrice){
    int ligne, colonne ;
    vector<int> vecteurValeur ;
    for(ligne = 0 ; ligne < matrice.size().height ; ligne++){
        for(colonne = 0 ; colonne < matrice.size().width ; colonne++){
            vecteurValeur.push_back((int)matrice.at<unsigned char>(ligne, colonne)) ;
        }
    }
    return VecteurMedian(vecteurValeur) ;
}

// Valeur mediane d'un vecteur
int VecteurMedian(vector<int> vecteur){
    if(vecteur.empty()) return 0 ;
    const size_t medianIndex = vecteur.size()/2 ;
    nth_element(vecteur.begin(), vecteur.begin() + medianIndex, vecteur.end()) ;
    return vecteur[medianIndex] ;
}

// Egalite entre deux matrices de memes dimensions
bool MatriceEgale(const Mat matrice1, const Mat matrice2){
    if(matrice1.size() != matrice2.size() || matrice1.type() != matrice2.type()) return false ;
    return cv::countNonZero(matrice1 != matrice2) == 0 ;
}

// Moyenne d'un vecteur reel
double MoyenneVecteur(const vector<double> vecteur){
    int c ;
    double moyenne = 0 ;
    for(c = 0 ; c < (int)vecteur.size() ; c++) moyenne += vecteur[c] ;
    return (double)moyenne / (int)vecteur.size() ;
}

// Variance d'un vecteur reel
double VarianceVecteur(const vector<double> vecteur){
    int c ;
    double somme = 0 ;
    double moyenne = MoyenneVecteur(vecteur) ;
    for(c = 0 ; c < (int)vecteur.size() ; c++) somme += vecteur[c]*vecteur[c] ;
    return (double)1/(int)vecteur.size()*somme - moyenne*moyenne ;
}

} // namespace ImageToolBox
