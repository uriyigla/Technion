#include <string>
#include "Player.h"

const int DEFAULT_FORCE = 5;
const int DEFAULT_MAXHP = 100;

Player::Player(const std::string name, const int maxHP, const int force) {
    this->m_name += name;
    this->m_level = 1;
    this->m_force = (force < 0) ? DEFAULT_FORCE : force;
    this->m_maxHP = (maxHP < 0) ? DEFAULT_MAXHP : maxHP;
    this->m_HP = maxHP;
    this->m_coins = 0;
}

void Player::printInfo() const {
    printPlayerInfo(&m_name[0], m_level, m_force, m_HP, m_coins);
}

void Player::levelUp() {
    if (m_level == 10) {
        return;
    }
    m_level++;
}

int Player::getLevel() const {
    return m_level;
}

void Player::buff(int improveForce) {
    if (improveForce >= 0) {
        m_force += improveForce;
    }
}

void Player::heal(int improveHP) {
    if (improveHP < 0) {
        return;
    }
    m_HP = std::min(m_HP + improveHP, m_maxHP);
}

void Player::damage(int damageHP) {
    if (damageHP < 0) {
        return;
    }
    m_HP = std::max(m_HP - damageHP, 0);
}

bool Player::isKnockedOut() const {
    return (m_HP == 0);
}

void Player::addCoins(int extraCoins) {
    if (extraCoins >= 0) {
        m_coins += extraCoins;
    }
}

bool Player::pay(int payment) {
    if (payment < 0) {
        return true;
    }
    if (payment <= m_coins) {
        m_coins -= payment;
        return true;
    }
    return false;
}

int Player::getAttackStrength() const {
    return (m_level + m_force);
}
