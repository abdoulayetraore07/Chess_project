#include <iostream>
#include <locale>
#include <iomanip>
#include <random>
#include <ctime>
#include "Echiquier.hpp"
#include "Position.hpp"



// ------------------------------------------------------
// 0) TEST IA ALPHA BETA OU MINIMAX CONTRE JOUEUR HUMAIN
// ------------------------------------------------------
int jeu_main() {
    // Pour l'affichage Unicode
    locale::global(std::locale("en_US.UTF-8"));
    wcout.imbue(std::locale("en_US.UTF-8"));

   
    cout << "Choisissez un jeu :\n";
    cout << "1. Tic-Tac-Toe (3x3)\n";
    cout << "2. Echecs (8x8)\n";
    cout << "Entrez votre choix (1 ou 2) : ";

    int choix;
    cin >> choix;
    if (choix < 1 || choix > 2)
    {
        cout << "Choix invalide. Veuillez relancer.\n";
        return 1;
    }

    int typeJeu = (choix == 1) ? 0 : 1;
    Plateau jeu(typeJeu);
    
    bool estalpha = true ;
    // Initialiser le générateur de nombres aléatoires
    srand(static_cast<unsigned int>(time(0)));
    
    //Changer le joueur qui commence la partie de maniere aleatoire
    jeu.setJoueurActuel(rand() % 2);
    
    cout << ((typeJeu == 0) ? "\n\n=== Jeu Tic-Tac-Toe ===\n"
                            : "\n\n                === Jeu d'Echecs ===\n");

    // -------------------------------------------------------------------------
    //                           CAS TIC-TAC-TOE
    // -------------------------------------------------------------------------
    if (typeJeu == 0)
    {
        
        while(true)
        {
            // Crée la position Tic-Tac-Toe
            
            vector<coup> coup_initial = {};
            position_tictactoe position(coup_initial);

            // Affiche le plateau
            jeu.afficher();

            // Tour du joueur X (0) ou O (1)
            if (jeu.getJoueurActuel() == 0)
            {
                cout << "\nJoueur X, entrez votre coup (ex: A1): ";
                char xx;
                int yy;
                cin >> xx >> yy;
                cin.clear();
                cin.ignore(10000, '\n');
                
                int x = toupper(xx) - 'A';
                int y = yy - 1;

                coup coupJoue(-1, -1, x, y);

                if (x < 0 || x >= 3 || y < 0 || y >= 3 || !jeu.jouerCoup(coupJoue))
                {
                    cout << "Coup invalide !\n";
                    continue;
                }
            }
            else
            {
                cout << "\nL'IA réfléchit à son tour...\n";
                // On calcule la meilleure position via minimax
                
                vector<coup> coups_possible = position.movepossible(jeu); // Liste de toutes les positions libres
                cout << "Coups possibles pour l'IA : " << coups_possible.size() << endl;

                // Affichage des coups possibles pour l'IA
                for (const auto& coup : coups_possible)
                {
                    cout << "  ➡️ (" << char('A' + coup.x2) << ", " << coup.y2 + 1 << ")";
                }
                
                Position* bestPos = position.meilleurCoup(jeu, estalpha, 'O');
                if(!bestPos)
                {
                    cout << "Aucun coup possible pour l'IA !\n";
                    break;
                }

                // Récupérer le premier coup joué dans la position
                const vector<coup>& coups = bestPos->GetCoups_successifs();
                if (!coups.empty())
                {
                    coup FirstMove = coups.front();
                    cout << "\n\nL'IA joue : (" << char('A' + FirstMove.x2)
                         << (FirstMove.y2 + 1) << ")\n";
                    jeu.jouerCoup(FirstMove); // On applique directement le coup
                }
                else
                {
                    cout << "Problème d'application du coup IA !\n";
                }
            }

            // Vérifie gagnant
            char g = jeu.verifierGagnant();
            if (g != ' ')
            {
                jeu.afficher();
                if(g=='X') cout<<"Vous avez gagné, bravo !\n";
                else if(g=='O') cout<<"Game Over, IA a gagné !\n";
                else if(g=='V') cout<<"Match nul !\n";
                break;
            }
        }
    }

    else
    {
        bool estEnOuverture = true;
        // ---------------------------------------------------------------------
        //                           CAS ÉCHECS
        // ---------------------------------------------------------------------
        
        jeu.afficher();
        while(true)
        {

            // Tour du joueur (Humain=0 et IA=1)
            if(jeu.getJoueurActuel()==0)
            {
                cout << "\nJoueur Humain, entrez votre coup (ex: A2 A4, Rp, Rg): ";
                string src, dst;
                cin >> src;

                // Gère la saisie roque
                if (src == "Rp" || src == "Rg")
                {
                    bool petit = (src == "Rp");
                    coup roqueCoup(7, 4, 7, petit?6:2,  // x1,y1,x2,y2
                                   piece(type_pion::ROI, true, 7,4), piece(),
                                   true, false, true);

                    if(!jeu.jouerRoque(roqueCoup))
                    {
                        cout << "Roque invalide.\n";
                    }
                    else {
                        jeu.ajouterCoup(roqueCoup);
                    }
                }
                else // Coup normal
                {
                    cin >> dst;
                    cin.clear();
                    cin.ignore(10000, '\n');
                    if(src.size() != 2 || dst.size() != 2)
                    {
                        cout << "Format invalide.\n";
                        continue;
                    }

                    int x1 = toupper(src[0]) - 'A';
                    int y1 = (src[1] - '1');
                    int x2 = toupper(dst[0]) - 'A';
                    int y2 = (dst[1] - '1');

                    // Il faut passer la piece qui se déplace.
                    piece pieceJouee = jeu.get_grille()[x1][y1];
                    // On ignore la piece prise pour le moment.
                    piece piecePrise = jeu.get_grille()[x2][y2];

                    coup coupHumain(x1, y1, x2, y2, pieceJouee, piecePrise,
                                    false, false, pieceJouee.getCouleur());

                    if(!jeu.jouerCoup(coupHumain))
                    {
                        cout<<"Coup invalide.\n";
                        continue;
                    }
                    else {
                        jeu.ajouterCoup(coupHumain);
                    }
                }
                jeu.afficher();
            }
            else
            {
                cout << "\nL'IA réfléchit...\n";
                
                Openings openings = jeu.getOpenings();
                vector<string> historique = jeu.getHistoriqueCoups();
                
                coup iaCoup;
                
                // Vérifier si on est encore en phase d'ouverture
                if (estEnOuverture) {
                    iaCoup = openings.getOpeningMove(historique);

                    // Vérifier la validité du coup d'ouverture
                    if (!jeu.estCoupValide(iaCoup)) {
                        cout<<"\nL'IA passe au jeu avec alpha-beta"<<endl;
                        estEnOuverture = false;
                        iaCoup = coup(); // Coup invalide pour forcer le Min-Max
                    }
                    else{
                        cout<<"\nL'IA joue une ouverture"<<endl;
                    }
                }
                
                // Si le coup d'ouverture est invalide ou si on est hors phase d'ouverture, utiliser Min-Max
                if (!estEnOuverture || iaCoup.x1 == -1) {
                    
                    vector<coup> coup_initial = {};
                    position_echec position(coup_initial);
                    
                    coup dernier_coup = jeu.getderniercoup();
                    Position* bestPos = position.meilleurCoup(jeu, estalpha, 'N');
                    jeu.setDernierCoup(dernier_coup);
                    
                    if(!bestPos)
                    {
                        cout << "Aucun coup IA !\n";
                        break;
                    }

                    const vector<coup>& coups = bestPos->GetCoups_successifs();
                   
                    if(!coups.empty())
                    {
                        iaCoup = coups.front();
                    }
                    else
                    {
                        cout << "Problème coup IA.\n";
                    }
                }
                
                cout << "L'IA joue : " << char('A'+iaCoup.x1) << (iaCoup.y1+1)
                     << " -> " << char('A'+iaCoup.x2) << (iaCoup.y2+1) << endl;

                // On applique ce coup
                jeu.jouerCoup(iaCoup);
                jeu.ajouterCoup(iaCoup);
                
                jeu.afficher();
                
            }

            char g = jeu.verifierGagnant();
            if(g!=' ')
            {
                jeu.afficher();
                if(g=='B') cout<<"Vous avez gagné, bravo !\n";
                else if(g=='N') cout<<"Oups, IA a gagné !\n";
                else if(g=='V') cout<<"Match nul !\n";
                break;
            }
        }
    }

    cout << "\n\nMerci d'avoir joué !\n";
    return 0;
}











