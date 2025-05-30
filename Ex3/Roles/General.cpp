//Email: yuvali532@gmail.com
#include "General.hpp"
#include <iostream>


General::General(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  
    }
std::string General::getRoleName() const {
    return "General";
}
// Allows the General to block a Coup action by paying 5 coins.
void General::blockCoup(Player& target, Player& attacker) {
    if (coins < 5) {
        throw std::runtime_error("General doesn't have enough coins to block.");
    }
    removeCoins(5); 
    std::cout << name << " blocked the coup attempt on " << target.getName() << "!\n";
}

//If the general is harmed by arrest, the coin status remains the same for both the general and the attacker.
void General::onArrested() {
    std::cout << name << " was arrested and got 1 coin back.\n";
}

void General::receiveArrestFrom(Player& attacker) {
    setArrestedLastTurn(true); 
    onArrested();               
}
 
