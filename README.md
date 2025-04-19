# 🎮 Projet Échecs & Tic-Tac-Toe en C++ ♜⭕✖

## 📌 Description
Ce projet implémente **deux jeux classiques** en C++ :
1. **Tic-Tac-Toe (3x3)**
2. **Jeu d'Échecs (8x8)** avec affichage Unicode des pièces.

L'utilisateur peut choisir entre **le Tic-Tac-Toe** et **les Échecs**, puis jouer en tour par tour.

---

## 🚀 **Fonctionnalités**
✔ **Affichage Unicode des pièces** (`♜ ♞ ♝ ♛ ♚` pour les échecs, `✖ ⭕` pour le Tic-Tac-Toe).  
✔ **Gestion complète des règles** (déplacement des pièces aux échecs).  
✔ **Détection de la fin du jeu** (victoire, match nul, échec et mat).  
✔ **Système de gestion des coups** (avec vérification des règles).  
✔ **Code organisé en fichiers `.hpp` et `.cpp`**.

---
🛠 Compilation avec Makefile

La compilation a été automatisée avec un **Makefile**, donc pour compiler le projet, il suffit d'exécuter la commande suivante dans le terminal : make

Cela va automatiquement compiler tous les fichiers source et générer l'exécutable. Si vous souhaitez nettoyer les fichiers compilés, vous pouvez utiliser la commande suivante : make clean

---

## 📂 **Structure du projet**
```
/projet-echecs
│── src/
│   │── main.cpp          # Programme principal, gère l'exécution du jeu
│   │── echiquier.hpp     # Déclaration de la classe Echiquier (jeu d'échecs)
│   │── echiquier.cpp     # Implémentation de la classe Echiquier
│   │── piece.hpp         # Déclaration de la classe Piece (représente les pièces d'échecs)
│   │── piece.cpp         # Implémentation des comportements des pièces
│   │── coup.hpp          # Déclaration de la classe Coup (mouvements des pièces)
│   │── coup.cpp          # Implémentation de la gestion des coups
│   │── openings.cpp      # Implémentation des ouvertures pour le jeu d'échecs
│   │── openings.hpp      # Déclaration de la classe Openings (gestion des ouvertures)
│   │── Test.cpp          # Implémentation des tests unitaires pour les jeux
│   │── Test.hpp          # Déclaration des tests unitaires
│── README.md             # Documentation du projet

```

---

## 🎲 **Comment jouer ?**
### 1️⃣ **Choix du mode de jeu**
Le programme demande si tu veux jouer :
```
Choisissez un jeu :
1. Tic-Tac-Toe (3x3)
2. Échecs (8x8)
Entrez votre choix (1 ou 2) :
```

- 🟢 **Tic-Tac-Toe** : Entrez des coups sous forme `A1`.
- 🟢 **Échecs** : Entrez des coups sous forme `E2 E4`.

---
2️⃣ Algorithmes et IA
Tic-Tac-Toe : L'IA joue en utilisant l'algorithme Minimax.
Échecs : L'IA utilise Minimax avec l'élagage Alpha-Beta pour optimiser le calcul et jouer de manière optimale. Le système gère les coups classiques ainsi que des ouvertures célèbres pour donner à l'IA un avantage stratégique au début du jeu.

---
3️⃣ Tests et validation
Le programme propose plusieurs tests automatiques, que vous pouvez activer en modifiant la variable test dans le fichier main.cpp. Voici les options disponibles :

test = 0 : Mode de jeu normal. Vous pouvez jouer au Tic-Tac-Toe ou aux Échecs avec l'IA ou un joueur humain.
test = 1 : Test de base pour Tic-Tac-Toe. Cela vérifie des scénarios comme les coups invalides, les coups gagnants, et les matchs nuls.
test = 2 : Tester l'IA contre un joueur aléatoire pour Tic-Tac-Toe. Cela simule des parties entre l'IA et un joueur qui joue de manière aléatoire.
test = 3 : Tester l'IA contre elle-même pour Tic-Tac-Toe. Cela lance des parties entre deux instances de l'IA pour évaluer son efficacité.
test = 4 : Test des fonctionnalités de base pour les Échecs, comme le roque, les prises en passant, et la détection de mat ou de stalemate.
test = 5 : Tester un scénario où l'IA gagne avec un Roi et une Dame contre un Roi seul (vérifie la fin du jeu dans un cas simple).

  

