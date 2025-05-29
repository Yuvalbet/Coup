// Header file for the Merchant class, a special player role in the game
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../Player.hpp"

// Merchant role: can perform a trade action to earn coins and be sanctioned
class Merchant : public Player {
public:
    Merchant(const std::string& name);

    std::string getRoleName() const override;

    void onStartTurn();
    void receiveArrestFrom(Player& attacker) override;

};

#endif // MERCHANT_HPP