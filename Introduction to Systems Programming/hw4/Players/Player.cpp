#include "Player.h"
#include <string>
#include <iostream>
#include "../utilities.h"
using std::cout;
using std::endl;

static const int  DEFAULT_LEVEL= 1;
static const int  DEFAULT_FORCE= 5;
static const int  DEFAULT_HP= 100;
static const int  DEFAULT_COINS= 10;
static const std::string PLAYER ="player";

Player::Player(const std::string name): m_name(name),m_level( DEFAULT_LEVEL),m_force(DEFAULT_FORCE),
m_HP(DEFAULT_HP), m_coins(DEFAULT_COINS),m_job(PLAYER) {}

//from ex2 start

void Player::levelUp()
{
    if (m_level==10) {
        return;
    }
    m_level++;
}
int Player::getLevel() const
{
    return m_level;
}
void Player::buff(int improveForce)
{
    if (m_force+improveForce<=0){
        m_force=0;
        return;
    }
    m_force+=improveForce;
}
void Player::heal(int improveHP)
{
    if (m_HP+improveHP>=DEFAULT_HP){
        m_HP=DEFAULT_HP;
    }
    else
    {
        m_HP+=improveHP;
    }
}
void Player::damage(int damageHP)
{
    if (damageHP<0)
    {
        return;
    }
    if (m_HP-damageHP<=0)
    {
        m_HP=0;
    }
    else
    {
        m_HP-=damageHP;
    }
}
bool Player::isKnockedOut() const
{
    return (m_HP==0);
}
void Player::addCoins(int extraCoins)
{
    if (extraCoins<0)
    {
        return;
    }
    m_coins+=extraCoins;
}
bool Player::pay(int payment)
{
    if (payment<0)
    {
        return true;
    }
    if (payment<=m_coins)
    {
        m_coins-=payment;
        return true;
    }
    return false;
}
int Player::getAttackStrength() const
{
    return (m_level+m_force);
}

//from ex2 end

std::ostream& operator<<(std::ostream& os, const Player& player)
{
    player.print(os);
    return os;
}
void Player::print(std::ostream& os) const
{
    printPlayerDetails(os,m_name,m_job,m_level,m_force,m_HP,m_coins);
}
std::string Player::getName() const
{
    return m_name;
}
std::string Player::getJob() const
{
    return m_job;
}
int Player::getCoins() const
{
    return m_coins;
}