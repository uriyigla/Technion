#ifndef PLAYER_H_DRAGON_H
#define PLAYER_H_DRAGON_H

#include "Card.h"

class Dragon : public Battle {

public:
    Dragon();
    Dragon(const Dragon&) = default;
    Dragon& operator=(const Dragon& other) = default;
    ~Dragon() = default;

};

#endif //PLAYER_H_DRAGON_H
