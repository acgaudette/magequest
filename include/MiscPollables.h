// MiscPollables.h

#ifndef MISC_POLLABLES_H
#define MISC_POLLABLES_H

#include "Pollable.h"
#include "Enums.h"

#include <string>

using namespace std;

class PlayerMage;
class Roller;

class Counter : public Pollable {

  int &count;

public:

  Counter(int &count);
  virtual bool Update();
};

// These classes read like functions because they essentially are--
// functions that execute in the context of a Pollable
// In the future, a Pollable callback wrapper class would be useful,
// likely using function pointers and the '...' specifier

class RandomizeWeather : public Pollable {

  weatherType &weather;
  Roller &roller;

public:

  RandomizeWeather(weatherType &weather, Roller &r);
  virtual bool Update();
};

class RegisterPlayerDay : public Pollable {

  PlayerMage &player;

public:

  RegisterPlayerDay(PlayerMage &player);
  virtual bool Update();
};

// Display tick info (and determine via input whether to quit or
// continue when not in realtime mode)
class ProcessTickHeader : public Pollable {

  int &tick;
  PlayerMage &player;
  appState &gameState;
  const bool realtime;
  string &in;

public:

  ProcessTickHeader(
    int &tick,
    PlayerMage &player,
    appState &gameState,
   const bool realtime,
   string &in
  );

  virtual bool Update();
};

#endif
