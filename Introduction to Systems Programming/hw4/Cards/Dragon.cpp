#include "Card.h"
#include "Dragon.h"

static const std::string DRAGON="Dragon";
static const int DRAGON_FORCE=25;
static const int DRAGON_DAMAGE=100;
static const int DRAGON_LOOT=1000;
static const int DRAGON_LOST_FORCE=0;

Dragon::Dragon():Battle()
{
    m_name=DRAGON;
    m_force=DRAGON_FORCE;
    m_damage=DRAGON_DAMAGE;
    m_loot=DRAGON_LOOT;
    m_lostForce=DRAGON_LOST_FORCE;
}