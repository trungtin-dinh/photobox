#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Histogramme discret d'une image monocanal (nbIntervalle intervalles reguliers)
vector<int> Histogramme(const Mat image, const int nbIntervalle){
    int c, ligne, colonne ;
    vector<int> occurence(nbIntervalle, 0) ;

    for(c = 0 ; c < nbIntervalle - 1 ; c++){
        for(ligne = 0 ; ligne < image.size().height ; ligne++){
            for(colonne = 0 ; colonne < image.size().width ; colonne++){
                if((image.at<unsigned char>(ligne, colonne) >= ((float)255/nbIntervalle*c)) &&
                   (image.at<unsigned char>(ligne, colonne) <  ((float)255/nbIntervalle*(c+1)))){
                    occurence[c]++ ;
                }
            }
        }
    }
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            if(image.at<unsigned char>(ligne, colonne) == ((float)255/nbIntervalle*c)){
                occurence[nbIntervalle - 1]++ ;
            }
        }
    }
    return occurence ;
}

// Egalisation de l'histogramme (CDF normalisee)
vector<int> Egalisation(const Mat image, const int nbIntervalle){
    int c ;
    int nbPixel = image.size().height * image.size().width ;
    double somme = 0 ;
    double coefficient = (double)255 / nbPixel ;
    vector<int> occurence     = Histogramme(image, nbIntervalle) ;
    vector<int> occurenceEgal(nbIntervalle) ;

    for(c = 0 ; c < nbIntervalle ; c++){
        somme += occurence[c] ;
        occurenceEgal[c] = coefficient * somme ;
    }
    return occurenceEgal ;
}

// Normalisation de l'histogramme d'une composante
Mat NormalisationMono(const Mat image, const int valeur){
    int ligne, colonne ;
    int min = MinImage(image) ;
    int max = MaxImage(image) ;
    int minNorm = min - valeur ;
    int maxNorm = max + valeur ;
    double val ;
    Mat imageNorm(image.size(), CV_8U) ;

    if(minNorm < 0)    minNorm = 0 ;
    if(minNorm > 127)  minNorm = 127 ;
    if(maxNorm > 255)  maxNorm = 255 ;
    if(maxNorm < 127)  maxNorm = 127 ;

    val = (double)(maxNorm - minNorm) / (max - min) ;
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageNorm.at<unsigned char>(ligne, colonne) = (double)(image.at<unsigned char>(ligne, colonne) - min)*val + minNorm ;
        }
    }
    return imageNorm ;
}

// Normalisation de l'histogramme d'une image (tous canaux)
Mat Normalisation(const Mat image, const int valeur){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageNorm ;
    vector<Mat> imageNormComposante ;

    imageNormComposante.clear() ;
    split(image, imageComposante) ;
    for(c = 0 ; c < nbComposante ; c++) imageNormComposante.push_back(NormalisationMono(imageComposante[c], valeur)) ;
    merge(imageNormComposante, imageNorm) ;
    return imageNorm ;
}

// Egalisation de l'histogramme d'une composante
Mat ImageEgalisationMono(const Mat image){
    int ligne, colonne ;
    vector<int> valeurEgal = Egalisation(image, 255) ;
    Mat imageEgal(image.size(), CV_8U) ;

    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageEgal.at<unsigned char>(ligne, colonne) = valeurEgal[(int)image.at<unsigned char>(ligne, colonne)] ;
        }
    }
    return imageEgal ;
}

// Egalisation de l'histogramme d'une image (tous canaux)
Mat ImageEgalisation(const Mat image){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageEgal ;
    vector<Mat> imageEgalComposante ;

    split(image, imageComposante) ;
    if(VerifierImage(image, imageEgal)){
        return MonoCouleur(ImageEgalisationMono(imageComposante[0])) ;
    }else{
        for(c = 0 ; c < nbComposante ; c++) imageEgalComposante.push_back(ImageEgalisationMono(imageComposante[c])) ;
        merge(imageEgalComposante, imageEgal) ;
        return imageEgal ;
    }
}

// Calcul de l'histogramme d'un canal (vecteur de 256 occurences)
Mat CalcHistogram(Mat image){
    Mat hist = Mat::zeros(256, 1, CV_32F) ;
    image.convertTo(image, CV_32F) ;
    double value = 0 ;
    for(int i = 0 ; i < image.rows ; i++){
        for(int j = 0 ; j < image.cols ; j++){
            value = image.at<float>(i, j) ;
            hist.at<float>(value) = hist.at<float>(value) + 1 ;
        }
    }
    return hist ;
}

// Image du graphe histogramme pour un canal (400×512 pixels)
Mat PlotHistogram1D(Mat image, int choixCouleur){
    Mat histogram_image(400, 512, CV_8UC3, Scalar(0, 0, 0)) ;
    Mat normalized_histogram ;
    Mat histogram = CalcHistogram(image) ;
    Scalar couleur ;

    normalize(histogram, normalized_histogram, 0, 400, NORM_MINMAX, -1, Mat()) ;
    if(choixCouleur==1)      couleur = Scalar(255,0,0) ;   // Bleu  (BGR)
    else if(choixCouleur==2) couleur = Scalar(0,255,0) ;   // Vert
    else if(choixCouleur==3) couleur = Scalar(0,0,255) ;   // Rouge
    else                     couleur = Scalar(255,255,255) ;

    for(int i = 0 ; i < 256 ; i++){
        rectangle(histogram_image,
                  Point(2*i,     histogram_image.rows - normalized_histogram.at<float>(i)),
                  Point(2*(i+1), histogram_image.rows),
                  couleur) ;
    }
    return histogram_image ;
}

// Image du graphe histogramme (1 canal ou 3 canaux BGR)
Mat PlotHistogram(Mat image){
    Mat imageTemp ;
    if(VerifierImage(image, imageTemp)){
        return PlotHistogram1D(imageTemp, 0) ;   // Niveaux de gris
    }
    Mat histogram_image_R = PlotHistogram1D(ImageExtractionCouleur(imageTemp, 3), 3) ;
    Mat histogram_image_G = PlotHistogram1D(ImageExtractionCouleur(imageTemp, 2), 2) ;
    Mat histogram_image_B = PlotHistogram1D(ImageExtractionCouleur(imageTemp, 1), 1) ;
    Mat histogram_image ;
    addWeighted(histogram_image_B, 0.5, histogram_image_G, 0.5, 0.0, histogram_image) ;
    addWeighted(histogram_image,   0.5, histogram_image_R, 0.5, 0.0, histogram_image) ;
    return histogram_image ;
}

} // namespace ImageToolBox
