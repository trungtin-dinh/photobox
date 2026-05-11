#include "headers/ImageToolBox.h"
#include <complex>

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Transformée de Fourier 2D discrète — module du spectre centré, visualisation log
//
// Formule : F(u,v) = Σ_x Σ_y f(x,y) · exp(-2πi(ux/M + vy/N))
// Le module |F(u,v)| = sqrt(Re² + Im²) est visualisé en échelle logarithmique
// afin de compresser la dynamique : la DC (basses fréquences, centre après fftshift)
// domine sinon tout le spectre. Etapes :
//   1. Conversion en niveaux de gris
//   2. Zero-padding à la taille optimale FFT (puissance de 2, via getOptimalDFTSize)
//   3. DFT complexe via cv::dft
//   4. Module |F(u,v)|
//   5. Mise à l'échelle : log(1 + |F|)
//   6. Normalisation en [0,255]
//   7. fftshift : swap des quadrants pour centrer la composante DC
Mat ImageFourier(const Mat image){
    Mat imageMono = ImageMonochrome(image) ;

    int M = cv::getOptimalDFTSize(imageMono.rows) ;
    int N = cv::getOptimalDFTSize(imageMono.cols) ;
    Mat padded ;
    cv::copyMakeBorder(imageMono, padded,
                       0, M - imageMono.rows,
                       0, N - imageMono.cols,
                       cv::BORDER_CONSTANT, cv::Scalar::all(0)) ;

    Mat planes[2] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) } ;
    Mat complexImg ;
    cv::merge(planes, 2, complexImg) ;
    cv::dft(complexImg, complexImg) ;

    cv::split(complexImg, planes) ;
    cv::magnitude(planes[0], planes[1], planes[0]) ;
    Mat mag = planes[0] ;

    mag += cv::Scalar::all(1.0) ;
    cv::log(mag, mag) ;

    mag = mag(cv::Rect(0, 0, imageMono.cols, imageMono.rows)).clone() ;
    cv::normalize(mag, mag, 0, 255, cv::NORM_MINMAX) ;
    Mat magU8 ;
    mag.convertTo(magU8, CV_8U) ;

    // fftshift : swap des 4 quadrants
    int cx = magU8.cols / 2 ;
    int cy = magU8.rows / 2 ;
    Mat q0(magU8, cv::Rect(0,  0,  cx,              cy)) ;
    Mat q1(magU8, cv::Rect(cx, 0,  magU8.cols - cx, cy)) ;
    Mat q2(magU8, cv::Rect(0,  cy, cx,              magU8.rows - cy)) ;
    Mat q3(magU8, cv::Rect(cx, cy, magU8.cols - cx, magU8.rows - cy)) ;
    Mat tmp ;
    q0.copyTo(tmp) ; q3.copyTo(q0) ; tmp.copyTo(q3) ;
    q1.copyTo(tmp) ; q2.copyTo(q1) ; tmp.copyTo(q2) ;

    return MonoCouleur(magU8) ;
}

// Calcul naïf du module de la DFT en un pixel (u,v) — O(N²) par pixel
// F(u,v) = (1/MN) Σ_x Σ_y f(x,y) · exp(-2πi(ux/M + vy/N))
double ImageFourierElement(const Mat image, const int ligne, const int colonne){
    int L, C ;
    double nbLigne   = (int)image.size().height ;
    double nbColonne = (int)image.size().width ;
    complex<double> resultat(0.0, 0.0) ;
    complex<double> val ;
    complex<double> j(0.0, 1.0) ;

    for(L = 0 ; L < nbLigne ; L++){
        for(C = 0 ; C < nbColonne ; C++){
            val.real(2*3.14*((double)ligne*L/nbLigne + (double)colonne*C/nbColonne)) ;
            resultat += exp(-j*val)*(double)image.at<unsigned char>(L, C) ;
        }
    }
    resultat /= (nbLigne*nbColonne) ;
    return abs(resultat) ;
}

// Rearrangement des quadrants du spectre de Fourier (fftshift manuel)
Mat ImageFourierArranger(const Mat image){
    int ligne, colonne ;
    int nbLigne   = (int)image.size().height ;
    int nbColonne = (int)image.size().width ;
    Mat imageHG(nbLigne/2, nbColonne/2, CV_8U) ;
    Mat imageHD(nbLigne/2, nbColonne/2, CV_8U) ;
    Mat imageBG(nbLigne - nbLigne/2, nbColonne - nbColonne/2, CV_8U) ;
    Mat imageBD(nbLigne - nbLigne/2, nbColonne - nbColonne/2, CV_8U) ;
    Mat imageArrangee(image.size(), CV_8U) ;

    for(ligne = 0 ; ligne < (int)imageHG.size().height ; ligne++)
        for(colonne = 0 ; colonne < (int)imageHG.size().width ; colonne++)
            imageHG.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne, colonne) ;

    for(ligne = 0 ; ligne < (int)imageHD.size().height ; ligne++)
        for(colonne = 0 ; colonne < (int)imageHD.size().width ; colonne++)
            imageHD.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne, colonne + nbColonne/2) ;

    for(ligne = 0 ; ligne < (int)imageBG.size().height ; ligne++)
        for(colonne = 0 ; colonne < (int)imageBG.size().width ; colonne++)
            imageBG.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne + nbLigne/2, colonne) ;

    for(ligne = 0 ; ligne < (int)imageBD.size().height ; ligne++)
        for(colonne = 0 ; colonne < (int)imageBD.size().width ; colonne++)
            imageBD.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne + nbLigne/2, colonne + nbColonne/2) ;

    for(ligne = 0 ; ligne < (int)imageBD.size().height ; ligne++)
        for(colonne = 0 ; colonne < (int)imageBD.size().width ; colonne++)
            imageArrangee.at<unsigned char>(ligne, colonne) = imageHG.at<unsigned char>(ligne, colonne) ;

    return imageArrangee ;
}

} // namespace ImageToolBox
