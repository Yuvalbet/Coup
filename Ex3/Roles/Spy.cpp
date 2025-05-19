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

    std::cout << name << " spied on " << other.getName()
              << " who has " << other.getCoins() << " coins.\n";

    std::cout << other.getName() << " will be prevented from arresting next turn.\n";
}
