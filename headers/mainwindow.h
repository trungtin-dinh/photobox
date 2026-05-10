/**
 * @file mainwindow.h
 * @brief Header de la classe MainWindow de QT
 * @version 0.1
 * @date 2022-01-22
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include "headers/ImageToolBox.h"
#include "headers/bibliotheque.h"

using namespace std;
using namespace ImageToolBox;

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    /**
     * @brief Slot du bouton identification : Menu Identification
     */
    void on_pushButtonIdentifier_clicked();
    /**
     * @brief Slot du bouton Quitter : Menu Identification
     */
    void on_pushButtonQuitter_clicked();
    /**
     * @brief Slot du bouton Charger une bibliothèque : Menu Pricipal
     */
    void on_pushButtonChargerBiblio_clicked();
    /**
     * @brief Slot du bouton Retour : Menu Pricipal
     */
    void on_pushButtonRetourIdentification_clicked();
    /**
     * @brief Slot Selection d'une image de la bibliotheque : Menu Bibliothèque
     */
    void on_tableBiblioRowClicked(int , int);
    /**
     * @brief Slot Bouton Sauvegarder la bibliotheque : Menu Bibliothèque
     */
    void on_pushButtonSauvegarder_clicked();
    /**
     * @brief Slot Supprimer l'image selectionnee de la bibliotheque : Menu Bibliothèque
     */
    void on_pushButtonSupprimerImage_clicked();
    /**
     * @brief Slot Bouton de Retour : Menu Bibliotheque
     */
    void on_pushButtonRetourMenuPrincipal_clicked();
    /**
     * @brief Slot liée à un clique sur la touche entrée après avoir écrit le mdp : Envoi un signal click sur le bouton S'identifier
     */
    void on_lineEditMdp_returnPressed();
    /**
     * @brief Slot Critère tri qui déclence le tri de la bibliotheque à chaque fois que le critère a changé
     */
    void on_comboBoxTrierIndexChanged(int);
    /**
     * @brief Slot comboBox Critère Cout qui construit la sous-liste à chaque fois que le critère a changé
     */
    void on_comboBoxCritereCoutIndexChanged(int);
    /**
     * @brief Slot comboBox Critère Date d'Ajout qui construit la sous-liste à chaque fois que le critère a changé
     */
    void on_comboBoxCritereDateAjoutIndexChanged(int);
    /**
     * @brief Slot Bouton Ajouter une image à la bibliothèque chargé : Menu Ajout Image
     */
    void on_pushButtonAjouterImage_clicked();
    /**
     * @brief Slot Bouton Annuler l'ajout de l'image et retourner au Menu Bibliothèque
     */
    void on_pushButtonAjoutImageAnnuler_clicked();
    /**
     * @brief Slot Ajouter l'image choisi à la bibliothèque après vérification de son descripteur rempli par l'utilisateur : Menu Ajout Image
     */
    void on_pushButtonAjoutImageAjouter_clicked();
    /**
     * @brief Slot Bouton Creer une nouvelle bibliotheque : Menu Principal
     */
    void on_pushButtonCreerBiblio_clicked();
    /**
     * @brief Slot Bouton Sauvegarder la sous-liste en tant que bibliothèque dans un nouveau fichier : Menu Bibliothèque
     */
    void on_pushButtonSauvegarderSousListe_clicked();
    /**
     * @brief Slot Bouton Ouvrir l'image sélectionnée de la bibliothèque : Menu Bibliothèque
     */
    void on_pushButtonOuvrirImage_clicked();
    /**
     * @brief Slot Bouton Retour : Menu Modification Image
     */
    void on_pushButtonRetourMenuModificationImage_clicked();
    /**
     * @brief Slot SpinBox de la valeur min  du critère de construction de sous-liste : Menu Bibliotheque
     */
    void on_doubleSpinBoxMin_valueChanged(double);
    /**
     * @brief Slot SpinBox de la valeur max  du critère de construction de sous-liste : Menu Bibliotheque
     */
    void on_doubleSpinBoxMax_valueChanged(double);

    // Page Consultation Image --------------------------------
    // Agrandissement de l'image
    /**
     * @brief Slot Slider lié au zoom de l'image affiché : Menu Consultation Image
     */
    void on_horizontalSlider_agrandissementNonModifiable_valueChanged(int) ;
    // Page Image modifiable--------------------------------
    // Bouton de sauvegarder les modifications
    /**
     * @brief Slot Bouton Sauvegarder Descripteur de l'image selectionnée : Menu Modification Image
     */
    void on_pushButton_modifier_clicked() ;
    // Agrandissement de l'image
    /**
     * @brief Slot Slider lié au zoom de l'image affiché : Menu Modification Image
     */
    void on_horizontalSlider_agrandissement_valueChanged(int) ;
    // Passer a la page des traitements de l'image choisie
    /**
     * @brief Slot Bouton Traiter l'image selectionnée : Menu Modification Image
     */
    void on_pushButton_traitementImage_clicked() ;

    // Page Traitement--------------------------------
    // -------------Standars-------------

    // Retour a la page pour choisir l'image a traiter
    /**
     * @brief Retour a la page pour choisir l'image a traiter
     */
    void on_pushButton_retour_clicked() ;
    // Agrandissement de l'image originale
    /**
     * @brief Agrandissement de l'image originale
     */
    void on_horizontalSlider_imageOriginale_valueChanged(int) ;
    // Agrandissement de l'image traitee
    /**
     * @brief  Agrandissement de l'image traitee
     */
    void on_horizontalSlider_imageTraitee_valueChanged(int) ;
    // Reinitialiser l'image originale
    /**
     * @brief Reinitialiser l'image originale
     */
    void on_pushButton_traitementReinitialiser_clicked() ;
    // Sauvegarder temporellement l'image de resultat
    /**
     * @brief Sauvegarder temporellement l'image de resultat
     */
    void on_pushButton_traitementAppliquer_clicked() ;
    // Sauvegarder l'image traitee
    /**
     * @brief Sauvegarder l'image traitée : Menu Traitment Image
     */
    void on_pushButton_traitementSauvegarder_clicked();
    // Afficher l'image traitee
    /**
     * @brief Afficher Image Traitée (version agrandi)
     */
    void on_pushButtonAfficherImageTraitee_clicked();

    // -------------Luminosite-------------

    // Initialiser
    /**
     * @brief Initialiser Luminosite
     */
    void on_groupBox_correction_clicked() ;
    // Bouton : Intensite lumineuse
    /**
     * @brief Bouton : Intensite lumineuse
     */
    void on_radioButton_luminosite_clicked() ;
    // Intensite lumineuse
    /**
     * @brief Intensite lumineuse
     */
    void on_horizontalSlider_luminosite_valueChanged(int) ;
    // Bouton : Contraste
    /**
     * @brief Bouton : Contraste
     */
    void on_radioButton_contraste_clicked() ;
    // Contraste
    /**
     * @brief Contraste
     */
    void on_horizontalSlider_contraste_valueChanged(int) ;
    // Bouton : Ombre
    /**
     * @brief  Bouton : Ombre
     */
    void on_radioButton_ombre_clicked() ;
    // Ombre
    /**
     * @brief Ombre
     */
    void on_horizontalSlider_ombre_valueChanged(int) ;
    // Bouton : Haute lumiere
    /**
     * @brief Slot Bouton : Haute lumiere
     */
    void on_radioButton_brillance_clicked() ;
    // Haute lumiere
    /**
     * @brief Slot Slider Haute lumiere
     */
    void on_horizontalSlider_brillance_valueChanged(int) ;

    // -------------Contours-------------

    // Initialiser
    /**
     * @brief Initialiser Details
     */
    void on_groupBox_details_clicked() ;
    // Bouton : Nettete
    /**
     * @brief Bouton : Nettete
     */
    void on_radioButton_nettete_clicked() ;
    // Nettete
    /**
     * @brief Nettete
     */
    void on_horizontalSlider_nettete_valueChanged(int) ;
    // Bouton : Contours
    /**
     * @brief Initialiser Contours
     */
    void on_groupBox_contours_clicked() ;
    // Contours par filtres gradients
    /**
     * @brief Contours par filtres gradients
     */
    void on_radioButton_contoursGradient_clicked() ;
    // Contours par filtre laplacien
    /**
     * @brief Contours par filtre laplacien
     */
    void on_radioButton_contoursLaplacien_clicked() ;

    // -------------Filtres de couleur-------------

    // Initialiser
    /**
     * @brief Initialiser Filtres de couleur
     */
    void on_groupBox_filtres_clicked() ;
    // Image originale
    /**
     * @brief Image originale
     */
    void on_radioButton_originale_clicked() ;
    // Image en niveau de gris
    /**
     * @brief Image en niveau de gris
     */
    void on_radioButton_niveauGris_clicked() ;
    // Image inversee
    /**
     * @brief Image inversee
     */
    void on_radioButton_inversement_clicked() ;
    // Image rouge
    /**
     * @brief Image rouge
     */
    void on_radioButton_rouge_clicked() ;
    // Image verte
    /**
     * @brief Image verte
     */
    void on_radioButton_vert_clicked() ;
    // Image bleue
    /**
     * @brief Image bleue
     */
    void on_radioButton_bleu_clicked() ;
    // Image jaune
    /**
     * @brief Image jaune
     */
    void on_radioButton_jaune_clicked() ;
    // Image cyane
    /**
     * @brief Image cyane
     */
    void on_radioButton_cyan_clicked() ;
    // Image sepia
    /**
     * @brief  Image sepia
     */
    void on_radioButton_sepia_clicked() ;
    // Image magenta
    /**

     * @brief  Image magenta
     */
    void on_radioButton_magenta_clicked() ;
    // Image RGB
    /**
     * @brief Image RGB
     */
    void on_radioButton_rgb_clicked() ;
    // Egalisation
    /**
     * @brief Egalisation d'image
     */
    void on_radioButtonEgalisation_clicked() ;
    // Bruit poivre et sel
    /**
     * @brief Bruit poivre et sel
     */
    void on_radioButtonBruitPoivreSel_clicked() ;

    // -------------Extraction du canal de couleur-------------

    // Initialiser
    /**
    * @brief Initialiser Extraction du canal de couleur
    */
   void on_groupBox_extractionRVB_clicked();
    // Extraction canal rouge
   /**
     * @brief Extraction canal rouge
     */
    void on_radioButton_extractionR_clicked() ;
    // Extraction canal vert
    /**
     * @brief Extraction canal vert
     */
    void on_radioButton_extractionV_clicked() ;
    // Extraction canal bleu
    /**
     * @brief Extraction canal bleu
     */
    void on_radioButton_extractionB_clicked() ;

    // -------------Seuillage & Segmentation-------------

    // Initialiser
    /**
     * @brief Initialiser Seuillage & Segmentation
     */
    void on_groupBox_seuillageSegmentation_clicked();
    // Seuillage
    /**
     * @brief Seuillage
     */
    void on_radioButton_seuillage_clicked() ;
    // Segmentation
    /**
     * @brief Segmentation
     */
    void on_radioButton_segmentation_clicked() ;
    // Seuils simples
    /**
     * @brief Seuils simples
     */
    void on_radioButton_seuillageSimple_clicked() ;
    // Seuils par hysteresis
    /**
     * @brief Seuils par hysteresis
     */
    void on_radioButton_seuillageHysteresis_clicked() ;
    // Seuil bas rouge
    /**
     * @brief Seuil bas rouge
     */
    void on_verticalSlider_seuilBasR_2_valueChanged(int) ;
    // Seuil bas vert
    /**
     * @brief Seuil bas vert
     */
    void on_verticalSlider_seuilBasV_2_valueChanged(int) ;
    // Seuil bas bleu
    /**
     * @brief Seuil bas bleu
     */
    void on_verticalSlider_seuilBasB_2_valueChanged(int) ;
    // Seuil haut rouge
    /**
     * @brief Seuil haut rouge
     */
    void on_verticalSlider_seuilHautR_2_valueChanged(int) ;
    // Seuil haut vert
    /**
     * @brief Seuil haut vert
     */
    void on_verticalSlider_seuilHautV_2_valueChanged(int) ;
    // Seuil haut bleu
    /**
     * @brief Seuil haut bleu
     */
    void on_verticalSlider_seuilHautB_2_valueChanged(int) ;

    // Reinitialiser seuil bas rouge
    /**
     * @brief Reinitialiser seuil bas rouge
     */
    void on_pushButton_seuilBasR_clicked() ;
    // Reinitialiser seuil bas vert
    /**
     * @brief Reinitialiser seuil bas vert
     */
    void on_pushButton_seuilBasV_clicked() ;
    // Reinitialiser seuil bas bleu
    /**
     * @brief Reinitialiser seuil bas bleu
     */
    void on_pushButton_seuilBasB_clicked() ;
    // Reinitialiser seuil haut rouge
    /**
     * @brief Reinitialiser seuil haut rouge
     */
    void on_pushButton_seuilHautR_clicked() ;
    // Reinitialiser seuil haut vert
    /**
     * @brief Reinitialiser seuil haut vert
     */
    void on_pushButton_seuilHautV_clicked() ;
    // Reinitialiser seuil haut bleu
    /**
     * @brief Reinitialiser seuil haut bleu
     */
    void on_pushButton_seuilHautB_clicked() ;

    //-------------Resolution-------------

    // Initialiser
    /**
     * @brief Initialiser Resolution
     */
    void on_groupBox_resolutionQuantification_clicked();
    // Bouton : Resolution
    /**
     * @brief Bouton : Resolution
     */
    void on_radioButton_resolution_clicked();
    // Bouton : Interpolation par pixel le plus proche
    /**
     * @brief Bouton : Interpolation par pixel le plus proche
     */
    void on_radioButton_PPP_clicked() ;
    // Bouton : Interpolation bilineaire
    /**
     * @brief Bouton : Interpolation bilineaire
     */
    void on_radioButton_bipolaire_clicked() ;
    // Resolution
    /**
     * @brief Resolution
     */
    void on_horizontalSlider_resolution_valueChanged(int ) ;
    // Bouton : Quantification
    /**
     * @brief Bouton : Quantification
     */
    void on_radioButton_quantification_clicked() ;
    // Quantification
    /**
     * @brief Quantification
     */
    void on_horizontalSlider_quantification_valueChanged(int ) ;

    // -------------Bruitage et debruitage-------------

    // Initialiser
    void on_groupBox_bruitageDebruitage_clicked() ;
    // Bouton : Bruitage
    /**
     * @brief  Bouton : Bruitage
     */
    void on_radioButton_bruitage_clicked() ;
    // Bruitage
    /**
     * @brief Bruitage
     */
    void on_horizontalSlider_Bruitage_valueChanged(int) ;
    // Bouton : Debruitage
    /**
     * @brief Initialiser Debruitage
     */
    void on_groupBox_debruitage_clicked() ;
    // Filtre moyeneur
    /**
     * @brief Filtre moyeneur
     */
    void on_radioButton_moyenneur_clicked() ;
    // Filtre gaussien
    /**
     * @brief Filtre gaussien
     */
    void on_radioButton_gaussien_clicked() ;
    // Filtre median
    /**
     * @brief Filtre median
     */
    void on_radioButton_median_clicked() ;
    // Filtre de Kuwahara-Nagao
    /**
     * @brief Filtre de Kuwahara-Nagao
     */
    void on_radioButton_kuwahara_clicked();

    // -------------Couleur-------------

    // Initialiser
    /**
     * @brief Initialiser Couleur
     */
    void on_groupBox_couleur_clicked() ;
    // Bouton : Temperature
    /**
     * @brief Bouton : Temperature
     */
    void on_radioButton_temperature_clicked() ;
    // Temperature
    /**
     * @brief Temperature
     */
    void on_horizontalSlider_temperature_valueChanged(int ) ;
    // Bouton : Vividite
    /**
     * @brief Bouton : Vividite
     */
    void on_radioButton_vividite_clicked() ;
    // Vividite
    /**
     * @brief Vividite
     */
    void on_horizontalSlider_vividite_valueChanged(int ) ;
    // Bouton : Teinte
    /**
     * @brief Bouton : Teinte
     */
    void on_radioButton_teinte_clicked() ;
    // Teinte
    /**
     * @brief Teinte
     */
    void on_horizontalSlider_teinte_valueChanged(int ) ;
    // Bouton : Saturation
    /**
     * @brief Bouton : Saturation
     */
    void on_radioButton_saturation_clicked() ;
    // Saturation
    /**
     * @brief Saturation
     */
    void on_horizontalSlider_saturation_valueChanged(int ) ;

    //-------------Autres-------------

    // Initialiser
    /**
     * @brief autres filtres
     */
    void on_groupBox_autre_clicked();
    // Transformee de Fourier
    /**
     * @brief Transformée de Frourier
     */
    void on_radioButton_fourier_clicked() ;
    // K-means
    /**
     * @brief radioBouton K-means
     */
    void on_radioButtonKmeans_clicked();
    // Bouton : Transformee de Hough
    /**
     * @brief Transformée de Hough
     */
    void on_radioButtonTransformeeHough_clicked();
    // Transformee de Hough
    /**
     * @brief Bouton OK pour appliquer la transformée de Hough
     */
    void on_pushButton_Appliquer_Hough_clicked();

    //-------------Complements-------------

    // Generer les icones
    /**
     * @brief  Generer les icones
     */
    void GenererIcone() ;
    // Afficher l'image resultante apres un traitement
    /**
     * @brief Afficher l'image resultante apres un traitement avec inversement des canaux de couleur
     * @param image
     * @param choix
     */
    void AffichageResultat(const Mat image, const int choix) ;
    // Messages d'aide
    /**
     * @brief Messages d'aide
     */
    void AfficherMessageAide() ;
    // Messages d'aide : Luminosite
    /**
     * @brief Messages d'aide : Luminosite
     */
    void AfficherMessageAideLuminosite() ;
    // Messages d'aide : Couleur
    /**
     * @brief Messages d'aide : Couleur
     */
    void AfficherMessageAideCouleur() ;
    // Messages d'aide : Details
    /**
     * @brief Messages d'aide : Details
     */
    void AfficherMessageAideDetail() ;

    // Messages d'aide : Resolution
    /**
     * @brief Messages d'aide : Resolution
     */
    void AfficherMessageAideResolution() ;
    // Messages d'aide : Extraction
    /**
     * @brief Messages d'aide : Extraction
     */
    void AfficherMessageAideExtraction() ;
    // Messages d'aide : Contours
    /**
     * @brief Messages d'aide : Contours
     */
    void AfficherMessageAideContour() ;
    // Messages d'aide : Debruitage
    /**
     * @brief Messages d'aide : Debruitage
     */
    void AfficherMessageAideDebruitage() ;
    // Messages d'aide : Seuillage et segmentation
    /**
     * @brief Messages d'aide : Seuillage et segmentation
     */
    void AfficherMessageAideSeuillageSegmentation() ;
    // Messages d'aide : Filtres
    /**
     * @brief Messages d'aide : Filtres
     */
    void AfficherMessageAideFiltre() ;

    // Reinitialisation generale
    /**
     * @brief  Reinitialisation generale
     */
    void Reinitialiser() ;
    // Reinitialiser : Box Luminosite
    /**
     * @brief Reinitialiser : Box Luminosite
     */
    void ReinitialiserLuminosite() ;
    // Reinitialiser : Box Couleur
    /**
     * @brief Reinitialiser : Box Couleur
     */
    void ReinitialiserCouleur() ;
    // Reinitialiser : Box Resolution
    /**
     * @brief Reinitialiser : Box Resolution
     */
    void ReinitialiserResolution() ;
    // Reinitialiser : Box Details
    /**
     * @brief Reinitialiser : Box Details
     */
    void ReinitialiserDetail() ;
    // Reinitialiser : Box Bruitage et debruitage
    /**
     * @brief RReinitialiser : Box Bruitage et debruitage
     */
    void ReinitialiserBruitageDebruitage() ;
    // Reinitialiser : Box Extraction
    /**
     * @brief Reinitialiser : Box Extraction
     */
    void ReinitialiserExtraction() ;
    // Reinitialiser : Box Contours
    /**
     * @brief Reinitialiser : Box Contours
     */
    void ReinitialiserContour() ;
    // Reinitialiser : Box Debruitage
    /**
     * @brief Reinitialiser : Box Debruitage
     */
    void ReinitialiserDebruitage() ;
    // Reinitialiser : Box Seuillage et segmentation
    /**
     * @brief  Reinitialiser : Box Seuillage et segmentation
     */
    void ReinitialiserSeuillageSegmentation() ;
    // Reinitialiser : Box Filtres
    /**
     * @brief  Reinitialiser : Box Filtres
     */
    void ReinitialiserFiltre() ;
    // Reinitialiser : Box Autres
    /**
     * @brief Reinitialiser : Box Autres
     */
    void ReinitialiserAutre() ;


    /**
     * @brief Bouton Retour : Menu Consultation Image
     */
    void on_pushButtonRetourMenuConsultationImage_clicked();

    /**
     * @brief Slot du bouton Retour après agrandissement de l'image traitée
     */
    void on_pushButton_clicked();

    /**
     * @brief mise à jour du tableau des descripteurs après chargement de la bibliothèque
     */
    void updateTableWidgetBiblio();

    /**
     * @brief mise à jour du tableau de la sous liste de la bibliothèque
     */
    void updateTableWidgetSousListeBiblio(Json::Value);

