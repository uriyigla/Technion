//
// Created by Daniel_Meents on 12/04/2022.
//


#include "utilities.h"
#include <iostream>
using std::cout;
using std::endl;

const char* LINE_DIVIDER  = "------------------------";


void printBattleCardInfo(const CardStats& card)
{
    cout << "Card drawn:" << endl;
    cout << "Type: Battle" << endl;
    cout << "Force: " << card.force << endl;
    cout << "Profit (on win): " << card.loot << endl;
    cout << "Damage taken (on loss): " << card.hpLossOnDefeat << endl;
    cout << LINE_DIVIDER << endl;
}

void printBattleResult(bool win)
{
    if (win){
        cout << "The player defeated the monster and won the loot! Hooray!" << endl;
        cout << LINE_DIVIDER << endl;
    } else{
        cout << "After a long battle, the player has fled wounded and failed." << endl;
        cout << LINE_DIVIDER << endl;
    }
}

void printBuffCardInfo(const CardStats& card)
{
    cout << "Card drawn:" << endl;
    cout << "Type: Buff" << endl;
    cout << "Buff points: " << card.buff << endl;
    cout << "Cost: " << card.cost << endl;
    cout << LINE_DIVIDER << endl;
}

void printHealCardInfo(const CardStats& card)
{
    cout << "Card drawn:" << endl;
    cout << "Type: Heal" << endl;
    cout << "Heal points: " << card.heal << endl;
    cout << "Cost: " << card.cost << endl;
    cout << LINE_DIVIDER << endl;
}

void printTreasureCardInfo(const CardStats& card)
{
    cout << "Card drawn:" << endl;
    cout << "Type: Treasure" << endl;
    cout << "Coins: " << card.loot << endl;
    cout << LINE_DIVIDER << endl;
}

void printPlayerInfo(const char* name, const int level, const int force, const int hp, const int coins)
{
    cout << "Player Details:" << endl;
    cout << "Name: " << name << endl;
    cout << "Level: " << level << endl;
    cout << "Force: " << force << endl;
    cout << "HP: " << hp << endl;
    cout << "Coins: " << coins << endl;
    cout << LINE_DIVIDER << endl;
}

