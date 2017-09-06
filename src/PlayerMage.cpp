// PlayerMage.cpp

#include "PlayerMage.h"

#include <iostream>
#include <math.h>

PlayerData::PlayerData(
  const int xp,
  const int daysActive,
  const int wins,
  const int losses,
  const int draws
) : xp(xp),
  daysActive(daysActive),
  wins(wins),
  losses(losses),
  draws(draws) { }

// Protected

// Override the '<<' operator to append the player marker '(You)'
void PlayerMage::PrintToStream(ostream &os) const { os << name() << " (You)"; }

// Public

PlayerMage::PlayerMage(
  const string &name,
  Spellbook* const sb,
  const PlayerData &pd,
  const int lvl,
  const int vMod,
  const int dMod
) : Mage(name, sb, lvl, vMod, dMod),
  playerData(pd) { }

PlayerMage::PlayerMage(
  const FileIO &fio,
  Spellbook* const sb
) : PlayerMage(
  fio.name,
  sb,
  PlayerData(
    fio.xp,
    fio.daysActive,
    fio.wins,
    fio.losses,
    fio.draws
  ),
  fio.level,
  fio.vMod,
  fio.dMod
) { }

string PlayerMage::xpString() const {
  return to_string(xpTarget() - playerData.xp) + " XP to next level";
}

string PlayerMage::header() const {
  // Override header() to return XP data
  return Mage::header() + ", " + xpString();
}

int PlayerMage::xp() const { return playerData.xp; }

// Function for level XP targets (how much XP it takes to level up,
// determined by level) -- 50 + 10.25 * 0.8(x+5)^1.8
int PlayerMage::xpTarget() const {
 // Rounding is not required here because bias does not affect balance
 return int(50 + 10.25f * 0.8f * pow(level() + 5, 1.8f));
}

int PlayerMage::daysActive() const { return playerData.daysActive; }
int PlayerMage::wins() const { return playerData.wins; }
int PlayerMage::losses() const { return playerData.losses; }
int PlayerMage::draws() const { return playerData.draws; }

// Function for calculating XP from mage duels:
int PlayerMage::CalculateDuelXP(const Mage &opponent, const float factor) const {
  // Reasoning: Incorporates all elements of the duel fairly while scaling to
  // player level (note: applies level difference bias)
  float resultFactor = 10 * ((
    opponent.vitalityMax() - opponent.vitality()
    + opponent.damage() + vitality()
  ) / level());

  // Reasoning: Level difference between the mages factors in greatly
  // as well as an XP boost as duels grow higher in level
  float levelFactor = pow(opponent.level(), 1.8f) / pow(level(), 1.8f)
    * (1 + (opponent.level() / 180.0f));

  return fmax(1, round(resultFactor * levelFactor * factor));
}

void PlayerMage::ApplyXP(
  const int delta,
  const element spellToLevel,
  const bool suppressMessage
) {
  if (delta <= 0) return; // Don't accept negative deltas
  playerData.xp += delta;

  // Calculate overflow (how much the delta carries on into the next level)
  int overflow = xp() > xpTarget() ? xp() - xpTarget() : 0;

  // Only show this message once no matter
  // how many times the function is called
  if (!suppressMessage) cout << *this << " gained " << delta << " XP." << endl;

  // If XP is equal to or greater than the level XP target,
  // level up and apply the overflow XP
  if (xp() >= xpTarget()) {
    LevelUp(spellToLevel);
    ApplyXP(overflow, spellToLevel, true);
  }
}

// Reset XP for next level on level up
bool PlayerMage::LevelUp(const element e, const int delta) {
  if (!Mage::LevelUp(e, delta)) return false;
  playerData.xp = 0;
  return true;
}

void PlayerMage::RegisterDay() { playerData.daysActive++; }
void PlayerMage::RegisterWin() { playerData.wins++; }
void PlayerMage::RegisterLoss() { playerData.losses++; }
void PlayerMage::RegisterDraw() { playerData.draws++; }
