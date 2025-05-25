// Game.cpp
#include "Game.hpp"
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include "Roles/Governor.hpp"
#include "Roles/Baron.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Spy.hpp"
#include "Player.hpp"
#include <limits>

Game::Game() : currentTurnIndex(0), sanctionedLastRound(nullptr), lastArrestedPlayer(nullptr) {}

void Game::addPlayer(Player* player) {
    players.push_back(player);
}

const std::vector<Player*>& Game::getPlayers() const {
    return players;
}

void Game::nextTurn() {

    std::cout << "---- NEXT TURN DEBUG ----\n";
    for (size_t i = 0; i < players.size(); ++i) {
        std::cout << "[" << i << "] " << players[i]->getName()
                  << " | Active: " << players[i]->isActive() << "\n";
    }

    do {
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
    } while (!players[currentTurnIndex]->isActive());

   

    // ✅ אם השחקן הנוכחי היה מי שסומן כ־sanctioned מהתור הקודם – החרם מבוטל רק עכשיו
    if (players[currentTurnIndex] == sanctionedLastRound) {
        players[currentTurnIndex]->setSanctioned(true);   // מפעילים את החרם עכשיו
        sanctionedLastRound = nullptr;                    // מסמנים שאין יותר חרם שממתין
    } else {
        players[currentTurnIndex]->setSanctioned(false);  // כל שאר השחקנים לא בחרם
    }


    Merchant* merchant = dynamic_cast<Merchant*>(players[currentTurnIndex]);
    if (merchant && merchant->getCoins() >= 3) {
        merchant->onStartTurn();
    }

    
}


Player* Game::currentPlayer() const {
    return players[currentTurnIndex];
}

std::vector<std::string> Game::getActivePlayerNames() const {
    std::vector<std::string> names;
    for (Player* p : players) {
        if (p->isActive()) {
            names.push_back(p->getName());
        }
    }
    return names;
}

std::string Game::getWinner() const {
    std::string winner;
    int activeCount = 0;
    for (Player* p : players) {
        if (p->isActive()) {
            winner = p->getName();
            activeCount++;
        }
    }
    if (activeCount == 1) return winner;
    throw std::runtime_error("Game is still ongoing.");
}

void Game::setCurrentTurnIndex(int index) {
    currentTurnIndex = index;
}





// void Game::tryBlockBribe(Judge* judge) {
//     for (auto it = pendingActions.begin(); it != pendingActions.end(); ++it) {
//         if (it->type == "bribe") {
//             judge->blockBribe(*(it->performer));
//             pendingActions.erase(it);
//             return;
//         }
//     }
//     std::cout << "No bribe action to block.\n";
// }

// void Game::tryBlockTax(Governor* governor) {
//     std::cout << "[DEBUG] Trying to block tax. Pending actions size: " << pendingActions.size() << "\n";
//     for (const auto& action : pendingActions) {
//         std::cout << "- " << action.type << " by " << action.performer->getName() << "\n";
//     }

//     for (auto it = pendingActions.begin(); it != pendingActions.end(); ++it) {
//         if (it->type == "tax") {
//             governor->blockTax(*(it->performer));
//             pendingActions.erase(it);
//             return;
//         }
//     }
//     std::cout << "No tax action to block.\n";
// }


void Game::playTurn(int choice, Player* target) {
    Player* player = currentPlayer();

    if (player->getCoins() >= 10 && choice != 6) {
        throw std::runtime_error("You have 10+ coins. You MUST perform a coup.");
    }

    int extraActions = 1;

    while (extraActions > 0) {
        if (player->getCoins() >= 10 && choice != 6) {
            std::cout << "You have 10+ coins. You MUST perform a coup.\n";
            return;
        }

        if ((choice == 3 && player->getCoins() < 4) ||
            (choice == 5 && player->getCoins() < 3) ||
            (choice == 6 && player->getCoins() < 7)) {
            std::cout << "Not enough coins for that action.\n";
            return;
        }

        if (player->isSanctioned() && (choice == 1 || choice == 2)) {
            std::cout << "You are sanctioned and cannot use gather or tax this turn.\n";
            return;
        }

        try {
            switch (choice) {
                case 1:
                    player->gather();
                    setLastActionMessage(player->getName() + " gathered 1 coin.");
                    break;

                case 2:
                    player->tax();
                    setLastActionMessage(player->getName() + " collected 2 coins from tax.");
                    break;

                case 3:
                    player->bribe();
                    extraActions += 2;
                    setLastActionMessage(player->getName() + " paid a bribe and received extra actions.");
                    break;

                case 4:
                    if (!target) return;
                    if (player->isArrestBlocked()) return;
                    if (target == lastArrestedPlayer) return;
                    player->arrest(*target);
                    lastArrestedPlayer = target;
                    setLastActionMessage(player->getName() + " arrested " + target->getName() + ".");
                    break;

                case 5:
                    if (!target) return;
                    player->sanction(*target);
                    sanctionedLastRound = target;
                    setLastActionMessage(player->getName() + " sanctioned " + target->getName() + ".");
                    break;

                case 6:
                    if (!target) return;
                    player->coup(*target);
                    setLastActionMessage(player->getName() + " performed a coup on " + target->getName() + ".");
                    break;

                case 8:
                    if (auto* b = dynamic_cast<Baron*>(player)) {
                        b->invest();
                        setLastActionMessage(player->getName() + " invested using Baron power.");
                    }
                    break;

                case 9:
                    if (auto* s = dynamic_cast<Spy*>(player)) {
                        if (!target) return;
                        s->spyOn(*target);
                        setLastActionMessage(player->getName() + " spied on " + target->getName() + ".");
                    }
                    break;

                default:
                    std::cout << "Invalid choice\n";
                    return;
            }
        } catch (const std::exception& e) {
            std::cerr << "Action failed: " << e.what() << "\n";
            setLastActionMessage("Action failed: " + std::string(e.what()));
            return;
        }

        extraActions--;
    }

    player->updateArrestBlock(false);
    nextTurn();
}



std::vector<Player*> Game::getValidTargets(Player* current) const {
    std::vector<Player*> targets;
    for (Player* p : players) {
        if (p != current && p->isActive()) {
            targets.push_back(p);
        }
    }
    return targets;
}

void Game::assignRandomRoles(const std::vector<std::string>& names) {
    std::srand(static_cast<unsigned int>(time(nullptr)));

    for (const std::string& name : names) {
        int r = std::rand() % 6;  // מספר רנדומלי בין 0 ל־5

        Player* player = nullptr;

        switch (r) {
            case 0: player = new Governor(name); break;
            case 1: player = new Spy(name); break;
            case 2: player = new Baron(name); break;
            case 3: player = new General(name); break;
            case 4: player = new Judge(name); break;
            case 5: player = new Merchant(name); break;
        }
        player->removeCoins(player->getCoins()); // 👈 אפס מיד את המטבעות

        this->addPlayer(player);  // ✅ נשמר לפי הסדר
        std::cout << "[DEBUG] Added player: " << player->getName() << " @ " << player << ", coins: " << player->getCoins() << "\n";

    }

    currentTurnIndex = 0; // תמיד מתחילים מהשחקן הראשון

    std::cout << "=== Players Memory Addresses ===\n";
for (Player* p : players) {
    std::cout << p->getName() << " @ " << p << ", coins: " << p->getCoins() << "\n";
}


}
std::string Game::getLastActionMessage() const {
    return lastActionMessage;
}

void Game::setLastActionMessage(const std::string& msg) {
    lastActionMessage = msg;
}

