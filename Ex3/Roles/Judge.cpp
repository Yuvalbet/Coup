#include "Judge.hpp"
#include <iostream> 


Judge::Judge(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // [TODO: Hebrew comment replaced – write English version]
}

std::string Judge::getRoleName() const {
    return "Judge";
}

// Judge's bribe block: cancels bribe effect by not granting extra turns
bool Judge::blockBribe(Player& performer) {
    std::cout << performer.getName() << "'s bribe was blocked by the Judge!\n";
    // Bribe was blocked, no extra turns granted – actual reversal should be handled in Game if needed
    return true;
}

void Judge::receiveSanctionFrom(Player& attacker) {
    attacker.removeCoins(1);
    std::cout << attacker.getName() << " had to pay an extra coin for sanctioning the Judge.\n";
}
