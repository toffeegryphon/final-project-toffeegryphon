#include <city.h>
#include <configuration.h>
#include <utils.h>

namespace epidemic {

using utils::CompareX;
using utils::DistanceX;

City::City() : City(vec2()) {
}

City::City(const vec2& bounds)
    : City(bounds, Configuration::kDefaultPopulationSize,
           Configuration::kDefaultSickCount) {
}

City::City(const vec2& bounds, size_t population_size, size_t sick_count)
    : Location(Location::Type::kCity, bounds) {
  for (size_t i = 0; i < sick_count; ++i) {
    individuals_.emplace_back(bounds, Individual::Status::kAsymptomatic);
  }

  for (size_t i = 0; i < population_size - sick_count; ++i) {
    individuals_.emplace_back(bounds_);
  }
}

City::City(const vec2& bounds, const vector<Individual>& individuals)
    : Location(Location::Type::kCity, bounds) {
  individuals_ = individuals;
}

vector<Individual> City::Add(const vector<Individual>& individuals) {
  return Location::Add(individuals);
}

// Interaction
vector<Individual> City::ExtractIndividualsAt(const vec2& position) {
  return Location::ExtractIndividualsAt(position);
}

// Lifecycle
void City::Update() {
  for (Individual& individual : individuals_) {
    individual.Update(bounds_);
  }

  UpdateSpread();
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
const vector<Individual>& City::GetIndividuals() const {
  return Location::GetIndividuals();
}

// Private

void City::UpdateSpread() {
  sort(individuals_.begin(), individuals_.end(), CompareX);
  for (size_t i = 0; i < individuals_.size(); ++i) {
    const Individual& source = individuals_[i];
    if (source.IsSneezing()) {
      for (size_t j = i + 1; j < individuals_.size(); ++j) {
        if (DistanceX(individuals_[j], source) >
            Configuration::kDefaultSneezeRadius) {
          break;
        }
        individuals_[j].CheckAndBecomeInfected(source);
      }

      // TODO better way
      for (size_t j = i - 1; j < individuals_.size(); --j) {
        if (DistanceX(individuals_[j], source) >
            Configuration::kDefaultSneezeRadius) {
          break;
        }
        individuals_[j].CheckAndBecomeInfected(source);
      }
    }
  }
}

}  // namespace epidemic