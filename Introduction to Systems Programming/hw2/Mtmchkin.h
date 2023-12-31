//
// Created by Daniel_Meents on 07/04/2022.
//

#ifndef EX2_GAME_H
#define EX2_GAME_H

#include "Card.h"
#include <iostream>

enum class GameStatus { Win, Loss, MidGame };

class Mtmchkin {
public:
    Mtmchkin(const char* playerName, const Card* cardsArray, int numOfCards);
    Mtmchkin(const Mtmchkin& mtmchkin);
    ~Mtmchkin();
    Mtmchkin& operator=(const Mtmchkin& other);

    void playNextCard();
    bool isOver() const;
    GameStatus getGameStatus() const;

private:
    Player m_player;
    GameStatus m_gameStatus;
    int m_currentCardIndex;
    Card* m_array;
    int m_numOfCards;
};

#endif //EX2_GAME_H
