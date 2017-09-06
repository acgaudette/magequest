// Spellbook.cpp

#include "Spellbook.h"
#include "Roller.h"

#include <iostream>
#include <vector>
#include <algorithm>

Spellbook::Spellbook(
  const string &name,
  const int fireLvl,
  const int iceLvl,
  const int waterLvl,
  const int earthLvl,
  const int shockLvl,
  const int airLvl,
  const int vMod
) : Entity(
  name,
  // Calculate spellbook level in reverse by combining each spell level
  fireLvl + iceLvl + waterLvl + earthLvl + shockLvl + airLvl - 5,
  vMod
), spellLevels_{ fireLvl, iceLvl, waterLvl, earthLvl, shockLvl, airLvl, 1 } { }

Spellbook::Spellbook(
  const string &name,
  const int (&spellLevels)[6],
  const int vMod
) : Spellbook(
  name,
  spellLevels[0],
  spellLevels[1],
  spellLevels[2],
  spellLevels[3],
  spellLevels[4],
  spellLevels[5],
  vMod
) { }

// Generate random levels for each of the six spells such that the total
// combined level equals the input spellbook level
Spellbook::Spellbook(
  const string &name,
  Roller &r,
  const int lvl,
  const int vMod
) : Entity(name, lvl, vMod) {
  // First get seven values: 0, input level (starting from zero), and five
  // random levels equal to or less than the input level (starting from zero)
  vector<int> randomLevels({
    0,
    r.Roll(lvl),
    r.Roll(lvl),
    r.Roll(lvl),
    r.Roll(lvl),
    r.Roll(lvl),
    lvl - 1
  });

  // Sort the values from least to greatest
  // The optimal method, actually, would be a sorting network
  sort(
    randomLevels.begin() + 1,
    randomLevels.end() - 1
  );

  // Next, each value is subtracted from the next value in the vector,
  // and 1 is added to shift the levels so they properly start from one
  vector<int>::iterator i;
  for (i = randomLevels.begin(); i != randomLevels.end() - 1; ++i) {
    *i = (*(i + 1) - *i) + 1;
  }

  // Remove last element, as it no longer serves any purpose
  randomLevels.pop_back();

  // Copy the vector into the array and initialize the unformed spell marker
  copy(randomLevels.begin(), randomLevels.end(), spellLevels_);
  spellLevels_[6] = 1;
}

string Spellbook::spellString(const element e) const {
  return elementString(e,"unformed spell")
    + (e != UNFORMED ? " (Lvl-" + to_string(spellLevels_[e]) + ")" : "");
}

// Iterate through elements and return their spell strings combined
string Spellbook::statSheet() const {
  string s = "";

  for (int e = 0; e < UNFORMED; ++e) {
    s += spellString((element)e);
    if (e < UNFORMED - 1) s += ", ";
  }

  return s;
}

int Spellbook::spellLevel(const element e) const { return spellLevels_[e]; }

// Level up one of the spells, but not the unformed spell marker
bool Spellbook::LevelUp(const element e, const int delta) {
  if (!Entity::LevelUp(delta) || e == UNFORMED)
    return false;

  spellLevels_[e] += delta;

  cout << "'" << elementString(e) << "' spell now level "
    << spellLevels_[e] << endl;
  return true;
}
