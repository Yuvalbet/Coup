//Email: yuvali532@gmail.com
#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "../Player.hpp"

// The General class represents a role that can block Coup actions.
class General : public Player {
public:
    General(const std::string& name);

    std::string getRoleName() const override;


// General's special ability to block a Coup by paying 5 coins.
    void blockCoup(Player& target, Player& attacker);

    virtual void receiveArrestFrom(Player& attacker) override;

    void onArrested();
};

#endif 