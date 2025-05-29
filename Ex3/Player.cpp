#include "Player.hpp"
#include <iostream>

// Constructor
// Constructor initializes player with name, 0 coins, and sets them as active
Player::Player(const std::string& name)
    : name(name), coins(0), active(true), sanctioned(false),arrestBlockedTurns(0) {}

// Getters
std::string Player::getName() const {
    return name;
}

// Returns the player's current coin count
int Player::getCoins() const {
    return coins;
}

// Returns true if the player is still active in the game
bool Player::isActive() const {
    return active;
}

bool Player::isSanctioned() const {
    return sanctioned;
}

bool Player::isArrestBlocked() const {
    return arrestBlockedTurns > 0;
}

// Setters
// Adds a given number of coins to the player
void Player::addCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot add negative coins.");
    }
    coins += amount;
}

// Removes a given number of coins; throws an error if insufficient funds
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
}

void Player::setArrestedLastTurn(bool value) {
    arrestedLastTurn = value;
}


void Player::updateArrestBlock(bool reset) {
    if (reset) {
        arrestBlockedTurns = 1;
    } else {
        arrestBlockedTurns = std::max(0, arrestBlockedTurns - 1);
    }

    std::cout << "[DEBUG] updateArrestBlock: " << name
              << " => arrestBlockedTurns = " << arrestBlockedTurns << std::endl;
}


void Player::receiveSanctionFrom(Player& attacker) {
    // [TODO: Hebrew comment replaced – write English version]
}

void Player::setRevealedBySpy(bool val) {
    revealedBySpy = val;
}


// Actions
void Player::gather() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot gather.");
    }
    addCoins(1);
    std::cout << name << " gathered 1 coin.\n";
     std::cout << "[DEBUG] " << name << " now has " << coins << " coins.\n";

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

    // [TODO: Hebrew comment replaced – write English version]
    std::cout << "[DEBUG] " << name << " wasSpiedLastTurn = "
              << (this->wasSpiedLastTurn() ? "YES" : "NO") << std::endl;

    if (this->isArrestBlocked() || this->wasSpiedLastTurn()) {
        std::cout << "[DEBUG] " << name << " tried to arrest but is blocked!\n";
        throw std::runtime_error(name + " is blocked from using arrest this turn.");
    }

    other.receiveArrestFrom(*this);
    std::cout << name << " arrested " << other.getName() << ".\n";
}

void Player::sanction(Player& other) {
    if (coins < 3) {
        throw std::invalid_argument("Not enough coins to perform sanction.");
    }
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot sanction an inactive player.");
    }

    removeCoins(3);
    other.setSanctioned(true);
    std::cout << name << " sanctioned " << other.getName() << ".\n";

    // [TODO: Hebrew comment replaced – write English version]
    other.receiveSanctionFrom(*this);
}


void Player::coup(Player& other) {
    if (coins < 7) {
        throw std::invalid_argument("Not enough coins to perform coup.");
    }
    removeCoins(7);
    other.deactivate();
    std::cout << name << " performed a coup on " << other.getName() << ".\n";
}

void Player::receiveArrestFrom(Player& attacker) {
    if (coins == 0) {
        throw std::runtime_error(name + " has no coins to be arrested.");
    }

    removeCoins(1);         // [TODO: Hebrew comment replaced – write English version]
    setArrestedLastTurn(true);
    onArrested();
}

void Player::onArrested() {
    // [TODO: Hebrew comment replaced – write English version]
}


bool Player::isRevealedBySpy() const {
    return revealedBySpy;
}

bool Player::wasSpiedLastTurn() const {
    return spiedLastTurn;
}

void Player::setSpiedLastTurn(bool value) {
    spiedLastTurn = value;
}


