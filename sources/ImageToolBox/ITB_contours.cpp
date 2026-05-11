#include "headers/ImageToolBox.h"

namespace ImageToolBox {
using namespace cv ;
using namespace std ;

// Detection de contours par filtres gradients (gradient simple, norme euclidienne)
Mat ImageContourGradient(const Mat image){
    int nbComposante = image.channels() ;
    Mat gradientX, gradientY ;
    Mat imageContour(image.size(), CV_8U) ;

    if(nbComposante > 1) imageContour = ImageMonochrome(image) ;

    gradientX = GradientX(imageContour, "Simple") ;
    gradientY = GradientY(imageContour, "Simple") ;
    imageContour = MatriceNorme(gradientX, gradientY) ;
    return imageContour ;
}

// Detection de contours par filtre laplacien
Mat ImageContourLaplace(const Mat image){
    return ImageConvolution(ImageMonochrome(image), GenererFiltre(2)) ;
}

// Rehaussement de contours (image + alpha * laplacien)
Mat ImageRehaussementContour(const Mat image, const int val){
    int c ;
    int nbComposante = image.channels() ;
    Mat imageComposante[nbComposante] ;
    Mat imageContour = ImageContourLaplace(image) ;
    Mat imageRehaussement ;
    vector<Mat> imageRehaussementComposante ;

    imageContour = imageContour / 255 ;

    if(nbComposante == 1){
        imageRehaussement = image + imageContour * val ;
    }else{
        split(image, imageComposante) ;
        for(c = 0 ; c < nbComposante ; c++) imageRehaussementComposante.push_back(imageComposante[c] + imageContour*val) ;
        merge(imageRehaussementComposante, imageRehaussement) ;
    }
    return imageRehaussement ;
}

} // namespace ImageToolBox
