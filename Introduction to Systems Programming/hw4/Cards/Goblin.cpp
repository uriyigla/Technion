#include "Card.h"
#include "Goblin.h"

static const std::string GOBLIN="Goblin";
static const int GOBLIN_FORCE=6;
static const int GOBLIN_DAMAGE=10;
static const int GOBLIN_LOOT=2;
static const int GOBLIN_LOST_FORCE=0;

Goblin::Goblin():Battle()
{
    m_name=GOBLIN;
    m_force=GOBLIN_FORCE;
    m_damage=GOBLIN_DAMAGE;
    m_loot=GOBLIN_LOOT;
    m_lostForce=GOBLIN_LOST_FORCE;
}

