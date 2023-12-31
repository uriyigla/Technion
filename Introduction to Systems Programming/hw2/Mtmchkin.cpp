#include "Mtmchkin.h"
#include "Player.h"
#include <string>

const int WIN_LEVEL = 10;

Mtmchkin::Mtmchkin(const char* playerName, const Card* cardsArray, int numOfCards)
        : m_player(""),
          m_gameStatus(GameStatus::MidGame),
          m_currentCardIndex(0),
          m_array(new Card[numOfCards]),
          m_numOfCards(numOfCards) {

    std::string str(playerName);
    Player newplayer(str);
    this->m_player = newplayer;

    for (int i = 0; i < numOfCards; i++) {
        this->m_array[i] = cardsArray[i];
    }
}

Mtmchkin::Mtmchkin(const Mtmchkin& mtmchkin)
        : m_player(mtmchkin.m_player),
          m_gameStatus(mtmchkin.m_gameStatus),
          m_currentCardIndex(mtmchkin.m_currentCardIndex),
          m_array(new Card[mtmchkin.m_numOfCards]),
          m_numOfCards(mtmchkin.m_numOfCards) {

    for (int i = 0; i < m_numOfCards; i++) {
        m_array[i] = mtmchkin.m_array[i];
    }
}

Mtmchkin::~Mtmchkin() {
    delete[] m_array;
}

Mtmchkin& Mtmchkin::operator=(const Mtmchkin& other) {
    if (this == &other) {
        return *this;
    }
    delete[] m_array;
    m_array = new Card[other.m_numOfCards];
    m_player = other.m_player;
    m_gameStatus = other.m_gameStatus;
    m_currentCardIndex = other.m_currentCardIndex;
    m_numOfCards = other.m_numOfCards;

    for (int i = 0; i < m_numOfCards; i++) {
        m_array[i] = other.m_array[i];
    }

    return *this;
}

GameStatus Mtmchkin::getGameStatus() const {
    return this->m_gameStatus;
}

void Mtmchkin::playNextCard() {
    m_array[m_currentCardIndex].printInfo();
    m_array[m_currentCardIndex].applyEncounter(m_player);
    m_player.printInfo();
    m_currentCardIndex++;
    if (m_currentCardIndex == m_numOfCards) {
        m_currentCardIndex = 0;
    }
}

bool Mtmchkin::isOver() const {
    return (this->m_player.getLevel() == WIN_LEVEL) || (this->m_player.isKnockedOut());
}
