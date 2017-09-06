// Spellbook.h

#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "Entity.h"
#include "Enums.h"

#include <string>

using namespace std;

class Roller;

// Spellbook class, representing a book of spells
// (which each represent one of the six elements),
// each with their own level and ability to level up
class Spellbook : public Entity {

  // The seventh spell represents an unformed spell,
  // the 'nonexistence' of a spell, and is always level 1
  int spellLevels_[7];

public:

  Spellbook(
    const string &name,
    const int fireLvl = 1,
    const int iceLvl = 1,
    const int waterLvl = 1,
    const int earthLvl = 1,
    const int shockLvl = 1,
    const int airLvl = 1,
    const int vMod = 1
  );

  Spellbook(
    const string &name,
    const int (&spellLevels)[6],
    const int vMod = 1
  );

  // Randomly generate a spellbook given a level
  Spellbook(
    const string &name,
    Roller &r,
    const int lvl = 1,
    const int vMod = 1
  );

  // Functions for getting readable spellbook data
  string spellString(const element e) const;
  virtual string statSheet() const;

  int spellLevel(const element e) const;

  // Note: overrides virtual inheritance from base class--
  // now a separate function
  bool LevelUp(const element e, const int delta = 1);
};

#endif
