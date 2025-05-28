#include "Judge.hpp"
#include <iostream> 


Judge::Judge(const std::string& name) : Player(name) {
    this->removeCoins(this->getCoins());  // מאפס את כמות המטבעות
}

std::string Judge::getRoleName() const {
    return "Judge";
}

bool Judge::blockBribe(Player& performer) {
    // אם חסם – מחזיר true, אם לא – false
    std::cout << "Do you want to block the bribe? (y/n): ";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
        std::cout << "Bribe was blocked.\n";
        return true;
    } else {
        std::cout << "Bribe not blocked.\n";
        return false;
    }
}


void Judge::receiveSanctionFrom(Player& attacker) {
    attacker.removeCoins(1);
    std::cout << attacker.getName() << " had to pay an extra coin for sanctioning the Judge.\n";
}

