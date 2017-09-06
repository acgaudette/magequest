// Roller.cpp

#include "Roller.h"

Roller::Roller() { engine.seed(seeder()); } // Seed engine on construction

int Roller::Roll(const int max) {
  uniform_int_distribution<int> distribution(0, max - 1);
  return distribution(engine);
}

int Roller::Range(const int min, const int max){
  // If min is greater than max, just return min
  return min < max ? Roll(max - min) + min : min;
}

float Roller::FloatRoll(const float max){
  uniform_real_distribution<float> distribution(0, max);
  return distribution(engine);
}

float Roller::FloatRange(const float min, const float max){
  return min < max ? FloatRoll(max - min) + min : min;
}
