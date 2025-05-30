//Email: yuvali532@gmail.com
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../Player.hpp"

// Inherits from Player and defines role-specific behavior.
class Merchant : public Player {
public:
    Merchant(const std::string& name);

    std::string getRoleName() const override;

    void onStartTurn();
    void receiveArrestFrom(Player& attacker) override;

};

#endif 