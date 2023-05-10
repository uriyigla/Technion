//
// Created by Shaked on 21/12/2022.
//

#ifndef AVLTREE_H_UF_NODE_H
#define AVLTREE_H_UF_NODE_H

#include "Player.h"
#include "t_Node.h"
#include "Team.h"
#include "wet2util.h"

class uf_Node {
public:
    uf_Node() :
    m_data(), m_father(nullptr), m_team()
    {}
    explicit uf_Node(Player* player) :
            m_data(player), m_father(nullptr), m_team()
    {}
    Team* find();
    void insert(Team* team);
    uf_Node* union1(uf_Node* root2);

    Player* m_data;
    uf_Node* m_father;
    Team* m_team;

};


#endif //AVLTREE_H_UF_NODE_H
