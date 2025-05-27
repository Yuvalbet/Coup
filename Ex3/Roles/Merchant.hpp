#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "../Player.hpp"

class Merchant : public Player {
public:
    Merchant(const std::string& name);

    std::string getRoleName() const override;

    void onStartTurn();
    void receiveArrestFrom(Player& attacker) override;

};

#endif // MERCHANT_HPP
