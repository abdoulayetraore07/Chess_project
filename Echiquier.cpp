#include "Echiquier.hpp"
#include <iostream>
#include <fcntl.h>
#include <cmath>

using namespace std;


//FAUDRA TRAITER LE CAS D'EGALITÉ AUX JEUX D'ECHECS ( MATCH NUL )


/*
 #MODULE Echiquier.cpp contenant les implémentations des méthodes de la classe Plateau
 
 Méthodes :
 
    Plateau::Plateau(int type) : Constructeur de la classe Plateau
        - Initialise la taille du plateau en fonction du type de jeu (Tic-Tac-Toe ou Échecs)
        - Initialise la grille avec des pièces vides
        - Initialise le joueur actuel (0 pour X, 1 pour O)
        - Si le type de jeu est Échecs, initialise les pièces sur l'échiquier

    void Plateau::initialiserEchecs() : Initialise les pièces sur l'échiquier
        - Place les pions noirs et blancs sur leurs positions initiales
        - Place les pièces majeures (tours, cavaliers, fous, reine, roi) sur leurs positions initiales

    void Plateau::afficher() const : Affiche la grille du jeu
        - Affiche les indices de colonnes et les lignes de séparation
        - Affiche les pièces sur la grille en utilisant leurs représentations Unicode

    bool Plateau::cheminLibre(int x1, int y1, int x2, int y2) : Vérifie si le chemin entre deux positions est libre
        - Calcule la direction du déplacement
        - Parcourt le chemin case par case pour vérifier s'il est libre

    bool Plateau::mouvementPionValide(int x1, int y1, int x2, int y2) : Vérifie si un mouvement de pion est valide
        - Vérifie les déplacements simples, les premiers déplacements de deux cases et les captures en diagonale

    bool Plateau::mouvementValide(int x1, int y1, int x2, int y2) : Vérifie si un mouvement est valide pour une pièce donnée
        - Vérifie si la destination est occupée par une pièce alliée
        - Applique les règles de déplacement spécifiques à chaque type de pièce (roi, reine, tour, fou, cavalier, pion)

    bool Plateau::jouerCoup(int x, int y) : Joue un coup dans le jeu de Tic-Tac-Toe
        - Vérifie si la case est valide et vide
        - Place un "X" ou un "O" selon le joueur actuel
        - Change de joueur après le coup

    bool Plateau::jouerCoup(int x1, int y1, int x2, int y2) : Joue un coup dans le jeu d'échecs
        - Vérifie si les positions sont valides
        - Vérifie si la case d'origine contient une pièce
        - Vérifie si le mouvement est valide selon les règles d'échecs
        - Déplace la pièce et change de joueur après le coup

    bool Plateau::estPlein() const : Vérifie si la grille est pleine (Tic-Tac-Toe)
        - Parcourt la grille pour vérifier si toutes les cases sont occupées

    char Plateau::verifierGagnant() const : Vérifie s'il y a un gagnant dans le jeu de Tic-Tac-Toe
        - Vérifie les lignes, colonnes et diagonales pour détecter un alignement de trois symboles identiques

    int Plateau::getJoueurActuel() const : Retourne le joueur actuel
        - Retourne 0 pour X, 1 pour O

    void Plateau::reset() : Réinitialise la grille
        - Réinitialise la grille avec des pièces vides
        - Si le type de jeu est Échecs, réinitialise les pièces sur l'échiquier

    vector<piece> Plateau::getPieces() const : Retourne un vecteur des pièces disponibles sur la grille
        - Parcourt la grille pour collecter toutes les pièces non vides

    vector<vector<piece>> Plateau::get_grille() : Retourne la grille actuelle
        - Retourne la grille représentant l'état actuel du jeu
*/




// Constructeur de la classe Plateau
Plateau::Plateau(int type) : dernierCoup(-10, -10)  {
    
    if (type == 0)
        taillePlateau = 3; // Tic-Tac-Toe
    else
        taillePlateau = 8; // Échecs

    // Initialisation de la grille avec des pièces vides (null)
    grille = vector<vector<piece>>(taillePlateau, vector<piece>(taillePlateau, piece()));
    joueurActuel = 0; // X ou Blanc commence toujours
    typePlateau = (type==1) ? TypeJeu::ECHECS : TypeJeu::TIC_TAC_TOE;  // Assignez le type du plateau à la variable crée à cet effet dans la classe plateau
   

    // Initialiser les pièces si c'est un échiquier
    if (typePlateau == TypeJeu::ECHECS) {
        openings = Openings(); // Initialiser les ouvertures
        initialiserEchecs();
    }
}


char Plateau::getSymboleH(char symboleIA) const {
    char symboleH = 'O';
    if (getTypePlateau() == TypeJeu::TIC_TAC_TOE) {
        symboleH = (symboleIA=='X') ? 'O' : 'X';
    }
    else {
        symboleH = (symboleIA=='N') ? 'B' : 'N';
    }
    return symboleH ;
}


