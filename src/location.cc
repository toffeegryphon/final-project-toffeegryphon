#include <configuration.h>
#include <location.h>
#include <utils.h>

namespace epidemic {

using cfg::kDefaultIndividualRadius;

using ci::Color;
using ci::Rectf;
using ci::gl::color;
using ci::gl::drawSolidRect;
using utils::CompareX;

// Constructors

epidemic::Location::Location(Type type, const vec2& bounds)
    : type_(type), bounds_(bounds) {
}

// Interaction

vector<Individual*> Location::Add(const vector<Individual*>& individuals) {
  individuals_.insert(individuals_.end(), individuals.begin(),
                      individuals.end());
  return vector<Individual*>();
}

// Interaction
vector<Individual*> Location::ExtractIndividualsAt(const vec2& position) {
  float radius = kDefaultIndividualRadius;

  vector<size_t> to_remove;
  vector<Individual*> individuals;

  Individual target((vec2()));
  target.SetPosition(vec2(position.x - radius, position.y));

  sort(individuals_.begin(), individuals_.end(), CompareX);
  auto lower =
      upper_bound(individuals_.begin(), individuals_.end(), &target, CompareX);
  target.SetPosition(vec2(position.x + radius, position.y));
  auto upper = upper_bound(lower, individuals_.end(), &target, CompareX);

  for (; lower != upper; ++lower) {
    if (distance((*lower)->GetPosition(), position) < radius) {
      individuals.push_back(*lower);
      to_remove.push_back(lower - individuals_.begin());
    }
  }

  // Already sorted
  size_t r_count = 0;
  for (size_t i : to_remove) {
    individuals_.erase(individuals_.begin() + (i - r_count++));
  }

  return individuals;
}

// Lifecycle

void Location::Draw(const vec2& offset) const {
  color(Color::gray(0.2f));
  drawSolidRect(Rectf(offset, bounds_ + offset));

  for (const Individual* individual : individuals_) {
    individual->Draw(offset);
  }
}

// Getters & Setters

Location::Type Location::GetType() const {
  return type_;
}

const vec2& Location::GetBounds() const {
  return bounds_;
}

const vector<Individual*>& Location::GetIndividuals() const {
  return individuals_;
}

}  // namespace epidemic
