#include "Baron.hpp"
#include <iostream>


Baron::Baron(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // מאפס את כמות המטבעות
}

std::string Baron::getRoleName() const {
    return "Baron";
}

void Baron::invest() {
    if (coins < 3) {
        throw std::invalid_argument("Not enough coins to invest.");
    }
    removeCoins(3);
    addCoins(6);
    std::cout << name << " invested 3 coins and received 6 coins.\n";
}

void Baron::receiveSanctionFrom(Player& attacker) {
    addCoins(1);
    std::cout << name << " received 1 coin as compensation for being sanctioned.\n";
}
