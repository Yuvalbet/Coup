#include "Judge.hpp"
#include <iostream> 


Judge::Judge(const std::string& name) : Player(name) {}

std::string Judge::getRoleName() const {
    return "Judge";
}

void Judge::blockBribe(Player& target) {
    std::cout << target.getName() << "'s bribe was blocked by the Judge!\n";
}

void Judge::receiveSanctionFrom(Player& attacker) {
    attacker.removeCoins(1);
    std::cout << attacker.getName() << " had to pay an extra coin for sanctioning the Judge.\n";
}

