#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

//////////////////// Traitement de couleur ////////////////////

// Modifier la temperature de l'image
Mat ImageTemperature(const Mat image, const int valeur){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int val ;                                   // Valeur de calcul intermediaire
    Mat imageResultante ;                       // Image resultante
    Mat imageComposante[3] ;                    // Composantes de l'image originale
    vector<Mat> imageResultanteComposante ;     // Composante de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    //  Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Si temperature chaude
    if(valeur > 0){
        // Augmenter l'intensite des canaux rouge et vert
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Canal rouge
                // Calcul pour verifier la saturation
                val = VerifierSaturation(imageComposante[2].at<unsigned char>(ligne, colonne) + valeur) ;
                imageComposante[2].at<unsigned char>(ligne, colonne) = val ;

                // Canal vert
                // Calcul pour verifier la saturation
                val = VerifierSaturation(imageComposante[1].at<unsigned char>(ligne, colonne) + valeur) ;
                imageComposante[1].at<unsigned char>(ligne, colonne) = val ;
            }
        }
    // Si temperature froide
    }else{
        // Augmenter les canaux bleu et vert
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Canal bleu
                // Calcul pour verifier la saturation
                val = VerifierSaturation(imageComposante[0].at<unsigned char>(ligne, colonne) + abs(valeur)) ;
                imageComposante[0].at<unsigned char>(ligne, colonne) = val ;

                // Canal vert
                // Calcul pour verifier la saturation
                val = VerifierSaturation(imageComposante[1].at<unsigned char>(ligne, colonne) + abs(valeur)) ;
                imageComposante[1].at<unsigned char>(ligne, colonne) = val ;
            }
        }
    }

    // Affceter les composante de l'image traitee
    for(c = 0 ; c < 3 ; c++){
        imageResultanteComposante.push_back(imageComposante[c])  ;
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

// Modifier la saturation
Mat ImageSaturation(const Mat image, const int valeur){
    // Declaration des variables
    Mat imageHSV = image ;                          // Image originale en HSV
    int c ;                                         // Indices
    Mat imageResultante ;                           // Image resultante
    Mat imageComposante[3] ;                        // Composantes de l'image originale
    vector<Mat> imageResultanteComposante ;         // Composante de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    // Convertir l'image originale en HSV
    cvtColor(imageHSV, imageHSV, COLOR_BGR2HSV) ;

    //  Decomposition des composantes
    split(imageHSV, imageComposante) ;

    // Si il n'ya aucune modification ou si l'image est en niveau de gris
    if(VerifierImage(image, imageResultante) == false){
        imageComposante[1] = ImageSaturationElement(imageComposante[1], valeur) ;
        // Affecter les composante de l'image traitee
        for(c = 0 ; c < 3 ; c++){
            imageResultanteComposante.push_back(imageComposante[c])  ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageHSV) ;

        // Convertir l'image traitee en RGB
        cvtColor(imageHSV, imageResultante, COLOR_HSV2BGR) ;
    }

    // Retour
    return imageResultante ;
}

// Calcul elementaire pour la modification la saturation
Mat ImageSaturationElement(const Mat image, const int valeur){
    // Declaration des variables
    int ligne, colonne ;            // Indices
    int val ;                       // Valeur de calcul intermediaire
    Mat imageResultante = image ;   // Resultat

    // Calculs
    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            // Calcul pour verifier la saturation
            val = VerifierSaturation(imageResultante.at<unsigned char>(ligne, colonne) + valeur) ;
            imageResultante.at<unsigned char>(ligne, colonne) = val ;
        }
    }

    // Retour
    return imageResultante ;
}

// Modifier la teinte
Mat ImageTeinte(const Mat image, const int valeur){
    // Declaration des variables
    Mat imageHSV ;                                  // Convertir l'image originale en HSV
    int c, ligne, colonne ;                         // Indices
    int val ;                                       // Valeur de calcul intermediaire
    Mat imageResultante ;                           // Image resultante
    Mat imageComposante[3] ;                        // Composantes de l'image originale
    vector<Mat> imageResultanteComposante ;         // Composante de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    // Convertir l'image originale en HSV
    cvtColor(image, imageHSV, COLOR_BGR2HSV) ;

    //  Decomposition des composantes
    split(imageHSV, imageComposante) ;

    // Modifier la teinte
    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            val = (double)imageComposante[0].at<unsigned char>(ligne, colonne) + valeur ;
            imageComposante[0].at<unsigned char>(ligne, colonne) = (int)val%255 ;
        }
    }
    // Affecter les composante de l'image traitee
    for(c = 0 ; c < 3 ; c++){
        imageResultanteComposante.push_back(imageComposante[c])  ;
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageHSV) ;

    // Convertir l'image traitee en RGB
    cvtColor(imageHSV, imageResultante, COLOR_HSV2BGR) ;

    // Retour
    return imageResultante ;
}

// Modifier la vividite
Mat ImageVividite(const Mat image, const int valeur){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int maxR, maxV, maxB ;                      // Valeurs maximum des composantes
    int minR, minV, minB ;                      // Valeurs minimum des composantes
    int val ;                                   // Valeur de calcul intermediaire
    Mat imageResultante ;                       // Image resultante
    Mat imageComposante[3] ;                    // Composantes de l'image originale
    vector<int> vectVal(3, 0) ;                 // Vecteur des valeurs a ajouter dans les composantes
    vector<int> intervalle ;                    // Intervalle d'intensite de l'histogramme des composantes
    vector<Mat> imageResultanteComposante ;     // Composante de l'image resultante

    // Initialisation
    intervalle.reserve(3) ;
    imageResultanteComposante.reserve(3) ;

    //  Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Determiner les intervalles d'intensite des composantes
    // Bleu
    maxB = MaxImage(imageComposante[0]) ;
    minB = MinImage(imageComposante[0]) ;
    intervalle.push_back(maxB - minB) ;
    // Vert
    maxV = MaxImage(imageComposante[1]) ;
    minV = MinImage(imageComposante[1]) ;
    intervalle.push_back(maxV - minV) ;
    // Rouge
    maxR = MaxImage(imageComposante[2]) ;
    minR = MinImage(imageComposante[2]) ;
    intervalle.push_back(maxR - minR) ;

    if((intervalle[0] != intervalle[1]) || (intervalle[0] != intervalle[2]) || (intervalle[2] != intervalle[1])){
        // Determiner l'intervalle la plus petite
        val = MinVecteur(intervalle) ;

        // Affecter les valeurs aux composantes
        for(c = 0 ; c < (int)intervalle.size() ; c++){
            if(intervalle[c] == val){
                vectVal[c] = valeur ;
            }else{
                vectVal[c] = valeur/3 ;
            }
        }
    }

    // Modifier la vividite
    if(VerifierImage(image, imageResultante) == false){
        for(c = 0 ; c < 3 ; c++){
            // Augmenter l'intensite du canal non sature
            for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
                for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                    val = VerifierSaturation((int)imageComposante[c].at<unsigned char>(ligne, colonne) + vectVal[c]) ;
                    imageComposante[c].at<unsigned char>(ligne, colonne) = val ;
                }
            }
        }

        // Affceter les composante de l'image traitee
        for(c = 0 ; c < 3 ; c++){
            imageResultanteComposante.push_back(imageComposante[c])  ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = image ;
    }

    // Retour
    return imageResultante ;
}

//////////////////// Filtre de couleur ////////////////////

// Convertir une image en niveau de gris
Mat ImageMonochrome(const Mat image){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageGris ;                             // Image resultante
    Mat imageDouble ;                           // Image convertie en type double

    // Convertion en double
    image.convertTo(imageDouble, CV_32FC3) ;

    // Decomposition des composantes de couleur
    split(imageDouble, imageComposante) ;

    // Initialisation
    imageGris = imageComposante[0] ;

    // Calculs
    for(c = 1 ; c < nbComposante ; c++){
        imageGris += imageComposante[c] ;
    }
    imageGris = imageGris/3 ;

    // Convertion en unsigned char
    imageGris.convertTo(imageGris, CV_8U) ;

    // Retour
    return imageGris ;
}

// Sepia
Mat ImageSepia(const Mat image){
    // Declaration des variables
    int c, ligne, colonne ;                                         // Indices
    int valMax = MaxImage(ImageMonochrome(image)) ;                 // Valeur maximum dans l'image au niveau de gris
    double val ;                                                    // Valeur intermediaire
    Mat imageResultanteComposante[3] ;                              // Composantes de l'image resultante
    Mat imageResultante = MonoCouleur(ImageMonochrome(image)) ;     // Image resultante
    vector<Mat> imageResultanteComposanteSepia ;                    // Composantes de l'image resultante apres le traitement

    // Initialisation
    imageResultanteComposanteSepia.clear() ;

    // Decomposition des composantes de couleur
    split(imageResultante, imageResultanteComposante) ;

    // Sepia
    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            // Composante rouge
            // Verifier la saturation
            val = (double)imageResultanteComposante[2].at<unsigned char>(ligne, colonne)/valMax*120 ;
            imageResultanteComposante[2].at<unsigned char>(ligne, colonne) = VerifierSaturation((int)val) ;

            // Composante verte
            // Verifier la saturation
            val = (double)imageResultanteComposante[1].at<unsigned char>(ligne, colonne)/valMax*70 ;
            imageResultanteComposante[1].at<unsigned char>(ligne, colonne) = VerifierSaturation((int)val) ;

            // Composante bleue
            // Verifier la saturation
            val = (double)imageResultanteComposante[0].at<unsigned char>(ligne, colonne)/valMax*20 ;
            imageResultanteComposante[0].at<unsigned char>(ligne, colonne) = VerifierSaturation((int)val) ;
        }
    }

    // Affceter les composante de l'image traitee
    for(c = 0 ; c < 3 ; c++){
        imageResultanteComposanteSepia.push_back(imageResultanteComposante[c])  ;
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposanteSepia, imageResultante) ;

    // Retour
    return imageResultante ;
}

//Inverser les intensites d'une composante de l'image
Mat ImageInversementMono(const Mat image){
    // Declaration des variables
    int ligne, colonne ;
    Mat imageInversee(image.size(), CV_8U) ;

    // Inversement
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageInversee.at<unsigned char>(ligne, colonne) = 255 - image.at<unsigned char>(ligne, colonne) ;
        }
    }

    // Retour
    return imageInversee ;
}

// Inverser les intensites d'une image
Mat ImageInversement(const Mat image){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageInversee ;                         // Image resultante
    vector<Mat> imageInverseeComposante ;       // Des composantes de l'image resultante

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Inverser sur chaque canal de couleur
    for(c = 0 ; c < nbComposante ; c++){
        imageInverseeComposante.push_back(ImageInversementMono(imageComposante[c]))  ;
    }

    // Fusion des composantes de couleur
    merge(imageInverseeComposante, imageInversee) ;

    // Retour
    return imageInversee ;
}

