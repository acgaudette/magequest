// MiscPollables.cpp

#include "MiscPollables.h"
#include "PlayerMage.h"
#include "Roller.h"

#include <iostream>

Counter::Counter(int &count):count(count) { }

bool Counter::Update() {
  count++;
  return true;
}

RandomizeWeather::RandomizeWeather(
  weatherType &weather,
  Roller &r
) : weather(weather), roller(r) { }

bool RandomizeWeather::Update() {
  weather = weatherType(roller.Roll(7));
  return true;
}

RegisterPlayerDay::RegisterPlayerDay(PlayerMage &player) : player(player) { }

bool RegisterPlayerDay::Update() {
  player.RegisterDay();
  return true;
}

ProcessTickHeader::ProcessTickHeader(
  int &tick,
  PlayerMage &player,
  appState &gameState,
  const bool realtime,
  string &in
) : tick(tick),
  player(player),
  gameState(gameState),
  realtime(realtime),
  in(in) { }

bool ProcessTickHeader::Update() {

  // Print player and tick data
  cout << "\tYear " << int(player.daysActive() / 365.0f) + 1 << ", Day "
    << (player.daysActive() % 365) + 1 << ", Tick " << tick
    << " (" << player.wins() << "W-" << player.losses()
    << "L-" << player.draws() << "D)" << endl;

  // Exit game loop when appropriate signal is received
  if (!realtime) {
    cout << "\tPress 'Enter' to continue, or 'q' to save and quit: ";
    getline(cin, in);

    if (in == "q") {
      cout << endl;
      gameState = EXITED;
      return false;
    }
  }

  cout << endl;
  return true;
}