// ------------------------------------------------------
// 1) TEST BASIQUES IA AVEC ALPHA BETA
// ------------------------------------------------------

// Fonction utilitaire pour formater le résultat d'un test
std::string formatResult(const std::string& testName, bool success) {
    return testName + " : " + (success ? "Succès" : "Échec");
}


void BasicTests_TIC() {
    std::vector<std::string> results;
    // Pour l'affichage Unicode
    locale::global(std::locale("en_US.UTF-8"));
    wcout.imbue(std::locale("en_US.UTF-8"));
    // ================================
    // Tests pour le Tic-Tac-Toe
    // ================================
    {
        // Test 1 : Coup invalide – tenter de jouer sur une case déjà occupée
        Plateau jeu1(0); // 0 pour Tic-Tac-Toe
        jeu1.setJoueurActuel(0);
        bool coup1 = jeu1.jouerCoup(coup(-1, -1, 0, 0));  // coup valide en A1 (cellule (0,0))
        bool coup2 = jeu1.jouerCoup(coup(-1, -1, 0, 0));  // même case, doit être rejeté
        results.push_back(formatResult("TicTacToe - Coup Invalide (case occupée)", coup1 && !coup2));
        
        {
            // Test 2 : Coup gagnant forcé via alpha beta – X a deux cases alignées et doit jouer en A3 pour gagner
            Plateau jeu2(0);
            jeu2.setJoueurActuel(0);
            // Simuler : X en A1, O en B1, X en A2, O en B2
            jeu2.jouerCoup(coup(-1, -1, 0, 0));  // X en A1
            jeu2.jouerCoup(coup(-1, -1, 1, 0));  // O en B1
            jeu2.jouerCoup(coup(-1, -1, 0, 1));  // X en A2
            jeu2.jouerCoup(coup(-1, -1, 1, 1));  // O en B2
            // Il ne reste plus qu'à jouer en A3 (cellule (0,2)) pour gagner
            position_tictactoe pos({});
            Position* bestPos = pos.meilleurCoup(jeu2, true, 'X');
            bool test2 = false;
            if (bestPos != nullptr) {
                const std::vector<coup>& coups = bestPos->GetCoups_successifs();
                if (!coups.empty()) {
                    coup move = coups.front();
                    test2 = (move.x2 == 0 && move.y2 == 2);
                }
            }
            results.push_back(formatResult("TicTacToe - Coup Gagnant via Alpha-beta", test2));
        }
        
        {
            // Test 2 : Coup gagnant forcé via alpha beta – X a deux cases alignées et doit jouer en A3 pour gagner
            Plateau jeu2(0);
            jeu2.setJoueurActuel(0);
            // Simuler : X en A1, O en B1, X en A2, O en B2
            jeu2.jouerCoup(coup(-1, -1, 0, 0));  // X en A1
            jeu2.jouerCoup(coup(-1, -1, 1, 2));  // O en B1
            jeu2.jouerCoup(coup(-1, -1, 1, 1));  // X en A2
            jeu2.jouerCoup(coup(-1, -1, 2, 2));  // O en B2
            //cout<<"le joueur actuel est: "<<jeu2.getJoueurActuel()<<endl;
            //jeu2.jouerCoup(coup(-1, -1, 1, 1));  // O en B2
            // Il ne reste plus qu'à jouer en A3 (cellule (0,2)) pour bloquer le coup

            position_tictactoe pos({});
            pos.SetJoueur_init(0);
            Position* bestPos = pos.meilleurCoup(jeu2, true, 'X');
            //cout<<"le joueur actuel est: "<<jeu2.getJoueurActuel()<<endl;
            bool test2 = false;
            if (bestPos != nullptr) {
                //cout<<"Vide 1"<<endl;
                const vector<coup>& coups = bestPos->GetCoups_successifs();
                if (!coups.empty()) {
                    //cout<<"Vide 2"<<endl;
                    coup move = coups.front();
                    jeu2.jouerCoup(move);
                    test2 = (move.x2 == 0 && move.y2 == 2);
                    //jeu2.afficher();
                }
            }
            results.push_back(formatResult("TicTacToe - Coup bloquant via Alpha-beta", test2));
        }
        
        // Test 4 : Match nul – remplir le plateau sans qu'aucun joueur ne gagne
        Plateau jeu4(0);
        jeu4.setJoueurActuel(0);
        // Remplissage manuel (exemple) :
        // Ligne 1 : X, O, X
        jeu4.jouerCoup(coup(-1, -1, 0, 0)); // X en A1
        jeu4.jouerCoup(coup(-1, -1, 1, 1)); // O en A2
        jeu4.jouerCoup(coup(-1, -1, 0, 1)); // X en A3
        // Ligne 2 : O, X, O
        jeu4.jouerCoup(coup(-1, -1, 0, 2)); // O en B1
        jeu4.jouerCoup(coup(-1, -1, 2, 0)); // X en B2
        jeu4.jouerCoup(coup(-1, -1, 1, 0)); // O en B3
        // Ligne 3 : X, O, X
        jeu4.jouerCoup(coup(-1, -1, 1, 2)); // X en C1
        jeu4.jouerCoup(coup(-1, -1, 2, 2)); // O en C2
        jeu4.jouerCoup(coup(-1, -1, 2, 1)); // X en C3
        char gagnant4 = jeu4.verifierGagnant();
        bool test4 = (gagnant4 == 'V'); // Supposé que 'V' indique match nul
        results.push_back(formatResult("TicTacToe - Match Nul", test4));
    }
    
    // Affichage final des résultats
    std::cout << "\n==== Résultats des tests automatisés pour le Tic-Tac-Toe ====\n" << std::endl;
    for (const auto &ligne : results) {
        std::cout << ligne << std::endl;
    }
    cout<<"\n"<<endl;
    
}



    
    
   

