#ifndef PIECE_HPP
#define PIECE_HPP

#include <iostream>
#include <string>

using namespace std;


/*
 #MODULE Pièce.hpp contenant :
 
 enum class type_pion                                 Permettant de définir les types de pièces pour le jeu de dame et le TIC_TAC TOE
                                                      (Exemple : roi, reine, tour, .... TIC_TAC_X, TIC_TAC_O)
 
 
 class piece     //Classe pièce permettant d'initialiser une pièce sur l'échiquier :
 
    attributs : type_pion type;                       répresente le type de la pièce, dans la liste disponible dans enum class type_pion
                int valeur;                           entier répresentant la valeur de la pièce en fonction de son type ( ROI vaut 5, REINE vaut 4, etc )
                bool couleur;                         bool répresentant la couleur d'une pièce, True = Blanc et False = noir pour différencier le jeu de pièce de                                     chaque joueur
                int x, y;                             entiers répresentant la position de la pièce sur la grille de dimensions 8x8
    constructeurs :
                piece();                              constructeur de pièce par défaut : type::null, valeur=0, couleur=true, x=0, y=0
                piece(type_pion t, bool estBlanc,int posX,int posY);
                                                      constructeur de pièce avec les paramètres = type, valeur, couleur, positon x, position y
    Accesseurs ;
                type_pion getType() const;            accesseur pour obtenir le type d'une pièce
                int getValeur() const;                accesseur pour obtenir la valeur d'une pièce
                int getX() const;                     accesseur pour obtenir la position x d'une pièce
                int getY() const;                     accesseur pour obtenir la position y d'une pièce
                bool getCouleur() const;              accesseur pour obtenir la couleur d'une pièce
                bool estNull() const;                 permettant de vérifier si la pièce est une pièce null
                void setPosition(int posX, int posY); permettant de modifier la position de la pièce aprèes un déplacement
                wchar_t getUnicode() const;           permettant de récupérer le caractère Unicode de la pièce pour un affichage attractif
 */



// Différents types de pions/pieces
enum class type_pion {
    null,
    TIC_TAC_X,
    TIC_TAC_O,
    ROI,
    REINE,
    TOUR,
    FOU,
    CAVALIER,
    PION
};

class piece {
private:
    type_pion type;
    int valeur;
    bool couleur;  // true = Blanc, false = Noir
    int x, y;

public:
    // Constructeur par défaut
    piece();

    // Constructeur paramétré
    piece(type_pion t, bool estBlanc, int posX, int posY);

    // Getters
    type_pion getType() const;
    int getValeur() const;
    int getX() const;
    int getY() const;
    bool getCouleur() const; // true = Blanc, false = Noir

    // Indique si la pièce est "vide"
    bool estNull() const;

    // Setter
    void setPosition(int posX, int posY);

    // Récupérer l'affichage Unicode
    wchar_t getUnicode() const;
};


#endif
