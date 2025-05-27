#include "Merchant.hpp"
#include <iostream>



Merchant::Merchant(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // מאפס את כמות המטבעות
}

std::string Merchant::getRoleName() const {
    return "Merchant";
}

// כשהסוחר מתחיל תור עם לפחות 3 מטבעות – הוא מקבל אחד נוסף
void Merchant::onStartTurn() {
    if (coins >= 3) {
        addCoins(1);
        std::cout << name << " received a bonus coin for starting turn with ≥3 coins.\n";
    }
}

void Merchant::receiveArrestFrom(Player& attacker) {
    if (coins < 2) {
        throw std::runtime_error(name + " does not have 2 coins to be arrested.");
    }

    removeCoins(2);               // ❗ יורדים 2 מטבעות מהסוחר
    setArrestedLastTurn(true);    // ✔️ נכנס למעצר
    std::cout << name << " was arrested and paid 2 coins (no coins given to " << attacker.getName() << ").\n";
}