// Extraction une composante de couleur de l'image
Mat ImageExtractionCouleur(const Mat image, int choix){
    // Declaration de variable
    Mat imageComposante[3] ;         // Des composantes de l'image originale
    Mat imageResultante ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;
    imageResultante = imageComposante[choix-1] ;
    // Retour
    return imageResultante ;
}

// Extraire la couleur rouge
Mat ImageRouge(const Mat image){
    // Declaration de variable
    Mat imageResultante ;   // Image resultante

    // Si l'image est en niveau de gris
    if(VerifierImage(image, imageResultante)){
        imageResultante = image ;
    // Si 'limage est en couleurs
    }else{
        imageResultante = ImageSegmentation(image, {256,256,0}) ;
    }

    // Retour
    return imageResultante ;
}

// Extraire la couleur verte
Mat ImageVert(const Mat image){
    // Declaration de variable
    Mat imageResultante ;   // Image resultante

    // Si l'image est en niveau de gris
    if(VerifierImage(image, imageResultante)){
        imageResultante = image ;
    // Si 'limage est en couleurs
    }else{
        imageResultante = ImageSegmentation(image, {256,0,256}) ;
    }

    // Retour
    return imageResultante ;
}

// Extraire la couleur bleue
Mat ImageBleu(const Mat image){
    // Declaration de variable
    Mat imageResultante ;   // Image resultante

    // Si l'image est en niveau de gris
    if(VerifierImage(image, imageResultante)){
        imageResultante = image ;
    // Si 'limage est en couleurs
    }else{
        imageResultante = ImageSegmentation(image, {0,256,256}) ;
    }

    // Retour
    return imageResultante ;
}

// Extraire la couleur jaune
Mat ImageJaune(const Mat image){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int nbComposante = (int)image.channels() ;  // Nombre de composantes de couleur
    int val ;                                   // Valeur de calcul intermediaire
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageResultante ;                       // Image resultante
    vector<Mat> imageResultanteComposante ;     // Des composantes de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    if(VerifierImage(image, imageResultante) == false){
        // Decomposition des composantes de couleur
        split(imageResultante, imageComposante) ;

        // Ne garder que la couleur jaune
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Calculer la difference entre la composante rouge et verte
                val = abs((int)imageComposante[2].at<unsigned char>(ligne, colonne) - (int)imageComposante[1].at<unsigned char>(ligne, colonne)) ;
                // Si la difference est faible : couleur jaune
                if(val < 50){
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }else{
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }
            }
        }

        // Vecteur des composantes de l'image resultante
        for(c = 0 ; c < nbComposante ; c++){
            imageResultanteComposante.push_back(imageComposante[c])  ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }

    // Retour
    return imageResultante ;
}

// Extraire la couleur cyane
Mat ImageCyan(const Mat image){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    int val ;                                   // Valeur de calcul intermediaire
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageResultante ;                       // Image resultante
    vector<Mat> imageResultanteComposante ;     // Des composantes de l'image resultante

    if(VerifierImage(image, imageResultante) == false){
        // Decomposition des composantes de couleur
        split(image, imageComposante) ;

        // Initialisation
        imageResultanteComposante.clear() ;

        // Ne garder que la couleur cyane
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Calculer la difference entre la composante verte et bleu
                val = abs((int)imageComposante[1].at<unsigned char>(ligne, colonne) - (int)imageComposante[0].at<unsigned char>(ligne, colonne)) ;
                // Si la difference est faible : couleur cyane
                if(val < 50){
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                }else{
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }
            }
        }

        // Vecteur des composantes de l'image resultante
        for(c = 0 ; c < nbComposante ; c++){
            imageResultanteComposante.push_back(imageComposante[c])  ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }

    // Retour
    return imageResultante ;
}

// Extraire la couleur magenta
Mat ImageMagenta(const Mat image){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    int val ;                                   // Valeur de calcul intermediaire
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageResultante ;                       // Image resultante
    vector<Mat> imageResultanteComposante ;     // Des composantes de l'image resultante

    if(VerifierImage(image, imageResultante) == false){
        // Decomposition des composantes de couleur
        split(image, imageComposante) ;

        // Initialisation
        imageResultanteComposante.clear() ;

        // Ne garder que la couleur magenta
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Calculer la difference entre la composante rouge et bleu
                val = abs((int)imageComposante[2].at<unsigned char>(ligne, colonne) - (int)imageComposante[0].at<unsigned char>(ligne, colonne)) ;
                // Si la difference est faible : couleur magenta
                if(val < 50){
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                }else{
                    imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;
                    imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;
                }
            }
        }

        // Vecteur des composantes de l'image resultante
        for(c = 0 ; c < nbComposante ; c++){
            imageResultanteComposante.push_back(imageComposante[c])  ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }

    // Retour
    return imageResultante ;
}

// Filtre RGB
Mat ImageRGB(const Mat image){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    int rouge, vert, bleu ;                     // Valeurs de calcul intermediaires
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageResultante ;                       // Image resultante
    vector<int> cps ;                           // Vecteur des composantes d'un pixel
    vector<Mat> imageResultanteComposante ;     // Des composantes de l'image resultante

    // Si l'image est en niveau de gris
    if(VerifierImage(image, imageResultante) == false){
        // Decomposition des composantes de couleur
        split(image, imageComposante) ;

        // Initialisation
        imageResultanteComposante.clear() ;

        // Determiner la composante maximum parmi les composantes existantes
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Affecter les valeurs des composantes du pixel sur les variables intermediaires
                cps.clear() ;
                rouge = (int)imageComposante[2].at<unsigned char>(ligne, colonne) ;      // Rouge
                vert = (int)imageComposante[1].at<unsigned char>(ligne, colonne) ;       // Vert
                bleu = (int)imageComposante[0].at<unsigned char>(ligne, colonne) ;       // Bleu
                cps.push_back(bleu) ;
                cps.push_back(vert) ;
                cps.push_back(rouge) ;
                // Si les trois composantes sont differentes
                if((bleu != vert) || (bleu != rouge) || (vert != rouge)){
                    // Si la couleur la plus intense est le rouge
                    if(rouge == MaxVecteur(cps)){
                        imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;      // Enlever le vert
                        imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;      // Enlever le bleu
                    // Si la couleur la plus intense est le vert
                    }else if(vert == MaxVecteur(cps)){
                        imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;      // Enlever le rouge
                        imageComposante[0].at<unsigned char>(ligne, colonne) = 0 ;      // Enlever le bleu
                    // Si la couleur la plus intense est le bleu
                    }else{
                        imageComposante[2].at<unsigned char>(ligne, colonne) = 0 ;      // Enlever le rouge
                        imageComposante[1].at<unsigned char>(ligne, colonne) = 0 ;      // Enlever le vert
                    }
                }
            }
        }

        // Vecteur des composantes de l'image resultante
        for(c = 0 ; c < nbComposante ; c++){
            imageResultanteComposante.push_back(imageComposante[c])  ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageResultante) ;
    }else{
        imageResultante = MonoCouleur(imageResultante) ;
    }

    // Retour
    return imageResultante ;
}

//////////////////// Traitement a base d'histogramme ////////////////////

// Normalisation de l'histogramme d'une composante de l'image
Mat NormalisationMono(const Mat image, const int valeur){
    // Declaration des variables
    int ligne, colonne ;                                // Indices
    int min = MinImage(image) ;                         // Valeur minimum l'image de depart
    int max = MaxImage(image) ;                         // Valeur maximum l'image de depart
    int minNormalisation = min - valeur ;               // Valeur minimum de l'image normalisee
    int maxNormalisation = max + valeur ;               // Valeur maximum de l'image normalisee
    double val ;                                        // Valeur intermediaire
    Mat imageNormalisation(image.size(), CV_8U) ;       // Image resultante

    // Verifier les valeurs des extremites de l'intervalle de sortie
    // Valeur minimum
    if(minNormalisation < 0){
        minNormalisation = 0 ;
    }else if(minNormalisation > 127){
        minNormalisation = 127 ;
    }
    // Valeur maximum
    if(maxNormalisation > 255){
        maxNormalisation = 255 ;
    }else if(maxNormalisation < 127){
        maxNormalisation = 127 ;
    }

    // Normalisation
    val = (double)(maxNormalisation - minNormalisation)/(max - min) ;
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageNormalisation.at<unsigned char>(ligne, colonne) = (double)(image.at<unsigned char>(ligne, colonne) - min)*val + minNormalisation ;
        }
    }

    // Retour
    return imageNormalisation ;
}

// Normalisation de l'histogramme d'une image avec valeur reglable
Mat Normalisation(const Mat image, const int valeur){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageNormalisation ;                    // Image resultante
    vector<Mat> imageNormalisationComposante ;  // Des composantes de l'image resultante

    // Initialisation
    imageNormalisationComposante.clear() ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Normalisation sur chaque canal de couleur
    for(c = 0 ; c < nbComposante ; c++){
        imageNormalisationComposante.push_back(NormalisationMono(imageComposante[c], valeur))  ;
    }

    // Fusion des composantes de couleur
    merge(imageNormalisationComposante, imageNormalisation) ;

    // Retour
    return imageNormalisation ;
}

// Egalisation de l'histogramme d'une composante de l'image avec valeur reglable
Mat ImageEgalisationMono(const Mat image){
    // Declaration des variables
    int ligne, colonne ;                                    // Indices
    vector<int> valeurEgal = Egalisation(image, 255) ;      // Histogramme egalise
    Mat imageEgalisation(image.size(), CV_8U) ;             // Image resultante

    // Remplacer les valeur d'intensite de chaque pixel par sa valeur egalisee
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageEgalisation.at<unsigned char>(ligne, colonne) = valeurEgal[(int)image.at<unsigned char>(ligne, colonne)] ;
        }
    }

    // Retour
    return imageEgalisation ;
}

// Egalisation d'une image en couleurs
Mat ImageEgalisation(const Mat image){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageEgalisation ;                      // Image resultante
    vector<Mat> imageEgalisationComposante ;    // Des composantes de l'image resultante


    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    if(VerifierImage(image, imageEgalisation)){
        return MonoCouleur(ImageEgalisationMono(imageComposante[0])) ;
    }else{
        // Egalisation sur chaque canal de couleur
        for(c = 0 ; c < nbComposante ; c++){
            imageEgalisationComposante.push_back(ImageEgalisationMono(imageComposante[c]))  ;
        }

        // Fusion des composantes de couleur
        merge(imageEgalisationComposante, imageEgalisation) ;

        // Retour
        return imageEgalisation ;
    }
}

