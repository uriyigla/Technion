
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "Players/Player.h"
#include "Cards/Card.h"
#include "Mtmchkin.h"
#include "Cards/Vampire.h"
#include "Cards/Barfight.h"
#include "Cards/Dragon.h"
#include "Cards/Fairy.h"
#include "Cards/Goblin.h"
#include "Cards/Merchant.h"
#include "Cards/Pitfall.h"
#include "Cards/Treasure.h"
#include "Players/Rogue.h"
#include "Players/Wizard.h"
#include "Players/Fighter.h"
#include "Exception.h"
#include <algorithm>
#include <sstream>
#include <random>
#include <cstdio>
#include <fstream>
#include <cstring>

using std::unique_ptr;
using std::cout;
using std::cerr;
using std::string;
using std::fstream;
using std::istringstream;
using std::vector;


/* ---------------------------------------------------------------------------------------------- */
// --------------------------------       General Helper Functions          ------------------------------

void createTextFile(const string &filename, const string &input)
{
    std::ofstream file(filename);
    if(file){
        file << input;
    }
}

void deleteTextFile(const string &filename)
{
    std::remove(filename.c_str());
}

bool compareFiles(const string &filename1, const string &filename2)
{
    string line1,line2;
    fstream file1(filename1),file2(filename2);
    if( !file2){
        cerr<<"Error opening file 2"<<std::endl;
        return false;
    }
    if(!file1 ){
        cerr<<"Error opening file 1"<<std::endl;
        return false;
    }
    while(!file1.eof()){ //read file until you reach the end
        getline(file1,line1);
        getline(file2,line2);
        if(!(line1==line2))
        {
            return false;
        }
    }
    if(!file2.eof()){
        return false;
    }
    return true;
}

bool GeneralGameSimulationTest(const string &tempDeckFilename, string input, string deck, string expectedOutputFileName)
{
    //   init cin from file
    createTextFile(tempDeckFilename+".txt",deck);
    istringstream in(input);
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf());

    std::ofstream outfile(tempDeckFilename+"out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(outfile.rdbuf());
    Mtmchkin game(tempDeckFilename+".txt");
    while(!game.isGameOver() && game.getNumberOfRounds() < 100){
        game.playRound();
        game.printLeaderBoard();
    }

    bool res = compareFiles(tempDeckFilename+"out.txt", expectedOutputFileName);
    outfile.close();
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    deleteTextFile(tempDeckFilename+".txt");
    return res;
}

void run_test(std::function<bool()> test, std::string test_name)
{
    if(!test()){
        std::cout<< test_name <<" FAILED."<<std::endl;
        std::cout << std::endl;
        return;
    }
    std::cout<<test_name<<" SUCCEEDED."<<std::endl;
    std::cout << std::endl;

}


/* ---------------------------------------------------------------------------------------------- */
// --------------------------------       Tests for Card class          ------------------------------

bool cardsPrintsTest()
{

    Barfight junta;
    Dragon mushu;
    Fairy alizaMalek;
    Goblin goblin;
    Merchant pizzaHut;
    Pitfall moedB;
    Treasure factor;
    Vampire dracula;
    cout << junta << std::endl << mushu << std::endl << alizaMalek
         << std::endl << goblin  << std::endl << pizzaHut
         << std::endl << moedB  << std::endl << factor
         << std::endl << dracula;
    return true;
}

bool playersPrintsTest()
{

    Rogue player1("Itay");
    Fighter player2("Efrat");
    Wizard player3("Jimmy");
    cout << player1 << std::endl << player2 << std::endl << player3
         << std::endl;
    return true;
}

bool testCard()
{
    vector<unique_ptr<Card>> cards;
    cards.push_back(unique_ptr<Card>(new Goblin()));
    cards.push_back(unique_ptr<Card>(new Vampire()));
    cards.push_back(unique_ptr<Card>(new Dragon()));
    cards.push_back(unique_ptr<Card>(new Treasure()));
    cards.push_back(unique_ptr<Card>(new Merchant()));
    cards.push_back(unique_ptr<Card>(new Fairy()));
    cards.push_back(unique_ptr<Card>(new Barfight()));
    cards.push_back(unique_ptr<Card>(new Pitfall()));
    for(unique_ptr<Card>& card : cards){
        cout << *card;
    }
    cards.erase(cards.begin(),cards.end());
    return true;
}

