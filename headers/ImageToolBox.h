/**
 * @file ImageToolBox.h
 * @brief Header du namespace ImageToolBox contenant les différentes fonctions de traitement d'images
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

// Systeme
#include <algorithm>
#include <vector>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


namespace ImageToolBox {
    using namespace std ;
    using namespace cv ;
    //------------------------------ Traitement de couleur
    /**
     * @brief Ajuster la temperature de l'image
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur de la température à ajuster
     * @return Image avec température ajustée
     */
    Mat ImageTemperature(const Mat image, const int valeur) ;               // Modifier la temperature de l'image
    
    /**
     * @brief Ajuster la saturation de l'image
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur de la saturation à ajuster
     * @return Image avec la saturation adjustée
     */
    Mat ImageSaturation(const Mat image, const int valeur) ;                // Modifier la saturation
    
    /**
     * @brief Ajuster la saturation d'une composante de l'image
     * @param image Composante de l'image de départ
     * @param valeur Valeur de la saturation à ajuster
     * @return Composante de l'image avec la saturation ajustée
     */
    Mat ImageSaturationElement(const Mat image, const int valeur) ;         // Calcul elementaire pour la modification la saturation
    
    /**
     * @brief Ajuster la teinte de l'image
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur de la teinte à ajuster
     * @return Image avec la tente ajustée
     */
    Mat ImageTeinte(const Mat image, const int valeur) ;                    // Modifier la teinte
    
    /**
     * @brief Ajuster la vividité de l'image
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur de la vividité à ajuster
     * @return Image avec la vividité ajustée
     */
    Mat ImageVividite(const Mat image, const int valeur) ;                  // Modifier la vividite

    //------------------------------ Filtre de couleur
    /**
     * @brief Convertir une image en couleurs en une image en niveau de gris
     * @param image Image de départ (dimensions quelconques)
     * @return Image en niveau de gris
     */
    Mat ImageMonochrome(const Mat image) ;						            // Convertir une image en niveau de gris
    
    /**
     * @brief Appliquer un filtre de couleur sépia sur l'image
     * @param image Image de départ (dimensions quelconques)
     * @return Image avec filtre sépia appliqué
     */
    Mat ImageSepia(const Mat image) ;                                       // Sepia
    
    /**
     * @brief Calculer la négative d'une composante de l'image de départ
     * @param image Composante de l'image de départ
     * @return Composante de l'image négative
     */
    Mat ImageInversementMono(const Mat image) ;				                // Inverser les intensites d'une composante de l'image
    
    /**
     * @brief Calculer l'image négative de l'image de départ
     * @param image Image de départ (dimensions quelconques)
     * @return Image négative
     */
    Mat ImageInversement(const Mat image) ;                                 // Inverser les intensites d'une image
    
    /**
     * @brief Extraire une composante de couleur (R/V/B) de l'image
     * @param image Image de départ (dimensions quelconques)
     * @param choix Choix de la composante à extraire : 1-Bleu, 2-Vert, 3-Rouge 
     * @return Composante de couleur choisie
     */
    Mat ImageExtractionCouleur(const Mat image, int choix) ;                // Extraction une composante de couleur de l'image
    
    /**
     * @brief Segmenter l'image en couleur rouge en annulant les composantes verte et bleue
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée en couleur rouge
     */
    Mat ImageRouge(const Mat image) ;                                       // Extraire la couleur rouge
    
    /**
     * @brief Segmenter l'image en couleur verte en annulant les composantes rouge et bleue
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée en couleur verte
     */
    Mat ImageVert(const Mat image) ;                                        // Extraire la couleur verte
    
    /**
     * @brief Segmenter l'image en couleur bleue en annulant les composantes rouge et verte
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée en couleur bleue
     */
    Mat ImageBleu(const Mat image) ;                                        // Extraire la couleur bleue
    
    /**
     * @brief Segmenter l'image en couleur jaune en annulant la composante bleue
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée en couleur jaune
     */
    Mat ImageJaune(const Mat image) ;                                       // Extraire la couleur jaune
    
    /**
     * @brief Segmenter l'image en couleur cyan en annulant la composante rouge
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée en couleur cyan
     */
    Mat ImageCyan(const Mat image) ;                                        // Extraire la couleur cyan
    
    /**
     * @brief Segmenter l'image en couleur magenta en annulant la composante verte
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée en couleur magenta
     */
    Mat ImageMagenta(const Mat image) ;                                     // Extraire la couleur magenta
    
    /**
     * @brief Pour chaque pixel de l'image, segmenter la composante ayant la plus grande valeur
     * @param image Image de départ (dimensions quelconques)
     * @return Image segmentée
     */
    Mat ImageRGB(const Mat image) ;                                         // Filtre RGB

    //------------------------------ Traitement a base d'histogramme
    /**
     * @brief Normaliser l'histogramme d'une composante de l'image avec intervalle réglable
     * @param image Composante de l'image
     * @param valeur Paramètre ajustable pour modifier l'intervalle
     * @return Composante de l'image avec histogramme normalisé 
     */
    Mat NormalisationMono(const Mat image, const int valeur) ;              // Normalisation de l'histogramme d'une composante de l'image
    
    /**
     * @brief Normaliser l'histogramme de l'image avec intervalle réglable
     * @param image Composante de l'image
     * @param valeur Paramètre ajustable pour modifier l'intervalle
     * @return Image avec histogramme normalisé 
     */
    Mat Normalisation(const Mat image, const int valeur) ;                  // Normalisation de l'histogramme d'une image avec valeur reglable
    
    /**
     * @brief Égaliser l'histogramme d'une composante de l'image avec nombre de pics réglable
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Paramètre ajustable pour modifier le nombre de pics de l'histogramme du résultat
     * @return Composante de l'image avec histogramme égalisé
     */
    Mat ImageEgalisationMono(const Mat image, const int valeur) ;           // Egalisation de l'histogramme d'une composante de l'image avec valeur reglable
    
    /**
     * @brief Égaliser l'histogramme de l'image sur 255 pics
     * @param image Image de départ (dimensions quelconques)
     * @return Image avec histogramme égalisé
     */
    Mat ImageEgalisation(const Mat image) ;                                 // Egalisation de l'histogramme d'une image
    
    /**
     * @brief Modifier l'intensité lumineuse de l'image en décalant son histogramme par un paramètre ajustable
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur de l'intensité pour l'ajustement
     * @return Image avec l'intensité lumineuse modifiée
     */
    Mat ImageLuminosite(const Mat image, const int valeur) ;                // Modifier l'intensite lumineuse de l'image
    
    /**
     * @brief Décaler la partie des niveaux d'intensité lumineuse inférieurs à 128 de l'histogramme de l'image en fonction d'un paramètre réglable
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur pour l'ajustement de l'histogramme
     * @return Image modifiée
     */
    Mat ImageOmbre(const Mat image, int valeur) ;                           // Modifier les ombres de l'image
    
    /**
     * @brief Décaler la partie des niveaux d'intensité lumineuse supérieurs à 128 de l'histogramme de l'image en fonction d'un paramètre réglable
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Valeur pour l'ajustement de l'histogramme
     * @return Image modifiée
     */
    Mat ImageHauteLumiere(const Mat image, int valeur) ;                    // Modifier les hautes lumieres de l'image

    //------------------------------ Filtrage
    /**
     * @brief Appliquer le filtre médian 3x3 sur une composante de l'image
     * @param image Image de départ (dimensions quelconques)
     * @return Composante filtrée par le filtre médian 3x3
     */
    Mat ImageMedianMono(const Mat image) ;                                  // Filtrer par filtre median pour une composante de l'image 
    
    /**
     * @brief Appliquer un filtre médian 3x3 sur l'image de façon marginale
     * @param image Image de départ (dimensions quelconques)
     * @return Image filtrée par le filtre médian 3x3 de façon marginale
     */
    Mat ImageMedian(const Mat image) ;                                      // Filtrer l'image par filtre median 
    
    /**
     * @brief Appliquer par convolution sur l'image un filtre 3x3 choisi en fonction d'un paramètre de choix
     * @param image Image de départ (dimensions quelconques)
     * @param choix Choix du filtre à appliquer : 1-Filtre moyenneur (3x3), 2-Filtre gaussien (3x3), 3-Filtre médian (3x3)
     * @return Image filtrée
     */
    Mat ImageFiltrage(const Mat image, const int choix) ;                   // Filtrage
    
    /**
     * @brief Lisser plusieurs fois les contours de l'image par application sucessive d'un filtre gaussien
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Nombre d'itérations de lissage
     * @return Image avec les contours lissés
     */
    Mat ImageLissage(const Mat image, const int valeur) ;                   // Lisser l'image
    
    /**
     * @brief Appliquer un filtre Kuwahara-Nagao (5x5) sur une composante de l'image
     * @param image Image de départ (dimensions quelconques)
     * @return Composante de l'image filtrée par le filtre Kuwahara-Nagao (5x5)
     */
    Mat ImageKuwaharaMono(const Mat image) ;                                // Filtrer par filtre Kuwahara-Nagao sur une composante de l'image
    
    /**
     * @brief Appliquer le filtre Kuwahara-Nagao (5x5) sur l'image
     * @param image Image de départ (dimensions quelconques)
     * @return Image filtrée par le filtre Kuwahara-Nagao (5x5)
     */
    Mat ImageKuwahara(const Mat image) ;                                    // Filtrer l'image par filtre Kuwahara-Nagao

    //------------------------------ Bruitage
    /**
     * @brief Ajouter un bruit gaussien de moyenne et écart-type réglable sur l'image
     * @param image Image de départ (dimensions quelconques)
     * @param moyenne Moyenne du bruit
     * @param sigma Écart-type du bruit
     * @return Image bruitée par le bruit gaussien paramétré
     */
    Mat ImageBruitGaussien(const Mat image, const double moyenne, const double sigma) ;             // Ajouter du bruit gaussien sur l'image
    
    /**
     * @brief Bruiter l'image par un bruit poivre et sel
     * @param image Image de départ (dimensions quelconques)
     * @return Image bruitée par le bruit poivre et sel
     */
    Mat ImageBruitPoivreSel(const Mat image) ;                              // Ajouter du bruit poivre et sel dans l'image

    //------------------------------ Contours
    /**
     * @brief Détecter les contours dans l'image par la norme du gradient (filtres gradients 3x3)
     * @param image Image de départ (dimensions quelconques)
     * @return Image des contours dans l'image de départ
     */
    Mat ImageContourGradient(const Mat image) ;                             // Detection de contours par filtres gradients
    
    /**
     * @brief Détecter les contours dans l'image par un filtre laplacien (3x3)
     * @param image Image de départ (dimensions quelconques)
     * @return Image des contours dans l'image de départ
     */
    Mat ImageContourLaplace(const Mat image) ;                              // Detection de contours par filtre laplacien
     
    /**
     * @brief Rehausser les contours de l'image en rajoutant d'une valeur d'intensité lumineuse sur les pixels des contours détéctés par un filtre choisi (3x3)
     * @param image Image de départ (dimensions quelconques)
     * @param val Valeur d'intensité à ajouter sur les contours
     * @param choix Choix du filtre pour la détection des contours : 1-Gradient, 2-Laplacien
     * @return Image avec les contours accentués
     */
    Mat ImageRehaussementContour(const Mat image, const int val) ;                 // Rehaussement de contours

    //------------------------------ Seuillage
    /**
     * @brief Seuiller d'une composante de l'image avec un seui de valeur réglable
     * @param image Composante de l'image de départ
     * @param seuil Valeur du seuil
     * @return Composante de l'image seuillée
     */
    Mat ImageSeuillage(const Mat image, const int seuil) ;                  // Seuillage simple d'une composante de l'image 
    
    /**
     * @brief Seuiller une image avec des seuils réglables pour chauqe composante
     * @param image Image de départ (dimensions quelconques)
     * @param seuil Les valeurs du seuil pour chaque composante de l'image
     * @return Image seuillée
     */
    Mat ImageSeuillage(const Mat image, vector<int> seuil) ;                // Seuillage simple d'une image
    
    /**
     * @brief Seuiller de façon hystérésis une composante de l'image avec les valeurs des seuils réglables
     * @param image Composante de l'image de départ
     * @param seuilBas Valeur du seuil bas
     * @param seuilHaut Valeur du seuil haut
     * @return Composante de l'image seuillée
     */
    Mat ImageSeuillage(const Mat image, const int seuilBas, const int seuilHaut) ;                  // Seuillage par hysteresis d'une composante de l'image
    
    /**
     * @brief Seuiller de façon hystérésis une image avec les valeurs des seuils réglables pour chaque composante
     * @param image Image de départ (dimensions quelconques)
     * @param seuilBas Les valeurs des seuils bas pour chaque composante de l'image
     * @param seuilHaut Les valeurs des seuils hauts pour chaque composante de l'image
     * @return Image seuillée
     */
    Mat ImageSeuillage(const Mat image, vector<int> seuilBas, vector<int> seuilHaut) ;              // Seuillage par hysteresis d'une image 

    //------------------------------ Segmentation
    /**
     * @brief Segmenter une composante de l'image par un seuillage simple avec la valeur du seuil réglable
     * @param image Composante de l'image de départ 
     * @param seuil Valeur du seuil
     * @return Composante de l'image segmentée
     */
    Mat ImageSegmentation(const Mat image, const int seuil) ;                                       // Segmentation d'une composante de l'image epar seuillage simple
    
    /**
     * @brief Segmenter une image par un seuillage simple avec les valeurs des seuils réglables pour chaque composante
     * @param image Image de départ (dimensions quelconques)
     * @param seuil Valeurs des seuils de chaque composante
     * @return Image segmentée
     */
    Mat ImageSegmentation(const Mat image, vector<int> seuil) ;                                     // Segmentation d'une image par seuillage simple
    
    /**
     * @brief Segmenter une composante de l'image par un seuillage à hystérésis avec les valeurs des seuils réglables
     * @param image Composante de l'image de départ
     * @param seuilBas Valeur du seuil bas
     * @param seuilHaut Valeur du seuil haut
     * @return Image segmentée
     */
    Mat ImageSegmentation(const Mat image, const int seuilBas, const int seuilHaut) ;               // Segmentation d'une composante de l'image par seuillage hysteresis
    
    /**
     * @brief Segmenter une image par un seuillage à hystérésis avec les valeurs des seuils réglables pour chaque composante
     * @param image Image de départ (dimensions quelconques)
     * @param seuilBas Valeurs des seuils bas de chaque composante
     * @param seuilHaut Valeurs des seuils hauts de chaque composante
     * @return Image segmentée
     */
    Mat ImageSegmentation(const Mat image, vector<int> seuilBas, vector<int> seuilHaut) ;           // Segmentation d'une image par seuillage hysteresis

    //------------------------------ Sous-echantillonnage, interpolation et quantification
    /**
     * @brief Modifier le nombre de bits à coder les intensités lumineuses de l'image
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Nombre de bits à coder
     * @return Image quantifiée
     */
    Mat ImageQuantification(const Mat image, const int valeur) ;            // Quantification
    
    /**
     * @brief Réduire la résolution d'une image par sous-échantillonnage
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Pas de sous-échantillonnage
     * @return Image sous-échantillonnée
     */
    Mat ImageResolutionReduction(const Mat image, const int valeur) ;       // Sous-echantillonnage de l'image
    
    /**
     * @brief Augmenter la résolution d'une image par interpolation par pixel le plus proche
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Échelle d'agrandissement
     * @return Image agrandie
     */
    Mat InterpolationPPP(const Mat image, const int valeur) ;               // Interpolation par pixel le plus proche
    
    /**
     * @brief Augmenter la résolution d'une image par interpolation bilinéaire
     * @param image Image de départ (dimensions quelconques)
     * @param valeur Échelle d'agrandissement
     * @return Image agrandie
     */
    Mat InterpolationBilineaire(const Mat image, const int valeur) ;        // Interpolation bilineaire d'une image
    
    /**
     * @brief Augmenter la résolution d'une composante de l'image par interpolation bilinéaire
     * @param image Composante de l'image de départ
     * @param valeur Échelle d'agrandissement
     * @return Composante de l'image agrandie
     */
    Mat InterpolationBilineaireMono(const Mat image, const int valeur) ;    // Interpolation bilineaire d'une composante de l'image

    //------------------------------ Operations mathematiques
    /**
     * @brief Déterminer la valeur minimale d'une matrice de type entier
     * @param image Matrice à déterminer la valeur minimale
     * @return Valeur minimale de la matrice
     */
    int MinImage(const Mat image) ;                                         // Determiner la valeur minimum dans une matrice entiere
    
    /**
     * @brief Déterminer la valeur maximale d'une matrice de type entier
     * @param image Matrice à déterminer la valeur maximale
     * @return Valeur maximale de la matrice
     */
    int MaxImage(const Mat image) ;                                         // Determiner la valeur maximum dans une matrice entiere
    
    /**
     * @brief Déterminer la valeur minimale d'un vecteur de type réel
     * @param vecteur Vecteur à déterminer la valeur minimale
     * @return Valeur minimale du vecteur
     */
    double MinVecteur(const vector<double> vecteur) ;                       // Determiner la valeur minimum dans un vecteur reel
    
    /**
     * @brief Déterminer la valeur maximale d'un vecteur de type réel
     * @param vecteur Vecteur à déterminer la valeur maximale
     * @return Valeur maximale du vecteur
     */
    double MaxVecteur(const vector<double> vecteur) ;                       // Determiner la valeur maximum dans un vecteur reel
    
    /**
     * @brief Déterminer la valeur minimale d'un vecteur de type entier
     * @param vecteur Vecteur à déterminer la valeur minimale
     * @return Valeur minimale du vecteur
     */
    int MinVecteur(const vector<int> vecteur) ;                             // Determiner la valeur minimum dans un vecteur entier
    
    /**
     * @brief Déterminer la valeur maximale d'un vecteur de type entier
     * @param vecteur Vecteur à déterminer la valeur maximale
     * @return Valeur maximale du vecteur
     */
    int MaxVecteur(const vector<int> vecteur) ;                             // Determiner la valeur maximum dans un vecteur entier
    
    /**
     * @brief Déterminer l'indice de la valeur minimale d'un vecteur de type entier
     * @param vecteur Vecteur à déterminer l'indice de la valeur minimale
     * @return Indice de la valeur minimale du vecteur
     */
    int MinVecteurIndice(const vector<int> vecteur) ;                       // Determiner l'indice de la valeur minimum dans un vecteur entier
    
    /**
     * @brief Déterminer l'indice de la valeur minimale d'un vecteur de type réel
     * @param vecteur Vecteur à déterminer l'indice de la valeur minimale
     * @return Indice de la valeur minimale du vecteur
     */
    int MinVecteurIndice(const vector<double> vecteur) ;                    // Determiner l'indice de la valeur minimum dans un vecteur reel
   
    /**
     * @brief Calculer l'histogramme d'une image avec le nombre d'intervalles réglable
     * @param image Image à calculer l'histogramme
     * @param nbIntervalle Nombre d'intervalle de l'histogramme
     * @return Occurences de l'histogramme de l'image
     */
    vector<int> Histogramme(const Mat image, const int nbIntervalle) ;	    // Histogramme
    
    /**
     * @brief Égaliser d'un histogramme sur un nombre d'intervalles réglable
     * @param image Image de départ (dimensions quelconques)
     * @param nbIntervalle Nombre d'intervalles à égaliser
     * @return Occurences de l'histogramme égalisé
     */
    vector<int> Egalisation(const Mat image, const int nbIntervalle) ;      // Egalisation de l'histogramme
   
    /**
     * @brief Générer une matrice de dimensions choisies contenant que des zéros
     * @param nbLigne Nombre de lignes de la matrice
     * @param nbColonne Nombre de colonne de la matrice
     * @return Matrice des zéros de dimensions choisies
     */
    Mat ImageZero(const int nbLigne, const int nbColonne) ; 	            // Generer une matrice zero
   
    /**
     * @brief Effectuer un produit de convolution entre une composante de l'image et un filtre
     * @param image Composante de l'image de départ (dimensions quelconques)
     * @param filtre Filtre (dimensions quelconques)
     * @return Composante de l'image convoluée par le filtre 
     */
    Mat MatriceConvolution(const Mat image, const Mat filtre) ;             // Produit de convolution entre deux matrices
    
    /**
     * @brief Effectuer un produit de convolution entre une image et un filtre
     * @param image Image de départ (dimensions quelconques)
     * @param filtre Filtre (dimensions quelconques)
     * @return Image convoluée par le filtre 
     */
    Mat ImageConvolution(const Mat image, const Mat filtre) ;	            // Produit de convolution entre deux images
   
    /**
     * @brief Calculer la norme d'une image avec leurs gradients dans les deux directions horizontale et verticale
     * @param imageX Gradient de l'image dans la direction horizontale
     * @param imageY Gradient de l'image dans la direction verticale
     * @return Image de la norme du gradient de l'image
     */
    Mat MatriceNorme(const Mat imageX, const Mat imageY) ;                  // Calculer la norme
    
    /**
     * @brief Calculer le gradient de l'image dans la direction horizontale
     * @param image Image de départ (dimensions quelconques)
     * @param typeFiltre Type de filtre gradient horizontal (3x3) : "Simple"-Filtre gradient par définition, "Sobel"-Filtre gradient de Sobel
     * @return Gradient de l'image dans la direction horizontale
     */
    Mat GradientX(const Mat image, const string typeFiltre) ;               // Gradient en X
    
    /**
     * @brief Calculer le gradient de l'image dans la direction verticale
     * @param image Image de départ (dimensions quelconques)
     * @param typeFiltre Type de filtre gradient vertical (3x3) : "Simple"-Filtre gradient par définition, "Sobel"-Filtre gradient de Sobel
     * @return Gradient de l'image dans la direction verticale
     */
    Mat GradientY(const Mat image, const string typeFiltre) ;               // Gradient en Y
    
    /**
     * @brief Déterminer le maximum de la norme du gradient dans la direction du gradient de l'image
     * @param gradientNorme Norme du gradient de l'image
     * @param gradientX Gradient dans la direction horizontale de l'image
     * @param gradientY Gradient dans la direction verticale de l'image
     * @return Maximum de la norme du gradient dans la direction du gradient de l'image
     */
    Mat MaxNormeGradient(const Mat gradientNorme, const Mat gradientX, const Mat gradientY) ;       // Maximum de la norme du gradient
    
    /**
     * @brief Déterminer la valeur médiane d'une matrice (dimensions quelconques)
     * @param matrice Matrice à déterminer sa valeur médiane (dimensions quelconques)
     * @return Valeur médiane de la matrice
     */
    int MatriceMedian(const Mat matrice) ;                                  // Determiner la valeur mediane d'une matrice
    
    /**
     * @brief Déterminer la valeur médiane d'un vecteur
     * @param vecteur Vecteur à déterminer la valeur médiane (dimensions quelconques)
     * @return Valeur médiane du vecteur
     */
    int VecteurMedian(vector<int> vecteur) ;                                // Determiner la valeur mediane d'un vecteur
    
    /**
     * @brief Vérifier l'égalité entre deux matrices de mêmes dimensions
     * @param matrice1 Première matrice de la comparaison
     * @param matrice2 Seconde matrice de la comparaison
     * @return "True" si les matrices sont égales, "False" sinon
     */
    bool MatriceEgale(const Mat matrice1, const Mat matrice2) ;             // Verifier l'egalite entre deux matrices de memes dimensions
    
    /**
     * @brief Calculer la moyenne des valeurs d'un vecteur réel
     * @param vecteur Vecteur à calculer sa moyenne (dimensions quelconques)
     * @return Moyenne des valeurs du vecteur
     */
    double MoyenneVecteur(const vector<double> vecteur) ;                   // Calculer la moyenne d'un vecteur
    
    /**
     * @brief Calculer la variance des valeurs d'un vecteur réel
     * @param vecteur Vecteur à calculer sa variance (dimensions quelconques)
     * @return Variance des valeurs du vecteur
     */
    double VarianceVecteur(const vector<double> vecteur) ;                  // Calculer la variance d'un vecteur
    
    /**
     * @brief Calculer la trasformée de Fourier 2D d'une image
     * @param image Image à calculer sa transformée de Fourier
     * @return Représentation du module de la transformée de Fourier 2D de l'image
     */
    Mat ImageFourier(const Mat image) ;                                     // Transformee de Fourier 2D de l'image (module)
    
    /**
     * @brief Calculer la transformée de Fourier 2D d'un pixel de l'image
     * @param image Image contenant le pixel à calculer sa stransformée de Fourier 2D
     * @param ligne Position en ligne du pixel courant
     * @param colonne Position en colonne du pixel courant
     * @return Module de la transformée de Fourier 2D du pixel
     */
    double ImageFourierElement(const Mat image, const int ligne, const int colonne) ;               // Element de calcul de la transformee de Fourier 2D de l'image (module)

    //------------------------------ Autres
    
    /**
     * @brief Effectuer l'effet miroir sur une image en basant sur ses dimensions et les dimensions du filtre à appliquer sur l'image
     * @param image Image de départ (dimensions quelconques)
     * @param filtre Filtre à appliquer sur l'image de départ (dimensions quelconques)
     * @return Image de départ avec l'effet de miroir appliqué
     */
    Mat ImageMiroir(const Mat image, const Mat filtre) ;	                // Effet miroir - Filtre donne
    
    /**
     * @brief Effectuer l'effet miroir sur une image avec un voisinage carré de taille quelconque
     * @param image Image de départ
     * @param taille Taille du voisinage
     * @return Image de départ avec l'effet de miroir appliqué
     */
    Mat ImageMiroir(const Mat image, const int taille) ;		            // Effet miroir - Taille donnee
    
    /**
     * @brief Générer un filtre 3x3 selon le type de filtre choisi
     * @param typeFiltre Type du filtre (3x3) à générer : 1-Moyenneur, 2-Laplacien, 3-Gaussien, 4-Gradient horizontal (Sobel), 5-Gradient vertical (Sobel), 6-Gradient horizontl (définition), 7-Gradient vertical (définition)
     * @return Filtre 3x3 correspondant au type choisi
     */
    Mat GenererFiltre(const int typeFiltre) ;					            // Generer les filtres
    
    /**
     * @brief Transformer une composante de l'image en une image de trois composantes identiques
     * @param image Composante de départ de l'image
     * @return Image de trois composantes identiques, égales à la composante de départ
     */
    Mat MonoCouleur(const Mat image) ;                                      // Convertir le format d'une image monochrome en format RVB

    /**
     * @brief Calculer et tracer l'histogramme d'une composante choisie de l'image
     * @param image Image de départ
     * @param choixCouleur Choix de la composante de l'image à calculer et tracer son histogramme
     * @return Image de l'histogramme de la composante choisie de l'image
     */
    Mat PlotHistogram1D(Mat image,int choixCouleur) ;                       // Image du Graphe Histogramme en niveau de gris
    
    /**
     * @brief Calculer l'histogramme de l'image
     * @param image Image à calculer son histogramme
     * @return 
     */
    Mat CalcHistogram(Mat image) ;                                          // Calcul d'histogramme
    
    /**
     * @brief PlotHistogram
     * @param image
     * @return
     */
    Mat PlotHistogram(Mat image) ;                                          // Image du Graphe Histogramme
    
    /**
     * @brief Vérifier si une image est en niveau de gris
     * @param image Image de départ
     * @param imageVerifiee Image après la vérification : Si l'image est en niveau de gris, elle reçoit une composante de l'image de départ. Sinon, elle reçcoit entièrement l'image de départ
     * @return "True" si l'image est en niveau de gris, "False" sinon
     */
    bool VerifierImage(const Mat image, Mat& imageVerifiee) ;               // Verifier si l'image est en couleurs ou en niveau de gris
    
    /**
     * @brief Réarranger les composantes de l'image de BGR vers RGB
     * @param image Image de départ
     * @return Image avec les composantes réarrangées
     */
    Mat ImageBGRRGB(const Mat image) ;                                      // Echanger les composantes R et B de l'image
    
    /**
     * @brief Vérifier la saturation de la valeur d'un pixel de l'image
     * @param valeur Valeur du pixel à vérifier la saturation
     * @return Retourner 0 si la valeur est négative, 255 si la valeur est supérieure à 255, valeur actuelle sinon
     */
    int VerifierSaturation(const int valeur) ;                              // Verifier la saturation
    
    /**
     * @brief Arranger la représentation de la transformee de Fourier 2D de l'image
     * @param image Image de la représentation de la transformée de Fourier 2D de l'image
     * @return Image arrangée de la représentation de la transformée de Fourier 2D de l'image
     */
    Mat ImageFourierArranger(const Mat image) ;                             // Arranger la representation de la transformee de Fourier
    /**
     * @brief Transformee de Hough
     * @param image
     * @return une image avec les lignes detectees et tracees
     */
    Mat TransformeedeHough(const Mat image,int num_theta,int threshold_acc); // Détection de ligne dans une images

    /**
     * @brief Segmenter l'image par la méthode K-means
     * @param image Image de départ (BGR ou niveaux de gris)
     * @param k Nombre de classes (clusters), k >= 2
     * @return Image segmentée : chaque pixel est remplacé par la couleur moyenne de son cluster
     */
    Mat ImageKMeans(const Mat image, const int k) ;
};
