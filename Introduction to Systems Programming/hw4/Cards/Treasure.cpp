#include "Treasure.h"

static const int  DEFAULT_COINS = 10;
static const std::string TREASURE ="Treasure";

Treasure::Treasure():Card()
{
    m_name=TREASURE;
    m_coins=DEFAULT_COINS;
}

void Treasure::applyEncounter(Player& player) const
{
    player.addCoins(m_coins);
    printTreasureMessage();
}