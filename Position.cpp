#include "Position.hpp"
#include "Echiquier.hpp"
#include "Piece.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <cassert>
#include <random>
#include "openings.hpp"
using namespace std;
 


/*################################################################################*/
/*################################################################################*/
/*################################################################################*/
/*FONCTIONS GENERALES DE LA CLASSE POSITION*/

Position* Position::GetSoeur() const { return soeur.get(); }
void Position::SetSoeur(unique_ptr<Position> s) { soeur = std::move(s); }


Position* Position::GetFille() const { return fille.get(); }
void Position::SetFille(unique_ptr<Position> f) { fille = std::move(f);  }


bool Position::GetMaximise() const { return maximise; }
void Position::SetMaximise(bool m) { maximise = m; }


int Position::GetDepth() const { return depth; }
void Position::SetDepth(int d) { depth = d; }

vector<coup> Position::GetCoups_successifs() const { return coups_successifs ;}
void Position::SetCoups_successifs(coup coup) { coups_successifs.push_back(coup);}

int Position::GetJoueur_init() const {return joueur_init ;}
void  Position::SetJoueur_init(int j) { joueur_init = j ;}


int getDepthMax( TypeJeu type_jeu) {
    int depth_tic = 10;
    int depth_Echec = 2;
    return (type_jeu == TypeJeu::TIC_TAC_TOE) ? depth_tic : depth_Echec;
}


float Position::minimax(Plateau& Board_ref, char symboleIA) {
    
    //std::cout << "\n\nMinimax : depth = " << depth << ", Maximise = " << maximise << std::endl;
    //std::cout<<"La fille est : "<<endl;
    //Board_ref.afficher();
    
    //evaluergagnant(Board_ref, symboleIA) ;  // Pour evaluer au cas ou il y'a un gagnant sinon continue (juste pour factoriser le code);
    
    // Vérifier qui a gagné
    MakeEchiquier(Board_ref) ;
    char gagnant = Board_ref.verifierGagnant();
    UndoEchiquier(Board_ref);

    // Vérifier si c'est une victoire de l'IA
    if (gagnant == symboleIA) return INFINITY;

    // Vérifier si c'est une défaite (le joueur humain a gagné)
    char SymboleH = Board_ref.getSymboleH(symboleIA); // Récupère le symbole de l'humain ou autre IA selon cas
    if (gagnant == SymboleH) return -INFINITY;

    // Vérifier si c'est un match nul
    if (gagnant == 'V') return 0;

    
    // Condition d'arrêt (profondeur maximale)
    if (depth >= getDepthMax(Board_ref.getTypePlateau())) {
        //std::cout << "\nProfondeur maximale atteinte" << std::endl;
        return heuristique(Board_ref, symboleIA);
    }
    
    
    // Condition d'arrêt (aucun coup possible)
    if (!genererFilles(Board_ref, symboleIA)) {    // Generer les positions enfants
        //std::cout << "Aucun coup possible à cette profondeur dans minimax" << std::endl;
        return heuristique(Board_ref, symboleIA);  // Match nul si aucun coup possible
    }
    
    
    float BestVal = maximise ? -INFINITY : INFINITY;  // Le joueur Max commence avec -∞ et Min commence à +∞
    
    for (Position* enfant = GetFille(); enfant != nullptr; enfant = enfant->GetSoeur()) {
        float valeur = enfant->minimax(Board_ref, symboleIA);
    
        if (this->maximise)
        {
            BestVal = max(BestVal, valeur);
        }
        else
        {
            BestVal = min(BestVal, valeur);
        }
    }
    return BestVal;

}



