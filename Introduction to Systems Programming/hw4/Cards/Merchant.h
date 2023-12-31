#include "Card.h"
#include "Merchant.h"
#include <string>

static const int MERCHANT_BUY_HP = 2;
static const int MERCHANT_BUY_FORCE = 1;
static const std::string MERCHANT ="Merchant";

Merchant::Merchant() : Card(), m_buyHp(MERCHANT_BUY_HP), m_buyForce(MERCHANT_BUY_FORCE)
{
    m_name = MERCHANT;
}

void Merchant::applyEncounter(Player &player) const
{
    printMerchantMessage();
    player.buff(m_buyForce);
    player.heal(m_buyHp);
}
