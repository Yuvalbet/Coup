//Email: yuvali532@gmail.com
#include "Judge.hpp"
#include <iostream> 


Judge::Judge(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins()); 
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

//If the Judge is attacked with a sanction, the player who imposed the sanction must pay an additional coin
void Judge::receiveSanctionFrom(Player& attacker) {
    attacker.removeCoins(1);
    std::cout << attacker.getName() << " had to pay an extra coin for sanctioning the Judge.\n";
}