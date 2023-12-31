#ifndef MAIN_CPP_ROGUE_H
#define MAIN_CPP_ROGUE_H
#include "Player.h"

class Rogue : public Player {

public:

    Rogue(const std::string name);
    Rogue(const Rogue&) = default;
    Rogue& operator=(const Rogue& rogue) = default;
    ~Rogue() = default;

    void addCoins(int extraCoins) override;

};

#endif //MAIN_CPP_ROGUE_H
