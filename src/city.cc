#include <city.h>
#include <configuration.h>
#include <utils.h>

namespace epidemic {

using utils::CompareX;
using utils::DistanceX;

City::City(const vec2& bounds)
    : City(bounds, Configuration::kDefaultPopulationSize,
           Configuration::kDefaultSickCount) {
}

City::City(const vec2& bounds, size_t population_size, size_t sick_count)
    : Location(bounds) {
  for (size_t i = 0; i < sick_count; ++i) {
    individuals_.emplace_back(bounds, Individual::Status::kAsymptomatic);
  }

  for (size_t i = 0; i < population_size - sick_count; ++i) {
    individuals_.emplace_back(bounds_);
  }
}

City::City(const vec2& bounds, const vector<Individual>& individuals)
    : Location(bounds) {
  individuals_ = individuals;
}

// Lifecycle
void City::Update() {
  for (Individual& individual : individuals_) {
    individual.Update(bounds_);
  }

  UpdateSpread();
}

// Getters & Setters

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