#ifndef PLAYER_H_TREASURE_H
#define PLAYER_H_TREASURE_H

#include <string>
#include "../Players/Player.h"
#include "../utilities.h"
#include "Card.h"

class Treasure:public Card {

protected:

    int m_coins;

public:

    Treasure();
    Treasure(const Treasure &) = default;
    Treasure &operator=(const Treasure &treasure) = default;
    ~Treasure() = default;
    void applyEncounter(Player& player) const override;

};

#endif //PLAYER_H_TREASURE_H