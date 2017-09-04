//Duel.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "Duel.h"
#include "PlayerMage.h"
#include "Spellbook.h"
#include "MiscPollables.h"
#include "Roller.h"

#include <iostream>
#include <thread>
#include <chrono>

DuelPvE::DuelPvE(PlayerMage &player, weatherType &weather, Roller &r, const bool realtime, string &in):
	player(player),weather(weather),roller(r),realtime(realtime),in(in){}

//Copy constructor resets event status--'opponent' gets initialized to null and 'result' to ACTIVE
DuelPvE::DuelPvE(const DuelPvE &d):DuelPvE(d.player,d.weather,d.roller,d.realtime,d.in){}
DuelPvE::~DuelPvE(){
	//Make sure to check if 'opponent' has already been destroyed first
	if(initialized()){
		delete opponent->spellbook; opponent->spellbook=nullptr;
		delete opponent; opponent=nullptr;
	}
}

bool DuelPvE::initialized(){return opponent!=nullptr;}
void DuelPvE::Initialize(){
	//Create new random opponent based on player
	opponent=new Mage("Unknown Opponent",nullptr,player,roller);
	opponent->spellbook=new Spellbook(opponent->name()+"'s Spellbook",roller,opponent->level());
	
	cout<<"The duel begins!"<<skipl;
	
	//Print player and opponent header/statsheet
	cout<<player.header()<<endl<<player.statSheet()<<skipl<<"\tvs."<<skipl<<
	opponent->header()<<endl<<opponent->statSheet()<<skipl;
	
	//Equal stats favor the 'home mage' (not the player)
	const Mage* favoredMage =
	((player.level()>opponent->level()) + (player.vitalityMax()>opponent->vitalityMax()) + (player.damage()>opponent->damage()))>
	((player.level()<opponent->level()) + (player.vitalityMax()<opponent->vitalityMax()) + (player.damage()<opponent->damage()))?
	&player : opponent;
	cout<<"The betting house favors "<<favoredMage->name()<<" for this match."<<endl;
	
	cout<<"The weather is "<<weatherString(weather)<<". ";
	if(weather!=CLEAR)cout<<elementString(element(weather))<<" spells are more effective."<<skipl;
	else cout<<skipl;
	
	Pause(realtime,in,2);
	result=ACTIVE;
}
bool DuelPvE::Update(){
	if(!initialized())return true; //Don't execute unless initialized
	
	//There is a 35% chance of it even being possible for either mage to fail to cast a spell (unformed spell)
	int roll = roller.Roll(100)+1>=65? 7:6;
	
	//Prepare spells on both sides
	player.PrepareSpell(element(roller.Roll(roll)));
	opponent->PrepareSpell(element(roller.Roll(roll)));
	
	//Process single-step duel
	player.Duel(*opponent,weather,roller);
	cout<<player<<" @"<<player.vitalityString()<<" Vitality, "<<*opponent<<" @"<<opponent->vitalityString()<<" Vitality"<<skipl;
	
	Pause(realtime,in,1.5f);
	//Calculate duel result and return false if duel is complete
	result=player.vitality()<=0? opponent->vitality()<=0?DRAW:LOSS : opponent->vitality()<=0?WIN:ACTIVE;
	return result==ACTIVE;
}
void DuelPvE::Cleanup(){
	if(!initialized() || result==ACTIVE)return; //Don't execute unless initialized and duel has completed
	
	//Register player stats and apply XP
	switch((int)result){ //Casting to int removes annoying 'enum value not handled in switch' warning
		case WIN:
			cout<<"* You won!"<<endl;
			player.RegisterWin();
			player.ApplyXP(player.CalculateDuelXP(*opponent),element(roller.Roll(6)));
			break;
		case LOSS:
			cout<<"* You lost..."<<endl;
			player.RegisterLoss();
			player.ApplyXP(player.CalculateDuelXP(*opponent,0.5f),element(roller.Roll(6)));
			break;
		case DRAW:
			cout<<"* It's a draw."<<endl;
			player.RegisterDraw();
			player.ApplyXP(player.CalculateDuelXP(*opponent,0.75f),element(roller.Roll(6)));
			break;
	}
	//Reset player's Vitality
	player.ApplyVitality(player.vitalityMax());
	cout<<endl;
	
	//Delete opponent and spellbook
	delete opponent->spellbook; opponent->spellbook=nullptr;
	delete opponent; opponent=nullptr;
	
	Pause(realtime,in,2.5f);
}

void Pause(const bool realtime, string &in, const float seconds){
	if(realtime)this_thread::sleep_for(chrono::milliseconds(int(seconds*1000))); //Rounding here would make virtually no difference
	else{
		cout<<tab<<"Press 'Enter' to continue... ";
		getline(cin,in);
		cout<<endl;
	}
}
