//Email: yuvali532@gmail.com
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "../Player.hpp"   

// Governor role: can collect tax and block tax actions of other players
class Governor : public Player {
public:
    Governor(const std::string& name);

    std::string getRoleName() const override;
    // Executes the Governor's tax action, gaining 3 coins
    void tax() override;
// Governor's special ability to block a Tax action by another player.
    void blockTax(Player& target);

};

#endif 