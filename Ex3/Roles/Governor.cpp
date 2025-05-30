//Email: yuvali532@gmail.com
#include "Governor.hpp"
#include "../Game.hpp" 
#include <iostream>


Governor::Governor(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  
}

std::string Governor::getRoleName() const {
    return "Governor";
}

// Governor's tax action: adds 3 coins to the player performing it
void Governor::tax() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot use tax.");
    }
    addCoins(3);
    std::cout << name << " received 3 coins from tax (Governor bonus).\n";
}

// Allows the Governor to block a Tax action performed by another player.
void Governor::blockTax(Player& target) {
    std::cout << target.getName() << "'s tax was blocked by the Governor!\n";
    target.removeCoins(2);
}