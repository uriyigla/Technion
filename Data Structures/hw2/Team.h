//
// Created by Shaked on 21/12/2022.
//

#ifndef WET2_TEAM_H
#define WET2_TEAM_H

#include "wet2util.h"
class uf_Node;

class Team {
public:
    Team() : m_id(0), m_ability(0),
             m_goalKeeper(false),
             m_points(0), m_gamesPlayed(0),
             m_size (0), m_totalSpirit(permutation_t::neutral()), m_rootPlayersTree(nullptr)
    {}
    Team(int teamId) : m_id(teamId), m_ability(0),
                       m_goalKeeper(false),
                       m_points(0), m_gamesPlayed(0),
                       m_size (0), m_totalSpirit(permutation_t::neutral()), m_rootPlayersTree(nullptr)

    {}

    ~Team()= default;

    explicit operator int() const;
    bool operator>(Team b) const;
    void unite(const Team* team2);
    void updateAddPlayer(const permutation_t &spirit, int ability, bool goalKeeper);
    void swapTeam (Team* team2);

    int m_id;
    int m_ability;
    bool m_goalKeeper;
    int m_points;
    int m_gamesPlayed;
    int m_size;
    permutation_t m_totalSpirit;
    uf_Node* m_rootPlayersTree;
};


#endif //WET2_TEAM_H