// Initialise les pièces sur l'échiquier
void Plateau::initialiserEchecs() {
    // Placer les pièces de l'échiquier
    for (int i = 0; i < 8; i++) {
        grille[1][i] = piece(type_pion::PION, false, 1, i); // Pions noirs
        grille[6][i] = piece(type_pion::PION, true, 6, i); // Pions blancs
    }
    
    // Simple placement des pièces sur l'echiquier selon les regles du jeu
    // Pièces majeures
    grille[0][0] = grille[0][7] = piece(type_pion::TOUR, false, 0, 0);
    grille[0][1] = grille[0][6] = piece(type_pion::CAVALIER, false, 0, 1);
    grille[0][2] = grille[0][5] = piece(type_pion::FOU, false, 0, 2);
    grille[0][3] = piece(type_pion::REINE, false, 0, 3);
    grille[0][4] = piece(type_pion::ROI, false, 0, 4);

    grille[7][0] = grille[7][7] = piece(type_pion::TOUR, true, 7, 0);
    grille[7][1] = grille[7][6] = piece(type_pion::CAVALIER,  true, 7, 1);
    grille[7][2] = grille[7][5] = piece(type_pion::FOU,  true, 7, 2);
    grille[7][3] = piece(type_pion::REINE,  true, 7, 3);
    grille[7][4] = piece(type_pion::ROI,  true, 7, 4);
}


void Plateau::vidergrille() {
    for (int i = 0; i < 8; i++) {
        grille[1][i] = piece(); // Pions noirs
        grille[6][i] = piece(); // Pions blancs
    }
    
    // Simple placement des pièces sur l'echiquier selon les regles du jeu
    // Pièces majeures
    grille[0][0] = grille[0][7] = piece();
    grille[0][1] = grille[0][6] = piece();
    grille[0][2] = grille[0][5] = piece();
    grille[0][3] = piece();
    grille[0][4] = piece();

    grille[7][0] = grille[7][7] = piece();
    grille[7][1] = grille[7][6] = piece();
    grille[7][2] = grille[7][5] = piece();
    grille[7][3] = piece();
    grille[7][4] = piece();
    
    grille[0][0] = piece();
    grille[7][7] = piece();
    grille[6][6] = piece();
}

// Affiche la grille du jeu
void Plateau::afficher() const {
    // Afficher les indices de colonnes
    wcout << L"  ";
    for (int i = 1; i <= taillePlateau; i++)
        wcout << L"   " << i << "  ";
    wcout << L"\n";

    // Ligne de séparation horizontale
    
    wstring ligne = L"  ╔";
    for (int j = 0; j < taillePlateau; j++) {
        ligne += L"═════";
        if (j < taillePlateau - 1)
            ligne += L"╦";
    }
    ligne += L"╗";
    wcout << ligne << L"\n";        // Chaîne pour afficher une ligne permettant d'espacer les cases horizontalement

    // Affichage de la grille avec les pièces
    for (int i = 0; i < taillePlateau; i++) {  // Parcours de chaque ligne de la grille
        wcout << wchar_t('A' + i) << L" ║"; // Affichage des lettres designant les lignes ( A : 1ère ligne, B : 2ème ligne, ... ) avec le caractere wchar_t
        
        for (int j = 0; j < taillePlateau; j++) {    // Parcours des colonnes de la ligne courante ppur afficher les pièces
            wcout << L"  " << grille[i][j].getUnicode() << L"  ║"; // Pièce et séparateur
        }
        wcout << L"\n";

        // Ligne de séparation horizontale entre les lignes
        if (i < taillePlateau - 1) {
            wcout << L"  ╠";
            for (int j = 0; j < taillePlateau; j++) {
                wcout << L"═════";
                if (j < taillePlateau - 1)
                    wcout << L"╬";
            }
            wcout << L"╣\n";
        }
    }

    // Ligne de séparation horizontale finale
    wcout << L"  ╚";
    for (int j = 0; j < taillePlateau; j++) {
        wcout << L"═════";
        if (j < taillePlateau - 1)
            wcout << L"╩";
    }
    wcout << L"╝\n";
}

// Vérifie si le chemin entre deux positions est libre
bool Plateau::cheminLibre(int x1, int y1, int x2, int y2) const {
    // Déterminer la direction du mouvement le long de l'axe x (dx) et de l'axe y (dy).
    // Explication : dx et dy représentent respectivement la direction du pas pour les coordonnées x et y.
    //Si x2 == x1, le mouvement est vertical, donc dx = 0. Si y2 == y1, le mouvement est horizontal, donc dy = 0.
    int dx = (x2 - x1) == 0 ? 0 : (x2 - x1) / abs(x2 - x1);   // dx vaut 0 si x1==x2  et sinon : dx = 1 si deplacement vers le bas et -1 si deplacement vers le                                                           haut
    int dy = (y2 - y1) == 0 ? 0 : (y2 - y1) / abs(y2 - y1);   // dy vaut 0 si y1==y2  et sinon : dy = 1 si deplacement vers la droite et -1 si deplacement vers la                                                           gauche

    int x = x1 + dx;   // La position de départ (x1, y1) n'est pas vérifiée car elle contient la pièce en mouvement déja.
    int y = y1 + dy;   // La boucle commence à (x1 + dx, y1 + dy), qui est la première cellule dans le sens du mouvement.
   

    while (x != x2 || y != y2) {        // Parcourir chaque cellule le long du chemin de (x1, y1) à (x2, y2) et vérifiez si elle est vide.
        if (!grille[x][y].estNull()) {  // Verifiez si la cellule x, y est non vide
            return false; // Si oui, il y a une pièce sur le chemin et retournez faux car chemin non libre
        }
        x += dx; // Sinon, on passe à la cellule suivante
        y += dy;
    }
    return true;  // Si destination atteinte sans encombre alors le chemin est libre
}

