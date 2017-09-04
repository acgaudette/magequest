//Mage.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "Mage.h"
#include "PlayerMage.h"
#include "Spellbook.h"
#include "Roller.h"

#include <iostream>
#include <math.h>

//Public

Mage::Mage(const string &name, Spellbook* const sb, const int lvl, const int vMod, const int dMod):
	Entity(name,lvl,vMod),spellbook(sb),damageMod_(dMod){}

Mage::Mage(const string &name, Spellbook* const sb,  const Mage &m, Roller &r, const float lvlVariation, const float statVariation):
	Entity(name,m,r,lvlVariation,statVariation),spellbook(sb),damageMod_(randomStat(DEFAULT_DMOD,statVariation,r)){}

Mage::Mage(const Mage &m, Spellbook* const sb):Mage(m.name(),sb,m.level(),m.vitalityMod(),m.damageMod()){}
//Note: the vanilla copy constructor assigns null to the new mage, because mages do not own their spellbook pointers
Mage::Mage(const Mage &m):Mage(m,nullptr){}
Mage& Mage::operator=(const Mage& m){
	if(this!=&m){
		//Note here the ommision of the 'spellbook' (and 'spell_') variable--mages will keep the value they had before the use of the assignment operator
		//The other option here, setting 'spellbook' to null, could potentially cause dangerous dangling pointer issues
		Entity::operator=(m);
		damageMod_=m.damageMod_;
	}
	return *this;
}

string Mage::type() const{return "Mage";}
string Mage::statSheet() const{
	//Return spellbook stat sheet with damage if a spellbook exists, otherwise just damage
	return (spellbook?spellbook->statSheet()+"\n":"")+to_string(damage())+" base damage";
}

element Mage::spell() const{return spellbook?spell_:UNFORMED;} //If no spellbook, just return UNFORMED regardless
int Mage::damageMod() const{return damageMod_;}
int Mage::damage() const{return damageMod()*level();}

void Mage::PrepareSpell(const element e){spell_=e;}
void Mage::ClearSpell(){spell_=UNFORMED;}

//Single-step duel--spells must be prepared beforehand for each mage in order for proper combat to ensue
void Mage::Duel(Mage &opponent, const weatherType weather, Roller &r){
	//Helpful functions to cut down on long, repeated code--will work even if one of the mages has no spellbook
	auto spellLevel = [](const Mage &m){
		return m.spellbook?m.spellbook->spellLevel(m.spell()):1;
	};
	auto spellString = [](const Mage &m){
		return m.spellbook?m.spellbook->spellString(m.spell()):elementString(UNFORMED,"unformed spell");
	};
	//Note: because of spell(), UNFORMED is returned when a mage has no spellbook
	
	//Function for calculating damage imparted from one mage to another
	//(Spell) level difference factor -- 1 + max between -0.99 and ((victor spell level * 0.1) ^ 1/2) - ((victim spell level * 0.1) ^ 1/2)
	//	Reasoning: Initial level differences are substantial but taper off in the upper ranges to prevent OP attacks
	//	A successful attack will always cause > 0 damage, but will be dampened if the victim's spell's level is higher
	//Weather modifier -- 1.5 if weather matches victor's spell (evaluated first), 0.66 if weather matches victim's spell, otherwise 1
	//	If both spells match and are affected positively by weather, the only result is that more damage is caused on both sides
	//	Clear weather (unformed spell) has no effect
	//Final damage -- max between 1 and victor damage stat * level difference factor * weather modifier
	auto totalDamage = [&](const Mage &victor, const Mage &victim){
		float levelFactor = 1+fmax(-0.99f,pow(spellLevel(victor)*0.1f,0.5f)-pow(spellLevel(victim)*0.1f,0.5f));
		float weatherMod = weather!=CLEAR? (int)weather==(int)victor.spell()? 1.5f : (int)weather==(int)victim.spell()? 0.66f : 1 : 1;
		return fmax(1,round(victor.damage()*levelFactor*weatherMod));
	};
	
	//Mages cast different spells and one is greater, causing damage
	if(spell()!=opponent.spell()){
		//Determine victor based on comparison operator for elements
		Mage *victor, *victim;
		victor = spell()<opponent.spell()?&opponent:this;
		victim = victor==&opponent?this:&opponent;
		cout<<*victor<<" casts '"<<spellString(*victor)<<"' which overpowers "<<*victim<<"'s '"<<spellString(*victim)<<"'!"<<endl;
		
		//Calculate damage and apply it to victim
		int damageDealt = totalDamage(*victor,*victim);
		victim->ApplyVitality(-damageDealt);
		cout<<"("<<damageDealt<<" damage dealt to "<<*victim<<")"<<endl;
	}
	//Mages cast the same spell, both causing damage
	else if(!(spell()==UNFORMED && opponent.spell()==UNFORMED)){
		cout<<*this<<" casts '"<<spellString(*this)<<"' which matches "<<opponent<<"'s '"<<spellString(opponent)<<"'."<<endl;
		
		//Damage is calculated and applied to both mages, but half of what it would be otherwise
		int damageDealt = int(totalDamage(*this,opponent)*0.5f), damageTaken = int(totalDamage(opponent,*this)*0.5f);
		opponent.ApplyVitality(-damageDealt);
		ApplyVitality(-damageTaken);
		cout<<"("<<opponent<<" is dealt "<<damageDealt<<" damage, "<<*this<<" is dealt "<<damageTaken<<" damage)"<<endl;
	}
	//Both mages fail to cast a spell (or have no spellbook), causing no damage
	else cout<<*this<<" and "<<opponent<<" both fail to cast a spell and cause no damage to each other."<<endl;
	
	//Clear spells afterwards
	ClearSpell();
	opponent.ClearSpell();
}
//A mage will level up its spellbook if it has one
bool Mage::LevelUp(const element e, const int delta){
	if(!Entity::LevelUp(delta))return false;
	if(spellbook)spellbook->LevelUp(e,delta);
	return true;
}
