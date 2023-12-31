#include "Merchant.h"
#include <iostream>
#include <string>
#include "../Exception.h"
#include <fstream>

static const int BUY_HP = 5;
static const int BUY_FORCE = 10;
static const std::string MERCHANT = "Merchant";
static const int BASE = 10;
static const unsigned int LENGTH = 1;
static const int ZERO = 0;
static const int ONE = 1;
static const int TWO = 2;

using std::ifstream;
using namespace std;

Merchant::Merchant() : Card(){
    m_name = MERCHANT;
    m_buyHp = BUY_HP;
    m_buyForce = BUY_FORCE;
}

static bool numIsGood(int num){
    switch (num)
    {
        case ZERO:
            return true;
        case ONE:
            return true;
        case TWO:
            return true;
        default:
            return false;
    }
}

static void purchaseZero(Player &player){
    printMerchantSummary(std::cout, player.getName(), ZERO, ZERO);
}

static void purchaseOne(Player &player){
    if (player.getCoins() >= BUY_HP)
    {
        player.pay(BUY_HP);
        player.heal(ONE);
        printMerchantSummary(std::cout, player.getName(), ONE, BUY_HP);
        return;
    }
    printMerchantInsufficientCoins(std::cout);
    printMerchantSummary(std::cout, player.getName(), ONE, ZERO);
}

static void purchaseTwo(Player &player){
    if (player.getCoins() >= BUY_FORCE)
    {
        player.pay(BUY_FORCE);
        player.buff(ONE);
        printMerchantSummary(std::cout, player.getName(), TWO, BUY_FORCE);
        return;
    }
    printMerchantInsufficientCoins(std::cout);
    printMerchantSummary(std::cout, player.getName(), TWO, ZERO);
}

static void purchase(Player &player, int num){
    switch (num)
    {
        case ZERO:
            purchaseZero(player);
            return;
        case ONE:
            purchaseOne(player);
            return;
        case TWO:
            purchaseTwo(player);
            return;
    }
}

void Merchant::applyEncounter(Player &player) const{
    printMerchantInitialMessageForInteractiveEncounter(std::cout, player.getName(), player.getCoins());
    std::string str;
    try{
        getline(std::cin, str, '\n');
    }
    catch (const ifstream::failure &e){
        cout << "Exception getting line" << endl;
    }
    int len = str.length();
    int num = -1;
    try{
        num = std::stoi(str);
    }
    catch (...){}
    while (!((len == LENGTH) && (numIsGood(num)))){
        printInvalidInput();
        try{
            getline(std::cin, str, '\n');
        }
        catch (const ifstream::failure &e){
            cout << "Exception getting line" << endl;
        }
        try{
            num = std::stoi(str);
        }
        catch (...){}
        len = str.length();
    }
    purchase(player, num);
}
