// This file defines the Player base class used in the Coup game.
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <stdexcept>


// Base class for all player types; holds common player data and methods.
class Player {
    class Game;
protected:
    // Player's name
    std::string name;
    // Number of coins the player currently has
    int coins;
    // Whether the player is still active in the game
    bool active;
    bool sanctioned;
    bool arrestedLastTurn;
    bool revealedBySpy = false;
    bool spiedLastTurn = false;




public:
    // Constructor: initializes a player with a name, 0 coins, and sets them as active
    Player(const std::string& name);
    virtual ~Player() = default;

    // Getters
    std::string getName() const;
    // Gets the current number of coins the player has
    int getCoins() const;
    // Checks if the player is active in the game
    bool isActive() const;
    bool isSanctioned() const;
    bool isArrestBlocked() const;
    bool isRevealedBySpy() const;
    bool wasSpiedLastTurn() const;
    int arrestBlockedTurns = 0;


    // Setters
    // Adds a specified amount of coins to the player
    void addCoins(int amount);
    // Deducts a specified amount of coins from the player
    void removeCoins(int amount);
    void deactivate();
    void setSanctioned(bool value);
    void setArrestedLastTurn(bool value);
    void updateArrestBlock(bool reset);
    void setRevealedBySpy(bool status);
    void setSpiedLastTurn(bool value);
    


    // [TODO: Hebrew comment replaced – write English version]
    virtual std::string getRoleName() const = 0;
    virtual void receiveSanctionFrom(Player& attacker);  // [TODO: Hebrew comment replaced – write English version]
    virtual void receiveArrestFrom(Player& attacker);
    virtual void onArrested();

    // [TODO: Hebrew comment replaced – write English version]
    virtual void gather();                      // [TODO: Hebrew comment replaced – write English version]
    virtual void tax();                         // [TODO: Hebrew comment replaced – write English version]
    virtual void bribe();                       // [TODO: Hebrew comment replaced – write English version]
    virtual void arrest(Player& other);         // [TODO: Hebrew comment replaced – write English version]
    virtual void sanction(Player& other);       // [TODO: Hebrew comment replaced – write English version]
    virtual void coup(Player& other);           // [TODO: Hebrew comment replaced – write English version]
};

#endif // PLAYER_HPP
 