#include "Merchant.hpp"
#include <iostream>



Merchant::Merchant(const std::string& name) : Player(name) {}

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

void Merchant::onArrestedBy(Player& attacker) {
    if (coins < 2) {
        throw std::invalid_argument("Merchant does not have enough coins to pay for arrest.");
    }
    removeCoins(2);
    std::cout << name << " was arrested and paid 2 coins (no coins transferred to attacker).\n";
}
