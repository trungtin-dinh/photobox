#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Transformée de Hough (détection de droites)
//
// Paramètre (ρ, θ) : chaque droite dans l'image est représentée par
//   ρ = x·cos(θ) + y·sin(θ)
// avec θ ∈ [0°, 180°) et ρ ∈ [-D, D] (D = diagonale de l'image).
// L'accumulateur H(ρ,θ) compte combien de points de contour votent pour chaque (ρ,θ).
// Une droite est détectée si H(ρ,θ) > threshold_acc.
Mat TransformeedeHough(const Mat image, int num_theta, int threshold_acc){
    Mat gray_image, edge_image, accumulateur, imageHough ;
    int edge_width, edge_height ;
    double edge_height_half, edge_width_half ;
    double dtheta, rho ;
    int theta_idx, rho_idx ;
    int Max_rho, Maxrho_idx ;
    double deg2rad ;
    int ligne, colonne ;
    Point pt1, pt2 ;

    image.copyTo(imageHough) ;

    vector<double> cos_theta(num_theta) ;
    vector<double> sin_theta(num_theta) ;
    vector<double> thetas(num_theta) ;

    gray_image = ImageMonochrome(image) ;
    Canny(gray_image, edge_image, 180, 255) ;

    edge_width      = edge_image.cols ;
    edge_height     = edge_image.rows ;
    edge_width_half = edge_width  / 2 ;
    edge_height_half = edge_height / 2 ;

    Max_rho    = round(sqrt(edge_width*edge_width + edge_height*edge_height)) ;
    Maxrho_idx = 2*Max_rho + 1 ;

    dtheta  = (double)180 / num_theta ;
    deg2rad = CV_PI / 180 ;

    for(theta_idx = 0 ; theta_idx < num_theta ; theta_idx++){
        thetas[theta_idx]    = theta_idx * dtheta ;
        cos_theta[theta_idx] = cos(deg2rad * thetas[theta_idx]) ;
        sin_theta[theta_idx] = sin(deg2rad * thetas[theta_idx]) ;
    }

    accumulateur = Mat::zeros(Maxrho_idx, num_theta, CV_32S) ;

    for(ligne = 0 ; ligne < edge_height ; ligne++){
        for(colonne = 0 ; colonne < edge_width ; colonne++){
            if(edge_image.at<unsigned char>(ligne, colonne) != 0){
                for(theta_idx = 0 ; theta_idx < num_theta ; theta_idx++){
                    rho     = (colonne - edge_width_half)*cos_theta[theta_idx] + (ligne - edge_height_half)*sin_theta[theta_idx] ;
                    rho_idx = round(rho) + Max_rho ;
                    accumulateur.at<int>(rho_idx, theta_idx)++ ;
                }
            }
        }
    }

    for(ligne = 0 ; ligne < accumulateur.rows ; ligne++){
        for(colonne = 0 ; colonne < accumulateur.cols ; colonne++){
            if(accumulateur.at<int>(ligne, colonne) > threshold_acc){
                if(thetas[colonne] >= 45 && thetas[colonne] <= 135){
                    pt1.x = 0 ;
                    pt1.y = ((double)(ligne - Max_rho) - ((pt1.x - edge_width_half)*cos_theta[colonne])) / sin_theta[colonne] + edge_height_half ;
                    pt2.x = edge_width ;
                    pt2.y = ((double)(ligne - Max_rho) - ((pt2.x - edge_width_half)*cos_theta[colonne])) / sin_theta[colonne] + edge_height_half ;
                }else{
                    pt1.y = 0 ;
                    pt1.x = ((double)(ligne - Max_rho) - ((pt1.y - edge_height_half)*sin_theta[colonne])) / cos_theta[colonne] + edge_width_half ;
                    pt2.y = edge_height ;
                    pt2.x = ((double)(ligne - Max_rho) - ((pt2.y - edge_height_half)*sin_theta[colonne])) / cos_theta[colonne] + edge_width_half ;
                }
                line(imageHough, pt1, pt2, Scalar(0,0,255), 5, LINE_AA) ;
            }
        }
    }
    return imageHough ;
}

} // namespace ImageToolBox
