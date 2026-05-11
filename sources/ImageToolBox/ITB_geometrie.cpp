#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// ---- Effet miroir (padding symétrique pour la convolution) ----

// Version avec filtre explicite
Mat ImageMiroir(const Mat image, const Mat filtre){
    int ligne, colonne, lig, col ;
    int L_image = image.size().height, C_image = image.size().width ;
    int L_filtre = filtre.size().height, C_filtre = filtre.size().width ;
    int L_miroir = L_image + L_filtre - 1 ;
    int C_miroir = C_image + C_filtre - 1 ;
    Mat ImageMiroir = ImageZero(L_miroir, C_miroir) ;

    // Premieres lignes
    lig = 0 ;
    for(ligne = floor(L_filtre/2) - 1 ; ligne >= 0 ; ligne--){
        col = 0 ;
        for(colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    // Dernieres lignes
    lig = L_image - floor(L_filtre/2) ;
    for(ligne = L_miroir - 1 ; ligne >= L_miroir - floor(L_filtre/2) ; ligne--){
        col = 0 ;
        for(colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    // Premieres colonnes
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for(colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    // Dernieres colonnes
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - 1 ;
        for(colonne = C_miroir - floor(C_filtre/2) ; colonne <= (C_miroir - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col--) ;
        }
        lig++ ;
    }
    // Coin haut-gauche
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = floor(C_filtre/2) ;
        for(colonne = 0 ; colonne <= (floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col--) ;
        }
        lig-- ;
    }
    // Coin haut-droite
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for(colonne = C_miroir - 1 ; colonne >= (C_miroir - floor(C_filtre/2)) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig-- ;
    }
    // Coin bas-gauche
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = 0 ;
        for(colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig-- ;
    }
    // Coin bas-droite
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for(colonne = C_miroir - 1 ; colonne >= C_miroir - floor(C_filtre/2) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig-- ;
    }
    // Contenu central
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for(colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    return ImageMiroir ;
}

// Version avec taille de voisinage carré
Mat ImageMiroir(const Mat image, const int taille){
    int ligne, colonne, lig, col ;
    int L_image = image.size().height, C_image = image.size().width ;
    int L_filtre = taille, C_filtre = taille ;
    int L_miroir = L_image + L_filtre - 1 ;
    int C_miroir = C_image + C_filtre - 1 ;
    Mat ImageMiroir = ImageZero(L_miroir, C_miroir) ;

    // Premieres lignes
    lig = 0 ;
    for(ligne = floor(L_filtre/2) - 1 ; ligne >= 0 ; ligne--){
        col = 0 ;
        for(colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    // Dernieres lignes
    lig = L_image - floor(L_filtre/2) ;
    for(ligne = L_miroir - 1 ; ligne >= L_miroir - floor(L_filtre/2) ; ligne--){
        col = 0 ;
        for(colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - ceil(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    // Premieres colonnes
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for(colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    // Dernieres colonnes
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - 1 ;
        for(colonne = C_miroir - floor(C_filtre/2) ; colonne <= (C_miroir - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col--) ;
        }
        lig++ ;
    }
    // Coin haut-gauche
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = floor(C_filtre/2) ;
        for(colonne = 0 ; colonne <= (floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col--) ;
        }
        lig-- ;
    }
    // Coin haut-droite
    lig = floor(L_filtre/2) ;
    for(ligne = 0 ; ligne <= (floor(L_filtre/2) - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for(colonne = C_miroir - 1 ; colonne >= (C_miroir - floor(C_filtre/2)) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig-- ;
    }
    // Coin bas-gauche
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = 0 ;
        for(colonne = floor(C_filtre/2) - 1 ; colonne >= 0 ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig-- ;
    }
    // Coin bas-droite
    lig = L_image - 1 ;
    for(ligne = L_miroir - floor(L_filtre/2) ; ligne <= (L_miroir - 1) ; ligne++){
        col = C_image - floor(C_filtre/2) ;
        for(colonne = C_miroir - 1 ; colonne >= C_miroir - floor(C_filtre/2) ; colonne--){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig-- ;
    }
    // Contenu central
    lig = 0 ;
    for(ligne = ceil(L_filtre/2) ; ligne <= (L_miroir - floor(L_filtre/2) - 1) ; ligne++){
        col = 0 ;
        for(colonne = ceil(C_filtre/2) ; colonne <= (C_miroir - floor(C_filtre/2) - 1) ; colonne++){
            ImageMiroir.at<unsigned char>(ligne, colonne) = image.at<unsigned char>(lig, col++) ;
        }
        lig++ ;
    }
    return ImageMiroir ;
}

// ---- Filtres 3×3 prédéfinis ----
Mat GenererFiltre(const int typeFiltre){
    switch(typeFiltre){
    case 1 : return ((Mat_<double>(3,3) << 1,1,1, 1,1,1, 1,1,1)/9) ;          // Moyenneur
    case 2 : return (Mat_<double>(3,3)  << -1,-1,-1, -1,8,-1, -1,-1,-1) ;     // Laplacien
    case 3 : return ((Mat_<double>(3,3) << 1,2,1, 2,4,2, 1,2,1)/16) ;         // Gaussien
    case 4 : return (Mat_<double>(3,3)  << -1,-2,-1, 0,0,0, 1,2,1) ;          // Sobel X
    case 5 : return (Mat_<double>(3,3)  << -1,0,1, -2,0,2, -1,0,1) ;          // Sobel Y
    case 6 : return (Mat_<double>(3,3)  << 0,1,0, 0,0,0, 0,-1,0) ;            // Gradient X simple
    case 7 : return (Mat_<double>(3,3)  << 0,0,0, 1,0,-1, 0,0,0) ;            // Gradient Y simple
    default: return (Mat_<double>(3,3)  << 0,0,0, 0,1,0, 0,0,0) ;             // Identite
    }
}

// ---- Convolution ----

// Produit de convolution discret sur une composante
Mat MatriceConvolution(const Mat image, const Mat filtre){
    int ligne, colonne, lig, col ;
    int L_image = image.size().height, C_image = image.size().width ;
    int L_filtre = filtre.size().height, C_filtre = filtre.size().width ;
    int val ;
    Mat imageConv(image.size(), CV_8U) ;
    Mat imageMiroir = ImageMiroir(image, filtre) ;

    for(ligne = 0 ; ligne <= L_image - 1 ; ligne++){
        for(colonne = 0 ; colonne <= C_image - 1 ; colonne++){
            val = 0 ;
            for(lig = 0 ; lig <= L_filtre/2 + 1 ; lig++){
                for(col = 0 ; col <= C_filtre/2 + 1 ; col++){
                    val += imageMiroir.at<unsigned char>(ligne + lig, colonne + col) * filtre.at<double>(lig, col) ;
                }
            }
            imageConv.at<unsigned char>(ligne, colonne) = VerifierSaturation(val) ;
        }
    }
    return imageConv ;
}

// Convolution sur tous les canaux d'une image
Mat ImageConvolution(const Mat image, const Mat filtre){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageConv ;
    Mat imageComposante[nbComposante] ;
    vector<Mat> imageConvComposante ;

    imageConvComposante.clear() ;
    split(image, imageComposante) ;
    for(c = 0 ; c < nbComposante ; c++) imageConvComposante.push_back(MatriceConvolution(imageComposante[c], filtre)) ;
    merge(imageConvComposante, imageConv) ;
    return imageConv ;
}

// ---- Gradients ----

// Norme du gradient : ‖∇I‖ = sqrt(Gx² + Gy²)
Mat MatriceNorme(const Mat imageX, const Mat imageY){
    int ligne, colonne ;
    double x2, y2 ;
    Mat imageNorme(imageX.size(), CV_8U) ;

    for(ligne = 0 ; ligne < imageX.size().height ; ligne++){
        for(colonne = 0 ; colonne < imageX.size().width ; colonne++){
            x2 = (double)imageX.at<unsigned char>(ligne, colonne) * imageX.at<unsigned char>(ligne, colonne) ;
            y2 = (double)imageY.at<unsigned char>(ligne, colonne) * imageY.at<unsigned char>(ligne, colonne) ;
            imageNorme.at<unsigned char>(ligne, colonne) = VerifierSaturation((int)sqrt(x2 + y2)) ;
        }
    }
    return imageNorme ;
}

// Gradient horizontal
Mat GradientX(const Mat image, const string typeFiltre){
    if(typeFiltre == "Sobel")  return ImageConvolution(image, GenererFiltre(4)) ;
    if(typeFiltre == "Simple") return ImageConvolution(image, GenererFiltre(6)) ;
    return image ;
}

// Gradient vertical
Mat GradientY(const Mat image, const string typeFiltre){
    if(typeFiltre == "Sobel")  return ImageConvolution(image, GenererFiltre(5)) ;
    if(typeFiltre == "Simple") return ImageConvolution(image, GenererFiltre(7)) ;
    return image ;
}

// Maximum de la norme dans la direction du gradient (suppression de non-maxima)
Mat MaxNormeGradient(const Mat gradientNorme, const Mat gradientX, const Mat gradientY){
    const double pi = 3.141592 ;
    int ligne, colonne ;
    double theta ;
    vector<double> val ;
    Mat imageMaxNormeGradient = gradientNorme ;

    for(ligne = 1 ; ligne < gradientNorme.size().height - 1 ; ligne++){
        for(colonne = 1 ; colonne < gradientNorme.size().width - 1 ; colonne++){
            val.clear() ;
            if(gradientX.at<unsigned char>(ligne, colonne) == 0){
                theta = pi/2 ;
            }else{
                theta = (double)gradientY.at<unsigned char>(ligne, colonne)/gradientX.at<unsigned char>(ligne, colonne) ;
                if((theta < 0) || (theta > pi)) theta += pi ;
            }
            if((theta > 3*pi/4) || (theta < pi/4)){
                val.push_back((double)gradientNorme.at<unsigned char>(ligne, colonne - 1)) ;
                val.push_back((double)gradientNorme.at<unsigned char>(ligne, colonne + 1)) ;
            }else{
                val.push_back((double)gradientNorme.at<unsigned char>(ligne - 1, colonne)) ;
                val.push_back((double)gradientNorme.at<unsigned char>(ligne + 1, colonne)) ;
            }
            val.push_back((double)gradientNorme.at<unsigned char>(ligne, colonne)) ;
            imageMaxNormeGradient.at<unsigned char>(ligne, colonne) = (unsigned char)MaxVecteur(val) ;
        }
    }
    return imageMaxNormeGradient ;
}

} // namespace ImageToolBox
