//Entity.h
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <fstream>
using namespace std;

const int DEFAULT_VMOD = 10;
const float DEFAULT_LVLVAR = 0.3f;
const float DEFAULT_STATVAR = 0.25f;

class Roller;

//Base entity class, representing beings or objects, including the player--not abstract because instantiating an Entity is a valid action
class Entity{
	//Order is important for initialization
	string name_;
	int level_;
	int vitalityMod_;
	int vitality_;
protected:
	//Function for overloaded '<<' operator, separate from overloaded operator function below to support polymorphism (virtual)
	virtual void PrintToStream(ostream &os) const;
public:
	Entity(const string &name, const int lvl = 1, const int vMod = DEFAULT_VMOD);
	//Generate a random entity similar in stats to another
	Entity(const string &name, const Entity &e, Roller &r, const float lvlVariation = DEFAULT_LVLVAR, const float statVariation = DEFAULT_STATVAR);
	
	//Virtual destructor, facilitating polymorphism
	virtual ~Entity();
	
	//Various string functions for getting readable entity data
	string name() const;
	virtual string type() const;
	string vitalityString() const;
	virtual string header() const;
	virtual string statSheet() const;
	
	//Various functions for getting entity stats
	int level() const;
	//Vitality is how much damage an entity can take before falling in combat
	//The base vitality modifier is multiplied by the entity level to get the final max vitality
	int vitalityMod() const;
	int vitalityMax() const;
	int vitality() const;
	
	//Functions that modify entity data directly
	virtual bool LevelUp(const int delta = 1); //Returning a bool allows derived classes to execute based on the base function's results
	void ApplyVitality(const int delta);
	
	//Member function so that PrintToStream can stay protected vs. public, friend so that 'this' doesn't interfere with the operator overload
	friend ostream& operator<<(ostream &os, const Entity &e);
	
	//Generate a random stat given a base value and factor of variation in either direction
	static int randomStat(const int stat, const float variation, Roller &r);
};

#endif