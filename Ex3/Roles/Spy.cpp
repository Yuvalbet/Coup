#include "Spy.hpp"
#include <iostream>


Spy::Spy(const std::string& name) : Player(name) {}
    
std::string Spy::getRoleName() const {
    return "Spy";
}

void Spy::spyOn(Player& other) {
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot spy on an inactive player.");
    }

    other.setRevealedBySpy(true);       // לחשוף ל־GUI
    other.setSpiedLastTurn(true);       // ✅ לחסום Arrest בתור הבא

    std::cout << "[DEBUG] " << other.getName() << " was spied on – blocking ARREST next turn" << std::endl;
}