// Modifier la luminosite de l'image
Mat ImageLuminosite(const Mat image, const int valeur){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int intensite ;                             // Intensite lumineuse
    Mat imageModif ;                            // Image modifiee
    Mat imageComposante[image.channels()] ;     // Composantes de l'image originale
    vector<Mat> imageModifComposante ;          // Composantes de l'image resultante

    // Initialisation
    imageModifComposante.clear() ;
    split(image, imageComposante) ;

    // Modifier la luminosite de l'image
    for(c = 0 ; c < image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                intensite = (int)imageComposante[c].at<unsigned char>(ligne, colonne) + valeur ;
                imageComposante[c].at<unsigned char>(ligne, colonne) = VerifierSaturation(intensite) ;
            }
        }
        imageModifComposante.push_back(imageComposante[c]) ;
    }

    // Fusion des composantes de couleur
    merge(imageModifComposante, imageModif) ;

    // Retour
    return imageModif ;
}

// Modifier les ombres
Mat ImageOmbre(const Mat image, int valeur){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int intensite ;                             // Intensite lumineuse
    Mat imageModif ;                            // Image modifiee
    Mat imageComposante[image.channels()] ;     // Composantes de l'image originale
    vector<Mat> imageModifComposante ;          // Composantes de l'image resultante

    // Initialisation
    imageModifComposante.clear() ;
    split(image, imageComposante) ;

    // Modifier la luminosite de l'image
    for(c = 0 ; c < image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Si la valeur se trouve dans la zone des basses intensites
                if(imageComposante[c].at<unsigned char>(ligne, colonne) < 128){
                    intensite = (int)imageComposante[c].at<unsigned char>(ligne, colonne) + valeur ;
                    imageComposante[c].at<unsigned char>(ligne, colonne) = VerifierSaturation(intensite) ;
                }
            }
        }
        imageModifComposante.push_back(imageComposante[c]) ;
    }

    // Fusion des composantes de couleur
    merge(imageModifComposante, imageModif) ;

    // Retour
    return imageModif ;
}

// Modifier les hautes mimieres
Mat ImageHauteLumiere(const Mat image, int valeur){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    int intensite ;                             // Intensite lumineuse
    Mat imageModif ;                            // Image modifiee
    Mat imageComposante[image.channels()] ;     // Composantes de l'image originale
    vector<Mat> imageModifComposante ;          // Composantes de l'image resultante

    // Initialisation
    imageModifComposante.clear() ;
    split(image, imageComposante) ;

    // Modifier la luminosite de l'image
    for(c = 0 ; c < image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Si la valeur se trouve dans la zone des basses intensites
                if(imageComposante[c].at<unsigned char>(ligne, colonne) >= 128){
                    intensite = (int)imageComposante[c].at<unsigned char>(ligne, colonne) + valeur ;
                    imageComposante[c].at<unsigned char>(ligne, colonne) = VerifierSaturation(intensite) ; 
                }
            }
        }
        imageModifComposante.push_back(imageComposante[c]) ;
    }

    // Fusion des composantes de couleur
    merge(imageModifComposante, imageModif) ;

    // Retour
    return imageModif ;
}

//////////////////// Debruitage ////////////////////

// Filtrer par filtre median pour une composante de l'image 
Mat ImageMedianMono(const Mat image){
    // Declaraton des variables
    int ligne, colonne, lig, col ;                      // Indices
    int taille = 3 ;                                    // Taille du filtre median
    Mat matrice = ImageZero(taille, taille) ;           // Matrice intermediaire
    Mat imageMiroir = ImageMiroir(image, taille) ;      // Image miroir
    Mat imageMedian(image.size(), CV_8U) ;              // Image resultante

    // Appliquer le filtre median
    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            // Matrice intermediaire
            for(lig = 0 ; lig < taille ; lig++){
                for(col = 0 ; col < 3 ; col++){
                    matrice.at<unsigned char>(lig, col) = imageMiroir.at<unsigned char>(ligne + lig, colonne + col) ;
                }
            }
            imageMedian.at<unsigned char>(ligne, colonne) = MatriceMedian(matrice) ;
        }
    }

    // Retour
    return imageMedian ;
}

// Filtrer l'image par filtre median 
Mat ImageMedian(const Mat image){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageMedian ;                           // Image resultante
    vector<Mat> imageMedianComposante ;         // Des composantes de l'image resultante

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Appliquer le filtre median sur chaque canal de couleur
    for(c = 0 ; c < nbComposante ; c++){
        imageMedianComposante.push_back(ImageMedianMono(imageComposante[c]))  ;
    }

    // Fusion des composantes de couleur
    merge(imageMedianComposante, imageMedian) ;

    // Retour
    return imageMedian ;
}

// Filtrage
Mat ImageFiltrage(const Mat image, const int choix){
    switch(choix){
        // Filtre moyenneur
        case 1 :
            return ImageConvolution(image, GenererFiltre(1)) ;
            break ;
        // Filtre gaussien
        case 2 :
            return ImageConvolution(image, GenererFiltre(3)) ;
            break ;
        // Filtre median
        case 3 :
            return ImageMedian(image) ;
            break ;
        default :
            return ImageConvolution(image, GenererFiltre(3)) ;
            break ;
    }
}

// Lisser l'image
Mat ImageLissage(const Mat image, const int valeur){
    // Declaration des variables
    int c ;                         // Indice
    Mat imageResultante = image ;   // Image resultante

    // Lisser l'image
    for(c = 0 ; c < valeur ; c++){
        // imageResultante = ImageFiltrage(imageResultante, 1) ;
        imageResultante = ImageConvolution(imageResultante, GenererFiltre(3)) ;
    }

    // Retour
    return imageResultante ;
}

// Filtrer par filtre Kuwahara-Nagao sur une composante de l'image
Mat ImageKuwaharaMono(const Mat image){
    // Declaration des variables
    int ligne, colonne ;                        // Indices
    int indiceMin ;                             // Indice de la valeur minimum
    int val ;                                   // Valeur de calcul intermediaire
    Mat imageMiroir  = ImageMiroir(image, 5) ;  // Image originale avec effet de miroir
    Mat imageResultante = image ;               // Image resultante
    vector<double> moyenne(9) ;                 // Vecteur des moyennes des domaines
    vector<double> variance(9) ;                // Vecteur des variances des domaines
    vector<double> domaine(9) ;                 // Vecteur des pixels d'un domaine

    for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
            // Initialisation
            moyenne.clear() ;
            variance.clear() ;

            // Domaine 1
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2))  ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+4)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 2
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 3
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+3)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 4
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 5
            // Recuperer les pixels du domaine
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
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 6
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 7
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+4)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+3)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+4)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 8
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+3, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+4, colonne+1)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Domaine 9
            // Recuperer les pixels du domaine
            domaine.clear() ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+1, colonne+2)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+1)) ;
            domaine.push_back((double)imageMiroir.at<unsigned char>(ligne+2, colonne+2)) ;
            domaine.push_back(0) ;
            domaine.push_back(0) ;
            // Calculer la moyenne
            moyenne.push_back(MoyenneVecteur(domaine)) ;
            // Calculer la variance
            variance.push_back(VarianceVecteur(domaine)) ;

            // Determiner le domaine ayant la plus faible variance
            indiceMin = MinVecteurIndice(variance) ;
            val = (int)moyenne[indiceMin] ;
            imageResultante.at<unsigned char>(ligne, colonne) = VerifierSaturation(val) ;
        }
    }

    // Retour
    return imageResultante ;
}

