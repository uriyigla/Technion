#include "Team.h"

Team::operator int() const{
    return this->m_id;
}

bool Team::operator>(Team b) const{
    if (this->m_ability > b.m_ability){
        return true;
    }
    if (this->m_ability < b.m_ability){
        return false;
    }
    return (this->m_id > b.m_id);
}

void Team::unite(const Team* team2) {
    m_size += team2->m_size;
    m_points += team2->m_points;
    m_ability += team2->m_ability;
    if (team2->m_goalKeeper) {
        m_goalKeeper = true;
    }
}

void Team::updateAddPlayer(const permutation_t &spirit, int ability, bool goalKeeper) {
    this->m_totalSpirit = this->m_totalSpirit * spirit;
    this->m_ability += ability;
    this->m_size++;
    if (!this->m_goalKeeper) {
        this->m_goalKeeper = goalKeeper;
    }
}

void Team::swapTeam(Team *team2) {
    this->m_ability = team2->m_ability;
    this->m_goalKeeper = team2->m_goalKeeper;
    this->m_points = team2->m_points;
    this->m_gamesPlayed = team2->m_gamesPlayed;
    this->m_size = team2->m_size;
    this->m_totalSpirit = team2->m_totalSpirit;
    this->m_rootPlayersTree = team2->m_rootPlayersTree;

    team2->m_rootPlayersTree = nullptr;
}

