#ifndef SPY_HPP
#define SPY_HPP

#include "../Player.hpp"

class Spy : public Player {
public:
    Spy(const std::string& name);

    std::string getRoleName() const override;

    // פעולה ייחודית של מרגל
    void spyOn(Player& other);
};

#endif // SPY_HPP
