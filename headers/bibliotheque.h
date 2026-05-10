/**
 * @file bibliotheque.h
 * @brief Header de la classe Bibliotheque
 * @version 0.1
 * @date 2022-01-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

// Systeme
#include <fstream>

// Json
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
// QT
#include <QDateTime>
// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace rapidjson ;
using namespace std ;
using namespace cv ;

/**
 * @class Bibliotheque 
 * @brief La classe Bibliotheque permet la gestion d'une bibliotheque d'images
 */
class Bibliotheque{
    private :
    /* Attributs */
        /**
         * @brief Objet Json de la bibliotheque
         * 
         */
        Json::Value _bibliotheque ;
        /**
         * @brief Chemin du fichier .json de la bibliotheque
         * 
         */
        string _cheminJson;    
    public :
        /*Constructeurs*/
        /**
         * @brief Construire un nouveau objet Bibliotheque vide
         * 
         */
        Bibliotheque() ;
        /**
         * @brief Construire un nouveau objet Bibliotheque avec le nom de la bibliotheque
         * 
         * @param nomBibliotheque string
         */
        Bibliotheque(string nomBibliotheque) ;
        /**
         * @brief Construire un nouveau objet Bibliotheque avec un objet Json
         * 
         * @param jsonBibliotheque Json::Value
         */
        Bibliotheque(const Json::Value jsonBibliotheque) ;     

        /*Getters*/
        /**
         * @brief Obtenir l'objet Json de la Bibliotheque 
         * 
         * @return  Json::Value : l'objet Json de la Bibliotheque
         */
        Json::Value getBilbiotheque() const ;
        /**
         * @brief Obtenir le chemin du fichier .json de la Bibliotheque.
         * 
         * @return string : une chaine de charactere contenant le chemin du fichier .json de la Bibliotheque.
         */
        string getCheminJson() const;
        /*Setters*/
        /**
         * @brief Affecter l'objet Json de la Bilbiotheque.
         * 
         * @param jsonBibliotheque Json::Value
         */
        void setBilbiotheque(const Json::Value jsonBibliotheque) ;
        /**
         * @brief Affecter le chemin du fichier .json de la Bibliotheque.
         * 
         * @param cheminJson string 
         */
        void setCheminJson(const string cheminJson) ;

