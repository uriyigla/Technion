#ifndef MTMCHKIN_H_
#define MTMCHKIN_H_

#include <string>
#include "Cards/Card.h"
#include <memory>
#include "Players/Player.h"
#include "IterableQueue.h"

class Mtmchkin {
private:
    iterable_queue<std::shared_ptr<Card>> queueCards;
    iterable_queue<std::shared_ptr<Player>> queuePlaying;
    iterable_queue<std::shared_ptr<Player>> queueWon;
    iterable_queue<std::shared_ptr<Player>> queueLost;
    int m_numOfRounds;

    void setCards(const std::string& fileName);
    void cardForQueueCreate(const std::string& cardName, int number);
    void moveToNextInLine();
    void setPlayer(const std::string& nameAndJob);
    void setPlayers(int length);

public:
    Mtmchkin(const Mtmchkin&) = delete;

    explicit Mtmchkin(const std::string& fileName);

    void playRound();

    void printLeaderBoard() const;

    bool isGameOver() const;

    int getNumberOfRounds() const;
};

#endif /* MTMCHKIN_H_ */
