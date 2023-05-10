#include "uf_Node.h"


Team* uf_Node::find() {
    uf_Node* findRoot = this;
    uf_Node* iterator = this;
    int gamesSum = 0;
    permutation_t productSpirit = permutation_t::neutral();

    while (findRoot->m_father != nullptr) {
        gamesSum += findRoot->m_data->m_gamesPlayed;
        productSpirit = findRoot->m_data->m_spirit * productSpirit;
        findRoot = findRoot->m_father;
    }

    while ((iterator != findRoot) && (iterator->m_father != findRoot)) {
        uf_Node* temp = iterator;
        iterator = iterator->m_father;
        temp->m_father = findRoot;

        int prevGames = temp->m_data->m_gamesPlayed;
        temp->m_data->m_gamesPlayed = gamesSum;
        gamesSum -= prevGames;

        permutation_t prevSpirit = temp->m_data->m_spirit;
        temp->m_data->m_spirit = productSpirit;
        productSpirit = productSpirit * prevSpirit.inv();
    }
    return findRoot->m_team;
}

void uf_Node::insert(Team* team) {
    if (team->m_rootPlayersTree == nullptr) {
        team->m_rootPlayersTree = this;
        this->m_team = team;
    }
    else {
        this->m_father = team->m_rootPlayersTree;
        int rootGamesPlayed = team->m_rootPlayersTree->m_data->m_gamesPlayed;
        int teamGamesPlayed = team->m_gamesPlayed;
        this->m_data->m_gamesPlayed -= (rootGamesPlayed + teamGamesPlayed);
        this->m_data->m_spirit = this->m_father->m_data->m_spirit.inv() * team->m_totalSpirit;
    }
}

uf_Node* uf_Node::union1(uf_Node* root2) {

    bool root;
    Team* buyerTeam = this->find();
    Team* boughtTeam = root2->find();

    if (buyerTeam->m_size > boughtTeam->m_size) {
        root2->m_father = this;
        root2->m_team = nullptr;

        root2->m_data->m_spirit = this->m_data->m_spirit.inv() * buyerTeam->m_totalSpirit * root2->m_data->m_spirit;

        this->m_data->m_gamesPlayed -= boughtTeam->m_gamesPlayed;
        root2->m_data->m_gamesPlayed -= (this->m_data->m_gamesPlayed + buyerTeam->m_gamesPlayed);
        root = false;
   }
    else {
        this->m_father = root2;
        root2->m_team = buyerTeam;
        this->m_team = nullptr;
        buyerTeam->m_rootPlayersTree = root2;
        boughtTeam->m_rootPlayersTree = nullptr;

        root2->m_data->m_spirit = buyerTeam->m_totalSpirit * root2->m_data->m_spirit;
        this->m_data->m_spirit = root2->m_data->m_spirit.inv() * this->m_data->m_spirit;

        root2->m_data->m_gamesPlayed -= buyerTeam->m_gamesPlayed;
        this->m_data->m_gamesPlayed -= (root2->m_data->m_gamesPlayed + boughtTeam->m_gamesPlayed);

        root = true;

        }
    buyerTeam->m_totalSpirit = buyerTeam->m_totalSpirit * boughtTeam->m_totalSpirit;
    buyerTeam->m_gamesPlayed += boughtTeam->m_gamesPlayed;
    boughtTeam->m_rootPlayersTree = nullptr;

    if (!root) {
       return this;
    }

    else {
        return root2;
    }
}