// Vérifie si un mouvement de piece de type "pion" est valide
bool Plateau::mouvementPionValide(int x1, int y1, int x2, int y2) const {
    piece p = grille[x1][y1];                  // recuperation de la piece
    int direction = (p.getCouleur()) ? -1 : 1; // Blancs avancent vers le haut (-1), Noirs vers le bas (+1)


    // Déplacement simple d'une case en avant (seulement si la case est vide)
    if (x2 == x1 + direction && y1 == y2 && grille[x2][y2].estNull()) {
        return true;
    }

    // Premier déplacement : 2 cases en avant (seulement si les 2 cases sont vides)
    if ((x1 == (p.getCouleur() ? 6 : 1)) && x2 == x1 + 2 * direction && y1 == y2 &&
        grille[x1 + direction][y2].estNull() && grille[x2][y2].estNull()) {
        return true;
    }


    // Capture en diagonale d'une piece adverse en s'assurant de se deplacer que d'une seule case et en verifant que la case de destination est non vide
    if (x2 == x1 + direction && abs(y2 - y1) == 1 && !grille[x2][y2].estNull() &&
        grille[x2][y2].getCouleur() != p.getCouleur()) {
        return true; // Capture valide
    }

    return false; // Mouvement invalide
}

// Vérifie si un mouvement est valide pour une pièce donnée
bool Plateau::mouvementValide(int x1, int y1, int x2, int y2) const {
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 || x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        //cout << "Erreur : Coordonnées hors limites ! \n";
        return false;
    }

    piece p = grille[x1][y1]; // Récupérer la pièce
    type_pion type = p.getType();  // Recuperer le type de la piece

    int dx = abs(x2 - x1);  // Distance ( decalage)  verticale entre les deux positions x1 et x2
    int dy = abs(y2 - y1);  // Distance ( decalage)  horizontale entre les deux positions y1 et y2

    // Vérifier si la destination est occupée par une pièce alliée
    if (!grille[x2][y2].estNull() && grille[x2][y2].getCouleur() == p.getCouleur()) { // Verifie si la case est non vide et occupé par une pièce alliée
        return false;   // Si oui, mouvement non valide
    }

    switch (type) {
            
    case type_pion::ROI:
        // Vérifier que le déplacement est valide (d'une case maximum)
        if (dx <= 1 && dy <= 1) {
            // Vérifier que la case où il veut se déplacer n'est pas attaquée par une pièce adverse
            Plateau temp = *this;
            temp.grille[x2][y2] = temp.grille[x1][y1]; // Déplacer le roi
            temp.grille[x1][y1] = piece(); // Vider l'ancienne case

            if (!temp.estEnEchec(p.getCouleur())) {
                return true; // Si le roi n'est pas en échec après déplacement, alors mouvement valide
            }
        }
        return false; // Déplacement invalide si la case est attaquée
            
    case type_pion::REINE:
        return (dx == dy || x1 == x2 || y1 == y2) && cheminLibre(x1, y1, x2, y2); // Vérifiez si la reine se déplace en ligne droite (horizontalement,                                                                                       verticalement ou en diagonale) et que le chemin est libre
    case type_pion::TOUR:
        return (x1 == x2 || y1 == y2) && cheminLibre(x1, y1, x2, y2);  // Vérifiez si la tour se déplace horizontalement ou verticalement et si le chemin est                                                                     libre.
            
    case type_pion::FOU:
        return (dx == dy) && cheminLibre(x1, y1, x2, y2);  // Vérifiez si le fou se déplace en diagonale et si le chemin est libre.

    case type_pion::CAVALIER:
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);  // Vérifiez si le cavalier se déplace en forme de "L".

    case type_pion::PION:
        return mouvementPionValide(x1, y1, x2, y2);  // Vérifiez si le déplacement du pion est valide à l'aide de la fonction mouvementPionValide.

    default:
        return false;  //  Gérez les types de pièces inconnus.
    }
}



