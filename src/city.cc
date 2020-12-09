#include <city.h>
#include <configuration.h>
#include <utils.h>

namespace epidemic {

using utils::CompareX;
using utils::DistanceX;

City::City() : City(vec2()) {
}

City::City(const vec2& bounds) : City(bounds, vector<Individual*>()) {
}

City::City(const vec2& bounds, const vector<Individual*>& individuals)
    : Location(Location::Type::kCity, bounds) {
  individuals_ = individuals;
}

vector<Individual*> City::Add(const vector<Individual*>& individuals) {
  return Location::Add(individuals);
}

// Interaction
vector<Individual*> City::ExtractIndividualsAt(const vec2& position) {
  return Location::ExtractIndividualsAt(position);
}

// Lifecycle
void City::Update() {
  for (Individual* individual : individuals_) {
    individual->Update(bounds_, type_);
  }

  frame_ = (frame_ + 1) % cfg::kSpreadCheckFrequency.value;
  if (frame_ == 0) {
    UpdateSpread();
  }
}

void City::Draw(const vec2& offset) const {
  Location::Draw(offset);
}

// Getters & Setters

Location::Type City::GetType() const {
  return Location::GetType();
}

const vec2& City::GetBounds() const {
  return Location::GetBounds();
}
const vector<Individual*>& City::GetIndividuals() const {
  return Location::GetIndividuals();
}

// Private

void City::UpdateSpread() {
  sort(individuals_.begin(), individuals_.end(), CompareX);
  for (size_t i = 0; i < individuals_.size(); ++i) {
    const Individual* source = individuals_[i];
    if (source->IsSneezing()) {
      for (size_t j = i + 1; j < individuals_.size(); ++j) {
        if (DistanceX(*individuals_[j], *source) > cfg::kSneezeRadius.value) {
          break;
        }
        individuals_[j]->CheckAndBecomeInfected(*source);
      }

      for (size_t j = i - 1; j < individuals_.size(); --j) {
        if (DistanceX(*individuals_[j], *source) > cfg::kSneezeRadius.value) {
          break;
        }
        individuals_[j]->CheckAndBecomeInfected(*source);
      }
    }
  }
}

}  // namespace epidemic