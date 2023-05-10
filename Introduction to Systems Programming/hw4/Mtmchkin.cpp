#include "Mtmchkin.h"
#include "utilities.h"
#include "Players/Player.h"
#include <string>
#include <fstream>
#include "Cards/Barfight.h"
#include "Cards/Dragon.h"
#include "Cards/Fairy.h"
#include "Cards/Goblin.h"
#include "Cards/Merchant.h"
#include "Cards/Pitfall.h"
#include "Cards/Treasure.h"
#include "Cards/Vampire.h"
#include "Players/Rogue.h"
#include "Players/Wizard.h"
#include "Players/Fighter.h"
#include "Exception.h"

using std::ifstream;
using namespace std;

static const int CARDS_NUMBER = 8;
static const std::string CARDS_NAMES [CARDS_NUMBER]=
        {"Barfight","Dragon","Fairy","Goblin","Merchant",
         "Pitfall","Treasure","Vampire"};
static const int WIN_LEVEL = 10;
static const char LOW_LIMIT = '2';
static const char HIGH_LIMIT = '6';
static const int MAX_PLAYER_NAME_LENGTH=15;
static const std::string ROGUE = "Rogue";
static const std::string WIZARD = "Wizard";
static const std::string FIGHTER = "Fighter";
static const int MIN_SIZE = 5;

static bool isNumGood(std::string strNum)
{
    if (strNum.size()!=1)
    {
        return false;
    }
    return ((int)LOW_LIMIT<=(int) strNum[0])&&((int) strNum[0]<=(int)HIGH_LIMIT);
}
static bool isPlayer (std::string playerName)
{
    if (playerName.length()>MAX_PLAYER_NAME_LENGTH)
    {
        return false;
    }
    int length=playerName.length();
    for (int i=0;i<length;i++)
    {
        if (!isalpha(playerName[i])){
            return false;
        }
    }
    return true;
}
static bool isJob (std::string jobName)
{
    return ((jobName.compare(ROGUE)==0)||(jobName.compare(WIZARD)==0)||(jobName.compare(FIGHTER)==0));
}
void Mtmchkin::setCards(std::string fileName)
{
    int lineNum=1;
    std::ifstream file;
    try {
        file.open(fileName);
    }
    catch (const ifstream::failure& e){
        cout << "Exception opening/reading file"<<endl;
    }
    if (!file.is_open()){
        throw (DeckFileNotFound());
    }
    std::string line;
    try {
        while (getline(file, line)){
            cardForQueueCreate(line,lineNum);
            lineNum++;
        }
    }
    catch (const ifstream::failure& e){
        cout << "Exception getting line"<<endl;
    }
    file.close();
    if (queueCards.size()<MIN_SIZE){
        throw ( DeckFileInvalidSize());
    }
}
void Mtmchkin::cardForQueueCreate (const std::string &cardName,int number) {
    if (cardName.compare(CARDS_NAMES[0]) == 0) {
        Barfight *temp = new Barfight;
        shared_ptr<Barfight> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[1]) == 0) {
        Dragon *temp = new Dragon;
        shared_ptr<Dragon> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[2]) == 0) {
        Fairy *temp = new Fairy;
        shared_ptr<Fairy> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[3]) == 0) {
        Goblin *temp = new Goblin;
        shared_ptr<Goblin> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[4]) == 0) {
        Merchant *temp = new Merchant;
        shared_ptr<Merchant> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[5]) == 0) {
        Pitfall *temp = new Pitfall;
        shared_ptr<Pitfall> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[6]) == 0) {
        Treasure *temp = new Treasure;
        shared_ptr<Treasure> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    if (cardName.compare(CARDS_NAMES[7]) == 0) {
        Vampire *temp = new Vampire;
        shared_ptr<Vampire> tempPtr(temp);
        queueCards.push(tempPtr);
        return;
    }
    throw(DeckFileFormatError(number));
}

