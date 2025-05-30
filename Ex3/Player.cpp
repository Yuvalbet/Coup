//Email: yuvali532@gmail.com
#include "Player.hpp"
#include <iostream>

// Constructor: Initializes a player with a name and 0 coins.
Player::Player(const std::string& name)
    : name(name), coins(0), active(true), sanctioned(false),arrestBlockedTurns(0) {}

// Getters
// Returns the player's name.
std::string Player::getName() const {
    return name;
}

// Returns the current number of coins the player has.
int Player::getCoins() const {
    return coins;
}

// Returns true if the player is still in the game.
bool Player::isActive() const {
    return active;
}

// Checks if the player is currently sanctioned.
bool Player::isSanctioned() const {
    return sanctioned;
}

// Returns true if the player is currently blocked from arresting due to temporary arrest block
bool Player::isArrestBlocked() const {
    return arrestBlockedTurns > 0;
}

// Setters
// Adds coins to the player's total.
void Player::addCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot add negative coins.");
    }
    coins += amount;
}

// Removes coins from the player's total.
void Player::removeCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot remove negative coins.");
    }
    if (coins < amount) {
        throw std::invalid_argument("Not enough coins to remove.");
    }
    coins -= amount;
}

// Marks the player as inactive
void Player::deactivate() {
    active = false;
    std::cout << name << " has been eliminated from the game.\n";
}

// Sets the player's sanctioned state.
void Player::setSanctioned(bool value) {
    sanctioned = value;
}

// Sets the flag indicating whether the player was arrested last turn.
void Player::setArrestedLastTurn(bool value) {
    arrestedLastTurn = value;
}

// Updates the number of turns the player is blocked from arresting.
void Player::updateArrestBlock(bool reset) {
    if (reset) {
        arrestBlockedTurns = 1;
    } else {
        arrestBlockedTurns = std::max(0, arrestBlockedTurns - 1);
    }

}

// A function called when a player is sanctioned by another. Can be overridden.
void Player::receiveSanctionFrom(Player& attacker) {
}


// Sets whether this player’s coins are temporarily revealed due to spy.
void Player::setRevealedBySpy(bool val) {
    revealedBySpy = val;
}


// Actions

// Adds 1 coin to the player unless sanctioned.
void Player::gather() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot gather.");
    }
    addCoins(1);
    std::cout << name << " gathered 1 coin.\n";
}

// Adds 2 coins to the player unless sanctioned.
void Player::tax() {
    if (sanctioned) {
        throw std::invalid_argument("Sanctioned player cannot use tax.");
    }
    addCoins(2);
    
    std::cout << name << " received 2 coins from tax.\n";
}

// Deducts 4 coins for bribe and allows extra action.
void Player::bribe() {
    removeCoins(4);
    std::cout << name << " paid 4 coins for a bribe and may perform an extra action.\n";
}

// Attempts to arrest another player unless blocked.
void Player::arrest(Player& other) {
    if (!other.isActive()) {
        throw std::invalid_argument("Cannot arrest an inactive player.");
    }

    if (this->isArrestBlocked() || this->wasSpiedLastTurn()) {
        throw std::runtime_error(name + " is blocked from using arrest this turn.");
    }

    other.receiveArrestFrom(*this);
    std::cout << name << " arrested " << other.getName() << ".\n";
}

// Attempts to sanction another player by paying 3 coins.
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

    other.receiveSanctionFrom(*this);
}

// Performs a coup on another player by paying 7 coins.
void Player::coup(Player& other) {
    if (coins < 7) {
        throw std::invalid_argument("Not enough coins to perform coup.");
    }
    removeCoins(7);
    other.deactivate();
    std::cout << name << " performed a coup on " << other.getName() << ".\n";
}

// Called when this player is arrested by another.
void Player::receiveArrestFrom(Player& attacker) {
    if (coins == 0) {
        throw std::runtime_error(name + " has no coins to be arrested.");
    }

    removeCoins(1);         
    setArrestedLastTurn(true);
    onArrested();
}

// A function called when the player is arrested 
void Player::onArrested() {
}

// Returns whether this player's coins are currently visible to other players.
bool Player::isRevealedBySpy() const {
    return revealedBySpy;
}

// Returns true if the player was spied on during the previous turn.
bool Player::wasSpiedLastTurn() const {
    return spiedLastTurn;
}

// Sets whether the player was spied on last turn.
void Player::setSpiedLastTurn(bool value) {
    spiedLastTurn = value;
}

