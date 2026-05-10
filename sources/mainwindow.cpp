#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

#ifndef PHOTOBOX_SOURCE_DIR
#define PHOTOBOX_SOURCE_DIR "."
#endif

namespace {
QString cleanPath(const QString& path){
    return QDir::cleanPath(path) ;
}

QString sourceDataRoot(){
    const QString sourceDir = QString::fromLocal8Bit(PHOTOBOX_SOURCE_DIR) ;
    if(sourceDir.isEmpty()){
        return QString() ;
    }
    return cleanPath(QDir(sourceDir).filePath("DATA")) ;
}

QString existingDirectory(const QStringList& candidatePaths){
    for(const QString& candidatePath : candidatePaths){
        if(candidatePath.isEmpty()){
            continue ;
        }
        const QDir dir(cleanPath(candidatePath)) ;
        if(dir.exists()){
            return dir.absolutePath() ;
        }
    }
    return QString() ;
}

QString dataRootPath(){
    static QString root ;
    if(!root.isEmpty()){
        return root ;
    }

    const QString appDir = QCoreApplication::applicationDirPath() ;

    // Prefer the project/release DATA directory. Do not base this primarily on
    // QDir::currentPath(), because QFileDialog can leave the process inside
    // DATA/Bibliotheques. Using currentPath() first caused nested paths like
    // DATA/Bibliotheques/DATA/Bibliotheques.
    const QStringList candidates = {
        sourceDataRoot(),
        appDir + "/DATA",
        appDir + "/../DATA",
        appDir + "/../photobox/DATA",
        appDir + "/../../photobox/DATA"
    } ;

    root = existingDirectory(candidates) ;
    if(root.isEmpty()){
        root = cleanPath(appDir + "/DATA") ;
    }

    QDir().mkpath(root + "/Images") ;
    QDir().mkpath(root + "/Bibliotheques") ;
    return root ;
}

QString dataFilePath(const QString& relativePath){
    return QDir(dataRootPath()).filePath(relativePath) ;
}

QString temporaryImagePath(){
    return dataFilePath("Images/imageTemp.jpg") ;
}

QString resolveImagePath(const QString& rawPath, const std::string& jsonPath){
    if(rawPath.isEmpty() || rawPath.startsWith(":")){
        return rawPath ;
    }

    const QFileInfo rawInfo(rawPath) ;
    if(rawInfo.isAbsolute()){
        return rawInfo.exists() ? rawInfo.absoluteFilePath() : rawPath ;
    }

    QStringList candidates ;

    if(!jsonPath.empty()){
        const QFileInfo jsonInfo(QString::fromStdString(jsonPath)) ;
        candidates << jsonInfo.absoluteDir().filePath(rawPath) ;
    }

    candidates << QDir::current().filePath(rawPath) ;
    candidates << QDir(QCoreApplication::applicationDirPath()).filePath(rawPath) ;

    QString normalizedPath = rawPath ;
    while(normalizedPath.startsWith("./")){
        normalizedPath.remove(0, 2) ;
    }

    if(normalizedPath.startsWith("../Images/")){
        candidates << dataFilePath("Images/" + normalizedPath.mid(QString("../Images/").length())) ;
    }
    if(normalizedPath.startsWith("Images/")){
        candidates << dataFilePath(normalizedPath) ;
    }
    if(normalizedPath.startsWith("DATA/")){
        candidates << QDir(QFileInfo(dataRootPath()).absoluteDir()).filePath(normalizedPath) ;
    }

    for(const QString& candidate : candidates){
        const QFileInfo candidateInfo(cleanPath(candidate)) ;
        if(candidateInfo.exists()){
            return candidateInfo.absoluteFilePath() ;
        }
    }

    return rawPath ;
}

QString selectedImagePath(const Bibliotheque& bibliotheque, int imageIndex){
    const QString rawPath = QString::fromStdString(bibliotheque.getBilbiotheque()["images"][imageIndex]["cheminAcces"].asString()) ;
    return resolveImagePath(rawPath, bibliotheque.getCheminJson()) ;
}

cv::Mat readImage(const QString& path){
    return cv::imread(path.toStdString()) ;
}
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this) ;
    // Connecter les signaux et slots
    connect(ui->tableWidgetBiblio, SIGNAL(cellClicked(int, int)), this,SLOT(on_tableBiblioRowClicked(int , int))) ;          // connecter la selection d'une ligne avec la selection d'un numero d'image
    connect(ui->comboBoxTrier,SIGNAL(currentIndexChanged(int)),this,SLOT(on_comboBoxTrierIndexChanged(int))) ;               // connecter la selection d'un critère de tri avec le tri
    connect(ui->comboBoxCritereCout,SIGNAL(activated(int)),this,SLOT(on_comboBoxCritereCoutIndexChanged(int))) ;             // connecter la selection d'un critère cout de creation de sous liste avec la creation d'une sous liste
    connect(ui->comboBoxCritereDateAjout,SIGNAL(activated(int)),this,SLOT(on_comboBoxCritereDateAjoutIndexChanged(int))) ;   // connecter la selection d'un critère date d'ajout de creation de sous liste avec la creation d'une sous liste

    // Initialisation des pages
    // Page MenuConsultationImage-----------------------------
    // Messages d'aide
    // Fenetre d'affichage
    ui->graphicsView_contenuImageNonModifiable->setToolTip("Contenu de l'image choisie") ;
    // Agrandissement
    ui->label_ZoomNonModifiable->setToolTip("Agrandissement") ;
    ui->horizontalSlider_agrandissementNonModifiable->setToolTip("Tirer vers la droite pour agrandir l'image") ;
    // Descripteur de l'image
    ui->groupBox_descripteurImageNiv1->setToolTip("Informations de l'image :\nAccès (Permission), coût, date d'ajout, date de création, numéro, source, titre") ;

    // Page MenuModificationImage-----------------------------
    // Fenetre d'affichage
    ui->graphicsView_contenuImage->setToolTip("Contenu de l'image choisie") ;
    // Agrandissement
    ui->label_Zoom->setToolTip("Agrandissement") ;
    ui->horizontalSlider_agrandissement->setToolTip("Tirer vers la droite pour agrandir l'image") ;
    // Descripteur de l'image
    ui->groupBox_descripteurImage->setToolTip("Informations de l'image :\nAccès (Permission), coût, date d'ajout, date de création, numéro, source, titre") ;

    ui->checkBoxModificationImageAcces->setToolTip("Accès :\nP : Publique , R : Restreint") ;
    ui->checkBoxAjoutImageAcces->setToolTip("Accès :\nP : Publique, R : Restreint") ;

    ui->doubleSpinBoxAjoutImageCout->setToolTip("Coût (€) : réel ") ;
    ui->doubleSpinBoxModificationImageCout->setToolTip("Coût (€) : réel ") ;

    ui->dateEditModificationImageDateAjout->setToolTip("Date d'ajout : (dd/mm/yyyy)") ;
    ui->dateEditAjoutImageDateAjout->setToolTip("Date d'ajout : (dd/mm/yyyy)") ;

    ui->dateEditModificationImageDateCreation->setToolTip("Date de création (dd/mm/yyyy)") ;
    ui->dateEditAjoutImageDateCreation->setToolTip("Date de création (dd/mm/yyyy)") ;

    ui->spinBoxAjoutImageNumero->setToolTip("Numéro") ;
    ui->spinBoxModificationImageNumero->setToolTip("Numéro") ;

    ui->lineEditAjoutImageSource->setToolTip("Source") ;
    ui->lineEditModificationImageSource->setToolTip("Source") ;

    ui->lineEditAjoutImageTitre->setToolTip("Titre") ;
    ui->lineEditModificationImageTitre->setToolTip("Titre") ;
    // Passer au traitement
    ui->pushButton_traitementImage->setToolTip("Commencer le traitement de l'image choisie") ;
    // Sauvegarder les modifications
    ui->pushButton_modifier->setToolTip("Sauvegarder les modifications") ;

    // Page du traitement-----------------------------
    // ajouter l'icon sur le boutton agrandir image traitee
    ui->pushButtonAfficherImageTraitee->setIcon(QIcon(":/icons/agrandir.png"));
    // Initialiser
    GenererIcone() ;
    // Messages d'aide
    AfficherMessageAide() ;
}

MainWindow::~MainWindow(){
    delete ui ;
}

void MainWindow::on_pushButtonIdentifier_clicked(){
    QString userMdp = ui->lineEditMdp->text() ;
    QStringRef codeDroitAcces(&userMdp, 4, 2) ;
    QFile userIdFile(":/idUsers.txt") ;
    bool idFound=false ;

   // tester l'ouverture du fichier
    if(!userIdFile.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::information(0,"error",userIdFile.errorString()) ;
    }

    QTextStream in(&userIdFile) ;
    QString line = in.readLine() ;

    while (!line.isNull()){
        if (userMdp==line){
            idFound = true ;
            break ;
        }
        line = in.readLine() ;
    }
    if (idFound){
        if (codeDroitAcces=="11"){
            _droitAcces = true ;
        }else if (codeDroitAcces=="00"){
            _droitAcces = false ;
        }
        //QMessageBox::information(0,"Identification","Identification Réussite !") ;
        if (_droitAcces){
            ui->statusbar->showMessage("Utilisateur de Niveau 2.") ;
        } else{
            ui->statusbar->showMessage("Utilisateur de Niveau 1.") ;
        }
        ui->stackedWidget->setCurrentIndex(1) ;
    }else{
        QMessageBox::information(0,"Identification","Le mot de passe est incorrect.") ;
    }
    // Mettre à jour l'application suivant le niveau de l'utilisateur
    if (!_droitAcces){
        ui->pushButtonSauvegarderSousListe->hide() ;
        ui->pushButtonCreerBiblio->hide() ;
        ui->pushButtonAjouterImage->hide() ;
        ui->pushButtonSauvegarder->hide() ;
        ui->pushButtonSauvegarderSousListe->hide() ;
        ui->pushButtonSupprimerImage->hide() ;
    }else{
        ui->pushButtonSauvegarderSousListe->show() ;
        ui->pushButtonCreerBiblio->show() ;
        ui->pushButtonAjouterImage->show() ;
        ui->pushButtonSauvegarder->show() ;
        ui->pushButtonSauvegarderSousListe->show() ;
        ui->pushButtonSupprimerImage->show() ;
    }

}

void MainWindow::on_pushButtonQuitter_clicked(){
    qApp->quit() ;
}

void MainWindow::on_pushButtonChargerBiblio_clicked(){
    QString fileName = QFileDialog::getOpenFileName(0,"Charger la bibliothèque",dataFilePath("Bibliotheques"),"*.json") ; // Ouvrir une fenetre pour que l'utilisateur choisisse un fichier .json
    QFile fileBiblio(fileName) ;
    // test sur l'ouverture du fichier
    if(fileBiblio.open(QIODevice::ReadOnly)){
        // Chargement de la bibliothèque
        _objBiblio = Bibliotheque(fileName.toStdString()) ;                                 // Chargement de la bibliotheque : construction d'un objet de classe bibliothèque à partir du fichier json choisi
        _objBiblio.majBiblioSuivantDroitAcces(_droitAcces) ;                                // mettre à jour l'objet bibliotheque suivant le niveau de l'utilisateur
        updateTableWidgetBiblio() ;                                                         // mettre à jour la table des descripteurs de la bibliotheque
        updateTableWidgetSousListeBiblio(Json::Value()) ;                                   // vider la table de la sous-liste de la bibliothèque
        ui->stackedWidget->setCurrentIndex(2) ;                                             // Aller au Menu Bibliothèque (page 2)
        // extraire le nom de la bibliotheque du chemin d acces du fichier .json
        QString biblioName ;                                                                // Nom de la bibliothèque
        int i=6 ;
        do{
            biblioName.push_front(fileName[fileName.length()-i]) ;
            i++ ;
        }while(fileName[fileName.length()-i]!='/') ;
        // ecrire le nom de la bibliotheque dans labelBiblioName
        ui->labelBiblioName->setText(biblioName) ;
        ui->labelSousListeBiblioName->setText(biblioName) ;
    }

}
void MainWindow::on_pushButtonCreerBiblio_clicked(){
    // ecrire le nom de la bibliotheque dans labelBiblioName
    ui->labelBiblioName->setText("nouvelle bibliotheque") ;
    ui->labelSousListeBiblioName->setText("nouvelle bibliotheque") ;
    // Creation de la bibliothèque
    _objBiblio.setBilbiotheque(Json::Value()) ;                                         // Creer la bibliotheque : construction d'un objet de classe bibliothèque à partir du fichier json choisi
    updateTableWidgetBiblio() ;                                                         // mettre à jour la table des descripteurs de la bibliotheque
    updateTableWidgetSousListeBiblio(Json::Value()) ;                                   // vider la table de la sous-liste de la bibliothèque
    ui->stackedWidget->setCurrentIndex(2) ;                                             // Aller au Menu Bibliothèque (page 2)
}

void MainWindow::on_pushButtonRetourIdentification_clicked(){
    ui->statusbar->showMessage("") ;                     // Vider le message de la statusbar
    ui->lineEditMdp->clear() ;                           // Vider le champ du mot de passe
    ui->stackedWidget->setCurrentIndex(0) ;              // Retour au Menu Identification (page 0)
}

void MainWindow::updateTableWidgetBiblio(){
    Json::Value biblio = _objBiblio.getBilbiotheque() ;        // Objet Json
    ui->tableWidgetBiblio->setRowCount(biblio["nbImages"].asInt()) ;           // Mettre à jour le nombre de lignes du tableWidgetBiblio suivant le Nombre d'images existantes dans la bibliotheque chargée
    for (int i = 0 ; i < ui->tableWidgetBiblio->rowCount() ; i++) {
        QTableWidgetItem *item ;
        for (int j = 0 ; j < ui->tableWidgetBiblio->columnCount() ; j++)     {
            item = new QTableWidgetItem ;
            if (j==0){item->setText(QString::fromStdString((biblio["images"][i]["titre"].asString()))) ;}
            if (j==1){item->setData(Qt::EditRole,biblio["images"][i]["cout"].asDouble()) ;}
            if (j==2){item->setData(Qt::EditRole,biblio["images"][i]["numero"].asDouble()) ;}
            if (j==3){item->setText(QString::fromStdString((biblio["images"][i]["source"].asString()))) ;}
            if (j==4){item->setData(Qt::EditRole,QDate::fromString(QString::fromStdString(biblio["images"][i]["dateCreation"].asString()),"dd'/'MM'/'yyyy")) ;}
            if (j==5){item->setData(Qt::EditRole,QDate::fromString(QString::fromStdString(biblio["images"][i]["dateAjout"].asString()),"dd'/'MM'/'yyyy")) ;}
            if (j==6){item->setText(QString::fromStdString((biblio["images"][i]["acces"].asString()))) ;}
            ui->tableWidgetBiblio->setItem(i,j,item) ;
        }
    }
}

void MainWindow::updateTableWidgetSousListeBiblio(Json::Value biblio){
    ui->tableWidgetSousListeBiblio->setRowCount(biblio["nbImages"].asInt()) ;           // Mettre à jour le nombre de lignes du tableWidgetBiblio suivant le Nombre d'images existantes dans la bibliotheque chargée
    for (int i = 0 ; i < ui->tableWidgetSousListeBiblio->rowCount() ; i++){
        QTableWidgetItem *item ;
        for (int j = 0 ; j < ui->tableWidgetSousListeBiblio->columnCount() ; j++){
            item = new QTableWidgetItem ;
            if (j==0){item->setText(QString::fromStdString((biblio["images"][i]["titre"].asString()))) ;}
            if (j==1){item->setData(Qt::EditRole,biblio["images"][i]["cout"].asDouble()) ;}
            if (j==2){item->setData(Qt::EditRole,biblio["images"][i]["numero"].asDouble()) ;}
            if (j==3){item->setText(QString::fromStdString((biblio["images"][i]["source"].asString()))) ;}
            if (j==4){item->setData(Qt::EditRole,QDate::fromString(QString::fromStdString(biblio["images"][i]["dateCreation"].asString()),"dd'/'MM'/'yyyy")) ;}
            if (j==5){item->setData(Qt::EditRole,QDate::fromString(QString::fromStdString(biblio["images"][i]["dateAjout"].asString()),"dd'/'MM'/'yyyy")) ;}
            if (j==6){item->setText(QString::fromStdString((biblio["images"][i]["acces"].asString()))) ;}
            ui->tableWidgetSousListeBiblio->setItem(i,j,item) ;
        }
    }
}

