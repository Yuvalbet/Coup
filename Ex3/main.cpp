#include "Game.hpp"
void runGameGUI(Game& game);  // הכרזה בלבד

int main() {
    Game game;

    // נריץ את המשחק הגרפי
    runGameGUI(game);

    // בסיום המשחק (למשל אם נסגר החלון) – ננקה זיכרון
    for (Player* p : game.getPlayers()) {
        delete p;
    }

    return 0;
}
