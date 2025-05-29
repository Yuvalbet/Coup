// Header file for the Governor class, a player role in the Coup game
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "../Player.hpp"  // [TODO: Hebrew comment replaced – write English version]

// Governor role: can collect tax and block tax actions of other players
class Governor : public Player {
public:
    Governor(const std::string& name);

    std::string getRoleName() const override;
    // Executes the Governor's tax action, gaining 3 coins
    void tax() override;
    void blockTax(Player& target);

};

#endif // GOVERNOR_HPP