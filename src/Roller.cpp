//Roller.cpp
//MageQuest v0.1a by Aaron C Gaudette
//An autonomous and balanced single-player RPG featuring elemental magic duels
//Created by Aaron C Gaudette on 22.06.15

#include "Roller.h"

Roller::Roller(){engine.seed(seeder());} //Seed engine on construction

int Roller::Roll(const int max){
	uniform_int_distribution<int> distribution(0,max-1);
	return distribution(engine);
}
int Roller::Range(const int min, const int max){
	return min<max? Roll(max-min)+min : min; //If min is greater than max, just return min
}
float Roller::FloatRoll(const float max){
	uniform_real_distribution<float> distribution(0,max);
	return distribution(engine);
}
float Roller::FloatRange(const float min, const float max){
	return min<max? FloatRoll(max-min)+min : min;
}