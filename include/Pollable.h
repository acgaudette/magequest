//Pollable.h
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#ifndef POLLABLE_H
#define POLLABLE_H

#include <stddef.h>

#define EVENT_BOND &SequentialBond::instance()

//The eponymous Pollable class is an abstract base class for any object that can be polled (for ex. in an update loop)
class Pollable{
public:
	virtual ~Pollable();
	virtual bool Update() = 0; //Returns false when desired result is immediate exit from parent loop
};
//Like Pollable, but has a start and end point
class Event:public virtual Pollable{
public:
	virtual bool initialized() = 0;
	
	virtual void Initialize() = 0;
	virtual bool Update() = 0; //Returns false if finished updating, true if in the process of updating
	virtual void Cleanup() = 0;
};

//A ConcurrentContainer holds multiple Pollables and iterates over them every Update(), calling Initialize() and Cleanup() on Events
class ConcurrentContainer:public virtual Pollable{
	//A (dynamic) container would have been better here, but instead I used this opportunity to demonstrate pointer-to-pointers and pointer arithmetic
	//Right now the array is immutable but could be opened up later if neccessary
	Pollable* const* const pollables_;
	const size_t pollablesCount_;
protected:
	Pollable* const* const pollables() const;
	size_t pollablesCount() const;
public:
	ConcurrentContainer(Pollable* const* const pollables, const int pollablesCount);
	//There is no need to have a copy constructor considering all member variables are const--introducing one would add unnecessary complications
	ConcurrentContainer(ConcurrentContainer const&) = delete;
	//Note: implicit assignment operator function will not be generated due to the const member variables
	//Ownership of 'pollables' falls to code hosting the container and such a destructor does not have to be defined here
	
	virtual bool Update();
};
//A SequentialContainer holds multiple Pollables and iterates over them in sequence, calling Initialize() and Cleanup() on Events
class SequentialContainer:public Event,public ConcurrentContainer{
	int position = 0;
public:
	//If root, the SequentialContainer will call Cleanup() on itself
	const bool root; //Right now the root flag is immutable but could be opened up later if neccessary
	
	SequentialContainer(Pollable* const* const pollables, const int pollablesCount, const bool root = false);
	SequentialContainer(SequentialContainer const&) = delete;
	
	virtual bool initialized();
	
	virtual void Initialize();
	virtual bool Update();
	virtual void Cleanup();
};
//A singleton that binds two Events together in a SequentialContainer so they can be processed together instead of separately between updates
class SequentialBond:public Pollable{
	SequentialBond();
public:
	static SequentialBond& instance();
	virtual bool Update();
	
	SequentialBond(SequentialBond const&) = delete;
	SequentialBond& operator=(SequentialBond const&) = delete;
};

#endif