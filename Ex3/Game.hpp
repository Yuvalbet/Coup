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

    std::vector<Player*> players;
    int currentTurnIndex;
    Player* sanctionedLastRound = nullptr;
    Player* lastArrestedPlayer;
    std::string lastActionMessage;


public:
    Game();

    void setCurrentTurnIndex(int index);

    void addPlayer(Player* player);
    void nextTurn();
    Player* currentPlayer() const;
    std::vector<std::string> getActivePlayerNames() const;
    std::string getWinner() const;

    void playTurn(int choice, Player* target = nullptr);

    void tryBlockBribe(Judge* judge);
    void tryBlockTax(Governor* governor);
    std::vector<Player*> getValidTargets(Player* current) const;

    void assignRandomRoles(const std::vector<std::string>& names);


    const std::vector<Player*>& getPlayers() const; // לשחרור זיכרון
    int getCurrentTurnIndex() const { return currentTurnIndex; }
    std::string getLastActionMessage() const;
    void setLastActionMessage(const std::string& msg);

};

#endif // GAME_HPP