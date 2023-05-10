#include "Card.h"
#include "Vampire.h"

static const std::string VAMPIRE="Vampire";
static const int VAMPIRE_FORCE=10;
static const int VAMPIRE_DAMAGE=10;
static const int VAMPIRE_LOOT=2;
static const int VAMPIRE_LOST_FORCE=-1;

Vampire::Vampire():Battle()
{
    m_name=VAMPIRE;
    m_force=VAMPIRE_FORCE;
    m_damage=VAMPIRE_DAMAGE;
    m_loot=VAMPIRE_LOOT;
    m_lostForce=VAMPIRE_LOST_FORCE;
}