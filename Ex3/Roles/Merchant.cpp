//Email: yuvali532@gmail.com
#include "Merchant.hpp"
#include <iostream>



Merchant::Merchant(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  
}

std::string Merchant::getRoleName() const {
    return "Merchant";
}

//If the merchant starts a turn with at least 3 coins, he gets an additional coin for free.
void Merchant::onStartTurn() {
    if (coins >= 3) {
        addCoins(1);
        std::cout << name << " received a bonus coin for starting turn with ≥3 coins.\n";
    }
}

//If the merchant is attacked by an arrest, he pays 2 coins instead of giving one to the attacking player.
void Merchant::receiveArrestFrom(Player& attacker) {
    if (coins < 2) {
        throw std::runtime_error(name + " does not have 2 coins to be arrested.");
    }

    removeCoins(2);               
    setArrestedLastTurn(true);  
    std::cout << name << " was arrested and paid 2 coins (no coins given to " << attacker.getName() << ").\n";
}
