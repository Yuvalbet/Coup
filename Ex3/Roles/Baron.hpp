//Email: yuvali532@gmail.com
#ifndef BARON_HPP
#define BARON_HPP

#include "../Player.hpp"

// Baron role: can perform an invest action 
class Baron : public Player {
public:
    Baron(const std::string& name);

    std::string getRoleName() const override;

// Performs the Baron's invest action to gain coins
    void invest();

// Handles logic when the Baron is sanctioned by another player.
    void receiveSanctionFrom(Player& attacker); 
    
};

#endif 