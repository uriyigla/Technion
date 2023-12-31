#include "HealthPoints.h"
#include <iostream>

void operator+=(HealthPoints& hp, const int num) {
    hp.m_hp += num;
    if (hp.m_hp < 0) {
        hp.m_hp = 0;
    }
    if (hp.m_hp > hp.m_maxHp) {
        hp.m_hp = hp.m_maxHp;
    }
}

void operator-=(HealthPoints& hp, const int num) {
    hp += -num;
}

HealthPoints operator+(HealthPoints& hp, const int num) {
    HealthPoints hpResult(hp.m_maxHp);
    hpResult.m_hp = hp.m_hp;
    hpResult += num;
    return hpResult;
}

HealthPoints operator+(int num, HealthPoints& hp) {
    return hp + num;
}

HealthPoints operator-(HealthPoints& hp, const int num) {
    return hp + (-num);
}

bool operator==(const HealthPoints& hp1, const HealthPoints& hp2) {
    return static_cast<int>(hp1) == static_cast<int>(hp2);
}

bool operator!=(const HealthPoints& hp1, const HealthPoints& hp2) {
    return !(hp1 == hp2);
}

bool operator>(const HealthPoints& hp1, const HealthPoints& hp2) {
    return hp1.m_hp > hp2.m_hp;
}

bool operator<(const HealthPoints& hp1, const HealthPoints& hp2) {
    return hp2 > hp1;
}

bool operator>=(const HealthPoints& hp1, const HealthPoints& hp2) {
    return (hp1 > hp2) || (hp1 == hp2);
}

bool operator<=(const HealthPoints& hp1, const HealthPoints& hp2) {
    return hp2 >= hp1;
}

std::ostream& operator<<(std::ostream& os, const HealthPoints hp) {
    os << hp.m_hp << "(" << hp.m_maxHp << ")";
    return os;
}