void MainWindow::on_pushButtonSauvegarder_clicked(){
    QString fileName = QFileDialog::getSaveFileName(0,tr("Sauvegarder bibliothèque d'images"), dataFilePath("Bibliotheques"), tr("*.json")) ;
    //QMessageBox::information(0,"Sauvegarge bibliothèque",fileName) ;
    if (!fileName.isEmpty()){
        string fileNameStr = fileName.toStdString() ;
        Bibliotheque::VerifierExtension(fileNameStr) ;
        _objBiblio.Sauvegarder(fileNameStr) ;
    }
}

void MainWindow::on_tableBiblioRowClicked(int row, int){
    ui->pushButtonSupprimerImage->setEnabled(_droitAcces) ;
    ui->pushButtonOuvrirImage->setEnabled(true) ;
    QTableWidgetItem *item = ui->tableWidgetBiblio->item(row,2) ;
    _numImageSelectionnee = item->text().toInt() ;

    for (int c = 0 ; c < _objBiblio.getBilbiotheque()["nbImages"].asInt() ; c++){
        if (_numImageSelectionnee == _objBiblio.getBilbiotheque()["images"][c]["numero"].asInt()){
            _indiceImageSelectionnee = c ;
            break ;
        }
    }


}

void MainWindow::on_pushButtonSupprimerImage_clicked(){
    QMessageBox::StandardButton reply ;
      reply = QMessageBox::question(0, "Supprression de l'image", "Etes-vous sûr de vouloir supprimer cette image?",QMessageBox::Yes | QMessageBox::No) ;
      if (reply == QMessageBox::Yes){
          _objBiblio.SupprimerImage(_numImageSelectionnee) ;
          updateTableWidgetBiblio() ;
      }
}

void MainWindow::on_pushButtonRetourMenuPrincipal_clicked(){
        ui->stackedWidget->setCurrentIndex(1) ;              // Retour au Menu Principal (page 1)
        // Reinitialiser le menu bibliothèque
        ui->comboBoxCritereCout->setCurrentIndex(0) ;
        ui->comboBoxCritereDateAjout->setCurrentIndex(0) ;
        ui->comboBoxTrier->setCurrentIndex(0) ;
        ui->doubleSpinBoxMin->setValue(0) ;
        ui->doubleSpinBoxMax->setValue(0) ;
        ui->pushButtonOuvrirImage->setEnabled(0) ;
        ui->pushButtonSupprimerImage->setEnabled(0) ;
        ui->tableWidgetBiblio->clearSelection() ;
}

void MainWindow::on_lineEditMdp_returnPressed(){
    emit ui->pushButtonIdentifier->click() ;            // emettre signal click du pushButtonIdentifier
}

void MainWindow::on_comboBoxTrierIndexChanged(int index){
    _objBiblio.Trier(index) ;
    updateTableWidgetBiblio() ;
}

void MainWindow::on_comboBoxCritereCoutIndexChanged(int index){
    ui->comboBoxCritereDateAjout->setCurrentIndex(0) ; // Reinitialiser le critere cout
    // Initialisation de la sous liste bibliothèque
    _objSousListeBiblio=Bibliotheque(_objBiblio.getBilbiotheque()) ;
    Json::Value sousListeJson ;

    if (index==5){
        sousListeJson = _objSousListeBiblio.ConstruireAfficherSousListeCout(ui->doubleSpinBoxMin->value(),ui->doubleSpinBoxMax->value()) ;
    }else{
    sousListeJson = _objSousListeBiblio.ConstruireAfficherSousListeCout(index) ;
    }
    updateTableWidgetSousListeBiblio(sousListeJson) ;
    // Metre à jour l'objet _objSousListeBiblio
    _objSousListeBiblio = Bibliotheque(sousListeJson) ;
}

void MainWindow::on_comboBoxCritereDateAjoutIndexChanged(int index){
    ui->comboBoxCritereCout->setCurrentIndex(0) ; // Reinitialiser le critere cout
    // Initialisation de la sous liste bibliothèque
    _objSousListeBiblio=Bibliotheque(_objBiblio.getBilbiotheque()) ;
    Json::Value sousListeJson ;

    sousListeJson = _objSousListeBiblio.ConstruireAfficherSousListeDateAjout(index) ;
    updateTableWidgetSousListeBiblio(sousListeJson) ;
    // Metre à jour l'objet _objSousListeBiblio
    _objSousListeBiblio = Bibliotheque(sousListeJson) ;
}

void MainWindow::on_pushButtonAjouterImage_clicked(){
    // Initialiser le menu Ajout Image
    ui->dateEditAjoutImageDateAjout->setDate(QDate::currentDate()) ;
    ui->dateEditAjoutImageDateCreation->setDate(QDate()) ;
    ui->doubleSpinBoxAjoutImageCout->setValue(0) ;
    ui->lineEditAjoutImageSource->clear() ;
    ui->lineEditAjoutImageTitre->clear() ;
    ui->spinBoxAjoutImageNumero->setValue(0) ;
    // Charger l image à ajouter
    QString fileName = QFileDialog::getOpenFileName(0,tr("Choisir une image à ajouter"), dataFilePath("Images") ,tr("Fichiers Images (*.png *.jpg *.bmp *.pgm *.jpeg *.tiff)")) ;
    if (!fileName.isEmpty()){
        _ImageAjouteeNomFichier = fileName.toStdString() ;
        QPixmap pix(fileName) ;
        ui->labelImageAjoutee->setStyleSheet("image:url("+fileName+") ;") ;
        ui->labelImageAjoutee->setPixmap(pix.scaled(ui->labelImageAjoutee->width(),ui->labelImageAjoutee->height(),Qt::KeepAspectRatio)) ;
        ui->labelImageAjoutee->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
        ui->stackedWidget->setCurrentIndex(3) ;          // Aller au menu Ajout Image
    }
}

void MainWindow::on_pushButtonAjoutImageAnnuler_clicked(){
    ui->stackedWidget->setCurrentIndex(2) ;          // Aller au menu bibliothèque
}

void MainWindow::on_pushButtonAjoutImageAjouter_clicked(){
    QString titre = ui->lineEditAjoutImageTitre->text() ;
    int numero = ui->spinBoxAjoutImageNumero->value() ;
    double cout = ui->doubleSpinBoxAjoutImageCout->value() ;
    QString source = ui->lineEditAjoutImageSource->text() ; source.replace(" ","") ;// chaine de caractère formée d'un seul mot
    string acces = ui->checkBoxAjoutImageAcces->isChecked() ? "R" : "P" ;  // si restreint est coché acces = "R", "P" sinon
    QString dateAjout = ui->dateEditAjoutImageDateAjout->date().toString("dd/MM/yyyy") ;
    QString dateCreation = ui->dateEditAjoutImageDateCreation->date().toString("dd/MM/yyyy") ;

    if (Bibliotheque::VerifierNumero(numero,_objBiblio.getBilbiotheque())){
        QMessageBox::information(0,"Erreur Ajout Image","Le numéro que vous avez choisi existe dèjà!") ;
    }else if( ui->dateEditAjoutImageDateAjout->date() < ui->dateEditAjoutImageDateCreation->date()){
        QMessageBox::information(0,"Erreur Ajout Image","La date d'ajout ne doit pas etre inférieur à la date de création!") ;
    }else if (source.length() && titre.length()){
    _objBiblio.AjouterImage(_ImageAjouteeNomFichier,titre.toStdString(),numero,cout,source.toStdString(),dateAjout.toStdString(),dateCreation.toStdString(),acces) ;
    updateTableWidgetBiblio() ;
    ui->stackedWidget->setCurrentIndex(2) ;          // Aller au menu bibliothèque
    }else{
        QMessageBox::information(0,"Erreur Ajout Image","Les champs Titre et Source sont obligatoire !") ;
    }
}


void MainWindow::on_pushButtonSauvegarderSousListe_clicked(){
    QString fileName = QFileDialog::getSaveFileName(0,tr("Sauvegarder bibliothèque d'images"), dataFilePath("Bibliotheques"), tr("*.json")) ;
    if (!fileName.isEmpty()){
        string fileNameStr = fileName.toStdString() ;
        Bibliotheque::VerifierExtension(fileNameStr) ;
        _objSousListeBiblio.Sauvegarder(fileNameStr) ;
    }
}

void MainWindow::on_pushButtonOuvrirImage_clicked(){
    if (!_droitAcces){
    // Declaration des variables
    QString chemin ;                                        // Chemin d'acces a l'image choisie
    QString descripteur ;                                   // Descripteur

    // Determiner le chemin d'acces a l'image
    chemin = selectedImagePath(_objBiblio, _indiceImageSelectionnee) ;

    // Affichage de l'image choisie
    // Charger l'image avec son chemin d'acces
    QPixmap image(chemin) ;
    QGraphicsScene* scene = new QGraphicsScene(this) ;
    scene->addPixmap(image) ;
    ui->graphicsView_contenuImageNonModifiable->setScene(scene) ;
    ui->graphicsView_contenuImageNonModifiable->show() ;
    ui->graphicsView_contenuImageNonModifiable->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;

    // Descripteur de l'image
    descripteur = QString::fromStdString("Titre            : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["titre"].asString() + "\n"
            + "Coût (€)         : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["cout"].asString() + "\n"
            + "Numéro           : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["numero"].asString() + "\n"
            + "Source           : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["source"].asString() + "\n"
            + "Date d'ajout     : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["dateAjout"].asString() + "\n"
            + "Date de creation : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["dateCreation"].asString() + "\n"
            + "Acces            : " + _objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["acces"].asString() + "\n") ;
    // Affichage du descripteur de l'image
    ui->label_descripteurImage_nonModifiable->setText(descripteur) ;
    // Aller vers la page Menu Consultation Image
    ui->stackedWidget->setCurrentIndex(4) ;
}else{
        // Declaration de variable
        string chemin ;             // Chemin d'acces a l'image choisie
        QString cheminQT ;          // Chemin d'acces a l'image choisie
        string  jour ;
        string  mois ;
        string  annee ;
        QDate dateTemp ;
        bool ok=true ;

        // Extraction des informations du descripteur de l'image choisie
        // Acces (Permission)
        ui->checkBoxModificationImageAcces->setChecked(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["acces"].asString()=="R" ? true : false) ;
        // Cout
        ui->doubleSpinBoxModificationImageCout->setValue(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["cout"].asDouble()) ;
        // Date d'ajout
        Bibliotheque::ExtraireDate(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["dateAjout"].asString(),jour,mois,annee) ;
        dateTemp = QDate(QString::fromStdString(annee).toInt(&ok),QString::fromStdString(mois).toInt(&ok),QString::fromStdString(jour).toInt(&ok)) ;
        if(!ok){
            QMessageBox::information(0,"Erreur Date d'Ajout","Impossible de charger la date d'ajout, fichier corrompu.") ;
        }else{
            ui->dateEditModificationImageDateAjout->setDate(dateTemp) ;
        }
        // Date de creation
        Bibliotheque::ExtraireDate(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["dateCreation"].asString(),jour,mois,annee) ;
        dateTemp = QDate(QString::fromStdString(annee).toInt(&ok),QString::fromStdString(mois).toInt(&ok),QString::fromStdString(jour).toInt(&ok)) ;
        if(!ok){
            QMessageBox::information(0,"Erreur Date de Creation","Impossible de charger la date d'creation, fichier corrompu.") ;
        }else{
            ui->dateEditModificationImageDateCreation->setDate(dateTemp) ;
        }
        // Numero
        ui->spinBoxModificationImageNumero->setValue(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["numero"].asInt()) ;
        // Source
        ui->lineEditModificationImageSource->setText(QString::fromStdString(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["source"].asString())) ;
        // Titre
        ui->lineEditModificationImageTitre->setText(QString::fromStdString(_objBiblio.getBilbiotheque()["images"][_indiceImageSelectionnee]["titre"].asString())) ;

        // Affichage de l'image choisie
        // Determiner le chemin d'acces a l'image
        cheminQT = selectedImagePath(_objBiblio, _indiceImageSelectionnee) ;
        chemin = cheminQT.toStdString() ;

        // Charger l'image avec son chemin d'acces
        _imageOriginale = readImage(cheminQT) ;
        QPixmap imageQT(cheminQT) ;
        QGraphicsScene* sceneImageDescripteur = new QGraphicsScene(this) ;
        sceneImageDescripteur->addPixmap(imageQT) ;
        ui->graphicsView_contenuImage->setScene(sceneImageDescripteur) ;
        ui->graphicsView_contenuImage->show() ;
        ui->graphicsView_contenuImage->fitInView(sceneImageDescripteur->sceneRect(), Qt::KeepAspectRatio) ;
        // Aller vers la page Menu Modification Image
        ui->stackedWidget->setCurrentIndex(5) ;
}
}

void MainWindow::on_pushButtonRetourMenuModificationImage_clicked(){
    ui->stackedWidget->setCurrentIndex(2) ;
}


void MainWindow::on_doubleSpinBoxMin_valueChanged(double){
    ui->doubleSpinBoxMax->setMinimum(ui->doubleSpinBoxMin->value()) ;
    if(ui->comboBoxCritereCout->currentIndex()==5){
                MainWindow::on_comboBoxCritereCoutIndexChanged(5) ;
    }
}

void MainWindow::on_doubleSpinBoxMax_valueChanged(double){
    ui->doubleSpinBoxMin->setMaximum(ui->doubleSpinBoxMax->value()) ;

    if(ui->comboBoxCritereCout->currentIndex()==5){
                MainWindow::on_comboBoxCritereCoutIndexChanged(5) ;
    }

}


// --------------------------Page Choisir Image (utilisateur niveau 1)--------------------------

// Agrandissement de l'image (non modifiable)
void MainWindow::on_horizontalSlider_agrandissementNonModifiable_valueChanged(int value){
    // Sevlaration des variables
    int hauteurImage = ui->graphicsView_contenuImageNonModifiable->scene()->height() ;      // Hauteur de l'image
    int largeurImage = ui->graphicsView_contenuImageNonModifiable->scene()->width() ;       // Largeur de l'image
    int hauteurFenetre = ui->graphicsView_contenuImageNonModifiable->height() ;             // Hauteur de la fenetre
    int largeurFenetre = ui->graphicsView_contenuImageNonModifiable->width() ;              // Largeur de la fenetre
    double facteur ;                                                                        // Facteur d'agrandissement

    // Calculer le facteur d'agrandissement
    if(hauteurImage > largeurImage){
        facteur = (double)hauteurFenetre/hauteurImage*value/2 ;
    }else{
        facteur = (double)largeurFenetre/largeurImage*value/2 ;
    }

    // Agrandissement
    if(value == 1){
        QGraphicsScene* scene = ui->graphicsView_contenuImageNonModifiable->scene() ;
        ui->graphicsView_contenuImageNonModifiable->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    }else{
        ui->graphicsView_contenuImageNonModifiable->setTransform(QTransform::fromScale(facteur, facteur)) ;
    }
}

// --------------------------Page Choisir Image (utilisateur niveau 2)--------------------------

