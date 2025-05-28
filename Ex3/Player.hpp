#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <stdexcept>


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
    Player(const std::string& name);
    virtual ~Player() = default;

    // Getters
    std::string getName() const;
    int getCoins() const;
    bool isActive() const;
    bool isSanctioned() const;
    bool wasArrestedLastTurn() const;
    bool isArrestBlocked() const;
    bool isRevealedBySpy() const;
    bool wasSpiedLastTurn() const;
    int arrestBlockedTurns = 0;


    // Setters
    void addCoins(int amount);
    void removeCoins(int amount);
    void deactivate();
    void setSanctioned(bool value);
    void setArrestedLastTurn(bool value);
    void updateArrestBlock(bool reset);
    void setRevealedBySpy(bool status);
    void setSpiedLastTurn(bool value);
    


    // תפקיד (מוחזר במחלקות יורשות)
    virtual std::string getRoleName() const = 0;
    virtual void receiveSanctionFrom(Player& attacker);  // מוסיף תגובה ייחודית ל־sanction
    virtual void receiveArrestFrom(Player& attacker);
    virtual void onArrested();

    // פעולות (ניתנות לשכתוב בתפקידים)
    virtual void gather();                      // איסוף מטבע אחד
    virtual void tax();                         // מיסוי – ברירת מחדל: 2 מטבעות
    virtual void bribe();                       // משלם 4 מטבעות
    virtual void arrest(Player& other);         // לוקח מטבע משחקן אחר
    virtual void sanction(Player& other);       // מטיל חרם
    virtual void coup(Player& other);           // מבצע הפיכה
};

#endif // PLAYER_HPP
 