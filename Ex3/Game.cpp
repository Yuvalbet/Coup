// Game.cpp
#include "Game.hpp"
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include "Roles/Governor.hpp"
#include "Roles/Baron.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Spy.hpp"
#include <limits>

Game::Game() : currentTurnIndex(0), coinBank(50) {}

void Game::addPlayer(Player* player) {
    players.push_back(player);
}

const std::vector<Player*>& Game::getPlayers() const {
    return players;
}

void Game::nextTurn() {
    do {
        currentTurnIndex = (currentTurnIndex + 1) % players.size();
    } while (!players[currentTurnIndex]->isActive());

    pendingActions.clear();

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

void Game::addToBank(int amount) {
    coinBank += amount;
}

void Game::removeFromBank(int amount) {
    if (coinBank < amount) {
        throw std::invalid_argument("Bank does not have enough coins.");
    }
    coinBank -= amount;
}

int Game::getBankCoins() const {
    return coinBank;
}

void Game::addBlockableAction(Player* performer, const std::string& type) {
    pendingActions.push_back({performer, type});
}

void Game::tryBlockBribe(Judge* judge) {
    for (auto it = pendingActions.begin(); it != pendingActions.end(); ++it) {
        if (it->type == "bribe") {
            judge->blockBribe(*(it->performer));
            pendingActions.erase(it);
            return;
        }
    }
    std::cout << "No bribe action to block.\n";
}

void Game::tryBlockTax(Governor* governor) {
    for (auto it = pendingActions.begin(); it != pendingActions.end(); ++it) {
        if (it->type == "tax") {
            governor->blockTax(*(it->performer));
            pendingActions.erase(it);
            return;
        }
    }
    std::cout << "No tax action to block.\n";
}

void Game::playTurn() {
    Player* player = currentPlayer();

    while (true) {
        std::cout << "\n== " << player->getName() << "'s turn (" << player->getRoleName() << ") ==\n";
        std::cout << "Coins: " << player->getCoins() << "\n";

        // חובה לבצע הפיכה אם יש 10 מטבעות
        if (player->getCoins() >= 10) {
            std::cout << "You have 10+ coins. You MUST perform a coup. Choose target:\n";
            std::vector<Player*> targets = getValidTargets(player);
            for (size_t i = 0; i < targets.size(); ++i) {
                std::cout << (i + 1) << ". " << targets[i]->getName() << "\n";
            }
            int choice;
            std::cin >> choice;
            if (std::cin.fail() || choice < 1 || choice > (int)targets.size()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid target.\n";
                continue;
            }
            Player* target = targets[choice - 1];
            try {
                player->coup(*target);
            } catch (const std::exception& e) {
                std::cerr << "Coup failed: " << e.what() << "\n";
                continue;
            }
            nextTurn();
            return;
        }

        // מציג את האפשרויות
        std::cout << "Choose an action:\n";
        std::cout << "1. Gather\n";
        std::cout << "2. Tax\n";
        std::cout << "3. Bribe (extra action)\n";
        std::cout << "4. Arrest another player\n";
        std::cout << "5. Sanction another player\n";
        std::cout << "6. Coup another player\n";
        std::cout << "7. Skip\n";

        bool isJudge = dynamic_cast<Judge*>(player);
        bool isGovernor = dynamic_cast<Governor*>(player);
        bool isSpy = dynamic_cast<Spy*>(player);

        if (isJudge) std::cout << "8. Judge blocks bribe\n";
        if (isGovernor) std::cout << "9. Governor blocks tax\n";
        if (isSpy) std::cout << "10. Spy on another player\n";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        // בדיקת חוקיות לפי תפקיד
        if ((choice == 8 && !isJudge) || (choice == 9 && !isGovernor) || (choice == 10 && !isSpy)) {
            std::cout << "Invalid choice for your role. Try again.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1:
                    player->gather();
                    break;
                case 2:
                    player->tax();
                    addBlockableAction(player, "tax");
                    break;
                case 3:
                    player->bribe();
                    addBlockableAction(player, "bribe");
                    break;
                case 4:
                case 5:
                case 6:
                case 10: {
                    std::vector<Player*> targets = getValidTargets(player);
                    if (targets.empty()) {
                        std::cout << "No valid targets.\n";
                        continue;
                    }

                    std::cout << "Choose a target:\n";
                    for (size_t i = 0; i < targets.size(); ++i) {
                        std::cout << (i + 1) << ". " << targets[i]->getName() << "\n";
                    }

                    int targetChoice;
                    std::cin >> targetChoice;
                    if (std::cin.fail() || targetChoice < 1 || targetChoice > (int)targets.size()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid target.\n";
                        continue;
                    }

                    Player* target = targets[targetChoice - 1];

                    if (choice == 4) {
                        Merchant* m = dynamic_cast<Merchant*>(target);
                        if (m) {
                            m->onArrestedBy(*player);
                        } else {
                            player->arrest(*target);
                            if (auto* g = dynamic_cast<General*>(target)) {
                                g->onArrested();
                            }
                        }
                    } else if (choice == 5) {
                        player->sanction(*target);
                        if (auto* b = dynamic_cast<Baron*>(target)) {
                            b->receiveSanctionFrom(*player);
                        }
                        if (auto* j = dynamic_cast<Judge*>(target)) {
                            j->receiveSanctionFrom(*player);
                        }
                    } else if (choice == 6) {
                        if (auto* g = dynamic_cast<General*>(target); g && g->getCoins() >= 5) {
                            std::cout << g->getName() << " blocked the coup using 5 coins!\n";
                            g->blockCoup(*target, *player);
                        } else {
                            player->coup(*target);
                        }
                    } else if (choice == 10) {
                        Spy* s = dynamic_cast<Spy*>(player);
                        if (s) s->spyOn(*target);
                    }

                    break;
                }
                case 7:
                    std::cout << "Skipping turn.\n";
                    break;
                case 8:
                    tryBlockBribe(static_cast<Judge*>(player));
                    break;
                case 9:
                    tryBlockTax(static_cast<Governor*>(player));
                    break;
                default:
                    std::cout << "Invalid choice. Try again.\n";
                    continue;
            }

            break;  // יציאה מהלולאה – פעולה חוקית בוצעה
        } catch (const std::exception& e) {
            std::cerr << "Action failed: " << e.what() << "\n";
            continue;
        }
    }

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