// Bouton de sauvegarder les modifications
void MainWindow::on_pushButton_modifier_clicked(){
    // Declaration des variables
    QString titre = ui->lineEditModificationImageTitre->text() ; // Titre
    int numero = ui->spinBoxModificationImageNumero->value() ; // numéro :
    double cout = ui->doubleSpinBoxModificationImageCout->value() ;
    QString source = ui->lineEditModificationImageSource->text() ; source.replace(" ","") ;// chaine de caractère formée d'un seul mot // Source
    string acces = ui->checkBoxModificationImageAcces->isChecked() ? "R" : "P" ;  // si restreint est coché acces = "R", "P" sinon
    QString dateAjout = ui->dateEditModificationImageDateAjout->date().toString("dd/MM/yyyy") ;
    QString dateCreation = ui->dateEditModificationImageDateCreation->date().toString("dd/MM/yyyy") ;
    Json::Value biblioJson = _objBiblio.getBilbiotheque() ;

    if (_numImageSelectionnee != numero && Bibliotheque::VerifierNumero(numero,_objBiblio.getBilbiotheque())){
        QMessageBox::information(0,"Erreur Ajout Image","Le numéro que vous avez choisi existe dèjà!") ;
    }else if( ui->dateEditModificationImageDateAjout->date() < ui->dateEditModificationImageDateCreation->date()){
        QMessageBox::information(0,"Erreur Ajout Image","La date d'ajout ne doit pas etre inférieur à la date de création!") ;
    }else if (source.length() && titre.length()){
    biblioJson["images"][_indiceImageSelectionnee]["titre"] = titre.toStdString() ;
    biblioJson["images"][_indiceImageSelectionnee]["numero"] = numero ;
    biblioJson["images"][_indiceImageSelectionnee]["cout"] = cout ;
    biblioJson["images"][_indiceImageSelectionnee]["source"] = source.toStdString() ;
    biblioJson["images"][_indiceImageSelectionnee]["dateAjout"] =  dateAjout.toStdString() ;
    biblioJson["images"][_indiceImageSelectionnee]["dateCreation"] =  dateCreation.toStdString() ;
    biblioJson["images"][_indiceImageSelectionnee]["acces"] =  acces ;
    _objBiblio.setBilbiotheque(biblioJson) ;
    updateTableWidgetBiblio() ;

    }else{
        QMessageBox::information(0,"Erreur Sauvegarder Descripteur Image","Les champs Titre et Source sont obligatoire !") ;
    }

}

// Agrandissement de l'image
void MainWindow::on_horizontalSlider_agrandissement_valueChanged(int value){
    // Sevlaration des variables
    int hauteurImage = ui->graphicsView_contenuImage->scene()->height() ;   // Hauteur de l'image
    int largeurImage = ui->graphicsView_contenuImage->scene()->width() ;    // Largeur de l'image
    int hauteurFenetre = ui->graphicsView_contenuImage->height() ;          // Hauteur de la fenetre
    int largeurFenetre = ui->graphicsView_contenuImage->width() ;           // Largeur de la fenetre
    double facteur ;                                                        // Facteur d'agrandissement

    // Calculer le facteur d'agrandissement
    if(hauteurImage > largeurImage){
        facteur = (double)hauteurFenetre/hauteurImage*value/2 ;
    }else{
        facteur = (double)largeurFenetre/largeurImage*value/2 ;
    }

    // Agrandissement
    if(value == 1){
        QGraphicsScene* scene = ui->graphicsView_contenuImage->scene() ;
        ui->graphicsView_contenuImage->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    }else{
        ui->graphicsView_contenuImage->setTransform(QTransform::fromScale(facteur, facteur)) ;
    }
}

