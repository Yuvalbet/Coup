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

    if (pendingExtraTurns > 0) {
        pendingExtraTurns--;
        return; // ❗❗ חייב לחזור מייד!
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




void Game::tryBlockBribe(Judge* judge) {
    for (size_t i = 0; i < pendingActionTypes.size(); ++i) {
    if (pendingActionTypes[i] == "bribe") {
        judge->blockBribe(*pendingPerformers[i]);
        pendingActionTypes.erase(pendingActionTypes.begin() + i);
        pendingPerformers.erase(pendingPerformers.begin() + i);
        pendingTargets.erase(pendingTargets.begin() + i);
        return;
    }
}

    std::cout << "No bribe action to block.\n";
}


void Game::tryBlockTax(Governor* governor) {
    for (size_t i = 0; i < pendingActionTypes.size(); ++i) {
    if (pendingActionTypes[i] == "tax") {
        governor->blockTax(*pendingPerformers[i]);
        pendingActionTypes.erase(pendingActionTypes.begin() + i);
        pendingPerformers.erase(pendingPerformers.begin() + i);
        pendingTargets.erase(pendingTargets.begin() + i);
        return;
    }
}

    std::cout << "No tax action to block.\n";
}
bool Game::tryBlockCoup(Player* source, Player* target) {
    for (Player* p : players) {
        if (!p->isActive()) continue;

        General* general = dynamic_cast<General*>(p);
        if (general != nullptr && general->getCoins() >= 5) {
            pendingPerformers.push_back(source);
            pendingTargets.push_back(target);
            pendingActionTypes.push_back("Coup");
            return true;
        }
    }
    return false;
}


void Game::playTurn(int choice, Player* target) {
    Player* player = currentPlayer();
    bool turnConsumed = false;  // ✅ חדש

    if (player->getCoins() >= 10 && choice != 6) {
        throw std::runtime_error("You have 10+ coins. You MUST perform a coup.");
    }

    if ((choice == 3 && player->getCoins() < 4) ||
        (choice == 5 && player->getCoins() < 3) ||
        (choice == 6 && player->getCoins() < 7)) {
        std::cout << "Not enough coins for that action.\n";
        return;
    }

    if (player->isSanctioned() && (choice == 1 || choice == 2)) {
        setLastActionMessage(player->getName() + " is sanctioned and cannot perform this action.");
        return;
    }

    try {
        switch (choice) {
            case 1:
                player->gather();
                setLastActionMessage(player->getName() + " gathered 1 coin.");
                turnConsumed = true;
                break;

            case 2:
                player->tax();
                pendingActionTypes.push_back("tax");
                pendingPerformers.push_back(player);
                pendingTargets.push_back(nullptr);
                setLastActionMessage(player->getName() + " collected 2 coins from tax.");
                turnConsumed = true;
                break;

            case 3:
                pendingActionTypes.push_back("bribe");
                pendingPerformers.push_back(player);
                pendingTargets.push_back(nullptr);
                setLastActionMessage(player->getName() + " paid a bribe and received extra actions.");
                turnConsumed = true;
                break;

            case 4:
                if (!target) return;
                if (target == lastArrestedPlayer){
                    throw std::runtime_error("This player is still under arrest protection.");
                }
                player->arrest(*target);
                lastArrestedPlayer = target;
                if (dynamic_cast<Merchant*>(target)) {
                    // הסוחר כבר שילם 2, אין שינוי לעוצר
                }
                else if (dynamic_cast<General*>(target)) {
                    // הגנרל לא משלם, ולא מוסיפים לעוצר
                }
                else {
                    player->addCoins(1);  // שחקנים רגילים – מקבלים מטבע
                }   
                setLastActionMessage(player->getName() + " arrested " + target->getName() + ".");
                turnConsumed = true;
                break;

            case 5:
                if (!target) return;
                player->sanction(*target);
                sanctionedLastRound = target;
                setLastActionMessage(player->getName() + " sanctioned " + target->getName() + ".");
                turnConsumed = true;
                break;

            case 6: {
                if (!target) return;

                if (player->getCoins() < 7) {
                    throw std::runtime_error("Not enough coins for coup.");
                }

                player->removeCoins(7); // ✅ תמיד יורדים 7 מטבעות

                bool hasGeneral = false;
                for (Player* p : players) {
                    if (p != player && p->isActive()) {
                        if (dynamic_cast<General*>(p)) {
                            hasGeneral = true;
                            break;
                        }
                    }
                }

                if (hasGeneral) {
                    pendingActionTypes.push_back("coup");
                    pendingPerformers.push_back(player);
                    pendingTargets.push_back(target);
                    setLastActionMessage(player->getName() + " attempted a coup. Waiting for General.");
                    return; // ❗ עצירה כאן – נחכה להחלטת הגנרל
                } else {
                    target->deactivate(); // ✅ מבצעים הפיכה אם אין מי שיחסום
                    setLastActionMessage(player->getName() + " performed a coup on " + target->getName() + ".");
                }

                turnConsumed = true;
                break;
            }

            case 8:
                if (auto* b = dynamic_cast<Baron*>(player)) {
                    b->invest();
                    setLastActionMessage(player->getName() + " invested using Baron power.");
                    turnConsumed = true;
                }
                break;

            case 9:
                if (auto* s = dynamic_cast<Spy*>(player)) {
                    if (!target) return;
                    s->spyOn(*target);
                    setLastActionMessage(player->getName() + " spied on " + target->getName() + ".");
                    turnConsumed = true;
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

    player->updateArrestBlock(false);

if (turnConsumed) {
    if (pendingExtraTurns > 0) {
        std::cout << "[DEBUG] BRIBE TURN used – still got more\n";
        pendingExtraTurns--;
    } else {
        std::cout << "[DEBUG] Regular turn used – moving to next player\n";
        nextTurn();
        try {
            std::string winner = getWinner();
            setLastActionMessage("🏆 Game Over! Winner: " + winner);
        } catch (...) {}
    }
} else {
    std::cout << "[DEBUG] No valid action performed – turn not consumed\n";
    // השחקן יישאר בתורו – ניסיון לא נחשב
}


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

void Game::addExtraTurns(int count) {
    pendingExtraTurns += count;
}

