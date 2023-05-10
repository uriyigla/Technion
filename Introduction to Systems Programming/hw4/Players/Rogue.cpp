#include "Rogue.h"

Rogue::Rogue(const std::string name): Player(name)
{
    m_job="Rogue";
}
void Rogue::addCoins(int extraCoins)
{
    if (extraCoins<0)
    {
        return;
    }
    extraCoins=extraCoins*2;
    m_coins+=extraCoins;
}