float Position::alphaBeta(Plateau& Board_ref, float alpha, float beta, char symboleIA) {
    
    //evaluergagnant(Board_ref, symboleIA) ;  // Pour evaluer au cas ou il y'a un gagnant sinon continue (juste pour factoriser le code);
    
    // Vérifier qui a gagné
    MakeEchiquier(Board_ref) ;
    char gagnant = Board_ref.verifierGagnant();
    UndoEchiquier(Board_ref);

    // Vérifier si c'est une victoire de l'IA
    if (gagnant == symboleIA) return INFINITY;

    // Vérifier si c'est une défaite (le joueur humain a gagné)
    char SymboleH = Board_ref.getSymboleH(symboleIA); // Récupère le symbole de l'humain ou autre IA selon cas
    if (gagnant == SymboleH) return -INFINITY;

    // Vérifier si c'est un match nul
    if (gagnant == 'V') return 0;

    
    if (depth >= getDepthMax(Board_ref.getTypePlateau())) {
        return heuristique(Board_ref, symboleIA); // Profondeur max atteinte
    }

    // Génération des positions enfants
    if (!genererFilles(Board_ref, symboleIA)) { return heuristique(Board_ref, symboleIA); }

    float bestValue = this->maximise ? -INFINITY : INFINITY;

    for (Position* enfant = GetFille(); enfant != nullptr; enfant = enfant->GetSoeur()) {
        float valeur = enfant->alphaBeta(Board_ref, alpha, beta, symboleIA);

        if (this->maximise) { // Cas du joueur Max
            
            if (valeur > bestValue) {
                bestValue = valeur;
                alpha = max(alpha, valeur);
            }
            if (valeur >= beta) { // Coupure beta
                //cout<<"On a fait un élagage beta"<<endl;
                return valeur;
            }
        } else { // Cas du joueur Min
            
            if (valeur < bestValue) {
                bestValue = valeur;
                beta = min(beta, valeur);
            }
            if (valeur <= alpha) { // Coupure alpha
                //cout<<"On a fait un élagage alpha"<<endl;
                return valeur;
            }
        }
    }

    return bestValue;
}




Position* Position::meilleurCoup(Plateau& Board_ref, bool estalpha, char symboleIA) {
    
    bool result = genererFilles(Board_ref, symboleIA);        // Generer les positions enfants
    
    if (!result) {
        //cout << "ERREUR: Aucun coup possible dans meilleurcoup! (genererEnfants a échoué)" << endl;
        return nullptr;
    }
    
    float BestVal = -INFINITY;
    Position* BestMove = nullptr;         // Retourner le meilleur coup
    vector<Position*> bestMoves = {};     // Stocker tous les meilleurs coups de meme valeur
    
    for (Position* enfant = GetFille(); enfant != nullptr; enfant = enfant->GetSoeur()) {
        
        float valeur = 0;
        
        if (estalpha) {
            //cout<<"\nUtilisation de alpha beta"<<endl;
            valeur = enfant->alphaBeta(Board_ref, -INFINITY, INFINITY, symboleIA) ;
        } else {
            //cout<<"\nUtilisation de minimax"<<endl;
            valeur = enfant->minimax(Board_ref, symboleIA);
        }
        
        //cout<<"\nLe maximise de enfant est :  " <<enfant->maximise<<endl;
        //cout<<"La valeur de cette branche est : "<<valeur<<" \n\n\n\n"<<endl;
        
        if (valeur > BestVal) {
            BestVal = valeur;
            bestMoves.clear(); // Réinitialiser la liste des meilleurs coups en cas d'un plus grand strict
            bestMoves.push_back(enfant);
        } else if (valeur == BestVal) {
           bestMoves.push_back(enfant); // Ajouter ce coup à la liste des meilleurs
        }
    }
    
    // Choisir un coup au hasard parmi les meilleurs ( on pourra ameliorer ça après si on a le temps)
    if (!bestMoves.empty()) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(bestMoves.begin(), bestMoves.end(), g);
        BestMove = std::move(bestMoves[0]);
    }
         
    
    return BestMove;
}




coup Position::getCoupJoue() const {
    if (coups_successifs.empty()) {
        return coup(-1, -1, -1, -1); // Retourne un coup invalide si aucun coup n'a été joué
    }
    return coups_successifs.back();
}




/*################################################################################*/
/*################################################################################*/
/*################################################################################*/
/*FONCTIONS TIC_TAC_TOE*/



vector<coup> position_tictactoe::movepossible(Plateau& Board_ref) {  // "Matrice des coups possibles pour  une piece donnée
    
    vector<coup> coups ;
    MakeEchiquier(Board_ref) ;
    vector<vector<piece>> grille = Board_ref.get_grille() ;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grille[i][j].estNull()) // Vérifie si la case est vide
            {
                coup move = coup(-1, -1, i, j, piece(), piece() , false , false, !maximise) ;
                coups.push_back(move);
            }
        }
    }
    UndoEchiquier(Board_ref) ;
    if (!coups.empty()) {
        return coups;
    } else {
        std::cerr << "Erreur: coups est vide dans movepossible_tic!" << std::endl;
        return {} ;
    }
}





