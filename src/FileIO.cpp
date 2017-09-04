//FileIO.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "FileIO.h"
#include "PlayerMage.h"
#include "Spellbook.h"

#include <iostream>

bool FileIO::LoadData(const string &filename){
	ifstream saveFile(filename+".mq");
	if(!saveFile)cout<<"Error while opening save file."<<skipl;
	else{
		saveFile>>*this; //Read file into FileIO
		if(!saveFile)cout<<"Error while reading save file."<<skipl;
	}
	saveFile.close();
	return (bool)saveFile; //Return true if load was successful, false if there was an error
}

void FileIO::DumpPlayerData(const PlayerMage &player){ //If the player doesn't have a spellbook, a default one is saved
	name=player.name(); bookName=player.spellbook?player.spellbook->name():"Default Spellbook";
	level=player.level(); vMod=player.vitalityMod(); dMod=player.damageMod();
	
	for(int e=0;e<UNFORMED;e++)spellLevels[e]=player.spellbook? player.spellbook->spellLevel((element)e) : 1;
	
	xp=player.xp(); daysActive=player.daysActive();
	wins=player.wins(); losses=player.losses(); draws=player.draws();
}
bool FileIO::SaveData(const string &filename) const{
	ofstream saveFile(filename+".mq");
	if(saveFile){
		saveFile<<*this; //Write FileIO to file
		saveFile.close();
	}
	else cout<<"Error while saving game."<<skipl;
	return (bool)saveFile; //Return true if save was successful, false if there was an error
}
void FileIO::Save(const PlayerMage &player){
	DumpPlayerData(player);
	SaveData(player.name());
}

istream& operator >>(istream &is, FileIO &fio){
	string in;
	//Read an int from a string before a delimiter
	auto loadInt = [&](int &destination){
		getline(is,in,delm);
		try{destination=stoi(in);}
		catch(const invalid_argument&){
			cout<<"Error while reading save file--file has been modified or written to improperly."<<endl;
		}
	};
	
	//Load names on first two lines
	getline(is,fio.name);
	getline(is,fio.bookName);
	
	//Load ints on the third line
	loadInt(fio.level); loadInt(fio.vMod); loadInt(fio.dMod);
	for(int e=0;e<UNFORMED;e++)loadInt(fio.spellLevels[e]);
	loadInt(fio.xp); loadInt(fio.daysActive);
	loadInt(fio.wins); loadInt(fio.losses); loadInt(fio.draws);
	
	return is;
}
ostream& operator <<(ostream &os, const FileIO &fio){
	//Write names on first two lines
	os<< fio.name <<endl<< fio.bookName <<endl<<
	
	//Write ints on the third line with delimiter
	fio.level <<delm<< fio.vMod <<delm<< fio.dMod <<delm;
	for(int e=0;e<UNFORMED;e++)os<< fio.spellLevels[e] <<delm;
	os<< fio.xp <<delm<< fio.daysActive <<delm<< fio.wins <<delm<< fio.losses <<delm<< fio.draws <<endl<<
	
	"MQv"<<VERSION<<"--Modifying this file may corrupt your game data";
	
	return os;
}