// ------------------------------------------------------
// 2) TEST IA ALPHABETA OU MINIMAX CONTRE JOUEUR ALEATOIRE
// ------------------------------------------------------
void jouerCoupAleatoire(Plateau& jeu)
{
    // Liste des cases vides (coups possibles)
    vector<coup> coupsPossibles;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(jeu.get_grille()[i][j].estNull()) // Case vide
            {
                coupsPossibles.push_back(coup(-1, -1, i, j)); // Coup tic-tac-toe
            }
        }
    }

    if(!coupsPossibles.empty())
    {
        // Sélection aléatoire
        static random_device rd;
        static mt19937 g(rd());
        uniform_int_distribution<int> distrib(0, int(coupsPossibles.size()-1));
        int index = distrib(g);

        // Appliquer le coup sélectionné
        jeu.jouerCoup(coupsPossibles[index]);
    }
}


void testerIAContreAleatoire(int nombreParties = 100)
{
    int victoiresIA = 0;
    int victoiresRandom = 0;
    int matchsNuls   = 0;
    bool estalpha = true ;

    srand(static_cast<unsigned>(time(nullptr))); // Init random

    for(int n = 0; n < nombreParties; n++)
    {
        if ((n+1)%(nombreParties/5) == 0 ) { cout<<"\nParties : " << n+1 << " /"<<nombreParties<<" terminées"<<endl; }
        // Création du plateau de Tic-Tac-Toe
        Plateau jeu(0); // "0" => type Tic-Tac-Toe
        jeu.setJoueurActuel(rand() % 2); // Choix aléatoire du premier joueur
        
        // Boucle de jeu
        while(true)
        {
            // Crée une position Tic-Tac-Toe
            vector<coup> coup_initial;
            position_tictactoe position(coup_initial);

            int joueurActuel = jeu.getJoueurActuel();

            // === CASE 1 : Tour de l’IA ===
            if(joueurActuel == 1)
            {
                //cout << "\nL'IA réfléchit...\n";

                // Calcul du meilleur coup via minimax
                Position* bestPos = position.meilleurCoup(jeu, estalpha, 'O');
                if(!bestPos)
                {
                    //cout << "Aucun coup possible pour l'IA !\n";
                    break;
                }

                // Récupérer le premier coup joué dans la position
                const vector<coup>& coups = bestPos->GetCoups_successifs();
                if (!coups.empty())
                {
                    coup FirstMove = coups.front();
                    /*cout << "\n\nL'IA joue : (" << char('A' + FirstMove.x2)
                         << (FirstMove.y2 + 1) << ")\n";*/
                    jeu.jouerCoup(FirstMove); // On applique directement le coup
                }
                else
                {
                    //cout << "Problème d'application du coup IA !\n";
                }
            }
            // === CASE 2 : Tour du joueur aléatoire ===
            else
            {
                jouerCoupAleatoire(jeu);
            }

            // Vérifier s'il y a un gagnant
            char g = jeu.verifierGagnant();
            if(g == 'X')
            {
                victoiresRandom++;
                break;
            }
            else if(g == 'O')
            {
                victoiresIA++;
                break;
            }
            else if(g == 'V') // match nul
            {
                matchsNuls++;
                break;
            }
        }
    }

    // Affichage des statistiques finales
    cout << "\n\n----------------------------------------\n";
    cout << "  Résultats après " << nombreParties << " parties (IA vs Random)\n";
    cout << "----------------------------------------\n";
    cout << "Victoires IA (O) : " << victoiresIA
         << " (" << fixed << setprecision(2)
         << (100.0*victoiresIA/nombreParties) << "%)\n";
    cout << "Victoires Random (X) : " << victoiresRandom
         << " (" << fixed << setprecision(2)
         << (100.0*victoiresRandom/nombreParties) << "%)\n";
    cout << "Matchs nuls : " << matchsNuls
         << " (" << fixed << setprecision(2)
         << (100.0*matchsNuls/nombreParties) << "%)\n";
}




