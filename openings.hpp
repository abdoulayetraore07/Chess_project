#ifndef OPENINGS_HPP
#define OPENINGS_HPP

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <random>
#include "coup.hpp"

class Openings {
private:
    map<string, vector<string>> ouvertureMoves; // Historique → Liste des coups possibles

public:
    Openings() {
        ouvertureMoves["italienne"] = {
            "Wg5e5", "Bb5d5",  // 1. e4 e5
            "Wh7f6", "Ba2c3",  // 2. Nf3 Nc6
            "Wh6e3", "Ba6d3"   // 3. Bc4 Bc5
        };

        ouvertureMoves["gambit_du_roi"] = {
            "Wg5e5", "Bb5d5",  // 1. f4 e5
            "Wg6e6"
        };

        ouvertureMoves["sicilienne"] = {
            "Wg5e5", "Bb3d3"
        };

        ouvertureMoves["francaise"] = {
            "Wg5e5", "Bb5c5"
        };

        ouvertureMoves["gambit_dame"] = {
            "Wg4e4", "Bb4d4",  // 1. d4 d5
            "Wg3e3"
        };

        ouvertureMoves["ruy_lopez"] = {
            "Wg5e5", "Bb5d5",  // 1. e4 e5
            "Wh7f6", "Ba2c3",  // 2. Nf3 Nc6
            "Wh6d2"
        };

        ouvertureMoves["caro_kann"] = {
            "Wg5e5", "Bb3c3"
        };

        ouvertureMoves["Scandinavian Defense"] = {
            "Wg5e5", "Bb4d4"
        };

        ouvertureMoves["pirc"] = {
            "Wg5e5", "Bb5c5",  // 1. e4 d6
            "Wg4f4", "Ba6f6"   // 2. d4 Nf6
        };

        ouvertureMoves["Alekhine's Defense"] = {
            "Wg5e5", "Ba7c6"
        };

        ouvertureMoves["scotch"] = {
            "Wg5e5", "Bb5d5",  // 1. e4 e5
            "Wh7f6", "Ba2c3",  // 2. Nf3 Nc6
            "Wg4e4"            // 3. d4
        };

        ouvertureMoves["vienna"] = {
            "Wg5e5", "Bb5d5",  // 1. e4 e5
            "Wh2f3"            // 2. Nc3
        };

        ouvertureMoves["slav"] = {
            "Wg4e4", "Bb4d4",  // 1. d4 d5
            "Wg3e3", "Bb3c3"   // 2. c4 c6
        };

        ouvertureMoves["kings_indian"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb7c5"   // 2. c4 g6
        };

        ouvertureMoves["nimzo_indian"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb5c5",  // 2. c4 e6
            "Wh2f2", "Ba6e2"   // 3. Nc3 Bb4
        };

        ouvertureMoves["queens_indian"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb5c5",  // 2. c4 e6
            "Wh7f6", "Bb2c2"   // 3. Nf3 b6
        };

        ouvertureMoves["catalan"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb5c5",  // 2. c4 e6
            "Wg7f7"            // 3. g3
        };

        ouvertureMoves["bogo_indian"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb5c5",  // 2. c4 e6
            "Wh7f6", "Ba6e2"   // 3. Nf3 Bb4+
        };

        ouvertureMoves["grunfeld"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb7c7",  // 2. c4 g6
            "Wh2f2", "Bb4d4"   // 3. Nc3 d5
        };

        ouvertureMoves["Dutch Defense"] = {
            "Wg4e4", "Bb6d6"
        };

        ouvertureMoves["trompowsky"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wh3d7"            // 2. Bg5
        };

        ouvertureMoves["benko_gambit"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb3d3",  // 2. c4 c5
            "We4d4", "Bb2d2"   // 3. d5 b5
        };

        ouvertureMoves["london_system"] = {
            "Wg4e4", "Bb4d4",  // 1. d4 d5
            "Wh7f6", "Ba7c6",  // 2. Nf3 Nf6
            "Wh3e6"            // 3. Bf4
        };

        ouvertureMoves["benoni_modern"] = {
            "Wg4e4", "Ba7c6",  // 1. d4 Nf6
            "Wg3e3", "Bb3d3",  // 2. c4 c5
            "We4d4", "Bb5c5",  // 3. d5 e6
            "Wh2f2", "Bc5d4",  // 4. Nc3 exd5
            "We3d4", "Bb4c4"   // 5. cxd5 d6
        };

        ouvertureMoves["kings_indian_attack"] = {
            "Wh7f6", "Bb4d4",  // 1. Nf3 d5
            "Wg7f7"            // 2. g3
        };
    }

    coup getOpeningMove(const vector<string>& historiqueCoups);
};

#endif