private:

    Ui::MainWindow *ui;
    /**
     * @brief Objet de classe Bibliothèque contenant la bibliothèque chargée par l'utilisateur
     */
    Bibliotheque _objBiblio;         
    /**
     * @brief droit d'acces de l'utilisateur, true : utilisateur niveau 2 , false : utilisateur niveau 1
     */
    bool _droitAcces;              
    /**
     * @brief  numéro de l'image selectionnee par l'utilisateur
     */
    int _numImageSelectionnee;      
    /**
     * @brief  Nom du Fichier de l'Image Ajoutee 
     */
    string _ImageAjouteeNomFichier;    
    /**
     * @brief Objet de classe Bibliothèque contenant la sous-liste construite à partir de la bibliothèque chargée par l'utilisateur
     */
    Bibliotheque _objSousListeBiblio ;
    /**
     * @brief Indice de l'image choisi dans la bibliotheque 
     */
    int _indiceImageSelectionnee ;   
    /**
     * @brief Image originale
     */
    Mat _imageOriginale ;            
    /**
    * @brief Histogramme de l'image originale du menu Traitement d'image
    */
   Mat _histoImageOriginale ;      

   /**
     * @brief Image resultante après traitement d'image 
     */
    Mat _imageResultat ;             
    /**
     * @brief Histogramme de l'image resultante après traitement d'image 
     */
    Mat _histoImageResultat ;        
    /**
     * @brief Vecteur des Seuils bas du menu Traitement d'image
     */
    vector<int> _seuilBas ;         
    /**
     * @brief Vecteur des Seuils hauts du menu Traitement d'image
     */
    vector<int> _seuilHaut;         




};

