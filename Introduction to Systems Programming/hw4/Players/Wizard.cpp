#include "Wizard.h"

static const int DEFAULT_HP = 100;

Wizard::Wizard(const std::string name) : Player(name) {
    m_job = "Wizard";
}

void Wizard::heal(int improveHP) {
    if (improveHP < 0) {
        m_HP += improveHP;
        return;
    }
    improveHP = 2 * improveHP;
    if (m_HP + improveHP >= DEFAULT_HP) {
        m_HP = DEFAULT_HP;
    } else {
        m_HP += improveHP;
    }
}
