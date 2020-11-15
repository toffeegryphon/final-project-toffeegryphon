#include <configuration.h>
#include <isolation.h>

namespace epidemic {

// Constructors

Isolation::Isolation(const vec2& bounds)
    : Isolation(bounds, Configuration::kIsolationDefaultCapacity) {
}

Isolation::Isolation(const vec2& bounds, size_t capacity)
    : Location(Location::Type::kIsolation, bounds) {
  capacity_ = capacity;
}

// Interaction

vector<Individual> epidemic::Isolation::Add(
    const vector<Individual>& individuals) {
  size_t total = individuals_.size() + individuals.size();
  if (total > capacity_) {
    size_t limit = individuals.size() - total + capacity_;
    Location::Add(
        vector<Individual>(individuals.begin(), individuals.begin() + limit));
    return vector<Individual>(individuals.begin() + limit, individuals.end());
  }
  return Location::Add(individuals);
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