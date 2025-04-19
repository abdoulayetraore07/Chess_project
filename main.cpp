#include <iostream>
#include <locale>
#include <iomanip>
#include <random>
#include <ctime>
#include "Echiquier.hpp"
#include "Position.hpp"
#include "Test.hpp"
#include <cassert>


int main()
{
    
    int test = 0;         // 0 pour le jeu en mode normal, voir Test.hpp Test.cpp pour les autres test.
    int nb_matchs = 50;  // Nombre de parties pour les tests.
    int result = 0;       // Retour de la console
    
    
    switch (test)
    {
        case 0:
            result = jeu_main();
            break;
        case 1:
            BasicTests_TIC();
            break;
        case 2:
            testerIAContreAleatoire(nb_matchs);
            break;
        case 3:
            testerIAcontreIA(nb_matchs);
            break;
        case 4:
            BasicTests_CHESS();
            break;
        case 5:
            testerFinaleDameRoi();
            break;
        case 6:
            BasicTests_TIC();
            testerIAContreAleatoire(nb_matchs);
            testerIAcontreIA(nb_matchs);
            BasicTests_CHESS();
            testerFinaleDameRoi();
            break;
        default:
            assert(false && "Erreur: choix de test inconnu !");
    }
    
    
    
    return result ;

}



