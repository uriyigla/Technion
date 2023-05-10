#ifndef MAIN_CPP_FIGHTER_H
#define MAIN_CPP_FIGHTER_H
#include "Player.h"

class Fighter : public Player {

public:

    Fighter(const std::string name);
    Fighter(const Fighter& )=default;
    Fighter & operator=(const Fighter& fighter)=default;
    ~Fighter()=default;

    int getAttackStrength() const override;
};

#endif //MAIN_CPP_FIGHTER_H