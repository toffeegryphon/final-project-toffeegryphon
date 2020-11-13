#include <city.h>
#include <configuration.h>
#include <utils.h>

namespace epidemic {

using glm::distance;
using std::upper_bound;
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

void City::Add(const vector<Individual>& individuals) {
  individuals_.insert(individuals_.end(), individuals.begin(),
                      individuals.end());
}

// Interaction
// TODO Test
vector<Individual> City::ExtractIndividualsAt(const vec2& position) {
  float radius = Configuration::kDefaultIndividualRadius;
  // TODO Binary search
  vector<size_t> to_remove;
  vector<Individual> individuals;

  // TODO sort of hacky...
  Individual target((vec2()));
  target.SetPosition(vec2(position.x - radius, position.y));
  auto lower =
      upper_bound(individuals_.begin(), individuals_.end(), target, CompareX);
  target.SetPosition(vec2(position.x + radius, position.y));
  auto upper = upper_bound(lower, individuals_.end(), target, CompareX);

  for (; lower != upper; ++lower) {
    if (distance(lower->GetPosition(), position) < radius) {
      individuals.push_back(*lower);
      to_remove.push_back(lower - individuals_.begin());
    }
  }

  for (size_t i : to_remove) {
    // TODO in this case maybe save the steps instead of abs index
    individuals_.erase(individuals_.begin() + i);
  }

  return individuals;
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