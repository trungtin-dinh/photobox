#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Quantification : réduction du nombre de niveaux d'intensité à 2^valeur niveaux
Mat ImageQuantification(const Mat image, const int valeur){
    int c, k, ligne, colonne ;
    int val = 1 ;
    int nbComposante = image.channels() ;
    Mat imageResultante ;
    Mat imageComposante[nbComposante] ;
    vector<int> niveau ;
    vector<Mat> imageResultanteComposante ;

    imageResultanteComposante.clear() ;
    niveau.clear() ;

    for(c = 1 ; c <= valeur ; c++) val *= 2 ;
    for(c = 0 ; c <= val ; c++) niveau.push_back((256/val)*c) ;

    split(image, imageComposante) ;

    for(k = 0 ; k < nbComposante ; k++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                if(imageComposante[k].at<unsigned char>(ligne, colonne) != 255){
                    for(c = 1 ; c < (int)niveau.size() ; c++){
                        if((imageComposante[k].at<unsigned char>(ligne, colonne) >= niveau[c-1]) &&
                           (imageComposante[k].at<unsigned char>(ligne, colonne) <  niveau[c+1])){
                            imageComposante[k].at<unsigned char>(ligne, colonne) = niveau[c-1] ;
                        }
                    }
                }
            }
        }
    }
    for(c = 0 ; c < nbComposante ; c++) imageResultanteComposante.push_back(imageComposante[c]) ;
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Sous-échantillonnage : réduction par un facteur 2*valeur
Mat ImageResolutionReduction(const Mat image, const int valeur){
    int c, ligne, colonne ;
    int minResolutionInit ;
    int pas = valeur*2 ;
    Size dim ;
    dim.height = (int)image.size().height/pas ;
    dim.width  = (int)image.size().width/pas ;
    Mat imageReduite[(int)image.channels()] ;
    Mat imageComposante[(int)image.channels()] ;
    Mat imageResultante(dim, CV_8UC3) ;
    vector<Mat> imageResultanteComposante ;

    minResolutionInit = ((int)image.size().height < (int)image.size().width) ? (int)image.size().height : (int)image.size().width ;
    if(minResolutionInit/pas == 0) return image ;

    split(image, imageComposante) ;
    split(imageResultante, imageReduite) ;

    for(c = 0 ; c < (int)image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height/pas ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width/pas ; colonne++){
                imageReduite[c].at<unsigned char>(ligne, colonne) = imageComposante[c].at<unsigned char>(ligne*pas, colonne*pas) ;
            }
        }
        imageResultanteComposante.push_back(imageReduite[c]) ;
    }
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Interpolation par pixel le plus proche (agrandissement par duplication)
Mat InterpolationPPP(const Mat image, const int valeur){
    int c, ligne, colonne, ligneInter, colonneInter ;
    int pas = valeur*2 ;
    Size dim ;
    dim.height = (int)image.size().height*pas ;
    dim.width  = (int)image.size().width*pas ;
    Mat imageInterpolee[(int)image.channels()] ;
    Mat imageComposante[(int)image.channels()] ;
    Mat imageResultante(dim, CV_8UC3) ;
    vector<Mat> imageResultanteComposante ;

    split(image, imageComposante) ;
    split(imageResultante, imageInterpolee) ;

    for(c = 0 ; c < (int)image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
                    for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                        imageInterpolee[c].at<unsigned char>(ligne*pas + ligneInter, colonne*pas + colonneInter) =
                            imageComposante[c].at<unsigned char>(ligne, colonne) ;
                    }
                }
            }
        }
        imageResultanteComposante.push_back(imageInterpolee[c]) ;
    }
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Interpolation bilinéaire sur tous les canaux
Mat InterpolationBilineaire(const Mat image, const int valeur){
    int c ;
    int pas = valeur*2 ;
    Size dim ;
    dim.height = (int)image.size().height*pas ;
    dim.width  = (int)image.size().width*pas ;
    Mat imageInterpolee[(int)image.channels()] ;
    Mat imageComposante[(int)image.channels()] ;
    Mat imageResultante(dim, CV_8UC3) ;
    vector<Mat> imageResultanteComposante ;

    split(image, imageComposante) ;
    split(imageResultante, imageInterpolee) ;

    for(c = 0 ; c < (int)image.channels() ; c++){
        imageInterpolee[c] = InterpolationBilineaireMono(imageComposante[c], valeur) ;
        imageResultanteComposante.push_back(imageInterpolee[c]) ;
    }
    merge(imageResultanteComposante, imageResultante) ;
    return imageResultante ;
}

// Interpolation bilinéaire sur une composante
// Pour chaque pixel interpolé (i,j), on projette horizontalement sur les lignes k et k+1 :
//   projHaut = ((pas-dj)/pas)*I(k,l) + (dj/pas)*I(k,l+1)
//   projBas  = ((pas-dj)/pas)*I(k+1,l) + (dj/pas)*I(k+1,l+1)
// puis on combine verticalement : pixel = (di/pas)*projHaut + ((pas-di)/pas)*projBas
Mat InterpolationBilineaireMono(const Mat image, const int valeur){
    int ligne, colonne, ligneInter, colonneInter ;
    int pas = valeur*2 ;
    int val ;
    double projectionHaut, projectionBas ;
    double coefHaut1, coefHaut2, coefBas1, coefBas2, coefMix1, coefMix2 ;
    Size dim ;
    dim.height = (int)image.size().height*pas ;
    dim.width  = (int)image.size().width*pas ;
    Mat imageResultante(dim, CV_8U) ;

    for(ligne = 0 ; ligne < (int)image.size().height - 1 ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width - 1 ; colonne++){
            for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
                for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                    coefHaut1 = (double)(pas - colonneInter)/pas ;
                    coefHaut2 = (double)colonneInter/pas ;
                    projectionHaut = coefHaut1*image.at<unsigned char>(ligne,   colonne) + coefHaut2*image.at<unsigned char>(ligne,   colonne+1) ;
                    coefBas1 = (double)(pas - colonneInter)/pas ;
                    coefBas2 = (double)colonneInter/pas ;
                    projectionBas  = coefBas1*image.at<unsigned char>(ligne+1, colonne) + coefBas2*image.at<unsigned char>(ligne+1, colonne+1) ;
                    coefMix1 = (double)ligneInter/pas ;
                    coefMix2 = (double)(pas - ligneInter)/pas ;
                    val = coefMix1*projectionHaut + coefMix2*projectionBas ;
                    imageResultante.at<unsigned char>(ligne*pas + ligneInter, colonne*pas + colonneInter) = (unsigned char)val ;
                }
            }
        }
    }
    // Derniere colonne
    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
            for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                imageResultante.at<unsigned char>(ligne*pas + ligneInter, ((int)image.size().width-1)*pas + colonneInter) =
                    image.at<unsigned char>(ligne, (int)image.size().width) ;
            }
        }
    }
    // Derniere ligne
    for(colonne = 0 ; colonne < (int)image.size().width - 1 ; colonne++){
        for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
            for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                imageResultante.at<unsigned char>(((int)image.size().height-1)*pas + ligneInter, colonne*pas + colonneInter) =
                    image.at<unsigned char>((int)image.size().height, colonne) ;
            }
        }
    }
    return imageResultante ;
}

} // namespace ImageToolBox
