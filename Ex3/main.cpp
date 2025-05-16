#include "Game.hpp"
#include "Roles/Governor.hpp"
#include "Roles/Spy.hpp"
#include "Roles/Baron.hpp"
#include "Roles/General.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Merchant.hpp"
#include <iostream>

int main() {
    Game game;

    game.addPlayer(new Governor("Dana"));    // תומך ב־blockTax
    game.addPlayer(new Spy("Eli"));          // יכול לחסום arrest
    game.addPlayer(new Baron("Noa"));        // מקבל פיצוי על sanction
    game.addPlayer(new General("Tom"));      // יכול לחסום coup
    game.addPlayer(new Judge("Maya"));       // יכול לחסום bribe
    game.addPlayer(new Merchant("Itay"));    // מקבל מטבע אוטומטי עם 3+

    std::cout << "== Start of Game ==\n";
    for (auto& name : game.getActivePlayerNames()) {
        std::cout << name << " is in the game.\n";
    }

    while (true) {
        game.playTurn();

        // בדיקה אם יש רק שחקן אחד פעיל
        int alive = 0;
        std::string potentialWinner;
        for (auto p : game.getPlayers()) {
            if (p->isActive()) {
                alive++;
                potentialWinner = p->getName();
            }
        }

        if (alive == 1) {
            std::cout << "\n🏆 Winner is: " << potentialWinner << " 🏆\n";
            break;
        }
    }

    // שחרור זיכרון
    for (Player* p : game.getPlayers()) {
        delete p;
    }

    return 0;
}
