#include "Fighter.h"

Fighter::Fighter(const std::string name):Player(name){
    m_job="Fighter";
}

int Fighter::getAttackStrength() const{
    return (m_level + 2 * m_force);
}
