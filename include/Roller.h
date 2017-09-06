// Roller.h

#ifndef ROLLER_H
#define ROLLER_H

#include <random>

using namespace std;

// The Roller class generates quality random numbers--
// not a singleton, instead it is passed by reference down the chain
class Roller {

  random_device seeder;
  mt19937 engine;

public:

  Roller();

  int Roll(const int max); // [0, Max)
  int Range(const int min, const int max); // [Min, Max)

  float FloatRoll(const float max); // [0,Max)
  float FloatRange(const float min, const float max); // [Min, Max]
};

#endif
