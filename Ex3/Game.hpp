#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Roles/Merchant.hpp"
#include "Roles/Judge.hpp"
#include "Roles/General.hpp"
#include "Roles/Governor.hpp"

#include <vector>
#include <string>

class Game {
private:
    struct BlockableAction {
        Player* performer;
        std::string type; // "bribe", "tax"
    };

    std::vector<Player*> players;
    int currentTurnIndex;
    int coinBank;
    std::vector<BlockableAction> pendingActions;

    std::vector<Player*> getValidTargets(Player* current) const;

public:
    Game();

    void addPlayer(Player* player);
    void nextTurn();
    Player* currentPlayer() const;
    std::vector<std::string> getActivePlayerNames() const;
    std::string getWinner() const;

    void addToBank(int amount);
    void removeFromBank(int amount);
    int getBankCoins() const;

    void playTurn();

    void addBlockableAction(Player* performer, const std::string& type);
    void tryBlockBribe(Judge* judge);
    void tryBlockTax(Governor* governor);

    const std::vector<Player*>& getPlayers() const; // לשחרור זיכרון
};

#endif // GAME_HPP
