#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "../Player.hpp"  // גישה למחלקת Player שנמצאת בתיקייה שמעל

class Governor : public Player {
public:
    Governor(const std::string& name);

    std::string getRoleName() const override;
    void tax() override;
    void blockTax(Player& target);

};

#endif // GOVERNOR_HPP