bool position_tictactoe::genererFilles(Plateau& Board_ref, char symboleIA)
{
    // Récupérer la liste des coups possibles
    vector<coup> coups = this->movepossible(Board_ref);
    
    // Vérifier s'il y a des coups possibles
    if (coups.empty()) return false;

    Position* derniereFille = nullptr; // Pointeur pour chaîner les soeurs

    // Parcourir tous les coups possibles
    for (auto & cp : coups)
    {
        // Créer la nouvelle position correspondant à ce coup
        auto nouvellePosition = std::make_unique<position_tictactoe>(this->coups_successifs); // Cette ligne initialise aussi le vecteur coup_successifs ;
        nouvellePosition->SetMaximise(!this->GetMaximise()); // Changer le joueur actif
        nouvellePosition->SetDepth(this->GetDepth() + 1);
        nouvellePosition->SetCoups_successifs(cp) ;  // Inserer le dernier coup pour arriver à la position fille ;
        nouvellePosition->SetJoueur_init(this->joueur_init);
       
        
        // Chaînage : insérer la nouvelle position dans la liste fille → soeur
        if (!GetFille()) {
            SetFille(std::move(nouvellePosition));
            derniereFille = GetFille(); // Garder en mémoire la position fille pour lui associer sa soeur plus tard
        } else {
            derniereFille->SetSoeur(std::move(nouvellePosition));
            derniereFille = derniereFille->GetSoeur(); // Garder en mémoire la position soeur pour lui associer sa soeur à elle plus tard
        }
    }

    return true; // On a généré des enfants
}





float position_tictactoe::heuristique(Plateau& Board_ref, char symboleIA)
{
    MakeEchiquier(Board_ref) ;
    char gagnant = Board_ref.verifierGagnant();
    UndoEchiquier(Board_ref);
    
    char symboleH = Board_ref.getSymboleH(symboleIA) ;
    
    int score = 0;  // Égalité d'office
    if (gagnant == symboleIA) { score =  INFINITY ;  }         // Victoire
    else if (gagnant == symboleH) { score = -INFINITY ; }    // Perte
    else if (gagnant == 'V') { score = 0 ;    }    // Nul
    else {
        score += Board_ref.compterLignesPresqueCompletes(maximise)*10 ;     // Nombre de lignes presque gagnantes pour chaque joueur, pas défini à                                                                         l'avance mais
                                                                            // forcément pris en compte dans les deux cas et l'implémentation de la fonction
        score += Board_ref.compterLignesPresqueCompletes(!maximise)*10 ;
    }
                                                    
    return score ;
}



void position_tictactoe::MakeEchiquier(Plateau& Board_ref)
{
    for(auto &coup :  coups_successifs)
    {
        //cout<<"Le joueur avant coup pour : "<<coups_successifs.size()<<" coups est :"<<Board_ref.getJoueurActuel()<<endl;
        Board_ref.jouerCoup(coup);
        //cout<<"Le joueur après coup est :"<<Board_ref.getJoueurActuel()<<endl;
    }
}



void position_tictactoe::UndoEchiquier(Plateau& Board_ref) 
{
    vector<vector<piece>> grille = Board_ref.get_grille() ;
    
    if (coups_successifs.empty()) return; // Rien à annuler
    
    // Parcourir les coups dans l'ordre inverse pour annuler chaque coup
    for (auto coup = coups_successifs.rbegin(); coup != coups_successifs.rend(); ++coup)
    {
       grille[coup->x2][coup->y2] = piece() ; // Vider la case ou on avait joué un coup
    }
    Board_ref.set_grille(grille);
    
    // Restaurer le tour du joueur pour revenir au joueur_reel courant;
    Board_ref.setJoueurActuel(joueur_init);
    //string joueur_s =  (joueur_init==0) ? "HUMAIN" : "IA" ;
    //cout<<"Le joueur actuel dans Undo est :"<< joueur_s <<endl;
}














/*################################################################################*/
/*################################################################################*/
/*################################################################################*/
/*FONCTIONS ECHECS*/
 

