//
// Created by Daniel_Meents on 07/04/2022.
//

#ifndef EX2_Card_H
#define EX2_Card_H

#include <string>
#include "Player.h"
#include "utilities.h"

enum class CardType { Battle, Buff, Heal, Treasure };

class Card {
public:
    Card(CardType type, const CardStats& stats);

    void applyEncounter(Player& player) const;

    void printInfo() const;

    Card() : m_effect(CardType::Treasure), m_stats() {}

    Card(const Card&) = default;
    ~Card() = default;
    Card& operator=(const Card& other) = default;

private:
    CardType m_effect;
    CardStats m_stats;
};

#endif //EX2_Card_H
