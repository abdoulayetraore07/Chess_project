#include "coup.hpp"

// Constructeur
coup::coup(int x1, int y1, int x2, int y2, piece pieceJouee, piece piecePrise, bool estRoque, bool enPassant, bool couleur)
    : x1(x1), y1(y1), x2(x2), y2(y2), pieceJouee(pieceJouee), piecePrise(piecePrise), estRoque(estRoque), enPassant(enPassant), couleur(couleur) {}


bool coup::operator==(const coup& other) const {
    return (x1 == other.x1 && y1 == other.y1 &&
        x2 == other.x2 && y2 == other.y2);
}

coup::coup(const std::string& notation) {
    if (notation.length() != 4) {
        x1 = y1 = x2 = y2 = -1; // Coup invalide
        return;
    }
    x1 = notation[0] - 'a';
    y1 = notation[1] - '1';
    x2 = notation[2] - 'a';
    y2 = notation[3] - '1';
}

string coup::traduireCoup(coup coup) {
    char couleur = coup.GetCouleur() ? 'W' : 'B';
    string coupStr = couleur + string(1, 'a' + coup.x1) + to_string(coup.y1 + 1) + string(1, 'a' + coup.x2) + to_string(coup.y2 + 1);

    return coupStr;
}

coup coup::traduireCoup(string coupStr) {
    if (coupStr.length() != 5) {
        return coup(-1, -1, -1, -1, piece(), piece(), false, false, false);
    }

    // Initialiser les coordonnées du coup
    int x1 = coupStr[1] - 'a';
    int y1 = coupStr[2] - '1';
    int x2 = coupStr[3] - 'a';
    int y2 = coupStr[4] - '1';

    // Déterminer la couleur du joueur
    bool couleur = (coupStr[0] == 'W');

    // Créer et retourner l'objet coup
    return coup(x1, y1, x2, y2, piece(), piece(), false, false, couleur);
}
