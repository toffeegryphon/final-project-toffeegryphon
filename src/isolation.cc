#include <configuration.h>
#include <isolation.h>

namespace epidemic {

using std::any_of;

// Constructors

Isolation::Isolation(const vec2& bounds)
    : Isolation(bounds, Configuration::kIsolationCapacity.value) {
}

Isolation::Isolation(const vec2& bounds, size_t capacity)
    : Location(Location::Type::kIsolation, bounds) {
  capacity_ = capacity;
}

// Interaction

vector<Individual*> epidemic::Isolation::Add(
    const vector<Individual*>& individuals) {
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
  // TODO Update chances
  // TODO Spread check
  for (Individual* individual : individuals_) {
    individual->Update(bounds_, type_);
  }

  // TODO Testing if after x frames and is asymptomatic set to symptomatic
  // TODO PRIORITY 1 IMPLEMENTATION
  if (Configuration::kIsolationWillSpread.value) {
    UpdateSpread();
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
  }
}

void Isolation::Discharge(vector<Individual*>* individuals) {
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