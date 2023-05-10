//
// Created by Shaked on 23/11/2022.
//

#ifndef WET1_PLAYER_H
#define WET1_PLAYER_H

#include "t_Node.h"

class Team;

class Player {
public:
    Player(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalkeeper) :
            m_id(playerId),
            m_gamesAlone(gamesPlayed),
            m_gamesWithoutPlayer(0),
            m_goals(goals),
            m_cards(cards),
            m_goalKeeper(goalkeeper),
            m_teamNode(nullptr)
    {}

    Player(const Player &a) = delete;
    Player &operator=(const Player &a) = delete;
    ~ Player()=default;

    bool operator>(const Player &a) const;
    bool operator==(const Player &a) const;
    bool operator!=(const Player &a) const;
    explicit operator int() const;
    void updateStats (int gamesPlayed,int  scoredGoals,int cardsReceived);
    int closer (Player* player1,Player* player2);

    int m_id;
    int m_gamesAlone;
    int m_gamesWithoutPlayer;
    int m_goals;
    int m_cards;
    bool m_goalKeeper;
    t_Node<Team>* m_teamNode;
};


#endif //WET1_PLAYER_H
