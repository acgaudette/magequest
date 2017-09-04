//Duel.h
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#ifndef DUEL_H
#define DUEL_H

#include "Pollable.h"
#include "Enums.h"

#include <string>
using namespace std;

extern const string skipl;
extern const string tab;

class Mage;
class PlayerMage;
class Roller;

//A duel event between the player and a random opponent
//In the future I'd like to create an abstract DuelXvX class for more extensibility or possibly a Duel class that takes two Entities as parameters
class DuelPvE:public Event{
	Mage* opponent = nullptr; //Mage pointer for opponent, owned by the DuelPvE class
	duelResult result = ACTIVE;
	
	PlayerMage &player;
	weatherType &weather;
	Roller &roller;
	const bool realtime;
	string &in;
public:
	DuelPvE(PlayerMage &player, weatherType &weather, Roller &r, const bool realtime, string &in);
	
	DuelPvE(const DuelPvE &d);
	//Note: implicit assignment operator function will not be generated due to the const member variable as well as the reference member variables
	virtual ~DuelPvE();
	
	virtual bool initialized();
	
	virtual void Initialize();
	virtual bool Update();
	virtual void Cleanup();
};

//If not realtime, wait for input before continuing, otherwise pause and continue
void Pause(const bool realtime, string &in, const float seconds = 1);

#endif
