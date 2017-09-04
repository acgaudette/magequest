//Enums.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "Enums.h"

string elementString(const element e, const string &unformed){ //The caller determines what is returned if case UNFORMED
	switch(e){
		case FIRE: return "Fire"; break;
		case ICE: return "Ice"; break;
		case WATER: return "Water"; break;
		case EARTH: return "Earth"; break;
		case SHOCK: return "Shock"; break;
		case AIR: return "Air"; break;
		case UNFORMED: return unformed; break;
		default: return "unknown element"; break;
	}
}
//Returns true if both the left element's flag for the right element is false, and the right element's flag for the left element is true
bool operator <(const element l, const element r){
	return !(elementFlags[l].test(6-r)) && elementFlags[r].test(6-l); //Note inversion of bitset
};
bool operator  >(const element l, const element r){return r<l;};
bool operator <=(const element l, const element r){return !(r<l);};
bool operator >=(const element l, const element r){return r<=l;};
bool operator ==(const element l, const element r){return r<=l && l<=r;}
bool operator !=(const element l, const element r){return !(l==r);}

string weatherString(const weatherType w){
	switch(w){
		case HOT: return "hot and dry"; break;
		case COLD: return "freezing cold"; break;
		case WET: return "wet and humid"; break;
		case DUSTY: return "thick and dusty"; break;
		case STORMY: return "violent and stormy"; break;
		case WINDY: return "extremely windy"; break;
		case CLEAR: return "clear and temperate"; break;
		default: return "inscrutable"; break;
	}
}