// Jouer le coup spécial grand Roque ou petit Roque
bool Plateau::EstRoque(coup coupJoue) {
    // Vérifier que la pièce déplacée est bien un ROI
    if (coupJoue.pieceJouee.getType() != type_pion::ROI) return false;

    // Vérifier que le déplacement du roi est de exactement 2 cases en colonne (roque)
    if (abs(coupJoue.y2 - coupJoue.y1) != 2) return false;

    // Déterminer s'il s'agit d'un petit roque ou d'un grand roque
    bool petitRoque = (coupJoue.y2 > coupJoue.y1);  // Petit roque : le roi va à droite (colonne `g`)
    int rangee = coupJoue.x1;                       // Rangée du roi (7 pour Blancs, 0 pour Noirs)
    int roiCol = 4;                                 // Position initiale du roi
    int tourCol = petitRoque ? 7 : 0;               // Position initiale de la tour (7 pour petit roque, 0 pour grand roque)

    // Vérifier la présence du roi et de la tour aux bonnes positions
    if (grille[rangee][roiCol].getType() != type_pion::ROI || grille[rangee][roiCol].getCouleur() != coupJoue.pieceJouee.getCouleur()) {
        return false; // Le roi n'est pas en place
    }
    if (grille[rangee][tourCol].getType() != type_pion::TOUR || grille[rangee][tourCol].getCouleur() != coupJoue.pieceJouee.getCouleur()) {
        return false; // La tour n'est pas en place
    }

    // Vérifier que les cases entre le roi et la tour sont vides
    int direction = petitRoque ? 1 : -1;
    for (int c = roiCol + direction; c != tourCol; c += direction) {
        if (!grille[rangee][c].estNull()) {
            return false; // Une pièce bloque le chemin
        }
    }

    // Vérifier que le roi n'est pas en échec et qu'il ne traverse pas une case attaquée
    if (estEnEchec(coupJoue.pieceJouee.getCouleur())) {
        return false; // Roque interdit si le roi est en échec
    }
    
    for (int step = 1; step <= 2; ++step) {
        int colIntermediaire = roiCol + step * direction;
        Plateau temp = *this;
        temp.grille[rangee][colIntermediaire] = temp.grille[rangee][roiCol];
        temp.grille[rangee][roiCol] = piece();
        if (temp.estEnEchec(coupJoue.pieceJouee.getCouleur())) {
            return false; // Roque interdit si le roi passe par une case attaquée
        }
    }
    
    return true;
}




bool Plateau::jouerRoque(coup coupJoue) {
    
    // Vérifier que la pièce déplacée est bien un ROI
    if (coupJoue.pieceJouee.getType() != type_pion::ROI) { return false; }

    // Vérifier que le déplacement du roi est de exactement 2 cases en colonne (roque)
    if (abs(coupJoue.y2 - coupJoue.y1) != 2) { return false; }

    // Déterminer s'il s'agit d'un petit roque ou d'un grand roque
    bool petitRoque = (coupJoue.y2 > coupJoue.y1);  // Petit roque : le roi va à droite (colonne `g`)
    int rangee = coupJoue.x1;                       // Rangée du roi (7 pour Blancs, 0 pour Noirs)
    int roiCol = 4;                                 // Position initiale du roi
    int tourCol = petitRoque ? 7 : 0;               // Position initiale de la tour (7 pour petit roque, 0 pour grand roque)

    // Vérifier la présence du roi et de la tour aux bonnes positions
    if (grille[rangee][roiCol].getType() != type_pion::ROI || grille[rangee][roiCol].getCouleur() != coupJoue.pieceJouee.getCouleur()) {
        return false; // Le roi n'est pas en place
    }
    if (grille[rangee][tourCol].getType() != type_pion::TOUR || grille[rangee][tourCol].getCouleur() != coupJoue.pieceJouee.getCouleur()) {
        return false; // La tour n'est pas en place
    }

    // Vérifier que les cases entre le roi et la tour sont vides
    int direction = petitRoque ? 1 : -1;
    for (int c = roiCol + direction; c != tourCol; c += direction) {
        if (!grille[rangee][c].estNull()) {
            return false; // Une pièce bloque le chemin
        }
    }

    // Vérifier que le roi n'est pas en échec et qu'il ne traverse pas une case attaquée
    if (estEnEchec(coupJoue.pieceJouee.getCouleur())) {
        return false; // Roque interdit si le roi est en échec
    }
    for (int step = 1; step <= 2; ++step) {
        int colIntermediaire = roiCol + step * direction;
        Plateau temp = *this;
        temp.grille[rangee][colIntermediaire] = temp.grille[rangee][roiCol];
        temp.grille[rangee][roiCol] = piece();
        if (temp.estEnEchec(coupJoue.pieceJouee.getCouleur())) {
            return false; // Roque interdit si le roi passe par une case attaquée
        }
    }

    // Effectuer le roque
    grille[rangee][roiCol + 2 * direction] = grille[rangee][roiCol];
    grille[rangee][roiCol + 2 * direction].setPosition(rangee, roiCol + 2 * direction);
    grille[rangee][roiCol] = piece(); // Roi déplacé

    // Déplacer la tour
    int nouvelleTourCol = roiCol + direction;
    grille[rangee][nouvelleTourCol] = grille[rangee][tourCol];
    grille[rangee][nouvelleTourCol].setPosition(rangee, nouvelleTourCol);
    grille[rangee][tourCol] = piece(); // Tour déplacée
    
    dernierCoup = coupJoue;
    
    // Mise à jour du joueur actif
    joueurActuel = (joueurActuel == 0 ? 1 : 0);
    return true;
}



