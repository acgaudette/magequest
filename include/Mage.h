// Mage.h

#ifndef MAGE_H
#define MAGE_H

#include "Entity.h"
#include "Enums.h"

#include <string>

using namespace std;

#define DEFAULT_DMOD 5

class PlayerMage;
class Spellbook;
class Roller;

// Mage class, representing beings who have spellbooks and use them
// to combat other entities or duel other mages
class Mage : public Entity {

  int damageMod_;
  element spell_ = UNFORMED;

public:

  // The mage's current spellbook (can be null),
  // a book of spells used for combat and dueling
  Spellbook* spellbook;

  Mage(
    const string &name,
    Spellbook* const sb,
    const int lvl = 1,
    const int vMod = DEFAULT_VMOD,
    const int dMod = DEFAULT_DMOD
  );

  // Generate a random mage similar in stats to another
  Mage(
    const string &name,
    Spellbook* const sb,
    const Mage &m,
    Roller &r,
    const float lvlVariation = DEFAULT_LVLVAR,
    const float statVariation = DEFAULT_STATVAR
  );

  // Copy constructors and assignment operator overloader
  Mage(const Mage &m, Spellbook* const sb);
  Mage(const Mage &m);
  Mage& operator=(const Mage &m);

  // Note: because the 'spellbook' pointer is managed outside
  // of the Mage instance, a destructor is not needed

  // String functions for getting readable entity data
  virtual string type() const;
  virtual string statSheet() const;

  // Currently readied spell
  element spell() const;

  // Damage represents the magical strength of a mage,
  // or how much a mage can negatively impact an entity's vitality
  // The base damage modifier is multiplied by the mage level
  // to get the final max damage
  int damageMod() const;
  int damage() const;

  // Ready a given spell or clear the currently readied spell
  void PrepareSpell(const element e);
  void ClearSpell();

  // Initiate and evaluate a single-step duel with another mage
  void Duel(Mage &opponent, const weatherType weather, Roller &r);

  // Note: overrides virtual inheritance from base class--now a separate function
  virtual bool LevelUp(const element e, const int delta = 1);
};

#endif