static int getNumberOfPlayers()
{
    std::string strNum;
    printEnterTeamSizeMessage();
    try {
        getline(std::cin, strNum);
    }
    catch (const ifstream::failure& e){
        cout << "Exception getting line"<<endl;
    }
    while (!isNumGood(strNum))
    {
        printInvalidTeamSize();
        printEnterTeamSizeMessage();
        try {
            getline(std::cin, strNum);
        }
        catch (const ifstream::failure& e){
            cout << "Exception getting line"<<endl;
        }
    }
    int num= (int)strNum[0]-(int)('0');
    return num;
}
static bool goodSpace (std::string name)
{
    int spaceNum=0;
    int length=name.length();
    for (int i=0;i<length-1;i++)
    {
        if (name[i]==' ')
        {
            spaceNum++;
        }
    }
    return (spaceNum==1);
}
static bool validNameAndJob(std::string nameAndJob)
{
    if(!goodSpace(nameAndJob))
    {
        printInvalidInput();
        return false;
    }
    std::string name,job;
    try {
        name = nameAndJob.substr(0, nameAndJob.find(' '));
        job = nameAndJob.substr(nameAndJob.find(' ') + 1);
    }
    catch (std::out_of_range& e) {
        cout << "out of range substring"<<endl;
    }
    if (!isPlayer(name))
    {
        printInvalidName();
        return false;
    }
    if (!isJob(job))
    {
        printInvalidClass();
        return false;
    }
    return true;
}
void Mtmchkin::setPlayer(std::string nameAndJob)
{
    std::string name,job;
    try {
        name = nameAndJob.substr(0, nameAndJob.find(' '));
        job = nameAndJob.substr(nameAndJob.find(' ') + 1);
    }
    catch (std::out_of_range& e) {
        cout << "out of range substring"<<endl;
    }
    if (job.compare("Rogue")==0)
    {
        Rogue* temp=new Rogue(name);
        shared_ptr<Rogue> tempSharedPtr(temp);
        queuePlaying.push(tempSharedPtr);
    }
    if (job.compare("Wizard")==0)
    {
        Wizard* temp=new Wizard(name);
        shared_ptr<Wizard> tempSharedPtr(temp);
        queuePlaying.push(tempSharedPtr);
    }
    if (job.compare("Fighter")==0)
    {
        Fighter* temp=new Fighter(name);
        shared_ptr<Fighter> tempSharedPtr(temp);
        queuePlaying.push(tempSharedPtr);
    }
}
void Mtmchkin::setPlayers(int length)
{
    std::string nameAndJob;
    for (int i=0;i<length;i++)
    {
        printInsertPlayerMessage();
        try {
            getline(std::cin, nameAndJob);
        }
        catch (const ifstream::failure& e){
            cout << "Exception getting line"<<endl;
        }
        while (!validNameAndJob(nameAndJob)) {
            try {
                getline(std::cin, nameAndJob);
            }
            catch (const ifstream::failure& e){
                cout << "Exception getting line"<<endl;
            }
        }
        setPlayer(nameAndJob);
    }
}

 Mtmchkin::Mtmchkin(const std::string& fileName)
{
    printStartGameMessage();
    m_numOfRounds=0;
    setCards(fileName);
    int length=getNumberOfPlayers();
    setPlayers(length);
}

void Mtmchkin::moveToNextInLine() {
    if (queuePlaying.front()->isKnockedOut())
    {
        int len=queueLost.size();
        queueLost.push(queuePlaying.front());
        queuePlaying.pop();
        for (int i=0;i<len;i++)
        {
            queueLost.push(queueLost.front());
            queueLost.pop();
        }
    }
    else
    {
        if (queuePlaying.front()->getLevel()==WIN_LEVEL)
        {
            queueWon.push(queuePlaying.front());
            queuePlaying.pop();
        }
        else
        {
            queuePlaying.push(queuePlaying.front());
            queuePlaying.pop();
        }
    }
}
void Mtmchkin::playRound()
{
    int len=queuePlaying.size();
    if (len==0)
    {
        return;
    }
    m_numOfRounds++;
    printRoundStartMessage(m_numOfRounds);
    for (int i=0;i<len;i++)
    {
        printTurnStartMessage(queuePlaying.front()->getName());
        queueCards.front()->applyEncounter(*(queuePlaying.front()));
        queueCards.push(queueCards.front());
        queueCards.pop();
        moveToNextInLine();
    }

    if (isGameOver())
    {
        printGameEndMessage();
    }

}
void printForPlayerQueue(const iterable_queue<std::shared_ptr <Player>> queue,int rank) {
    iterable_queue<std::shared_ptr <Player>>::const_iterator i;
    int innerRank=1;
    for(  i=queue.begin();i!=queue.end();i++)
    {
        printPlayerLeaderBoard(innerRank+rank,**i);
        innerRank++;
    }
}
void Mtmchkin::printLeaderBoard() const
{
    printLeaderBoardStartMessage();
    printForPlayerQueue(queueWon,0);
    printForPlayerQueue(queuePlaying,queueWon.size());
    printForPlayerQueue(queueLost,queueWon.size()+queuePlaying.size());
}
bool Mtmchkin::isGameOver() const
{
    return (queuePlaying.size()==0);
}
int Mtmchkin::getNumberOfRounds() const
{
    return m_numOfRounds;
}
