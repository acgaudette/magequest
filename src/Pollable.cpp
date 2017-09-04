//Pollable.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "Pollable.h"
#include <iostream>
using namespace std;

//Protected

Pollable* const* const ConcurrentContainer::pollables() const{return pollables_;}
size_t ConcurrentContainer::pollablesCount() const{return pollablesCount_;}

//Public

Pollable::~Pollable(){}

ConcurrentContainer::ConcurrentContainer(Pollable* const* const pollables, const int pollablesCount):
	pollables_(pollables),pollablesCount_(pollablesCount){}

bool ConcurrentContainer::Update(){
	for(Pollable* const* p=pollables_;p<pollables_+pollablesCount_;p++){
		Event* const e = dynamic_cast<Event*>(*p);
		if(e && !e->initialized())e->Initialize(); //If Event, initialize
		
		if(!(*p)->Update()){ //Only continue if Update() returned false
			if(e)e->Cleanup(); //If Event, cleanup
			else return false; //If Pollable, exit immediately
		}
	}
	return true;
}

SequentialContainer::SequentialContainer(Pollable* const* const pollables, const int pollablesCount, const bool root):
	ConcurrentContainer(pollables,pollablesCount),root(root){}

//Special case for SequentialContainer, does not have to be initialized--instead just uses Cleanup()
bool SequentialContainer::initialized(){return true;}
void SequentialContainer::Initialize(){}

bool SequentialContainer::Update(){
	if(position>=pollablesCount())return false; //Make sure no invalid Pollables are processed
	
	Pollable* const* p = pollables()+position;
	Event* const e = dynamic_cast<Event*>(*p);
	if(e && !e->initialized())e->Initialize(); //If Event, initialize
	
	if((*p)->Update()==!e){ //Only continue if Update() returned true and type is Pollable or if Update() returned false and type is Event
		position++; //Increment sequential loop position
		if(e)e->Cleanup(); //If Event, cleanup
		//If (old) position is not last and type is SequentialBond, process the next Pollable in the sequential loop along with this one
		if(position<pollablesCount()){
			if(dynamic_cast<SequentialBond*>(*p) || dynamic_cast<SequentialBond*>(*(p+1)))return Update();
		}
	}
	else if(!e){ //Exit update immediately if Pollable returns false, resetting container
		if(root)Cleanup();
		return false;
	}
	
	if(position>=pollablesCount() && root)Cleanup(); //Cleanup automatically if root
	//Return false when ready to start again from the top
	return position<pollablesCount();
}
void SequentialContainer::Cleanup(){position=0;} //Reset sequential loop position

SequentialBond::SequentialBond(){}
SequentialBond& SequentialBond::instance(){
	static SequentialBond bond;
	return bond;
}
bool SequentialBond::Update(){return true;}