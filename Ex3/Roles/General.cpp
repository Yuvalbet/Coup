#include "General.hpp"
#include <iostream>


General::General(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // מאפס את כמות המטבעות
}

std::string General::getRoleName() const {
    return "General";
}

void General::blockCoup(Player& target, Player& attacker) {
    if (coins < 5) {
        throw std::invalid_argument("Not enough coins to block coup.");
    }

    removeCoins(5);
    std::cout << name << " blocked the coup on " << target.getName() << " and paid 5 coins.\n";
}

void General::onArrested() {
    addCoins(1);
    std::cout << name << " was arrested and got 1 coin back.\n";
}