bool Plateau::EstPriseEnPassant(coup coupJoue) {
    int x1 = coupJoue.x1;
    int y1 = coupJoue.y1;
    int x2 = coupJoue.x2;
    int y2 = coupJoue.y2;
    
    // Vérifier que les coordonnées sont valides
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 ||
        x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        return false;
    }
    
    // Le pion qui capture doit être un pion du joueur actif
    if (grille[x1][y1].getType() != type_pion::PION ||
        grille[x1][y1].getCouleur() != (joueurActuel == 0)) {
        return false;
    }
    
    // Vérifier que le pion qui capture est sur la 5ème rangée
    if ((grille[x1][y1].getCouleur() && x1 != 3) ||  // Pion blanc doit être en rangée 3
        (!grille[x1][y1].getCouleur() && x1 != 4)) { // Pion noir doit être en rangée 4
        return false;
    }
    
    // Le déplacement doit être diagonal d'une case et la case d'arrivée doit être vide
    if (abs(x2 - x1) != 1 || abs(y2 - y1) != 1 || !grille[x2][y2].estNull()) {
        return false;
    }
    
    // Vérifier que le dernier coup joué était bien le double pas d'un pion adverse
    if (!(dernierCoup.pieceJouee.getType() == type_pion::PION &&
          abs(dernierCoup.x2 - dernierCoup.x1) == 2 &&
          dernierCoup.y2 == dernierCoup.y1)) {
        return false;
    }
    
    return true;
}



bool Plateau::JouerpriseEnPassant(coup coupJoue) {
    
    int x1 = coupJoue.x1;
    int y1 = coupJoue.y1;
    int x2 = coupJoue.x2;
    int y2 = coupJoue.y2;
    
    // Vérifier que les coordonnées sont valides
    if (x1 < 0 || x1 >= 8 || y1 < 0 || y1 >= 8 ||
        x2 < 0 || x2 >= 8 || y2 < 0 || y2 >= 8) {
        return false;
    }
    
    // Le pion qui capture doit être un pion du joueur actif
    if (grille[x1][y1].getType() != type_pion::PION ||
        grille[x1][y1].getCouleur() != (joueurActuel == 0)) {
        return false;
    }
    
    // Le déplacement doit être diagonal d'une case et la case d'arrivée doit être vide
    if (abs(x2 - x1) != 1 || abs(y2 - y1) != 1 || !grille[x2][y2].estNull()) {
        return false;
    }
    
    // Vérifier que le dernier coup joué était bien le double pas d'un pion adverse
    if (!(dernierCoup.pieceJouee.getType() == type_pion::PION &&
          abs(dernierCoup.x2 - dernierCoup.x1) == 2 &&
          dernierCoup.y2 == dernierCoup.y1)) {
        return false;
    }
    
    // Simulation du coup pour vérifier si le joueur se met en échec
    Plateau temp = *this;
    temp.grille[x2][y2] = temp.grille[x1][y1];
    temp.grille[x2][y2].setPosition(x2, y2);
    temp.grille[x1][y1] = piece(); // Vider l'ancienne case

    if (temp.estEnEchec(joueurActuel == 0)) {
        return false; // Interdit de jouer un coup qui met son propre roi en échec
    }
    
    // La case où se trouve le pion adverse capturable est celle d'arrivée du dernier coup
    int capturerX = dernierCoup.x2;
    int capturerY = dernierCoup.y2;
    

    // Exécuter la prise en passant :
    // 1. Supprimer le pion adverse sur sa case d'arrivée
    grille[capturerX][capturerY] = piece();
    // 2. Déplacer le pion capturant vers la case d'arrivée du coup en passant
    grille[x2][y2] = grille[x1][y1];
    grille[x2][y2].setPosition(x2, y2);
    // 3. Vider la case d'origine du pion capturant
    grille[x1][y1] = piece();
    
   
    dernierCoup = coupJoue;
    
    // Mettre à jour le joueur actif
    joueurActuel = (joueurActuel == 0 ? 1 : 0);
    return true;
}



bool Plateau::estCoupValide(coup coupJoue) {
    int x1 = coupJoue.x1;
    int y1 = coupJoue.y1;
    int x2 = coupJoue.x2;
    int y2 = coupJoue.y2;
    
    
    // Vérification des limites du plateau
    if (x1 < 0 || x1 >= taillePlateau || y1 < 0 || y1 >= taillePlateau ||
        x2 < 0 || x2 >= taillePlateau || y2 < 0 || y2 >= taillePlateau) {
        return false;
    }
    
    
    // Vérifier qu'une pièce est présente à la position d'origine
    if (grille[x1][y1].estNull()) {
        //cout<<"C'est ici le probleme dans est null"<<endl;
        return false;
    }

    // Vérifier que le joueur joue bien avec ses propres pièces
    if ( (grille[x1][y1].getCouleur() != (joueurActuel == 0)) ){
        //cout<<"C'est ici le probleme dans le joueur joue pas avec ses propres pièces"<<endl;
        return false;
    }
    
    // Empêcher la capture du roi
    if (!grille[x2][y2].estNull() && grille[x2][y2].getType() == type_pion::ROI) {
       //cout<<"C'est ici le probleme dans interdit de capturer un roi"<<endl;
       return false;  // Interdit de capturer un roi
    }
        
    //Verifier si c'est un roque
    if (EstRoque(coupJoue)) {
        return true;
    }
    
    //Verifier si c'est une prise en passant
    if (EstPriseEnPassant(coupJoue)) {
        return true;
    }
    
    // Vérifier si le mouvement est valide selon les règles d'échecs
    if (!mouvementValide(x1, y1, x2, y2)) {
        //cout<<"C'est ici le probleme dans mouvement valide"<<endl;
        return false;
    }
    
    
    // Simulation du coup pour vérifier si le joueur se met en échec
    Plateau temp = *this;
    temp.grille[x2][y2] = temp.grille[x1][y1];
    temp.grille[x2][y2].setPosition(x2, y2);
    temp.grille[x1][y1] = piece(); // Vider l'ancienne case

    if (temp.estEnEchec(joueurActuel == 0)) {
        //cout<<"C'est ici le probleme dans on doit pas mettre le roi en echec"<<endl;
        return false; // Interdit de jouer un coup qui met son propre roi en échec
    }

    return true;
}