vector<coup> position_echec::movepossible(Plateau& Board_ref, piece p) {
    
    type_pion t = p.getType() ;
    vector<vector<int>> moves ;    // "Matrice de tous les mouvements possibles de base pour la piece en argument                                                                                 independant de la configuration actuelle de l'echiquier
            
    switch (t)
    {
        case type_pion::ROI: moves = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1},
            {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
            }; break;
            
        case type_pion::REINE: moves = {
            // Déplacements verticaux (haut et bas)
            {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0},
            {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}, {-8, 0},

            // Déplacements horizontaux (gauche et droite)
            {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
            {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}, {0, -8},

            // Déplacements diagonaux
            {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8},
            {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}, {-8, -8},
            {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}, {8, -8},
            {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}, {-8, 8}
            }; break;
            
        case type_pion::TOUR: moves = {
            {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0},
            {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}, {-8, 0},
            {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8},
            {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}, {0, -8}
            }; break;
            
        case type_pion::FOU: moves = {
            {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8},
            {-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}, {-8, -8},
            {1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}, {8, -8},
            {-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}, {-8, 8}
            }; break;
            
        case type_pion::CAVALIER: moves = {
            {1, 2}, {1, -2}, {-1, 2}, {-1, -2},
            {2, 1}, {2, -1}, {-2, 1}, {-2, -1}
            }; break;
            
        case type_pion::PION:
            
            if (!maximise) {
                moves = {{-1, 0}, {-2, 0}, {-1, -1}, {-1, 1}};
            } else {
                moves = {{1, 0}, {2, 0}, {1, -1}, {1, 1}};
            }
            break;
            
        default:
            moves = {{1,1}} ; // Valeur par défaut pour un type inconnu ou null
            std::cerr << "Erreur: type_pion inconnu ou non pris en charge !" << std::endl; // Affiche une erreur si le type n'est pas reconnu
            break;
    }
    
    
    vector<coup> moves_real ;   // Vecteur de coups de type coup à retourner après vérificatiuon des coups valides.
    
    MakeEchiquier(Board_ref) ;  // Créer l'échiquier temporaire
    
    //Ajout des coups de roque eventuels si la pièce est un roi
    if (t == type_pion::ROI && p.getY() == 4) {
        // Vérifier que le roi est à sa position initiale
        int row = p.getX();
        
        // Vérifier si le petit roque est autorisé
        coup petitRoque(row, 4, row, 6, p, piece(), true, false, p.getCouleur());
        if (Board_ref.EstRoque(petitRoque)) {
            // Vérifier que le roque ne met pas le roi en échec
            Plateau temp = Board_ref;
            temp.jouerCoup(petitRoque);
            if (!temp.estEnEchec(p.getCouleur())) {
                moves_real.push_back(petitRoque);
            }
        }

        // Vérifier si le grand roque est autorisé
        coup grandRoque(row, 4, row, 2, p, piece(), true, false, p.getCouleur());
        if (Board_ref.EstRoque(grandRoque)) {
            // Vérifier que le roque ne met pas le roi en échec
            Plateau temp = Board_ref;
            temp.jouerCoup(grandRoque);
            if (!temp.estEnEchec(p.getCouleur())) {
                moves_real.push_back(grandRoque);
            }
        }
    }

    // Ajout des coups de prise en passant eventuels si la pièce est un pion
    if (t == type_pion::PION) {
        int row = p.getX();
        int col = p.getY();
        bool estBlanc = p.getCouleur();
        int direction = estBlanc ? -1 : 1; // Les Blancs avancent vers le haut, les Noirs vers le bas

        // Vérifier si une prise en passant est possible (gauche et droite)
        for (int dY : {-1, 1}) {  // Vérifie les deux côtés (gauche et droite)
            int xCible = row + direction; // Case d'arrivée
            int yCible = col + dY;        // Case cible en passant

            if (xCible >= 0 && xCible < 8 && yCible >= 0 && yCible < 8) {
                if (Board_ref.EstPriseEnPassant(coup(row, col, xCible, yCible, p, piece(), false, true, estBlanc))) {
                    // Ajout du coup de prise en passant
                    coup priseEnPassant(row, col, xCible, yCible, p, piece(), false, true, estBlanc);
                    moves_real.push_back(priseEnPassant);
                }
            }
        }
    }

   
    
    for (auto& move : moves ) {
        
        coup coup_a_jouer = coup(p.getX() ,p.getY() ,p.getX()+move[0] ,p.getY()+move[1], p, piece() , false , false, p.getCouleur() ) ;

       if (Board_ref.mouvementValide(p.getX(), p.getY(), p.getX() + move[0], p.getY() + move[1])) {       // Verifier les mouvements possibles dans la                                                                                                               configuration actuelle de l'echiquier
           piece piecePrise = Board_ref.get_grille()[p.getX() + move[0]][p.getY() + move[1]]; // Pour savoir s'il y'a une piece qui sera capturée
           coup_a_jouer = coup(p.getX() ,p.getY() ,p.getX()+move[0], p.getY()+move[1], p, piecePrise , false, false, p.getCouleur() );
           moves_real.push_back(coup_a_jouer) ;
        }
        
    }
    
    UndoEchiquier(Board_ref) ;    // Annuler les coups joués
    
    if (!moves_real.empty()) {
        return moves_real;
    } else {
        //std::cerr << "Erreur: moves_real est vide dans movepossible_echec !" << std::endl;
        return {} ;
    }
    
    
}





