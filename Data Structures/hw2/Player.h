//
// Created by Shaked on 21/12/2022.
//

#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H

#include "wet2util.h"
class uf_Node;

class Player {
public:
    Player() : m_id(0),
    m_goalKeeper(false),
    m_cards(0),
    m_gamesPlayed(0),
    m_spirit(permutation_t::neutral())
    {}
    Player(int playerId, const permutation_t &spirit, int gamesPlayed, int cards, bool goalKeeper) :
            m_id(playerId),
            m_goalKeeper(goalKeeper),
            m_cards(cards),
            m_gamesPlayed(gamesPlayed),
            m_spirit(spirit)
    {}

    explicit operator int() const;

    ~Player() = default;
    int m_id;
    bool m_goalKeeper;
    int m_cards;
    int m_gamesPlayed;
    permutation_t m_spirit;
    uf_Node* m_placeInTree;

};


#endif //WET2_PLAYER_H






//    explicit operator int() const;
//    bool operator>(const Team& b) const;



