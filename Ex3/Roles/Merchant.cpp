#include "Merchant.hpp"
#include <iostream>



Merchant::Merchant(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // [TODO: Hebrew comment replaced – write English version]
}

std::string Merchant::getRoleName() const {
    return "Merchant";
}

// [TODO: Hebrew comment replaced – write English version]
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

    removeCoins(2);               // [TODO: Hebrew comment replaced – write English version]
    setArrestedLastTurn(true);    // [TODO: Hebrew comment replaced – write English version]
    std::cout << name << " was arrested and paid 2 coins (no coins given to " << attacker.getName() << ").\n";
}

