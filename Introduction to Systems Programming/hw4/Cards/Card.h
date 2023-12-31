#ifndef EX2_Card_H
#define EX2_Card_H

#include <string>
#include "../Players/Player.h"
#include "../utilities.h"

class Card {

protected:

    std::string m_name;

    friend std::ostream& operator<<(std::ostream& os, const Card& card);

public:

    Card();
    Card(const Card&) = default;
    Card& operator=(const Card& other) = default;
    virtual ~Card() = default;

    virtual void applyEncounter(Player& player) const = 0;
    virtual void print(std::ostream& os) const;

};

class Battle : public Card {

protected:

    int m_force, m_damage, m_loot, m_lostForce;

public:

    Battle();
    Battle(const Battle&) = default;
    Battle& operator=(const Battle& other) = default;
    virtual ~Battle() = default;

    void applyEncounter(Player& player) const override;
    void print(std::ostream& os) const override;

};

class Hp : public Card {

protected:

    int m_Hp;

public:

    Hp();
    Hp(const Hp&) = default;
    Hp& operator=(const Hp& other) = default;
    virtual ~Hp() = default;

    virtual void applyEncounter(Player& player) const = 0;
    void print(std::ostream& os) const override;

};

#endif //EX2_Card_H
