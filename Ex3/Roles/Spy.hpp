// Header file for the Spy class, a unique player role in the game
#ifndef SPY_HPP
#define SPY_HPP

#include "../Player.hpp"

// Spy role: can reveal a target's coins and block their next arrest
class Spy : public Player {
public:
    Spy(const std::string& name);

    std::string getRoleName() const override;

    // [TODO: Hebrew comment replaced – write English version]
    void spyOn(Player& other);
};

#endif // SPY_HPP