        /*Methodes*/
        /**
         * @brief Ajouter une image a l'objet Json de la Bibliotheque.
         * 
         * @param cheminAccesContenu string
         * @param titre string
         * @param numero int
         * @param cout double
         * @param source string
         * @param dateAjout string
         * @param dateCreation string
         * @param acces string
         */
        void AjouterImage(string cheminAccesContenu,string titre,int numero,double cout,string source,
                          string dateAjout,string dateCreation,string acces);           // Ajouter une image dans la bibliotheque
        /**
         * @brief Supprimer une image de l'objet Json de la Bibliotheque.
         * @param numero int
         */
        void SupprimerImage(int numero) ;                                               // Supprimer une image de la bibliotheque
        /**
         * @brief Sauvegarder l'objet json de la Bibliotheque dans un fichier .json.
         * @param fileName string
         */
        void Sauvegarder(string fileName) ;                                             // Sauvegarder une bibliotheque
        /**
         * @brief Mettre à jour la Bibliotheque suivant suivant les droits d'acces de l'utilisateur. 
         * \n Si droitAcces = true (Utilisateur de Niveau 1) alors les images ayant un acces restreint sont supprimées de l'objet json de la Bibliotheque.
         * \n Si droitAcces = false (Utilisateur de Niveau 2) alors on garde toutes les images dans l'objet json.
         * @param droitAcces bool
         */
        void majBiblioSuivantDroitAcces(const bool droitAcces) ;                        // Creer une sous-bibliotheque avec les images correspondantes au droit d'utilisateur
        /**
         * @brief Construire une sous liste d'images de la Bibliotheque suivant le choix de la plage de cout : 
         *  \n 1 : gratuit  
         *  \n 2 : cout <= 99,99€
         *  \n 3 : cout <= 999,99€
         *  \n 4 : cout > 1000€
         * @param choix int
         * @return Json::Value : un objet json contenant la sous liste construite.
         */
        Json::Value ConstruireAfficherSousListeCout(const int choix) ;                  // Construire et afficher une sous-liste en fonction du cout (4 premieres options)
        /**
         * @brief Construire une sous liste d'images de la Bibliotheque suivant la plage de cout [coutMin, coutMax] 
         * @param coutMin double
         * @param coutMax double
         * @return Json::Value : un objet json contenant la sous liste construite.
         */
        Json::Value ConstruireAfficherSousListeCout(double coutMin, double coutMax) ;   // Construire et afficher une sous-liste en fonction du cout (derniere option)
        /**
         * @brief Construire une sous liste d'images de la Bibliotheque suivant le choix de la plage de la date d'ajout :
         * \n 1 : Ajouter aujourd'hui
         * \n 2 : Ajouter il y a moins de 7 jours
         * \n 3 : Ajouter ce mois
         * \n 4 : Ajouter cette annee
         * @param choix int
         * @return Json::Value : un objet json contenant la sous liste construite suivant le critère choisi.
         */
        Json::Value ConstruireAfficherSousListeDateAjout(const int choix) ;             // Construire et afficher une sous-liste en fonction de la date d'ajout
        /**
         * @brief  Trier l'objet json de la Bibliotheque suivant les critères :
         * \n 1 : Cout decroissant
         * \n 2 : Numero decroissant
         * \n 3 : Acces Restreint puis Acces Publique 
         * @param  choix 
         * @return Json::Value : un objet json de contenant une bibliothèque triée suivant le critère choisi.
         */
        Json::Value Trier(const int choix) ;                                             // Trier l'objet json de la Bibliotheque en fonction des caractéristiques : cout,numero et acces
        /**
         * @brief Trier un vecteur de reels dans l'ordre décroissant.
         * @param valeurNonTri vector<double>
         * @return vector<int> : un vecteur d'entiers contenant les indices triés du vecteur d'entrée.
         */
        vector<int> Trier(vector<double> valeurNonTri) ;                                 // Determiner les indices des elements avant le tri (reel)
        /**
         * @brief Trier un vecteur de chaines characteres dans l'ordre décroissant.
         * @param valeurNonTri vector<string>
         * @return vector<int> : un vecteur d'entiers contenant les indices triés du vecteur d'entrée.
         */
        vector<int> Trier(vector<string> valeurNonTri) ;                                 // Determiner les indices des elements avant le tri (chaine de caracteres)
        /**
         * @brief Trier un vecteur d'entiers dans l'ordre décroissant.
         * @param valeurNonTri vector<int>
         * @return vector<int> : un vecteur d'entiers contenant les indices triés du vecteur d'entrée.
         */
        vector<int> Trier(vector<int> valeurNonTri) ;                                    // Determiner les indices des elements avant le tri (entier)
        
        /*Date*/
        /**
         * @brief Extraire jour, mois, annee a partir d'une date.
         * @param date
         * @param jour
         * @param mois
         * @param annee
         */
        static void ExtraireDate(const string date, string& jour, string& mois, string& annee) ;	        // Extraire jour, mois, annee a partir d'une date

        /*Numero*/
        /**
         * @brief Verifier l'extension ".json"
         * @param nom
         */
        static void VerifierExtension (string& nom) ;												        // Verifier l'extension ".json"
        /**
         * @brief Verifier l'existance d'une image de numero spécifique dans l'objet json de la bibliotheque.
         * @param numero
         * @param biblio
         * @return
         */
        static bool VerifierNumero(const int numero, const Json::Value biblio) ;               	        // Verifier l'existance d'un numero de l'image
};

