// Game.cpp
#include "Game.hpp"
#include <iostream>
#include <stdexcept>
#include <typeinfo>
#include "Roles/Governor.hpp"
#include "Roles/Baron.hpp"
#include "Roles/Judge.hpp"
#include "Roles/Spy.hpp"

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
    std::cout << "\n== " << player->getName() << "'s turn (" << player->getRoleName() << ") ==\n";
    std::cout << "Coins: " << player->getCoins() << "\n";

    if (player->getCoins() >= 10) {
        std::cout << "You have 10+ coins. You MUST perform a coup. Choose target:\n";
        for (size_t i = 0; i < players.size(); ++i) {
            if (players[i] != player && players[i]->isActive()) {
                std::cout << i << ". " << players[i]->getName() << "\n";
            }
        }
        int targetIndex;
        std::cin >> targetIndex;
        Player* target = players[targetIndex];
        try {
            player->coup(*target);
        } catch (const std::exception& e) {
            std::cerr << "Coup failed: " << e.what() << "\n";
        }
        nextTurn();
        return;
    }

    std::cout << "Choose an action:\n";
    std::cout << "1. Gather\n";
    std::cout << "2. Tax\n";
    std::cout << "3. Bribe (extra action)\n";
    std::cout << "4. Arrest another player\n";
    std::cout << "5. Sanction another player\n";
    std::cout << "6. Coup another player\n";
    std::cout << "7. Skip\n";
    std::cout << "8. Judge blocks bribe\n";
    std::cout << "9. Governor blocks tax\n";

    int choice;
    std::cin >> choice;

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
            case 6: {
                std::cout << "Choose target:\n";
                for (size_t i = 0; i < players.size(); ++i) {
                    if (players[i] != player && players[i]->isActive()) {
                        std::cout << i << ". " << players[i]->getName() << "\n";
                    }
                }
                int targetIndex;
                std::cin >> targetIndex;
                if (targetIndex < 0 || targetIndex >= players.size() || !players[targetIndex]->isActive()) {
                    std::cout << "Invalid target.\n";
                    return;
                }
                Player* target = players[targetIndex];

                if (choice == 4) {
                    Merchant* m = dynamic_cast<Merchant*>(target);
                    if (m) {
                        m->onArrestedBy(*player);
                    } else {
                        player->arrest(*target);
                        General* g = dynamic_cast<General*>(target);
                        if (g) g->onArrested();
                    }
                } else if (choice == 5) {
                    player->sanction(*target);
                    Baron* b = dynamic_cast<Baron*>(target);
                    if (b) b->receiveSanctionFrom(*player);
                    Judge* j = dynamic_cast<Judge*>(target);
                    if (j) j->receiveSanctionFrom(*player);
                } else if (choice == 6) {
                    General* defender = dynamic_cast<General*>(target);
                    if (defender && defender->getCoins() >= 5) {
                        std::cout << defender->getName() << " blocked the coup using 5 coins!\n";
                        defender->blockCoup(*target, *player);
                    } else {
                        player->coup(*target);
                    }
                }
                break;
            }
            case 7:
                std::cout << "Skipping turn.\n";
                break;
            case 8: {
                Judge* j = dynamic_cast<Judge*>(player);
                if (j) tryBlockBribe(j);
                else std::cout << "You are not a Judge.\n";
                break;
            }
            case 9: {
                Governor* g = dynamic_cast<Governor*>(player);
                if (g) tryBlockTax(g);
                else std::cout << "You are not a Governor.\n";
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Action failed: " << e.what() << "\n";
    }

    nextTurn();
}