/* ---------------------------------------------------------------------------------------------- */
// --------------------------------       Tests for Mtmchkin class          ------------------------------

bool gameRunTest()
{
    //   init cin from file
    std::ifstream in("in.txt");
    if(!in.is_open()){
        throw std::exception();
    }
    std::cin.rdbuf(in.rdbuf());
    Mtmchkin game("gametest.txt");
    while(!game.isGameOver()){
        game.playRound();
    }

    return true;
}

bool dragonDenTest()
{
    const string tmp_file("dragonDen_test");
    string input("2\nJimmy Wizard\nPikachu Fighter");
    string deck("Dragon\nDragon\nDragon\nDragon\nDragon");
    string expectedOutputFilename("tests/dragonDen_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool vampireLairTest()
{
    const string tmp_file("vampireLiar_test");
    string input("2\nItay Fighter\nPikachu Rogue");
    string deck("Vampire\nVampire\nVampire\nVampire\nVampire");
    string expectedOutputFilename("tests/vampireLair_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool goblinCaveTest()
{
    const string tmp_file("goblinCave_test");
    string input("2\nItay Wizard\nPikachu Rogue");
    string deck("Goblin\nGoblin\nGoblin\nGoblin\nGoblin");
    string expectedOutputFilename("tests/goblinCave_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool nonMostersTest()
{
    const string tmp_file("noMonster_test");
    string input("2\nItay Wizard\nPikachu Rogue");
    string deck("Fairy\nBarfight\nPitfall\nTreasure\nFairy");
    string expectedOutputFilename("tests/noMonster_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool roundLimitTest()
{
    const string tmp_file("roundLimit_test");
    string input("2\nmatamDalf Wizard\nrocky Fighter");
    string deck("Fairy\nFairy\nFairy\nFairy\nFairy");
    string expectedOutputFilename("tests/roundLimit_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool allTenTest()
{
    const string tmp_file("allTen_test");
    string input("2\nmatamDalf Wizard\nrocky Fighter");
    string deck("Goblin\nGoblin\nGoblin\nGoblin\nGoblin");
    string expectedOutputFilename("tests/allTen_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool badPlayerInputTest()
{
    const string tmp_file("badPlayerInput_test");
    string input("2\nmatamDalf Wizardd\nmatamDalf rogoe\nmatamDalf Wizard\nrocky Fighter");
    string deck("Goblin\nVampire\nGoblin\nGoblin\nDragon");
    string expectedOutputFilename("tests/badPlayerInput_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool merchantInputTest()
{
    const string tmp_file("merchantInput_test");
    string input("2\nmatamDalf Wizardd\nmatamDalf rogoe\nmatamDalf Wizard\nrocky Fighter\n"
                 "1\n"
                 "1\n"
                 "0\n"
                 "2\n"
                 "0\n");
    string deck("Goblin\nGoblin\nMerchant\nMerchant\nDragon");
    string expectedOutputFilename("tests/merchantInput_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

/* ---------------------------------------------------------------------------------------------- */
// --------------------------------       Tests for Exceptions          ------------------------------

bool badSizeTest()
{
    const string tmp_file("badSize_test");
    string input("4\nBarbieGirl Wizard\nInABarbieWorld Rogue\nMadeOfPlastic Rogue\nITSFANTASTIC Wizard");
    string deck("Fairy");
    string expectedOutputFilename("notneeded.txt");
    bool flag= false;
    try{
        Mtmchkin("inputs/empty.txt");
    }
    catch(const DeckFileInvalidSize& e){
        flag = true;
    }
    return flag;
}

bool noFileTest()
{
    const string tmp_file("noFile_test");
    string input("2\nItay Wizard\nPikachu Rogue");
    string expectedOutputFilename("notneeded.txt");
    string deck("This_is_not_the_file_your_looking_for");
    bool flag = false;
    try{
        Mtmchkin("inputs/noFile.txt");
    }
    catch(const DeckFileNotFound& e){
        flag=true;
    }
    return flag;
}

bool badFormatTest()
{
    const string tmp_file("badFormat_test");
    string input("2\nItay Wizard\nPikachu Rogue");
    string deck("SomeBody Once told me the world is gonna roll me\nVampire\n");
    string expectedOutputFilename("notneeded.txt");
    bool flag = false;
    try {
        Mtmchkin("inputs/badFormat_test.txt");
    }
    catch(const DeckFileFormatError& e){
        if(strcmp(e.what(),"Deck File Error: File format error in line 2")==0) {
            flag=true;
        }
    }
    return flag;
}

bool badFormatStartTest()
{
    const string tmp_file("badFormat_test");
    string input("2\nItay Wizard\nPikachu Rogue");
    string deck("I aint the sharpest tool in the shed\nVampire");
    string expectedOutputFilename("notneeded.txt");
    bool flag = false;
    try {
        Mtmchkin("inputs/badFormat_test_start_of_file.txt");
    }
    catch(const DeckFileFormatError& e){
        if(strcmp(e.what(),"Deck File Error: File format error in line 1")==0)
        {
            flag = true;
        }
    }
    return flag;
}

/* ---------------------------------------------------------------------------------------------- */
/* --------------------------------        Bonus Tests         ---------------------------------- */
bool gangTest1(){
    const string tmp_file("gangTest1_test");
    string input("2\nmatamDalf Wizardd\nmatamDalf rogoe\nmatamDalf Wizard\nrocky Fighter");
    string deck("Goblin\nGang\nGoblin\nGoblin\nGoblin\nVampire\nDragon\nEndGang\nGoblin\nGoblin\nGoblin");
    string expectedOutputFilename("tests/gangTest1_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}


bool gangTest2(){
    const string tmp_file("gangTest2_test");
    string input("2\nOmerTests Wizardd\nRegev rogoe\nmatamDalf Wizard\nrocky Fighter");
    string deck("Gang\nDragon\nGoblin\nGoblin\nGoblin\nGoblin\nEndGang\nGoblin\nGoblin\nGoblin\nPitfall");
    string expectedOutputFilename("tests/gangTest2_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}

bool gangTest3(){
    const string tmp_file("gangTest3_test");
    string input("2\nOmerTests Wizardd\nRegev rogoe\nOmerTests Wizard\nrocky Fighter");
    string deck("Gang\nGoblin\nGoblin\nEndGang\nGoblin\nGoblin\nGoblin\nPitfall\nPitfall");
    string expectedOutputFilename("tests/gangTest3_test_expected.txt");
    return GeneralGameSimulationTest(tmp_file, input, deck, expectedOutputFilename);
}
/* ---------------------------------------------------------------------------------------------- */
// --------------------------------       Main function          ------------------------------

int main(){

    run_test(cardsPrintsTest,"cardsPrintsTest");
    run_test(playersPrintsTest,"playersPrintsTest");
    run_test(testCard,"Deck creation test");
    run_test(dragonDenTest,"Dragon Den simulation test");
    run_test(goblinCaveTest,"Goblin Cave simulation test");
    run_test(vampireLairTest,"Vampire Lair simulation test");
    run_test(nonMostersTest,"Non monsters cards simulation test");
    run_test(badFormatStartTest,"Bad format at start of file exception test");
    run_test(badFormatTest,"Bad format exception test");
    run_test(noFileTest,"File Doesnt exist exception test");
    run_test(badSizeTest,"Bad size exception test");
    run_test(roundLimitTest,"Round upper limit test");
    run_test(allTenTest,"All reach lvl 10 test");
    run_test(badPlayerInputTest,"Bad player input test");
//    run_test(gangTest1,"Gang Test 1");
//    run_test(gangTest2,"Gang Test 2");
//    run_test(gangTest3,"Gang Test 3");
    return 0;
}