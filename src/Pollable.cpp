// Pollable.cpp

#include "Pollable.h"
#include <iostream>

using namespace std;

// Protected

Pollable* const* const ConcurrentContainer::pollables() const {
  return pollables_;
}

size_t ConcurrentContainer::pollablesCount() const {
  return pollablesCount_;
}

// Public

Pollable::~Pollable() { }

ConcurrentContainer::ConcurrentContainer(
  Pollable* const* const pollables,
  const int pollablesCount
) : pollables_(pollables),
  pollablesCount_(pollablesCount) { }

bool ConcurrentContainer::Update() {
  for (
    Pollable* const* p = pollables_;
    p < pollables_ + pollablesCount_;
    ++p
  ) {
    Event* const e = dynamic_cast<Event*>(*p);
    // If Event, initialize
    if (e && !e->initialized()) e->Initialize();

    // Only continue if Update() returned false
    if (!(*p)->Update()) {
      if (e)e->Cleanup(); // If Event, cleanup
      else return false; // If Pollable, exit immediately
    }
  }

  return true;
}

SequentialContainer::SequentialContainer(
  Pollable* const* const pollables,
  const int pollablesCount,
  const bool root
) : ConcurrentContainer(pollables, pollablesCount),
  root(root) { }

// Special case for SequentialContainer, does not have to be initialized--
// instead just uses Cleanup()
bool SequentialContainer::initialized() { return true; }

void SequentialContainer::Initialize() { }

bool SequentialContainer::Update() {
  // Make sure no invalid Pollables are processed
  if (position >= pollablesCount()) return false;

  Pollable* const* p = pollables() + position;
  Event* const e = dynamic_cast<Event*>(*p);
  if (e && !e->initialized()) e->Initialize(); // If Event, initialize

  // Only continue if Update() returned true and type is Pollable or
  // if Update() returned false and type is Event
  if ((*p)->Update() == !e) {
    position++; // Increment sequential loop position
    if(e) e->Cleanup(); // If Event, cleanup

    //If (old) position is not last and type is SequentialBond,
    // process the next Pollable in the sequential loop along with this one
    if (position<pollablesCount()) {
      if (
        dynamic_cast<SequentialBond*>(*p)
        || dynamic_cast<SequentialBond*>(*(p+1))
      ) { return Update(); }
    }
  }

  // Exit update immediately if Pollable returns false, resetting container
  else if (!e) {
    if(root) Cleanup();
    return false;
  }

  // Cleanup automatically if root
  if (position >= pollablesCount() && root) Cleanup();
  // Return false when ready to start again from the top
  return position < pollablesCount();
}

// Reset sequential loop position
void SequentialContainer::Cleanup() { position = 0; }

SequentialBond::SequentialBond() { }

SequentialBond& SequentialBond::instance() {
  static SequentialBond bond;
  return bond;
}

bool SequentialBond::Update() { return true; }
