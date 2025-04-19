/// coup.hpp
#ifndef COUP_HPP
#define COUP_HPP

#include "Piece.hpp"
#include <iostream>
#include <string>



/*
  #MODULE coup.hpp

  Représente un coup :
    - bool couleur : true = Blanc, false = Noir
    - piece pieceDeplacee : la pièce qu’on déplace
    - (x1, y1) : position de départ
    - (x2, y2) : position d’arrivée
    - bool prise : prise d’une pièce adverse ?

*/

class coup
{
public:
    bool couleur;        // true=Blanc, false=Noir
    int x1, y1;          // Coordonnées source
    int x2, y2;          // Coordonnées destination
    bool prise;          // Indique s’il y a prise
    piece pieceJouee;   // pièce réprésentant la pièce qui effectue le mouvement
    piece piecePrise;   // pièce réprésentant eventuellement une pièce capturée lors du coup (s'il y en a une)
    bool estRoque;      // bool indiquant si c'est un roque : coup spécial pour défendre en rapprochant un roi et une tour
    bool enPassant;     // bool indiquant si c'est une prise en passant

    // Constructeur principal
    coup() : couleur(false), x1(-1), y1(-1), x2(-1), y2(-1), pieceJouee(), piecePrise(), estRoque(false), enPassant(false) {}
    coup(int x1, int y1, int x2=-1, int y2=-1, piece pieceJouee = piece(), piece piecePrise = piece() , bool estRoque = false , bool enPassant = false, bool couleur = false ) ;
    coup(const std::string& notation);

    
    //Getters
    bool GetCouleur() {return couleur;}
    bool GetX1() {return x1;}
    bool GetY1() {return y1;}
    bool GetX2() {return x2;}
    bool GetY2() {return y2;}
    piece GetPieceJouee() {return pieceJouee;}
    piece GetPiecePrise() {return piecePrise;}
    
    //Setters
    void SetCouleur(bool c) {couleur=c ;}
    void SetX1(int X1) {x1 = X1;}
    void SetY1(int Y1) {y1 = Y1;}
    void SetX2(int X2) {x2 = X2;}
    void SetY2(int Y2) {y2 = Y2;}
    
    // Openings
    bool operator==(const coup& other) const;
    string traduireCoup(coup coup);
    coup traduireCoup(string coupStr);
};

#endif































/*
 #MODULE coup.hpp contenant :
 
 
 class Coup                                         Classe coup permettant à un joueur de jouer un coup
 
    attributs :  int x1, y1;                        entiers réprésentant la position initiale du pion jouant le coup
                 int x2, y2;                        entiers réprésentant la position finale du pion jouant le coup après le déplacement (coup)
                 piece pieceJouee;                  pièce réprésentant la pièce qui effectue le mouvement
                 piece piecePrise;                  pièce réprésentant eventuellement une pièce capturée lors du coup (s'il y en a une)
                 bool estRoque;                     bool indiquant si c'est un roque : coup spécial pour défendre en rapprochant un roi et une tour
                 bool enPassant;                    bool indiquant si c'est une prise en passant
    constructeurs :
                 Coup(int x1, int y1, int x2, int y2, piece pieceJouee, piece piecePrise = piece(), bool estRoque = false, bool enPassant = false);
                                                    constructeur pour réaliser un coup avec les parametres définis sur dessus
    Accesseurs ;
                 int getX1() const;                 accesseur pour obtenir la position x1 de départ de la pièce
                 int getY1() const;                 accesseur pour obtenir la position y1 de départ de la pièce
                 int getX2() const;                 accesseur pour obtenir la position x2 de départ de la pièce
                 int getY2() const;                 accesseur pour obtenir la position y2 de départ de la pièce
                 piece getPieceJouee() const;       accesseur retournant la piece effectuant le coup
                 piece getPiecePrise() const;       accesseur retournant la piece eventuellement capturée lors du coup
                 bool estUnRoque() const;           accesseur retournant l'attribut estRoque : True si on joue un roque et False sinon
                 bool estUnePriseEnPassant() const; accesseur retournant l'attribut enPassant : True si c'est une prise en passant et False sinon
 
 */

/*
class Coup {
private:
    int x1, y1;         // entiers réprésentant la position initiale du pion jouant le coup
    int x2, y2;         // entiers réprésentant la position finale du pion jouant le coup après le déplacement (coup)
    piece pieceJouee;   // pièce réprésentant la pièce qui se déplace
    piece piecePrise;   // pièce réprésentant eventuellement une pièce capturée lors du coup (s'il y en a une)
    bool estRoque;      // bool indiquant si c'est un roque : coup spécial pour défendre en rapprochant un roi et une tour
    bool enPassant;     // bool indiquant si c'est une prise en passant

public:
    // Constructeur
    Coup(int sx1, int sy1, int sx2, int sy2,
             const piece& pj = piece(),
             const piece& pprise = piece(),
             bool estRoque = false,
             bool priseEnPassant = false)
            : x1(sx1), y1(sy1), x2(sx2), y2(sy2),
              pieceJouee(pj), piecePrise(pprise),
              estRoque(estRoque), enPassant(priseEnPassant)
    {}
                        // constructeur pour réaliser un coup avec les parametres définis sur dessus
    

    // Getters
    int getX1() const { return x1; }                        // accesseur pour obtenir la position x1 de départ de la pièce
    int getY1() const { return x2; }                        // accesseur pour obtenir la position y1 de départ de la pièce
    int getX2() const { return y1; }                        // accesseur pour obtenir la position x2 de départ de la pièce
    int getY2() const { return y2; }                        // accesseur pour obtenir la position y2 de départ de la pièce
    piece getPieceJouee() const { return pieceJouee; }      // accesseur retournant la piece effectuant le coup
    piece getPiecePrise() const { return piecePrise; }      // accesseur retournant la piece eventuellement capturée lors du coup
    bool estUnRoque() const { return estRoque; }            // accesseur retournant l'attribut estRoque : True si on joue un roque et False sinon
    bool estUnePriseEnPassant() const { return enPassant; } // accesseur retournant l'attribut enPassant : True si c'est une prise en passant et False sinon
};

#endif // COUP_HPP
*/
