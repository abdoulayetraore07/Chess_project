# Nom de l'exécutable
EXEC = jeu

# Compilateur
CXX = g++

# Options de compilation
CXXFLAGS = -Wall -Wextra -std=c++17

# Liste des fichiers sources
SRCS = main.cpp Echiquier.cpp Piece.cpp Position.cpp Test.cpp coup.cpp openings.cpp

# Génération des fichiers objets correspondants
OBJS = $(SRCS:.cpp=.o)

# Règle principale pour compiler l'exécutable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJS)

# Règle pour compiler chaque fichier .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(EXEC)

# Règle pour forcer la recompilation complète
rebuild: clean $(EXEC)