// ------------------------------------------------------
// 3) TEST IA MINIMAX CONTRE LUI-MEME
// ------------------------------------------------------
void testerIAcontreIA(int nombreParties = 100)
{
    int victoiresIA = 0;
    int matchsNuls = 0;
    
    // Pour l'affichage Unicode
    locale::global(std::locale("en_US.UTF-8"));
    wcout.imbue(std::locale("en_US.UTF-8"));

    for (int n = 0; n < nombreParties; n++)
    {
        if ((n+1)%(nombreParties/5) == 0 ) { cout<<"\nParties : " << n+1 << " /"<<nombreParties<<" terminées"<<endl; }
        Plateau jeu(0);
        jeu.setJoueurActuel(rand()%2); // L'un ou l'autre commence de façon aléatoire

        while (true)
        {
            

            /*
            char symboleIA = 'O';
            if (jeu.getJoueurActuel()==0) { symboleIA = 'X';}
            jeu.afficher();
             */
            
            if(jeu.getJoueurActuel() == 0)
            {
                //cout<<"Le symbole est : "<<symboleIA<<" et le joueur actuel : "<<jeu.getJoueurActuel()<<endl;
                // Crée une position Tic-Tac-Toe
                vector<coup> coup_initial = {};
                position_tictactoe position(coup_initial);
               
                position.SetJoueur_init(0);
                //cout << "\nL'IA réfléchit...\n";

                // Calcul du meilleur coup via minimax
                Position* bestPos = position.meilleurCoup(jeu, true, 'X');
                if(!bestPos)
                {
                    //cout << "Aucun coup possible pour l'IA !\n";
                    break;
                }
                
                // Récupérer le premier coup joué dans la position
                const vector<coup>& coups = bestPos->GetCoups_successifs();
                if (!coups.empty())
                {
                    coup FirstMove = coups.front();
                    /*cout << "\n\nL'IA joue : (" << char('A' + FirstMove.x2)
                         << (FirstMove.y2 + 1) << ")\n";*/
                    jeu.jouerCoup(FirstMove); // On applique directement le coup
                }
            }
            // === CASE 2 : Tour du joueur aléatoire ===
            else
            {
                //cout<<"Le symbole est : "<<symboleIA<<" et le joueur actuel : "<<jeu.getJoueurActuel()<<endl;
                // Crée une position Tic-Tac-Toe
                vector<coup> coup_initial = {};
                position_tictactoe position(coup_initial);
                
                //cout << "\nL'IA réfléchit...\n";

                // Calcul du meilleur coup via minimax
                Position* bestPos = position.meilleurCoup(jeu, true, 'O');
                if(!bestPos)
                {
                    //cout << "Aucun coup possible pour l'IA !\n";
                    break;
                }

                // Récupérer le premier coup joué dans la position
                const vector<coup>& coups = bestPos->GetCoups_successifs();
                if (!coups.empty())
                {
                    coup FirstMove = coups.front();
                    /*cout << "\n\nL'IA joue : (" << char('A' + FirstMove.x2)
                         << (FirstMove.y2 + 1) << ")\n";*/
                    jeu.jouerCoup(FirstMove); // On applique directement le coup
                }
            }
            
           

            char g = jeu.verifierGagnant();
            if (g == 'O' || g == 'X')
            {
                victoiresIA++;
                //jeu.afficher();
                break;
            }
            else if (g == 'V')
            {
                matchsNuls++;
                //jeu.afficher();
                break;
            }
        }
    }
    cout << "\n\n----------------------------------------\n";
    cout << "  Résultats après " << nombreParties << " parties (IA vs IA)\n";
    cout << "----------------------------------------\n";
    cout << "Victoires IA : " << victoiresIA << "\n";
    cout << "Matchs nuls : " << matchsNuls << "\n";
}