// Joue un coup dans le jeu d'échecs
bool Plateau::jouerCoup(coup coupJoue) {
    
    int x1 = coupJoue.x1;
    int y1 = coupJoue.y1;
    int x2 = coupJoue.x2;
    int y2 = coupJoue.y2;
    
    
    // Gestion du Tic-Tac-Toe
    if (x1 == -1 && y1 == -1) {
        if (x2 < 0 || x2 >= taillePlateau || y2 < 0 || y2 >= taillePlateau || !grille[x2][y2].estNull())
            return false; // Coup invalide

        grille[x2][y2] = piece(joueurActuel == 0 ? type_pion::TIC_TAC_X : type_pion::TIC_TAC_O, joueurActuel == 0, x2, y2);
        joueurActuel = (joueurActuel == 0) ? 1 : 0;
        
        return true;
    }

    // Vérification des limites du plateau
    if (x1 < 0 || x1 >= taillePlateau || y1 < 0 || y1 >= taillePlateau ||
        x2 < 0 || x2 >= taillePlateau || y2 < 0 || y2 >= taillePlateau) {
        return false;
    }
    
    
    // Vérifier qu'une pièce est présente à la position d'origine
    if (grille[x1][y1].estNull()) {
        //cout<<"C'est ici le probleme dans est null"<<endl;
        return false;
    }

    // Vérifier que le joueur joue bien avec ses propres pièces
    if ( (grille[x1][y1].getCouleur() != (joueurActuel == 0)) ){
        //cout<<"C'est ici le probleme dans le joueur joue pas avec ses propres pièces"<<endl;
        return false;
    }
    
    // Empêcher la capture du roi
    if (!grille[x2][y2].estNull() && grille[x2][y2].getType() == type_pion::ROI) {
       //cout<<"C'est ici le probleme dans interdit de capturer un roi"<<endl;
       return false;  // Interdit de capturer un roi
    }
        
    //Verifier si c'est un roque
    if (EstRoque(coupJoue)) {
        return jouerRoque(coupJoue);
    }
    
    //Verifier si c'est une prise en passant
    if (EstPriseEnPassant(coupJoue)) {
        return JouerpriseEnPassant(coupJoue);
    }
    
    // Vérifier si le mouvement est valide selon les règles d'échecs
    if (!mouvementValide(x1, y1, x2, y2)) {
        //cout<<"C'est ici le probleme dans mouvement valide"<<endl;
        return false;
    }
    
    // Simulation du coup pour vérifier si le joueur se met en échec
    Plateau temp = *this;
    temp.grille[x2][y2] = temp.grille[x1][y1];
    temp.grille[x2][y2].setPosition(x2, y2);
    temp.grille[x1][y1] = piece(); // Vider l'ancienne case

    if (temp.estEnEchec(joueurActuel == 0)) {
        //cout<<"C'est ici le probleme dans on doit pas mettre le roi en echec"<<endl;
        return false; // Interdit de jouer un coup qui met son propre roi en échec
    }

    // Déplacer la pièce sur la nouvelle case
    grille[x2][y2] = grille[x1][y1];
    grille[x2][y2].setPosition(x2, y2);
    grille[x1][y1] = piece(); // Vider l'ancienne case
    
    // Promotion de pion : si la pièce déplacée est un pion et qu'elle arrive à la dernière rangée
    if (grille[x2][y2].getType() == type_pion::PION) {
        bool estBlanc = grille[x2][y2].getCouleur();
        if ((estBlanc && x2 == 0) || (!estBlanc && x2 == 7)) {
            grille[x2][y2] = piece(type_pion::REINE, estBlanc, x2, y2);
        }
    }
    
    dernierCoup = coupJoue;
    

    // Mettre à jour le joueur actuel
    joueurActuel = (joueurActuel == 0) ? 1 : 0;
    
    return true;
}



// Vérifie si la grille est pleine pour le Tic-Tac-Toe
bool Plateau::estPlein() const {
    for (const auto& ligne : grille)
        for (const auto& case_ : ligne)
            if (case_.estNull()) return false;
    return true;
}