bool position_echec::genererFilles (Plateau& Board_ref, char symboleIA)
{
    MakeEchiquier(Board_ref) ;  // Créer l'échiquier temporaire
    
    vector<piece> pieces = Board_ref.getPieces() ;   // Recuperer un vecteur des pieces disponibles sur l'echiquier
    
    UndoEchiquier(Board_ref) ;    // Annuler les coups joués
    
    bool res = false ;
    int maxCoupsParPiece = 100;
    
    Position* derniereFille = nullptr; // Pointeur pour chaîner les soeurs
    
    // Fonction lambda pour déterminer la couleur du joueur actif pour l'IA
    auto couleurJoueurCourant = [symboleIA](bool maximise) -> bool {
        if (symboleIA == 'B') {
            return maximise ? true : false;  // Maximisation → Blanc, Minimisation → Noir
        } else { // symboleIA == 'N'
            return maximise ? false : true;  // Maximisation → Noir, Minimisation → Blanc
        }
    };

    for ( const auto &p : pieces) {                         // Boucle pour parcourir chaque piece et deduire les coups possibles pour chacune
        
        
        if ( p.getCouleur() == couleurJoueurCourant(GetMaximise()) ) {          // Test conditionnel pour traiter que les pieces du joueur actuel
            
            vector<coup> moves = movepossible(Board_ref,p) ;   // "Matrice" des moves possibles pour la pièce courante
           
    
            if (!moves.empty()) {
                
                res = true ;
                int nbCoups = 0;
                
                for (const auto& coup : moves)
                {
                   
                    if (nbCoups >= maxCoupsParPiece) break;  // Limite le nombre de coups par pieces
                    
                    // Créer la nouvelle position correspondant à ce coup
                    auto nouvellePosition = std::make_unique<position_echec>(this->coups_successifs);// Cette ligne initialise aussi le vecteur coup_successifs
                    nouvellePosition->SetMaximise(!this->GetMaximise()); // Changer le joueur actif
                    nouvellePosition->SetDepth(this->GetDepth() + 1);
                    nouvellePosition->SetCoups_successifs(coup) ;
                    nouvellePosition->SetJoueur_init(this->joueur_init);
                   
    
                    // Chaînage : insérer la nouvelle position dans la liste fille → soeur
                    if (!GetFille()) {
                        SetFille(std::move(nouvellePosition));
                        derniereFille = GetFille(); // Garder en mémoire la position fille pour lui associer sa soeur plus tard
                    } else {
                        derniereFille->SetSoeur(std::move(nouvellePosition));
                        derniereFille = derniereFille->GetSoeur(); // Garder en mémoire la position soeur pour lui associer sa soeur à elle plus tard
                    }
                    
                    // Mettre à jour le nombre de coups joués par pièce
                    nbCoups++;
                    
                }
            }
            
        }
    }
    

    if ( res==false ) {
        //std::cout << "Aucun coup possible dans genererEnfants_echec." << std::endl;
    }
    return res ;

}




