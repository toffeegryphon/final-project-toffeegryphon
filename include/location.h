#pragma once

#include <cinder/gl/gl.h>
#include <individual.h>

namespace epidemic {

using epidemic::Individual;
using glm::vec2;
using std::vector;

class Location {
 public:
  // Constructors
  explicit Location(const vec2& bounds);

  // Lifecycle
  virtual void Update();
  virtual void Draw(const vec2& offset) const;

  // Getters & Setters
  virtual const vec2& GetBounds() const;
  virtual const vector<Individual>& GetIndividuals() const;

 protected:
  vec2 bounds_;
  vector<Individual> individuals_;
};

}  // namespace epidemic
