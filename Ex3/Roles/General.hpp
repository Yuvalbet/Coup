#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "../Player.hpp"

class General : public Player {
public:
    General(const std::string& name);

    std::string getRoleName() const override;


    // פעולה ייחודית – חוסם הפיכה
    void blockCoup(Player& target, Player& attacker);

    virtual void receiveArrestFrom(Player& attacker) override;

    // מקבל בחזרה את המטבע במקרה של arrest
    void onArrested();
};

#endif // GENERAL_HPP
