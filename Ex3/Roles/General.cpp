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
        throw std::runtime_error("General doesn't have enough coins to block.");
    }
    removeCoins(5);  // ✅ משלמים עבור החסימה
    std::cout << name << " blocked the coup attempt on " << target.getName() << "!\n";
}


void General::onArrested() {
    addCoins(1);
    std::cout << name << " was arrested and got 1 coin back.\n";
}
