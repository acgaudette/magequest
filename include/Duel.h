// Duel.h

#ifndef DUEL_H
#define DUEL_H

#include "Pollable.h"
#include "Enums.h"

#include <string>

using namespace std;

#define SKIPL "\n\n"

class Mage;
class PlayerMage;
class Roller;

// A duel event between the player and a random opponent
class DuelPvE : public Event {

  // Mage pointer for opponent, owned by the DuelPvE class
  Mage* opponent = nullptr;
  duelResult result = ACTIVE;

  PlayerMage &player;
  weatherType &weather;
  Roller &roller;
  const bool realtime;
  string &in;

public:

  DuelPvE(
    PlayerMage &player,
    weatherType &weather,
    Roller &r,
    const bool realtime,
    string &in
  );

  DuelPvE(const DuelPvE &d);

  // Note: implicit assignment operator function will not be generated
  // due to the const member variable as well as the reference member variables
  virtual ~DuelPvE();

  virtual bool initialized();

  virtual void Initialize();
  virtual bool Update();
  virtual void Cleanup();
};

// If not realtime, wait for input before continuing,
// otherwise pause and continue
void Pause(const bool realtime, string &in, const float seconds = 1);

#endif
