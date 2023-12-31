#include "Card.h"

Card::Card(CardType type, const CardStats& stats) {
    this->m_effect = type;

    switch (this->m_effect) {
        case CardType::Battle:
            this->m_stats.loot = stats.loot;
            this->m_stats.force = stats.force;
            this->m_stats.hpLossOnDefeat = stats.hpLossOnDefeat;
            break;
        case CardType::Buff:
            this->m_stats.cost = stats.cost;
            this->m_stats.buff = stats.buff;
            break;
        case CardType::Heal:
            this->m_stats.cost = stats.cost;
            this->m_stats.heal = stats.heal;
            break;
        case CardType::Treasure:
            this->m_stats.loot = stats.loot;
            break;
    }
}

void Card::applyEncounter(Player& player) const {
    switch (this->m_effect) {
        case CardType::Battle:
            if (player.getAttackStrength() >= this->m_stats.force) {
                player.levelUp();
                player.addCoins(this->m_stats.loot);
                printBattleResult(1);
            } else {
                player.damage(this->m_stats.hpLossOnDefeat);
                printBattleResult(0);
            }
            break;
        case CardType::Buff:
            int checkBuff;
            checkBuff = player.pay(this->m_stats.cost);
            if (checkBuff) {
                player.buff(this->m_stats.buff);
            }
            break;
        case CardType::Heal:
            int checkHeal;
            checkHeal = player.pay(this->m_stats.cost);
            if (checkHeal) {
                player.heal(this->m_stats.heal);
            }
            break;
        case CardType::Treasure:
            player.addCoins(this->m_stats.loot);
            break;
    }
}

void Card::printInfo() const {
    if (Card::m_stats.force > 0) {
        printBattleCardInfo(this->m_stats);
    } else {
        if (Card::m_stats.heal > 0) {
            printHealCardInfo(this->m_stats);
        } else {
            if (Card::m_stats.buff > 0) {
                printBuffCardInfo(this->m_stats);
            } else {
                printTreasureCardInfo(this->m_stats);
            }
        }
    }
}
