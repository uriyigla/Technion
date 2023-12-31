//
// Created by Shaked on 23/11/2022.
//

#ifndef WET1_TEAM_H
#define WET1_TEAM_H

#include "AVLTree.h"
#include "Player.h"

class Player;

class Team {
public:
    Team(int teamId, int points) : m_id(teamId),
                                   m_points(points),
                                   m_size(0),
                                   m_games(0),
                                   m_goalKeeper(0),
                                   m_cards(0),
                                   m_goals(0),
                                   m_teamPlayersTree(),
                                   m_teamPlayersByIdTree()
    {
    }

    ~Team() = default;

    bool operator>(const Team &b) const;

    bool operator==(const Team &b) const;

    bool operator!=(const Team &b) const;

    explicit operator int() const;

    void addPlayerStatsToTeam(const Player *playerAdded);

    void removePlayerStatsFromTeam(const Player *playerAdded);

    bool isValid() const;

    void playMatch(Team *team);

    void mergeTeam(const Team *team1, const Team *team2);

    int m_id;
    int m_points;
    int m_size;
    int m_games;
    int m_goalKeeper;
    int m_cards;
    int m_goals;
    AVLTree<Player> m_teamPlayersTree;
    AVLTree<Player> m_teamPlayersByIdTree;
};

#endif //WET1_TEAM_H
