#ifndef HW2_PLAYER_H
#define HW2_PLAYER_H

#include <string>
#include "utilities.h"

class Player {
public:

    Player(std::string name, const int maxHP = 5, const int force = 100);

    Player(const Player&) = default;
    ~Player() = default;
    Player& operator=(const Player& player) = default;

    void printInfo() const;
    void levelUp();
    int getLevel() const;
    void buff(int improveForce);
    void heal(int improveHP);
    void damage(int damageHP);
    bool isKnockedOut() const;
    void addCoins(int extraCoins);
    bool pay(int payment);
    int getAttackStrength() const;

private:
    std::string m_name;
    int m_level, m_force, m_maxHP, m_HP, m_coins;

};

#endif //HW2_PLAYER_H
