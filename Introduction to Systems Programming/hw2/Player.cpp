
#include <string>
#include "Player.h"

const int DEFAULT_FORCE=5;
const int DEFAULT_MAXHP=100;

Player::Player(const std::string name, const int maxHP,const int force) {

    this->m_name+=name;
    this->m_level=1;
    if (force<0){
        this->m_force=DEFAULT_FORCE;
    }
    else {
        this->m_force=force;
    }
    if (maxHP<0){
        this->m_maxHP=DEFAULT_MAXHP;
    }
    else {
        this->m_maxHP=maxHP;
    }
    this->m_HP=maxHP;
    this->m_coins=0;

}

void Player::printInfo() const{

    printPlayerInfo( &m_name[0], m_level, m_force, m_HP, m_coins);

}

void Player::levelUp(){

    if (m_level==10) {
      return;
    }
    m_level++;

}

int Player::getLevel() const {

    return m_level;

}

void Player::buff(int improveForce){

    if (improveForce>=0){
        m_force+=improveForce;
    }

}

void  Player::heal(int improveHP){

    if (improveHP<0){
        return;
    }
    if (m_HP+improveHP>=m_maxHP){
        m_HP=m_maxHP;
    }
    else{
        m_HP+=improveHP;
    }

}

void Player::damage(int damageHP){

    if (damageHP<0){
        return;
    }
    if (m_HP-damageHP<=0){
        m_HP=0;
    }
    else{
        m_HP-=damageHP;
    }

}

bool Player::isKnockedOut() const{

    return (m_HP==0);

}

void Player::addCoins(int extraCoins){

    if (extraCoins<0){
        return;
    }
    m_coins+=extraCoins;

}

bool Player::pay(int payment){

    if (payment<0){
        return true;
    }
    if (payment<=m_coins){
        m_coins-=payment;
        return true;
    }
    return false;

}

int Player::getAttackStrength() const{

    return (m_level+m_force);

}









