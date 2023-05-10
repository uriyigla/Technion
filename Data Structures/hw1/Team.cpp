//
// Created by Shaked on 23/11/2022.
//

#include "Team.h"

bool Team::operator>(const Team& b) const {
    return this->m_id > b.m_id; //m_id is unique
}

bool Team::operator==(const Team& b) const {
    return this->m_id == b.m_id; //m_id is unique
}

bool Team::operator!=(const Team& b) const {
    return this->m_id != b.m_id; //m_id is unique
}
Team::operator int() const{
    return this->m_id;
}

void Team::addPlayerStatsToTeam  (const Player* playerAdded) {
    m_cards += playerAdded->m_cards;
    m_size += 1;
    m_goals += playerAdded->m_goals;
    if (playerAdded->m_goalKeeper){
        m_goalKeeper += 1;
    }
}

void Team::removePlayerStatsFromTeam  (const Player* playerAdded) {
    m_cards -= playerAdded->m_cards;
    m_size -= 1;
    m_goals -= playerAdded->m_goals;
    if (playerAdded->m_goalKeeper){
        m_goalKeeper -= 1;
    }
}

bool Team::isValid() const{
    return ((m_goalKeeper > 0) && (m_size >= 11));
}

void Team::playMatch(Team* team){
    int team1score=this->m_points+this->m_goals-this->m_cards;
    int team2score=team->m_points+team->m_goals-team->m_cards;

    this->m_games+=1;
    team->m_games+=1;

    if (team1score>team2score){
        this->m_points+=3;
        return;
    }
    if (team1score<team2score){
        team->m_points+=3;
        return;
    }
    this->m_points+=1;
    team->m_points+=1;
}

void Team::mergeTeam (const Team* team1,const Team* team2){

    this->m_goals=team1->m_goals+team2->m_goals;
    this->m_size=team1->m_size+team2->m_size;
    this->m_goalKeeper=team1->m_goalKeeper+team2->m_goalKeeper;
    this->m_points=team1->m_points+team2->m_points;
    this->m_cards=team1->m_cards+team2->m_cards;

    l_Node<Player>* iterator = team1->m_teamPlayersTree.m_list.m_head;
    int length1=team1->m_teamPlayersTree.m_list.m_size;
    for (int i=0;i<length1;i++){
        iterator->m_data->m_gamesWithoutPlayer+=team2->m_games;
        iterator=iterator->m_next;
    }

    iterator =team2->m_teamPlayersTree.m_list.m_head;
    int length2=team2->m_teamPlayersTree.m_list.m_size;
    for (int i=0;i<length2;i++){
        iterator->m_data->m_gamesWithoutPlayer+=team1->m_games;
        iterator=iterator->m_next;
    }

    this->m_games=team1->m_games+team2->m_games;

}