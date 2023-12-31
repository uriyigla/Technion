#ifndef PLAYER_H_FAIRY_H
#define PLAYER_H_FAIRY_H

#include "Card.h"

class Fairy : public Hp {

public:

    Fairy();
    Fairy(const Fairy&) = default;
    Fairy& operator=(const Fairy& other) = default;
    ~Fairy() = default;

    void applyEncounter(Player& player) const;

};

#endif // PLAYER_H_FAIRY_H
