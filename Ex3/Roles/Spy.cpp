//Email: yuvali532@gmail.com
#include "Spy.hpp"
#include <iostream>


Spy::Spy(const std::string& name) : Player(name) {}
    
std::string Spy::getRoleName() const {
    return "Spy";
}

// Spy action: reveals the target player's number of coins and prevents them from using the arrest on their next turn.
void Spy::spyOn(Player& other) {
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot spy on an inactive player.");
    }

    other.setRevealedBySpy(true);      
    other.setSpiedLastTurn(true);      

}