bool Plateau::estEnEchec(bool roiBlanc) const {
    pair<int, int> positionRoi;
    bool roiTrouve = false;
    // Trouver la position du roi en quittant les deux boucles dès qu'il est trouvé.
    for (int i = 0; i < 8 && !roiTrouve; i++) {
        for (int j = 0; j < 8; j++) {
            if (!grille[i][j].estNull() && grille[i][j].getType() == type_pion::ROI &&
                grille[i][j].getCouleur() == roiBlanc) {
                positionRoi = { i, j };
                roiTrouve = true;
                break;
            }
        }
    }
    // Si le roi n'a pas été trouvé, on peut retourner false ou lever une exception.
    if (!roiTrouve) 
        return false;

    
    // Vérifier si une pièce adverse peut attaquer le roi
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (!grille[i][j].estNull() && grille[i][j].getCouleur() != roiBlanc) {
                    type_pion type = grille[i][j].getType();
                    
                    // Vérifier si la pièce peut atteindre la position du roi
                    if (mouvementValide(i, j, positionRoi.first, positionRoi.second)) {
                        
                        // Si c'est une tour, un fou ou une reine, il faut que le chemin soit libre
                        if (type == type_pion::TOUR || type == type_pion::FOU || type == type_pion::REINE) {
                            if (cheminLibre(i, j, positionRoi.first, positionRoi.second)) {
                                return true;  // Roi en échec
                            }
                        } else {
                            return true;  // Cavaliers et pions n'ont pas besoin de chemin libre
                        }
                    }
                }
            }
        }
        
    
    return false;  // Le roi n'est pas en échec
}

bool Plateau::estEchecEtMat(bool roiBlanc) const {
    // Si le roi n'est pas en échec, ce n'est pas un échec et mat.
    if (!estEnEchec(roiBlanc)) return false;

    pair<int, int> positionRoi;
    bool roiTrouve = false;
    // Recherche du roi en quittant les deux boucles dès qu'il est trouvé.
    for (int i = 0; i < 8 && !roiTrouve; i++) {
        for (int j = 0; j < 8; j++) {
            if (!grille[i][j].estNull() && grille[i][j].getType() == type_pion::ROI &&
                grille[i][j].getCouleur() == roiBlanc) {
                positionRoi = { i, j };
                roiTrouve = true;
                break;
            }
        }
    }
    if (!roiTrouve)
        return false;

    // Vérifier si le roi peut s'échapper en se déplaçant sur une case adjacente
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = positionRoi.first + dx;
            int ny = positionRoi.second + dy;
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                // On considère les cases vides ou occupées par une pièce adverse.
                if (grille[nx][ny].estNull() || grille[nx][ny].getCouleur() != roiBlanc) {
                    Plateau temp = *this;
                    temp.grille[nx][ny] = temp.grille[positionRoi.first][positionRoi.second];
                    temp.grille[positionRoi.first][positionRoi.second] = piece();
                    if (!temp.estEnEchec(roiBlanc)) {
                        return false; // Le roi peut s'échapper
                    }
                }
            }
        }
    }

    // Vérifier si une pièce alliée peut intervenir (bloquer l'attaque ou capturer l'attaquant)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!grille[i][j].estNull() && grille[i][j].getCouleur() == roiBlanc && grille[i][j].getType() != type_pion::ROI) {
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (mouvementValide(i, j, x, y)) {
                            Plateau temp = *this;
                            temp.grille[x][y] = temp.grille[i][j];
                            temp.grille[i][j] = piece();
                            if (!temp.estEnEchec(roiBlanc)) {
                                return false; // Une pièce alliée peut intervenir
                            }
                        }
                    }
                }
            }
        }
    }
    return true; // Aucun coup légal n'est trouvé → échec et mat
}


bool Plateau::estPat(bool roiBlanc) const {
    
    // Si le roi est en échec, il ne peut pas y avoir de pat.
    if (estEnEchec(roiBlanc))
        return false;
    
    // Parcourir toutes les pièces du joueur de couleur roiBlanc et tenter tous les coups possibles.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!grille[i][j].estNull() && grille[i][j].getCouleur() == roiBlanc) {
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (mouvementValide(i, j, x, y)) {
                            Plateau temp = *this;
                            temp.grille[x][y] = temp.grille[i][j];
                            temp.grille[i][j] = piece();
                            if (!temp.estEnEchec(roiBlanc))
                                return false; // Un coup légal qui sort du roi de l'échec (ou le maintient en sécurité) a été trouvé.
                        }
                    }
                }
            }
        }
    }
    // Aucun coup légal n'est possible : c'est un pat.
    return true;
}


// Fonction qui vérifie si le matériel est insuffisant pour forcer une victoire.
// Pour simplifier, ici on considère le cas minimal : s'il ne reste que les deux rois.
bool Plateau::insufficientMaterial() const {
    int compteurAutres = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!grille[i][j].estNull() && grille[i][j].getType() != type_pion::ROI) {
                compteurAutres++;
            }
        }
    }
    return (compteurAutres == 0);
}



