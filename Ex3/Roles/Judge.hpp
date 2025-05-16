#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "../Player.hpp"

class Judge : public Player {
public:
    Judge(const std::string& name);

    std::string getRoleName() const override;

    void blockBribe(Player& target);
    void receiveSanctionFrom(Player& attacker);
};

#endif // JUDGE_HPP
