# Chess & Tic-Tac-Toe Project in C++
## 📌 Description
This project implements **two classic games** in C++:
1. **Tic-Tac-Toe (3x3)**
2. **Chess Game (8x8)** with Unicode display of pieces.
The user can choose between **Tic-Tac-Toe** and **Chess**, then play turn by turn.
---
## **Features**
✔ **Unicode display of pieces** (`♜ ♞ ♝ ♛ ♚` for chess, `✖ ⭕` for Tic-Tac-Toe).  
✔ **Complete rules management** (piece movement in chess).  
✔ **End of game detection** (victory, draw, checkmate).  
✔ **Move management system** (with rules verification).  
✔ **Code organized in `.hpp` and `.cpp` files**.
---
Compilation with Makefile
Compilation has been automated with a **Makefile**, so to compile the project, simply execute the following command in the terminal: make
This will automatically compile all source files and generate the executable. If you want to clean the compiled files, you can use the following command: make clean
---
## **Project Structure**
```
/chess-project
│── src/
│   │── main.cpp          # Main program, manages game execution
│   │── echiquier.hpp     # Declaration of the Echiquier class (chess game)
│   │── echiquier.cpp     # Implementation of the Echiquier class
│   │── piece.hpp         # Declaration of the Piece class (represents chess pieces)
│   │── piece.cpp         # Implementation of piece behaviors
│   │── coup.hpp          # Declaration of the Coup class (piece movements)
│   │── coup.cpp          # Implementation of move management
│   │── openings.cpp      # Implementation of openings for chess
│   │── openings.hpp      # Declaration of the Openings class (openings management)
│   │── Test.cpp          # Implementation of unit tests for games
│   │── Test.hpp          # Declaration of unit tests
│── README.md             # Project documentation
```
---
## **How to play?**
### 1️⃣ **Game mode selection**
The program asks if you want to play:
```
Choose a game:
1. Tic-Tac-Toe (3x3)
2. Chess (8x8)
Enter your choice (1 or 2):
```
- **Tic-Tac-Toe**: Enter moves in the form `A1`.
- **Chess**: Enter moves in the form `E2 E4`.
---
### 2️⃣ Algorithms and AI
Tic-Tac-Toe: The AI plays using the Minimax algorithm.
Chess: The AI uses Minimax with Alpha-Beta pruning to optimize calculation and play optimally. The system manages classic moves as well as famous openings to give the AI a strategic advantage at the beginning of the game.
---
### 3️⃣ Tests and validation
The program offers several automatic tests, which you can activate by modifying the test variable in the main.cpp file. Here are the available options:
```
test = 0: Normal game mode. You can play Tic-Tac-Toe or Chess with AI or a human player.
test = 1: Basic test for Tic-Tac-Toe. This checks scenarios such as invalid moves, winning moves, and draws.
test = 2: Test AI against a random player for Tic-Tac-Toe. This simulates games between the AI and a player who plays randomly.
test = 3: Test AI against itself for Tic-Tac-Toe. This launches games between two instances of the AI to evaluate its effectiveness.
test = 4: Test basic chess functionalities, such as castling, en passant captures, and detection of checkmate or stalemate.
test = 5: Test a scenario where the AI wins with a King and Queen against a lone King (checks end of game in a simple case).
```
