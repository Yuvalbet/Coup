// Header file for the Baron class, a specific role in the Coup game
#ifndef BARON_HPP
#define BARON_HPP

#include "../Player.hpp"

// Baron role: can perform an invest action and handle sanctions from others
class Baron : public Player {
public:
    Baron(const std::string& name);

    std::string getRoleName() const override;

    // [TODO: Hebrew comment replaced – write English version]
    // Performs the Baron's invest action to gain coins
    void invest();

    // [TODO: Hebrew comment replaced – write English version]
    // Called when the Baron is sanctioned by another player
    void receiveSanctionFrom(Player& attacker);  // [TODO: Hebrew comment replaced – write English version]
    
};

#endif // BARON_HPP