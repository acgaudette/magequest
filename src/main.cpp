//main.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

//Desired features for future development:
//	Random opponent name generation
//	More flavor text (attacks, setting, titles, etc.)
//	Better readability at speed--either a 'non-verbose' mode or an updating interface
//	Underlying math more visible to the user--expose complexity, but not too much
//	More content (coin, equipment, resting, travel, stats, heal/dodge)--although there is a charm to simplicity
//	Utilize the extensibility you've build into the code for cool features (for ex. pollables, entities, DuelXvX)
//	Gambling and AI duels
//	PvP with saves--'hotseat' multiplayer
//	Reputation based on WLR (win/loss ratio) or quality of matches--also opponent WLR
//	Lower/upper damage modifiers, critical damage
//	Spellbook naming, spellbook trading
//	More initial player customization--for ex. personality
//	More modifiers like weather

//Note: To demonstrate my competence in raw (vs. smart) pointers, I used them exclusively throughout this project

#include "Entity.h"
#include "Mage.h"
#include "PlayerMage.h"
#include "Spellbook.h"

#include "Pollable.h"
#include "Duel.h"
#include "MiscPollables.h"

#include "Enums.h"
#include "FileIO.h"
#include "Roller.h"

#include <iostream>
#include <string>
#include <thread>
#include <stddef.h>
using namespace std;

const string VERSION = "0.1a";
const string skipl = "\n\n";
const string tab = "\t";

//Shared data to pass around between the various main functions
struct SharedGameData{
	appState gameState = MENU;
	Roller roller;
	string in;
};

//Full definitions below
void ProcessMenu(SharedGameData&,bool&,FileIO&,PlayerMage*&);
string MenuOption(string&,const string&,const string& = "for defaults",const string& = ": ");
string LoopedMenuOption(string&, const string&);
void PlayGame(SharedGameData&,const bool,PlayerMage*&);
void GetRealtimeInput(appState&);

int main(int argc, const char* argv[]){ //I may process these later, so leaving them in
	cout<<endl<<"MageQuest v"<<VERSION<<" by Aaron C Gaudette"<<skipl;
	
	//Initialize shared data
	SharedGameData sharedGameData;
	bool realtime = true;
	FileIO fileIO;
	PlayerMage* player = nullptr;
	
	//Process the UI (and load data if necessary), run the game loop, then save and quit
	ProcessMenu(sharedGameData,realtime,fileIO,player);
	PlayGame(sharedGameData,realtime,player);
	fileIO.Save(*player);
	
	//Delete the player and its spellbook and exit the program
	//Spellbook pointers are not owned and managed by Mage instances--spellbooks, as separate entities, were built to be independent from mages
	delete player->spellbook; player->spellbook=nullptr;
	delete player; player=nullptr;
}

void ProcessMenu(SharedGameData &gd, bool &realtime, FileIO &fio, PlayerMage* &player){
	while(true){
		MenuOption(gd.in,"Enter character name to load previous save","to start a new game");
		//New game--player name and vMod/dMod customization
		if(gd.in.empty()){
			fio.name = LoopedMenuOption(gd.in,"Enter your new character's name (avoid special characters as you may lose your save file)");
			
			MenuOption(gd.in,"Enter your focus ('v' for vitality, 'd' for damage)");
			if(gd.in=="v" || gd.in=="V"){fio.vMod = DEFAULT_VMOD + 1; fio.dMod = DEFAULT_DMOD - 1;}
			else if(gd.in=="d" || gd.in=="D"){fio.vMod = DEFAULT_VMOD - 1; fio.dMod = DEFAULT_DMOD + 1;}
			else{fio.vMod = DEFAULT_VMOD; fio.dMod = DEFAULT_DMOD;}
			
			//Create new level 1 player (and spellbook) with the user's initial settings, then exit loop
			Spellbook* sb = new Spellbook(fio.name+"'s Spellbook");
			player = new PlayerMage(fio.name,sb,PlayerData(),1,fio.vMod,fio.dMod);
			break;
		}
		//Load player data... if successful, create player (and spellbook) and exit loop
		else if(fio.LoadData(gd.in)){
			Spellbook* sb = new Spellbook(fio.bookName,fio.spellLevels);
			player = new PlayerMage(fio,sb);
			break;
		}
	}
	//Determine realtime flag (will the game run automatically, or be advanced manually by the user)
	MenuOption(gd.in,"Enter 't' to run in turns/steps (recommended for new players)","for realtime");
	cout<<endl;
	if(gd.in=="t" || gd.in=="T")realtime=false;
	else{
		MenuOption(gd.in,"* While in realtime mode, enter 'q' to save and quit. Press 'Enter' to continue","","... ");
		cout<<endl;
	}
	
	//Exit UI and transition to playing state
	cout<<player->name()<<" steps into the dueling grounds..."<<skipl;
	gd.gameState=PLAYING;
}
//Menu option--returns input string as well as assigns to reference, so the function can be used dynamically
//Default 'alternative' = "for defaults", Default 'punctuation' = ":"
string MenuOption(string &in, const string &prompt, const string &alternative, const string &punctuation){
	cout<<prompt<<(alternative.empty()?"":", or press 'Enter' "+alternative)<<(punctuation.empty()?"":punctuation);
	getline(cin,in);
	return in;
}
//Like MenuOption, but will not return until input is registered
string LoopedMenuOption(string &in, const string &prompt){
	while(MenuOption(in,prompt,"").empty());
	return in;
}

void PlayGame(SharedGameData &gd, const bool realtime, PlayerMage* &player){
	//Initialize shared data
	int tick = 0;
	weatherType weather = CLEAR;
	
	//Start up a second thread for input if in realtime mode
	thread* input = nullptr;
	if(realtime)input = new thread(GetRealtimeInput,ref(gd.gameState));
	
	//Build day event container--first randomize the weather, then run a duel between the player and a random opponent, then increment the day counter
	RandomizeWeather randomizeWeather(weather,gd.roller);
	DuelPvE duel(*player,weather,gd.roller,realtime,gd.in);
	RegisterPlayerDay registerPlayerDay(*player);
	Pollable* dayPollables[5] = {&randomizeWeather,EVENT_BOND,&duel,EVENT_BOND,&registerPlayerDay};
	SequentialContainer day(dayPollables,5);
	
	//Build game loop container--each tick, display tick info (and determine via input whether to quit or continue when not in realtime mode),
	//update the day, and increment the tick counter
	ProcessTickHeader processTickHeader(tick,*player,gd.gameState,realtime,gd.in);
	Counter ticker(tick);
	Pollable* updatePollables[3] = {&processTickHeader,&day,&ticker};
	ConcurrentContainer gameLoop(updatePollables,3);
	
	//Run the game
	while(gd.gameState==PLAYING)gameLoop.Update();
	
	//If realtime, join and delete thread
	if(input!=nullptr){
		input->join();
		delete input;
		input=nullptr;
	}
}
//Realtime input function for second thread--exits game loop when appropriate signal is received
void GetRealtimeInput(appState &gameState){
	string in;
	while(in!="q")getline(cin,in);
	cout<<endl;
	gameState=EXITED;
}
