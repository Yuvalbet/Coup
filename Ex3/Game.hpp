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
    int pendingExtraTurns = 0;  // בתוך private

    std::vector<std::string> pendingActionTypes;
    std::vector<Player*> pendingPerformers;
    std::vector<Player*> pendingTargets; // יכול להיות nullptr





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
    bool tryBlockCoup(Player* source, Player* target);
    void addSpyBlockedPlayer(Player* p);

    std::vector<Player*> getValidTargets(Player* current) const;

    void assignRandomRoles(const std::vector<std::string>& names);


    const std::vector<Player*>& getPlayers() const; // לשחרור זיכרון
    int getCurrentTurnIndex() const { return currentTurnIndex; }
    std::string getLastActionMessage() const;
    void setLastActionMessage(const std::string& msg);
    void addExtraTurns(int count); // ✅ פונקציה חדשה לצבירת תורות נוספים



};

#endif // GAME_HPP