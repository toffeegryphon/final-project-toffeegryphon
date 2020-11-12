#include <location.h>

namespace epidemic {

// Constructors

epidemic::Location::Location(const vec2& bounds) : bounds_(bounds) {
}

// Lifecycle

void Location::Update() {
}

void Location::Draw(const vec2& offset) const {
  for (const Individual& individual : individuals_) {
    individual.Draw(offset);
  }
}

// Getters & Setters

const vec2& Location::GetBounds() const {
  return bounds_;
}

const vector<Individual>& Location::GetIndividuals() const {
  return individuals_;
}

}  // namespace epidemic
