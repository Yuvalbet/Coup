#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <stdexcept>

class Player {
protected:
    std::string name;
    int coins;
    bool active;
    bool sanctioned;
    bool arrestedLastTurn;

public:
    Player(const std::string& name);
    virtual ~Player() = default;

    // Getters
    std::string getName() const;
    int getCoins() const;
    bool isActive() const;
    bool isSanctioned() const;
    bool wasArrestedLastTurn() const;

    // Setters
    void addCoins(int amount);
    void removeCoins(int amount);
    void deactivate();
    void setSanctioned(bool value);
    void setArrestedLastTurn(bool value);

    // תפקיד (מוחזר במחלקות יורשות)
    virtual std::string getRoleName() const = 0;

    // פעולות (ניתנות לשכתוב בתפקידים)
    virtual void gather();                      // איסוף מטבע אחד
    virtual void tax();                         // מיסוי – ברירת מחדל: 2 מטבעות
    virtual void bribe();                       // משלם 4 מטבעות
    virtual void arrest(Player& other);         // לוקח מטבע משחקן אחר
    virtual void sanction(Player& other);       // מטיל חרם
    virtual void coup(Player& other);           // מבצע הפיכה
};

#endif // PLAYER_HPP