// ------------------------------------------------------
// 4) TEST IA ECHECS TEST BASIQUES
// ------------------------------------------------------

void BasicTests_CHESS() {
    // ================================
    // Tests pour les Échecs
    // ================================
    std::vector<std::string> results;
    // Pour l'affichage Unicode
    locale::global(std::locale("en_US.UTF-8"));
    wcout.imbue(std::locale("en_US.UTF-8"));
    
    {
        // Test 4 : Petit roque valide pour les blancs
        Plateau jeu4(1); // 1 pour Échecs
        std::vector<std::vector<piece>> grille4 = jeu4.get_grille();
        // Pour le petit roque, il faut que les cases entre le roi (7,4) et la tour (7,7) soient vides
        grille4[7][5] = piece();
        grille4[7][6] = piece();
        jeu4.set_grille(grille4);
        coup roqueCoup(7, 4, 7, 6, grille4[7][4], piece(), true, false, true);
        bool test4 = jeu4.jouerCoup(roqueCoup);
        results.push_back(formatResult("Échecs - Petit Roque Valide", test4));

        
        // Test pour le grand roque valide pour les blancs
        Plateau jeuGrand(1); // 1 pour Échecs
        std::vector<std::vector<piece>> grilleGrand = jeuGrand.get_grille();
        // Pour le grand roque, il faut que les cases entre le roi (7,4) et la tour (7,0) soient vides,
        grilleGrand[7][1] = piece();
        grilleGrand[7][2] = piece();
        grilleGrand[7][3] = piece();
        jeuGrand.set_grille(grilleGrand);
        // Construction du coup de grand roque : le roi se déplace de (7,4) vers (7,2)
        coup roqueGrand(7, 4, 7, 2, jeuGrand.get_grille()[7][4], piece(), true, false, true);
        bool testGrand = jeuGrand.jouerCoup(roqueGrand);
        results.push_back(formatResult("Échecs - Grand Roque Valide", testGrand));
        
        
        // Test 5 : Roque invalide – chemin bloqué par une pièce (ex : un pion sur (7,5))
        Plateau jeu5(1);
        std::vector<std::vector<piece>> grille5 = jeu5.get_grille();
        grille5[7][5] = piece(type_pion::PION, true, 7, 5); // pièce bloquante
        jeu5.set_grille(grille5);
        coup roqueCoupInvalid(7, 4, 7, 6, grille5[7][4], piece(), true, false, true);
        bool test5 = !jeu5.jouerCoup(roqueCoupInvalid);
        results.push_back(formatResult("Échecs - Roque Invalide (chemin bloqué)", test5));
        
        
        // Test 6 : Prise en passant valide
        Plateau jeu6(1);
        // Créer une grille vide
        jeu6.vidergrille();
        std::vector<std::vector<piece>> grille6  = jeu6.get_grille();
        // Placer un pion noir en (3,4) et un pion blanc en (3,5)
        grille6[3][4] = piece(type_pion::PION, false, 3, 4);
        grille6[3][5] = piece(type_pion::PION, true, 3, 5);
        jeu6.set_grille(grille6);
        jeu6.setJoueurActuel(0); // Blanc à jouer
        // Simuler le double pas du pion noir : de (1,4) à (3,4)
        jeu6.setDernierCoup(coup(1, 4, 3, 4, grille6[3][4], piece(), false, false, false));
        // Le coup en passant : le pion blanc de (3,5) se déplace en (2,4)
        coup enPassantCoup(3, 5, 2, 4, grille6[3][5], grille6[3][4], false, true, true);
        bool test6 = jeu6.jouerCoup(enPassantCoup);
        results.push_back(formatResult("Échecs - Prise en Passant Valide", test6));
        
        {
        // Test 6 : Test de promotion de Pion
        Plateau jeu6(1);
        // Créer une grille vide
        jeu6.vidergrille();
        std::vector<std::vector<piece>> grille6  = jeu6.get_grille();
        grille6[1][4] = piece(type_pion::PION, true, 1, 4);
        jeu6.set_grille(grille6);
        jeu6.setJoueurActuel(0); // Blanc à jouer
        // Le coup en passant : le pion blanc de (1,4) se déplace en (0,4)
        coup Coup(1, 4, 0, 4, grille6[1][4],piece(), false, false, true);
        jeu6.jouerCoup(Coup);
        std::vector<std::vector<piece>> grille6_1  = jeu6.get_grille();
        bool test6 = (grille6_1[0][4].getType()==type_pion::REINE);
        results.push_back(formatResult("Échecs - Promotion de pion", test6));
        }
        
        // Test 7 : Mouvement du Cavalier – vérifier que le cavalier placé en (4,4) dispose de 8 coups possibles
        Plateau jeu7(1);
        jeu7.vidergrille();
        std::vector<std::vector<piece>> grille7 = jeu7.get_grille();
        grille7[4][4] = piece(type_pion::CAVALIER, true, 4, 4);
        jeu7.set_grille(grille7);
        position_echec posEch({});
        std::vector<coup> coupsCavalier = posEch.movepossible(jeu7, grille7[4][4]);
        bool test7 = (coupsCavalier.size() == 8);
        results.push_back(formatResult("Échecs - Mouvement Cavalier (8 coups)", test7));
        
        
        // Test 9 : Détection de stalemate (pat) – position minimaliste de match nul
        Plateau jeu9(1);
        jeu9.vidergrille();
        std::vector<std::vector<piece>> grille9 = jeu9.get_grille();
        // Placer le roi noir dans un coin (0,0)
        grille9[0][0] = piece(type_pion::ROI, false, 0, 0);
        // Placer le roi blanc de façon à empêcher tout déplacement légal pour le roi noir
        grille9[2][2] = piece(type_pion::ROI, true, 2, 2);
        jeu9.set_grille(grille9);
        char gagnant9 = jeu9.verifierGagnant();
        bool test9 = (gagnant9 == 'V'); // 'V' indiquant match nul
        results.push_back(formatResult("Échecs - Stalemate Détecté (match nul)", test9));
        
        
        // Test 8 : Détection de checkmate (mat) – construire une position minimaliste de mat
        Plateau jeu8(1);
        jeu8.vidergrille();
        std::vector<std::vector<piece>> grille8 = jeu8.get_grille();
        // Placer le roi noir en (0,4)
        grille8[0][4] = piece(type_pion::ROI, false, 0, 4);
        // Placer deux pièces blanches menaçantes pour bloquer les cases d'évasion du roi
        grille8[1][4] = piece(type_pion::REINE, true, 1, 4);
        grille8[0][5] = piece(type_pion::TOUR, true, 0, 5);
        grille8[1][3] = piece(type_pion::TOUR, true, 1, 3);
        jeu8.set_grille(grille8);
        char gagnant8 = jeu8.verifierGagnant();
        // Supposons que la victoire des blancs soit indiquée par 'B' (ou 'O', selon votre convention)
        bool test8 = (gagnant8 == 'B');
        results.push_back(formatResult("Échecs - Checkmate Détecté (victoire)", test8));

    }
    
    // Affichage final des résultats
    std::cout << "\n==== Résultats des tests automatisés pour les échecs ====\n" << std::endl;
    for (const auto &ligne : results) {
        std::cout << ligne << std::endl;
    }
}