// Filtrer l'image par filtre Kuwahara-Nagao
Mat ImageKuwahara(const Mat image){
    // Declaration des variables
    int c ;                                         // Indice
    Mat imageComposante[(int)image.channels()] ;    // Des composantes de l'image originale
    Mat imageResultante ;                           // Image resultante
    vector<Mat> imageResultanteComposante ;         // Des composantes de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Filtrage
    for(c = 0 ; c < (int)image.channels() ; c++){
        imageResultanteComposante.push_back(ImageKuwaharaMono(imageComposante[c]));
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

//////////////////// Bruitage ////////////////////

// Ajouter du bruit gaussien sur l'image
Mat ImageBruitGaussien(const Mat image, const double moyenne, const double sigma){
    // Declaration des variables
    int c, ligne, colonne ;                         // Indices
    int val ;                                       // Valeur de calcul intermediaire
    Mat imageComposante[(int)image.channels()] ;    // Des composantes de l'image originale
    Mat imageBruit(image.size(), CV_8U) ;           // Image du bruit gaussien
    Mat imageResultante ;                           // Image resultante
    vector<Mat> imageResultanteComposante ;         // Des composantes de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    // Calculer l'image du bruit gaussien
    randn(imageBruit, moyenne, sigma) ;

    // Decomposition des composantes de couleur
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

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

// Ajouter du bruit poivre et sel dans l'image
Mat ImageBruitPoivreSel(const Mat image){
    // Declaration des variables
    int c, ligne, colonne ;                         // Indices
    int val ;                                       // Valeur de calcul intermediaire
    Mat imageComposante[(int)image.channels()] ;    // Des composantes de l'image originale
    Mat imageBruitSel(image.size(), CV_8U) ;        // Image du bruit "sel"
    Mat imageBruitPoivre(image.size(), CV_8U) ;     // Image du bruit "poivre"
    Mat imageResultante ;                           // Image resultante
    vector<Mat> imageResultanteComposante ;         // Des composantes de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;

    // Creer l'image du bruit poivre et sel
    // Sel
    randn(imageBruitSel, 75, 20) ;
    imageBruitSel = ImageSeuillage(imageBruitSel, 127) ;
    // Poivre
    randn(imageBruitPoivre, 75, 20) ;
    imageBruitPoivre = ImageSeuillage(imageBruitPoivre, 127) ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Ajouter du bruit dans l'image
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

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

//////////////////// Contours ////////////////////

// Detection de contours par filtres gradients
Mat ImageContourGradient(const Mat image){
    // Declaration des variables
    int nbComposante = image.channels() ;                   // Nombre de composantes de couleur de l'image originale
    Mat gradientX, gradientY ;                              // Gradients en x et en y
    Mat imageContour(image.size(), CV_8U) ;                 // Image resultante

    // Verifier si l'image de depart est en niveau de gris
    if(nbComposante > 1){
        imageContour = ImageMonochrome(image) ;
    }

    // Calculer des gradients
    gradientX = GradientX(imageContour, "Simple") ;          // En x
    gradientY = GradientY(imageContour, "Simple") ;          // En y

    // Norme du gradient
    imageContour = MatriceNorme(gradientX, gradientY) ;
    // imageContour = ImageSeuillage(MatriceNorme(gradientX, gradientY), 250) ;

    // Maximum de la norme du gradient dans la direction du gradient
    //imageContour = MaxNormeGradient(ImageSeuillage(MatriceNorme(gradientX, gradientY), 250), gradientX, gradientY) ;
    // Retour
    return imageContour ;
}

// Detection de contours par filtre laplacien
Mat ImageContourLaplace(const Mat image){
    return ImageConvolution(ImageMonochrome(image), GenererFiltre(2)) ;
}

// Rehaussement de contours
Mat ImageRehaussementContour(const Mat image, const int val){
    // Declaration de variables
    int c ;                                             // Indice
    int nbComposante = image.channels() ;               // Nombre de composantes de couleurs de l'image originale
    Mat imageComposante[nbComposante] ;                 // Composantes de couleurs de l'image originale
    Mat imageContour = ImageContourLaplace(image) ;     // Image de contours
    Mat imageRehaussement ;                             // Image resultante
    vector<Mat> imageRehaussementComposante ;           // Composantes de couleurs de l'image resultante

    imageContour = imageContour/255 ;

    if(nbComposante == 1){
        imageRehaussement = image + imageContour*val ;
    }else{
        split(image, imageComposante) ;
        for(c = 0 ; c < nbComposante ; c++){
            imageRehaussementComposante.push_back(imageComposante[c] + imageContour*val) ;
        }
        merge(imageRehaussementComposante, imageRehaussement) ;
    }

    // Retour
    return imageRehaussement ;
}

//////////////////// Seuillage ////////////////////

// Seuillage simple d'une composante de l'image 
Mat ImageSeuillage(const Mat image, const int seuil){
    // Declaration des ariables
    int ligne, colonne ;                        // Indice
    Mat imageSeuil(image.size(), CV_8U) ;       // Image resultante

    // Seuillage
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            if((int)image.at<unsigned char>(ligne, colonne) < seuil){
                imageSeuil.at<unsigned char>(ligne, colonne) = 0 ;
            }else{
                imageSeuil.at<unsigned char>(ligne, colonne) = 255 ;
            }
        }
    }

    // Retour
    return imageSeuil ;
}

// Seuillage simple d'une image
Mat ImageSeuillage(const Mat image, vector<int> seuil){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageSeuillage ;                        // Image resultante
    vector<Mat> imageSeuillageComposante ;      // Des composantes de l'image resultante

    // Initialisation
    imageSeuillageComposante.clear() ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Seuillage sur chaque canal de couleur
    for(c = 0 ; c < nbComposante ; c++){
        imageSeuillageComposante.push_back(ImageSeuillage(imageComposante[c], seuil[c]))  ;
    }

    // Fusion des composantes de couleur
    merge(imageSeuillageComposante, imageSeuillage) ;

    // Retour
    return imageSeuillage ;
}

// Seuillage par hysteresis d'une composante de l'image
Mat ImageSeuillage(const Mat image, const int seuilBas, const int seuilHaut){
    return (ImageSeuillage(image, seuilBas) - ImageSeuillage(image, seuilHaut)) ;
}

// Seuillage par hysteresis d'une image 
Mat ImageSeuillage(const Mat image, vector<int> seuilBas, vector<int> seuilHaut){
    return(ImageSeuillage(image, seuilBas) - ImageSeuillage(image, seuilHaut)) ;
}

//////////////////// Segmentation ////////////////////

// Segmentation d'une composante de l'image epar seuillage simple
Mat ImageSegmentation(const Mat image, const int seuil){
    // Declaration des variables
    int ligne, colonne ;
    Mat imageSeuillee = ImageSeuillage(image, seuil) ;
    Mat imageSegmentee(image.size(), CV_8U) ;

    // Segmentation
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            imageSegmentee.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne, colonne)*(imageSeuillee.at<unsigned char>(ligne, colonne)/255) ;
        }
    }

    // Retour
    return imageSegmentee ;
}

// Segmentation d'une image par seuillage simple
Mat ImageSegmentation(const Mat image, vector<int> seuil){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    Mat imageSegmentee ;                        // Image resultante
    vector<Mat> imageSegmenteeComposante ;      // Des composantes de l'image resultante

    // Initialisation
    imageSegmenteeComposante.clear() ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Segmentation sur chaque canal de couleur
    for(c = 0 ; c < nbComposante ; c++){
        imageSegmenteeComposante.push_back(ImageSegmentation(imageComposante[c], seuil[c]))  ;
    }

    // Fusion des composantes de couleur
    merge(imageSegmenteeComposante, imageSegmentee) ;

    // Retour
    return imageSegmentee ;
}

// Segmentation d'une composante de l'image par seuillage hysteresis
Mat ImageSegmentation(const Mat image, const int seuilBas, const int seuilHaut){
    return (ImageSegmentation(image, seuilBas) - ImageSegmentation(image, seuilHaut)) ;
}

// Segmentation d'une image par seuillage hysteresis
Mat ImageSegmentation(const Mat image, vector<int> seuilBas, vector<int> seuilHaut){
    return (ImageSegmentation(image, seuilBas) - ImageSegmentation(image, seuilHaut)) ;
}

//////////////////// Resolution ////////////////////

// Quantification
Mat ImageQuantification(const Mat image, const int valeur){
    // Declaration des variables
    int c, k, ligne, colonne ;                  // Indices
    int val = 1 ;                               // Valeur de calcul intermediaire
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageResultante ;                       // Image resultante
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    vector<int> niveau ;                        // Niveau d'intensite apres la quantification
    vector<Mat> imageResultanteComposante ;     // Des composantes de l'image resultante

    // Initialisation
    imageResultanteComposante.clear() ;
    niveau.clear() ;

    // Calculer 2^valeur
    for(c = 1 ; c <= valeur ; c++){
        val *= 2 ;
    }

    // Affecter le vecteur des niveaux
    for(c = 0 ; c <= val ; c++){
        niveau.push_back((256/val)*c) ;
    }

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Convolution sur chaque canal de couleur
    for(k = 0 ; k < nbComposante ; k++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne ++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                // Si la valeur du pixel est differente a 255
                if(imageComposante[k].at<unsigned char>(ligne, colonne) != 255){
                    // Determiner l'intervalle ou se trouve la valeur du pixel
                    for(c = 1 ; c < (int)niveau.size() ; c++){
                        if((imageComposante[k].at<unsigned char>(ligne, colonne) >= niveau[c-1]) && (imageComposante[k].at<unsigned char>(ligne, colonne) < niveau[c+1])){
                            // Affecter le pixel par la valeur de la borne inferieure
                            imageComposante[k].at<unsigned char>(ligne, colonne) = niveau[c-1] ;
                        }
                    }
                }

            }
        }
    }

    // Affecter le vecteur des composantes de l'image resultante
    for(c = 0 ; c < nbComposante ; c++){
        imageResultanteComposante.push_back(imageComposante[c])  ;
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

// Sous-echantillonnage de l'image
Mat ImageResolutionReduction(const Mat image, const int valeur){
    // Declaration des variables
    int c, ligne, colonne ;                                 // Indices
    int minResolutionInit ;                                 // Dimension la plus petite de l'image originale
    int pas = valeur*2 ;                                    // Pas de saut
    Size imageReduiteDimension ;                            // Dimensions de l'image reduite
    imageReduiteDimension.height = (int)image.size().height/pas ;
    imageReduiteDimension.width = (int)image.size().width/pas ;
    Mat imageReduite[(int)image.channels()] ;               // Image reduite
    Mat imageComposante[(int)image.channels()] ;            // Composantes de l'image de depart
    Mat imageResultante(imageReduiteDimension, CV_8UC3) ;   // Image resultante
    vector<Mat> imageResultanteComposante ;                 // Composantes de l'image resultante

    // Determiner la dimension la plus grande
    if((int)image.size().height < (int)image.size().width){
        minResolutionInit = (int)image.size().height ;
    }else{
        minResolutionInit = (int)image.size().width ;
    }

    // Si la plus petite dimension de l'image ne peut plus reduite ou il n'y a acune modification
    if(minResolutionInit/pas == 0){
        return image ;
    // Sinon
    }else{
        // Decomposition des composantes de couleur
        split(image, imageComposante) ;
        split(imageResultante, imageReduite) ;

        // Reduire de la dimension
        for(c = 0 ; c < (int)image.channels() ; c++){
            for(ligne = 0 ; ligne < (int)image.size().height/pas ; ligne++){
                for(colonne = 0 ; colonne < (int)image.size().width/pas ; colonne++){
                    imageReduite[c].at<unsigned char>(ligne, colonne) = imageComposante[c].at<unsigned char>(ligne*pas, colonne*pas) ;
                }
            }
            imageResultanteComposante.push_back(imageReduite[c]) ;
        }

        // Fusion des composantes de couleur
        merge(imageResultanteComposante, imageResultante) ;

        // Retour
        return imageResultante ;
    }
}

// Interpolation par pixel le plus proche
Mat InterpolationPPP(const Mat image, const int valeur){
    // Declaration des variables
    int c, ligne, colonne, ligneInter, colonneInter ;           // Indices
    int pas = valeur*2 ;                                        // Pas de saut
    Size imageInterpoleeDimension ;                             // Dimensions de l'image interpolee
    imageInterpoleeDimension.height = (int)image.size().height*pas ;
    imageInterpoleeDimension.width = (int)image.size().width*pas ;
    Mat imageInterpolee[(int)image.channels()] ;                // Image interpolee
    Mat imageComposante[(int)image.channels()] ;                // Composantes de l'image de depart
    Mat imageResultante(imageInterpoleeDimension, CV_8UC3) ;    // Image resultante
    vector<Mat> imageResultanteComposante ;                     // Composantes de l'image resultante

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;
    split(imageResultante, imageInterpolee) ;

    // Interpolation
    for(c = 0 ; c < (int)image.channels() ; c++){
        for(ligne = 0 ; ligne < (int)image.size().height ; ligne++){
            for(colonne = 0 ; colonne < (int)image.size().width ; colonne++){
                for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
                    for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                        imageInterpolee[c].at<unsigned char>(ligne*pas + ligneInter, colonne*pas + colonneInter) = imageComposante[c].at<unsigned char>(ligne, colonne) ;
                    }
                }
            }
        }
        imageResultanteComposante.push_back(imageInterpolee[c]) ;
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

// Interpolation bilineaire d'une image
Mat InterpolationBilineaire(const Mat image, const int valeur){
    // Declaration des variables
    int c ;                                                     // Indices
    int pas = valeur*2 ;                                        // Pas de saut
    Size imageInterpoleeDimension ;                             // Dimensions de l'image interpolee
    imageInterpoleeDimension.height = (int)image.size().height*pas ;
    imageInterpoleeDimension.width = (int)image.size().width*pas ;
    Mat imageInterpolee[(int)image.channels()] ;                // Image interpolee
    Mat imageComposante[(int)image.channels()] ;                // Composantes de l'image de depart
    Mat imageResultante(imageInterpoleeDimension, CV_8UC3) ;    // Image resultante
    vector<Mat> imageResultanteComposante ;                     // Composantes de l'image resultante

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;
    split(imageResultante, imageInterpolee) ;

    // Interpolation
    for(c = 0 ; c < (int)image.channels() ; c++){
        imageInterpolee[c] = InterpolationBilineaireMono(imageComposante[c], valeur) ;
        imageResultanteComposante.push_back(imageInterpolee[c]) ;
    }

    // Fusion des composantes de couleur
    merge(imageResultanteComposante, imageResultante) ;

    // Retour
    return imageResultante ;
}

// Interpolation bilineaire d'une composante de l'image
Mat InterpolationBilineaireMono(const Mat image, const int valeur){
    // Declaration des variables
    int ligne, colonne, ligneInter, colonneInter ;              // Indices
    int pas = valeur*2 ;                                        // Pas de saut
    int val ;                                                   // Valeur temporaire
    double projectionHaut, projectionBas ;                      // Valeurs projetees sur les deux cotes en haut et en bas
    double coefHaut1, coefHaut2, coefBas1, coefBas2 ;           // Coefficients pour l'interpolation sur deux cotes en haut et en bas
    double coefMix1, coefMix2 ;                                 // Coefficients pour l'interpolation generale
    Size imageInterpoleeDimension ;                             // Dimensions de l'image interpolee
    imageInterpoleeDimension.height = (int)image.size().height*pas ;
    imageInterpoleeDimension.width = (int)image.size().width*pas ;
    Mat imageResultante(imageInterpoleeDimension, CV_8U) ;      // Image resultante

    // Interpolation
    for(ligne = 0 ; ligne < (int)image.size().height - 1 ; ligne++){
        for(colonne = 0 ; colonne < (int)image.size().width - 1 ; colonne++){
            for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
                for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                    // Projection sur la ligne precedente
                    coefHaut1 = (double)(pas-colonneInter)/pas ;
                    coefHaut2 = (double)colonneInter/pas ;
                    projectionHaut = coefHaut1*image.at<unsigned char>(ligne, colonne) + coefHaut2*image.at<unsigned char>(ligne, colonne+1) ;
                    // Projection sur la ligne suivante
                    coefBas1 = (double)(pas-colonneInter)/pas ;
                    coefBas2 = (double)colonneInter/pas ;
                    projectionBas = coefBas1*image.at<unsigned char>(ligne+1, colonne) + coefBas2*image.at<unsigned char>(ligne+1, colonne+1) ;
                    // Interpolation
                    coefMix1 = (double)ligneInter/pas ;
                    coefMix2 = (double)(pas-ligneInter)/pas ;
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
                imageResultante.at<unsigned char>(ligne*pas + ligneInter, ((int)image.size().width-1)*pas + colonneInter) = image.at<unsigned char>(ligne, (int)image.size().width) ;
            }
        }
    }
    // Derniere ligne
    for(colonne = 0 ; colonne < (int)image.size().width - 1 ; colonne++){
        for(ligneInter = 0 ; ligneInter < pas ; ligneInter++){
            for(colonneInter = 0 ; colonneInter < pas ; colonneInter++){
                imageResultante.at<unsigned char>(((int)image.size().height-1)*pas + ligneInter, colonne*pas + colonneInter) = image.at<unsigned char>((int)image.size().height, colonne) ;
            }
        }
    }

    // Retour
    return imageResultante ;
}

