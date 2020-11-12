#include <city.h>
#include <configuration.h>

namespace epidemic {

City::City(const vec2& bounds) : Location(bounds) {
  float sick_count = Configuration::kDefaultSickCount;
  for (size_t i = 0; i < sick_count; ++i) {
    individuals_.emplace_back(bounds, Individual::Status::kAsymptomatic);
  }

  for (size_t i = 0; i < Configuration::kDefaultPopulationSize - sick_count;
       ++i) {
    individuals_.emplace_back(bounds_);
  }
}

// Getters & Setters

const vec2& City::GetBounds() const {
  return Location::GetBounds();
}
const vector<Individual>& City::GetIndividuals() const {
  return Location::GetIndividuals();
}

}  // namespace epidemic