float position_echec::heuristique(Plateau& Board_ref, char symboleIA) {
    
    MakeEchiquier(Board_ref) ;  // Créer l'échiquier temporaire
    char gagnant = Board_ref.verifierGagnant();
    //UndoEchiquier(Board_ref) ;  // Annuler les coups joués
    
    float score = 0 ;
    
    
    
    char symboleH = Board_ref.getSymboleH(symboleIA) ;
    
    if (gagnant == symboleIA) { score =  INFINITY ;  }         // Victoire
    else if (gagnant == symboleH) { score = -INFINITY ; }      // Perte
    else if (gagnant == 'V') { score = 0 ;   }                 //Nul
    else {
        
        int signe = (symboleIA=='N') ? 1 : -1;
        // Calcul des valeurs des pièces et du contrôle du plateau pour chaque joueur
        int valeur_joueur_B = 0;    // Valeur totale des pièces pour le joueur 'N'
        int valeur_joueur_N = 0;   // Valeur totale des pièces pour le joueur 'B'
        int controle_joueur_B = 0;  // Nombre total des pièces pour le joueur 'N'
        int controle_joueur_N = 0; // Nombre total des pièces pour le joueur 'B'
        int mobilite_B = 0 ;        // Nombre de mobilité des pièces pour le joueur 'N'
        int mobilite_N = 0;        // Nombre de mobilité des pièces pour le joueur 'B'
        int controle_centre_B = 0 ; // Nombre de centre pris par les pièces pour le joueur 'N'
        int controle_centre_N = 0; // Nombre de centre pris par les pièces pour le joueur 'B'
        
        
        // Coefficients pour l'heuristique (flottants pour plus de précision)
        float alpha = 0.7;  // Poids pour la valeur des pièces
        float beta = 0.3;   // Poids pour le contrôle du plateau
        float gama = 0.3;   // Poids pour la mobilité des pièces
        float delta = 0.2;  // Poids pour le contrôle du centre
        
        //MakeEchiquier(Board_ref) ;  // Créer l'échiquier temporaire
        vector<piece> pieces = Board_ref.getPieces() ;
        //UndoEchiquier(Board_ref) ;  // Annuler les coups joués
        
        for (auto& p : pieces) {   // Boucle pour computer le nombre et les valeurs des pieces de chaque joueur
            
            if(p.getCouleur()) {   // Joueur BLANC
                controle_joueur_B += 1 ;
                valeur_joueur_B += p.getValeur() ;
                mobilite_B += movepossible(Board_ref, p).size() ;
                if ((p.getX() == 3 || p.getX() == 4) && (p.getY() == 3 || p.getY() == 4)) {controle_centre_B++ ;}
            }
            else {                 // Joueur NOIR
                controle_joueur_N += 1 ;
                valeur_joueur_N += p.getValeur() ;
                mobilite_N += movepossible(Board_ref, p).size() ;
                if ((p.getX() == 3 || p.getX() == 4) && (p.getY() == 3 || p.getY() == 4)) {controle_centre_N++ ;}
            }
            
        }
        
        // alpha, beta, gama sont des poids pour pour priviligier sa strategie, on choisira les valeurs adéquates plus tard si on a le temps
        score = alpha * (valeur_joueur_N - valeur_joueur_B) +
                beta  * (controle_joueur_N - controle_joueur_B)  +
                gama  * (mobilite_N -   mobilite_B) +
                delta * (controle_centre_N - controle_centre_B) ;
        
        score*=signe;
        //cout << "L'heuristique de cette position terminale est : " << score << endl;
    }
    
    UndoEchiquier(Board_ref) ;  // Annuler les coups joués
    
    return score;
}





void position_echec::MakeEchiquier(Plateau& Board_ref)
{
    for(auto &coup :  coups_successifs)
    {
        Board_ref.jouerCoup(coup);
    }
}



void position_echec::UndoEchiquier(Plateau& Board_ref)
{
    if (coups_successifs.empty()) return; // Rien à annuler
    
    vector<vector<piece>> grille = Board_ref.get_grille() ;

    // Parcourir les coups en sens inverse pour annuler chaque coup
    for (auto coup = coups_successifs.rbegin(); coup != coups_successifs.rend(); ++coup)
    {
        int x1 = coup->x1;
        int y1 = coup->y1;
        int x2 = coup->x2;
        int y2 = coup->y2;
        
        // Restaurer la pièce déplacée
        grille[x1][y1] = coup->pieceJouee ;
        grille[x1][y1].setPosition(x1, y1);
        
        // Restaurer la pièce capturée s'il y en avait une
        if (!coup->piecePrise.estNull()) {
            grille[x2][y2] = coup->piecePrise;
            grille[x2][y2].setPosition(x2, y2);
        } else {
            grille[x2][y2] = piece(); // Vider la case si aucune pièce capturée
        }
    }
    
    // Restaurer le tour du joueur pour revenir au joueur_reel courant;
    Board_ref.set_grille(grille);
    // Restaurer le tour du joueur pour revenir au joueur_reel courant;
    Board_ref.setJoueurActuel(joueur_init);
    //string joueur_s =  (joueur_init==0) ? "HUMAIN" : "IA" ;
    //cout<<"Le joueur actuel est :"<< joueur_s <<endl;
}




