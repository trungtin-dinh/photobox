#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Segmentation K-means
//
// Minimise la variance intra-classe : argmin_{C_k} Σ_k Σ_{x∈C_k} ‖x − μ_k‖²
// où μ_k est le centroïde du cluster k et x le vecteur de couleur d'un pixel.
//
// Initialisation KMeans++ : le 1er centroïde est tiré aléatoirement, chaque
// centroïde suivant est choisi avec probabilité ∝ D²(x), la distance carrée
// au centroïde le plus proche déjà sélectionné. Cela garantit une meilleure
// dispersion initiale et accélère la convergence par rapport à l'init aléatoire.
//
// cv::kmeans effectue 5 tentatives indépendantes et retient la meilleure
// (variance intra-classe minimale). Critère d'arrêt : 100 itérations ou ε = 0.2.
Mat ImageKMeans(const Mat image, const int k){
    if(k < 2 || image.empty()) return image ;

    // Reshape : (H×W) lignes × nbCanaux colonnes, type float (requis par cv::kmeans)
    Mat data ;
    image.convertTo(data, CV_32F) ;
    data = data.reshape(1, data.rows * data.cols) ;

    Mat labels, centers ;
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.2) ;
    cv::kmeans(data, k, labels, criteria, 5, cv::KMEANS_PP_CENTERS, centers) ;

    // Reconstruction : chaque pixel prend la couleur du centroïde de son cluster
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

} // namespace ImageToolBox
