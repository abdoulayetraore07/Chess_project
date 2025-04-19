#include "Piece.hpp"
#include <iostream>
#include <cassert>

// Constructeur par défaut
piece::piece() : type(type_pion::null), valeur(0), couleur(true), x(0), y(0) {}
/*
    Constructeur par défaut de la classe `piece`.
    Initialise une pièce avec les valeurs par défaut suivantes :
    - type : type_pion::null (pièce non définie)
    - valeur : 0 (valeur par défaut pour une pièce null)
    - couleur : true (blanc par défaut)
    - position : x = 0, y = 0 (position par défaut sur l'échiquier)
*/


// Constructeur avec paramètres
piece::piece(type_pion t, bool estBlanc, int posX, int posY)
    : type(t), valeur(0), couleur(estBlanc), x(posX), y(posY)
{
    switch (t)
    {
        case type_pion::ROI: valeur = 0; break;       // Le ROI a une valeur de 0
        case type_pion::REINE: valeur = 9; break;    // La REINE a une valeur de 4
        case type_pion::TOUR: valeur = 5; break;     // La TOUR a une valeur de 3
        case type_pion::FOU: valeur = 3; break;     // Le FOU a une valeur de 2
        case type_pion::CAVALIER: valeur = 3; break; // Le CAVALIER a une valeur de 1
        case type_pion::PION: valeur = 1; break;    // Le PION a une valeur de 0
        case type_pion::TIC_TAC_X: valeur = 0; break; // Le TIC_TAC_X a une valeur de 3
        case type_pion::TIC_TAC_O: valeur = 0; break; // Le TIC_TAC_O a une valeur de 3
        case type_pion::null:
        default:
            assert(false && "Erreur: type_pion inconnu !");
    }
}
/*
    Constructeur avec paramètres de la classe `piece`.
    Initialise une pièce avec les paramètres suivants :
    - type : type_pion (type de la pièce)
    - estBlanc : bool (couleur de la pièce, true = blanc, false = noir)
    - posX : int (position x sur l'échiquier)
    - posY : int (position y sur l'échiquier)
    La valeur de la pièce est déterminée en fonction de son type via un switch.
*/


// Getters
type_pion piece::getType() const {return type; }
/*
    Accesseur pour obtenir le type de la pièce.
    Retourne : type_pion (type de la pièce)
*/

int piece::getValeur() const { return valeur; }
/*
    Accesseur pour obtenir la valeur de la pièce.
    Retourne : int (valeur de la pièce)
*/

int piece::getX() const { return x; }
/*
    Accesseur pour obtenir la position x de la pièce.
    Retourne : int (position x sur l'échiquier)
*/

int piece::getY() const { return y; }
/*
    Accesseur pour obtenir la position y de la pièce.
    Retourne : int (position y sur l'échiquier)
*/

bool piece::getCouleur() const { return couleur; }
/*
    Accesseur pour obtenir la couleur de la pièce.
    Retourne : bool (true = blanc, false = noir)
*/

// Vérifier si la pièce est une pièce null
bool piece::estNull() const {
    return (type == type_pion::null);
}
/*
    Méthode pour vérifier si la pièce est une pièce null.
    Retourne : bool (true si la pièce est null, false sinon)
*/

// Changer la position de la pièce
void piece::setPosition(int posX, int posY) {
    x = posX;
    y = posY;
}
/*
    Méthode pour modifier la position de la pièce.
    Paramètres :
    - posX : int (nouvelle position x sur l'échiquier)
    - posY : int (nouvelle position y sur l'échiquier)
*/


// Récupérer le caractère Unicode de la pièce
wchar_t piece::getUnicode() const {
    switch (type) {
        case type_pion::ROI:        return !couleur ? L'♔' : L'♚'; // Roi (King)
        case type_pion::REINE:      return !couleur ? L'♕' : L'♛'; // Reine (Queen)
        case type_pion::TOUR:       return !couleur ? L'♖' : L'♜'; // Tour (Rook)
        case type_pion::FOU:        return !couleur ? L'♗' : L'♝'; // Fou (Bishop)
        case type_pion::CAVALIER:   return !couleur ? L'♘' : L'♞'; // Cavalier (Knight)
        case type_pion::PION:       return !couleur ? L'♙' : L'♟'; // Pion (Pawn)
        case type_pion::TIC_TAC_X:  return L'X'; // X pour Tic-Tac-Toe
        case type_pion::TIC_TAC_O:  return L'O'; // O pour Tic-Tac-Toe
        case type_pion::null:       return L' '; //Piece vide
    }
    throw std::runtime_error("Erreur : Type de pièce inconnu dans getUnicode().");
    
}

/*
    Méthode pour récupérer le caractère Unicode correspondant à la pièce.
    Retourne : wchar_t (caractère Unicode représentant la pièce)
    Les caractères sont choisis en fonction du type et de la couleur de la pièce.
*/






