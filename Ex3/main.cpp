// Include the Game class definition to use the game logic here
#include "Game.hpp"
void runGameGUI(Game& game);  // [TODO: Hebrew comment replaced – write English version]

// Main function: entry point of the application
int main() {
    Game game;

    // [TODO: Hebrew comment replaced – write English version]
    runGameGUI(game);

    // [TODO: Hebrew comment replaced – write English version]
    for (Player* p : game.getPlayers()) {
        delete p;
    }

    return 0;
}