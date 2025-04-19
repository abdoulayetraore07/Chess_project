#include <vector>
#include <memory>
#include "Echiquier.hpp"
#include "coup.hpp"
#include "openings.hpp"
using namespace std;

coup Openings::getOpeningMove(const vector<string>& historiqueCoups) 
{
    
        vector<pair<string, string>> coupsPossibles; // Liste des coups que l'IA peut jouer avec le nom de l'ouverture
        coup coupChoisi = coup();
        string ouvertureChoisie = "";

        for (const auto& ouverture : ouvertureMoves) {
            const vector<string>& coupsOuverture = ouverture.second;

            // Vérifier si l'historique correspond au début de cette ouverture
            if (historiqueCoups.size() < coupsOuverture.size() &&
                equal(historiqueCoups.begin(), historiqueCoups.end(), coupsOuverture.begin())) {

                // Ajouter le prochain coup de cette ouverture à la liste avec son nom
                coupsPossibles.emplace_back(ouverture.first, coupsOuverture[historiqueCoups.size()]);
            }
        }

        // Si aucun coup possible, arrêter la phase d'ouverture
        if (coupsPossibles.empty()) 
        {
            return coupChoisi.traduireCoup("");
        }

        // Mélanger les coups pour en choisir un aléatoirement
        random_device rd;
        mt19937 g(rd());
        shuffle(coupsPossibles.begin(), coupsPossibles.end(), g);

        // Sélection du coup aléatoire et stockage du nom de l'ouverture choisie
        ouvertureChoisie = coupsPossibles[0].first;
        string coupStr = coupsPossibles[0].second;

        // Afficher l'ouverture choisie
        cout << "L'IA a décidé de jouer l'ouverture : " << ouvertureChoisie << endl;

        // Retourner le coup choisi
        return coupChoisi.traduireCoup(coupStr);
    
};
