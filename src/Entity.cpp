// Entity.cpp

#include "Entity.h"
#include "Roller.h"

#include <iostream>
#include <sstream>
#include <math.h>

// Protected

void Entity::PrintToStream(ostream &os) const { os << name_; }

// Public

Entity::Entity(
  const string &name,
  const int lvl,
  const int vMod
) : name_(name),
  level_(lvl),
  vitalityMod_(vMod),
  vitality_(vitalityMax()) { };

Entity::Entity(
  const string &name,
  const Entity &e,
  Roller &r,
  const float lvlVariation,
  const float statVariation
) : Entity(name, randomStat(e.level(), lvlVariation, r),
  randomStat(DEFAULT_VMOD, statVariation, r)) { }

Entity::~Entity() { }

string Entity::name() const { return name_; }
string Entity::type() const { return "Unknown"; }

string Entity::vitalityString() const {
  return to_string(vitality()) + "/" + to_string(vitalityMax());
}

// String representing essential entity data in one line
string Entity::header() const {
  stringstream ss;
  ss << *this << ", Level " << level() << " " << type() << ", "
    << vitalityString() << " Vitality";

  return ss.str();
}

// Generally multi-line string representing entity stats,
// commonly prefixed by the header
string Entity::statSheet() const { return "Nothing to see here"; }

int Entity::level() const { return level_; }
int Entity::vitalityMod() const { return vitalityMod_; }
int Entity::vitalityMax() const { return vitalityMod() * level(); }
int Entity::vitality() const { return vitality_; }

bool Entity::LevelUp(const int delta) {
  // Don't accept negative deltas
  if (delta <= 0 ) return false;

  level_ += delta;

  cout << "* " << *this << " leveled up! (Now level " << level_ << ")" << endl;
  return true;
}

// Apply vitality (heal or cause damage), but don't overflow or underflow
void Entity::ApplyVitality(const int delta) {
  vitality_ + delta <= 0 ? vitality_ = 0 : vitality_ + delta >= vitalityMax()
    ? vitality_ = vitalityMax() : vitality_ += delta;
}

ostream& operator<<(ostream &os, const Entity &e) {
  e.PrintToStream(os);
  return os;
}

int Entity::randomStat(const int stat, const float variation, Roller &r) {
  return fmax(1, round(
    stat + (stat * r.FloatRange(-variation, variation))
  ));
}
