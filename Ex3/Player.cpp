#include "Player.hpp"
#include <iostream> // חובה בשביל std::cout

// Constructor
Player::Player(const std::string& name)
    : name(name), coins(0), active(true), sanctioned(false), arrestedLastTurn(false) {}

// Getters
std::string Player::getName() const {
    return name;
}

int Player::getCoins() const {
    return coins;
}

bool Player::isActive() const {
    return active;
}

bool Player::isSanctioned() const {
    return sanctioned;
}

bool Player::wasArrestedLastTurn() const {
    return arrestedLastTurn;
}

// Setters
void Player::addCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot add negative coins.");
    }
    coins += amount;
}

void Player::removeCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot remove negative coins.");
    }
    if (coins < amount) {
        throw std::invalid_argument("Not enough coins to remove.");
    }
    coins -= amount;
}

void Player::deactivate() {
    active = false;
    std::cout << name << " has been eliminated from the game.\n";
}

void Player::setSanctioned(bool value) {
    sanctioned = value;
    if (value) {
        std::cout << name << " has been sanctioned and cannot use gather or tax.\n";
    } else {
        std::cout << name << "'s sanction has been lifted.\n";
    }
}

void Player::setArrestedLastTurn(bool value) {
    arrestedLastTurn = value;
}

// Actions
void Player::gather() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot gather.");
    }
    addCoins(1);
    std::cout << name << " gathered 1 coin.\n";
}

void Player::tax() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot use tax.");
    }
    addCoins(2);
    std::cout << name << " received 2 coins from tax.\n";
}

void Player::bribe() {
    removeCoins(4);
    std::cout << name << " paid 4 coins for a bribe and may perform an extra action.\n";
}

void Player::arrest(Player& other) {
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot arrest an inactive player.");
    }
    if (other.wasArrestedLastTurn()) {
        throw std::invalid_argument("Player was already arrested last turn.");
    }
    other.removeCoins(1);
    other.setArrestedLastTurn(true);
    std::cout << name << " arrested " << other.getName() << " and took 1 coin.\n";
}

void Player::sanction(Player& other) {
    if (coins < 3) {
        throw std::invalid_argument("Not enough coins to perform sanction.");
    }
    removeCoins(3);
    other.setSanctioned(true);
    std::cout << name << " sanctioned " << other.getName() << ".\n";
}

void Player::coup(Player& other) {
    if (coins < 7) {
        throw std::invalid_argument("Not enough coins to perform coup.");
    }
    removeCoins(7);
    other.deactivate();
    std::cout << name << " performed a coup on " << other.getName() << ".\n";
}
