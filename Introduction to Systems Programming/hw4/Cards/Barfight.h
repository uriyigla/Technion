#ifndef PLAYER_H_BARFIGHT_H
#define PLAYER_H_BARFIGHT_H

#include "Card.h"

class Barfight : public Hp {

public:

    Barfight();
    Barfight(const Barfight&) = default;
    Barfight& operator=(const Barfight& other) = default;
    ~Barfight() = default;

    void applyEncounter(Player& player) const;

};

#endif // PLAYER_H_BARFIGHT_H
