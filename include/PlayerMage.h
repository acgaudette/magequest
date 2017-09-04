//PlayerMage.h
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#ifndef PLAYER_MAGE_H
#define PLAYER_MAGE_H

#include "Mage.h"
#include "FileIO.h"

#include <string>
#include <fstream>
using namespace std;

class Spellbook;

//Container of various player data, for a shorter constructor
struct PlayerData{
	int xp;
	int daysActive;
	int wins, losses, draws;
	PlayerData(const int xp = 0, const int daysActive = 0, const int wins = 0, const int losses = 0, const int draws = 0);
};

//PlayerMage class, representing the player--the player tracks various game statistics and
//levels up automatically via applied XP (experience points, generally awarded for completing some task)
class PlayerMage:public Mage{
	PlayerData playerData;
protected:
	virtual void PrintToStream(ostream &os) const;
public:
	PlayerMage(const string &name, Spellbook* const sb, const PlayerData &pd, const int lvl = 1,
			   const int vMod = DEFAULT_VMOD, const int dMod = DEFAULT_DMOD);
	//Create player from FileIO data (likely a savefile)
	PlayerMage(const FileIO &fio, Spellbook* const sb);
	
	//String functions for getting readable player data
	string xpString() const;
	virtual string header() const;
	
	//Various functions for getting player data
	int xp() const;
	int xpTarget() const;
	int daysActive() const;
	int wins() const;
	int losses() const;
	int draws() const;
	
	//Calculate XP from a duel with another mage (must be called directly after combat)
	int CalculateDuelXP(const Mage &opponent, const float factor = 1) const;
	//Apply XP to player, possibly leveling the player up
	void ApplyXP(const int delta, const element spellToLevel, const bool suppressMessage = false);
	virtual bool LevelUp(const element e, const int delta = 1);
	
	//Various functions for registering player statistics
	void RegisterDay();
	void RegisterWin();
	void RegisterLoss();
	void RegisterDraw();
};

#endif
