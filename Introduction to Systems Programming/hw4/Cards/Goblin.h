#ifndef MAIN_CPP_GOBLIN_H
#define MAIN_CPP_GOBLIN_H

#include "Card.h"

class Goblin : public Battle
{
public:
    Goblin();
    Goblin(const Goblin &) = default;
    Goblin &operator=(const Goblin &other) = default;
    ~Goblin() = default;
};

#endif //MAIN_CPP_GOBLIN_H
