//Email: yuvali532@gmail.com
#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Roles/Merchant.hpp"
#include "Roles/Judge.hpp"
#include "Roles/General.hpp"
#include "Roles/Governor.hpp"

#include <vector>
#include <string>

// The Game class manages the overall game state, turn logic, and interactions between players.
class Game {
private:

    // List of all players in the game
    std::vector<Player*> playerList;
    // Index of the player whose turn it currently is
    int currentTurnIndex;
    // Player who was sanctioned in the previous round
    Player* sanctionedLastRound = nullptr;
    // Player who was last arrested
    Player* lastArrestedPlayer;
    // Description of the last action that occurred (for GUI display)
    std::string lastActionMessage;
    // Extra turns awarded due to bribe action
    int pendingExtraTurns = 0;  

    // List of pending action types to be resolved 
    std::vector<std::string> pendingActionTypes;
    // List of players who performed pending actions
    std::vector<Player*> pendingPerformers;
    // Targets of the pending actions (can be null)
    std::vector<Player*> pendingTargets; 




public:
    // Constructor that initializes game state and variables
    Game();

    // Destructor: cleans up any dynamically allocated resources
    ~Game();

// Sets the index of the current player's turn.
    void setCurrentTurnIndex(int index);

// Adds a new player to the game.
    void addPlayer(Player* player);
    void turn() const; 
    
// Moves the game to the next active player's turn.
    void nextTurn();
// Returns the player whose turn it is.
    Player* currentPlayer() const;
    std::vector<std::string> players() const;
// Returns the winner of the game if only one player is left.
    std::string winner() const;

// Executes a player's chosen action for their turn.
    void playTurn(int choice, Player* target = nullptr);
// Attempts to block a Bribe action if a Judge is present.
    void tryBlockBribe(Judge* judge);
// Attempts to block a Tax action if a Governor is present.
    void tryBlockTax(Governor* governor);
// Attempts to block a Coup action if a General is present.
    bool tryBlockCoup(Player* source, Player* target);
    
// Returns a list of valid players that can be targeted for an action.
    std::vector<Player*> getValidTargets(Player* current) const;

// Assigns roles randomly to each player at the start of the game.
    void assignRandomRoles(const std::vector<std::string>& names);

// Returns a reference to the list of players.
    const std::vector<Player*>& getPlayers() const; 
    int getCurrentTurnIndex() const { return currentTurnIndex; }
// Returns the description of the most recent action.
    std::string getLastActionMessage() const;
// Sets the message describing the last action taken.
    void setLastActionMessage(const std::string& msg);
// Grants additional turns to the current player.
    void addExtraTurns(int count); 



};

#endif // GAME_HPP