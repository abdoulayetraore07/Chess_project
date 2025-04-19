#ifndef ECHIQUIER_HPP
#define ECHIQUIER_HPP

#include <vector>
#include <iostream>
#include <locale> // Pour g�rer l'encodage
#include <codecvt>
#include "Piece.hpp"
#include "coup.hpp"
#include "openings.hpp"

using namespace std;



/*
 #MODULE Echiquier.hpp contenant :
 
 
 class Plateau                                                             Classe Plateau permettant d'initialiser la grille pour chaque jeu et sa gestion
 
    attributs :
                 vector<vector<piece>> grille;                             Grille 3x3 pour le jeu de Tic_tac TOE et 8x8 pour le jeu d'échecs
                 int typePlateau;                                          Entier pour définir le type de jeu : Tic_tac TOE ou jeu d'échecs
                 int taillePlateau;                                        Entier désignant la taille du plateau en fonction du type de jeu
                 int joueurActuel;                                         Entier désignant le joueur courant : On convient que 0 c'est blanc et 1                                                           c'est noir
 constructeurs :
                 Plateau(int type);                                        initialise la grille vide pour le jeu
    Accesseurs ;
                 vector<vector<piece>> get_grille();                        accesseur retournant la grille : une "Matrice" de pièces représentant l'état du jeu
                 void initialiserEchecs();                                  permettant d'initialiser le jeu d'échec en plaçant les pièces sur la grille de départ
                 int getTaillePlateau() const { return taillePlateau; } ;          accesseur retournant l'attribut taillePlateau
                 int getTypePlateau() const { return typePlateau; } ;              accesseur retournant l'attribut typePlateau


                 void afficher() const; // Affiche la grille                permettant d'afficher l'état du jeu ( la grille )
                 bool cheminLibre(int x1, int y1, int x2, int y2);          bool indiquant s'il existe un chemin pour se déplacer d'une position donnée à une autre
                 bool mouvementValide(int x1, int y1, int x2, int y2);      bool indiquant si un mouvement est valide en fonction du type de piece, d'une position                                                         de départ et de fin
                 bool mouvementPionValide(int x1, int y1, int x2, int y2);  bool indiquant si un mouvement basique est valide en fonction d'une position                                                                   de départ et de fin ; indépendant du type de pièce
                 bool jouerCoup(int x, int y);                              bool indiquant si un coup est valide dans le TIC_TAC TOE : x,y désignant la positionn                                                          d'arrivée
                 bool jouerCoup(int x1, int y1, int x2=-1, int y2=-1);            bool indiquant si un coup est valide dans le jeu d'Echecs : (x1,y1) désigant la                                                                position de départ et (x2,y2) la positon finale
                 bool estPlein() const; // Vérifie si la grille est pleine  bool indiquant si la grille est pleine dans le jeu du TIC_TAC TOE
                 char verifierGagnant() const;                              char 'X', 'O', ' ' vérifiant s'il y'a un gagnant ou pas dans le TIC_TAC TOE

                 int getJoueurActuel() const;                               Entier retournant le joueur actuel

                 vector<piece> getPieces() const;                           Accesseur retournant un vecteur des pieces disponibles sur la grille

                 void reset();                                              Réinitialise la grille
                 
 */




//Enum class pour plus de lisibilité
enum class TypeJeu {
    TIC_TAC_TOE,
    ECHECS
};

//Classe Plateau permettant d'initialiser la grille pour chaque jeu et sa gestion
class Plateau
{
private:
    vector<vector<piece>> grille;   // Grille 3x3 pour le jeu de Tic_tac TOE et 8x8 pour le jeu d'échecs
    TypeJeu typePlateau;                // Entier pour définir le type de jeu : 0 Tic_tac TOE et 1 jeu d'échecs
    int taillePlateau;              // Entier désogant la taille du plateau en fonction du type de jeu
    int joueurActuel;               // Entier désigant le joueur courant : On convient que 0 c'est blanc et 1 c'est noir
    coup dernierCoup;               // Stocke le dernier coup joué juste pour la prise en passant mais c'est pas obligatoire
    vector<string> historiqueCoups;
    Openings openings;

public:
    
    Plateau(int type = 0);                                  // initialise la grille vide pour le jeu
    
    vector<vector<piece>> get_grille() const;                 // accesseur retournant la grille : une "Matrice" de pièces réprésentant l'état du jeu
    void set_grille(vector<vector<piece>> grille_x ) { grille = grille_x ;} 
    coup getderniercoup() const { return dernierCoup ;}
    void setDernierCoup(coup d) { dernierCoup = d;}
    
    void initialiserEchecs();                           // permettant d'initialiser le jeu d'échec en plaçant les pièces sur la grille de départ
    void vidergrille() ;
    char getSymboleH(char symboleIA) const ;
    int getTaillePlateau() const { return taillePlateau; } ;   // accesseur retournant l'attribut taillePlateau
    TypeJeu getTypePlateau() const { return typePlateau; } ;       // accesseur retournant l'attribut typePlateau
    int getJoueurActuel() const { return joueurActuel; };   // Entier retournant le joueur actuel : 0 pour joueur X ou Blanc, 1 pour joueur O ou Noir;
    vector<piece> getPieces() const;                          // Accesseur retournant un vecteur des pieces non "null" disponibles sur la grille
    void setJoueurActuel(int d) { joueurActuel=d; } ;   // Setter pour modifier le jeu commençant au début
    
    void afficher() const;                                    // permettant d'afficher l'état du jeu ( la grille )

    bool cheminLibre(int x1, int y1, int x2, int y2) const;         // bool indiquant s'il existe un chemin pour se déplacer d'une position donnée à une autre
    bool mouvementValide(int x1, int y1, int x2, int y2) const;     // bool indiquant si un mouvement est valide en fonction du type de piece, d'une position                                                                   de départ et de fin
    bool mouvementPionValide(int x1, int y1, int x2, int y2) const; // bool indiquant si un mouvement basique est valide en fonction d'une position                                                                             de départ et de fin ; indépendant du type de pièce
    
    //Coups spéciaux
    bool EstRoque(coup coupJoue);
    bool jouerRoque(coup coupJoue);
    bool EstPriseEnPassant(coup coupJoue);
    bool JouerpriseEnPassant(coup coupJoue);

    bool jouerCoup(coup coup);           // bool indiquant si un coup est valide dans le jeu d'Echecs : (x1,y1) désigant la                                                                          position de départ et (x2,y2) la position finale
    
    //Openings
    vector<string> getHistoriqueCoups() const;
    Openings getOpenings() const { return openings; }
    void ajouterCoup(coup coup);
    
    bool estCoupValide(coup coupJoue);
    bool estEnEchec(bool roiBlanc) const;
    bool estEchecEtMat(bool roiBlanc) const;
    // Détecte si le joueur de couleur roiBlanc est en pat (stalemate)
    bool estPat(bool roiBlanc) const;
    // Détecte si le matériel est insuffisant pour forcer une victoire
    bool insufficientMaterial() const;

    bool estPlein() const;                                    // bool indiquant si la grille est pleine dans le jeu du TIC_TAC TOE

    char verifierGagnantTic() const;                             // char 'X', 'O', ' ' vérifiant s'il y'a un gagnant ou pas dans le TIC_TAC TOE
    char verifierGagnantEchec() const;
    char verifierGagnant() const ;                              // Fonction globale pour verifier gagnant
    
    int compterLignesPresqueCompletes(bool maximise) const  ;     //Fonction pour compter le nombre de lignes presques completes pour heureustique TTT

    void reset();                                             // Réinitialise la grille
};

#endif

