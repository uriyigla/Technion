#include "Card.h"
#include "Fairy.h"
#include <string>

static const int FAIRY_HP = 10;
static const std::string FAIRY ="Fairy";
static const std::string WIZARD ="Wizard";

Fairy::Fairy():Hp()
{
    m_name=FAIRY;
    m_Hp=FAIRY_HP;
}

void Fairy::applyEncounter(Player& player) const
{
    if (player.getJob()==WIZARD)
    {
        printFairyMessage(true);
        player.heal(FAIRY_HP);
        return;
    }
    printFairyMessage(false);
    return;
}

