#include <hand.h>

namespace epidemic {

using std::move;

void Hand::Add(const Individual& individual) {
  individuals_.push_back(individual);
}

void Hand::Add(const vector<Individual>& individuals) {
  individuals_.insert(individuals_.end(), individuals.begin(),
                      individuals.end());
}

vector<Individual> Hand::Release() {
  vector<Individual> individuals(move(individuals_));
  individuals_.clear();
  return individuals;
}

// Lifecycle
void Hand::Update(const vec2& position) {
  for (Individual& individual : individuals_) {
    individual.SetPosition(position);
  }
}

void Hand::Draw() {
  vec2 zero;
  for (Individual& individual : individuals_) {
    // TODO maybe instead can let indiv have a draw func that takes in position
    // to draw at
    // TODO also spread out, do not cluster
    individual.Draw(zero);
  }
}

// Getters & Setters
const vector<Individual>& Hand::GetIndividuals() const {
  return individuals_;
}

}  // namespace epidemic