#ifndef PLAYER_H_PITFALL_H
#define PLAYER_H_PITFALL_H

#include "Card.h"

class Pitfall : public Hp {

public:

    Pitfall();
    Pitfall(const Pitfall &) = default;
    Pitfall &operator=(const Pitfall &other) = default;
    ~Pitfall() = default;

    void applyEncounter(Player &player) const;

};

#endif //PLAYER_H_PITFALL_H