// Vérifie s'il y a un gagnant dans le jeu de Tic-Tac-Toe
char Plateau::verifierGagnantTic() const {
    
    for (int i = 0; i < 3; i++) {
        if (!grille[i][0].estNull() && grille[i][0].getType() == grille[i][1].getType() && grille[i][1].getType() == grille[i][2].getType())
            return grille[i][0].getCouleur() ? 'X' : 'O';
        if (!grille[0][i].estNull() && grille[0][i].getType() == grille[1][i].getType() && grille[1][i].getType() == grille[2][i].getType())
            return (grille[0][i].getCouleur()) ? 'X' : 'O';
    }

    if (!grille[0][0].estNull() && grille[0][0].getType() == grille[1][1].getType() && grille[1][1].getType() == grille[2][2].getType())
        return (grille[0][0].getCouleur()) ? 'X' : 'O';
    if (!grille[0][2].estNull() && grille[0][2].getType() == grille[1][1].getType() && grille[1][1].getType() == grille[2][0].getType())
        return (grille[0][2].getCouleur()) ? 'X' : 'O';
    
    if (estPlein()) {
        return 'V' ; //V pour match null
    }

    return ' '; // Pas de gagnant
}


char Plateau::verifierGagnantEchec() const {
    // Vérifier l'échec et mat
    if (estEchecEtMat(true))  return 'N';  // Roi blanc mat → Noir gagne
    if (estEchecEtMat(false)) return 'B';  // Roi noir mat → Blanc gagne

    // Vérifier le pat (aucun coup légal mais pas en échec)
    if (estPat(true) || estPat(false)) {
        //cout<<"c'est ici dans estPat"<<endl;
        return 'V'; // Match nul (pat)
    }
    
    // Vérifier une insuffisance matérielle (exemple minimal : uniquement les rois restent).
    if (insufficientMaterial()) {
        //cout<<"c'est ici dans materiel"<<endl;
        return 'V'; // Match nul
    }
    
    return ' '; // Partie non terminée
}




char Plateau::verifierGagnant() const {
    if (getTypePlateau()==TypeJeu::TIC_TAC_TOE) {
        return verifierGagnantTic() ;
    }
    else{
        return verifierGagnantEchec() ;
    }
};



int Plateau::compterLignesPresqueCompletes(bool maximise) const {
    int count = 0;
    type_pion t = type_pion::TIC_TAC_X ;
    int signe = -1; // Si on maximise, on renvoie un comptage positif sinon un comptage negatif;
    
    if (maximise) {
        t = type_pion::TIC_TAC_O ;
        signe = 1 ; // Si on maximise, on renvoie un comptage positif sinon un comptage negatif;
    }

    // Vérifier les lignes et colonnes
    for (int i = 0; i < 3; i++) {
        // Lignes
        if (grille[i][0].getType() == t && grille[i][1].getType() == t && grille[i][2].estNull()) count++;
        if (grille[i][0].getType() == t && grille[i][2].getType() == t && grille[i][1].estNull()) count++;
        if (grille[i][1].getType() == t && grille[i][2].getType() == t && grille[i][0].estNull()) count++;

        // Colonnes
        if (grille[0][i].getType() == t && grille[1][i].getType() == t && grille[2][i].estNull()) count++;
        if (grille[0][i].getType() == t && grille[2][i].getType() == t && grille[1][i].estNull()) count++;
        if (grille[1][i].getType() == t && grille[2][i].getType() == t && grille[0][i].estNull()) count++;
    }

    // Diagonales
    if (grille[0][0].getType() == t && grille[1][1].getType() == t && grille[2][2].estNull()) count++;
    if (grille[0][0].getType() == t && grille[2][2].getType() == t && grille[1][1].estNull()) count++;
    if (grille[1][1].getType() == t && grille[2][2].getType() == t && grille[0][0].estNull()) count++;

    if (grille[0][2].getType() == t && grille[1][1].getType() == t && grille[2][0].estNull()) count++;
    if (grille[0][2].getType() == t && grille[2][0].getType() == t && grille[1][1].estNull()) count++;
    if (grille[1][1].getType() == t && grille[2][0].getType() == t && grille[0][2].estNull()) count++;

    return count*signe;
}



//Réinitialise la grille
void Plateau::reset() {
    grille = vector<vector<piece>>(taillePlateau, vector<piece>(taillePlateau, piece()));  //Réinitialise la grille avec des pièces vides
    joueurActuel = 0;  // Redefinir le premier joueur à se lancer

    if (typePlateau == TypeJeu::ECHECS) {   //Si le type de jeu est Échecs,
        initialiserEchecs();  // réinitialise les pièces sur l'échiquier
    }
}



//Retourne un vecteur des pièces (non NULL) disponibles sur la grille
vector<piece> Plateau::getPieces() const
{
    vector<piece> piesse;  // Déclarer un vecteur de pieces vide ; l'orthographe est volontaire pour eviter toute confusion avec un objet de type piece tout en                         restant significatif
    for (const auto& ligne : grille)  // Parcourt des lignes de la grille
    {
        for (const auto& p : ligne)   // Parcourt des colonnes de la ligne courante
        {
            if (!p.estNull())  // Verifiez si la pièce considérée est non NULL
            {
                piesse.push_back(p); // Si oui, l'inserer dans le vecteur de pieces.
            }
        }
    }
    return piesse;  // Retourner le vecteur piesse
}



//Retourne la grille actuelle
vector<vector<piece>> Plateau::get_grille() const
{
    return grille; //Retourne la grille représentant l'état actuel du jeu
}


vector<string> Plateau::getHistoriqueCoups() const {
    return historiqueCoups;
}


void Plateau::ajouterCoup(coup coup) {
    historiqueCoups.push_back(coup.traduireCoup(coup));
}



