// FileIO.h

#ifndef FILE_IO_H
#define FILE_IO_H

#include "Enums.h"
#include "Entity.h"
#include "Mage.h"

#include <string>
#include <fstream>

using namespace std;

#define DELM '$'
#define SKIPL "\n\n"

class PlayerMage;
class Roller;

// FileIO manages saving and loading player data
struct FileIO {

  // Player data to save/load with defaults
  string name = "Default Player", bookName = "Default Spellbook";
  int level = 1, vMod = DEFAULT_VMOD, dMod = DEFAULT_DMOD;
  int xp = 0, daysActive = 0, wins = 0, losses = 0, draws = 0;
  int spellLevels[6] = { 1, 1, 1, 1, 1, 1 };

  // Load data from file into FileIO
  bool LoadData(const string &filename);

  // Dump data from player into FileIO and save data into file
  void DumpPlayerData(const PlayerMage &player);
  bool SaveData(const string &filename) const;

  // Helpful function that does both at once,
  // given the file name is the player name
  void Save(const PlayerMage &player);
};

// Stream operator overloading for saving/loading
istream& operator>>(istream& is, FileIO& fio);
ostream& operator<<(ostream& os, const FileIO& fio);

#endif
