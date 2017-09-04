//Spellbook.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "Spellbook.h"
#include "Roller.h"

#include <iostream>
#include <vector>
#include <algorithm>

Spellbook::Spellbook(const string &name,
	const int fireLvl, const int iceLvl, const int waterLvl, const int earthLvl, const int shockLvl, const int airLvl,
	const int vMod):
		//Calculate spellbook level in reverse by combining each spell level (remember, a level starts at one, not zero)
		Entity(name,fireLvl+iceLvl+waterLvl+earthLvl+shockLvl+airLvl-5,vMod),spellLevels_{fireLvl,iceLvl,waterLvl,earthLvl,shockLvl,airLvl,1}{}

Spellbook::Spellbook(const string &name, const int (&spellLevels)[6], const int vMod):
	Spellbook(name,spellLevels[0],spellLevels[1],spellLevels[2],spellLevels[3],spellLevels[4],spellLevels[5],vMod){}

//Generate random levels for each of the six spells such that the total combined level equals the input spellbook level
Spellbook::Spellbook(const string &name, Roller &r, const int lvl, const int vMod):Entity(name,lvl,vMod){
	//I used this space to demonstrate usage of the STL
	//This also could have been performed recursively
	
	//First get seven values: 0, input level (starting from zero), and five random levels equal to or less than the input level (starting from zero)
	vector<int> randomLevels({0,r.Roll(lvl),r.Roll(lvl),r.Roll(lvl),r.Roll(lvl),r.Roll(lvl),lvl-1});
	//Sort the values from least to greatest
	sort(randomLevels.begin()+1,randomLevels.end()-1); //The optimal method, actually, would be a sorting network (only five elements)
  // TODO: this was sort?
	
	//Next, each value is subtracted from the next value in the vector, and 1 is added to shift the levels so they properly start from one
	vector<int>::iterator i;
	for(i=randomLevels.begin();i!=randomLevels.end()-1;i++){
		*i=(*(i+1)-*i)+1;
	}
	randomLevels.pop_back(); //Remove last element, as it no longer serves any purpose
	
	//Copy the vector into the array and initialize the unformed spell marker
	copy(randomLevels.begin(),randomLevels.end(),spellLevels_);
	spellLevels_[6]=1;
}

string Spellbook::spellString(const element e) const{
	return elementString(e,"unformed spell")+(e!=UNFORMED? " (Lvl-"+to_string(spellLevels_[e])+")" : "");
}
string Spellbook::statSheet() const{ //Iterate through elements and return their spell strings combined
	string s = "";
	for(int e = 0;e<UNFORMED;e++){
		s+=spellString((element)e);
		if(e<UNFORMED-1)s+=", ";
	}
	return s;
}

int Spellbook::spellLevel(const element e) const{return spellLevels_[e];}

//Level up one of the spells, but not the unformed spell marker
bool Spellbook::LevelUp(const element e, const int delta){
	if(!Entity::LevelUp(delta) || e==UNFORMED)return false;
	spellLevels_[e]+=delta;
	cout<<"'"<<elementString(e)<<"' spell now level "<<spellLevels_[e]<<endl;
	return true;
}