//////////////////// Operations ////////////////////

// Determiner la valeur minimum dans une matrice entiere
int MinImage(const Mat image){
    double minVal ;
    cv::minMaxLoc(image, &minVal, nullptr) ;
    return (int)minVal ;
}

// Determiner la valeur maximum dans une matrice entiere
int MaxImage(const Mat image){
    double maxVal ;
    cv::minMaxLoc(image, nullptr, &maxVal) ;
    return (int)maxVal ;
}

// Determiner la valeur minimum dans un vecteur reel
double MinVecteur(const vector<double> vecteur){
    // Declaration des variables
    int c ;                 // Indice
    double valeurMin ;      // Valeur minimum

    // Initialisation
    valeurMin = vecteur[0] ;

    // Chercher le maximum
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMin >= vecteur[c]){
            valeurMin = vecteur[c] ;
        }
    }

    // Retour
    return valeurMin ;
}

// Determiner la valeur maximum dans un vecteur reel
double MaxVecteur(const vector<double> vecteur){
    // Declaration des variables
    int c ;                 // Indice
    double valeurMax ;      // Valeur maximum

    // Initialisation
    valeurMax = vecteur[0] ;

    // Chercher le maximum
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMax <= vecteur[c]){
            valeurMax = vecteur[c] ;
        }
    }

    // Retour
    return valeurMax ;
}

// Determiner la valeur minimum dans un vecteur entier
int MinVecteur(const vector<int> vecteur){
    // Declaration des variables
    int c ;                 // Indice
    int valeurMin ;         // Valeur minimum

    // Initialisation
    valeurMin = vecteur[0] ;

    // Chercher le maximum
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMin >= vecteur[c]){
            valeurMin = vecteur[c] ;
        }
    }

    // Retour
    return valeurMin ;
}

// Determiner la valeur maximum dans un vecteur entier
int MaxVecteur(const vector<int> vecteur){
    // Declaration des variables
    int c ;             // Indice
    int valeurMax ;      // Valeur maximum

    // Initialisation
    valeurMax = vecteur[0] ;

    // Chercher le maximum
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(valeurMax < vecteur[c]){
            valeurMax = vecteur[c] ;
        }
    }

    // Retour
    return valeurMax ;
}

// Determiner l'indice de la valeur minimum dans un vecteur entier
int MinVecteurIndice(const vector<int> vecteur){
    // Declaration des variables
    int c ;                 // Indice
    int valeurMin ;         // Valeur minimum
    int indiceMin ;         // Indice de la valeur minimum

    // Initialisation
    valeurMin = vecteur[0] ;
    indiceMin = 0 ;

    // Chercher le minimum
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(vecteur[c] < valeurMin){
            valeurMin = vecteur[c] ;
            indiceMin = c ;
        }
    }

    // Retour
    return indiceMin ;
}

// Determiner l'indice de la valeur minimum dans un vecteur reel
int MinVecteurIndice(const vector<double> vecteur){
    // Declaration des variables
    int c ;                 // Indice
    double valeurMin ;      // Valeur minimum
    int indiceMin ;         // Indice de la valeur minimum

    // Initialisation
    valeurMin = vecteur[0] ;
    indiceMin = 0 ;

    // Chercher le minimum
    for(c = 1 ; c < (int)vecteur.size() ; c++){
        if(vecteur[c] < valeurMin){
            valeurMin = vecteur[c] ;
            indiceMin = c ;
        }
    }

    // Retour
    return indiceMin ;
}

// Histogramme
vector<int> Histogramme(const Mat image, const int nbIntervalle){
    // Declaration des variables
    int c, ligne, colonne ;                     // Indices
    vector<int> occurence(nbIntervalle) ;       // Vecteur des occurences

    // Initialisation
    for(c = 0 ; c < nbIntervalle ; c++){
        occurence[c] = 0 ;
    }

    // Determiner le nombre d'occurences de chaque intervalle
    for(c = 0 ; c < nbIntervalle - 1 ; c++){
        for(ligne = 0 ; ligne < image.size().height ; ligne++){
            for(colonne = 0 ; colonne < image.size().width ; colonne++){
                if((image.at<unsigned char>(ligne, colonne) >= ((float)255/nbIntervalle*c)) && (image.at<unsigned char>(ligne, colonne) < ((float)255/nbIntervalle*(c+1)))){
                    occurence[c]++ ;
                }
            }
        }
    }

    // Derniere intervalle
    for(ligne = 0 ; ligne < image.size().height ; ligne++){
        for(colonne = 0 ; colonne < image.size().width ; colonne++){
            if((image.at<unsigned char>(ligne, colonne) == ((float)255/nbIntervalle*c))){
                occurence[nbIntervalle - 1]++ ;
            }
        }
    }

    // Retour
    return occurence ;
}

// Egalisation de l'histogramme d'une image au niveau de gris
vector<int> Egalisation(const Mat image, const int nbIntervalle){
    // Declaration des variables
    int c ;                                                         // Indice
    int nbPixel = image.size().height*image.size().width ;          // Nombre de pixels de l'image
    double somme = 0 ;                                              // Histogramme cumule
    double coefficient = (double)255/nbPixel ;                              // Niveau de gris divise par pixel
    vector<int> occurence = Histogramme(image, nbIntervalle) ;      // Histogramme de l'image
    vector<int> occurenceEgal(nbIntervalle) ;                       // Histogramme egalise

    // Egalisation
    for(c = 0 ; c < nbIntervalle ; c++){
        somme += occurence[c] ;
        occurenceEgal[c] = coefficient*somme ;
    }

    // Retour
    return occurenceEgal ;
}

// Generer une matrice zero
Mat ImageZero(const int nbLigne, const int nbColonne){
    return Mat::zeros(nbLigne, nbColonne, CV_8U) ;
}

// Produit de convolution entre deux matrices
Mat MatriceConvolution(const Mat image, const Mat filtre){
    // Declaration des variables
    int ligne, colonne, lig, col ;                                  // Indices
    int L_image, C_image ;                                          // Dimensions de l'image
    int L_filtre, C_filtre ;                                        // Dimensions du filtre
    int val ;                                                       // Variable intermediaire
    Mat imageConv(image.size(), CV_8U) ;                            // Image resultante
    Mat imageMiroir = ImageMiroir(image, filtre) ;                  // Image avec effet miroir

    // Dimensions de l'image
    L_image = image.size().height ;                                 // Nombre de lignes
    C_image = image.size().width ;                                  // Nombre de colonnes

    // Dimensions du filtre
    L_filtre = filtre.size().height ;                               // Nombre de lignes
    C_filtre = filtre.size().width ;                                // Nombre de colonnes

    // Produit de convolution
    for(ligne = 0 ; ligne <= L_image - 1 ; ligne++){                // Pour chaque ligne de l'image
        for(colonne = 0 ; colonne <= C_image - 1 ; colonne++){      // Pour chaque colonne de l'image
            val = 0 ;                                               // Remise a zero la variable intermediaire
            for(lig = 0 ; lig <= L_filtre/2 + 1  ; lig++){          // Pour chaque ligne du filtre
                for(col = 0 ; col <= C_filtre/2 + 1 ; col++){       // Pour chaque colonne du filtre
                    val += imageMiroir.at<unsigned char>(ligne + lig, colonne + col)*filtre.at<double>(lig, col) ;
                }
            }
            imageConv.at<unsigned char>(ligne, colonne) = VerifierSaturation(val) ;
        }
    }

    // Retour
    return imageConv ;
}

