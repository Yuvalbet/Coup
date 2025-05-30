//Email: yuvali532@gmail.com
#include "Game.hpp"

// Starts the graphical user interface for the game.
void runGameGUI(Game& game); 

// Initializes the game and launches the GUI interface.
int main() {
// Creates a new Game instance to manage players and turns.
    Game game;

 // Starts the graphical user interface for the game.
    runGameGUI(game);

    return 0;
}