#include "Card.h"


static const std::string BATTLE="Battle";

std::ostream& operator<<(std::ostream& os, const Card& card)
{
    card.print(os);
    return os;
}

Card::Card():m_name("Card"){}

void Card::print(std::ostream& os) const
{
    printCardDetails(os,m_name);
    printEndOfCardDetails(os);
}
Battle::Battle():Card()
{
    m_name=BATTLE;
    m_force=0;
    m_damage=0;
    m_loot=0;
    m_lostForce=0;
}
void Battle::applyEncounter(Player& player) const
{
    if (player.getAttackStrength()>=m_force)
    {
        player.addCoins(m_loot);
        player.levelUp();
        printWinBattle(player.getName(),m_name);
    }
    else
    {
        player.buff(m_lostForce);
        player.damage(m_damage);
        printLossBattle(player.getName(),m_name);
    }
}
void Battle::print(std::ostream& os) const
{
    printCardDetails(os,m_name);
    bool isDragon=(m_name=="Dragon");
    printMonsterDetails(os, m_force, m_damage,m_loot,  isDragon);
    printEndOfCardDetails(os);
}

Hp::Hp():Card()
{
    m_Hp=0;
}

void Hp::print(std::ostream& os) const
{
    printCardDetails(os,m_name);
    printEndOfCardDetails(os);
}