// Passer a la page des traitements de l'image choisie
void MainWindow::on_pushButton_traitementImage_clicked(){
    // Declaration de variable
    Size n ;
    n.height = 1 ;
    n.width = 3 ;
    vector<int> temp(3) ;
    _seuilBas = temp ;
    _seuilHaut = temp ;
    string nbLigne, nbColonne ;     // Nombre de lignes et de colonnes
    string resolution ;             // Resolution
    QString cheminQT = selectedImagePath(_objBiblio, _indiceImageSelectionnee) ;

    // Preparation de l'espace de travail
    Reinitialiser() ;

    _imageOriginale = readImage(selectedImagePath(_objBiblio, _indiceImageSelectionnee)) ;
    _imageResultat = _imageOriginale ;
    // Affichage de l'image choisie dans la page du traitement
    QPixmap imageOriginaleQT(cheminQT) ;
    QGraphicsScene* sceneImageOriginale = new QGraphicsScene(this) ;
    sceneImageOriginale->addPixmap(imageOriginaleQT) ;
    ui->graphicsView_imageOriginale->setScene(sceneImageOriginale) ;
    ui->graphicsView_imageOriginale->show() ;
    ui->graphicsView_imageOriginale->fitInView(sceneImageOriginale->sceneRect(), Qt::KeepAspectRatio) ;

    // Histogramme de l'image choisie
    _histoImageOriginale = Normalisation(PlotHistogram(_imageOriginale), 255) ;
    QImage histoImageOriginaleQT = QImage((uchar*) _histoImageOriginale.data, _histoImageOriginale.cols, _histoImageOriginale.rows, _histoImageOriginale.step, QImage::Format_RGB888) ;
    QPixmap histoImageOrginalePixmap = QPixmap::fromImage(histoImageOriginaleQT) ;
    int hauteur = ui->label_histogrammeImageOriginale->height() ;       // Hauteur de la fenetre d'affichage
    int largeur = ui->label_histogrammeImageOriginale->width() ;        // Largeur de la fenetre d'affichage
    ui->label_histogrammeImageOriginale->setPixmap(histoImageOrginalePixmap.scaled(largeur, hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_histogrammeImageOriginale->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;

    // Initialiser l'image resultante et son histogramme
    ui->graphicsView_imageTraitee->setScene(sceneImageOriginale) ;
    ui->graphicsView_imageTraitee->show() ;
    ui->graphicsView_imageTraitee->fitInView(sceneImageOriginale->sceneRect(), Qt::KeepAspectRatio) ;

    // Histogramme - Initialisation
    ui->label_histogrammeImageTraitee->setPixmap(histoImageOrginalePixmap.scaled(largeur, hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_histogrammeImageTraitee->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;

    // Afficher la resolution
    nbLigne = to_string((int)_imageOriginale.size().height) ;
    nbColonne = to_string((int)_imageOriginale.size().width) ;
    resolution = "Resolution : [" + nbLigne + " x " + nbColonne + "]" ;
    // Image originale
    ui->label_resolutionOriginale->setText(QString::fromStdString(resolution)) ;
    // Image traitee
    ui->label_resolutionTraitee->setText(QString::fromStdString(resolution)) ;

    // Passer a la page du traitement
    ui->stackedWidget->setCurrentIndex(6) ;
}

// --------------------------Page Traitement--------------------------

// -------------Standars-------------

// Retour a la page pour choisir l'image a traiter
void MainWindow::on_pushButton_retour_clicked(){
    // Desactiver les foncitonnalites
    Reinitialiser() ;

    // Passer a la page precedente
    ui->stackedWidget->setCurrentIndex(5) ;
}

// Agrandissement de l'image originale
void MainWindow::on_horizontalSlider_imageOriginale_valueChanged(int value){
    // Sevlaration des variables
    int hauteurImage = ui->graphicsView_imageOriginale->scene()->height() ;     // Hauteur de l'image
    int largeurImage = ui->graphicsView_imageOriginale->scene()->width() ;      // Largeur de l'image
    int hauteurFenetre = ui->graphicsView_imageOriginale->height() ;            // Hauteur de la fenetre
    int largeurFenetre = ui->graphicsView_imageOriginale->width() ;             // Largeur de la fenetre
    double facteur ;                                                            // Facteur d'agrandissement

    // Calculer le facteur d'agrandissement
    if(hauteurImage > largeurImage){
        facteur = (double)hauteurFenetre/hauteurImage*value/2 ;
    }else{
        facteur = (double)largeurFenetre/largeurImage*value/2 ;
    }

    // Agrandissement
    if(value == 1){
        QGraphicsScene* scene = ui->graphicsView_imageOriginale->scene() ;
        ui->graphicsView_imageOriginale->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    }else{
        ui->graphicsView_imageOriginale->setTransform(QTransform::fromScale(facteur, facteur)) ;
    }
}

// Agrandissement de l'image traitee
void MainWindow::on_horizontalSlider_imageTraitee_valueChanged(int value){
    // Sevlaration des variables
    int hauteurImage = ui->graphicsView_imageTraitee->scene()->height() ;   // Hauteur de l'image
    int largeurImage = ui->graphicsView_imageTraitee->scene()->width() ;    // Largeur de l'image
    int hauteurFenetre = ui->graphicsView_imageTraitee->height() ;          // Hauteur de la fenetre
    int largeurFenetre = ui->graphicsView_imageTraitee->width() ;           // Largeur de la fenetre
    double facteur ;                                                        // Facteur d'agrandissement

    // Calculer le facteur d'agrandissement
    if(hauteurImage > largeurImage){
        facteur = (double)hauteurFenetre/hauteurImage*value/2 ;
    }else{
        facteur = (double)largeurFenetre/largeurImage*value/2 ;
    }

    // Agrandissement
    if(value == 1){
        QGraphicsScene* scene = ui->graphicsView_imageTraitee->scene() ;
        ui->graphicsView_imageTraitee->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    }else{
        ui->graphicsView_imageTraitee->setTransform(QTransform::fromScale(facteur, facteur)) ;
    }
}

// Reinitialiser
void MainWindow::on_pushButton_traitementReinitialiser_clicked(){
    // Declaration de variable
    string nbLigne, nbColonne ;     // Nombre de lignes et de colonnes
    string resolution ;             // Resolution
    QString cheminQT = selectedImagePath(_objBiblio, _indiceImageSelectionnee) ;

    Reinitialiser() ;
    // Affichage de l'image originale
    QPixmap imageOriginaleQT(cheminQT) ;
    QGraphicsScene* sceneImageOriginale = new QGraphicsScene(this) ;
    sceneImageOriginale->addPixmap(imageOriginaleQT) ;
    ui->graphicsView_imageOriginale->setScene(sceneImageOriginale) ;
    ui->graphicsView_imageOriginale->show() ;
    ui->graphicsView_imageOriginale->fitInView(sceneImageOriginale->sceneRect(), Qt::KeepAspectRatio) ;

    // Histogramme de l'image originale
    _imageOriginale = readImage(selectedImagePath(_objBiblio, _indiceImageSelectionnee)) ;
    _histoImageOriginale= Normalisation(PlotHistogram(_imageOriginale), 255) ;
    QImage histoImageOriginaleQT = QImage((uchar*) _histoImageOriginale.data, _histoImageOriginale.cols, _histoImageOriginale.rows, _histoImageOriginale.step, QImage::Format_RGB888) ;
    QPixmap histoImageOrginalePixmap = QPixmap::fromImage(histoImageOriginaleQT) ;
    int hauteur = ui->label_histogrammeImageOriginale->height() ;       // Hauteur de la fenetre d'affichage
    int largeur = ui->label_histogrammeImageOriginale->width() ;        // Largeur de la fenetre d'affichage
    ui->label_histogrammeImageOriginale->setPixmap(histoImageOrginalePixmap.scaled(largeur, hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_histogrammeImageOriginale->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;

    // Initialiser l'image resultante et son histogramme
    ui->graphicsView_imageTraitee->setScene(sceneImageOriginale) ;
    ui->graphicsView_imageTraitee->show() ;
    ui->graphicsView_imageTraitee->fitInView(sceneImageOriginale->sceneRect(), Qt::KeepAspectRatio) ;

    // Histogramme - Initialisation
    ui->label_histogrammeImageTraitee->setPixmap(histoImageOrginalePixmap.scaled(largeur, hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_histogrammeImageTraitee->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;

    // Afficher la resolution
    // Image originale
    nbLigne = to_string((int)_imageOriginale.size().height) ;
    nbColonne = to_string((int)_imageOriginale.size().width) ;
    resolution = "Resolution : [" + nbLigne + " x " + nbColonne + "]" ;
    ui->label_resolutionOriginale->setText(QString::fromStdString(resolution)) ;
    // Image traitee
    ui->label_resolutionTraitee->setText(QString::fromStdString(resolution)) ;
}

// Appliquer le traitement
void MainWindow::on_pushButton_traitementAppliquer_clicked(){
    // Declaration des variables
    string nbLigne, nbColonne ;     // Nombre de lignes et de colonnes
    string resolution ;             // Resolution

    // Mise a jour l'image originale temporelle
    imwrite(temporaryImagePath().toStdString(), _imageResultat) ;
   _imageOriginale= imread(temporaryImagePath().toStdString()) ;
    // Convertir en format QT
    QImage imageOriginaleQT(temporaryImagePath()) ;
    // Histogramme
    _histoImageOriginale= Normalisation(PlotHistogram(_imageOriginale), 255) ;
    QImage histoImageOriginaleQT = QImage((uchar*) _histoImageOriginale.data, _histoImageOriginale.cols, _histoImageOriginale.rows, _histoImageOriginale.step, QImage::Format_RGB888) ;

    // Affichage : Nouvelle image originale
    QGraphicsScene* scene = new QGraphicsScene(this) ;
    scene->addPixmap(QPixmap::fromImage(imageOriginaleQT)) ;
    ui->graphicsView_imageOriginale->setScene(scene) ;
    ui->graphicsView_imageOriginale->show() ;
    ui->graphicsView_imageOriginale->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    // Affichage : Histogramme
    QPixmap histogrammeOriginalePixmap(QPixmap::fromImage(histoImageOriginaleQT)) ;
    int hauteur = ui->label_histogrammeImageOriginale->height() ;       // Hauteur de la fenetre d'affichage
    int largeur = ui->label_histogrammeImageOriginale->width() ;        // Largeur de la fenetre d'affichage
    ui->label_histogrammeImageOriginale->setPixmap(histogrammeOriginalePixmap.scaled(largeur, hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_histogrammeImageOriginale->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;

    // Afficher la resolution
    // Image originale
    nbLigne = to_string((int)_imageOriginale.size().height) ;
    nbColonne = to_string((int)_imageOriginale.size().width) ;
    resolution = "Resolution : [" + nbLigne + " x " + nbColonne + "]" ;
    ui->label_resolutionOriginale->setText(QString::fromStdString(resolution)) ;
    // Image traitee
    ui->label_resolutionTraitee->setText(QString::fromStdString(resolution)) ;
}

// Sauvegarder l'image traitee
void MainWindow::on_pushButton_traitementSauvegarder_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(0,tr("Sauvegarder l'image traitée"), dataFilePath("Images") ,tr("Fichiers Images (*.png *.jpg *.bmp *.pgm *.jpeg *.tiff)")) ;
    if (!fileName.isEmpty()){
        if (fileName.count('.')){
         string fileNameStr = fileName.toStdString() ;
         imwrite(fileNameStr, _imageResultat) ;
    }else{
        fileName.append(".jpg");
        string fileNameStr = fileName.toStdString() ;
        imwrite(fileNameStr, _imageResultat) ;
    }
    }
}

// Afficher l'image traitee
void MainWindow::on_pushButtonAfficherImageTraitee_clicked()
{
    // Mise a jour l'image originale temporelle
    Mat imageResultat = ImageBGRRGB(_imageResultat);
    QImage imageResultatQT = QImage((uchar*) imageResultat.data, imageResultat.cols, imageResultat.rows, imageResultat.step, QImage::Format_RGB888) ;
    // Affichage de l'image traitée
    QGraphicsScene* scene = new QGraphicsScene(this) ;
    scene->addPixmap(QPixmap::fromImage(imageResultatQT));
    ui->graphicsView_contenuImageTraiteeAgrandi->setScene(scene) ;
    ui->graphicsView_contenuImageTraiteeAgrandi->show() ;
    ui->graphicsView_contenuImageTraiteeAgrandi->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    ui->stackedWidget->setCurrentIndex(7);


}

// -------------Luminosite-------------

// Initialiser
void MainWindow::on_groupBox_correction_clicked(){
    AffichageResultat(_imageOriginale, 1) ;                      // Image originale
    // Si le box est choisi
    if(ui->groupBox_correction->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserDetail() ;                 // Details
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserContour() ;                // Contours
        ReinitialiserDebruitage() ;             // Debruitage
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserAutre() ;                  // Autres
    // Sinon : Reinitialiser
    }else{
        ReinitialiserLuminosite() ;
    }
}

// Bouton : Luminosite
void MainWindow::on_radioButton_luminosite_clicked(){
    // Si le bouton est coche
    if(ui->radioButton_luminosite->isChecked()){
        // Desactiver les autres fonctionnalites
        // Contraste
        ui->radioButton_contraste->setChecked(false) ;
        ui->horizontalSlider_contraste->setValue(0) ;
        ui->horizontalSlider_contraste->setEnabled(false) ;
        // Ombre
        ui->radioButton_ombre->setChecked(false) ;
        ui->horizontalSlider_ombre->setValue(0) ;
        ui->horizontalSlider_ombre->setEnabled(false) ;
        // Haute lumiere
        ui->radioButton_brillance->setChecked(false) ;
        ui->horizontalSlider_brillance->setValue(0) ;
        ui->horizontalSlider_brillance->setEnabled(false) ;

        // Activer et initialiser le slider
        ui->horizontalSlider_luminosite->setEnabled(true) ;
        ui->horizontalSlider_luminosite->setValue(0) ;
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Modifier la luminosite de l'image
void MainWindow::on_horizontalSlider_luminosite_valueChanged(int value){
    // Application du reglage de luminosite
    _imageResultat = ImageLuminosite(_imageOriginale, value) ;

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Bouton : Contraste
void MainWindow::on_radioButton_contraste_clicked(){
    // Si le bouton est coche
    if(ui->radioButton_contraste->isChecked()){
        // Desactiver les autres fonctionnalites
        // Luminosite
        ui->radioButton_luminosite->setChecked(false) ;
        ui->horizontalSlider_luminosite->setValue(0) ;
        ui->horizontalSlider_luminosite->setEnabled(false) ;
        // Ombre
        ui->radioButton_ombre->setChecked(false) ;
        ui->horizontalSlider_ombre->setValue(0) ;
        ui->horizontalSlider_ombre->setEnabled(false) ;
        // Haute lumiere
        ui->radioButton_brillance->setChecked(false) ;
        ui->horizontalSlider_brillance->setValue(0) ;
        ui->horizontalSlider_brillance->setEnabled(false) ;

        // Activer et initialiser le slider
        ui->horizontalSlider_contraste->setEnabled(true) ;
        ui->horizontalSlider_contraste->setValue(0) ;
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Modifier le constraste de l'image
void MainWindow::on_horizontalSlider_contraste_valueChanged(int value){
    this->setCursor(Qt::WaitCursor);
    // Application de la normalisation
    _imageResultat = Normalisation(_imageOriginale, value) ;

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
    this->setCursor(Qt::ArrowCursor);
}

// Bouton : Ombre
void MainWindow::on_radioButton_ombre_clicked(){
    // Si le bouton est coche
    if(ui->radioButton_ombre->isChecked()){
        // Desactiver les autres fonctionnalites
        // Luminosite
        ui->radioButton_luminosite->setChecked(false) ;
        ui->horizontalSlider_luminosite->setValue(0) ;
        ui->horizontalSlider_luminosite->setEnabled(false) ;
        // Contraste
        ui->radioButton_contraste->setChecked(false) ;
        ui->horizontalSlider_contraste->setValue(0) ;
        ui->horizontalSlider_contraste->setEnabled(false) ;
        // Haute lumiere
        ui->radioButton_brillance->setChecked(false) ;
        ui->horizontalSlider_brillance->setValue(0) ;
        ui->horizontalSlider_brillance->setEnabled(false) ;

        // Activer et initialiser le slider
        ui->horizontalSlider_ombre->setEnabled(true) ;
        ui->horizontalSlider_ombre->setValue(0) ;
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Ombre
void MainWindow::on_horizontalSlider_ombre_valueChanged(int value){
    _imageResultat = ImageOmbre(_imageOriginale, value) ;
    AffichageResultat(_imageResultat, 1) ;
}

// Bouton : Haute lumiere
void MainWindow::on_radioButton_brillance_clicked(){
    // Si le bouton est coche
    if(ui->radioButton_brillance->isChecked()){
        // Desactiver les autres fonctionnalites
        // Luminosite
        ui->radioButton_luminosite->setChecked(false) ;
        ui->horizontalSlider_luminosite->setValue(0) ;
        ui->horizontalSlider_luminosite->setEnabled(false) ;
        // Contraste
        ui->radioButton_contraste->setChecked(false) ;
        ui->horizontalSlider_contraste->setValue(0) ;
        ui->horizontalSlider_contraste->setEnabled(false) ;
        // Ombre
        ui->radioButton_ombre->setChecked(false) ;
        ui->horizontalSlider_ombre->setValue(0) ;
        ui->horizontalSlider_ombre->setEnabled(false) ;

        // Activer et initialiser le slider
        ui->horizontalSlider_brillance->setEnabled(true) ;
        ui->horizontalSlider_brillance->setValue(0) ;
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Haute lumiere
void MainWindow::on_horizontalSlider_brillance_valueChanged(int value){
    _imageResultat = ImageHauteLumiere(_imageOriginale, value) ;
    AffichageResultat(_imageResultat, 1) ;
}

// -------------Details-------------

// Initialiser
void MainWindow::on_groupBox_details_clicked(){
    AffichageResultat(_imageOriginale, 1) ;     // Image originale
    // Si le box est choisi
    if(ui->groupBox_details->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserAutre() ;                  // Autres
    // Sinon : Reinitialiser
    }else{
        ReinitialiserDetail() ;
    }
}

// Bouton nettete
void MainWindow::on_radioButton_nettete_clicked(){
    if(ui->radioButton_nettete->isChecked()){
        // Activer et initialiser le slider
        ui->horizontalSlider_nettete->setEnabled(true) ;
        ui->horizontalSlider_nettete->setValue(0) ;

        ReinitialiserContour() ;

        // Initialiser l'image traitee
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Modifier la nettete de l'image
void MainWindow::on_horizontalSlider_nettete_valueChanged(int value){
        this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_nettete->isChecked()){
        // Rendre flou
        if(value < 0){
            // _imageResultat = imageTemp[abs(value)-1] ;
            _imageResultat = ImageLissage(_imageOriginale, abs(value)) ;
            /*if(value%2 == 0){
                _imageResultat = ImageBGRRGB(_imageResultat) ;
            }*/
        // Rehaussement de contours
        }else if(value > 0){
            _imageResultat = ImageRehaussementContour(_imageOriginale, value*10) ;
        // Image de depart
        }else{
            _imageResultat = _imageOriginale ;
        }

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
        this->setCursor(Qt::ArrowCursor);
}

// Bouton : Detection de contours
void MainWindow::on_groupBox_contours_clicked(){
    AffichageResultat(_imageOriginale, 1) ;                      // Image originale
    // Si le box est choisi
    if(ui->groupBox_contours->isChecked()){
        // Desactiver les autres fonctionnalites
        ui->radioButton_nettete->setEnabled(false) ;
        ui->radioButton_nettete->setChecked(false) ;
        ui->radioButton_nettete->setEnabled(true) ;
        ui->horizontalSlider_nettete->setEnabled(false) ;
        ui->horizontalSlider_nettete->setValue(0) ;

    // Sinon : Reinitialiser
    }else{
        ReinitialiserContour() ;
    }
}

// Detection de contours - Gradient
void MainWindow::on_radioButton_contoursGradient_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_contoursGradient->isChecked()){
        ui->radioButton_contoursLaplacien->setChecked(false) ;
        imwrite(temporaryImagePath().toStdString(), _imageOriginale) ;
        _imageResultat = MonoCouleur(ImageContourGradient(_imageOriginale)) ;
        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// Detection de contours - Laplacien
void MainWindow::on_radioButton_contoursLaplacien_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_contoursLaplacien->isChecked()){
        if(ui->groupBox_contours->isChecked()){
            ui->radioButton_contoursGradient->setChecked(false) ;
            _imageResultat = MonoCouleur(ImageContourLaplace(_imageOriginale)) ;
            // Affichage du resultat
            AffichageResultat(_imageResultat, 1) ;
        }
    }
    this->setCursor(Qt::ArrowCursor);
}

// -------------Bruitage et debruitage-------------

// Initialiser
void MainWindow::on_groupBox_bruitageDebruitage_clicked(){
    AffichageResultat(_imageOriginale, 1) ;     // Image originale
    // Si le box est choisi
    if(ui->groupBox_bruitageDebruitage->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserDetail() ;                 // Contours
        ReinitialiserAutre() ;                  // Autres
    // Sinon : Reinitialiser
    }else{
        ReinitialiserBruitageDebruitage() ;
    }
}

// Bouton bruitage
void MainWindow::on_radioButton_bruitage_clicked(){
    AffichageResultat(_imageOriginale, 1) ;     // Image originale
    if(ui->radioButton_bruitage->isChecked()){
        // Desactiver les autres corrections
        ui->groupBox_debruitage->setChecked(false) ;
        ReinitialiserDebruitage() ;

        // Activer et initialiser le slider
        ui->horizontalSlider_Bruitage->setEnabled(true) ;
        ui->horizontalSlider_Bruitage->setValue(0) ;

        // Initialiser l'image traitee
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Bruiter l'image
void MainWindow::on_horizontalSlider_Bruitage_valueChanged(int value){
    // Bruiter l'image par un bruit gaussien
    _imageResultat = ImageBruitGaussien(_imageOriginale,0, value) ;

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Bouton : Debruitage
void MainWindow::on_groupBox_debruitage_clicked(){
    AffichageResultat(_imageOriginale, 1) ;     // Image originale
    // Si le box est choisi
    if(ui->groupBox_debruitage->isChecked()){
        // Desactiver les aures fonctionnalites
        ui->radioButton_bruitage->setChecked(false) ;
        ui->horizontalSlider_Bruitage->setEnabled(false) ;
        ui->horizontalSlider_Bruitage->setValue(0) ;

        // Activer les fonctionnalites
        ui->radioButton_moyenneur->setEnabled(true) ;
        ui->radioButton_gaussien->setEnabled(true) ;
        ui->radioButton_median->setEnabled(true) ;
        ui->radioButton_kuwahara->setEnabled(true) ;
    // Sinon : Reinitialiser
    }else{
        ReinitialiserDebruitage() ;
    }
}

// Filtre moyeneur
void MainWindow::on_radioButton_moyenneur_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_moyenneur->isChecked() == false){
        ui->radioButton_gaussien->setChecked(false) ;
        ui->radioButton_median->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(false) ;
        ui->radioButton_moyenneur->setChecked(true) ;
    }
    if(ui->radioButton_moyenneur->isChecked()){
        ui->radioButton_gaussien->setChecked(false) ;
        ui->radioButton_median->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(false) ;

        ui->radioButton_moyenneur->setChecked(true) ;
        _imageResultat = ImageFiltrage(_imageOriginale, 1) ;
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// Filtre gaussien
void MainWindow::on_radioButton_gaussien_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_gaussien->isChecked() == false){
        ui->radioButton_moyenneur->setChecked(false) ;
        ui->radioButton_median->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(false) ;
        ui->radioButton_gaussien->setChecked(true) ;
    }
    if(ui->radioButton_gaussien->isChecked()){
        ui->radioButton_moyenneur->setChecked(false) ;
        ui->radioButton_median->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(false) ;

        _imageResultat = ImageFiltrage(_imageOriginale, 2) ;
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// Filtre median
void MainWindow::on_radioButton_median_clicked(){
    this->setCursor(Qt::WaitCursor) ;
    if(ui->radioButton_median->isChecked() == false){
        ui->radioButton_moyenneur->setChecked(false) ;
        ui->radioButton_gaussien->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(false) ;
        ui->radioButton_median->setChecked(true) ;
    }
    if(ui->radioButton_median->isChecked()){
        ui->radioButton_moyenneur->setChecked(false) ;
        ui->radioButton_gaussien->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(false) ;

        _imageResultat = ImageMedian(_imageOriginale) ;
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor) ;
}

// Kuwahara-Nagao
void MainWindow::on_radioButton_kuwahara_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_kuwahara->isChecked() == false){
        ui->radioButton_moyenneur->setChecked(false) ;
        ui->radioButton_gaussien->setChecked(false) ;
        ui->radioButton_median->setChecked(false) ;
        ui->radioButton_kuwahara->setChecked(true) ;
    }
    if(ui->radioButton_kuwahara->isChecked()){
        ui->radioButton_moyenneur->setChecked(false) ;
        ui->radioButton_gaussien->setChecked(false) ;
        ui->radioButton_median->setChecked(false) ;

        _imageResultat = ImageKuwahara(_imageOriginale) ;
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// -------------Effets-------------

// Initialiser
void MainWindow::on_groupBox_filtres_clicked(){
    AffichageResultat(_imageOriginale, 1) ;                      // Image originale
    // Si le box est choisi
    if(ui->groupBox_filtres->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserDetail() ;                 // COntours
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserAutre() ;                  // Autres
        // Si l'image est en niveau de gris : Desactiver les filtres en couleurs
        if(VerifierImage(_imageOriginale, _imageResultat)){
            ui->frame_bleu->setEnabled(false) ;
            ui->frame_rouge->setEnabled(false) ;
            ui->frame_vert->setEnabled(false) ;
            ui->frame_pop->setEnabled(false) ;
            ui->frame_jaune->setEnabled(false) ;
            ui->frame_cyan->setEnabled(false) ;
            ui->frame_magenta->setEnabled(false) ;
        }
        // Sinon : Reinitialiser
    }else{
        ReinitialiserFiltre() ;
    }
}

// Originale
void MainWindow::on_radioButton_originale_clicked(){
    if(ui->radioButton_originale->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_niveauGris->setChecked(false) ;     // Niveau de gris
        ui->radioButton_sepia->setChecked(false) ;          // Sepia
        ui->radioButton_inversement->setChecked(false) ;    // Inversement
        ui->radioButton_rouge->setChecked(false) ;          // Rouge
        ui->radioButton_vert->setChecked(false) ;           // Vert
        ui->radioButton_bleu->setChecked(false) ;           // Bleu
        ui->radioButton_cyan->setChecked(false) ;           // Cyan
        ui->radioButton_jaune->setChecked(false) ;          // Jaune
        ui->radioButton_magenta->setChecked(false) ;        // Magenta
        ui->radioButton_rgb->setChecked(false) ;            // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;  // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;     // Egalisation d'histogramme

        _imageResultat =_imageOriginale ;
        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image monochrome
void MainWindow::on_radioButton_niveauGris_clicked(){
    ui->radioButton_niveauGris->setChecked(true) ;
    if(ui->radioButton_niveauGris->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        // Convertir l'image en niveau de gris
        if(ui->radioButton_niveauGris->isChecked()){
            _imageResultat = MonoCouleur(ImageMonochrome(_imageOriginale)) ;
        }

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image sepia
void MainWindow::on_radioButton_sepia_clicked(){
    ui->radioButton_sepia->setChecked(true) ;
    if(ui->radioButton_sepia->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        // Convertir l'image en sepia
        _imageResultat = ImageSepia(_imageOriginale) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image inversee
void MainWindow::on_radioButton_inversement_clicked(){
    ui->radioButton_inversement->setChecked(true) ;
    if(ui->radioButton_inversement->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        // Inverser les niveaux d'intensite de l'image
        _imageResultat = ImageInversement(_imageOriginale) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image rouge
void MainWindow::on_radioButton_rouge_clicked(){
    ui->radioButton_rouge->setChecked(true) ;
    if(ui->radioButton_rouge->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        // Image rouge
        _imageResultat = ImageRouge(_imageOriginale) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image verte
void MainWindow::on_radioButton_vert_clicked(){
    ui->radioButton_vert->setChecked(true) ;
    if(ui->radioButton_vert->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        // Image verte
        _imageResultat = ImageVert(_imageOriginale) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 0) ;
    }
}

// Image bleue
void MainWindow::on_radioButton_bleu_clicked(){
    ui->radioButton_bleu->setChecked(true) ;
    if(ui->radioButton_bleu->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        // Image bleue
        _imageResultat = ImageBleu(_imageOriginale) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image jaune
void MainWindow::on_radioButton_jaune_clicked(){
    ui->radioButton_jaune->setChecked(true) ;
    if(ui->radioButton_jaune->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB


        _imageResultat = ImageJaune(_imageOriginale.clone()) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image cyane
void MainWindow::on_radioButton_cyan_clicked(){
    ui->radioButton_cyan->setChecked(true) ;
    if(ui->radioButton_cyan->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme


        _imageResultat = ImageCyan(_imageOriginale.clone()) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image magenta
void MainWindow::on_radioButton_magenta_clicked(){
    ui->radioButton_magenta->setChecked(true) ;
    if(ui->radioButton_magenta->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        _imageResultat = ImageMagenta(_imageOriginale.clone()) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Image RGB
void MainWindow::on_radioButton_rgb_clicked(){
    ui->radioButton_rgb->setChecked(true) ;
    if(ui->radioButton_rgb->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_niveauGris->setChecked(false) ;             // Monochrome
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel
        ui->radioButtonEgalisation->setChecked(false) ;             // Egalisation d'histogramme

        _imageResultat = ImageRGB(_imageOriginale.clone()) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Egalisation d'histogramme
void MainWindow::on_radioButtonEgalisation_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButtonEgalisation->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButton_niveauGris->setChecked(false) ;             // Nilveau de gris
        ui->radioButtonBruitPoivreSel->setChecked(false) ;          // Bruit poivre et sel

        // Egalisation d'histogramme
        _imageResultat = ImageEgalisation(_imageOriginale.clone()) ;
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// Bruit poivre et sel
void MainWindow::on_radioButtonBruitPoivreSel_clicked(){
    if(ui->radioButtonBruitPoivreSel->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_originale->setChecked(false) ;              // Image originale
        ui->radioButton_sepia->setChecked(false) ;                  // Sepia
        ui->radioButton_inversement->setChecked(false) ;            // Inversement
        ui->radioButton_rouge->setChecked(false) ;                  // Rouge
        ui->radioButton_vert->setChecked(false) ;                   // Vert
        ui->radioButton_bleu->setChecked(false) ;                   // Bleu
        ui->radioButton_cyan->setChecked(false) ;                   // Cyan
        ui->radioButton_jaune->setChecked(false) ;                  // Jaune
        ui->radioButton_magenta->setChecked(false) ;                // Magenta
        ui->radioButton_rgb->setChecked(false) ;                    // RGB
        ui->radioButton_niveauGris->setChecked(false) ;             // Nilveau de gris
        ui->radioButtonEgalisation->setChecked(false) ;           // Bruit gaussien

        // Ajouter du bruit poivre et sel dans l'image
        _imageResultat = ImageBruitPoivreSel(_imageOriginale) ;
        AffichageResultat(_imageResultat, 1) ;
    }

}

// -------------Extraction du canal de couleur-------------

// Initialiser
void MainWindow::on_groupBox_extractionRVB_clicked(){
    AffichageResultat(_imageOriginale, 1) ;                      // Image originale
    // Si le box est choisi
    if(ui->groupBox_extractionRVB->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserDetail() ;                 // Contours
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserAutre() ;                  // Autres

        // Activer les fonctionnalites
        ui->radioButton_extractionR->setEnabled(true) ;             // Rouge
        ui->radioButton_extractionV->setEnabled(true) ;             // Vert
        ui->radioButton_extractionB->setEnabled(true) ;             // Bleu
    // Sinon
    }else{
        ReinitialiserExtraction() ;
    }
}

// Extraction du canal rouge de l'image
void MainWindow::on_radioButton_extractionR_clicked(){
    if(ui->radioButton_extractionR->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_extractionB->setChecked(false) ;    // Canal bleu
        ui->radioButton_extractionV->setChecked(false) ;    // Canal vert

        // Extraction du canal rouge
        _imageResultat = MonoCouleur(ImageExtractionCouleur(_imageOriginale, 3)) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Extraction du canal vert de l'image
void MainWindow::on_radioButton_extractionV_clicked(){
    if(ui->radioButton_extractionV->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_extractionR->setChecked(false) ;    // Canal rouge
        ui->radioButton_extractionB->setChecked(false) ;    // Canal bleu

        // Extraction du canal vert
        _imageResultat = MonoCouleur(ImageExtractionCouleur(_imageOriginale, 2)) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// Extraction du canal bleu de l'image
void MainWindow::on_radioButton_extractionB_clicked(){
    if(ui->radioButton_extractionB->isChecked()){
        // Desactiver les autres filtres
        ui->radioButton_extractionR->setChecked(false) ;    // Canal rouge
        ui->radioButton_extractionV->setChecked(false) ;    // Canal vert

        // Extraction du canal bleu
        _imageResultat = MonoCouleur(ImageExtractionCouleur(_imageOriginale, 1)) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
}

// -------------Seuillage & Segmentation-------------

// Initialiser
void MainWindow::on_groupBox_seuillageSegmentation_clicked(){
    AffichageResultat(_imageOriginale, 1) ;                      // Image originale
    // Si le box est choisi
    if(ui->groupBox_seuillageSegmentation->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserDetail() ;                 // Contours
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserAutre() ;                  // Autres
    // Sinon : Reinitialiser
    }else{
        ReinitialiserSeuillageSegmentation() ;
    }
}

// Initialiser le seuillage
void MainWindow::on_radioButton_seuillage_clicked(){
    _imageResultat = _imageOriginale ;
    AffichageResultat(_imageResultat, 1) ;
    // Si l'utilisateur choisit le seuillage
    if(ui->radioButton_seuillage->isChecked()){
        ui->radioButton_segmentation->setChecked(false) ;       // Desactiver la segmentation
        ui->groupBox_simpleHysteresis->setEnabled(true) ;       // Activer la zone de choix du type de seuils
        ui->groupBox_seuilBas->setEnabled(true) ;               // Activer les sliders des seuils bas

        // Initialiser les sliders
        // Seuils bas
        ui->verticalSlider_seuilBasR_2->setValue(0) ;           // Rouge
        ui->verticalSlider_seuilBasR_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasV_2->setValue(0) ;           // Vert
        ui->verticalSlider_seuilBasV_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasB_2->setValue(0) ;           // Bleu
        ui->verticalSlider_seuilBasB_2->setEnabled(true) ;
        // Seuils hauts
        ui->verticalSlider_seuilHautR_2->setValue(256) ;        // Rouge
        ui->verticalSlider_seuilHautR_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautV_2->setValue(256) ;        // Vert
        ui->verticalSlider_seuilHautV_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautB_2->setValue(256) ;        // Bleu
        ui->verticalSlider_seuilHautB_2->setEnabled(true) ;
    }else{
        ReinitialiserSeuillageSegmentation() ;
        ui->groupBox_seuillageSegmentation->setChecked(true) ;
    }
}

// Initialiser la segmentation
void MainWindow::on_radioButton_segmentation_clicked(){
    _imageResultat = _imageOriginale ;
    AffichageResultat(_imageResultat, 1) ;
    if(ui->radioButton_segmentation->isChecked()){
        ui->radioButton_seuillage->setChecked(false) ;
        ui->groupBox_simpleHysteresis->setEnabled(true) ;       // Activer la zone de choix du type de seuils
        ui->groupBox_seuilBas->setEnabled(true) ;               // Activer les sliders des seuils bas

        // Initialiser les sliders
        // Seuils bas
        ui->verticalSlider_seuilBasR_2->setValue(0) ;           // Rouge
        //ui->verticalSlider_seuilBasR_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasV_2->setValue(0) ;           // Vert
        //ui->verticalSlider_seuilBasV_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasB_2->setValue(0) ;           // Bleu
        //ui->verticalSlider_seuilBasB_2->setEnabled(true) ;
        // Seuils hauts
        ui->verticalSlider_seuilHautR_2->setValue(256) ;        // Rouge
        //ui->verticalSlider_seuilHautR_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautV_2->setValue(256) ;        // Vert
        //ui->verticalSlider_seuilHautV_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautB_2->setValue(256) ;        // Bleu
        //ui->verticalSlider_seuilHautB_2->setEnabled(true) ;
    }else{
        ReinitialiserSeuillageSegmentation() ;
        ui->groupBox_seuillageSegmentation->setChecked(true) ;
    }
}

// Initialiser le seuillage par seuils simples
void MainWindow::on_radioButton_seuillageSimple_clicked(){
    _imageResultat = _imageOriginale ;
    AffichageResultat(_imageResultat, 1) ;
    // Si l'utilisateur choisit d'appliquer les seuils simples
    if(ui->radioButton_seuillageSimple->isChecked()){
        // Desactiver le seuillage par hysteresis
        ui->radioButton_seuillageHysteresis->setChecked(false) ;

        ui->verticalSlider_seuilBasR_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasV_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasB_2->setEnabled(true) ;

        // Desactiver les sliders des seuils hauts
        ui->groupBox_seuilHaut->setEnabled(false) ;
        // Reinitialiser les seuils hauts
        _seuilBas[2] = 0 ;      // Rouge
        _seuilBas[1] = 0 ;      // Vert
        _seuilBas[0] = 0 ;      // Bleu

        // Reinitialiser les seuils hauts
        _seuilHaut[2] = 256 ;    // Rouge
        _seuilHaut[1] = 256 ;    // Vert
        _seuilHaut[0] = 256 ;    // Bleu
    }
}

// Initialiser le seuillage par seuils par hysteresis
void MainWindow::on_radioButton_seuillageHysteresis_clicked(){
    _imageResultat = _imageOriginale ;
    AffichageResultat(_imageResultat, 1) ;
    // Si l'utilisateur choisit d'appliquer les seuils hysteresis
    if(ui->radioButton_seuillageHysteresis->isChecked()){
        // Desactiver le seuillage simple
        ui->radioButton_seuillageSimple->setChecked(false) ;
        // Activer les sliders des seuils hauts
        ui->groupBox_seuilHaut->setEnabled(true) ;
        ui->verticalSlider_seuilBasR_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasV_2->setEnabled(true) ;
        ui->verticalSlider_seuilBasB_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautR_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautV_2->setEnabled(true) ;
        ui->verticalSlider_seuilHautB_2->setEnabled(true) ;
    }
}

// Seuil bas rouge
void MainWindow::on_verticalSlider_seuilBasR_2_valueChanged(int value){
    // Si l'image est en niveau de gris, les trois seuils sont egaux
    if(VerifierImage(_imageOriginale, _imageResultat)){
        ui->verticalSlider_seuilBasV_2->setSliderPosition(value) ;      // Regler slider du seuil bas - vert
        ui->verticalSlider_seuilBasB_2->setSliderPosition(value) ;      // Regler slider du seuil bas - bleu
        // Relever les valeurs des seuils
        _seuilBas[2] = value ;                                           // Seuil bas rouge
        _seuilBas[1] = value ;                                           // Seuil bas vert
        _seuilBas[0] = value ;                                           // Seuil bas bleu
    // Si seuillage par hysteresis
    }else{
        _seuilBas[2] = value ;                                           // Seuil bas rouge
    }

    // Definir l'intervalle du seuil bas
    // Si seuillage simple
    if(ui->radioButton_seuillageSimple->isChecked()){
        ui->verticalSlider_seuilBasR_2->setRange(0, 256) ;
    // Si seuillage par hysteresis
    }else if(ui->radioButton_seuillageHysteresis->isChecked()){
        ui->verticalSlider_seuilBasR_2->setRange(0, ui->verticalSlider_seuilHautR_2->value()) ;
    }

    // Image resultante
    // Seuillage
    if(ui->radioButton_seuillage->isChecked()){
        _imageResultat = ImageSeuillage(_imageOriginale, _seuilBas, _seuilHaut) ;
    // Segmentation
    }else if(ui->radioButton_segmentation->isChecked()){
        _imageResultat = ImageSegmentation(_imageOriginale, _seuilBas, _seuilHaut) ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Seuil bas vert
void MainWindow::on_verticalSlider_seuilBasV_2_valueChanged(int value){
    // Si l'image est en niveau de gris, les trois seuils sont egaux
    if(VerifierImage(_imageOriginale, _imageResultat)){
        ui->verticalSlider_seuilBasR_2->setSliderPosition(value) ;      // Regler slider du seuil bas - rouge
        ui->verticalSlider_seuilBasB_2->setSliderPosition(value) ;      // Regler slider du seuil bas - bleu
        // Relever les valeurs des seuils
        _seuilBas[2] = value ;                                           // Seuil bas rouge
        _seuilBas[1] = value ;                                           // Seuil bas vert
        _seuilBas[0] = value ;                                           // Seuil bas bleu
    // Si seuillage par hysteresis
    }else{
        _seuilBas[1] = value ;                                           // Seuil bas vert
    }

    // Si seuillage simple
    // Definir l'intervalle du seuil bas
    if(ui->radioButton_seuillageSimple->isChecked()){
        ui->verticalSlider_seuilBasV_2->setRange(0, 256) ;
    // Si seuillage par hysteresis
    }else if(ui->radioButton_seuillageHysteresis->isChecked()){
        ui->verticalSlider_seuilBasV_2->setRange(0, ui->verticalSlider_seuilHautV_2->value()) ;
    }

    // Image resultante
    // Seuillage
    if(ui->radioButton_seuillage->isChecked()){
        _imageResultat = ImageSeuillage(_imageOriginale, _seuilBas, _seuilHaut) ;
    // Segmentation
    }else if(ui->radioButton_segmentation->isChecked()){
        _imageResultat = ImageSegmentation(_imageOriginale, _seuilBas, _seuilHaut) ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Seuil bas bleu
void MainWindow::on_verticalSlider_seuilBasB_2_valueChanged(int value){
    // Si l'image est en niveau de gris, les trois seuils sont egaux
    if(VerifierImage(_imageOriginale, _imageResultat)){
        ui->verticalSlider_seuilBasR_2->setSliderPosition(value) ;      // Regler slider du seuil bas - rouge
        ui->verticalSlider_seuilBasV_2->setSliderPosition(value) ;      // Regler slider du seuil bas - vert
        // Relever les valeurs des seuils
        _seuilBas[2] = value ;                                           // Seuil bas rouge
        _seuilBas[1] = value ;                                           // Seuil bas vert
        _seuilBas[0] = value ;                                           // Seuil bas bleu
    }else{
        _seuilBas[0] = value ;                                           // Seuil bas bleu
    }

    // Si seuillage simple
    // Definir l'intervalle du seuil bas
    if(ui->radioButton_seuillageSimple->isChecked()){
        ui->verticalSlider_seuilBasB_2->setRange(0, 256) ;
    // Si seuillage par hysteresis
    }else if(ui->radioButton_seuillageHysteresis->isChecked()){
        ui->verticalSlider_seuilBasB_2->setRange(0, ui->verticalSlider_seuilHautB_2->value()) ;
    }

    // Image resultante
    // Seuillage
    if(ui->radioButton_seuillage->isChecked()){
        _imageResultat = ImageSeuillage(_imageOriginale, _seuilBas, _seuilHaut) ;
    // Seuillage
    }else if(ui->radioButton_segmentation->isChecked()){
        _imageResultat = ImageSegmentation(_imageOriginale, _seuilBas, _seuilHaut) ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Seuil haut rouge
void MainWindow::on_verticalSlider_seuilHautR_2_valueChanged(int value){
    // Si l'image est en niveau de gris, les trois seuils sont egaux
    if(VerifierImage(_imageOriginale, _imageResultat)){
        ui->verticalSlider_seuilHautV_2->setSliderPosition(value) ;     // Regler slider du seuil haut - vert
        ui->verticalSlider_seuilHautB_2->setSliderPosition(value) ;     // Regler slider du seuil haut - bleu
        // Relever les valeurs des seuils
        _seuilHaut[2] = value ;                                          // Seuil haut rouge
        _seuilHaut[1] = value ;                                          // Seuil haut vert
        _seuilHaut[0] = value ;                                          // Seuil haut bleu
    }else{
        _seuilHaut[2] = value ;                                          // Seuil haut rouge
    }

    // Si seuillage hysteresis
    // Definir l'intervalle du seuil haut
    if(ui->radioButton_seuillageHysteresis->isChecked()){
        ui->verticalSlider_seuilHautR_2->setRange(ui->verticalSlider_seuilBasR_2->value(), 256) ;
    }

    // Image resultante
    // Seuillage
    if(ui->radioButton_seuillage->isChecked()){
        _imageResultat = ImageSeuillage(_imageOriginale, _seuilBas, _seuilHaut) ;
    // Segmentation
    }else if(ui->radioButton_segmentation->isChecked()){
        _imageResultat = ImageSegmentation(_imageOriginale, _seuilBas, _seuilHaut) ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Seuil haut vert
void MainWindow::on_verticalSlider_seuilHautV_2_valueChanged(int value){
    // Si l'image est en niveau de gris, les trois seuils sont egaux
    if(VerifierImage(_imageOriginale, _imageResultat)){
        ui->verticalSlider_seuilHautR_2->setSliderPosition(value) ;     // Regler slider du seuil haut - rouge
        ui->verticalSlider_seuilHautB_2->setSliderPosition(value) ;     // Regler slider du seuil haut - bleu
        // Relever les valeurs des seuils
        _seuilHaut[2] = value ;                                          // Seuil haut rouge
        _seuilHaut[1] = value ;                                          // Seuil haut vert
        _seuilHaut[0] = value ;                                          // Seuil haut bleu
    }else{
        _seuilHaut[1] = value ;                                          // Seuil haut vert
    }

    // Definir l'intervalle du seuil haut
    // Si seuillage hysteresis
    if(ui->radioButton_seuillageHysteresis->isChecked()){
        ui->verticalSlider_seuilHautV_2->setRange(ui->verticalSlider_seuilBasV_2->value(), 256) ;
    }

    // Image resultante
    // Seuillage
    if(ui->radioButton_seuillage->isChecked()){
        _imageResultat = ImageSeuillage(_imageOriginale, _seuilBas, _seuilHaut) ;
    // Seuillage
    }else if(ui->radioButton_segmentation->isChecked()){
        _imageResultat = ImageSegmentation(_imageOriginale, _seuilBas, _seuilHaut) ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Seuil haut bleu
void MainWindow::on_verticalSlider_seuilHautB_2_valueChanged(int value){
    // Si l'image est en niveau de gris, les trois seuils sont egaux
    if(VerifierImage(_imageOriginale, _imageResultat)){
        ui->verticalSlider_seuilHautR_2->setSliderPosition(value) ;     // Regler slider du seuil haut - rouge
        ui->verticalSlider_seuilHautV_2->setSliderPosition(value) ;     // Regler slider du seuil haut - ver
        // Relever les valeurs des seuils
        _seuilHaut[2] = value ;                                          // Seuil haut rouge
        _seuilHaut[1] = value ;                                          // Seuil haut vert
        _seuilHaut[0] = value ;                                          // Seuil haut bleu
    }else{
        _seuilHaut[0] = value ;                                          // Seuil haut bleu
    }

    // Si seuillage hysteresis
    // Definir l'intervalle du seuil haut
    if(ui->radioButton_seuillageHysteresis->isChecked()){
        ui->verticalSlider_seuilHautB_2->setRange(ui->verticalSlider_seuilBasB_2->value(), 256) ;
    }

    // Image resultante
    // Seuillage
    if(ui->radioButton_seuillage->isChecked()){
        _imageResultat = ImageSeuillage(_imageOriginale, _seuilBas, _seuilHaut) ;
    // Segmentation
    }else if(ui->radioButton_segmentation->isChecked()){
        _imageResultat = ImageSegmentation(_imageOriginale, _seuilBas, _seuilHaut) ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Reinitialiser seuil bas rouge
void MainWindow::on_pushButton_seuilBasR_clicked(){
    ui->verticalSlider_seuilBasR_2->setRange(0, 255) ;          // Intervalle
    ui->verticalSlider_seuilBasR_2->setValue(0) ;               // Valeur
}

// Reinitialiser seuil bas vert
void MainWindow::on_pushButton_seuilBasV_clicked(){
    ui->verticalSlider_seuilBasV_2->setRange(0, 255) ;          // Intervalle
    ui->verticalSlider_seuilBasV_2->setValue(0) ;               // Valeur
}

// Reinitialiser seuil bas bleu
void MainWindow::on_pushButton_seuilBasB_clicked(){
    ui->verticalSlider_seuilBasB_2->setRange(0, 255) ;          // Intervalle
    ui->verticalSlider_seuilBasB_2->setValue(0) ;               // Valeur
}

// Reinitialiser seuil haut rouge
void MainWindow::on_pushButton_seuilHautR_clicked(){
    ui->verticalSlider_seuilHautR_2->setRange(0, 255) ;         // Intervalle
    ui->verticalSlider_seuilHautR_2->setValue(255) ;            // Valeur
}

// Reinitialiser seuil haut vert
void MainWindow::on_pushButton_seuilHautV_clicked(){
    ui->verticalSlider_seuilHautV_2->setRange(0, 255) ;         // Intervalle
    ui->verticalSlider_seuilHautV_2->setValue(255) ;            // Valeur
}

// Reinitialiser seuil haut bleu
void MainWindow::on_pushButton_seuilHautB_clicked(){
    ui->verticalSlider_seuilHautB_2->setRange(0, 255) ;         // Intervalle
    ui->verticalSlider_seuilHautB_2->setValue(255) ;            // Valeur
}

// -------------Resolution-------------

// Initialiser
void MainWindow::on_groupBox_resolutionQuantification_clicked(){
    AffichageResultat(_imageOriginale, 1) ;                      // Image originale
    // Si le box est choisi
    if(ui->groupBox_resolutionQuantification->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleur
        ReinitialiserDetail() ;                 // Contours
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserAutre() ;                  // Autres
    // Sinon : Reinitialiser
    }else{
        ReinitialiserResolution() ;
    }
}

// Bouton : Resolution
void MainWindow::on_radioButton_resolution_clicked(){
    if(ui->radioButton_resolution->isChecked()){
        // Desactiver la quantification
        ui->radioButton_quantification->setChecked(false) ;
        ui->horizontalSlider_quantification->setValue(8) ;
        ui->horizontalSlider_quantification->setEnabled(false) ;

        // Initialiser la zone de reglage
        ui->horizontalSlider_resolution->setEnabled(true);
        ui->groupBox_interpolation->setEnabled(true) ;
        ui->horizontalSlider_resolution->setValue(0) ;
        ui->radioButton_PPP->setChecked(true) ;
        AffichageResultat(_imageOriginale, 1) ;
    // Sinon : Reinitialiser
    }else{
        ui->groupBox_interpolation->setEnabled(false) ;
        ui->horizontalSlider_resolution->setValue(0) ;
        ui->radioButton_PPP->setChecked(true) ;
        AffichageResultat(_imageOriginale, 1) ;
    }
}

// Bouton : Pixel le plus proche
void MainWindow::on_radioButton_PPP_clicked(){
    ui->radioButton_PPP->setChecked(true) ;
    if(ui->radioButton_PPP->isChecked()){
        ui->radioButton_bipolaire->setChecked(false) ;
        ui->horizontalSlider_resolution->setValue(0) ;
    }
}

// Bouton : Bilineaire
void MainWindow::on_radioButton_bipolaire_clicked(){
    ui->radioButton_bipolaire->setChecked(true) ;
    if(ui->radioButton_bipolaire->isChecked()){
        ui->radioButton_PPP->setChecked(false) ;
        ui->horizontalSlider_resolution->setValue(0) ;
    }
}

// Resolution
void MainWindow::on_horizontalSlider_resolution_valueChanged(int value){
    this->setCursor(Qt::WaitCursor);
    // Reduction de resolution
    if(value < 0){
        _imageResultat = ImageResolutionReduction(_imageOriginale, abs(value)) ;
    // Agrandissement
    }else if(value > 0){
        // Pixel le plus proche
        if(ui->radioButton_PPP->isChecked()){
            _imageResultat = InterpolationPPP(_imageOriginale, value) ;
        }else if(ui->radioButton_bipolaire->isChecked()){
            _imageResultat = InterpolationBilineaire(_imageOriginale, value) ;
        }
    }else{
        _imageResultat =_imageOriginale ;
    }

    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
    // Affichage de la nouvelle resolution
    string nbLigne = to_string((int)_imageResultat.size().height) ;
    string nbColonne = to_string((int)_imageResultat.size().width) ;
    string resolution = "Resolution : [" + nbLigne + " x " + nbColonne + "]" ;
    ui->label_resolutionTraitee->setText(QString::fromStdString(resolution)) ;
    this->setCursor(Qt::ArrowCursor);
}

// Bouton : Quantification
void MainWindow::on_radioButton_quantification_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_quantification->isChecked()){
        // Desactiver le redimensionnement
        ui->radioButton_resolution->setChecked(false) ;
        ui->radioButton_PPP->setChecked(false) ;
        ui->radioButton_bipolaire->setChecked(false) ;
        ui->horizontalSlider_resolution->setValue(0) ;
        ui->groupBox_interpolation->setEnabled(false) ;

        // Initialiser la zone de reglage
        ui->horizontalSlider_quantification->setEnabled(true) ;
        ui->horizontalSlider_quantification->setValue(8) ;
        AffichageResultat(_imageOriginale, 1) ;
    // Sinon : Reinitialiser
    }else{
        ui->horizontalSlider_quantification->setEnabled(false) ;
        ui->horizontalSlider_quantification->setValue(8) ;
        AffichageResultat(_imageOriginale, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// Quantification
void MainWindow::on_horizontalSlider_quantification_valueChanged(int value){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButton_quantification->isChecked()){
        _imageResultat = ImageQuantification(_imageOriginale, value) ;

        // Affichage du resultat
        AffichageResultat(_imageResultat, 1) ;
    }
    this->setCursor(Qt::ArrowCursor);
}

// -------------Couleur-------------

// Initialiser
void MainWindow::on_groupBox_couleur_clicked(){
    AffichageResultat(_imageOriginale, 1) ;     // Image originale
    // Si le box est choisi
    if(ui->groupBox_couleur->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserDetail() ;                 // Contours
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserFiltre() ;                 // Filtres
        ReinitialiserAutre() ;                  // Autres

    // Sinon : Reinitialiser
    }else{
        ReinitialiserCouleur() ;
    }
}

// Bouton : Temperature
void MainWindow::on_radioButton_temperature_clicked(){
    // Si la case est cochee
    if(ui->radioButton_temperature->isChecked()){
        // Reinitialiser les autres sliders
        // Teinte
        ui->radioButton_teinte->setChecked(false) ;
        ui->horizontalSlider_teinte->setValue(0) ;
        ui->horizontalSlider_teinte->setEnabled(false) ;
        // Saturation
        ui->radioButton_saturation->setChecked(false) ;
        ui->horizontalSlider_saturation->setValue(0) ;
        ui->horizontalSlider_saturation->setEnabled(false) ;
        // Vividite
        ui->radioButton_vividite->setChecked(false) ;
        ui->horizontalSlider_vividite->setValue(0) ;
        ui->horizontalSlider_vividite->setEnabled(false) ;

        // Activer le slider
        ui->horizontalSlider_temperature->setEnabled(true) ;
        ui->horizontalSlider_temperature->setValue(0) ;
    }else{
        ui->horizontalSlider_temperature->setEnabled(false) ;
        ui->horizontalSlider_temperature->setValue(0) ;
    }
}

// Temperature
void MainWindow::on_horizontalSlider_temperature_valueChanged(int value){
    // S'il n'y a aucune modification
    if(value ==0){
        _imageResultat = _imageOriginale ;
    // S'il y a des changements
    }else{
        _imageResultat = ImageTemperature(_imageOriginale, value) ;
    }
    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Bouton : Vividite
void MainWindow::on_radioButton_vividite_clicked(){
    // Si la case est cochee
    if(ui->radioButton_vividite->isChecked()){
        // Reinitialiser les autres sliders
        // Temperature
        ui->radioButton_temperature->setChecked(false) ;
        ui->horizontalSlider_temperature->setValue(0) ;
        ui->horizontalSlider_temperature->setEnabled(false) ;
        // Saturation
        ui->radioButton_saturation->setChecked(false) ;
        ui->horizontalSlider_saturation->setValue(0) ;
        ui->horizontalSlider_saturation->setEnabled(false) ;
        // Teinte
        ui->radioButton_teinte->setChecked(false) ;
        ui->horizontalSlider_teinte->setValue(0) ;
        ui->horizontalSlider_teinte->setEnabled(false) ;

        if(VerifierImage(_imageOriginale, _imageOriginale) == false){
            // Activer le slider
            ui->horizontalSlider_vividite->setEnabled(true) ;
            ui->horizontalSlider_vividite->setValue(0) ;
        }
    }else{
        ui->horizontalSlider_vividite->setEnabled(false) ;
        ui->horizontalSlider_vividite->setValue(0) ;
    }
}

// Vividite
void MainWindow::on_horizontalSlider_vividite_valueChanged(int value){
    // S'il n'y a aucune modification
    if(value == 0){
        _imageResultat = _imageOriginale ;
    // S'il y a des changements
    }else{
        _imageResultat = ImageVividite(_imageOriginale.clone(), value) ;
    }
    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Bouton : Teinte
void MainWindow::on_radioButton_teinte_clicked(){
    // Si la case est cochee
    if(ui->radioButton_teinte->isChecked()){
        // Reinitialiser les autres sliders
        // Temperature
        ui->radioButton_temperature->setChecked(false) ;
        ui->horizontalSlider_temperature->setValue(0) ;
        ui->horizontalSlider_temperature->setEnabled(false) ;
        // Saturation
        ui->radioButton_saturation->setChecked(false) ;
        ui->horizontalSlider_saturation->setValue(0) ;
        ui->horizontalSlider_saturation->setEnabled(false) ;
        // Vividite
        ui->radioButton_vividite->setChecked(false) ;
        ui->horizontalSlider_vividite->setValue(0) ;
        ui->horizontalSlider_vividite->setEnabled(false) ;

        // Activer le slider
        ui->horizontalSlider_teinte->setEnabled(true) ;
        ui->horizontalSlider_teinte->setValue(0) ;
    }else{
        ui->horizontalSlider_teinte->setEnabled(false) ;
        ui->horizontalSlider_teinte->setValue(0) ;
    }
}

// Teinte
void MainWindow::on_horizontalSlider_teinte_valueChanged(int value){
    // S'il n'y a aucune modification
    if(value != 0){
        _imageResultat = ImageTeinte(_imageOriginale, value) ;
    // S'il y a des changements
    }else{
        _imageResultat = _imageOriginale ;
    }
    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// Bouton : Saturation
void MainWindow::on_radioButton_saturation_clicked(){
    // Si la case est cochee
    if(ui->radioButton_saturation->isChecked()){
        // Reinitialiser les autres sliders
        // Teinte
        ui->radioButton_teinte->setChecked(false) ;
        ui->horizontalSlider_teinte->setValue(0) ;
        ui->horizontalSlider_teinte->setEnabled(false) ;
        // Temperature
        ui->radioButton_temperature->setChecked(false) ;
        ui->horizontalSlider_temperature->setValue(0) ;
        ui->horizontalSlider_temperature->setEnabled(false) ;
        // Vividite
        ui->radioButton_vividite->setChecked(false) ;
        ui->horizontalSlider_vividite->setValue(0) ;
        ui->horizontalSlider_vividite->setEnabled(false) ;

        if(VerifierImage(_imageOriginale, _imageOriginale) == false){
            // Activer le slider
            ui->horizontalSlider_saturation->setEnabled(true) ;
            ui->horizontalSlider_saturation->setValue(0) ;
        }
    }else{
        ui->horizontalSlider_saturation->setEnabled(false) ;
        ui->horizontalSlider_saturation->setValue(0) ;
    }
}

// Saturation
void MainWindow::on_horizontalSlider_saturation_valueChanged(int value){
    // S'il n'y a aucune modification
    if(value != 0){
        _imageResultat = ImageSaturation(_imageOriginale.clone(), value) ;
    // S'il y a des changements
    }else{
        _imageResultat = _imageOriginale ;
    }
    // Affichage du resultat
    AffichageResultat(_imageResultat, 1) ;
}

// -------------Autres-------------

// Initialiser
void MainWindow::on_groupBox_autre_clicked(){
    AffichageResultat(_imageOriginale, 1) ;     // Image originale
    // Si le box est choisi
    if(ui->groupBox_autre->isChecked()){
        // Desactiver les autres fonctionnalites
        ReinitialiserLuminosite() ;             // Luminosite
        ReinitialiserCouleur() ;                // Couleurs
        ReinitialiserDetail() ;                 // Contours
        ReinitialiserBruitageDebruitage() ;     // Bruitage et debruitage
        ReinitialiserResolution() ;             // Resolution
        ReinitialiserExtraction() ;             // Extraction
        ReinitialiserSeuillageSegmentation() ;  // Seuillage et segmentation
        ReinitialiserFiltre() ;                 // Filtres
    // Sinon : Reinitialiser
    }else{
        ReinitialiserAutre() ;
    }
}

// Transformee de Fourier
void MainWindow::on_radioButton_fourier_clicked(){
    if(ui->radioButton_fourier->isChecked()){
        // Desactiver les autres fonctionnalites

        ui->radioButtonTransformeeHough->setChecked(false) ;
        ui->spinBoxHough->setEnabled(false) ;
        ui->spinBoxHough->setValue(0) ;
        ui->spinBoxHough_seuil_theta->setEnabled(false) ;
        ui->spinBoxHough_seuil_theta->setValue(0) ;

        ui->radioButtonKmeans->setChecked(false) ;
        ui->spinBoxKmeans->setEnabled(false) ;
        ui->spinBoxKmeans->setValue(0) ;

        QMessageBox::information(0, "Transformée de Fourier", "Cette fonctionnalité va arriver bientôt dans la version suivante.") ;

        /*
        this->setCursor(Qt::WaitCursor);
        if(ui->radioButton_fourier->isChecked()){
            _imageResultat = ImageFourier(_imageOriginale) ;
            AffichageResultat(_imageResultat,1);
        }
        this->setCursor(Qt::ArrowCursor);*/
    }
}

// K-means
void MainWindow::on_radioButtonKmeans_clicked(){
    if(ui->radioButtonKmeans->isChecked()){
        QMessageBox::information(0, "Segmentation par K-means", "Cette fonctionnalité va arriver bientôt dans la version suivante.") ;
        // Desactiver les autres fonctionnalites
        ui->radioButtonTransformeeHough->setChecked(false) ;
        ui->spinBoxHough->setEnabled(false) ;
        ui->spinBoxHough->setValue(0) ;
        ui->spinBoxHough_seuil_theta->setEnabled(false) ;
        ui->spinBoxHough_seuil_theta->setValue(0) ;

        ui->radioButton_fourier->setChecked(false) ;
        // Initialiser le critere
        ui->spinBoxKmeans->setEnabled(true) ;
        ui->spinBoxKmeans->setValue(0) ;
    }
}

// Bouton : Transformee de Hough
void MainWindow::on_radioButtonTransformeeHough_clicked(){
    if(ui->radioButtonTransformeeHough->isChecked()){
        // Desactiver les autres fonctionnalites
        ui->radioButtonKmeans->setChecked(false) ;
        ui->spinBoxKmeans->setEnabled(false) ;
        ui->spinBoxKmeans->setValue(0) ;

        ui->radioButton_fourier->setChecked(false) ;

        // Initialiser le critere
        ui->spinBoxHough->setEnabled(true) ;
        ui->spinBoxHough_seuil_theta->setEnabled(true);
        ui->spinBoxHough->setValue(0) ;
        ui->spinBoxHough_seuil_theta->setValue(0) ;
    }else{
        ui->spinBoxHough->setEnabled(false) ;
        ui->spinBoxHough_seuil_theta->setEnabled(false);
        ui->spinBoxHough->setValue(0) ;
        ui->spinBoxHough_seuil_theta->setValue(0) ;
    }
}

// Transformee de Hough
void MainWindow::on_pushButton_Appliquer_Hough_clicked(){
    this->setCursor(Qt::WaitCursor);
    if(ui->radioButtonTransformeeHough->isChecked()){
        _imageResultat = TransformeedeHough(_imageOriginale, ui->spinBoxHough_seuil_theta->value(), ui->spinBoxHough->value()) ;
        AffichageResultat(_imageResultat,1);
    }
    this->setCursor(Qt::ArrowCursor);
}

// -------------Complements-------------

// Generer les icones
void MainWindow::GenererIcone(){
    // Preparer les icones pour l'espace de travail
    //string cheminExemple = "/home/vm/M2SIA-projet-QT/DATA/Images/rgbexample.jpeg" ;
    //Mat exemple = imread(cheminExemple) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleMono.png", ImageMonochrome(exemple)) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleNeg.png", ImageInversement(exemple)) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleR.png", ImageSegmentation(exemple,{255,255,0})) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleV.png", ImageSegmentation(exemple,{255,0,255})) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleB.png", ImageSegmentation(exemple,{0,255,255})) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleJ.png", ImageJaune(exemple)) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleC.png", ImageCyan(exemple)) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleM.png", ImageMagenta(exemple)) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleSepia.png", ImageSepia(exemple)) ;
    //imwrite("/home/vm/M2SIA-projet-QT/DATA/icon/exempleRGB.png", ImageRGB(exemple)) ;

    // Dimensions des fenetre exemplaires
    int hauteur = ui->label_mono->height() ;    // Hauteur de la fenetre d'affichage
    int largeur = ui->label_mono->width() ;     // Largeur de la fenetre d'affichage
    // Originale
    QPixmap exempleOriginale(":/icons/galaxy.jpeg") ;
    ui->label_original->setPixmap(exempleOriginale.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_original->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Mono
    QPixmap exempleMono(":/icons/exempleMono.jpeg") ;
    ui->label_mono->setPixmap(exempleMono.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_mono->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Sepia
    QPixmap exempleS(":/icons/exempleSepia.jpeg") ;
    ui->label_sepia->setPixmap(exempleS.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_sepia->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Inversement
    QPixmap exempleNeg(":/icons/exempleNeg.jpeg") ;
    ui->label_negatif->setPixmap(exempleNeg.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_negatif->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Rouge
    QPixmap exempleR(":/icons/exempleR.jpeg") ;
    ui->label_rouge->setPixmap(exempleR.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_rouge->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Vert
    QPixmap exempleV(":/icons/exempleV.jpeg") ;
    ui->label_vert->setPixmap(exempleV.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_vert->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Bleu
    QPixmap exempleB(":/icons/exempleB.jpeg") ;
    ui->label_bleu->setPixmap(exempleB.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_bleu->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Jaune
    QPixmap exempleJ(":/icons/exempleJ.jpeg") ;
    ui->label_jaune->setPixmap(exempleJ.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_jaune->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Cyan
    QPixmap exempleC(":/icons/exempleC.jpeg") ;
    ui->label_cyan->setPixmap(exempleC.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_cyan->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Magenta
    QPixmap exempleM(":/icons/exempleM.jpeg") ;
    ui->label_magenta->setPixmap(exempleM.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_magenta->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // RGB
    QPixmap exempleRGB(":/icons/exempleRGB.jpeg") ;
    ui->label_rgb->setPixmap(exempleRGB.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->label_rgb->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Bruit poivre et sel
    QPixmap exemplePeS(":/icons/exemplePeS.jpeg") ;
    ui->labelBruitPoivreSel->setPixmap(exemplePeS.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->labelBruitPoivreSel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Egalisation
    QPixmap exempleEgalisation(":/icons/exempleEgalisation.jpeg") ;
    ui->labelEgalisation->setPixmap(exempleEgalisation.scaled(largeur, hauteur, Qt::KeepAspectRatio)) ;
    ui->labelEgalisation->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    // Icones zoom
    int hauteurZoom = ui->label_Zoom->height() ;    // Hauteur de la fenetre d'affichage
    int largeurZoom = ui->label_Zoom->width() ;     // Largeur de la fenetre d'affichage
    QPixmap zoom(":/icons/zoom.png") ;
    ui->label_Zoom->setPixmap(zoom.scaled(largeurZoom, hauteurZoom, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_Zoom->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    ui->label_ZoomNonModifiable->setPixmap(zoom.scaled(largeurZoom, hauteurZoom, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_ZoomNonModifiable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    ui->label_zoomImageOriginale->setPixmap(zoom.scaled(largeurZoom, hauteurZoom, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_zoomImageOriginale->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
    ui->label_zoomImageTraitee->setPixmap(zoom.scaled(largeurZoom, hauteurZoom, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_zoomImageTraitee->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
}

// Afficher l'image resultante apres un traitement avec inversement les canaux de couleur
void MainWindow::AffichageResultat(const Mat image, const int choix){
    // Declaration des variables
    Mat imageAffichage = image ;

    // Histogramme de l'image resultante
    _histoImageResultat = Normalisation(PlotHistogram(imageAffichage), 255) ;

    // Echanger les composantes suivant la norme du QT
    if(choix != 0){
        imageAffichage = ImageBGRRGB(imageAffichage) ;
    }
    // Convertir en format QT
    QImage imageResultatQT = QImage((uchar*) imageAffichage.data, imageAffichage.cols, imageAffichage.rows, imageAffichage.step, QImage::Format_RGB888) ;
    QImage histoImageResultatQT = QImage((uchar*) _histoImageResultat.data, _histoImageResultat.cols, _histoImageResultat.rows, _histoImageResultat.step, QImage::Format_RGB888) ;

    // Affichage : Image resultante
    QGraphicsScene* scene = new QGraphicsScene(this) ;
    scene->addPixmap(QPixmap::fromImage(imageResultatQT)) ;
    ui->graphicsView_imageTraitee->setScene(scene) ;
    ui->graphicsView_imageTraitee->show() ;
    ui->graphicsView_imageTraitee->fitInView(scene->sceneRect(), Qt::KeepAspectRatio) ;
    // Affichage : Histogramme
    QPixmap histogrammeTraitee(QPixmap::fromImage(histoImageResultatQT)) ;
    int hauteur = ui->label_histogrammeImageTraitee->height() ;       // Hauteur de la fenetre d'affichage
    int largeur = ui->label_histogrammeImageTraitee->width() ;        // Largeur de la fenetre d'affichage
    ui->label_histogrammeImageTraitee->setPixmap(histogrammeTraitee.scaled(largeur, hauteur, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) ;
    ui->label_histogrammeImageTraitee->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter) ;
}

// Messages d'aide
void MainWindow::AfficherMessageAide(){
    // Box : Luminosite
    AfficherMessageAideLuminosite() ;
    // Box : Couleur
    AfficherMessageAideCouleur() ;
    // Box : Resolution
    AfficherMessageAideResolution() ;
    // Box : Details
    AfficherMessageAideDetail() ;
    // Box : Extraction
    AfficherMessageAideExtraction() ;
    // Box : Contours
    AfficherMessageAideContour() ;
    // Box : Debruitage
    AfficherMessageAideDebruitage() ;
    // Box : Seuillage et segmentation
    AfficherMessageAideSeuillageSegmentation() ;
    // Box : Filtres
    AfficherMessageAideFiltre() ;

    // Zone d'affichage
    // Image de départ
    ui->groupBox_imageOriginale->setToolTip("Image de départ :\nContenu, résolution, histogramme") ;
    ui->graphicsView_imageOriginale->setToolTip("Image de départ") ;
    ui->label_resolutionOriginale->setToolTip("Résolution de l'image de départ") ;
    ui->label_zoomImageOriginale->setToolTip("Agrandissement de l'image de départ") ;
    ui->horizontalSlider_imageOriginale->setToolTip("Tirer vers la droite pour agrandir l'image") ;
    ui->label_histogrammeImageOriginale->setToolTip("Histogramme de l'image de départ") ;

    // Image traitée
    ui->groupBox_imageTraitee->setToolTip("Image de départ :\nContenu, résolution, histogramme") ;
    ui->graphicsView_imageTraitee->setToolTip("Image traitée") ;
    ui->label_resolutionTraitee->setToolTip("Résolution de l'image traitée") ;
    ui->label_zoomImageTraitee->setToolTip("Agrandissement de l'image traitée") ;
    ui->horizontalSlider_imageTraitee->setToolTip("Tirer vers la droite pour agrandir l'image") ;
    ui->label_histogrammeImageTraitee->setToolTip("Histogramme de l'image traitée") ;

    // Boutons
    ui->pushButton_retour->setToolTip("Retourner à la page précédente (choisir l'image à traiter)") ;
    ui->pushButton_traitementAppliquer->setToolTip("Appliquer le traitement actuel :\nL'image de départ sera remplacé par l'image traitée en cours d'affichage") ;
    ui->pushButton_traitementReinitialiser->setToolTip("Réinitialiser l'image de départ par l'image choisie dans la page précédente") ;
    ui->pushButton_traitementSauvegarder->setToolTip("Sauvegarder l'image traitée") ;
    // Transformée de Hough
    ui->spinBoxHough->setToolTip("Seuille de détection des droites. Plus c'est élevé plus, nous eliminons les droites redondantes ou les faux-positives.");
    ui->spinBoxHough_seuil_theta->setToolTip("Nombre de projection que theta peut faire dans l'intervalle [0;180]. \n A 1: Effectué  une seul projection pour un angle de 0. \n A 180 : une projection effectué tous les 1 degrés");
}

// Messages d'aide : Luminosite
void MainWindow::AfficherMessageAideLuminosite(){
    ui->groupBox_correction->setToolTip("Modifier les paramètres liés à la luminosité de l'image :\nIntensité, contraste, ombres hautes lumières") ;
    // Intensité
    ui->radioButton_luminosite->setToolTip("Modifier l'intensité lumineuse de l'image") ;
    ui->horizontalSlider_luminosite->setToolTip("Tirer à gauche : Atténuer l'intensité lumineuse\nTirer à droite : Augmenter l'intensité lumineuse") ;
    // Contraste
    ui->radioButton_contraste->setToolTip("Modifier le contraste de l'image") ;
    ui->horizontalSlider_contraste->setToolTip("Tirer à gauche : Atténuer le contraste\nTirer à droite : Augmenter le contraste") ;
    // Ombres
    ui->radioButton_ombre->setToolTip("Modifier les ombres (shadows) de l'image") ;
    ui->horizontalSlider_ombre->setToolTip("Tirer à gauche : Atténuer les ombres\nTirer à droite : Augmenter les ombres") ;
    // Hautes lumières
    ui->radioButton_brillance->setToolTip("Modifier les hautes lumières (highlights) de l'image") ;
    ui->horizontalSlider_brillance->setToolTip("Tirer à gauche : Atténuer les hautes lumières\nTirer à droite : Augmenter les hautes lumières") ;
}

// Messages d'aide : Couleur
void MainWindow::AfficherMessageAideCouleur(){
    ui->groupBox_couleur->setToolTip("Modifier les paramètres liés aux couleurs de l'image :\nTempérature, vividité, teinte, saturation") ;
    // Température
    ui->radioButton_temperature->setToolTip("Modifier la température de l'image") ;
    ui->horizontalSlider_temperature->setToolTip("Tirer à gauche : Couleur froide\nTirer à droite : Couleur chaude") ;
    // Vividité
    ui->radioButton_vividite->setToolTip("Modifier la vividité de l'image") ;
    ui->horizontalSlider_vividite->setToolTip("Tirer à gauche : Atténuer la vividité\nTirer à droite : Augmenter la vividité") ;
    // Teinte
    ui->radioButton_teinte->setToolTip("Modifier la teinte de l'image") ;
    // Saturation
    ui->radioButton_saturation->setToolTip("Modifier la saturation de l'image") ;
    ui->horizontalSlider_saturation->setToolTip("Tirer à gauche : Atténuer la saturation\nTirer à droite : Augmenter la saturation") ;
}

// Messages d'aide : Details
void MainWindow::AfficherMessageAideDetail(){
    ui->groupBox_details->setToolTip("Modifier la nettete de l'image\nBruiter l'image (bruit uniforme)") ;
    // Nettete
    ui->radioButton_nettete->setToolTip("Modifier la netteté de l'image") ;
    ui->horizontalSlider_nettete->setToolTip("Tirer à gauche : Lisser l'image\nTirer à droite : Réhausser les contours dans l'image") ;
    // Bruitage
    ui->radioButton_bruitage->setToolTip("Ajouter du bruit additif (bruit uniforme) à l'image") ;
    ui->horizontalSlider_Bruitage->setToolTip("Tirer à droite : Augmenter l'intensité du bruit") ;
}

// Messages d'aide : Resolution
void MainWindow::AfficherMessageAideResolution(){
    ui->groupBox_resolutionQuantification->setToolTip("Modifier la resolution & Quantification") ;
    // Resolution
    ui->radioButton_resolution->setToolTip("Redimensionner l'image") ;
    ui->radioButton_PPP->setToolTip("Interpolation par pixel le plus proche") ;
    ui->radioButton_bipolaire->setToolTip("Interpolation bilinéaire") ;
    ui->horizontalSlider_resolution->setToolTip("Tirer à gauche : Dininuer a résolution\nTirer à droite : Augmenter la résolution (interpolation)") ;
    // Quantification
    ui->radioButton_quantification->setToolTip("Quantification :\nMax : 8 bits\nMin : 1 bit") ;
    ui->horizontalSlider_quantification->setToolTip("Tirer à gauche : Atténuer le nombre de nuances") ;
}

// Messages d'aide : Extraction
void MainWindow::AfficherMessageAideExtraction(){
    ui->groupBox_extractionRVB->setToolTip("Extraire une composante de couleur de l'image (rouge, verte, bleue)") ;
    // Canal rouge
    ui->radioButton_extractionR->setToolTip("Extraction de la composante rouge de l'image") ;
    // Canal vert
    ui->radioButton_extractionV->setToolTip("Extraction de la composante verte de l'image") ;
    // Canal bleu
    ui->radioButton_extractionB->setToolTip("Extraction de la composante bleue de l'image") ;
}

// Messages d'aide : Contours
void MainWindow::AfficherMessageAideContour(){
    ui->groupBox_contours->setToolTip("Détecter les contours de l'image par différents méthodes :\nFiltres gradients, filtre laplacien, transformée de Hough") ;
    // Filtrage de type gradient
    ui->radioButton_contoursGradient->setToolTip("Détection de contours par la norme du gradient") ;
    // Filtrage de type laplacien
    ui->radioButton_contoursLaplacien->setToolTip("Détection de contours par filtre laplacien") ;
}

// Messages d'aide : Debruitage
void MainWindow::AfficherMessageAideDebruitage(){
    ui->groupBox_debruitage->setToolTip("Débruiter l'image par différents types de filtre :\nMoyenneur, gaussien (filtres linéaires)\nMédian, Kuwahara-Nagao (non-linéaire)") ;
    // Filtres lineaires
    ui->radioButton_moyenneur->setToolTip("Filtre moyenneur (linéaire)") ;
    ui->radioButton_gaussien->setToolTip("Filtre gaussien (linéaire)") ;
    // Filtres non lineaires
    ui->radioButton_median->setToolTip("Filtre médian (non linéaire)") ;
    ui->radioButton_kuwahara->setToolTip("Filtre de Kuwahara-Nagao (non linéaire)") ;
}

// Messages d'aide : Seuillage et segmentation
void MainWindow::AfficherMessageAideSeuillageSegmentation(){
    ui->groupBox_seuillageSegmentation->setToolTip("Seuiller ou segmenter l'image") ;
    ui->radioButton_seuillage->setToolTip("Seuillage simple ou à hystérésis") ;
    ui->radioButton_segmentation->setToolTip("Segmentation par seuillage simple ou à hystérésis") ;
    ui->radioButton_seuillageSimple->setToolTip("Seuillage simple (conserver les valeurs supérieures au seuil)") ;
    ui->radioButton_seuillageHysteresis->setToolTip("Seuillage à hystérésis (conserver les valeurs supérieures au seuil bas et inférieures au seuil haut)") ;
    // Seuils bas
    ui->groupBox_seuilBas->setToolTip("Seuils (rouge, vert, bleu) du seuillage simple\nSeuils (rouge, vert, bleu) bas du seuillage à hystérésis\n"
                                      "Pour une image en niveau de gris, la valeur du seuil est la même pour les trois composantes") ;
    ui->verticalSlider_seuilBasR_2->setToolTip("Seuil de la composante rouge") ;
    ui->verticalSlider_seuilBasV_2->setToolTip("Seuil de la composante verte") ;
    ui->verticalSlider_seuilBasB_2->setToolTip("Seuil de la composante bleue") ;
    ui->pushButton_seuilBasR->setToolTip("Réinitialiser le seuil de la composante rouge à 0") ;
    ui->pushButton_seuilBasV->setToolTip("Réinitialiser le seuil de la composante verte à 0") ;
    ui->pushButton_seuilBasB->setToolTip("Réinitialiser le seuil de la composante bleue à 0") ;
    // Seuils hauts
    ui->groupBox_seuilHaut->setToolTip("Seuils hauts(rouge, vert, bleu) du seuillage à hystérésis\nPour une image en niveau de gris, la valeur du seuil est la même pour les trois composantes") ;
    ui->verticalSlider_seuilHautR_2->setToolTip("Seuil de la composante rouge") ;
    ui->verticalSlider_seuilHautV_2->setToolTip("Seuil de la composante verte") ;
    ui->verticalSlider_seuilHautB_2->setToolTip("Seuil de la composante bleue") ;
    ui->pushButton_seuilHautR->setToolTip("Réinitialiser le seuil de la composante rouge à 256") ;
    ui->pushButton_seuilHautV->setToolTip("Réinitialiser le seuil de la composante verte à 256") ;
    ui->pushButton_seuilHautB->setToolTip("Réinitialiser le seuil de la composante bleue à 256") ;
}

// Messages d'aide : Filtres
void MainWindow::AfficherMessageAideFiltre(){
    ui->groupBox_filtres->setToolTip("Appliquer les filtres de couleurs") ;
    // Niveau de gris
    ui->radioButton_niveauGris->setToolTip("Niveau de gris") ;
    ui->label_mono->setToolTip("Niveau de gris") ;
    // Inversement
    ui->radioButton_inversement->setToolTip("Inversement des intensités lumineuses") ;
    ui->label_negatif->setToolTip("Inversement des intensités lumineuses") ;
    // Sepia
    ui->radioButton_sepia->setToolTip("Sepia") ;
    ui->label_sepia->setToolTip("Sepia") ;
    // Rouge
    ui->radioButton_rouge->setToolTip("Rouge") ;
    ui->label_rouge->setToolTip("Rouge") ;
    // Vert
    ui->radioButton_vert->setToolTip("Vert") ;
    ui->label_vert->setToolTip("Vert") ;
    // Bleu
    ui->radioButton_bleu->setToolTip("Bleu") ;
    ui->label_bleu->setToolTip("Bleu") ;
    // RGB
    ui->radioButton_rgb->setToolTip("Couleur extrême RGB") ;
    ui->label_rgb->setToolTip("Couleur extrême RGB") ;
    // Cyan
    ui->radioButton_cyan->setToolTip("Cyan") ;
    ui->label_cyan->setToolTip("Cyan") ;
    // Magenta
    ui->radioButton_magenta->setToolTip("Magenta") ;
    ui->label_magenta->setToolTip("Magenta") ;
    // Jaune
    ui->radioButton_jaune->setToolTip("Jaune") ;
    ui->label_jaune->setToolTip("Jaune") ;
}

// Reinitialisation generale
void MainWindow::Reinitialiser(){
    ReinitialiserLuminosite() ;             // Luminosite
    ReinitialiserCouleur() ;                // Couleurs
    ReinitialiserResolution() ;             // Resolution
    ReinitialiserDetail() ;                 // COntours
    ReinitialiserExtraction() ;             // Extraction
    ReinitialiserBruitageDebruitage();      // Bruitage et debruitage
    ReinitialiserSeuillageSegmentation() ;  // Segmentation et seuillage
    ReinitialiserFiltre() ;                 // Effets
    ReinitialiserAutre() ;                  // Autres
    AffichageResultat(_imageOriginale, 1) ;
}

// Reinitialiser : Box Luminosite
void MainWindow::ReinitialiserLuminosite(){
    ui->radioButton_luminosite->setChecked(false) ;
    ui->horizontalSlider_luminosite->setValue(0) ;
    ui->horizontalSlider_luminosite->setEnabled(false) ;

    ui->radioButton_contraste->setChecked(false) ;
    ui->horizontalSlider_contraste->setValue(0) ;
    ui->horizontalSlider_contraste->setEnabled(false) ;

    ui->radioButton_brillance->setChecked(false) ;
    ui->horizontalSlider_brillance->setValue(0) ;
    ui->horizontalSlider_brillance->setEnabled(false) ;

    ui->radioButton_ombre->setChecked(false) ;
    ui->horizontalSlider_ombre->setValue(0) ;
    ui->horizontalSlider_ombre->setEnabled(false) ;

    ui->groupBox_correction->setChecked(false) ;
}

// Reinitialiser : Box Couleur
void MainWindow::ReinitialiserCouleur(){
    ui->horizontalSlider_temperature->setValue(0) ;
    ui->horizontalSlider_temperature->setEnabled(false) ;
    ui->radioButton_temperature->setChecked(false) ;

    ui->horizontalSlider_vividite->setValue(0) ;
    ui->horizontalSlider_vividite->setEnabled(false) ;
    ui->radioButton_vividite->setChecked(false) ;

    ui->horizontalSlider_teinte->setValue(0) ;
    ui->horizontalSlider_teinte->setEnabled(false) ;
    ui->radioButton_teinte->setChecked(false) ;

    ui->horizontalSlider_saturation->setValue(0) ;
    ui->horizontalSlider_saturation->setEnabled(false) ;
    ui->radioButton_saturation->setChecked(false) ;

    ui->groupBox_couleur->setChecked(false) ;
}

// Reinitialiser : Box Resolution
void MainWindow::ReinitialiserResolution(){
    ui->radioButton_resolution->setChecked(false) ;
    ui->radioButton_PPP->setChecked(false) ;
    ui->radioButton_bipolaire->setChecked(false) ;
    ui->radioButton_quantification->setChecked(false) ;
    ui->horizontalSlider_resolution->setValue(0) ;
    ui->horizontalSlider_resolution->setEnabled(false) ;
    ui->horizontalSlider_quantification->setValue(8) ;
    ui->horizontalSlider_quantification->setEnabled(false) ;
    ui->groupBox_resolutionQuantification->setChecked(false) ;
}

// Reinitialiser : Box Contours
void MainWindow::ReinitialiserDetail(){
    ui->radioButton_nettete->setChecked(false) ;
    ui->horizontalSlider_nettete->setValue(0) ;
    ui->groupBox_details->setChecked(false) ;
    ui->groupBox_contours->setChecked(false) ;
    ReinitialiserContour() ;
}

// Reinitialiser : Box Bruitage et debruitage
void MainWindow::ReinitialiserBruitageDebruitage(){
    ui->radioButton_bruitage->setChecked(false) ;
    ui->horizontalSlider_Bruitage->setValue(0) ;
    ui->groupBox_bruitageDebruitage->setChecked(false) ;
    ui->groupBox_debruitage->setChecked(false) ;
    ReinitialiserDebruitage() ;
}

// Reinitialiser : Box Extraction
void MainWindow::ReinitialiserExtraction(){
    ui->radioButton_extractionR->setChecked(false) ;
    ui->radioButton_extractionV->setChecked(false) ;
    ui->radioButton_extractionB->setChecked(false) ;
    ui->groupBox_extractionRVB->setChecked(false) ;
}

// Reinitialiser : Box Contours
void MainWindow::ReinitialiserContour(){
    ui->radioButton_contoursGradient->setChecked(false) ;
    ui->radioButton_contoursLaplacien->setChecked(false) ;
    ui->groupBox_contours->setChecked(false) ;
}

// Reinitialiser : Box Debruitage
void MainWindow::ReinitialiserDebruitage(){
    ui->radioButton_moyenneur->setChecked(false) ;
    ui->radioButton_moyenneur->setEnabled(false) ;
    ui->radioButton_gaussien->setChecked(false) ;
    ui->radioButton_gaussien->setEnabled(false) ;
    ui->radioButton_median->setChecked(false) ;
    ui->radioButton_median->setEnabled(false) ;
    ui->radioButton_kuwahara->setChecked(false) ;
    ui->radioButton_kuwahara->setEnabled(false) ;
    ui->groupBox_debruitage->setChecked(false) ;
}

// Reinitialiser : Box Seuillage et segmentation
void MainWindow::ReinitialiserSeuillageSegmentation(){
    ui->radioButton_seuillage->setChecked(false) ;
    ui->radioButton_seuillageSimple->setChecked(false) ;
    ui->radioButton_segmentation->setChecked(false) ;
    ui->radioButton_seuillageHysteresis->setChecked(false) ;

    ui->verticalSlider_seuilBasB_2->setValue(0) ;
    ui->verticalSlider_seuilBasB_2->setEnabled(false) ;
    ui->verticalSlider_seuilBasR_2->setValue(0) ;
    ui->verticalSlider_seuilBasR_2->setEnabled(false) ;
    ui->verticalSlider_seuilBasV_2->setValue(0) ;
    ui->verticalSlider_seuilBasV_2->setEnabled(false) ;

    ui->verticalSlider_seuilHautB_2->setValue(256) ;
    ui->verticalSlider_seuilHautB_2->setEnabled(false) ;
    ui->verticalSlider_seuilHautR_2->setValue(256) ;
    ui->verticalSlider_seuilHautR_2->setEnabled(false) ;
    ui->verticalSlider_seuilHautV_2->setValue(256) ;
    ui->verticalSlider_seuilHautV_2->setEnabled(false) ;

    ui->groupBox_seuillageSegmentation->setChecked(false) ;
}

// Reinitialiser : Box Filtres
void MainWindow::ReinitialiserFiltre(){
    ui->radioButton_originale->setChecked(false) ;
    ui->radioButton_niveauGris->setChecked(false) ;
    ui->radioButton_sepia->setChecked(false) ;
    ui->radioButton_inversement->setChecked(false) ;
    ui->radioButton_rouge->setChecked(false) ;
    ui->radioButton_vert->setChecked(false) ;
    ui->radioButton_bleu->setChecked(false) ;
    ui->radioButton_rgb->setChecked(false) ;
    ui->radioButton_cyan->setChecked(false) ;
    ui->radioButton_magenta->setChecked(false) ;
    ui->radioButton_jaune->setChecked(false) ;
    ui->radioButtonEgalisation->setChecked(false) ;
    ui->radioButtonBruitPoivreSel->setChecked(false) ;
    ui->groupBox_filtres->setChecked(false) ;
}

// Reinitialiser : Box Autres
void MainWindow::ReinitialiserAutre(){
   ui->radioButtonTransformeeHough->setChecked(false) ;
   ui->radioButton_fourier->setChecked(false) ;

   ui->radioButtonKmeans->setChecked(false) ;
   ui->spinBoxKmeans->setValue(0) ;
   ui->spinBoxKmeans->setEnabled(false) ;

   ui->spinBoxHough->setValue(0) ;
   ui->spinBoxHough_seuil_theta->setValue(0) ;
   ui->spinBoxHough->setEnabled(false) ;
   ui->spinBoxHough_seuil_theta->setEnabled(false) ;

   ui->groupBox_autre->setChecked(false) ;
}


void MainWindow::on_pushButtonRetourMenuConsultationImage_clicked(){
    MainWindow::on_pushButtonRetourMenuModificationImage_clicked() ;
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}




