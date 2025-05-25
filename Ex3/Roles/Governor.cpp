#include "Governor.hpp"
#include <iostream>


Governor::Governor(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // מאפס את כמות המטבעות
}

std::string Governor::getRoleName() const {
    return "Governor";
}

void Governor::tax() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot use tax.");
    }
    addCoins(3);
    std::cout << name << " received 3 coins from tax (Governor bonus).\n";
}

void Governor::blockTax(Player& target) {
    std::cout << target.getName() << "'s tax was blocked by the Governor!\n";
    target.removeCoins(2);
}
