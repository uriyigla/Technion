#include "Card.h"
#include "Barfight.h"
#include <string>

static const int BARFIGHT_HP = 10;
static const std::string BARFIGHT ="Barfight";
static const std::string FIGHTER ="Fighter";

Barfight::Barfight():Hp()
{
    m_name=BARFIGHT;
    m_Hp=BARFIGHT_HP;
}
void Barfight::applyEncounter(Player& player) const
{
    if (player.getJob()==FIGHTER)
    {
        printBarfightMessage(true);
        return;
    }
    player.damage(BARFIGHT_HP);
    printBarfightMessage(false);
    return;
}

