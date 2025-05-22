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



void Game::playTurn() {
    Player* player = currentPlayer();
    int extraActions = 1;

    while (extraActions > 0) {
        std::cout << "\n== " << player->getName() << "'s turn (" << player->getRoleName() << ") ==\n";
        std::cout << "Coins: " << player->getCoins() << "\n";

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
                player->setSanctioned(false);
            } catch (const std::exception& e) {
                std::cerr << "Coup failed: " << e.what() << "\n";
                continue;
            }
            nextTurn();
            return;
        }

        std::cout << "Choose an action:\n";
        std::cout << "1. Gather\n";
        std::cout << "2. Tax\n";
        std::cout << "3. Bribe (extra actions)\n";
        std::cout << "4. Arrest another player\n";
        std::cout << "5. Sanction another player\n";
        std::cout << "6. Coup another player\n";

        bool isJudge = dynamic_cast<Judge*>(player);
        bool isGovernor = dynamic_cast<Governor*>(player);
        bool isSpy = dynamic_cast<Spy*>(player);
        bool isBaron = dynamic_cast<Baron*>(player);
        
        if (isBaron) std::cout << "8. Invest (Baron only)\n";
        if (isSpy) std::cout << "9. Spy on another player\n";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 9 && !isSpy) {
            std::cout << "Invalid choice for your role. Try again.\n";
            continue;
        }

        if ((choice == 3 && player->getCoins() < 4) ||
            (choice == 5 && player->getCoins() < 3) ||
            (choice == 6 && player->getCoins() < 7)) {
            std::cout << "Not enough coins for that action. Try again.\n";
            continue;
        }

        if (player->isSanctioned() && (choice == 1 || choice == 2)) {
            std::cout << "You are sanctioned and cannot use gather or tax this turn.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1:
                    player->gather();
                    break;
                case 2: 
                    player->tax();
                    for (Player* p : players) {
                        if (p == player || !p->isActive()) continue;

                        if (Governor* g = dynamic_cast<Governor*>(p)) {
                            std::cout << g->getName() << ", do you want to block the tax? (1 = yes, 0 = no): ";
                            int response;
                            std::cin >> response;

                            if (response == 1) {
                                g->blockTax(*player);
                                break;
                            }
                        }
                    }
                

                    break;
                


                case 3:
                    player->bribe();
                    for (Player* p : players) {
                        if (p == player || !p->isActive()) continue;

                        if (Judge* g = dynamic_cast<Judge*>(p)) {
                            std::cout << g->getName() << ", do you want to block the bribe? (1 = yes, 0 = no): ";
                            int response;
                            std::cin >> response;

                            if (response == 1) {
                                g->blockBribe(*player);
                                break;
                            }
                            else{
                                extraActions += 2;
                            }
                        }
                    }
                    

                    break;
                case 8: {
                    Baron* baron = dynamic_cast<Baron*>(player);
                    if (!baron) {
                        std::cout << "Invalid choice for your role. Try again.\n";
                        continue;
                    }
                    try {
                        baron->invest();
                    } catch (const std::exception& e) {
                        std::cerr << "Investment failed: " << e.what() << "\n";
                        continue;
                    }
                    break;
                }
                        
                case 4:
                case 5:
                case 6:
                case 9: {
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
                        if (player->isArrestBlocked()) {
                            std::cout << player->getName() << " is under surveillance and cannot perform arrest this turn.\n";
                                continue;
                            }

                        if (target == lastArrestedPlayer) {
                            std::cout << "You cannot arrest the same player consecutively. Try another target.\n";
                            continue;
                        }

                     

                        try {
                            if (auto* m = dynamic_cast<Merchant*>(target)) {
                                m->onArrestedBy(*player);  // ייתכן שתיזרק כאן שגיאה
                            } else {
                                player->arrest(*target);
                                if (auto* g = dynamic_cast<General*>(target)) {
                                    g->onArrested();
                                }
                            }

                            lastArrestedPlayer = target;
                        } catch (const std::exception& e) {
                            std::cerr << "Arrest failed: " << e.what() << "\n";
                            continue;  // חוזר לתחילת הלולאה – אותו שחקן בוחר שוב פעולה
                        }

                    }
                        else if (choice == 5) {
                        player->sanction(*target);
                        sanctionedLastRound = target;
                        if (auto* b = dynamic_cast<Baron*>(target)) b->receiveSanctionFrom(*player);
                        if (auto* j = dynamic_cast<Judge*>(target)) j->receiveSanctionFrom(*player);
                    } else if (choice == 6) {
                        // קודם כל מוודאים שלשחקן יש מספיק מטבעות
                        if (player->getCoins() < 7) {
                            std::cout << "Not enough coins to perform a coup.\n";
                            continue;
                        }

                        // שלם את מחיר ההפיכה בכל מקרה
                        player->removeCoins(7);

                        // נבדוק אם מישהו חוסם
                        bool blocked = false;

                        for (Player* p : players) {
                            if (!p->isActive() || p == player) continue;

                            if (auto* g = dynamic_cast<General*>(p); g && g->getCoins() >= 5) {
                                std::cout << g->getName() << ", do you want to block the coup on " << target->getName() << "? (1 = yes, 0 = no): ";
                                int response;
                                std::cin >> response;

                                if (response == 1) {
                                    g->blockCoup(*target, *player);  // מבצע את חסימת ההפיכה
                                    blocked = true;
                                    break;
                                }
                            }
                        }

                        // אם לא נחסמה – ההפיכה מתבצעת
                        if (!blocked) {
                            player->coup(*target);
                        } else {
                            std::cout << player->getName() << " paid for the coup, but it was blocked.\n";
                        }
                    } else if (choice == 9) {
                        if (auto* s = dynamic_cast<Spy*>(player)) s->spyOn(*target);
                        continue;
                    }
                    break;
                }
                
                default:
                    std::cout << "Invalid choice. Try again.\n";
                    continue;
            }
        } 
        catch (const std::exception& e) {
            std::cerr << "Action failed: " << e.what() << "\n";
            continue;
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

        this->addPlayer(player);  // מוסיפים את השחקן למשחק
    }
}
