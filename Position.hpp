#ifndef POSITION_HPP
#define POSITION_HPP

#include <vector>
#include <memory>
#include "Echiquier.hpp"
#include "coup.hpp"

/*
   Classe abstraite Position imposée par le sujet,
   avec :
     - un pointeur sur la position soeur,
     - un pointeur sur la première position fille,
     - un champ 'joueur' (0 ou 1),
     - la fonction getValeur() (ici, on utilisera heuristique() ou semblable),
     - la fonction genererEnfants().
*/

// ===================== position_generale =====================
class Position
{
protected:
    
    unique_ptr<Position> soeur;     // Pointeur sur la position soeur (même niveau)
    unique_ptr<Position> fille;     // Pointeur sur la première position fille
    bool maximise;       // Indique qui doit jouer (Vrai pour IA et Faux pour Humain)
    int depth;           // Profondeur
    vector<coup> coups_successifs;    // Les coups joués pour atteindre cette position à partir de l'échiquier de référence.
    int joueur_init;

public:
    // Constructeur
    Position(vector<coup> coups_successifs): soeur(nullptr), fille(nullptr), maximise(true), depth(0), coups_successifs(coups_successifs), joueur_init(1){};

    // Destructeur virtuel
    virtual ~Position() = default;  // Plus besoin de gérer delete

    // Pointeur soeur
    Position* GetSoeur() const;
    void SetSoeur(unique_ptr<Position> s);

    // Pointeur fille
    Position* GetFille() const;
    void SetFille(unique_ptr<Position> f);

    // Maximise (True pour IA et False pour Humain)
    bool GetMaximise() const;
    void SetMaximise(bool m);

    // Depth position
    int GetDepth() const;
    void SetDepth(int d);
    
    // Vecteurs des coups successifs
    vector<coup> GetCoups_successifs() const;
    void SetCoups_successifs(coup coup) ;
    
    //Joueur_initial, celui qui use la fonction meilleurcoup
    int GetJoueur_init() const;
    void SetJoueur_init(int j);
    
    
    // Echiquier
    virtual void MakeEchiquier(Plateau& Board_ref) = 0;
    virtual void UndoEchiquier(Plateau& Board_ref) = 0;

    // Fonctions
    virtual float heuristique(Plateau& Board_ref, char symboleIA) = 0;
    virtual bool genererFilles(Plateau& Board_ref, char symboleIA) = 0;

    // Méthodes minimax et meilleurCoup()
    float minimax(Plateau& Board_ref, char symboleIA);
    float alphaBeta(Plateau& Board_ref, float alpha, float beta, char symboleIA );
    Position* meilleurCoup(Plateau& Board_ref, bool estalpha, char symboleIA );
    
    // Openings
    coup getCoupJoue() const;
    
};




// ===================== position_tictactoe =====================
class position_tictactoe : public Position
{
public:
    position_tictactoe(vector<coup> coups_successifs):Position(coups_successifs){};
    
    // Méthode utilitaire
    vector<coup> movepossible(Plateau& Board);
    
    bool genererFilles(Plateau& Board_ref, char symboleIA = 'O') override;
    float heuristique(Plateau& Board_ref, char symboleIA ) override;
    
    void MakeEchiquier(Plateau& Board_ref) override;
    void UndoEchiquier(Plateau& Board_ref) override;
    
};


// ===================== position_echec =====================
class position_echec : public Position
{
public:
    position_echec(vector<coup> coups_successifs):Position(coups_successifs){};;
    
    // Méthode utilitaire
    vector<coup> movepossible(Plateau& Board_ref, piece p);
    
    bool genererFilles(Plateau& Board_ref, char symboleIA) override;
    float heuristique(Plateau& Board_ref, char symboleIA ) override;
    
    void MakeEchiquier(Plateau& Board_ref) override;
    void UndoEchiquier(Plateau& Board_ref) override;

};



#endif

