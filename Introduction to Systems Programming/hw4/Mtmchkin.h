#ifndef MTMCHKIN_H_
#define MTMCHKIN_H_
#include <string>
#include "Cards/Card.h"
#include <memory>
#include "Players//Player.h"


class Mtmchkin{

private:

    iterable_queue <std::shared_ptr <Card>> queueCards;
    iterable_queue <std::shared_ptr <Player>> queuePlaying;
    iterable_queue <std::shared_ptr <Player>>  queueWon;
    iterable_queue <std::shared_ptr <Player>>  queueLost;
    int m_numOfRounds;
    void setCards(std::string fileName);
    void cardForQueueCreate (const std::string& cardName,int number);
    void moveToNextInLine();
    void setPlayer(std::string nameAndJob);
    void setPlayers(int length);


public:
    Mtmchkin(const Mtmchkin&) = delete;
    
    /*
    * C'tor of Mtmchkin class
    *
    * @param filename - a file which contains the cards of the deck.
    * @return
    *      A new instance of Mtmchkin.
    */
    explicit Mtmchkin(const std::string &fileName);
    
    /*
    * Play the next Round of the game - according to the instruction in the exercise document.
    *
    * @return
    *      void
    */
    void playRound();
    
    /*
    * Prints the leaderBoard of the game at a given stage of the game - according to the instruction in the exercise document.
    *
    * @return
    *      void
    */
    void printLeaderBoard() const;
    
    /*
    *  Checks if the game ended:
    *
    *  @return
    *          True if the game ended
    *          False otherwise
    */
    bool isGameOver() const;
    
	/*
    *  Returns the number of rounds played.
    *
    *  @return
    *          int - number of rounds played
    */
    int getNumberOfRounds() const;
};



#endif /* MTMCHKIN_H_ */
