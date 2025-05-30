//Email: yuvali532@gmail.com
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

// Constructor: Initializes the game state and variables.
Game::Game() : currentTurnIndex(0), sanctionedLastRound(nullptr), lastArrestedPlayer(nullptr) {}

// Destructor: cleans up any dynamically allocated resources
Game::~Game() {
    for (Player* p : playerList) {
        delete p;
    }
    playerList.clear();
}

// Adds a new player to the game.
void Game::addPlayer(Player* player) {
    playerList.push_back(player);
}

// Returns a reference to the list of players in the game.
const std::vector<Player*>& Game::getPlayers() const {
    return playerList;
}

// Advances the game to the next active player's turn.
void Game::nextTurn() {
    if (pendingExtraTurns > 0) {
        pendingExtraTurns--;
        return;
    }

    Player* previousPlayer = playerList[currentTurnIndex];

    do {
        currentTurnIndex = (currentTurnIndex + 1) % playerList.size();
    } while (!playerList[currentTurnIndex]->isActive());

    previousPlayer->setSpiedLastTurn(false);

    if (playerList[currentTurnIndex] == sanctionedLastRound) {
        playerList[currentTurnIndex]->setSanctioned(true);
        sanctionedLastRound = nullptr;
    } else {
        playerList[currentTurnIndex]->setSanctioned(false);
    }

    Merchant* merchant = dynamic_cast<Merchant*>(playerList[currentTurnIndex]);
    if (merchant && merchant->getCoins() >= 3) {
        merchant->onStartTurn();
    }

    for (Player* p : playerList) {
        if (p->isActive()) {
            p->updateArrestBlock(false);
        }
    }
}



// Returns the current player whose turn it is
Player* Game::currentPlayer() const {
    return playerList[currentTurnIndex];
}

std::vector<std::string> Game::players() const {
    std::vector<std::string> names;
    for (Player* p : playerList) {
        if (p->isActive()) {
            names.push_back(p->getName());
        }
    }
    return names;
}


// Returns the name of the winning player, if the game is over.
std::string Game::winner() const {
    std::string winner;
    int activeCount = 0;
    for (Player* p : playerList) {
        if (p->isActive()) {
            winner = p->getName();
            activeCount++;
        }
    }
    if (activeCount != 1) {
        throw std::runtime_error("Game is not over yet.");
    }

    std::cout << " Winner is: " << winner  << std::endl;  
    return winner;
}


// Sets the current turn index to a specific value.
void Game::setCurrentTurnIndex(int index) {
    currentTurnIndex = index;
}




// Handles the logic for attempting to block a bribe action.
void Game::tryBlockBribe(Judge* judge) {
    for (size_t i = 0; i < pendingActionTypes.size(); ++i) {
    if (pendingActionTypes[i] == "bribe") {
            bool blocked = judge->blockBribe(*pendingPerformers[i]);
            if (!blocked) {
                addExtraTurns(2);
            }
        pendingActionTypes.erase(pendingActionTypes.begin() + i);
        pendingPerformers.erase(pendingPerformers.begin() + i);
        pendingTargets.erase(pendingTargets.begin() + i);
        return;
    }
}

    std::cout << "No bribe action to block.\n";
}


// Handles the logic for attempting to block a tax action.
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
// Handles the logic for attempting to block a coup action.
bool Game::tryBlockCoup(Player* source, Player* target) {
    for (Player* p : playerList) {
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


// Handles the current player's action based on the chosen option
void Game::playTurn(int choice, Player* target) {
    turn();
    Player* player = currentPlayer();
    bool turnConsumed = false;  

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
                }
                else if (dynamic_cast<General*>(target)) {
                }
                else {
                    player->addCoins(1);  
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

                player->removeCoins(7); 

                bool hasGeneral = false;
                for (Player* p : playerList) {
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
                    return; 
                } else {
                    target->deactivate(); 
                    turn();
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
                    lastArrestedPlayer = target;
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


if (turnConsumed) {
    for (size_t i = 0; i < pendingActionTypes.size(); ++i) {
        if (pendingActionTypes[i] == "bribe" && pendingPerformers[i] == player) {
            addExtraTurns(2);
            pendingActionTypes.erase(pendingActionTypes.begin() + i);
            pendingPerformers.erase(pendingPerformers.begin() + i);
            pendingTargets.erase(pendingTargets.begin() + i);
            break;
        }
    }

    if (pendingExtraTurns > 0) {
        pendingExtraTurns--;
        std::cout << " BRIBE TURN used – still got more (" << pendingExtraTurns << " left)\n";
    } else {
        std::cout << " Regular turn used – moving to next player\n";
        nextTurn();
        try {
            std::string winningPlayer = winner();
            setLastActionMessage("🏆 Game Over! Winner: " + winner());
        } catch (...) {}
    }
}



}


// Returns a list of players that can be targeted by an action.
std::vector<Player*> Game::getValidTargets(Player* current) const {
    std::vector<Player*> targets;
    for (Player* p : playerList) {
        if (p != current && p->isActive()) {
            targets.push_back(p);
        }
    }
    return targets;
}

// Randomly assigns roles to all players at the start of the game.
void Game::assignRandomRoles(const std::vector<std::string>& names) {
    std::srand(static_cast<unsigned int>(time(nullptr)));

    for (const std::string& name : names) {
        int r = std::rand() % 6;  
        Player* player = nullptr;

        switch (r) {
            case 0: player = new Governor(name); break;
            case 1: player = new Spy(name); break;
            case 2: player = new Baron(name); break;
            case 3: player = new General(name); break;
            case 4: player = new Judge(name); break;
            case 5: player = new Merchant(name); break;
        }
        player->removeCoins(player->getCoins()); 

        this->addPlayer(player);  

    }

    currentTurnIndex = 0;


}
// Returns the message describing the last action taken in the game.
std::string Game::getLastActionMessage() const {
    return lastActionMessage;
}

// Sets the message that describes the most recent game action.
void Game::setLastActionMessage(const std::string& msg) {
    lastActionMessage = msg;
}

// Adds extra turns to the current player.
void Game::addExtraTurns(int count) {
    pendingExtraTurns += count;
}

void Game::turn() const {
    if (currentTurnIndex >= 0 && currentTurnIndex < playerList.size() && playerList[currentTurnIndex]->isActive()) {
        std::cout << "It's " << playerList[currentTurnIndex]->getName() << "'s turn.\n";
    } else {
        std::cout << "No active player for current turn.\n";
    }

    std::cout << "Active players: ";
    for (const std::string& name : players()) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    try {
    std::string win = winner();
    std::cout << "🏆 Winner is: " << win << std::endl;
} catch (const std::exception& e) {
    std::cout << " " << e.what() << std::endl;
}


}


