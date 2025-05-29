#include "General.hpp"
#include <iostream>


General::General(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // [TODO: Hebrew comment replaced – write English version]
}

std::string General::getRoleName() const {
    return "General";
}
void General::blockCoup(Player& target, Player& attacker) {
    if (coins < 5) {
        throw std::runtime_error("General doesn't have enough coins to block.");
    }
    removeCoins(5);  // [TODO: Hebrew comment replaced – write English version]
    std::cout << name << " blocked the coup attempt on " << target.getName() << "!\n";
}


void General::onArrested() {
    std::cout << name << " was arrested and got 1 coin back.\n";
}

void General::receiveArrestFrom(Player& attacker) {
    setArrestedLastTurn(true);  // [TODO: Hebrew comment replaced – write English version]
    onArrested();               // [TODO: Hebrew comment replaced – write English version]
}
