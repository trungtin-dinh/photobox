#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Convertir une image en niveau de gris
Mat ImageMonochrome(const Mat image){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageGris ;
    Mat imageDouble ;

    image.convertTo(imageDouble, CV_32FC3) ;
    split(imageDouble, imageComposante) ;
    imageGris = imageComposante[0] ;
    for(c = 1 ; c < nbComposante ; c++) imageGris += imageComposante[c] ;
    imageGris = imageGris/3 ;
    imageGris.convertTo(imageGris, CV_8U) ;
    return imageGris ;
}

// Filtre sepia
Mat ImageSepia(const Mat image){
    int c, ligne, colonne ;
    int valMax = MaxImage(ImageMonochrome(image)) ;
    double val ;
    Mat imageResultanteComposante[3] ;
    Mat imageResultante = MonoCouleur(ImageMonochrome(image)) ;
    vector<Mat> imageResultanteComposanteSepia ;

    imageResultanteComposanteSepia.clear() ;
    split(imageResultante, imageResultanteComposante) ;

    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            val = (double)imageResultanteComposante[2].at<unsigned char>(ligne, colonne)/valMax*120 ;
            imageResultanteComposante[2].at<unsigned char>(ligne, colonne) = VerifierSaturation((int)val) ;
            val = (double)imageResultanteComposante[1].at<unsigned char>(ligne, colonne)/valMax*70 ;
            imageResultanteComposante[1].at<unsigned char>(ligne, colonne) = VerifierSaturation((int)val) ;
            val = (double)imageResultanteComposante[0].at<unsigned char>(ligne, colonne)/valMax*20 ;
            imageResultanteComposante[0].at<unsigned char>(ligne, colonne) = VerifierSaturation((int)val) ;
        }
    }

    for(c = 0 ; c < 3 ; c++) imageResultanteComposanteSepia.push_back(imageResultanteComposante[c]) ;
    merge(imageResultanteComposanteSepia, imageResultante) ;
    return imageResultante ;
}

// Inverser les intensites d'une composante
Mat ImageInversementMono(const Mat image){
    int ligne, colonne ;
    Mat imageInversee(image.size(), CV_8U) ;
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageInversee.at<unsigned char>(ligne, colonne) = 255 - image.at<unsigned char>(ligne, colonne) ;
        }
    }
    return imageInversee ;
}

// Inverser les intensites d'une image
Mat ImageInversement(const Mat image){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageInversee ;
    vector<Mat> imageInverseeComposante ;

    split(image, imageComposante) ;
    for(c = 0 ; c < nbComposante ; c++) imageInverseeComposante.push_back(ImageInversementMono(imageComposante[c])) ;
    merge(imageInverseeComposante, imageInversee) ;
    return imageInversee ;
}

// Extraire une composante de couleur (1=Bleu, 2=Vert, 3=Rouge)
Mat ImageExtractionCouleur(const Mat image, int choix){
    Mat imageComposante[3] ;
    split(image, imageComposante) ;
    return imageComposante[choix-1] ;
}

// Segmenter en rouge (annuler vert et bleu)
Mat ImageRouge(const Mat image){
    Mat imageResultante ;
    if(VerifierImage(image, imageResultante)){
        imageResultante = image ;
    }else{
        imageResultante = ImageSegmentation(image, {256,256,0}) ;
    }
    return imageResultante ;
}

// Segmenter en vert (annuler rouge et bleu)
Mat ImageVert(const Mat image){
    Mat imageResultante ;
    if(VerifierImage(image, imageResultante)){
        imageResultante = image ;
    }else{
        imageResultante = ImageSegmentation(image, {256,0,256}) ;
    }
    return imageResultante ;
}

// Segmenter en bleu (annuler rouge et vert)
Mat ImageBleu(const Mat image){
    Mat imageResultante ;
    if(VerifierImage(image, imageResultante)){
        imageResultante = image ;
    }else{
        imageResultante = ImageSegmentation(image, {0,256,256}) ;
    }
    return imageResultante ;
}

// Segmenter en jaune (conserver pixels ou R ≈ G)
Mat ImageJaune(const Mat image){
    int c, ligne, colonne ;
    int nbComposante = (int)image.channels() ;
    int val ;
    Mat imageComposante[nbComposante] ;
    Mat imageResultante ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;

    if(VerifierImage(image, imageResultante) == false){
        split(imageResultante, imageComposante) ;
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = abs((int)imageComposante[2].at<unsigned char>(ligne, colonne) - (int)imageComposante[1].at<unsigned char>(ligne, colonne)) ;
                if(val < 50){
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }else{
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }
            }
        }
        for(c = 0 ; c < nbComposante ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }
    return imageResultante ;
}

// Segmenter en cyan (conserver pixels ou G ≈ B)
Mat ImageCyan(const Mat image){
    int c, ligne, colonne ;
    int nbComposante = image.channels() ;
    int val ;
    Mat imageComposante[nbComposante] ;
    Mat imageResultante ;
    vector<Mat> imageResultanteComposante ;

    if(VerifierImage(image, imageResultante) == false){
        split(image, imageComposante) ;
        imageResultanteComposante.clear() ;
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = abs((int)imageComposante[1].at<unsigned char>(ligne, colonne) - (int)imageComposante[0].at<unsigned char>(ligne, colonne)) ;
                if(val < 50){
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                }else{
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }
            }
        }
        for(c = 0 ; c < nbComposante ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }
    return imageResultante ;
}

// Segmenter en magenta (conserver pixels ou R ≈ B)
Mat ImageMagenta(const Mat image){
    int c, ligne, colonne ;
    int nbComposante = image.channels() ;
    int val ;
    Mat imageComposante[nbComposante] ;
    Mat imageResultante ;
    vector<Mat> imageResultanteComposante ;

    if(VerifierImage(image, imageResultante) == false){
        split(image, imageComposante) ;
        imageResultanteComposante.clear() ;
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                val = abs((int)imageComposante[2].at<unsigned char>(ligne, colonne) - (int)imageComposante[0].at<unsigned char>(ligne, colonne)) ;
                if(val < 50){
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                }else{
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }
            }
        }
        for(c = 0 ; c < nbComposante ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }
    return imageResultante ;
}

// Filtre RGB : garder uniquement la composante dominante de chaque pixel
Mat ImageRGB(const Mat image){
    int c, ligne, colonne ;
    int nbComposante = image.channels() ;
    int rouge, vert, bleu ;
    Mat imageComposante[nbComposante] ;
    Mat imageResultante ;
    vector<int> cps ;
    vector<Mat> imageResultanteComposante ;

    if(VerifierImage(image, imageResultante) == false){
        split(image, imageComposante) ;
        imageResultanteComposante.clear() ;
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                cps.clear() ;
                rouge = (int)imageComposante[2].at<unsigned char>(ligne, colonne) ;
                vert  = (int)imageComposante[1].at<unsigned char>(ligne, colonne) ;
                bleu  = (int)imageComposante[0].at<unsigned char>(ligne, colonne) ;
                cps.push_back(bleu) ; cps.push_back(vert) ; cps.push_back(rouge) ;
                if((bleu != vert) || (bleu != rouge) || (vert != rouge)){
                    if(rouge == MaxVecteur(cps)){
                        imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                        imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                    }else if(vert == MaxVecteur(cps)){
                        imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                        imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                    }else{
                        imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                        imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    }
                }
            }
        }
        for(c = 0 ; c < nbComposante ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }
    return imageResultante ;
}

} // namespace ImageToolBox