// ------------------------------------------------------
// 5) TEST IA ALPHABETA AVEC UNE SITUATION FAVORABLE
// ------------------------------------------------------


void testerFinaleDameRoi()
{
    // Pour l'affichage Unicode
    locale::global(std::locale("en_US.UTF-8"));
    wcout.imbue(std::locale("en_US.UTF-8"));

    Plateau jeu(1); // Plateau d'échecs
    jeu.vidergrille();
    
    vector<vector<piece>> grille = jeu.get_grille();
    grille[0][0] = piece(type_pion::ROI, false, 0, 0); // Roi Noir
    grille[7][7] = piece(type_pion::ROI, true, 7, 7);  // Roi Blanc
    grille[6][6] = piece(type_pion::REINE, true, 6, 6); // Dame Blanche
    
    jeu.set_grille(grille);
    jeu.setJoueurActuel(0); // L'IA joue en premier (Blanc)
    
    while (true)
    {
        //cout << "\n-----------------------------------\n";
        //cout << "Tour du joueur : " << jeu.getJoueurActuel() << endl;
        //jeu.afficher();

        // 🔵 Si c'est au joueur 0 (Blanc) de jouer
        if (jeu.getJoueurActuel() == 0)
        {
            position_echec position({});
            position.SetJoueur_init(0);
            
            //cout << "\nL'IA réfléchit (Blancs)...\n";
            Position* bestPos = position.meilleurCoup(jeu, true, 'B');
            //cout<<"Le joueur est : "<<jeu.getJoueurActuel()<<endl;

            if (!bestPos)
            {
                //cout << "Aucun coup trouvé pour les Blancs !\n";
                break;
            }

            // Récupération du premier coup joué
            const vector<coup>& coups = bestPos->GetCoups_successifs();
            if (!coups.empty())
            {
                coup FirstMove = coups.front();
                //cout << "L'IA Blanc joue : " << char('A' + FirstMove.x1) << (FirstMove.y1 + 1)
                //    << " -> " << char('A' + FirstMove.x2) << (FirstMove.y2 + 1) << endl;

                jeu.jouerCoup(FirstMove);
                //cout<<"J'ai trouvé un coup chez les blancs, ce coup est : "<<res;
            }
            jeu.setJoueurActuel(1);
        }
        else
        {
            position_echec position({});
            position.SetJoueur_init(1);

            //cout << "\nL'IA réfléchit (Noirs)...\n";
            Position* bestPos = position.meilleurCoup(jeu, true, 'N');

            if (!bestPos)
            {
                //cout << "Aucun coup trouvé pour les Noirs !\n";
                break;
            }

            // Récupération du premier coup joué
            const vector<coup>& coups = bestPos->GetCoups_successifs();
            if (!coups.empty())
            {
                coup FirstMove = coups.front();
                //cout << "L'IA Noir joue : " << char('A' + FirstMove.x1) << (FirstMove.y1 + 1)
                //    << " -> " << char('A' + FirstMove.x2) << (FirstMove.y2 + 1) << endl;
                jeu.jouerCoup(FirstMove);
                //cout<<"J'ai trouvé un coup chez les noirs, ce coup est : "<<res;
            }
        }

        char g = jeu.verifierGagnant();
        if (g == 'B')
        {
            jeu.afficher();
            cout << "\nL'IA Blanc a maté avec Roi + Dame !\n";
            break;
        }
        else if (g == 'N')
        {
            jeu.afficher();
            cout << "\nL'IA Noir a gagné !\n";
            break;
        }
        else if (g == 'V')
        {
            jeu.afficher();
            cout << "\nMatch nul détecté !\n";
            break;
        }
    }
}

