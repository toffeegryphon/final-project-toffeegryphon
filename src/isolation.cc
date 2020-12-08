#include <configuration.h>
#include <isolation.h>

namespace epidemic {

using std::any_of;

// Constructors

Isolation::Isolation(const vec2& bounds)
    : Isolation(bounds, cfg::kIsolationCapacity.value) {
}

Isolation::Isolation(const vec2& bounds, size_t capacity)
    : Location(Location::Type::kIsolation, bounds) {
  capacity_ = capacity;
}

// Interaction

vector<Individual*> epidemic::Isolation::Add(
    const vector<Individual*>& individuals) {
  // TODO Only pass required number of individuals
  vector<Individual*> rejected;

  size_t total = individuals_.size() + individuals.size();
  if (total > capacity_) {
    size_t limit = individuals.size() - total + capacity_;
    Location::Add(
        vector<Individual*>(individuals.begin(), individuals.begin() + limit));
    rejected =
        vector<Individual*>(individuals.begin() + limit, individuals.end());
  } else {
    rejected = Location::Add(individuals);
  }
  UpdateAdmission();
  return rejected;
}

vector<Individual*> epidemic::Isolation::ExtractIndividualsAt(
    const vec2& position) {
  vector<Individual*> individuals = Location::ExtractIndividualsAt(position);
  Discharge(&individuals);
  return individuals;
}

// Lifecycle

void epidemic::Isolation::Update() {
  for (Individual* individual : individuals_) {
    individual->Update(bounds_, type_);

    if (individual->GetStatus() == Individual::Status::kAsymptomatic) {
      // C++ does not support get_or_default
      auto it = frames_warded_.find(individual->GetID());
      if (it != frames_warded_.end()) {
        if (it->second >= cfg::kIsolationDetectionFrames.value) {
          individual->SetStatus(Individual::Status::kSymptomatic);
        } else {
          ++it->second;
        }
      } else {
        // Start at 1, missed 1
        frames_warded_[individual->GetID()] = 2;
      }
    }
  }

  if (cfg::kIsolationWillSpread.value) {
    frame_ = (frame_ + 1) % cfg::kSpreadCheckFrequency.value;
    if (frame_ == 0) {
      UpdateSpread();
    }
    // UpdateSpread();
  }
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

const vector<Individual*>& epidemic::Isolation::GetIndividuals() const {
  return Location::GetIndividuals();
}

void Isolation::UpdateAdmission() {
  for (size_t i = 0; i < individuals_.size(); ++i) {
    queue<vec2> destinations;
    destinations.push(vec2(bounds_.x * (i + 0.5) / capacity_, bounds_.y / 2));
    Individual* individual = individuals_[i];
    individual->SetDestinations(destinations);
    individual->SetRouteMode(Route::Mode::kDeplete);

    // https://stackoverflow.com/questions/6952486/recommended-way-to-insert-elements-into-map
    if (individual->GetStatus() == Individual::Status::kAsymptomatic) {
      frames_warded_.insert(pair<size_t, int>(individual->GetID(), 1));
    }
  }
}

void Isolation::Discharge(vector<Individual*>* individuals) {
  // TODO Automated discharge as powerup maybe
  for (Individual* individual : *individuals) {
    individual->SetDestinations(queue<vec2>());
    individual->SetRouteMode(Route::Mode::kContinuous);
  }
}

void Isolation::UpdateSpread() {
  // TODO Consider whether spreading to all is good idea
  if (any_of(individuals_.begin(), individuals_.end(),
             [](const Individual* i) { return i->IsSneezing(); })) {
    for (Individual* individual : individuals_) {
      individual->CheckAndBecomeInfected(*individual);
    }
  }
}

}  // namespace epidemic