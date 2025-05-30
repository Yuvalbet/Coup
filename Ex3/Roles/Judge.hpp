//Email: yuvali532@gmail.com
#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "../Player.hpp"

// Judge role: can perform and block bribes in the game
class Judge : public Player {
public:
    Judge(const std::string& name);

    std::string getRoleName() const override;

    bool blockBribe(Player& target);
    void receiveSanctionFrom(Player& attacker);
};

#endif 