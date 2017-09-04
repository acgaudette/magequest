//Enums.h
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <bitset>
using namespace std;

enum appState{MENU,PLAYING,EXITED}; //State of app determines flow

//The six elements, each one either greater than, less than, or equal to each other element
//Spells represent elements in a harnessed and levelable form
//The seventh element represents the 'nonexistence' of an element
enum element{FIRE,ICE,WATER,EARTH,SHOCK,AIR,UNFORMED};
string elementString(const element e, const string &unformed = "unformed element");

//These seven bitsets hold the comparison flags for each element (1 for greater than, 0 for lesser than or equal)
//Each element is more powerful than two other elements, less powerful than two other elements, and equal in power to two other elements
const bitset<7> elementFlags[7] = {0b0100011,0b0010011,0b1001001,0b1000101,0b0110001,0b0001101,0b0000000};

bool operator  <(const element l, const element r);
bool operator  >(const element l, const element r);
bool operator <=(const element l, const element r);
bool operator >=(const element l, const element r);
bool operator ==(const element l, const element r);
bool operator !=(const element l, const element r);

//The seven weather types, each one boosting a particular element
enum weatherType{HOT,COLD,WET,DUSTY,STORMY,WINDY,CLEAR};
string weatherString(const weatherType w);

enum duelResult{WIN,LOSS,DRAW,ACTIVE}; //State of duel determines flow and result calculation

#endif