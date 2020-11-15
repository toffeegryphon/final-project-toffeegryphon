#include <isolation.h>

namespace epidemic {

// Constructors

Isolation::Isolation(const vec2& bounds)
    : Location(Location::Type::kIsolation, bounds) {
}

// Interaction

void epidemic::Isolation::Add(const vector<Individual>& individuals) {
  Location::Add(individuals);
}

vector<Individual> epidemic::Isolation::ExtractIndividualsAt(
    const vec2& position) {
  return Location::ExtractIndividualsAt(position);
}

// Lifecycle

void epidemic::Isolation::Update() {
  // TODO Update chances
  // TODO Spread check
  Location::Update();
}

void epidemic::Isolation::Draw(const vec2& offset) const {
  Location::Draw(offset);
}

// Getters & Setters

epidemic::Location::Type epidemic::Isolation::GetType() const {
  return Location::GetType();
}

const vec2& epidemic::Isolation::GetBounds() const {
  return Location::GetBounds();
}

const vector<Individual>& epidemic::Isolation::GetIndividuals() const {
  return Location::GetIndividuals();
}

}  // namespace epidemic