// Produit de convolution entre deux images
Mat ImageConvolution(const Mat image, const Mat filtre){
    // Declaration des variables
    int c ;                                     // Indice
    int nbComposante = image.channels() ;       // Nombre de composantes de couleur
    Mat imageConv ;                             // Image resultante
    Mat imageComposante[nbComposante] ;         // Des composantes de l'image originale
    vector<Mat> imageConvComposante ;           // Des composantes de l'image resultante

    // Initialisation
    imageConvComposante.clear() ;

    // Decomposition des composantes de couleur
    split(image, imageComposante) ;

    // Convolution sur chaque canal de couleur
    for(c = 0 ; c < nbComposante ; c++){
        imageConvComposante.push_back(MatriceConvolution(imageComposante[c], filtre))  ;
    }

    // Fusion des composantes de couleur
    merge(imageConvComposante, imageConv) ;

    // Retour
    return imageConv ;
}

// Calculer la norme
Mat MatriceNorme(const Mat imageX, const Mat imageY){
    // Declaration des variables
    int ligne, colonne ;                            // Indices
    double x2, y2 ;                                 // x au carre et y au carre
    Mat imageNorme(imageX.size(), CV_8U) ;          // Image resultante

    // Calculer la norme de chaque pixel
    for(ligne = 0 ; ligne < imageX.size().height ; ligne++){
        for(colonne = 0 ; colonne < imageX.size().width ; colonne++){
            x2 = (double)imageX.at<unsigned char>(ligne, colonne)*imageX.at<unsigned char>(ligne, colonne) ;
            y2 = (double)imageY.at<unsigned char>(ligne, colonne)*imageY.at<unsigned char>(ligne, colonne) ;
            imageNorme.at<unsigned char>(ligne, colonne) = VerifierSaturation((int)sqrt(x2 + y2)) ;
        }
    }

    // Retour
    return imageNorme ;
}

// Gradient en X
Mat GradientX(const Mat image, const string typeFiltre){
    if(typeFiltre == "Sobel"){
        return ImageConvolution(image, GenererFiltre(4)) ;
    }else if(typeFiltre == "Simple"){
        return ImageConvolution(image, GenererFiltre(6)) ;
    }else{
        //cout << "Type de filtre invalide." << endl ;
        return image ;
    }
}

// Gradient en Y
Mat GradientY(const Mat image, const string typeFiltre){
    if(typeFiltre == "Sobel"){
        return ImageConvolution(image, GenererFiltre(5)) ;
    }else if(typeFiltre == "Simple"){
        return ImageConvolution(image, GenererFiltre(7)) ;
    }else{
        //cout << "Type de filtre invalide." << endl ;
        return image ;
    }
}

// Maximum de la norme du gradient// Ne pas marcher  pour le moment
Mat MaxNormeGradient(const Mat gradientNorme, const Mat gradientX, const Mat gradientY){
    // Declaration des variables
    const double pi = 3.141592 ;                                                                // Nombre pi
    int ligne, colonne ;                                                                        // Indices
    double theta ;                                                                              // Direction de la norme du gradient
    vector<double> val ;                                                                        // Vecteur des valeurs a determiner le maximum
    Mat imageMaxNormeGradient(gradientNorme.size(), CV_8U) ;                                    // Image resultante

    // Initialisation
    imageMaxNormeGradient = gradientNorme ;

    // Ne pas traiter les bords horizontaux de l'image
    for(ligne = 1 ; ligne < gradientNorme.size().height - 1 ; ligne++){
        // Ne pas traiter les bords verticaux de l'image
        for(colonne = 1 ; colonne < gradientNorme.size().width - 1 ; colonne++){
            // Initialisation
            val.clear() ;

            // Corriger les division par zero
            if(gradientX.at<unsigned char>(ligne, colonne) == 0){
                theta = pi/2 ;
            }else{
                theta = (double)gradientY.at<unsigned char>(ligne, colonne)/gradientX.at<unsigned char>(ligne, colonne) ;
                // Mettre l'angle theta a la partie superieure du cercle trigonometrique
                if((theta < 0) || (theta > pi)){
                    theta += pi ;
                }
            }

            // Remplir le vecteur des valeurs a comparer
            // Si theta se situe entre [0, pi/4[ ou ]3pi/4, pi]
            if((theta > 3*pi/4) || (theta < pi/4)){
                val.push_back((double)gradientNorme.at<unsigned char>(ligne, colonne - 1)) ;    // Valeur du pixel a gauche
                val.push_back((double)gradientNorme.at<unsigned char>(ligne, colonne + 1)) ;    // Valeur du pixel a droite
            // Si theta se situe entre [pi/4, 3pi/4]
            }else{
                val.push_back((double)gradientNorme.at<unsigned char>(ligne - 1, colonne)) ;    // Valeur du pixel en bas
                val.push_back((double)gradientNorme.at<unsigned char>(ligne + 1, colonne)) ;    // Valeur du pixel en haut
            }

            // Valeur du pixel courant
            val.push_back((double)gradientNorme.at<unsigned char>(ligne, colonne)) ;

            // Determiner la valeur maximum
            imageMaxNormeGradient.at<unsigned char>(ligne, colonne) = (unsigned char)MaxVecteur(val) ;
        }
    }

    // Retour
    return imageMaxNormeGradient ;
}

// Determiner la valeur mediane d'une matrice
int MatriceMedian(const Mat matrice){
    // Declaration des variables
    int ligne, colonne ;
    vector<int> vecteurValeur ;

    // Initialisation
    vecteurValeur.clear() ;

    // Redimensionner la matrice en un veteur
    for(ligne = 0 ; ligne < matrice.size().height ; ligne++){
        for(colonne = 0 ; colonne < matrice.size().width ; colonne++){
            vecteurValeur.push_back((int)matrice.at<unsigned char>(ligne, colonne)) ;
        }
    }

    // Retour
    return VecteurMedian(vecteurValeur) ;
}

// Determiner la valeur mediane d'un vecteur
int VecteurMedian(vector<int> vecteur){
    if(vecteur.empty()){
        return 0 ;
    }

    const size_t medianIndex = vecteur.size()/2 ;
    nth_element(vecteur.begin(), vecteur.begin() + medianIndex, vecteur.end()) ;

    // Retour
    return vecteur[medianIndex] ;
}

// Verifier l'egalite entre deux matrices de memes dimensions
bool MatriceEgale(const Mat matrice1, const Mat matrice2){
    if(matrice1.size() != matrice2.size() || matrice1.type() != matrice2.type()){
        return false ;
    }
    return cv::countNonZero(matrice1 != matrice2) == 0 ;
}

// Calculer la moyenne d'un vecteur
double MoyenneVecteur(const vector<double> vecteur){
    // Declaration des variables
    int c ;                 // Indice
    double moyenne = 0 ;    // Moyenne

    // Calculer la somme
    for(c = 0 ; c < (int)vecteur.size() ; c++){
        moyenne += vecteur[c] ;
    }

    // Calculer la moyenne
    moyenne = (double)moyenne/(int)vecteur.size() ;

    // Retour
    return moyenne ;
}

// Calculer la variance d'un vecteur
double VarianceVecteur(const vector<double> vecteur){
    // Declaration des variables
    int c ;
    double somme = 0 ;
    double moyenne = MoyenneVecteur(vecteur) ;
    double variance ;

    // Calculer la somme au carre
    for(c = 0 ; c < (int)vecteur.size() ; c++){
        somme += vecteur[c]*vecteur[c] ;
    }

    // Calculer la variance
    variance = (double)1/(int)vecteur.size()*somme - moyenne*moyenne ;

    // Retour
    return variance ;
}

// Transformee de Fourier 2D de l'image (module)
// Transformée de Fourier 2D — module du spectre, centré, échelle log
Mat ImageFourier(const Mat image){
    // 1. Conversion en niveaux de gris (CV_8U)
    Mat imageMono = ImageMonochrome(image) ;

    // 2. Taille optimale pour la FFT (puissance de 2) → zero-padding
    int M = cv::getOptimalDFTSize(imageMono.rows) ;
    int N = cv::getOptimalDFTSize(imageMono.cols) ;
    Mat padded ;
    cv::copyMakeBorder(imageMono, padded,
                       0, M - imageMono.rows,
                       0, N - imageMono.cols,
                       cv::BORDER_CONSTANT, cv::Scalar::all(0)) ;

    // 3. Construire l'entrée complexe [partie réelle | partie imaginaire = 0]
    Mat planes[2] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) } ;
    Mat complexImg ;
    cv::merge(planes, 2, complexImg) ;

    // 4. DFT
    cv::dft(complexImg, complexImg) ;

    // 5. Module : |F(u,v)| = sqrt(Re² + Im²)
    cv::split(complexImg, planes) ;
    cv::magnitude(planes[0], planes[1], planes[0]) ;
    Mat mag = planes[0] ;

    // 6. Échelle logarithmique : log(1 + |F|) pour compresser la dynamique
    mag += cv::Scalar::all(1.0) ;
    cv::log(mag, mag) ;

    // 7. Recadrage à la taille originale (supprimer le zero-padding)
    mag = mag(cv::Rect(0, 0, imageMono.cols, imageMono.rows)).clone() ;

    // 8. Normalisation en [0, 255]
    cv::normalize(mag, mag, 0, 255, cv::NORM_MINMAX) ;
    Mat magU8 ;
    mag.convertTo(magU8, CV_8U) ;

    // 9. Centrage du spectre (fftshift) : la composante DC passe au centre
    int cx = magU8.cols / 2 ;
    int cy = magU8.rows / 2 ;
    Mat q0(magU8, cv::Rect(0,  0,  cx,              cy)) ;
    Mat q1(magU8, cv::Rect(cx, 0,  magU8.cols - cx, cy)) ;
    Mat q2(magU8, cv::Rect(0,  cy, cx,              magU8.rows - cy)) ;
    Mat q3(magU8, cv::Rect(cx, cy, magU8.cols - cx, magU8.rows - cy)) ;
    Mat tmp ;
    q0.copyTo(tmp) ; q3.copyTo(q0) ; tmp.copyTo(q3) ;
    q1.copyTo(tmp) ; q2.copyTo(q1) ; tmp.copyTo(q2) ;

    // 10. Retour en 3 canaux (requis par AffichageResultat)
    return MonoCouleur(magU8) ;
}

