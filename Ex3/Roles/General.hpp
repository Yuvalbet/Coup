// Header file for the General class, a player role in the Coup game
#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "../Player.hpp"

// General role: can perform a coup and block coups from other players
class General : public Player {
public:
    General(const std::string& name);

    std::string getRoleName() const override;


    // [TODO: Hebrew comment replaced – write English version]
    void blockCoup(Player& target, Player& attacker);

    virtual void receiveArrestFrom(Player& attacker) override;

    // [TODO: Hebrew comment replaced – write English version]
    void onArrested();
};

#endif // GENERAL_HPP