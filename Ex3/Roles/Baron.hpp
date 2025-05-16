#ifndef BARON_HPP
#define BARON_HPP

#include "../Player.hpp"

class Baron : public Player {
public:
    Baron(const std::string& name);

    std::string getRoleName() const override;

    // פעולה ייחודית
    void invest();

    // Override לפעולה sanction – כדי שיקבל פיצוי
    void receiveSanctionFrom(Player& attacker);  // נקרא מתוך Game או מתוך sanction()
};

#endif // BARON_HPP