// Element de calcul de la transformee de Fourier 2D de l'image (module)
double ImageFourierElement(const Mat image, const int ligne, const int colonne){
    // Declaration des variables
    int L, C ;                                      // Indices
    double nbLigne = (int)image.size().height ;     // Nombre de lignes de l'image
    double nbColonne = (int)image.size().width ;    // Nombre de colonnes de l'image
    complex<double> resultat ;                      // Resutat
    complex<double> val ;                           // Valeur de calcul intermediaire
    complex<double> j(0.0, 1.0) ;                   // i imaginaire

    resultat = 0 ;
    for(L = 0 ; L < nbLigne ; L++){
        for(C = 0 ; C < nbColonne ; C++){
            val.real(2*3.14*((double)ligne*L/nbLigne + (double)colonne*C/nbColonne)) ;
            resultat += exp(-j*val)*(double)image.at<unsigned char>(L, C) ;
        }
    }
    resultat /= (nbLigne*nbColonne) ;

    // Retour
    return abs(resultat) ;
}

// Segmentation K-means
// Chaque pixel est remplacé par la couleur moyenne (centroïde) de son cluster.
// L'algorithme minimise la variance intra-classe :
//   argmin_{C_k} Σ_k Σ_{x∈C_k} ||x − μ_k||²
// où μ_k est le centroïde du cluster k et x est le vecteur de couleur d'un pixel.
// Initialisation KMeans++ : le premier centroïde est tiré aléatoirement,
// chaque centroïde suivant est choisi avec probabilité proportionnelle à D²(x),
// la distance carrée au centroïde le plus proche déjà choisi.
Mat ImageKMeans(const Mat image, const int k){
    if(k < 2 || image.empty()) return image ;

    // Reshape : (H×W) lignes × C colonnes, en float (requis par cv::kmeans)
    Mat data ;
    image.convertTo(data, CV_32F) ;
    data = data.reshape(1, data.rows * data.cols) ;  // (H*W) × nbCanaux

    // Segmentation K-means
    Mat labels, centers ;
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.2) ;
    cv::kmeans(data, k, labels, criteria, 5, cv::KMEANS_PP_CENTERS, centers) ;

    // Reconstruction : chaque pixel ← couleur du centroïde de son cluster
    centers.convertTo(centers, CV_8U) ;
    Mat result(image.size(), image.type()) ;
    int nC = image.channels() ;
    for(int i = 0 ; i < (int)data.rows ; i++){
        int row   = i / image.cols ;
        int col   = i % image.cols ;
        int label = labels.at<int>(i) ;
        if(nC == 3){
            result.at<Vec3b>(row, col) = Vec3b(
                centers.at<uchar>(label, 0),
                centers.at<uchar>(label, 1),
                centers.at<uchar>(label, 2)) ;
        }else{
            result.at<uchar>(row, col) = centers.at<uchar>(label, 0) ;
        }
    }
    return result ;
}

//////////////////// Autres ////////////////////

