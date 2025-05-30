//Email: yuvali532@gmail.com
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <stdexcept>


// The Player class defines a generic player in the game.
// It includes common attributes and actions shared by all roles.
class Player {
    class Game;
protected:
    std::string name;
    int coins;
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
// Returns the player's name.
    std::string getName() const;
// Returns the number of coins the player has.
    int getCoins() const;
// Checks if the player is still active in the game.
    bool isActive() const;
// Checks if the player is currently under sanction.
    bool isSanctioned() const;
    bool isArrestBlocked() const;
    bool isRevealedBySpy() const;
    bool wasSpiedLastTurn() const;
    int arrestBlockedTurns = 0;


    // Setters
// Adds coins to the player's balance.
    void addCoins(int amount);
// Deducts coins from the player's balance.
    void removeCoins(int amount);
    void deactivate();
// Sets the player's sanction status.
    void setSanctioned(bool value);
    void setArrestedLastTurn(bool value);
    void updateArrestBlock(bool reset);
    void setRevealedBySpy(bool status);
    void setSpiedLastTurn(bool value);
    


    virtual std::string getRoleName() const = 0;
    virtual void receiveSanctionFrom(Player& attacker);  
    virtual void receiveArrestFrom(Player& attacker);
    virtual void onArrested();

    virtual void gather();          
    virtual void tax();                         
    virtual void bribe();                       
    virtual void arrest(Player& other);         
    virtual void sanction(Player& other);       
    virtual void coup(Player& other);           
};

#endif 
 