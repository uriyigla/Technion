#include "Card.h"
#include "Pitfall.h"
#include <string>

static const int PITFALL_HP = 10;
static const std::string PITFALL ="Pitfall";
static const std::string ROGUE ="Rogue";  // Fixed the typo in the job name

Pitfall::Pitfall() : Hp()
{
    m_name = PITFALL;
    m_Hp = PITFALL_HP;
}

void Pitfall::applyEncounter(Player &player) const
{
    if (player.getJob() == ROGUE)
    {
        printPitfallMessage(true);
        return;
    }
    player.damage(PITFALL_HP);
    printPitfallMessage(false);
    return;
}
