//Email: yuvali532@gmail.com
#ifndef SPY_HPP
#define SPY_HPP

#include "../Player.hpp"

// The Spy class represents a role that can reveal another player's coin count.
class Spy : public Player {
public:
    Spy(const std::string& name);

    std::string getRoleName() const override;

    void spyOn(Player& other);
};

#endif 