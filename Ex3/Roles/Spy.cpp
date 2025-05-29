#include "Spy.hpp"
#include <iostream>


Spy::Spy(const std::string& name) : Player(name) {}
    
std::string Spy::getRoleName() const {
    return "Spy";
}

// Spy action: reveals target player's coin count and blocks their next arrest
void Spy::spyOn(Player& other) {
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot spy on an inactive player.");
    }

    other.setRevealedBySpy(true);       // [TODO: Hebrew comment replaced – write English version]
    other.setSpiedLastTurn(true);       // [TODO: Hebrew comment replaced – write English version]

    std::cout << "[DEBUG] " << other.getName() << " was spied on – blocking ARREST next turn" << std::endl;
}
