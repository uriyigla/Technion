#ifndef PLAYER_H_MERCHANT_H
#define PLAYER_H_MERCHANT_H

#include <string>
#include "../Players/Player.h"
#include "../utilities.h"
#include "Card.h"

class Merchant:public Card {

protected:

    int m_buyHp,m_buyForce;

public:

    Merchant();
    Merchant(const Merchant &) = default;
    Merchant &operator=(const Merchant &merchant) = default;
    ~Merchant() = default;
    void applyEncounter(Player& player) const override;

};

#endif //PLAYER_H_MERCHANT_H
