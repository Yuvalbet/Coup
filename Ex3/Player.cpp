#include "Player.hpp"
#include <iostream>

// Constructor
Player::Player(const std::string& name)
    : name(name), coins(0), active(true), sanctioned(false),arrestBlockedTurns(0) {}

// Getters
std::string Player::getName() const {
    return name;
}

int Player::getCoins() const {
    return coins;
}

bool Player::isActive() const {
    return active;
}

bool Player::isSanctioned() const {
    return sanctioned;
}

bool Player::isArrestBlocked() const {
    return arrestBlockedTurns > 0;
}

// Setters
void Player::addCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot add negative coins.");
    }
    coins += amount;
}

void Player::removeCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot remove negative coins.");
    }
    if (coins < amount) {
        throw std::invalid_argument("Not enough coins to remove.");
    }
    coins -= amount;
}

void Player::deactivate() {
    active = false;
    std::cout << name << " has been eliminated from the game.\n";
}

void Player::setSanctioned(bool value) {
    sanctioned = value;
}

void Player::setArrestedLastTurn(bool value) {
    arrestedLastTurn = value;
}


void Player::updateArrestBlock(bool reset) {
    if (reset) {
        arrestBlockedTurns = 1; // קביעת חסימה לתור אחד
    } else if (arrestBlockedTurns > 0) {
        arrestBlockedTurns--;   // הפחתה בתום תור
    }
}

void Player::receiveSanctionFrom(Player& attacker) {
    // ברירת מחדל – לא עושה כלום
}




// Actions
void Player::gather() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot gather.");
    }
    addCoins(1);
    std::cout << name << " gathered 1 coin.\n";
     std::cout << "[DEBUG] " << name << " now has " << coins << " coins.\n";

}

void Player::tax() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot use tax.");
    }
    addCoins(2);
    
    std::cout << name << " received 2 coins from tax.\n";
}

void Player::bribe() {
    removeCoins(4);
    std::cout << name << " paid 4 coins for a bribe and may perform an extra action.\n";
}

void Player::arrest(Player& other) {
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot arrest an inactive player.");
    }

    other.receiveArrestFrom(*this);  // ✅ ככה ניתנת שליטה לתפקיד
    std::cout << name << " arrested " << other.getName() << ".\n";
}


void Player::sanction(Player& other) {
    if (coins < 3) {
        throw std::invalid_argument("Not enough coins to perform sanction.");
    }
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot sanction an inactive player.");
    }

    removeCoins(3);
    other.setSanctioned(true);
    std::cout << name << " sanctioned " << other.getName() << ".\n";

    // ✅ הוספה: אם ל־other יש תגובה מיוחדת ל־sanction (כמו השופט), היא תופעל כאן
    other.receiveSanctionFrom(*this);
}


void Player::coup(Player& other) {
    if (coins < 7) {
        throw std::invalid_argument("Not enough coins to perform coup.");
    }
    removeCoins(7);
    other.deactivate();
    std::cout << name << " performed a coup on " << other.getName() << ".\n";
}

void Player::receiveArrestFrom(Player& attacker) {
    if (coins == 0) {
        throw std::runtime_error(name + " has no coins to be arrested.");
    }

    removeCoins(1);         // יורד מהשחקן שנעצר
    setArrestedLastTurn(true);
    onArrested();
}

void Player::onArrested() {
    // ברירת מחדל – לא עושה כלום
}

