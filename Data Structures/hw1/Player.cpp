//
// Created by Shaked on 23/11/2022.
//

#include "Player.h"
#include <cmath>


bool Player::operator==(const Player& a) const {
    return this->m_id == a.m_id;
}

bool Player::operator!=(const Player& a) const {
    return this->m_id != a.m_id;
}

bool Player::operator>(const Player& a) const {

    if (this->m_goals>a.m_goals)
        return true;
    if (this->m_goals<a.m_goals)
        return false;

    if (this->m_cards>a.m_cards)
        return false;
    if (this->m_cards<a.m_cards)
        return true;

    if (this->m_id < a.m_id)
        return false;

    return true;

}

Player::operator int() const{
    return this->m_id;
}

void Player::updateStats (int gamesPlayed,int  scoredGoals,int cardsReceived){
    m_gamesAlone+=gamesPlayed;
    m_goals+=scoredGoals;
    m_cards+=cardsReceived;
}

int Player::closer (Player* player1,Player* player2){

    int dif_1_goals=abs(this->m_goals-player1->m_goals);
    int dif_2_goals=abs(player2->m_goals-this->m_goals);
    if (dif_1_goals<dif_2_goals){
        return player1->m_id;
    }
    if (dif_1_goals>dif_2_goals){
        return player2->m_id;
    }
    int dif_1_cards=abs(player1->m_cards-this->m_cards);
    int dif_2_cards=abs(this->m_cards-player2->m_cards);
    if (dif_1_cards<dif_2_cards){
        return player1->m_id;
    }
    if (dif_1_cards>dif_2_cards){
        return player2->m_id;
    }
    int dif_1_id=abs(player1->m_id-this->m_id);
    int dif_2_id=abs(this->m_id-player2->m_id);
    if (dif_1_id<dif_2_id){
        return player1->m_id;
    }
    if (dif_1_id>dif_2_id){
        return player2->m_id;
    }
    if (player1->m_id>player2->m_id){
        return player1->m_id;
    }
    return player2->m_id;
}