// Effet miroir avec un filtre determine
Mat ImageMiroir(const Mat image, const Mat filtre){
    // Declaration des variables
    int ligne, colonne, lig, col ;                                          // Indices
    int L_image, C_image, L_filtre, C_filtre, L_miroir, C_miroir  ;         // Dimensions

    // Dimensions de l'image
    L_image = image.size().height ;                                         // Nombre de lignes
    C_image = image.size().width ;                                          // Nombre de colonnes

    // Dimensions du filtre
    L_filtre = filtre.size().height ;                                       // Nombre de lignes
    C_filtre = filtre.size().width ;                                        // Nombre de colonnes

    // Initialisation de l'image resultante - Effet miroir
    L_miroir = L_image + L_filtre - 1 ;                                     // Nombre de lignes
    C_miroir = C_image + C_filtre - 1 ;                                     // Nombre de colonnes

    // Initialisation de l'image resultante
    Mat ImageMiroir = ImageZero(L_miroir, C_miroir) ;

    // Les bords horizontaux de l'image avec effet miroir
    // Premieres lignes
    // Initialisation des indices
    lig = 0 ;
    for(ligne = floor(L_filtre/2) - 1 ; ligne >= 0 ; ligne--){
        col = 0 ;
        for (colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    // Dernieres lignes
    // Initialisation des indices
    lig = L_image - floor(L_filtre/2) ;
    for(ligne = L_miroir - 1 ; ligne >= L_miroir - floor(L_filtre/2) ; ligne--){
        col = 0 ;
        for (colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    // Les bords verticaux de l'image avec effet miroir
    // Premieres colonnes
    // Initialisation des indices
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for (colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    // Dernieres colonnes
    // Initialisation des indices
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - 1 ;
        for (colonne = C_miroir - floor(C_filtre/2) ; colonne <= (C_miroir - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col-- ;
        }
        lig++ ;
    }

    // Les coins
    // En haut, a gauche
    // Initialisation des indices
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = floor(C_filtre/2) ;
        for (colonne = 0 ; colonne <= (floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col-- ;
        }
        lig-- ;
    }

    // En haut, a droite
    // Initialisation des indices
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for (colonne = C_miroir - 1 ; colonne >= (C_miroir - floor(C_filtre/2)) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig-- ;
    }

    // En bas, a gauche
    // Initialisation des indices
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = 0 ;
        for (colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig-- ;
    }

    // En bas, a droite
    // Initialisation des indices
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for (colonne = C_miroir - 1 ; colonne >= C_miroir - floor(C_filtre/2) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig-- ;
    }

    // Contenu
    // Initialisation des indices
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for (colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    return ImageMiroir ;
}

// Effet miroir avec la taille du miroir
Mat ImageMiroir(const Mat image, const int taille){
    // Declaration des variables
    int ligne, colonne, lig, col ;                                          // Indices
    int L_image, C_image, L_filtre, C_filtre, L_miroir, C_miroir  ;         // Dimensions

    // Dimensions de l'image
    L_image = image.size().height ;                                         // Nombre de lignes
    C_image = image.size().width ;                                          // Nombre de colonnes

    // Dimensions du filtre
    L_filtre = taille ;                                                     // Nombre de lignes
    C_filtre = taille ;                                                     // Nombre de colonnes

    // Initialisation de l'image resultante - Effet miroir
    L_miroir = L_image + L_filtre - 1 ;                                     // Nombre de lignes
    C_miroir = C_image + C_filtre - 1 ;                                     // Nombre de colonnes

    // Initialisation de l'image resultante
    Mat ImageMiroir = ImageZero(L_miroir, C_miroir) ;

    // Les bords horizontaux de l'image avec effet miroir
    // Premieres lignes
    // Initialisation des indices
    lig = 0 ;
    for(ligne = floor(L_filtre/2) - 1 ; ligne >= 0 ; ligne--){
        col = 0 ;
        for (colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    // Dernieres lignes
    // Initialisation des indices
    lig = L_image - floor(L_filtre/2) ;
    for(ligne = L_miroir - 1 ; ligne >= L_miroir - floor(L_filtre/2) ; ligne--){
        col = 0 ;
        for (colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    // Les bords verticaux de l'image avec effet miroir
    // Premieres colonnes
    // Initialisation des indices
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for (colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    // Dernieres colonnes
    // Initialisation des indices
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - 1 ;
        for (colonne = C_miroir - floor(C_filtre/2) ; colonne <= (C_miroir - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col-- ;
        }
        lig++ ;
    }

    // Les coins
    // En haut, a gauche
    // Initialisation des indices
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = floor(C_filtre/2) ;
        for (colonne = 0 ; colonne <= (floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col-- ;
        }
        lig-- ;
    }

    // En haut, a droite
    // Initialisation des indices
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for (colonne = C_miroir - 1 ; colonne >= (C_miroir - floor(C_filtre/2)) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig-- ;
    }

    // En bas, a gauche
    // Initialisation des indices
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = 0 ;
        for (colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig-- ;
    }

    // En bas, a droite
    // Initialisation des indices
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for (colonne = C_miroir - 1 ; colonne >= C_miroir - floor(C_filtre/2) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig-- ;
    }

    // Contenu
    // Initialisation des indices
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for (colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col) ;
            // Incrementation des indices
            col++ ;
        }
        lig++ ;
    }

    return ImageMiroir ;
}

// Generer les filtres
Mat GenererFiltre(const int typeFiltre){
    switch (typeFiltre){
    // Filtre moyenneur
    case 1 :
        return ((Mat_<double>(3,3) << 1, 1, 1, 1, 1, 1, 1, 1, 1)/9) ;
        break ;
    // Filtre laplacien
    case 2 :
        return (Mat_<double>(3,3) << -1, -1, -1, -1, 8, -1, -1, -1, -1) ;
        break ;
    // Filtre gaussien
    case 3 :
        return ((Mat_<double>(3,3) << 1, 2, 1, 2, 4, 2, 1, 2, 1)/16) ;
        break ;
    // Filtre gradient en x (Sobel)
    case 4 :
        return (Mat_<double>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1) ;
        break ;
    // Filtre gradient en y (Sobel)
    case 5 :
        return (Mat_<double>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1) ;
        break ;
    // Filtre gradient en x (simple)
    case 6 :
        return (Mat_<double>(3,3) << 0, 1, 0, 0, 0, 0, 0, -1, 0) ;
        break ;
    // Filtre gradient en y (simple)
    case 7 :
        return (Mat_<double>(3,3) << 0, 0, 0, 1, 0, -1, 0, 0, 0) ;
        break ;
    default:
        return (Mat_<double>(3,3) << 0, 0, 0, 0, 1, 0, 0, 0, 0) ;
        break;
    }
}

// Convertir le format d'une image monochrome en format RVB
Mat MonoCouleur(const Mat image){
    // Declaration des variables
    int c ;                                             // Indice
    Mat imageCouleur(image.size(), CV_8UC3) ;           // Image resultante
    vector<Mat> imageCouleurComposante ;                // Des composantes de l'image resultante

    // Copier le contenu de l'image de depart dans les trois canaux de couleurs
    for(c = 0 ; c < 3 ; c++){
        imageCouleurComposante.push_back(image) ;
    }

    // Fusion
    merge(imageCouleurComposante, imageCouleur) ;
    // Retour
    return imageCouleur ;
}

// Calcul d'histogramme sur un cannal
Mat CalcHistogram(Mat image) {
    Mat hist;
    hist = Mat::zeros(256, 1, CV_32F);
    image.convertTo(image, CV_32F);
    double value = 0;
    for (int i = 0; i < image.rows; i++)
    {
    for (int j = 0; j < image.cols; j++)
        {
            value = image.at<float>(i, j);
            hist.at<float>(value) = hist.at<float>(value) + 1;
        }
    }
    return hist;
}

// Image du Graphe Histogramme d'un seul canal
Mat PlotHistogram1D(Mat image,int choixCouleur) {
    Mat histogram_image(400, 512, CV_8UC3, Scalar(0, 0, 0));
    Mat normalized_histogram;
    Mat histogram = CalcHistogram(image);
    Scalar couleur;
    normalize(histogram, normalized_histogram, 0, 400, NORM_MINMAX, -1, Mat());
    if (choixCouleur==1) {
        couleur=Scalar(255,0,0);
    }else if (choixCouleur==2) {
        couleur=Scalar(0,255,0);
    }else if(choixCouleur==3){
        couleur=Scalar(0,0,255);
    }else{
        couleur=Scalar(255,255,255);
    }
    for (int i = 0; i < 256; i++)
    {
        rectangle(histogram_image, Point(2 * i, histogram_image.rows - normalized_histogram.at<float>(i)),
            Point(2 * (i + 1), histogram_image.rows), couleur);
    }
    return histogram_image;
}

// Image du graphe histogramme d'image 1 canals ou 3 canals
Mat PlotHistogram(Mat image) {
    Mat imageTemp ;
    if (VerifierImage(image, imageTemp)){
        return PlotHistogram1D(imageTemp, 0) ;
    }
    // Image couleur : tracer un histogramme par canal (BGR)
    // choix=1→Bleu, choix=2→Vert, choix=3→Rouge  /  couleur=1→bleu, 2→vert, 3→rouge
    Mat histogram_image_R = PlotHistogram1D(ImageExtractionCouleur(imageTemp, 3), 3) ;
    Mat histogram_image_G = PlotHistogram1D(ImageExtractionCouleur(imageTemp, 2), 2) ;
    Mat histogram_image_B = PlotHistogram1D(ImageExtractionCouleur(imageTemp, 1), 1) ;
    Mat histogram_image ;
    addWeighted(histogram_image_B, 0.5, histogram_image_G, 0.5, 0.0, histogram_image) ;
    addWeighted(histogram_image,   0.5, histogram_image_R, 0.5, 0.0, histogram_image) ;
    return histogram_image ;
}

// Verifier si l'image est en couleurs ou en niveau de gris
bool VerifierImage(const Mat image, Mat& imageVerifiee){
    // Declaration des variables
    bool imageNiveauGris ;          // Verification si l'image est en niveau de gris
    vector<Mat> composante ;        // Composantes de couleur de l'image

    // Si l'image contient une seule composante
    if(image.channels() == 1){
        // L'image est en niveau de gris
        imageVerifiee = image ;
        imageNiveauGris = true ;
    // Si l'image contient plusieurs composantes
    }else{
        // Decomposer l'image en composantes
        split(image, composante) ;
        // Si les trois composantes sont egales
        if(MatriceEgale(composante[0], composante[1]) && MatriceEgale(composante[1], composante[2])){
            imageVerifiee = composante[0] ;
            imageNiveauGris = true ;
        // Sinon : image en couleurs
        }else{
            imageVerifiee = image ;
            imageNiveauGris = false ;
        }
    }

    // Retour
    return imageNiveauGris ;
}

// Echanger les composantes R et B de l'image
Mat ImageBGRRGB(const Mat image){
    if(image.channels() == 1){
        return image.clone() ;
    }
    Mat imageResultante ;
    cvtColor(image, imageResultante, COLOR_BGR2RGB) ;
    return imageResultante ;
}

// Verifier la saturation
int VerifierSaturation(const int valeur){
    if(valeur > 255){
        return 255 ;
    }else if(valeur < 0){
        return 0 ;
    }else{
        return valeur ;
    }
}

// Arranger la representation de la transformee de Fourier
Mat ImageFourierArranger(const Mat image){
    // Declaration des variables
    int ligne, colonne ;
    int nbLigne = (int)image.size().height ;
    int nbColonne = (int)image.size().width ;
    Mat imageHG(nbLigne/2, nbColonne/2, CV_8U) ;
    Mat imageHD(nbLigne/2, nbColonne/2, CV_8U) ;
    Mat imageBG(nbLigne - nbLigne/2, nbColonne - nbColonne/2, CV_8U) ;
    Mat imageBD(nbLigne - nbLigne/2, nbColonne - nbColonne/2, CV_8U) ;
    Mat imageArrangee(image.size(), CV_8U) ;

    // Partie en haut, a gauche de l'image
    for(ligne = 0 ; ligne < (int)imageHG.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)imageHG.size().width ; colonne++){
            imageHG.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne, colonne) ;
        }
    }
    // Partie en haut, a droite de l'image
    for(ligne = 0 ; ligne < (int)imageHD.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)imageHD.size().width ; colonne++){
            imageHD.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne, colonne + nbColonne/2) ;
        }
    }
    // Partie en bas, a gauche de l'image
    for(ligne = 0 ; ligne < (int)imageBG.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)imageBG.size().width ; colonne++){
            imageBG.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne + nbLigne/2, colonne) ;
        }
    }
    // Partie en bas, a droite de l'image
    for(ligne = 0 ; ligne < (int)imageBD.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)imageBD.size().width ; colonne++){
            imageBD.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(ligne + nbLigne/2, colonne + nbColonne/2) ;
        }
    }

    // Image arrangee
    // Partie en haut, a gauche
    for(ligne = 0 ; ligne < (int)imageBD.size().height ; ligne++){
        for(colonne = 0 ; colonne < (int)imageBD.size().width ; colonne++){
            imageArrangee.at<unsigned char>(ligne, colonne) = imageHG.at<unsigned char>(ligne, colonne) ;
        }
    }
    // Retour
    return imageArrangee ;
}
Mat TransformeedeHough(const Mat image,int num_theta,int threshold_acc){
    Mat gray_image;
    Mat edge_image;
    Mat accumulateur;
    Mat imageHough;

    int edge_width,edge_height;
    double edge_height_half, edge_width_half;

    double dtheta;
    int theta_idx;

    double rho;
    int rho_idx;


    int Max_rho;
    int Maxrho_idx; //index maximal de rho

    double deg2rad;
    // défini le nombre de point pour discrétiser les rho et théta
    //int num_theta =180;
    //int threshold_acc = 80; //Seuillage

    int ligne,colonne;


    Point pt1, pt2;

    image.copyTo(imageHough);

    // Tables de sinus/cosinus pre-calculees
    vector<double> cos_theta(num_theta) ;
    vector<double> sin_theta(num_theta) ;
    vector<double> thetas(num_theta) ;

    // Mettre l'image en niveau de gris
    gray_image = ImageMonochrome(image);

    // Detection des contours
    Canny(gray_image,edge_image,180,255);

    //taille de l'image suite à la détention d'un contour
    edge_width = edge_image.cols; // largeur de l'image
    edge_height = edge_image.rows; // hauteur de l'image
    edge_width_half = edge_width / 2;  // mi- largeur de l'image
    edge_height_half = edge_height / 2; // mi- hauteur de l'image


    // Iniatilisation des pas de theta et de rho
    //rho varie entre 0 et la diagonale de l'image
    Max_rho = round(sqrt(edge_width*edge_width + edge_height*edge_height));
    Maxrho_idx = 2*Max_rho+1;

    dtheta = (double)180/ num_theta;
    deg2rad = CV_PI /180; // conversion des degré en radian

    // Discrétisation de théta et calcul de cos(theta), sin(theta)
    for(theta_idx=0;theta_idx<num_theta;theta_idx++){
        thetas [theta_idx] = theta_idx*dtheta;
        //cout << "valeur de theta : " << thetas [theta_idx]<<endl;

        //Calcule des cos(theta) et sinus(thetas) en radian;
        cos_theta [theta_idx] = cos(deg2rad*thetas [theta_idx]);
        sin_theta [theta_idx] = sin(deg2rad*thetas [theta_idx]);
    }


    // Creation de l'accumulateur. CV_32S evite les saturations a 255 votes.
    accumulateur = Mat::zeros(Maxrho_idx,num_theta,CV_32S);

    // Calcul dans l'espace de Hough
    for (ligne = 0;ligne < edge_height; ligne ++){
        for(colonne = 0; colonne < edge_width; colonne ++){
            //détection des pixel ayant une valeur différent de 0
            if (edge_image.at<unsigned char>(ligne,colonne)!=0){


                for(theta_idx = 0; theta_idx < num_theta; theta_idx ++){

                    rho = (colonne - edge_width_half) * cos_theta[theta_idx] +  (ligne - edge_height_half) * sin_theta[theta_idx];
                    rho_idx = round(rho)+Max_rho;
                    //cout << "valeur de rho" << rho<< "valeur de lindex "<< rho_idx<< endl;
                    //cout << index_Rho << endl;
                    accumulateur.at<int>(rho_idx,theta_idx)++;

                }
            }
        }

    }

    //Tracage ds droite détecter à partir de l'accumulateur
    for (ligne = 0;ligne < accumulateur.rows; ligne ++){
        for(colonne = 0; colonne < accumulateur.cols; colonne ++){
            if(accumulateur.at<int>(ligne,colonne) > (threshold_acc)){

                //y = (r - x cos(t)) / sin(t)
                if(thetas[colonne] >= 45 && thetas[colonne] <= 135){
                    pt1.x = 0;
                    pt1.y =((double)(ligne-Max_rho) - ((pt1.x - (edge_width_half) ) * cos_theta [colonne])) / sin_theta [colonne] + (edge_height_half);
                    pt2.x = edge_width;
                    pt2.y = ((double)(ligne-Max_rho) - ((pt2.x - (edge_width_half) ) * cos_theta [colonne])) / sin_theta [colonne] + (edge_height_half);

               }

                //x = (r - y sin(t)) / cos(t);
                else{

                    pt1.y = 0;
                    pt1.x = ((double)(ligne-Max_rho) - ((pt1.y - (edge_height_half) ) * sin_theta [colonne])) /cos_theta [colonne] + (edge_width_half);
                    pt2.y = edge_height;
                    pt2.x = ((double)(ligne-Max_rho) - ((pt2.y - (edge_height_half) ) * sin_theta [colonne])) / cos_theta [colonne] + (edge_width_half);
                }

                line( imageHough, pt1, pt2, Scalar(0,0,255), 5, LINE_AA);

            }

        }

    }

    //cout << accumulateur << endl;

    return imageHough;
   }
}
