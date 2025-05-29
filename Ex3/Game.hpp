// Header file for the Game class which manages game state, turns, and player actions
#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Roles/Merchant.hpp"
#include "Roles/Judge.hpp"
#include "Roles/General.hpp"
#include "Roles/Governor.hpp"

#include <vector>
#include <string>

// Game class manages all players, tracks turns, and handles the game logic and rules
class Game {
private:

    // List of all players in the game
    std::vector<Player*> playerList;
    // Index of the player whose turn it currently is
    int currentTurnIndex;
    // Tracks which player was sanctioned in the last round (if any)
    Player* sanctionedLastRound = nullptr;
    // Tracks the last player who was arrested
    Player* lastArrestedPlayer;
    // Message describing the most recent action taken in the game
    std::string lastActionMessage;
    // Number of additional turns granted by specific actions like bribe
    int pendingExtraTurns = 0;  // [TODO: Hebrew comment replaced – write English version]

    // Stores action types currently pending (used for block handling)
    std::vector<std::string> pendingActionTypes;
    // List of players who performed pending actions
    std::vector<Player*> pendingPerformers;
    // Targets of the pending actions (can be null)
    std::vector<Player*> pendingTargets; // [TODO: Hebrew comment replaced – write English version]





public:
    // Constructor that initializes game state and variables
    Game();

    void setCurrentTurnIndex(int index);

    // Adds a new player to the game
    void addPlayer(Player* player);
    void turn() const; 
    
    // Advances the turn to the next active player
    void nextTurn();
    // Returns the current player taking their turn
    Player* currentPlayer() const;
    std::vector<std::string> players() const;
    std::string winner() const;

    // Processes the player's selected action for the current turn
    void playTurn(int choice, Player* target = nullptr);
    void tryBlockBribe(Judge* judge);
    void tryBlockTax(Governor* governor);
    bool tryBlockCoup(Player* source, Player* target);
    void addSpyBlockedPlayer(Player* p);

    std::vector<Player*> getValidTargets(Player* current) const;

    void assignRandomRoles(const std::vector<std::string>& names);


    // Returns the current list of players
    const std::vector<Player*>& getPlayers() const; // [TODO: Hebrew comment replaced – write English version]
    int getCurrentTurnIndex() const { return currentTurnIndex; }
    // Returns the most recent action message
    std::string getLastActionMessage() const;
    // Sets the latest action message
    void setLastActionMessage(const std::string& msg);
    void addExtraTurns(int count); // [TODO: Hebrew comment replaced – write English version]



};

#endif // GAME_HPP